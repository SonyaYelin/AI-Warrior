#include "Storage.h"


Storage::Storage(int id, Room &containedInRoom, Point2D &location, int type, int amount): room(&containedInRoom), location(&location), type(type), amount(amount), id(id)
{
}


Storage::~Storage()
{
	delete(location);
}

Point2D &Storage::getLocation() const
{
	return *location;
}

int Storage::getType() const
{
	return type;
}