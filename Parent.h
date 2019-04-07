#pragma once
#include "Point2D.h"
class Parent
{
public:
	Parent();
private:
	Point2D current;
	Point2D prev;
	bool	has_prev;
public:
	Parent(Point2D &c, Point2D &p, bool has);

	bool operator==(Parent other);
	bool hasParent() const;
	Point2D &getPrev();
	Point2D &getCurrent();
};

