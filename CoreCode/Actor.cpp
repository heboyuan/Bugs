#include "Actor.h"
#include "StudentWorld.h"
#include"Compiler.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//=======================ACTOR====================================
Actor::Actor(int ID, string myid, int sx, int sy,  StudentWorld* mw, int depth)
	:GraphObject(ID, sx, sy, none, depth),  world(mw), iden(myid), moved(false)
{
}

bool Actor::die() const	//check if the actor has died
{
	return false;
}

int Actor::changeHP(int h)
{
	return 0;
}

void Actor::doSomething()	//doSomthing does nothing
{
	return;
}

StudentWorld* Actor::myworld() const	//return the pointer to the studentworld that the actor is in
{
	return world;
}

string Actor::getiden()	const	//return the identity of the actor
{
	return iden;
}

void Actor::bitten(int hplose)	//the actor is bitten
{
	return;
}

void Actor::setStun(int num)	//the actor is stunned
{
	return;
}

 bool Actor::checkmove() const	//check to see if the actor has got a chance to move during this turn.
 {
	 return moved;
 }

 void Actor::changemove(bool tf)	//change the indicator of whether the actor has moved
 {
	 moved = tf;
 }


 bool Actor::ally(string id) const	//check if the actor is ally of another actor
 {
	 return true;
 }

Actor:: ~Actor()
{}

//===========================Poison=============================================
Poison::Poison(int stx, int sty, StudentWorld* mw)	//construct the poison
	:Actor(IID_POISON,"Poison", stx, sty,  mw, 2)
{
	setDirection(right);
}


void Poison::doSomething()
{
	myworld()->poisonactors(getX(), getY());	//ask the studentworld to poison the actors on the poison
}

//====================water============================================
Water::Water(int stx, int sty, StudentWorld* mw)	//construct the water
	:Actor(IID_WATER_POOL,"Water", stx, sty, mw, 2)
{
	setDirection(right);
}

void Water::doSomething()
{
	myworld()->stunactors(getX(), getY(), StunedActors);	//ask the studentworld to stun the actors on the water
}


//====================Pebbles=============================================
Pebbles::Pebbles(int stx, int sty, StudentWorld* mw)	//construct the pebble
	:Actor(IID_ROCK,"Pebble", stx, sty, mw, 1)
{
	setDirection(right);
}

//====================EnergyHolder===========================================

EnergyHolder::EnergyHolder(int ID, string myid, int sx, int sy, int HP, StudentWorld* mw, int depth)
	:Actor( ID,  myid,  sx,  sy, mw,  depth), HP(HP)
{}

int EnergyHolder::getHP() const	//get the hitpoint of the player
{
	return HP;
}

int EnergyHolder::changeHP(int h)		//change the HP of the player
{
	if ((HP + h) <= 0)		//if the HP would be changed to negative, change it to 0,return the actual HP changed 
	{
		int n = HP;
		HP = 0;
		return -n;
	}
	else	//else, change the HP and return the HP changed
	{
		HP += h;
		return h;
	}
}

bool EnergyHolder::die() const
{
	return HP <= 0;
}

//====================Food===========================================
Food::Food(int stx, int sty, int amount, StudentWorld* mw)	//construct the food
	:EnergyHolder(IID_FOOD,"Food", stx, sty, amount, mw, 2)
{
	setDirection(right);
}


//=========================ElapseActor=========================================================

ElapseActor::ElapseActor(const int id, int stx, int sty, StudentWorld* mw, string i, int health, int depth)	//the actors whose HP decrease each turn
	:EnergyHolder(id, i, stx, sty, health, mw, depth)
{}

void ElapseActor::doSomething()
{
	changeHP(-1);
}

//=========================Pheromone=========================================================

Pheromone::Pheromone(int stx, int sty, StudentWorld* mw, const int id, string myid)	//construct the pheromone
	:ElapseActor(id, stx,sty,mw,myid,256,2)
{
	setDirection(right);
}

int Pheromone::changeHP(int h)		//change HP with maximum of 768
{
	if (getHP() + h > 768)
		return EnergyHolder::changeHP(-getHP() + 768);
	else
		return EnergyHolder::changeHP(h);
}



//========================insect=================================
insect::insect(const int id, int stx, int sty, StudentWorld* mw, string i, int health, int depth)	//construct a insect
	:ElapseActor(id, stx, sty, mw,i, health, depth)
{
	faceRandom();
	stun = 0;
}

bool insect::ally(string id) const
{
	return id == getiden();
}

void insect::faceRandom()	//pick a random direction for the actor
{
	switch (randInt(0, 3))
	{
	case 0:
		setDirection(up);
		break;
	case 1:
		setDirection(down);
		break;
	case 2:
		setDirection(left);
		break;
	case 3:
		setDirection(right);
		break;
	}
}

void insect::setStun(int num)	//make the insect stun
{
	stun += num;
}


int insect::getStun() const		//get the turn that the insect should stun
{
	return stun;
}


void insect::bitten(int hplose)	//the insect get bitten, if bitten to die, change it to food
{
	changeHP(hplose);
	if (getHP() <= 0)
		myworld()->changefood(getX(),getY(),100);
}

bool insect::insectsGeneralMove()	//the general moves of all insects, if should sleep or stun, return true
{
	changemove(true);	//mark the insects as moved
	if (die())	//if the insect die for starve, change it to food
	{
		myworld()->changefood(getX(), getY(), 100);
		return true;
	}
	else if (getStun() > 0)	//stun the insect if supposed to
	{
		setStun(-1);
		return true;
	}
	return false;
}

//====================GrassHopper=====================================
Grasshopper::Grasshopper(const int id, int stx, int sty, StudentWorld* mw, string i, int health, int depth)	//construct the grasshopper
	:insect(id, stx, sty,  mw, i, health, depth)
{
	desire = randInt(2, 10);
}

void Grasshopper::GrasshopperGeneralMove()	//the general move of all grasshoppers
{
	if (myworld()->check(getX(), getY(),"Food"))	//eat the food if there is
	{
		changeHP(-myworld()->changefood(getX(), getY(), -200));
		if (randInt(0, 1) == 0)	//50% chance sleep after eat
		{
			setStun(2);
			return;
		}
	}

	if (getDesire() == 0)	//if no desire to walk more, change to random direction
	{
		faceRandom();
		setDesire(randInt(2, 10));
	}
	int x = getX(), y = getY();
	if (getDirection() == left)	//if is face left
	{
		if ((myworld()->check(x - 1, y,"Pebble")))	//if can't move to left, sleep and no desire to move 
		{
			setDesire(-getDesire());
			setStun(2);
		}
		else	//if can move, move and then sleep
		{
			moveTo(x - 1, y);
			setDesire(-1);
			setStun(2);
		}
	}
	else if (getDirection() == right)	//if is face right
	{
		if ((myworld()->check(x + 1, y,"Pebble")))	//if can't move to right, sleep and no desire to move 
		{
			setDesire(-getDesire());
			setStun(2);
		}
		else	//if can move, move and then sleep
		{
			moveTo(x + 1, y);
			setDesire(-1);
			setStun(2);
		}
	}
	else if (getDirection() == up)	//if is face up
	{
		if ((myworld()->check(x, y + 1,"Pebble")))	//if can't move up, sleep and no desire to move 
		{
			setDesire(-getDesire());
			setStun(2);
		}
		else	//if can move, move and then sleep
		{
			moveTo(x, y + 1);
			setDesire(-1);
			setStun(2);
		}
	}
	else if (getDirection() == down)	//if is face down
	{
		if ((myworld()->check(x, y - 1,"Pebble")))	//if can't move down, sleep and no desire to move 
		{
			setDesire(-getDesire());
			setStun(2);
		}
		else	//if can move, move and then sleep
		{
			moveTo(x, y - 1);
			setDesire(-1);
			setStun(2);
		}
	}
}


void Grasshopper::setDesire(int num)	//set the desire
{
	desire += num;
}

int Grasshopper::getDesire() const	//get the desire
{
	return desire;
}

//=======================BABYGRASSHOPPER=====================================
BabyGrassHopper::BabyGrassHopper(int stx, int sty, StudentWorld* mw)	//construct a babygrasshopper
	:Grasshopper(IID_BABY_GRASSHOPPER, stx, sty,  mw,"BabyGrassHopper", 500, 0)
{}

void BabyGrassHopper::doSomething()
{
	ElapseActor::doSomething();
	if (insectsGeneralMove())	//follow the general move of insects, if should sleep or stun, return 
		return;
	if (getHP() >= 1600)	//if get more than 1600 HP, evolve to andultgrasshopper
	{
		changeHP(-getHP());
		myworld()->addadultgrasshopper(getX(), getY());
		myworld()->changefood(getX(), getY(), 100);
		return;
	}
	GrasshopperGeneralMove();	//follow the general move of grasshoppers
}

//===========================AdultGrassHopper=============================================

AdultGrassHopper::AdultGrassHopper(int stx, int sty, StudentWorld* mw)
	:Grasshopper(IID_ADULT_GRASSHOPPER, stx, sty, mw, "AdultGrassHopper", 1600, 0)
{}

void AdultGrassHopper::bitten(int hplose)	//the reaction of adultgrasshopper when get bit
{
	changeHP(hplose);
	if (die())
		myworld()->changefood(getX(), getY(), 100);
	else
	{
		if (randInt(0, 1) == 0)	//if decide to revenge
		{
			Actor* victim = myworld()->findtarget(getX(), getY(), this);	//ask the student to find a target
			if (victim != nullptr)	//if there is a target, bit!
				victim->bitten(-50);
		}
	}
}

void AdultGrassHopper::doSomething()
{
	ElapseActor::doSomething();
	if (insectsGeneralMove())	//follow the general move of insects
		return;
	if (randInt(0, 2) == 0)	//if decide to bit
	{
		Actor* victim = myworld()->findtarget(getX(), getY(), this);
		if (victim != nullptr)	//if there is target to bit, bit and sleep
		{
			victim->bitten(-50);
			setStun(2);
			return;
		}
	}
	
	
	if (randInt(0, 9) == 0)	//if decide to jump
	{
		int x, y;
		while (1)	//keep find until get a vilad place to jump
		{
			x = randInt(-10, 10);	//random an x
			if (x + getX() >= VIEW_WIDTH||x+getX() < 0)	//if is out of range, find again
				continue;
			y = randInt(static_cast<int>(-sqrt(100 - x*x)), static_cast<int>(sqrt(100 - x*x)));	//random a y according to x
			if (y + getY() >= VIEW_HEIGHT || y + getY() < 0)	//if is out of range, find again
				continue;

			if (myworld()->check(getX() + x, getY() + y,"Pebble"))	//if the place is a pebble, find another place
				continue;
			moveTo(getX() + x, getY() + y);	//move to the position and sleep
			setStun(2);
			return;
		}
	}
	
	GrasshopperGeneralMove();	//follow the general move of grasshoppers
}

//========================Ant=============================================================
Ant::Ant(int stx, int sty, StudentWorld* mw, const int id, string myid, Compiler* my)
	:insect(id, stx, sty, mw, myid, 1500, 1), food(0), myCo(my), ins(0),block(false), bit(false), random(0)
{}

void Ant::bitten(int hplose)
{
	bit = true;
	insect::bitten(hplose);

}

void Ant::doSomething()
{
	ElapseActor::doSomething();
	if (insectsGeneralMove())	//follow the general move of insects
		return;
	Compiler::Command cmd;
	int exeCom = 0;		//set an idicator of how many commands have been processed
	while (exeCom < 10)	//if less than 10 commands been processed, go on processthe command
	{
		if (myCo->getCommand(ins, cmd))		//get the command
		{


			switch (cmd.opcode)
			{
			case Compiler::moveForward:
				if (getDirection() == left)	//if indicate to move left
				{
					if ((myworld()->check(getX() - 1, getY(),"Pebble")))	//and there is a pebble, update the state
					{
						block = true;
						ins++;
						return;
					}
					else	//otherwise, move and update the state
					{
						moveTo(getX() - 1, getY());
						block = false;
						bit = false;
						ins++;
						return;
					}
				}
				else if (getDirection() == right)	//if indicate to move right
				{
					if ((myworld()->check(getX() + 1, getY(),"Pebble")))	//and there is a pebble, update the state
					{
						block = true;
						ins++;
						return;
					}
					else	//otherwise, move and update the state
					{
						moveTo(getX() + 1, getY());
						block = false;
						bit = false;
						ins++;
						return;
					}
				}
				else if (getDirection() == up)	//if indicate to move up
				{
					if ((myworld()->check(getX(), getY() + 1,"Pebble")))	//and there is a pebble, update the state
					{
						block = true;
						ins++;
						return;
					}
					else	//otherwise, move and update the state
					{
						moveTo(getX(), getY() + 1);
						block = false;
						bit = false;
						ins++;
						return;
					}
				}
				else if (getDirection() == down)	//if indicate to move down
				{
					if ((myworld()->check(getX(), getY() - 1,"Pebble")))	//and there is a pebble, update the state
					{
						block = true;
						ins++;
						return;
					}
					else	//otherwise, move and update the state
					{
						moveTo(getX(), getY() - 1);
						block = false;
						bit = false;
						ins++;
						return;
					}
				}
				break;
			case Compiler::eatFood:
				if ((food - 100) <= 0)	//if there is not enough food, eat all the ant have 
				{
					changeHP(food);
					food = 0;
					ins++;
					return;
				}
				else		//otherwise, eat 100 food
				{
					food -= 100;
					changeHP(100);
					ins++;
					return;
				}
				break;
			case Compiler::dropFood:
				myworld()->changefood(getX(), getY(), food);	//drop all the food
				food = 0;
				ins++;
				return;
				break;
			case Compiler::bite:
				if (myworld()->findtargetant(getX(), getY(), getiden()) != nullptr)		//if we can find a target to bite, bite
					myworld()->findtargetant(getX(), getY(), getiden())->bitten(-15);
				ins++;
				return;
				break;
			case Compiler::pickupFood:
				if ((food + 400) > 1800)	//if the ant will pick more than max food it can have, pick all it can
					food -= myworld()->changefood(getX(), getY(), food - 1800);
				else
					food -= myworld()->changefood(getX(), getY(), -400);	//otherwise, pick 400 food
				ins++;
				return;
				break;
			case Compiler::emitPheromone:
				if (getiden() == "Ant_0")	//add pheromone according to the ant
					myworld()->addpheromone(getX(), getY(), IID_PHEROMONE_TYPE0, "Pheromone_0");
				else if (getiden() == "Ant_1")
					myworld()->addpheromone(getX(), getY(), IID_PHEROMONE_TYPE1, "Pheromone_1");
				else if (getiden() == "Ant_2")
					myworld()->addpheromone(getX(), getY(), IID_PHEROMONE_TYPE2, "Pheromone_2");
				else if (getiden() == "Ant_3")
					myworld()->addpheromone(getX(), getY(), IID_PHEROMONE_TYPE3, "Pheromone_3");
				ins++;
				return;
				break;
			case Compiler::faceRandomDirection:
				faceRandom();
				ins++;
				return;
				break;
			case Compiler::rotateClockwise:
				switch (getDirection())
				{
				case right:
					setDirection(down);
					break;
				case up:
					setDirection(right);
					break;
				case left:
					setDirection(up);
					break;
				case down:
					setDirection(left);
					break;
				}
				ins++;
				return;
				break;
			case Compiler::rotateCounterClockwise:
				switch (getDirection())
				{
				case right:
					setDirection(up);
					break;
				case up:
					setDirection(left);
					break;
				case left:
					setDirection(down);
					break;
				case down:
					setDirection(right);
					break;
				}
				ins++;
				return;
				break;
			case Compiler::generateRandomNumber:
				if (stoi(cmd.operand1) == 0)	//if the operand is 0, random number is 0
					random = 0;
				else
					random = randInt(0, stoi(cmd.operand1) - 1);	//otherwise, generate random number according to operand
				ins++;
				exeCom++;
				break;
			case Compiler::goto_command:
				ins = stoi(cmd.operand1);
				exeCom++;
				break;
			case Compiler::if_command:
				switch (stoi(cmd.operand1))
				{
				case Compiler::last_random_number_was_zero:
					if (random == 0)
						ins = stoi(cmd.operand2);
					else
						ins++;
					break;
				case Compiler::i_am_carrying_food:
					if (food > 0)
					{
						ins = stoi(cmd.operand2);
					}
					else
					{
						ins++;
					}
					break;
				case Compiler::i_am_hungry:
					if (getHP() <= 25)
						ins = stoi(cmd.operand2);
					else
						ins++;
					break;
				case Compiler::i_am_standing_with_an_enemy:
					if (myworld()->findtargetant(getX(), getY(), getiden()) != nullptr)	//ask the studentworld to check if there is an enemy
					{
						ins = stoi(cmd.operand2);
					}
					else
					{
						ins++;
					}
					break;
				case Compiler::i_am_standing_on_food:
					if (myworld()->check(getX(), getY(),"Food"))	//ask the studentworld to check if there is an food
						ins = stoi(cmd.operand2);
					else
						ins++;
					break;
				case Compiler::i_am_standing_on_my_anthill:
					if (myworld()->checkanthill(getX(), getY(), getiden()))		//ask the studentworld to check if there is the ant's anthill
						ins = stoi(cmd.operand2);
					else
						ins++;
					break;
				case Compiler::i_smell_pheromone_in_front_of_me:	//ask the studentworld to check the pheromone according to the direction
					if (getDirection() == left && myworld()->checkpheromone(getX() - 1, getY()))
						ins = stoi(cmd.operand2);
					else if (getDirection() == right && myworld()->checkpheromone(getX() + 1, getY()))
						ins = stoi(cmd.operand2);
					else if (getDirection() == up && myworld()->checkpheromone(getX(), getY() + 1))
						ins = stoi(cmd.operand2);
					else if (getDirection() == down && myworld()->checkpheromone(getX(), getY() - 1))
						ins = stoi(cmd.operand2);
					else
						ins++;
					break;
				case Compiler::i_smell_danger_in_front_of_me:		//ask the studentworld to check the danger according to the direction
					if (getDirection() == left && (myworld()->check(getX() - 1, getY(), "Poison") || myworld()->check(getX() - 1, getY(), "Water") || myworld()->findtargetant(getX() - 1, getY(), getiden()) != nullptr))
					{
						ins = stoi(cmd.operand2);
					}
					else if (getDirection() == right && (myworld()->check(getX() + 1, getY(), "Poison") || myworld()->check(getX() + 1, getY(), "Water") || myworld()->findtargetant(getX() + 1, getY(), getiden()) != nullptr))
					{
						ins = stoi(cmd.operand2);
					}
					else if (getDirection() == up && (myworld()->check(getX(), getY() + 1, "Poison") || myworld()->check(getX(), getY() + 1, "Water") || myworld()->findtargetant(getX(), getY() + 1, getiden()) != nullptr))
					{
						ins = stoi(cmd.operand2);
					}
					else if (getDirection() == down && (myworld()->check(getX(), getY() - 1, "Poison") || myworld()->check(getX(), getY() - 1, "Water") || myworld()->findtargetant(getX(), getY() - 1, getiden()) != nullptr))
					{
						ins = stoi(cmd.operand2);
					}
					else
					{
						ins++;
					}
						break;
				case Compiler::i_was_bit:
					if (bit == true)
					{
						ins = stoi(cmd.operand2);
					}
					else
					{
						ins++;
					}
						break;
				case Compiler::i_was_blocked_from_moving:
					if (block == true)
					{
						ins = stoi(cmd.operand2);
					}
					else
					{
						ins++;
					}
					break;
				}
				exeCom++;
				break;
			}
		}
		else
		{
			changeHP(-getHP());
			return;
		}
	}
}

//=========================Anthills===========================================================
Anthill::Anthill(int stx, int sty, StudentWorld* mw, string myid, Compiler* my)
	:ElapseActor(IID_ANT_HILL, stx, sty ,mw, myid, 8999, 2), myCo(my)
{
	setDirection(right);
}

void Anthill::doSomething()
{	
	ElapseActor::doSomething();
	if (die())
		return;
	if (myworld()->check(getX(), getY(),"Food"))
	{
		changeHP(-myworld()->changefood(getX(), getY(), -10000));
		return;
	}

	if (getHP() >= 2000)	//add ant if have enough food
	{
		if(getiden()=="Anthill_0")
		{
			myworld()->addant(getX(), getY(), IID_ANT_TYPE0, "Ant_0", myCo);
		}
		else if (getiden() == "Anthill_1")
		{
			myworld()->addant(getX(), getY(), IID_ANT_TYPE1, "Ant_1", myCo);
		}
		else if (getiden() == "Anthill_2")
		{
			myworld()->addant(getX(), getY(), IID_ANT_TYPE2, "Ant_2", myCo);
		}
		else if (getiden() == "Anthill_3")
		{
			myworld()->addant(getX(), getY(), IID_ANT_TYPE3, "Ant_3", myCo);
		}

		changeHP(-1500);
	}
}