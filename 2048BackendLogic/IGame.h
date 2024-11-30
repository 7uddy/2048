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
		virtual void InitializeRandomPieces() = 0;
		virtual void PlacePieceAtRandomPosition() = 0;
		virtual void UpdateMaxScore() = 0;

		virtual void NotifyListenersForMoveDone() const = 0;
		virtual void NotifyListenersForGameOver() const = 0;
		virtual void NotifyListenersForGameReset() const = 0;

		virtual bool ApplyMoveUtil(Movement position) = 0;
	};
}