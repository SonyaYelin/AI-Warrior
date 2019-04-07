#include "Point2D.h"



Point2D::Point2D(): x(0)
{
}

Point2D::Point2D(int x, int y)
{
	this->x = x;
	this->y = y;
}


int Point2D::getX() const
{
	return x;
}


int Point2D::getY() const
{
	return y;
}


bool Point2D::operator==(const Point2D& other)
{
	return x == other.x && y == other.y;
}
