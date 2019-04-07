#pragma once

#include "Point2D.h"
class Node
{
public:
	Node();
private:
	double	g;
	Point2D	point;
	Point2D	target;

public:
	Node(const Point2D& p, const Point2D& t, double g);

	double getF() const;
	double getG() const;
	double getH() const;
	Point2D getPoint() const;
};

