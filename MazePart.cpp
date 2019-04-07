#include "MazePart.h"


MazePart::MazePart(Point2D *location, int type, double saftyScore) : location(location), type(type), saftyScore(saftyScore)
{
}

MazePart::~MazePart()
{
	delete location;
}

void MazePart::resetType()
{
	type = originType;
}
