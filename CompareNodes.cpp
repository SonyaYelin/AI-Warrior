#include "CompareNodes.h"


bool CompareNodes::operator ( )(const Node* n1, const Node* n2)
{
	Point2D &p1 = n1->getPoint();
	Point2D &p2 = n2->getPoint();

	int x1 = p1.getX(), y1 = p1.getY(), x2 = p2.getX(), y2 = p2.getY();
	Maze &maze = Maze::getInstance();
	double safty1 = maze.parts[y1][x1].getSaftyScore();
	double safty2 = maze.parts[y2][x2].getSaftyScore();

	return n1->getF()+(1-safty1) > n2->getF()+(1-safty2);
}
