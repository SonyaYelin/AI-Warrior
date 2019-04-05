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

class Maze;
class Point2D;
using namespace std;

class Warrior
{
private:
	static const int	MAX_LIFE = 100;
	static const int	MAX_GUNS_AMMO = 50;
	static const int	MAX_GRANDE_AMMO = 2;
	static const int	THROW_GRENADE_MAX_DISTANCE = 15;
	static const int	SHOOT_DISTANCE = 5;
	static const int	MAX_NUM_OF_BULLETS = 50;
	static const int	MAX_NUM_OF_GRENADES = 2;
	static const int	GRENADE_DEMAGE_RADIOS = 7;

	Maze				*maze;

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

	Room				*currentRoom;
	Room				*destRoom;
	Action				*currentAction;
	Point2D				&location;

	stack<Point2D>		walkingPath;
	priority_queue<Action*, vector<Action*>, CompareActions> actionQueue;


	void exitRoom(Room &room);
	double getDistance(const Point2D &p1, const Point2D &p2) const;
	double getDistance(const Warrior &other) const;
	void lookForEnemyInRoom(Warrior &other);
	void shoot(Warrior &other);
	void injured(double hitPoint);
	void updateActions();
	void clearActions();
	void clearPath();
	void moveWarrior(Point2D &nextStep);
	void lookForStorage(Storage &s, bool ammo);
	void findEnemy(Warrior &other);
	void updateCurrentRoom();
	bool canShoot(Warrior &other, int maxDist);
	bool isInRoom();

public:
	static const int	MAX_SAFTY_SCORE = 9;

	Warrior::Warrior(Room &room, Point2D &location, int lifePoint, int gunsAmmo, int grenadeAmmo, int interval, int shootingSkiils, double coward);
	~Warrior();

	void selectAction(Warrior& other);

	// getters
	int	getGunsAmmo() const { return gunsAmmo; }
	int	getGrenadeAmmo() const { return grenadeAmmo; }
	Room &getCurrentRoom() const { return *currentRoom; }
	Room &getDestRoom() const { return *destRoom; }
	Point2D getLocation() const { return location; }
	double getlifePoints() const { return lifePoint; }
	double getMaxLife() const { return MAX_LIFE; }
	double getMaxGuns() const { return MAX_GUNS_AMMO; }
	double getMaxGrandes() const { return MAX_GRANDE_AMMO; }
	double getCoward() const { return coward; }
	bool isAlive() const { return life; }
	
	// setters
	void setX(int x) { this->location.setX(x); }
	void setY(int y) { this->location.setY(y); }
};

