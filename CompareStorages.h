#pragma once
#include "StorageNode.h"
#include "Maze.h"

class CompareStorages
{
public:
	bool operator( ) (const StorageNode* n1, const StorageNode* n2);
};

