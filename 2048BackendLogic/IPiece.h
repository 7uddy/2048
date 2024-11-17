#pragma once
#include <memory>

namespace game 
{
	class IPiece
	{
	public:
		virtual int GetValue() const = 0;
		virtual void SetValue(int value) = 0;
		virtual bool CanCombineWith(std::shared_ptr<IPiece> piece) const = 0;
		virtual std::shared_ptr<IPiece> CombinePieces(std::shared_ptr<IPiece> piece) const = 0;
	};
}