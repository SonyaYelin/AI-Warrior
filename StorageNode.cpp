#include "StorageNode.h"
#include <math.h>


StorageNode::StorageNode(Point2D& c, Point2D& e, Storage &s) :
	current(&c), enamy(&e), s(&s), g(0)
{
}

StorageNode::StorageNode(): g(0)
{
}

double StorageNode::GetF() const
{
	return GetH() - GetG();
}

double StorageNode::GetG() const
{
	// the bigger the distance the better
	int enamyX = enamy->getX();
	int enamyY = enamy->getY();
	return sqrt(pow(s->getLocation().getX() - enamyX, 2) + pow(s->getLocation().getY() - enamyY, 2));
	return g;
}

double StorageNode::GetH() const
{
	return sqrt(pow(current->getX() - s->getLocation().getX(), 2) +
		pow(current->getY() - s->getLocation().getY(), 2));
}

Storage & StorageNode::getStorage() const
{
	return *s;
}

Point2D & StorageNode::getEnamy() const
{
	return *enamy;
}

Point2D & StorageNode::getCurrent() const
{
	return *current;
}
