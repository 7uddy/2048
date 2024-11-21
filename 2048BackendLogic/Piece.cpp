#include "Piece.h"

using namespace game;

Piece::Piece(int value) : m_value{ value }
{
    /*EMPTY*/
}

int Piece::GetValue() const
{
    return m_value;
}

void Piece::SetValue(int value)
{
    m_value = value;
}

bool Piece::CanCombineWith(std::shared_ptr<IPiece> piece2) const
{
	if (!piece2.get())
		return false;
	return this->GetValue() == piece2->GetValue();
}

std::shared_ptr<IPiece> Piece::CombinePieces(std::shared_ptr<IPiece> piece2) const
{
	if (!CanCombineWith(piece2))
	{
		return nullptr;
	}

	int newValue = this->GetValue() + piece2->GetValue();
	return std::make_shared<Piece>(newValue);
}
