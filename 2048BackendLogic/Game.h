#pragma once
#include <iostream>
#include <fstream>
#include <stack>
#include "IGame.h"
#include "Board.h"

namespace game
{
	class Game : public IGame
	{
	public:
		const static std::string DefaultPathToBoardFile;
		const static unsigned int DefaultBoardSize{ 3u };
		const static unsigned int DefaultNumberOfUsesForSpecialAbilities{ 2u };
		const static int DefaultPieceValueThatIncreasesSpecialMovesUses{ 256 };

	public:
		Game(unsigned int sizeOfBoard);

		void ApplyMove(Movement move) override;
		void ApplySwitchTiles(Position position1, Position position2) override;
		void ApplyUndo() override;

		bool IsGameOver() override;

		void ResetGame() override;

		unsigned int GetScore() const override;
		unsigned int GetMaxScore() const override;

		void SetPathToBoardFile(const std::string& newPath) override;
		std::string GetPathToBoardFile() const override;

		void SetBoard(const std::string& board) override;
		std::string GetBoard() const override;

		void AddListener(std::shared_ptr<IGameListener> observer) override;
		void RemoveListener(IGameListener* observer) override;

		void ReadGameStateFromFile() override;
		void SaveGameStateInFile() override;
	private:
		MoveResult SquashColumn(unsigned int columIndex) override;
		void InitializeRandomPieces() override;
		void PlacePieceAtRandomPosition() override;
		void UpdateMaxScore() override;
		void UpdateSpecialMovesOnInterface() override;

		bool ApplyMoveUtil(Movement position) override;

		void NotifyListenersForMoveDone() const override;
		void NotifyListenersForGameOver() const override;
		void NotifyListenersForGameReset() const override;
		void NotifyListenersForEnableUndoMove(bool isEnabled) const override;
		void NotifyListenersForEnableSwapMove(bool isEnabled) const override;
		

	private:
		std::unique_ptr<IBoard> m_board;
		std::vector<std::weak_ptr<IGameListener>> m_observers;
		std::string m_pathToFileWithBoard;

		GameState m_gameState;
		std::stack<GameState> m_previousGameStates;
	};
}
