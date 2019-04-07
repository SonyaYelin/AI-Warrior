#include "Warrior.h"
#include "Action.h"
#include "Door.h"
#include "CompareActions.h"

int Warrior::idGen = 0;


Warrior::Warrior(Room &room, Point2D &location, int lifePoint, int gunsAmmo, int grenadeAmmo, int interval, int	shootingSkiils, double	coward):
				currentRoom(&room), location(location), lifePoint(lifePoint), gunsAmmo(gunsAmmo),
				grenadeAmmo(grenadeAmmo), id(idGen++), interval(interval), shootingSkills(shootingSkiils), coward(coward)
{
	maze = &Maze::getInstance();
	updateActions();
}

Warrior::~Warrior()
{
	delete(&location);
	clearActions();
	clearPath();
}

void Warrior::updateCurrentRoom()
{
	Room *rooms = maze->getRooms();

	for (int i = 0; i < Constants::NUM_ROOMS; i++)
	{
		if (rooms[i].locatedInTheRoom(location))
		{
			currentRoom = &rooms[i];
			return;
		}
	}
	currentRoom = destRoom;
}

void Warrior::clearActions()
{
	while (!actionQueue.empty())
	{
		Action *a = actionQueue.top();
		actionQueue.pop();
		delete(a);
	}
}

void Warrior::clearPath()
{
	while (!walkingPath.empty())
	{
		Point2D &p = walkingPath.top();
		walkingPath.pop();
		delete(&p);
	}
}

void Warrior::updateActions()
{
	clearActions();

	actionQueue.push(new Action(*this, Action::FIGHT));
	actionQueue.push(new Action(*this, Action::FIND_AMMO));
	actionQueue.push(new Action(*this, Action::FIND_MED));
}

bool Warrior::isInRoom() const
{
	return currentRoom != nullptr;
}

bool Warrior::canShoot(const Warrior &other, int maxDist) const
{
	return	isInRoom()
			&& other.isInRoom()
			&& currentRoom->getId() == other.getCurrentRoom().getId()
			&& getDistance(other) <= maxDist;
}

void Warrior::makeMove(Warrior& other)
{
	static int count = 0;

	// reset path after each interval
	if ( ++count >= interval && ( isInRoom() || other.isInRoom()) )
	{
		count = 0;
		while (!walkingPath.empty())
			walkingPath.pop();
	}

	// if there is a path - move one step
	if (walkingPath.size() > 0) 
	{
		moveWarrior(walkingPath.top()); 
		walkingPath.pop();
	}
	else 
	{
		createPath(other);
		updateActions();
	}
}

void Warrior::createPath(Warrior &other)
{
	currentAction = actionQueue.top();	// FIND_AMMO, FIND_MED, FIGHT
	int type = currentAction->getType();
	Storage *s;

	switch (type)
	{
		case Action::FIGHT:
			lookForEnemy(other);
			break;

		case Action::FIND_AMMO:
			s = maze->getIfStorage(location, Action::FIND_AMMO);
			if (s != nullptr)
			{
				double neededAmount = Constants::MAX_GUNS_AMMO - gunsAmmo + Constants::MAX_GRANDE_AMMO - grenadeAmmo;
				takeAmmoStorage(*s, neededAmount);
			}
			else
				lookForStorage(maze->getTargetStorage(Action::FIND_AMMO, location, other.getLocation()), true);
			break;

		case Action::FIND_MED:
			s = maze->getIfStorage(location, Action::FIND_MED);
			if (s != nullptr)
			{
				double neededAmount = Constants::MAX_LIFE - lifePoint;
				takeMedStorage(*s, neededAmount);
			}
			else
				lookForStorage(maze->getTargetStorage(Action::FIND_MED, location, other.getLocation()), false);
			break;
	}
}

void Warrior::takeMedStorage(Storage &s, double neededAmount)
{
	double avilable = s.getAmount();
	if (avilable >= neededAmount)
	{
		s.takeAmount(neededAmount);
		lifePoint = Constants::MAX_LIFE;
	}
	else if (avilable > 0)
	{
		s.takeAmount(avilable);
		lifePoint += avilable;
		maze->removeStorage(s, Action::FIND_MED);
	}
	cout << "--- Warrior" << id << "found AMMO !!! ---" << endl;
}

void Warrior::takeAmmoStorage(Storage &s, double neededAmount)
{
	double avilable = s.getAmount();

	if (avilable >= neededAmount)
	{
		s.takeAmount(neededAmount);
		gunsAmmo = Constants::MAX_GUNS_AMMO;
		grenadeAmmo = Constants::MAX_GRANDE_AMMO;
	}
	
	else if (avilable >= 0)
	{
		s.takeAmount(avilable);
		if (avilable > Constants::MAX_GUNS_AMMO)
		{
			avilable -= Constants::MAX_GUNS_AMMO - s.getAmount();
			gunsAmmo = Constants::MAX_GUNS_AMMO;
			grenadeAmmo += (int)avilable;
			s.takeAmount(avilable);

		}
		else
			gunsAmmo += (int)avilable;

		maze->removeStorage(s, Action::FIND_AMMO);
	}
	cout << "--- Warrior" << id << "found AMMO !!! ---" << endl;
}

void Warrior::exitRoom(const Room &dest)
{
	cout << "--- " << id << " is exiting room: " << currentRoom->getId() << "---" << endl;

	// search for a door that leads to the destination
	vector<Door*> doors = currentRoom->getDoors();
	int numOfDoors = doors.size();
	Door *nextDoor = nullptr;
	
	for (int i = 0; i < numOfDoors; i++)
	{
		if (doors[i]->isDestinationDoor(dest))
		{
			nextDoor = doors[i];
			break;
		}
	}
	walkingPath = maze->aStar(location, nextDoor->getExitLocation());
	destRoom = nextDoor->getDestinations()[0];
}

void Warrior::lookForEnemy(Warrior &other)
{
	// if close enough to shoot
	if (canShoot(other, Constants::SHOOT_DISTANCE))
	{
		clearPath();
		shoot(other);
		return;
	}

	// if in the same room
	if (isInRoom() && other.isInRoom() && currentRoom->getId() == other.getCurrentRoom().getId())
		lookForEnemyInRoom(other);

	// enemy is in a different room
	else if (isInRoom() && other.isInRoom())
		exitRoom(other.getCurrentRoom());
	
	// enemy is in hallway 
	else if (&other.getDestRoom() != nullptr)
		exitRoom(other.getDestRoom());
	else
	{
		int idx = rand() % 10;
		Point2D &p = maze->getRooms()[idx].GetCenter();
		walkingPath = maze->aStar(location, p);
	}


}

void Warrior::shoot(Warrior &other)
{
	if (gunsAmmo <= 0)
		return;

	cout << "--- warrior " << id << " trying to shoot ---" << endl;

	srand(time(0));
	if (rand() % 10 > shootingSkills)
	{
		cout << "Warrior " << id << " Missed the shoot" << endl;
		return;
	}

	// Check if the warrior is close enough
	double distance = Constants::SHOOT_DISTANCE - (int)getDistance(other);
	if (getDistance(other) >= Constants::SHOOT_DISTANCE)
	{
		if (grenadeAmmo <= 0)
			walkingPath = maze->aStar(location, other.getLocation());
		else
		{
			srand(time(0));
			grenadeAmmo--;
			if (rand() % Constants::MAX_GRENADE == Constants::GRENADE)
			{
				other.injured(Constants::GRENADE_DAMAGE);
				cout << " warrior " << id << " threw grande at " << other.id << endl;
			}
		}
	}
	else
	{
		double damage = distance;
		currentAction->updateScore();
		gunsAmmo--;
		cout << " warrior " << id << " hitted warrior " << other.id  << endl;
		other.injured(damage);
	}
}

void Warrior::injured(double damage)
{
	lifePoint -= damage;
	if (lifePoint <= 0)
	{
		lifePoint = 0;
		life = false;
		cout << "--- Warrior " << id << " died ---" << endl;
		cout << "Game Over!" << endl;
	}
	else
		cout << "--- Warrior " << id << " lost " << lifePoint << " lifepoints" << endl;
}

double Warrior::getDistance(const Point2D &p1, const Point2D &p2) const
{
	int x = p1.getX() - p2.getX();
	int y = p1.getY() - p2.getY();

	return sqrt(pow(x, 2) + pow(y, 2));
}

double Warrior::getDistance(const Warrior & other) const
{
	return getDistance(this->getLocation(), other.getLocation());
}

void Warrior::lookForStorage(const Storage &s, bool ammo)
{
	// different rooms 
	if ( isInRoom() && currentRoom->getId() != s.getRoom().getId())
		exitRoom(s.getRoom());
	
	else if (ammo)
	{
		walkingPath = maze->aStar(location, s.getLocation()); // enemy is in the room
		cout << "warrior " << id << " looking for ammo" << endl;
	}
	else
	{
		walkingPath = maze->aStar(location, s.getLocation());
		cout << "warrior " << id << " looking for med" << endl;
	}
}

void Warrior::lookForEnemyInRoom(Warrior &other)
{
	if (canShoot(other, Constants::SHOOT_DISTANCE))
		shoot(other);
	else
		walkingPath = maze->aStar(location, other.getLocation());
}

void Warrior::moveWarrior(const Point2D &nextStep)
{
	// deleate the warrior from the maze
	maze->parts[location.getY()][location.getX()].resetType();

	// change the location of warrior.
	location.setX(nextStep.getX());
	location.setY(nextStep.getY());
	updateCurrentRoom();

	// draw the warrior on the maze
	maze->parts[location.getY()][location.getX()].setType(MazePart::WARRIOR);
}

