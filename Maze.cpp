#include "Maze.h"
#include "CompareNodes.h"
#include "CompareStorages.h"
#include "Warrior.h"

Maze* Maze::maze = nullptr;

Maze::Maze()
{
	for (int i = 0; i < MSIZE; i++)
	{
		for (int j = 0; j < MSIZE; j++)
			parts[i][j] = *new MazePart(new Point2D(j, i));
	}

	loadMazeFromFile();
	setSaftyScores();
	createStorages();
}

Maze::~Maze()
{
	delete maze;
	for (int i = 0; i < MSIZE; i++)
	{
		for (int j = 0; j < MSIZE; j++)
			delete(&parts[i][j]);
	}
	for (int i = 0; i < STORAGE_NUM; i++)
	{
		delete(ammoStorage.at(i));
		delete(medicalStorage.at(i));
	}
	for (int i = 0; i < NUM_ROOMS; i++)
		delete(&all_rooms[i]);
}

Maze& Maze::getInstance()
{
	if (maze == nullptr)
		maze = new Maze();
	return *maze;
}

void Maze::loadMazeFromFile()
{
	ifstream file;

	// load maze
	file.open("maze.txt");
	int col, row, type;
	file >> col;
	file >> row;
	for (int i = 0; i < MSIZE; i++)
	{
		for (int j = 0; j < MSIZE; j++)
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
	for (int i = 0; i < NUM_ROOMS; i++)
	{
		file >> x;
		file >> y;
		file >> h;
		file >> w;
		int idx = i + 1;
		all_rooms[i] = *new Room(idx, *new Point2D(x, y), w, h);
	}
	file.close();

	// load doors
	file.open("doors2.txt");
	int from, x1, y1, x2, y2, rooms_num;
	file >> size;

	// sourceRoom ,enter and exit location of the doors, vactor of destionations
	for (int i = 0; i < size; i++)
	{
		file >> from;
		file >> x1;
		file >> y1;
		file >> x2;
		file >> y2;
		file >> rooms_num;
		Door* door = new Door(all_rooms[from - 1], *new Point2D(x1, y1), *new Point2D(x2, y2));

		for (int j = 0; j < rooms_num; j++) {
			int roomIndex;
			file >> roomIndex;
			door->addDestination(all_rooms[roomIndex - 1]);
		}
		all_rooms[from - 1].addDoor(*door);
	}
	file.close();
}

Storage* Maze::getIfStorage(const Point2D &p, int type) const
{
	Storage *storage = nullptr;

	switch (type)
	{
	case Action::FIND_AMMO:
		for (int i = 0; i < STORAGE_NUM; i++)
		{
			if (ammoStorage.at(i)->getLocation() == p)
				return (ammoStorage.at(i));
		}
		break;

	case Action::FIND_MED:
		for (int i = 0; i < STORAGE_NUM; i++)
		{
			if (medicalStorage.at(i)->getLocation() == p)
				return (medicalStorage.at(i));
		}
		break;
	}
	return nullptr;
}

void Maze::createStorages() {
	
	for (int i = 0; i < STORAGE_NUM; i++)
	{
		int roomIndex = i;

		// set random points in the selected room		
		Point2D *rand1 = new Point2D();
		Point2D *rand2 = new Point2D();
		all_rooms[roomIndex].setRandomLocation(*rand1, *rand2);

		Storage *s1 = new Storage(i, all_rooms[roomIndex], *rand1, MazePart::AMMO, 52);
		Storage *s2 = new Storage(i, all_rooms[roomIndex], *rand2, MazePart::MEDICAL, 80);

		ammoStorage.push_back(s1);
		medicalStorage.push_back(s2);

		drawStorage(*s1);
		drawStorage(*s2);
	}
}

void Maze::removeStorage(Storage &s, int type)
{
	int x = s.getLocation().GetX();
	int y = s.getLocation().GetY();
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
	parts[location.GetY()][location.GetX()].setType(s.getType());
	parts[location.GetY()][location.GetX()].setOriginType(s.getType());
}

void Maze::setSaftyScores()
{
	int spaces = 0;
	int total = Warrior::MAX_SAFTY_SCORE; 

	for (int i = 0; i < MSIZE; i++)
	{
		for (int j = 0; j < MSIZE; j++)
		{
			spaces = countSpaces(i, j);

			double score = (spaces / total);
			parts[i][j].setSaftyScore(score);
		}
	}
} 

double Maze::getSafty(int x, int y) const
{
	return parts[y][x].getSaftyScore();
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
	return (Room*)all_rooms;
}

double Maze::getSaftyScore(Point2D &point) const
{
	return parts[point.GetY()][point.GetX()].getSaftyScore();
}

Storage &Maze::getTargetStorage(int type, Point2D &currentLocation, Point2D &enamy) const
{
	priority_queue<StorageNode*, vector<StorageNode*>, CompareStorages> targetQueue;

	if (type == Action::FIND_AMMO)
	{
		int size = ammoStorage.size();
		for (int i = 0; i < size; i++)
		{
			StorageNode *n = new StorageNode(currentLocation, enamy, *ammoStorage.at(i));
			targetQueue.push(n);
		}		
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
	priority_queue<Node*, vector<Node*>, CompareNodes> pq; // the compare node class may not considare the saftyScore.
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
		parents.push_back(*new Parent(current->GetPoint(), current->GetPoint(), false));

		// target has been found
		if (current->GetH() == 0)
		{
			finished = true;

			// go back to start and enter the steps to walkingPath 
			itr = find(parents.begin(), parents.end(), Parent(current->GetPoint(), current->GetPoint(), true));
			walkingPath.push((itr->GetCurrent()));

			while (itr->HasParent())
			{
				Point2D &tmp_prev = itr->GetPrev();
				Point2D &tmp_cur = itr->GetCurrent();
				walkingPath.push(tmp_cur);
				itr = find(parents.begin(), parents.end(),
					Parent(tmp_prev, current->GetPoint(), true));

			}
		}

		// check the neighbours
		else
		{
			// remove current from gray 
			gray_it = find(gray.begin(), gray.end(), current->GetPoint());
			if (gray_it != gray.end())
				gray.erase(gray_it);

			// and paint it black
			black.push_back(current->GetPoint());

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
	Node* tmp;
	Point2D* pt;
	vector<Point2D>::iterator gray_it;
	vector<Point2D>::iterator black_it;
	double space_weight = 0.1, wall_weight = 5, weight;
	int dx, dy;
	bool finished = false;

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
	} // switch

	if (maze->parts[current.GetPoint().GetY()][current.GetPoint().GetX() - 1].getType() == MazePart::WARRIOR)
		finished = true;

	if (direction == UP && current.GetPoint().GetY() > 0 ||
		direction == DOWN && current.GetPoint().GetY() < MSIZE - 1 ||
		direction == LEFT && current.GetPoint().GetX() > 0 ||
		direction == RIGHT && current.GetPoint().GetX() < MSIZE - 1)
	{
		pt = new Point2D(current.GetPoint().GetX() + dx, current.GetPoint().GetY() + dy);
		gray_it = find(gray.begin(), gray.end(), *pt);
		black_it = find(black.begin(), black.end(), *pt);
		if (gray_it == gray.end() && black_it == black.end()) 
		{
			if (maze->parts[current.GetPoint().GetY() + dy][current.GetPoint().GetX() + dx].getType() == MazePart::WALL)
				weight = wall_weight;
			else weight = space_weight;
			// weight depends on previous weight and wheater we had to dig
			// to this point or not
			tmp = new Node(*pt, targetLocation, current.GetG() + weight);
			pq.emplace(tmp); // insert first node to priority queue
			gray.push_back(*pt); // paint it gray
			// add Parent
			parents.push_back(Parent(tmp->GetPoint(), current.GetPoint(), true));
		}
	}
	return finished;
}