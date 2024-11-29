#pragma once
#include <iostream>
#include <fstream>
#include "IGame.h"
#include "Board.h"

namespace game
{
	class Game : public IGame
	{
	public:
		const static std::string DefaultPathToBoardFile;
		const static unsigned int DefaultBoardSize{ 3u };

	public:
		Game(unsigned int sizeOfBoard);
		Game(const std::string& pathToFileWithBoard = DefaultPathToBoardFile);

		void ApplyMove(Movement move) override;

		bool IsGameOver() override;

		void ResetGame() override;

		unsigned int GetScore() const override;
		unsigned int GetMaxScore() const override;

		void SetBoard(const std::string& board) override;
		std::string GetBoard() const override;
		Board GetBoardObject() const noexcept;

		void AddListener(std::shared_ptr<IGameListener> observer) override;
		void RemoveListener(IGameListener* observer) override;

		void ReadGameStateFromFile() override;
		void SaveGameStateInFile() override;
	private:
		void InitializeRandomPieces() override;
		void PlacePieceAtRandomPosition() override;
		void UpdateMaxScore() override;

		bool ApplyMoveUtil(Movement position) override;

		void NotifyListenersForMoveDone() const override;
		void NotifyListenersForGameOver() const override;
		void NotifyListenersForGameReset() const override;
		

	private:
		Board m_board;
		std::vector<std::weak_ptr<IGameListener>> m_observers;
		unsigned int m_score;
		unsigned int m_maxScore;
		std::string m_pathToFileWithBoard;
	};
}
