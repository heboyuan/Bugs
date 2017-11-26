#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include<string>
#include<list>
using namespace std;

class StudentWorld;
class Compiler;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//=======================ACTOR====================================
class Actor : public GraphObject
{
public:
	Actor(int ID,string myid, int sx, int sy,  StudentWorld* mw, int depth);
	virtual bool die() const;
	virtual int changeHP(int h);
	virtual void doSomething();
	StudentWorld* myworld() const;
	string getiden() const;
	virtual void bitten(int hplose);
	virtual void setStun(int num);
	bool checkmove() const;
	void changemove(bool tf);
	virtual bool ally(string id) const;;
	virtual ~Actor();
private:
	StudentWorld* world;	//pointer to the studentworld that the actor is in
	string iden;	//represent the identity of the actor
	bool moved;		//represent whether the actor get a chance to move
};

//===========================Poison=============================================
class Poison : public Actor
{
public:
	Poison(int stx, int sty, StudentWorld* mw);
	virtual void doSomething();
};


//====================water============================================

class Water : public Actor
{
public:
	Water(int stx, int sty, StudentWorld* mw);
	virtual void doSomething();
private:
	list<Actor*> StunedActors;
};

//====================Pebbles======================================
class Pebbles : public Actor
{
public:
	Pebbles(int stx, int sty, StudentWorld* mw);
};

//====================EnergyHolder===========================================
class EnergyHolder :public Actor
{
public:
	EnergyHolder(int ID, string myid, int sx, int sy, int HP, StudentWorld* mw, int depth);
	int getHP() const;
	virtual int changeHP(int h);
	virtual bool die() const;
private:
	int HP;		//the hitpoint of actor
};

//====================Food===========================================
class Food : public EnergyHolder
{
public:
	Food(int stx, int sty, int amount, StudentWorld* mw);
};

//=========================ElapseActor=========================================================
class ElapseActor : public EnergyHolder
{
public:
	ElapseActor(const int id, int stx, int sty, StudentWorld* mw, string i, int health, int depth);
	virtual void doSomething();
};


//=========================Pheromone=========================================================
class Pheromone : public ElapseActor
{
public:
	Pheromone(int stx, int sty, StudentWorld* mw, const int id, string myid);
	virtual int changeHP(int h);
};
//====================insect=================================

class insect : public ElapseActor
{
public:
	insect(const int id, int stx, int sty, StudentWorld* mw, string i, int health, int depth);
	int getStun() const;
	void setStun(int num);
	virtual void bitten(int hplose);
	bool insectsGeneralMove();
	void faceRandom();
	virtual bool ally(string id) const;
private:
	int stun;
};

//====================GrassHopper=====================================

class Grasshopper :public insect
{
public:
	Grasshopper(const int id, int stx, int sty, StudentWorld* mw, string i, int health, int depth);
	void GrasshopperGeneralMove();
	void setDesire(int num);
	int getDesire() const;
private:
	int desire;
};

//====================BabyGrassHopper=================================
class BabyGrassHopper : public Grasshopper
{
public:
	BabyGrassHopper(int stx, int sty, StudentWorld* mw);
	void doSomething();
};

//===========================AdultGrassHopper=============================================
class AdultGrassHopper : public Grasshopper
{
public:
	AdultGrassHopper(int stx, int sty, StudentWorld* mw);
	void doSomething();
	virtual void bitten(int hplose);
};

//========================Ant=============================================================
class Ant : public insect
{
public:
	Ant(int stx, int sty, StudentWorld* mw, const int id, string myid, Compiler* my);
	void doSomething();
	virtual void bitten(int hplose);
private:
	int food;
	Compiler* myCo;
	int ins;
	bool block, bit;
	int random;
};

//=========================Anthills===========================================================
class Anthill : public ElapseActor
{
public:
	Anthill(int stx, int sty, StudentWorld* mw, string myid, Compiler* my);
	void doSomething();
private:
	Compiler* myCo;
};
#endif // ACTOR_H_
