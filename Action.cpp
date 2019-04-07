#include "Action.h"
#include "Warrior.h"

Action::Action(Warrior& warrior, eType type): warrior(warrior), type(type)
{
	updateScore();
}

void Action::updateScore()
{
	double val = calcScore(type, warrior);
	if (val >= 0 && val <= 100)
		score = val;
}
double Action::calcScore(Action::eType type, const Warrior & warrior)
{
	switch (type)
	{
	case Action::FIND_AMMO:
		return calcScoreFindAmmo(warrior);
		break;
	case Action::FIND_MED:
		return calcScoreFindMed(warrior);
		break;
	case Action::FIGHT:
		return calcScoreFight(warrior);
		break;
	}
	return 0;
}

double Action::calcScoreFindAmmo(const Warrior & w)
{
	double neededAmmo = w.getMaxGuns() - w.getGunsAmmo();
	double neededGrande = w.getMaxGrandes() - w.getGrenadeAmmo();
	double maxAmount = w.getMaxGuns() + w.getMaxGrandes();
	return ((neededAmmo + neededGrande) / maxAmount) * MAX_SCORE * w.getCoward();
}

double Action::calcScoreFindMed(const Warrior & w)
{
	double maxLife = w.getMaxLife(), life = w.getlifePoints();
	return (maxLife - life) / maxLife * MAX_SCORE;
}

double Action::calcScoreFight(const Warrior & w)
{
	return MAX_SCORE - calcScoreFindAmmo(w) ;
}
