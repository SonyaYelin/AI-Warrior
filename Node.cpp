#include "Node.h"
#include <math.h>


Node::Node(): point(0,0), target(0,0), g(0)
{
}

double Node::getF() const
{
	return getG()+getH();
}

double Node::getG() const
{
	return g;
}

double Node::getH() const
{
	return sqrt(pow(point.getX() - target.getX(), 2) +
			pow(point.getY() - target.getY(), 2));
}


Node::Node(const Point2D& p, const Point2D& t, double g)
{
	point = p;
	target = t;
	this->g = g;
}


Point2D Node::getPoint() const
{
	return point;
}
