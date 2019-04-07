#include "Maze.h"
#include "CompareNodes.h"
#include "CompareStorages.h"
#include "Warrior.h"

Maze* Maze::maze = nullptr;

Maze::Maze()
{
	int size = Constants::MSIZE;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			parts[i][j] = *new MazePart(new Point2D(j, i));
		}
	}

	loadMazeFromFile();
	setSaftyScores();
	createStorages();
}

Maze::~Maze()
{
	int size = Constants::MSIZE;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
			delete(&parts[i][j]);
	}
	for (int i = 0; i < Constants::STORAGE_NUM; i++)
	{
		delete(ammoStorage.at(i));
		delete(medicalStorage.at(i));
	}
	for (int i = 0; i < Constants::NUM_ROOMS; i++)
		delete(&rooms[i]);

	delete maze;
}

Maze& Maze::getInstance()
{
	if (maze == nullptr)
		maze = new Maze();
	return *maze;
}

/*
Loads maze stracture from file - walls, rooms and doors
*/
void Maze::loadMazeFromFile()
{
	int mSize = Constants::MSIZE;
	ifstream file;

	// load maze
	file.open("maze.txt");
	int col, row, type;
	file >> col;
	file >> row;
	for (int i = 0; i < mSize; i++)
	{
		for (int j = 0; j < mSize; j++)
		{
			file >> type;
			parts[i][j].setType(type);
			parts[i][j].setOriginType(type);
		}
	}
	file.close();

	// load rooms 
	file.open("rooms.txt");
	int size, x, y, h, w;
	file >> size;
	for (int i = 0; i < Constants::NUM_ROOMS; i++)
	{
		file >> x;
		file >> y;
		file >> h;
		file >> w;
		int idx = i + 1;
		rooms[i] = *new Room(idx, *new Point2D(x, y), w, h);
	}
	file.close();

	// load doors
	file.open("doors.txt");
	int from, x1, y1, x2, y2, rooms_num;
	file >> size;

	for (int i = 0; i < size; i++)
	{
		file >> from;	// srcRoom 
		file >> x1;		// enter point
		file >> y1;
		file >> x2;		// exit point
		file >> y2;
		file >> rooms_num;
		Door* door = new Door(rooms[from - 1], *new Point2D(x1, y1), *new Point2D(x2, y2));

		// doors destinations
		for (int j = 0; j < rooms_num; j++) {
			int roomIndex;
			file >> roomIndex;
			door->addDestination(rooms[roomIndex - 1]);
		}
		rooms[from - 1].addDoor(*door);
	}
	file.close();
}

/*
Checks if there is a storage located in point p, if so - returns the storage
*/
Storage* Maze::getIfStorage(const Point2D &p, int type) const
{
	vector<Storage*>::const_iterator itr;

	switch (type)
	{
	case Action::FIND_AMMO:
		for (itr = ammoStorage.begin() ; itr != ammoStorage.end() ; ++itr)
		{
			if ( (*itr)->getLocation() == p )
				return *itr;
		}
		break;

	case Action::FIND_MED:
		for (itr = medicalStorage.begin(); itr != medicalStorage.end(); ++itr)
		{
			if ((*itr)->getLocation() == p)
				return *itr;
		}
		break;
	}

	return nullptr;
}

void Maze::createStorages() {	
	for (int i = 0; i < Constants::STORAGE_NUM; i++)
	{
		int roomIndex = i;

		// set random points in the selected room		
		Point2D *rand1 = new Point2D();
		Point2D *rand2 = new Point2D();
		rooms[roomIndex].setRandomLocation(*rand1, *rand2);

		Storage *s1 = new Storage(i, rooms[roomIndex], *rand1, MazePart::AMMO, 52);
		ammoStorage.push_back(s1);
		drawStorage(*s1);

		Storage *s2 = new Storage(i, rooms[roomIndex], *rand2, MazePart::MEDICAL, 80);
		medicalStorage.push_back(s2);
		drawStorage(*s2);
	}
}

/*
Removes storage from storages-vector
Called when storage is empty
*/
void Maze::removeStorage(Storage &s, int type)
{
	int x = s.getLocation().getX();
	int y = s.getLocation().getY();
	vector<Storage*>::const_iterator itr;

	switch (type)
	{
	case Action::FIND_AMMO :
		itr = ammoStorage.end() - 1;

		if (s.getId() == (*itr)->getId())
			((vector<Storage*>)ammoStorage).pop_back();
		else
		{
			*(&s) = *(*itr);
			((vector<Storage*>)ammoStorage).pop_back();
		}
		break;
	case Action::FIND_MED:
		itr = medicalStorage.end() - 1;

		if (s.getId() == (*itr)->getId())
			((vector<Storage*>)medicalStorage).pop_back();
		else
		{
			*(&s) = *(*itr);
			((vector<Storage*>)medicalStorage).pop_back();
		}
		break;
	}

	MazePart &p = parts[y][x];
	p.setOriginType(MazePart::SPACE);
	p.setType(MazePart::SPACE);
}

void Maze::drawStorage(const Storage &s)
{
	Point2D &location = s.getLocation();
	parts[location.getY()][location.getX()].setType(s.getType());
	parts[location.getY()][location.getX()].setOriginType(s.getType());
}

/*
Each part of the maze gets a safty-score
*/
void Maze::setSaftyScores()
{
	int spaces = 0;
	int total = Warrior::MAX_SAFTY_SCORE; 
	int size = Constants::MSIZE;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			// safty determined by the number of spaces around the point
			spaces = countSpaces(i, j); 
			double score = (spaces / total);

			parts[i][j].setSaftyScore(score);
		}
	}
} 

int Maze::countSpaces(int i, int j)
{
	int count = 0;
	for (int k = i - 1; k < i + 1; k++)
	{
		for (int l = j - 1; l < j + 1; l++)
		{
			if (parts[k][l].getType() == MazePart::SPACE)
				count++;
		}
	}
	return count;
}

Room* Maze::getRooms() const
{
	return (Room*)rooms;
}

double Maze::getSaftyScore(Point2D &point) const
{
	return parts[point.getY()][point.getX()].getSaftyScore();
}

/*
Finds the best storage to go to 
Storage needs to be both far from enemy and close to the warrior
*/
Storage &Maze::getTargetStorage(int type, Point2D &currentLocation, Point2D &enamy) const
{
	priority_queue<StorageNode*, vector<StorageNode*>, CompareStorages> targetQueue;

	if (type == Action::FIND_AMMO)
	{
		int size = ammoStorage.size();
		for (int i = 0; i < size; i++)
			targetQueue.push(new StorageNode(currentLocation, enamy, *ammoStorage.at(i)));
	}

	else
	{
		int size = medicalStorage.size();
		for (int i = 0; i < size; i++)
			targetQueue.push(new StorageNode(currentLocation, enamy, *medicalStorage.at(i)));
	}
	return targetQueue.top()->getStorage();
}

stack<Point2D> Maze::aStar(Point2D &currentLocation, Point2D &targetLocation)
{
	stack<Point2D> walkingPath;
	Node *current = nullptr;
	priority_queue<Node*, vector<Node*>, CompareNodes> pq;
	vector<Point2D>::iterator gray_it;
	vector<Point2D*>::iterator black_it;
	vector <Point2D> gray;
	vector <Point2D> black;
	vector <Parent> parents;
	bool finished = false;

	pq.emplace(new Node(currentLocation, targetLocation, 0));
	while (!finished)
	{
		if (pq.empty())
			return walkingPath;

		vector<Parent>::iterator itr;
		if (current != nullptr)
			delete(current);

		current = pq.top();
		pq.pop(); 
		parents.push_back(*new Parent(current->getPoint(), current->getPoint(), false));

		// target has been found
		if (current->getH() == 0)
		{
			finished = true;

			// go back to start and enter the steps to walkingPath 
			itr = find(parents.begin(), parents.end(), Parent(current->getPoint(), current->getPoint(), true));
			walkingPath.push((itr->getCurrent()));

			while (itr->hasParent())
			{
				Point2D &tmp_prev = itr->getPrev();
				Point2D &tmp_cur = itr->getCurrent();
				walkingPath.push(tmp_cur);
				itr = find(parents.begin(), parents.end(),
					Parent(tmp_prev, current->getPoint(), true));
			}
		}

		// check the neighbours
		else
		{
			// remove current from gray and paint it black
			gray_it = find(gray.begin(), gray.end(), current->getPoint());
			black.push_back(current->getPoint());
			if (gray_it != gray.end())
				gray.erase(gray_it);

			AddNewNode(*current, targetLocation, gray, black, parents, pq, UP);
			AddNewNode(*current, targetLocation, gray, black, parents, pq, DOWN);
			AddNewNode(*current, targetLocation, gray, black, parents, pq, LEFT);
			AddNewNode(*current, targetLocation, gray, black, parents, pq, RIGHT);
		}
	}
	return walkingPath;
}

bool Maze::AddNewNode(Node & current, Point2D & targetLocation, vector<Point2D>& gray, vector<Point2D>& black, vector<Parent>& parents, priority_queue<Node*, vector<Node*>, CompareNodes>& pq, int direction)
{
	int dx, dy;
	int size = Constants::MSIZE;
	bool finished = false;
	double space_weight = 0.1, wall_weight = 5, weight;
	Node* tmp;
	Point2D* pt;
	vector<Point2D>::iterator gray_it;
	vector<Point2D>::iterator black_it;

	switch (direction)
	{
	case UP:
		dx = 0;
		dy = -1;
		break;
	case DOWN:
		dx = 0;
		dy = 1;
		break;
	case LEFT:
		dx = -1;
		dy = 0;
		break;
	case RIGHT:
		dx = 1;
		dy = 0;
		break;
	}

	if (maze->parts[current.getPoint().getY()][current.getPoint().getX() - 1].getType() == MazePart::WARRIOR)
		finished = true;

	if (direction == UP && current.getPoint().getY() > 0 ||
		direction == DOWN && current.getPoint().getY() < size - 1 ||
		direction == LEFT && current.getPoint().getX() > 0 ||
		direction == RIGHT && current.getPoint().getX() < size - 1)
	{
		pt = new Point2D(current.getPoint().getX() + dx, current.getPoint().getY() + dy);
		gray_it = find(gray.begin(), gray.end(), *pt);
		black_it = find(black.begin(), black.end(), *pt);
		
		if (gray_it == gray.end() && black_it == black.end()) 
		{
			if (maze->parts[current.getPoint().getY() + dy][current.getPoint().getX() + dx].getType() == MazePart::WALL)
				weight = wall_weight;
			else weight = space_weight;
			// weight depends on previous weight and wheater we had to dig to this point or not
			tmp = new Node(*pt, targetLocation, current.getG() + weight);
			pq.emplace(tmp);
			gray.push_back(*pt); 
			parents.push_back(Parent(tmp->getPoint(), current.getPoint(), true));
		}
	}
	return finished;
}