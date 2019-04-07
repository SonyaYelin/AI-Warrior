#include "Room.h"
#include "Door.h"
#include "Maze.h"
#include <stdlib.h>
Room::Room()
{
}


Room::~Room()
{
}


Room::Room(int id, const Point2D& center_point, int w, int h)
{
	this->id = id;
	center = (Point2D* )&center_point;
	width = w;
	height = h;
}


Point2D& Room::GetCenter() const
{
	return *center;
}


int Room::GetWidth() const
{
	return width;
}


int Room::GetHeight() const
{
	return height;
}


bool Room::IsOverlap(const Room& other)
{
	return abs(center->getX()-other.GetCenter().getX())
		< (width+other.width)/2+5 && abs(center->getY() - 
			other.GetCenter().getY()) < (height+other.height)/2+5  ;
}

int Room::getLeft() const
{
	int left = this->GetCenter().getX() - this->GetWidth() / 2;
	if (left < 0)
		return 0;
	else
		return left;
}

int Room::getRight() const
{
	int right = this->GetCenter().getX() + this->GetWidth() / 2;
	if (right >= Constants::MSIZE)
		return Constants::MSIZE - 1 ;
	return right;
}

int Room::getTop() const
{
	int top = this->GetCenter().getY() - this->GetHeight() / 2;
	if(top < 0)
		return 0;
	else 
		return top;
}

int Room::getBottom() const
{
	int bottom = this->GetCenter().getY() + this->GetHeight() / 2;
	if (bottom >= Constants::MSIZE) return bottom = Constants::MSIZE - 1;
	else
		return bottom;
}

vector<Door*> Room::getDoors() const
{
	return doors;
}

void Room::addDoor(const Door& door)
{
	doors.push_back((Door*)&door);
}

bool Room::locatedInTheRoom(const Point2D & p) const
{
	int x = p.getX();
	int y = p.getY();

	int top = center->getY() + (height / 2);
	int bottom = center->getY() - (height / 2);
	int left = center->getX() - (width / 2);
	int right = center->getX() + (width / 2);
	
	if (y <= top + 1 && y >= bottom -1 && x >= left - 1 && x <= right + 1)
		return true;

	return false;
}

Point2D& Room::getRandomPointInRoom()
{	
	srand(time(0));

	int posX = rand() % 2 == 1 ? 1 : -1;
	int posY = rand() % 2 == 1 ? 1 : -1;

	int x = center->getX() + posX * (rand() % (width / 2));
	int y = center->getY() + posY * (rand() % (height / 2));

	return *new Point2D(x, y);
}

void Room::setRandomLocation(Point2D &p1, Point2D &p2)
{
	int posX = rand() % 2 == 1 ? 1 : -1;
	int posY = rand() % 2 == 1 ? 1 : -1;

	int x = center->getX() + posX * (rand() % (width / 2));
	int y = center->getY() + posY * (rand() % (height / 2));

	p1.setX(x);
	p1.setY(y);

	if (y + 3 <= getTop())
		y = y + 3;
	else if (y - 3 >= getBottom())
		y = y - 3;
	else if(y + 2 <= getTop())
		y = y + 2;
	else if (y - 2 <= getTop())
		y = y - 2;
	else if (y + 1 <= getTop())
		y = y + 1;
	else if (y - 1 <= getTop())
		y = y - 1;

	if (x + 3 <= getRight())
		x = x + 3;
	else if (x - 3 >= getLeft())
		x = x - 3;
	if (x + 2 <= getRight())
		x = x + 2;
	else if (x - 2 >= getLeft())
		x = x - 2;
	else if (y + 1 <= getTop())
		y = y + 1;
	else if (y - 1 <= getTop())
		y = y - 1;

	p2.setX(x);
	p2.setY(y);
}