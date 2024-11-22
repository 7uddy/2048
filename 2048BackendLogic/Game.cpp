#include "Game.h"

game::Game::Game(unsigned int sizeOfBoard) : m_board{sizeOfBoard}
{
    for (int i = 0; i < 3; i++)
    {
        auto position = m_board.GetRandomEmptyPosition();
        m_board.PlacePiece(position, std::make_shared<Piece>());
    }
}

void game::Game::Move(Movement direction)
{
    unsigned int startIndex{}, endIndex{};
    int increaseFactorForStart{};

    int m_size{ m_board.GetBoardSize() };

    switch (direction)
    {
    case Movement::DOWN:
        startIndex = m_size * (m_size - 1);
        endIndex = 0u;
        increaseFactorForStart = -(int)m_size;

        break;
    case Movement::UP:
        startIndex = 0u;
        endIndex = m_size * (m_size - 1);
        increaseFactorForStart = m_size;

        break;
    case Movement::RIGHT:
        startIndex = m_size - 1u;
        endIndex = 0u;
        increaseFactorForStart = -1;

        break;
    case Movement::LEFT:
        startIndex = 0u;
        endIndex = m_size - 1u;
        increaseFactorForStart = 1;

        break;
    default:
        std::cerr << "Invalid move.";

        break;
    }

    //Process each line or column based on direction
    for (int index{ 0 }; index < (int)m_size; index++)
    {
        //std::cout << "\n" << startIndex << " to " << endIndex << "\n";
        MoveRowOrColumnWithData(startIndex, endIndex, increaseFactorForStart);

        //Move to the next line
        if (direction == Movement::LEFT || direction == Movement::RIGHT)
        {
            startIndex += m_size;
            endIndex += m_size;
        }
        //Move to the next column
        else
        {
            startIndex += 1u;
            endIndex += 1u;
        }
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

void game::Game::AddListener(IGameListenerPtr observer)
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

void game::Game::MoveRowOrColumnWithData(unsigned int startIndex, unsigned int endIndex, int increaseFactorForStart)
{
    int m_size{ m_board.GetBoardSize() };
    auto GetEarliestEmptyPosition = [&]() {
        if (startIndex < endIndex)
        {
            for (int currentIndex{ (int)startIndex }; currentIndex <= (int)endIndex; currentIndex += increaseFactorForStart)
            {
                if (!m_board.GetPieceAtPosition(Position::FromIndexToPosition(m_size, currentIndex)).get())
                {
                    return currentIndex;
                }
            }
        }
        else
        {
            for (int currentIndex{ (int)startIndex }; currentIndex >= (int)endIndex; currentIndex += increaseFactorForStart)
            {
                if (!m_board.GetPieceAtPosition(Position::FromIndexToPosition(m_size, currentIndex)).get())
                {
                    return currentIndex;
                }
            }
        }
        return NULL;
        };


    int indexOfEarliestEmptyPosition{ GetEarliestEmptyPosition() };

    if (startIndex < endIndex)
    {
        for (int currentIndex{ (int)startIndex }; currentIndex <= (int)endIndex; currentIndex += increaseFactorForStart)
        {
            //Check if current position is empty.
            if (!m_board.GetPieceAtPosition(Position::FromIndexToPosition(m_size, currentIndex)).get())
            {
                /*  // This can be completely skipped if using GetEarliestEmptyPosition.
                if (indexOfEarliestEmptyPosition == INT_MAX)
                    indexOfEarliestEmptyPosition = currentIndex;
                */
                continue;
            }

            //Means that there is a piece at current position.


            //If there exists an empty position, move the current piece to it.
            if (indexOfEarliestEmptyPosition != NULL)
            {
                m_board.SwapPiecesAtPositions(Position::FromIndexToPosition(m_size, indexOfEarliestEmptyPosition), Position::FromIndexToPosition(m_size, currentIndex));

                //Check if there exists a previosu position
                auto previousPosition{ indexOfEarliestEmptyPosition - increaseFactorForStart };
                if (previousPosition < startIndex || previousPosition > endIndex)
                {
                    //Update empty index
                    indexOfEarliestEmptyPosition = GetEarliestEmptyPosition();
                    continue;
                }
                else
                {
                    //Check if they can combine
                    auto pieceAtPreviousLocation = m_board.GetPieceAtPosition(Position::FromIndexToPosition(m_size, previousPosition));
                    auto pieceAtCurrentLocation = m_board.GetPieceAtPosition(Position::FromIndexToPosition(m_size, indexOfEarliestEmptyPosition));
                    if (pieceAtPreviousLocation.get() && pieceAtPreviousLocation->CanCombineWith(pieceAtCurrentLocation))
                    {
                        auto newPiece = pieceAtPreviousLocation->CombinePieces(pieceAtCurrentLocation);
                        m_board.ErasePiece(Position::FromIndexToPosition(m_size, indexOfEarliestEmptyPosition));
                        //m_board.ErasePiece(Position::FromIndexToPosition(m_size, previousPosition));
                        m_board.PlacePiece(Position::FromIndexToPosition(m_size, previousPosition), newPiece);
                    }
                    else
                    {
                        //Update empty index
                        indexOfEarliestEmptyPosition = GetEarliestEmptyPosition();
                    }
                }
            }

            //Check if the current piece can be combined with the previous piece
            auto previousPosition{ currentIndex - increaseFactorForStart };
            if (previousPosition < startIndex || previousPosition > endIndex)
            {
                //No piece was before this one, so no combination.
                continue;
            }

            //Check if the pieces can combine
            auto pieceAtPreviousLocation = m_board.GetPieceAtPosition(Position::FromIndexToPosition(m_size, previousPosition));
            auto pieceAtCurrentLocation = m_board.GetPieceAtPosition(Position::FromIndexToPosition(m_size, currentIndex));
            if (pieceAtPreviousLocation.get() && pieceAtPreviousLocation->CanCombineWith(pieceAtCurrentLocation))
            {
                auto newPiece = pieceAtPreviousLocation->CombinePieces(pieceAtCurrentLocation);
                m_board.ErasePiece(Position::FromIndexToPosition(m_size, currentIndex));
                //m_board.ErasePiece(Position::FromIndexToPosition(m_size, previousPosition));
                m_board.PlacePiece(Position::FromIndexToPosition(m_size, previousPosition), newPiece);
                indexOfEarliestEmptyPosition = currentIndex;
            }

        }
    }
    else
    {
        for (int currentIndex{ (int)startIndex }; currentIndex >= (int)endIndex; currentIndex += increaseFactorForStart)
        {
            //Check if current position is empty.
            if (!m_board.GetPieceAtPosition(Position::FromIndexToPosition(m_size, currentIndex)).get())
            {
                /*  // This can be completely skipped if using GetEarliestEmptyPosition.
                if (indexOfEarliestEmptyPosition == INT_MAX)
                    indexOfEarliestEmptyPosition = currentIndex;
                */
                continue;
            }

            //Means that there is a piece at current position.

            //If there exists an empty position, move the current piece to it.
            if (indexOfEarliestEmptyPosition != NULL)
            {
                m_board.SwapPiecesAtPositions(Position::FromIndexToPosition(m_size, indexOfEarliestEmptyPosition), Position::FromIndexToPosition(m_size, currentIndex));

                //Check if there exists a previosu position
                auto previousPosition{ indexOfEarliestEmptyPosition - increaseFactorForStart };
                if (previousPosition < startIndex || previousPosition > endIndex)
                {
                    //Update empty index
                    indexOfEarliestEmptyPosition = GetEarliestEmptyPosition();
                    continue;
                }
                else
                {
                    //Check if they can combine
                    auto pieceAtPreviousLocation = m_board.GetPieceAtPosition(Position::FromIndexToPosition(m_size, previousPosition));
                    auto pieceAtCurrentLocation = m_board.GetPieceAtPosition(Position::FromIndexToPosition(m_size, indexOfEarliestEmptyPosition));
                    if (pieceAtPreviousLocation && pieceAtPreviousLocation->CanCombineWith(pieceAtCurrentLocation))
                    {
                        auto newPiece = pieceAtPreviousLocation->CombinePieces(pieceAtCurrentLocation);
                        m_board.ErasePiece(Position::FromIndexToPosition(m_size, indexOfEarliestEmptyPosition));
                        //m_board.ErasePiece(Position::FromIndexToPosition(m_size, previousPosition));
                        m_board.PlacePiece(Position::FromIndexToPosition(m_size, previousPosition), newPiece);
                    }
                    else
                    {
                        //Update empty index
                        indexOfEarliestEmptyPosition = GetEarliestEmptyPosition();
                    }
                }
            }

            //Check if the current piece can be combined with the previous piece
            auto previousPosition{ currentIndex - increaseFactorForStart };
            if (previousPosition < startIndex || previousPosition > endIndex)
            {
                //No piece was before this one, so no combination.
                continue;
            }

            //Check if the pieces can combine
            auto pieceAtPreviousLocation = m_board.GetPieceAtPosition(Position::FromIndexToPosition(m_size, previousPosition));
            auto pieceAtCurrentLocation = m_board.GetPieceAtPosition(Position::FromIndexToPosition(m_size, currentIndex));
            if (pieceAtPreviousLocation && pieceAtPreviousLocation->CanCombineWith(pieceAtCurrentLocation))
            {
                auto newPiece = pieceAtPreviousLocation->CombinePieces(pieceAtCurrentLocation);
                m_board.ErasePiece(Position::FromIndexToPosition(m_size, currentIndex));
                //m_board.ErasePiece(Position::FromIndexToPosition(m_size, previousPosition));
                m_board.PlacePiece(Position::FromIndexToPosition(m_size, previousPosition), newPiece);
                indexOfEarliestEmptyPosition = currentIndex;
            }

        }
    }

}
