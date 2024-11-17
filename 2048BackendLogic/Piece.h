#pragma once
#include "IPiece.h"

namespace game
{
	class Piece : public IPiece
	{
	public:
		Piece(int value = 2);

		int GetValue() const override;

		void SetValue(int value) override;

		bool CanCombineWith(std::shared_ptr<IPiece> piece) const override;

		std::shared_ptr<IPiece> CombinePieces(std::shared_ptr<IPiece> piece) const override;

	private:
		int m_value;
	};
}
