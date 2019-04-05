#include "GLUT.h"
#include <math.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include "Point2D.h"
#include "Room.h"
#include "Node.h"
#include "CompareNodes.h"
#include "Parent.h"
#include "Storage.h" 
#include "Door.h" 
#include "Warrior.h" 
#include "Maze.h"

using namespace std;

static const int W = 600;	// window width
static const int H = 600;	// window height
static const int MSIZE = Maze::MSIZE;
static const double SQSIZE = 2.0 / MSIZE;
static const int NUM_OF_WARRIORS = 2;

Warrior *warriors[NUM_OF_WARRIORS];
Maze* maze;

void delay(int number_of_seconds);
void init();
void createWarriors();
void drawWarrior(const Warrior &warrior);
void DrawMaze();
void display();
void idle();


void delay(int number_of_seconds)
{
	// Converting time into milli_seconds 
	int milli_seconds = 10 * number_of_seconds;

	// Stroing start time 
	clock_t start_time = clock();

	// looping till required time is not acheived 
	while (clock() < start_time + milli_seconds);
}

void init()
{
	maze = &Maze::getInstance();
	createWarriors();

	glClearColor(0.7, 0.7, 0.7, 0);
	glOrtho(-1, 1, -1, 1, -1, 1);
}

void createWarriors()
{
	Room *r1 = nullptr, *r2 = nullptr;

	// get 2 random rooms
	srand(time(0));
	while (r1 == r2)
	{
		r1 = &( maze->getRooms()[rand() % Maze::NUM_ROOMS]);
		r2 = &( maze->getRooms()[rand() % Maze::NUM_ROOMS]);
	}

	warriors[0] = new Warrior(*r1, *new Point2D(r1->GetCenter().GetX(), r1->GetCenter().GetY()), 30, 40, 2, 15, 6, 0.8);
	warriors[1] = new Warrior(*r2, *new Point2D(r2->GetCenter().GetX(), r2->GetCenter().GetY()), 80, 20, 2, 10, 7, 0.7);

	drawWarrior(*warriors[0]);
	drawWarrior(*warriors[1]);
}

void drawWarrior(const Warrior &warrior)
{
	Point2D location = warrior.getLocation();
	maze->parts[location.GetY()][location.GetX()].setType(MazePart::WARRIOR);
}

void DrawMaze()
{
	for(int i = 0;i<MSIZE;i++)
		for (int j = 0; j < MSIZE; j++)
		{
			switch (maze->parts[i][j].getType())
			{
			case MazePart::WALL:
				glColor3d(0, 0, 0); // black
				break;
			case MazePart::SPACE:
				glColor3d(1, 1, 1); // white;
				break;
			case MazePart::MEDICAL:
				glColor3d(0,0,1); //blue
				break;
			case MazePart::AMMO:
				glColor3d(1,0,0); //red
				break;
			case MazePart::WARRIOR:
				glColor3d(1, .8, 0); // ORANGE
				break;

			}
			// draw square
			glBegin(GL_POLYGON);
				glVertex2d(j*SQSIZE - 1- SQSIZE/2, i*SQSIZE - 1+SQSIZE/2);
				glVertex2d(j*SQSIZE - 1 + SQSIZE / 2, i*SQSIZE - 1 + SQSIZE / 2);
				glVertex2d(j*SQSIZE - 1 + SQSIZE / 2, i*SQSIZE - 1 - SQSIZE / 2);
				glVertex2d(j*SQSIZE - 1 - SQSIZE / 2, i*SQSIZE - 1 - SQSIZE / 2);
			glEnd();
		}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	DrawMaze();

	// show what was drawn in "frame buffer"
	glutSwapBuffers();
}

void idle()
{
	if (!warriors[0]->isAlive() || !warriors[1]->isAlive())
		return;
	
	(warriors[0])->selectAction(*warriors[1]);
	(warriors[1])->selectAction(*warriors[0]);
	
	glutPostRedisplay(); // calls indirectly to display
	delay(5);
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("");

	glutDisplayFunc(display);	// refresh function
	glutIdleFunc(idle);			// idle: when nothing happens
	init();

	glutMainLoop();
}


