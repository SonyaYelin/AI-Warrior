#include "CompareActions.h"
#include "Warrior.h"

bool CompareActions::operator( )(Action* a1, Action* a2)
{
	return a1->getScore() < a2->getScore();
}