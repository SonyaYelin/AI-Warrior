#include "Node.h"
#include <math.h>


Node::Node(): point(0,0), target(0,0), g(0)
{
}

double Node::GetF() const
{
	return GetG()+GetH();
}

double Node::GetG() const
{
	return g;
}

double Node::GetH() const
{
	return sqrt(pow(point.GetX() - target.GetX(), 2) +
		pow(point.GetY() - target.GetY(), 2));
}


Node::Node(const Point2D& p, const Point2D& t, double g)
{
	point = p;
	target = t;
	this->g = g;
}


Point2D Node::GetPoint() const
{
	return point;
}
