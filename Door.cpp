#include "Door.h"

Door::Door(Room& current, Point2D &enterlocation, Point2D &exitlocation):
	current(current), enterLocation(enterlocation), exitLocation(exitlocation)
{
	current.getBottom();
}


Door::~Door()
{
}

bool Door::isDestinationDoor(const Room& destination) const
{
	int size = destinations.size();

	for (int i = 0; i < size; i++)
	{
		if (destinations[i]->GetCenter().GetX() == destination.GetCenter().GetX() 
			&& destinations[i]->GetCenter().GetY() == destination.GetCenter().GetY())
			return true;
	}
	return false;
}

