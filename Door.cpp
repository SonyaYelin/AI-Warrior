#include "Door.h"

Door::Door(Room& current, Point2D &enterlocation, Point2D &exitlocation):
	current(current), enterLocation(enterlocation), exitLocation(exitlocation)
{
	current.getBottom();
}


Door::~Door()
{
	delete &enterLocation;
	delete &exitLocation;
}

bool Door::isDestinationDoor(const Room& destination) const
{
	int size = destinations.size();

	for (int i = 0; i < size; i++)
	{
		if (destinations[i]->GetCenter().getX() == destination.GetCenter().getX() 
			&& destinations[i]->GetCenter().getY() == destination.GetCenter().getY())
			return true;
	}
	return false;
}

