#pragma once

class Scorekeeper
{
public:
	enum Score
	{
		Other,
		Soul,
		Both,
	};

	Scorekeeper() = default;

	void ChangeScore(const Score score, const int amount);

	const int GetScore(const Score score) const;

	void SetScore(const Score score, const int value);

private:
	int otherScore_{ 0 };
	int soulScore_{ 0 };
};

