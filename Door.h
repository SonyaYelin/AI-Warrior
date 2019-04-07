#pragma once
#include "Room.h"
class Door
{
private:
	Room			&current;
	vector<Room*>	destinations;
	Point2D			&enterLocation;
	Point2D			&exitLocation;

public:
	Door(Room& current, Point2D &enterlocation, Point2D &exitlocation);
	~Door();

	void addDestination(Room& r) { destinations.push_back(&r); }

	bool isDestinationDoor(const Room& destination) const;
	vector<Room*> getDestinations() const { return destinations; }
	Point2D& getEnterLocation() const { return enterLocation; }
	Point2D& getExitLocation() const { return exitLocation; }

};

