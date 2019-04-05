#pragma once
#include "Point2D.h"
#include "Room.h"
#include <stdlib.h>
#include <time.h>

class Storage
{
private:
	int id;
	Point2D *location; 
	Room *room; 
	int type;
	double amount;

	
public:	
	Storage(int id, Room &containedInRoom, Point2D &location, int type, int amount);
	Storage() {};
	~Storage();
	
	// getters
	Point2D &getLocation() const;
	Room &getRoom() const { return *room; }
	int getType() const;
	double getAmount() const { return amount; }
	int getId() const { return id; }

	void takeStorage(double amount) { this->amount -= amount; }
};

