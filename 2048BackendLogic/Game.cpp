#include "Game.h"

const std::string game::Game::DefaultPathToBoardFile{ "board.txt" };

game::Game::Game(unsigned int sizeOfBoard) : m_board{ std::make_unique<Board>(sizeOfBoard) }, m_gameState{}, m_pathToFileWithBoard { DefaultPathToBoardFile }
{
    InitializeRandomPieces();
}

void game::Game::ApplyMove(Movement direction)
{
    GameState possiblePreviousState{ m_gameState };

    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board->FlipDiagonally();

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board->FlipVertically();

    bool modificationWasMade{ false };
    for (unsigned int index{ 0 }; index < m_board->GetBoardSize(); ++index)
    {
        MoveResult result = SquashColumn(index);
        modificationWasMade = modificationWasMade || result.modificationWasMade;
        m_gameState.score += result.scoreGained;
    }

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board->FlipVertically();

    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board->FlipDiagonally();

    if (modificationWasMade) 
    {
        UpdateMaxScore();
        m_previousGameStates.push(possiblePreviousState);
        PlacePieceAtRandomPosition();
    }
    
    if (IsGameOver())
    {
        NotifyListenersForGameOver();
    }
    else
    {
        NotifyListenersForMoveDone();
    }

    UpdateSpecialMovesOnInterface();
}

void game::Game::ApplySwitchTiles(Position position1, Position position2)
{
    if (!m_gameState.timesLeftToUseSwap)
        return;

    m_previousGameStates.push(m_gameState);
    m_board->SwapPiecesAtPositions(position1, position2);
    m_gameState.timesLeftToUseSwap--;
    m_gameState.board = m_board->GetBoard();

    NotifyListenersForEnableSwapMove(false);
    NotifyListenersForMoveDone();
}

void game::Game::ApplyUndo()
{
    if (m_previousGameStates.empty() || !m_gameState.timesLeftToUseUndo)
        return;

    GameState toBeCurrentState = m_previousGameStates.top();
    m_previousGameStates.pop();
    
    unsigned int currentTimeLeftToUseUndo{ m_gameState.timesLeftToUseUndo };
    m_gameState = toBeCurrentState;
    m_board->SetBoard(toBeCurrentState.board);
    m_gameState.timesLeftToUseUndo = --currentTimeLeftToUseUndo;

    NotifyListenersForEnableUndoMove(false);
    NotifyListenersForMoveDone();
}

void game::Game::SetPathToBoardFile(const std::string& newPath)
{
    m_pathToFileWithBoard = newPath;
}

std::string game::Game::GetPathToBoardFile() const
{
    return m_pathToFileWithBoard;
}

void game::Game::SetBoard(const std::string& board)
{
	m_board->SetBoard(board);
}

std::string game::Game::GetBoard() const
{
	return m_gameState.board;
}

void game::Game::AddListener(std::shared_ptr<IGameListener> observer)
{
    m_observers.emplace_back(observer);
}

void game::Game::RemoveListener(IGameListener* observer)
{
	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
        if (auto sp = it->lock())
        {
            if (sp.get() == observer)
                    it = m_observers.erase(it);
                else
                    it++;
        }
        else
            it = m_observers.erase(it);
	}
}

void game::Game::NotifyListenersForMoveDone() const
{
    for (auto& listener : m_observers)
    {
        if (auto sp = listener.lock())
        {
            sp->OnMoveDone();
        }
    }
}

void game::Game::NotifyListenersForGameOver() const
{
    for (auto& listener : m_observers)
    {
        if (auto sp = listener.lock())
        {
            sp->OnGameOver();
        }
    }
}

void game::Game::NotifyListenersForGameReset() const
{
    for (auto& listener : m_observers)
    {
        if (auto sp = listener.lock())
        {
            sp->OnGameReset();
        }
    }
}

void game::Game::NotifyListenersForEnableUndoMove(bool isEnabled) const
{
    for (auto& listener : m_observers)
    {
        if (auto sp = listener.lock())
        {
            sp->OnEnableUndoMove(isEnabled);
        }
    }
}

void game::Game::NotifyListenersForEnableSwapMove(bool isEnabled) const
{
    for (auto& listener : m_observers)
    {
        if (auto sp = listener.lock())
        {
            sp->OnEnableSwapTileMove(isEnabled);
        }
    }
}

game::GameState game::Game::GetGameState() const
{
    return m_gameState;
}

void game::Game::ReadGameStateFromFile()
{
    std::ifstream file{ m_pathToFileWithBoard };
    if (!file.is_open())
    {
        throw std::runtime_error("Nu s-a putut deschide fisierul pentru citire: " + m_pathToFileWithBoard);
    }

    unsigned int lineNumber{ 0u };
    std::string readBoard, line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        if (lineNumber == 0)
        {
            std::istringstream scoreStream(line);
            if (!(scoreStream >> m_gameState.score >> m_gameState.maxScore >> m_gameState.timesLeftToUseUndo    ))
            {
                file.close();
                throw std::runtime_error("Eroare la citirea scorurilor din fisier: " + m_pathToFileWithBoard);
            }
        }
        else
        {
            readBoard += line + "\n";
        }

        lineNumber++;
    }

    if (lineNumber == 0)
    {
        file.close();
        throw std::runtime_error("Fisierul este gol sau nu contine date valide: " + m_pathToFileWithBoard);
    }

    try 
    {
        m_board->SetBoard(readBoard);
        m_gameState.board = readBoard;
    }
    catch (...)
    {
        ResetGame();
    }

    file.close();
}


void game::Game::SaveGameStateInFile()
{

    std::ofstream file{ m_pathToFileWithBoard };
    if (!file.is_open())
    {
        throw std::runtime_error("Nu s-a putut deschide fisierul pentru scriere: " + m_pathToFileWithBoard);
    }

    file << "#    This file contains the game data of a particular instance of a game.\n";
    file << "#    All lines marked with # at the beginning are considered comments.\n";
    file << "#    Data related information:\n";
    file << "#      -on the first line, the first integer represents the current score;\n";
    file << "#      -on the first line, the second integer represents the maximum acquired score;\n";
    file << "#      -on the following lines, the numbers represent the game board pieces.\n";
    file << m_gameState.score << " " << m_gameState.maxScore << " " << m_gameState.timesLeftToUseUndo << "\n";
    file << m_gameState.board;

    file.close();
}

unsigned int game::Game::GetMaxScore() const
{
    return m_gameState.maxScore;
}

void game::Game::UpdateMaxScore()
{
    m_gameState.maxScore = std::max(m_gameState.maxScore, m_gameState.score);
}

void game::Game::UpdateSpecialMovesOnInterface()
{
    if (m_gameState.timesLeftToUseSwap && m_board->GetNumberOfPiecesOnBoard() > 2u)
        NotifyListenersForEnableSwapMove(true);
    else
        NotifyListenersForEnableSwapMove(false);

    if (m_gameState.timesLeftToUseUndo && !m_previousGameStates.empty())
        NotifyListenersForEnableUndoMove(false);
    else
        NotifyListenersForEnableUndoMove(true);
}


void game::Game::ResetGame()
{
    
    m_previousGameStates = std::stack<GameState>();
    m_board = std::make_unique<Board>(m_board->GetBoardSize());
    m_gameState = GameState{};
    InitializeRandomPieces();

    UpdateSpecialMovesOnInterface();
    NotifyListenersForGameReset();
}

unsigned int game::Game::GetScore() const
{
    return m_gameState.score;
}

unsigned int game::Game::GetTimesLeftToUseUndo() const
{
    return m_gameState.timesLeftToUseUndo;
}

game::MoveResult game::Game::SquashColumn(unsigned int columnIndex)
{
    MoveResult result{ 0u, false };
    for (int destinationIndex{ (int)m_board->GetBoardSize() - 1 }; destinationIndex >= 0; --destinationIndex)
    {
        int sourceIndex{ destinationIndex - 1 };
        while (sourceIndex >= 0 && !m_board->GetPieceAtPosition(Position{ (unsigned int) sourceIndex, columnIndex }))
            --sourceIndex;

        if (sourceIndex < 0)
            break;

        std::shared_ptr<IPiece> destinationPiece = m_board->GetPieceAtPosition(Position{ (unsigned int) destinationIndex, columnIndex });
        std::shared_ptr<IPiece> sourcePiece = m_board->GetPieceAtPosition(Position{ (unsigned int) sourceIndex, columnIndex });
        if (destinationPiece && sourcePiece && destinationPiece->CanCombineWith(sourcePiece))
        {
            std::shared_ptr<IPiece> newPiece = sourcePiece->CombinePieces(destinationPiece);
            m_board->PlacePiece(Position{ (unsigned int)destinationIndex, columnIndex }, newPiece);
            m_board->ErasePiece(Position{ (unsigned int)sourceIndex, columnIndex });
            result.scoreGained += newPiece->GetValue();
            result.modificationWasMade = true;

            //Dynamically add chances to use special moves.
            if (newPiece->GetValue() == DefaultPieceValueThatIncreasesSpecialMovesUses)
            {
                m_gameState.timesLeftToUseSwap = std::max(m_gameState.timesLeftToUseSwap + 1u, DefaultNumberOfUsesForSpecialAbilities);
            }

            if (newPiece->GetValue() == DefaultPieceValueThatIncreasesSpecialMovesUses)
            {
                m_gameState.timesLeftToUseUndo = std::max(m_gameState.timesLeftToUseUndo + 1u, DefaultNumberOfUsesForSpecialAbilities);
            }
        }

        if (!m_board->GetPieceAtPosition(Position{ (unsigned int) destinationIndex, columnIndex }))
        {
            m_board->SwapPiecesAtPositions(Position{ (unsigned int) destinationIndex, columnIndex }, Position{ (unsigned int)sourceIndex, columnIndex });
            ++destinationIndex;
            result.modificationWasMade = true;
        }
    }
    return result;
}

void game::Game::InitializeRandomPieces()
{
    while (m_board->GetNumberOfPiecesOnBoard() < 2u)
    {
        PlacePieceAtRandomPosition();
    }
}

void game::Game::PlacePieceAtRandomPosition()
{
    if (m_board->IsBoardFull())
        return;

    Position position{ m_board->GetRandomEmptyPosition() };
    while (m_board->GetPieceAtPosition(position))
    {
        position = m_board->GetRandomEmptyPosition();
    }
    m_board->PlacePiece(position, std::make_shared<Piece>());

    m_gameState.board = m_board->GetBoard();
}

bool game::Game::IsGameOver()
{
    std::string currentBoard{ m_board->GetBoard() };

    if (WasModifiedOnMove(Movement::UP) || WasModifiedOnMove(Movement::DOWN) || WasModifiedOnMove(Movement::LEFT) || WasModifiedOnMove(Movement::RIGHT))
    {
        m_board->SetBoard(currentBoard);
        return false;
    }

    return true;
}

bool game::Game::WasModifiedOnMove(Movement direction)
{
    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board->FlipDiagonally();

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board->FlipVertically();

    for (unsigned int index{ 0 }; index < m_board->GetBoardSize(); ++index)
    {
        if (SquashColumn(index).modificationWasMade)
            return true;
    }

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board->FlipVertically();

    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board->FlipDiagonally();

    return false;
}
