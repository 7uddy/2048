#include "Game.h"

const std::string game::Game::DefaultPathToBoardFile{ "board.txt" };


game::Game::Game(unsigned int sizeOfBoard) : m_board{ sizeOfBoard }, m_score{ 0u }, m_maxScore{ 0u }, m_pathToFileWithBoard{ DefaultPathToBoardFile }
{
    InitializeRandomPieces();
}

game::Game::Game(const std::string& pathToFileWithBoard) : m_board{ DefaultBoardSize }, m_score{ 0u }, m_maxScore{ 0u }, m_pathToFileWithBoard{ pathToFileWithBoard }
{
    ReadGameStateFromFile();
}

void game::Game::ApplyMove(Movement direction)
{
    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board.FlipDiagonally();

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board.FlipVertically();

    bool modificationWasMade{ false };
    for (unsigned int index{ 0 }; index < m_board.GetBoardSize(); ++index)
    {
        MoveResult result = m_board.SquashColumn(index);
        modificationWasMade = modificationWasMade || result.modificationWasMade;
        m_score += result.scoreGained;
    }

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board.FlipVertically();

    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board.FlipDiagonally();

    UpdateMaxScore();

    if (modificationWasMade)
        PlacePieceAtRandomPosition();

    NotifyListenersForMoveDone();
    
    if (IsGameOver())
    {
        NotifyListenersForGameOver();
    }
}

void game::Game::SetBoard(const std::string& board)
{
	m_board.SetBoard(board);
}

std::string game::Game::GetBoard() const
{
	return m_board.GetBoard();
}

game::Board game::Game::GetBoardObject() const noexcept
{
	return m_board;
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
            if (!(scoreStream >> m_score >> m_maxScore))
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
        m_board.SetBoard(readBoard);
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

    std::string board{ m_board.GetBoard() };
    file << "#    This file contains the game data of a particular instance of a game.\n";
    file << "#    All lines marked with # at the beginning are considered comments.\n";
    file << "#    Data related information:\n";
    file << "#      -on the first line, the first integer represents the current score;\n";
    file << "#      -on the first line, the second integer represents the maximum acquired score;\n";
    file << "#      -on the following lines, the numbers represent the game board pieces.\n";
    file << m_score << " " << m_maxScore << "\n";
    file << board;

    file.close();
}

unsigned int game::Game::GetMaxScore() const
{
    return m_maxScore;
}

void game::Game::UpdateMaxScore()
{
    m_maxScore = std::max(m_maxScore, m_score);
}


void game::Game::ResetGame()
{
    m_board.ResetBoard();
    InitializeRandomPieces();
    m_score = 0u;
    NotifyListenersForGameReset();
}

unsigned int game::Game::GetScore() const
{
    return m_score;
}

void game::Game::InitializeRandomPieces()
{
    while (m_board.GetNumberOfPiecesOnBoard() < 2u)
    {
        PlacePieceAtRandomPosition();
    }
}

void game::Game::PlacePieceAtRandomPosition()
{
    if (m_board.IsBoardFull())
        return;

    Position position{ m_board.GetRandomEmptyPosition() };
    while (m_board.GetPieceAtPosition(position))
    {
        position = m_board.GetRandomEmptyPosition();
    }
    m_board.PlacePiece(position, std::make_shared<Piece>());
}

bool game::Game::IsGameOver()
{
    std::string currentBoard{ m_board.GetBoard() };

    if (ApplyMoveUtil(Movement::UP) || ApplyMoveUtil(Movement::DOWN) || ApplyMoveUtil(Movement::LEFT) || ApplyMoveUtil(Movement::RIGHT))
    {
        m_board.SetBoard(currentBoard);
        return false;
    }

    return true;
}

bool game::Game::ApplyMoveUtil(Movement direction)
{
    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board.FlipDiagonally();

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board.FlipVertically();

    for (unsigned int index{ 0 }; index < m_board.GetBoardSize(); ++index)
    {
        if (m_board.SquashColumn(index).modificationWasMade)
            return true;
    }

    if (direction == Movement::UP || direction == Movement::LEFT)
        m_board.FlipVertically();

    if (direction == Movement::LEFT || direction == Movement::RIGHT)
        m_board.FlipDiagonally();

    return false;
}
