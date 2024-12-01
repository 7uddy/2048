#pragma once

namespace game
{
	class IGameListener
	{
	public:

		//Tells the listener that the move has been finished.
		virtual void OnMoveDone() = 0;

		//Tells the listener that the game is over.
		virtual void OnGameOver() = 0;

		//Tells the listener that the game got reset.
		virtual void OnGameReset() = 0;

		//Tells the listener if the undo move is possible or not.
		virtual void OnEnableUndoMove(bool isEnabled) = 0;

		//Tells the listener if the swap tiles move is possible or not.
		virtual void OnEnableSwapTileMove(bool isEnabled) = 0;

		virtual ~IGameListener() = default;
	};

	using IGameListenerPtr = std::shared_ptr<IGameListener>;
	using IGameListenerWeakPtr = std::weak_ptr<IGameListener>;
}