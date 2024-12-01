#pragma once
#include <string>
#include "IBoard.h"
#include "IGameListener.h"

namespace game
{
	enum class Movement
	{
		UP = 0,
		DOWN,
		LEFT,
		RIGHT
	};

	struct MoveResult
	{
		unsigned int scoreGained;
		bool modificationWasMade;

		MoveResult(unsigned int scoreGained, bool modificationWasMade) {
			this->scoreGained = scoreGained;
			this->modificationWasMade = modificationWasMade;
		}
	};

	struct GameState
	{
		unsigned int score;
		unsigned int maxScore;
		std::string board; 
		unsigned int timesLeftToUseSwap;
		unsigned int timesLeftToUseUndo;

		GameState() : score{ 0u }, maxScore{ 0u }, board{ "" }, timesLeftToUseSwap{ 2u }, timesLeftToUseUndo{ 2u } {
			/*EMPTY*/
		}

		GameState(unsigned int score, unsigned int maxScore, const std::string& board, unsigned int timesLeftToUseSwap, unsigned int timesLeftToUseUndo) {
			this->score = score;
			this->maxScore = maxScore;
			this->board = board;
			this->timesLeftToUseSwap = timesLeftToUseSwap;
			this->timesLeftToUseUndo = timesLeftToUseUndo;
		}
	};

	class IGame
	{
	public:
		virtual void ApplyMove(Movement position) = 0;
		virtual void ApplySwitchTiles(Position position1, Position position2) = 0;
		virtual void ApplyUndo() = 0;

		virtual bool IsGameOver() = 0;

		virtual void ResetGame() = 0;

		virtual void SetPathToBoardFile(const std::string& newPath) = 0;
		virtual void SetBoard(const std::string& board) = 0;
		virtual std::string GetBoard() const = 0;

		virtual unsigned int GetScore() const = 0;
		virtual unsigned int GetMaxScore() const = 0;
		virtual std::string GetPathToBoardFile() const = 0;

		virtual void AddListener(std::shared_ptr<IGameListener> observer) = 0;
		virtual void RemoveListener(IGameListener* observer) = 0;

		virtual void ReadGameStateFromFile() = 0;
		virtual void SaveGameStateInFile() = 0;

	private:
		virtual MoveResult SquashColumn(unsigned int columIndex) = 0;
		virtual void InitializeRandomPieces() = 0;
		virtual void PlacePieceAtRandomPosition() = 0;
		virtual void UpdateMaxScore() = 0;
		virtual void UpdateSpecialMovesOnInterface() = 0;

		virtual void NotifyListenersForMoveDone() const = 0;
		virtual void NotifyListenersForGameOver() const = 0;
		virtual void NotifyListenersForGameReset() const = 0;
		virtual void NotifyListenersForEnableUndoMove(bool isEnabled) const = 0;
		virtual void NotifyListenersForEnableSwapMove(bool isEnabled) const = 0;

		virtual bool ApplyMoveUtil(Movement position) = 0;
	};
}