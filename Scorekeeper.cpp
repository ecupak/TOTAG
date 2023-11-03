#include "precomp.h"
#include "Scorekeeper.h"


void Scorekeeper::ChangeScore(const Score score, const int amount)
{
	if (score == Score::Other)
	{
		otherScore_ += amount;
	}
	else
	{
		soulScore_ += amount;
	}
}


const int Scorekeeper::GetScore(const Score score) const
{
	return score == Score::Both
		? otherScore_ + soulScore_
		: score == Score::Other
			? otherScore_
			: soulScore_;
}


void Scorekeeper::SetScore(const Score score, const int value)
{
	if (score == Score::Other)
	{
		otherScore_ = value;
	}
	else
	{
		soulScore_ = value;
	}
}