#pragma once
#include <fstream>
#include "Room.h"
#include "Door.h"
#include "Storage.h"
#include "MazePart.h"
#include <queue>
#include <vector>
#include <stack>
#include <iostream>
#include "CompareActions.h"
#include "Parent.h"
#include "Node.h"
#include "StorageNode.h"
#include <math.h>
#include "Constants.h"

using namespace std;

class Warrior;
class CompareNodes;
class CompareNodesRun;

class Maze
{
public:
	static const int	UP = 1;
	static const int	DOWN = 2;
	static const int	LEFT = 3;
	static const int	RIGHT = 4;

	MazePart			parts[Constants::MSIZE][Constants::MSIZE];

private:
	static Maze			*maze;
	Room				rooms[Constants::NUM_ROOMS];
	vector<Storage*>	ammoStorage;
	vector<Storage*>	medicalStorage;

	Maze();
	int countSpaces(int i, int j);
	void loadMazeFromFile();
	void createStorages();
	void drawStorage(const Storage &s);

	bool AddNewNode(Node & current, Point2D & targetLocation, vector<Point2D>& gray,
		vector<Point2D>& black, vector<Parent>& parents, priority_queue<Node*, vector<Node*>, CompareNodes>& pq, int direction);
public:
	~Maze();
	Maze(Maze &other) = delete;
	Maze& operator=(Maze &other) = delete;
	static Maze& getInstance();

	// getters
	Room* getRooms() const;
	double getSaftyScore(Point2D &point) const;
	Storage &getTargetStorage(int type, Point2D &currentLocation, Point2D &enamy) const;
	Storage* getIfStorage(const Point2D &p, int type) const;

	// setters
	void setSaftyScores();

	// actions
	void removeStorage(Storage &s, int type);
	stack<Point2D> aStar(Point2D &currentLocation, Point2D &targetLocation);
};
