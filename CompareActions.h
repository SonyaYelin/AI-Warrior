#pragma once
#include "Action.h"
#include "Maze.h"

class Warrior;

class CompareActions
{

public:
	bool operator( ) (Action* a1, Action* a2);
};

