#pragma once

#include "Point2D.h"
#include <iostream>

class Warrior;
using namespace std;

class Action
{
public:
	enum eType { FIND_AMMO, FIND_MED, FIGHT };
	
	Action(Warrior& warrior, eType type);
	Action &operator=(const Action & other) = delete;

	double getScore() const { return score; }
	Warrior &getWarrior() const { return warrior; }
	eType getType() const { return type; }
	void updateScore();

private:
	const double MAX_SCORE = 100;

	Warrior& warrior;
	double score;
	eType type;

	double calcScore(Action::eType type, const Warrior &warrior);
	double calcScoreFindAmmo(const Warrior& w);
	double calcScoreFindMed(const Warrior& w);
	double calcScoreFight(const Warrior& w);
};