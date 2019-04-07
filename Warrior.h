#pragma once

#include "Point2D.h"
#include "Room.h"
#include "Node.h"
#include <queue>
#include <vector>
#include <stack>
#include <iostream>
#include "CompareNodes.h"
#include "CompareActions.h"
#include "Parent.h"
#include "Action.h"
#include "Maze.h"
#include <math.h>
#include "Constants.h"

class Maze;
class Point2D;
using namespace std;

class Warrior
{
private:
	
	static int			idGen;

	int					id;
	int					gunsAmmo;
	int					grenadeAmmo; 
	int					interval;
	int					shootingSkills;
	bool				life = true;
	double				coward;
	double				safetyScore;
	double				lifePoint; // range [0,100]

	Maze				*maze;
	Room				*currentRoom;
	Room				*destRoom;
	Action				*currentAction;
	Point2D				&location;

	stack<Point2D>		walkingPath;
	priority_queue<Action*, vector<Action*>, CompareActions> actionQueue;

	void createPath(Warrior &other);
	void updateActions();
	void clearActions();
	void clearPath();
	void updateCurrentRoom();
	void moveWarrior(const Point2D &nextStep);
	void exitRoom(const Room &dest);
	void lookForEnemy(Warrior &other);
	void lookForEnemyInRoom(Warrior &other);
	void lookForStorage(const Storage &s, bool ammo);
	void shoot(Warrior &other);
	void takeAmmoStorage(Storage &s, double neededAmount);
	void takeMedStorage(Storage &s, double neededAmount);
	double getDistance(const Point2D &p1, const Point2D &p2) const;
	double getDistance(const Warrior &other) const;
	bool canShoot(const Warrior &other, int maxDist) const;
	bool isInRoom() const;
	
public:
	static const int	MAX_SAFTY_SCORE = 9;

	Warrior(Room &room, Point2D &location, int lifePoint, int gunsAmmo, int grenadeAmmo, int interval, int shootingSkiils, double coward);
	~Warrior();
	Warrior(Warrior&) = delete;	
	Warrior operator=(Warrior&) = delete;

	void makeMove(Warrior& other);
	void injured(double damage);

	// getters
	int	getGunsAmmo() const { return gunsAmmo; }
	int	getGrenadeAmmo() const { return grenadeAmmo; }
	double getlifePoints() const { return lifePoint; }
	double getMaxLife() const { return Constants::MAX_LIFE; }
	double getMaxGuns() const { return Constants::MAX_GUNS_AMMO; }
	double getMaxGrandes() const { return Constants::MAX_GRANDE_AMMO; }
	double getCoward() const { return coward; }
	bool isAlive() const { return life; }
	Room &getCurrentRoom() const { return *currentRoom; }
	Room &getDestRoom() const { return *destRoom; }
	Point2D getLocation() const { return location; }

	// setters
	void setX(int x) { location.setX(x); }
	void setY(int y) { location.setY(y); }
};

