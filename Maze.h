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

using namespace std;

class Warrior;
class CompareNodes;
class CompareNodesRun;

class Maze
{
public:
	static const int MSIZE = 100;
	static const int NUM_ROOMS = 10;
	static const int STORAGE_NUM = 10;


	//static const int NUM_OF_STORAGES = 2;


	//


	static const int UP = 1;
	static const int DOWN = 2;
	static const int LEFT = 3;
	static const int RIGHT = 4;


	//static const int BULLET = 2;
	//static const int GRENADE = 3;
	//static const int LIFE = 2;

	MazePart parts[MSIZE][MSIZE];

private:
	static Maze* maze;
	Room all_rooms[NUM_ROOMS];
	vector<Storage*> ammoStorage;
	vector<Storage*> medicalStorage;

	Maze();
	void createStorages();
	int countSpaces(int i, int j);
	void loadMazeFromFile();
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
	double getSafty(int x, int y) const;
	Storage* getIfStorage(const Point2D &p, int type) const;

	// setters
	void setSaftyScores();

	// actions
	void removeStorage(Storage &s, int type);
	stack<Point2D> aStar(Point2D &currentLocation, Point2D &targetLocation);
};
