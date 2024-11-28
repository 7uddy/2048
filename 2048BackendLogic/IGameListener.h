#pragma once

namespace game
{
	class IGameListener
	{
	public:

		//Tells the listener that the board needs to be updated on the interface after move.
		virtual void OnMoveDone() = 0;

		//Tells the listener that the game is over.
		virtual void OnGameOver() = 0;

		//Tells the listener that the game got reset.
		virtual void OnGameReset() = 0;

		virtual ~IGameListener() = default;
	};

	using IGameListenerPtr = std::shared_ptr<IGameListener>;
	using IGameListenerWeakPtr = std::weak_ptr<IGameListener>;
}