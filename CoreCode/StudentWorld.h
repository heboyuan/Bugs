#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include"Actor.h"
#include "Field.h"
#include<list>
#include <sstream> 
#include <iostream> 
#include <string>
using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
		for (int n = 0; n < 4; n++)
			playerCount[n] = 0;
		tickcount = 0;
	}

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	void removeDeadSimulationObjects();

	void setDisplayText();

	int changefood(int x, int y, int amount);

	bool check(int x, int y, string id);

	bool checkanthill(int x, int y, string id);

	bool checkpheromone(int x, int y);

	void addpheromone(int x, int y, const int i, string id);

	void addadultgrasshopper(int x, int y);

	void addant(int x, int y, const int id, string myid, Compiler* my);

	void stunactors(int x, int y, list<Actor*>& old);

	void poisonactors(int x, int y);

	Actor* findtarget(int x, int y, Actor* me);

	Actor* findtargetant(int x, int y,  string id);

	void addcount(string colony);

	~StudentWorld();


private:
	list<Actor*> actors[64][64];
	int tickcount;
	Field* myfield;
	int playerCount[4];
	vector<Compiler*> compilerForEntrant;
};

#endif // STUDENTWORLD_H_
