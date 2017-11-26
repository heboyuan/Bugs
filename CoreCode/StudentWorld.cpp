#include "StudentWorld.h"
#include <string>
#include"Compiler.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

 int StudentWorld::init()
 {

	 Field f;
	 string fieldFile = getFieldFilename();

	 if (f.loadField(fieldFile) != Field::LoadResult::load_success)
		 return GWSTATUS_LEVEL_ERROR;

	 vector<string> fileNames = getFilenamesOfAntPrograms();
	 string error;

	 for (int x = 0; x < 64; x++)		//go through the two dimensional array and initialize it accordingly
		 for (int y = 0; y < 64; y++)
		 {
			 switch (f.getContentsOf(x, y))
			 {
			case Field::FieldItem::anthill0:
				if (fileNames.size() >= 1)
				{
				compilerForEntrant.push_back(new Compiler);
					if (!compilerForEntrant.back()->compile(fileNames[0], error))
					{
						setError(fileNames[0] + " " + error);
						return GWSTATUS_LEVEL_ERROR;
					}
					actors[x][y].push_front(new Anthill(x, y, this, "Anthill_0", compilerForEntrant.back()));
				}
				  break;
			  case Field::FieldItem::anthill1:
				  if (fileNames.size() >= 2)
				  {
					  compilerForEntrant.push_back(new Compiler);
					  if (!compilerForEntrant.back()->compile(fileNames[1], error))
					  {
						  setError(fileNames[1] + " " + error);
						  return GWSTATUS_LEVEL_ERROR;
					  }
					  actors[x][y].push_front(new Anthill(x, y, this, "Anthill_1", compilerForEntrant.back()));
				  }
				  break;
			  case Field::FieldItem::anthill2:
				  if (fileNames.size() >= 3)
				  {
					  compilerForEntrant.push_back(new Compiler);
					  if (!compilerForEntrant.back()->compile(fileNames[2], error))
					  {
						  setError(fileNames[2] + " " + error);
						  return GWSTATUS_LEVEL_ERROR;
					  }
					  actors[x][y].push_front(new Anthill(x, y, this, "Anthill_2", compilerForEntrant.back()));
				  }
				  break;
			  case Field::FieldItem::anthill3:
				  if (fileNames.size() >= 4)
				  {
					  compilerForEntrant.push_back(new Compiler);
					  if (!compilerForEntrant.back()->compile(fileNames[3], error))
					  {
						  setError(fileNames[3] + " " + error);
						  return GWSTATUS_LEVEL_ERROR;
					  }
					  actors[x][y].push_front(new Anthill(x, y, this, "Anthill_3", compilerForEntrant.back()));
				  }
				  break;
			  case Field::FieldItem::poison:
				  actors[x][y].push_front(new Poison(x, y, this));
				  break;
			  case Field::FieldItem::water:
				  actors[x][y].push_front(new Water(x, y, this));
				  break;
			  case Field::FieldItem::food:
				  actors[x][y].push_front(new Food(x, y, 6000, this));
				  break;
			  case Field::FieldItem::rock:
				  actors[x][y].push_front(new Pebbles(x, y, this));
				  break;
			  case Field::FieldItem::grasshopper:
				  actors[x][y].push_front(new BabyGrassHopper(x, y, this));
				  break;
			 }
		 }

	 myfield = &f;	//store the field

	 return GWSTATUS_CONTINUE_GAME;
 }

  int StudentWorld::move()
 {
	 setDisplayText();
	 tickcount++;
	 for (int x = 0; x < 64; x++)	//go through the array of actors
		 for (int y = 0; y < 64; y++)
			 if (!actors[x][y].empty())		//if there is actor
			 {
				 list<Actor*>::iterator p = actors[x][y].begin();	//go through the list
				 while (p != actors[x][y].end())
				 {
					 if (!(*p)->die() && !(*p)->checkmove())		//if the player is not die and has not move in this turn
					 {
						 (*p)->doSomething();		//the player get a chance to move
						 if ((*p)->getX() != x || (*p)->getY() != y)	//if it move to new location, updata it
						 {
							 actors[(*p)->getX()][(*p)->getY()].push_front((*p));
							 p = actors[x][y].erase(p);
						 }
						 else
							 p++;
					 }
					 else
						 p++;
				 }
			 }

	 for (int x = 0; x < 64; x++)	//go through all the actors and set the move indicator to haven't moved
		 for (int y = 0; y < 64; y++)
			 if (!actors[x][y].empty())
			 {
				 list<Actor*>::iterator p = actors[x][y].begin();
				 while (p != actors[x][y].end())
				 {
					 if (!(*p)->die() && (*p)->checkmove())
					 {
						 (*p)->changemove(false);
						 p++;
					 }
					 else
						 p++;
				 }
			 }

	 removeDeadSimulationObjects();

	void setDisplayText();

	if (tickcount >= 2000)
	{
		int n = 0, max = playerCount[0];
		for (unsigned int k = 0; k < compilerForEntrant.size(); k++)
			if (playerCount[k] > max)
			{
				n = k;
				max = playerCount[k];
			}
		if (max <= 5)
		{
			return GWSTATUS_NO_WINNER;
		}
		else
		{
			setWinner(compilerForEntrant[n]->getColonyName());
			return GWSTATUS_PLAYER_WON;
		}
	}
	 else
	 { 
		 return GWSTATUS_CONTINUE_GAME;
	 }
 }

  void StudentWorld::cleanUp()
  {
	  for (int x = 0; x < 64; x++)	//go through all the actors and delete them
		  for (int y = 0; y < 64; y++)
		  {
			  list<Actor*>::iterator p = actors[x][y].begin();
			  while (p != actors[x][y].end())
			  {
				  delete (*p);
				  p = actors[x][y].erase(p);
			  }
		  }
	  vector<Compiler*>::iterator z = compilerForEntrant.begin();
	  while (z != compilerForEntrant.end())
	  {
		  delete (*z);
		  z = compilerForEntrant.erase(z);
	  }
  }

  void StudentWorld::removeDeadSimulationObjects()
  {
	  for (int x = 0; x < 64; x++)	//go through all the actors, if they are died, remove them
		  for (int y = 0; y < 64; y++)
			  if (!actors[x][y].empty())
			  {
				  list<Actor*>::iterator p = actors[x][y].begin();
				  while (p != actors[x][y].end())
				  {
					  if ((*p)->die())
					  {
						  delete (*p);
						  p = actors[x][y].erase(p);
					  }
					  else
						  p++;
				  }
			  }
  }

  void StudentWorld::setDisplayText()
  {
	  ostringstream ts;
	  ts << "Ticks: " << 2000 - tickcount << " -";
	  for (unsigned int n = 0; n < compilerForEntrant.size(); n++)
		  ts << " " << compilerForEntrant[n]->getColonyName() << ": " << playerCount[n]<<" ants";
	  string s = ts.str();
	  setGameStatText(s);
  }

  int StudentWorld::changefood(int x, int y, int amount)
  {
	  if (!actors[x][y].empty())	//if there is actors at x,y
	  {
		  list<Actor*>::iterator p = actors[x][y].begin();
		  while (p != actors[x][y].end())	//go through the actos
		  {
			  if ((*p)->getiden() == "Food")	//if find the actor food
			  {
				  return  (*p)->changeHP(amount);	//change it according to the amount and return the amount changed
			  }
			  p++;
		  }
	  }

	  if (amount > 0)	//if there is no food and we try to increase food 
	  {
		  actors[x][y].push_front(new Food(x, y, amount, this));	//create new food at the location
		  return amount;
	  }
	  else
		  return 0;
  }

  void StudentWorld::addpheromone(int x, int y, const int i, string id)
  {
	  if (!actors[x][y].empty())	//if there is actors at indicated position
	  {
		  list<Actor*>::iterator p = actors[x][y].begin();
		  while (p != actors[x][y].end())	//go through the actors
		  {
			  if ((*p)->getiden() == id)	//if we find the indicated pheromone
			  {
				  (*p)->changeHP(256);//increace the HP of the pheromone
				  return;
			  }
			  p++;
		  }
	  }
	  actors[x][y].push_front(new Pheromone(x, y, this,i,id));	//if there is no indicated pheromone, create a new one
  }

 void StudentWorld::addadultgrasshopper(int x, int y)
  {
	  actors[x][y].push_front(new AdultGrassHopper(x, y, this));
  }

 void StudentWorld::addant(int x, int y, const int id, string myid, Compiler* my)
 {
	 actors[x][y].push_front(new Ant(x, y, this, id, myid, my));
	 addcount(myid);
 }

  bool StudentWorld::check(int x, int y, string id)
  {
	  if (!actors[x][y].empty())	//if there is actors at the indicated location
	  {
		  list<Actor*>::iterator p = actors[x][y].begin();
		  while (p != actors[x][y].end())	//go through the actors
		  {
			  if ((*p)->getiden() == id)	//if there is idicated actor, return true
				  return true;
			  p++;
		  }
	  }
	  return false;
  }

  bool StudentWorld::checkanthill(int x, int y, string id)
  {
	  if (!actors[x][y].empty())	//if there is actors at the indicated location
	  {
		  list<Actor*>::iterator p = actors[x][y].begin();
		  while (p != actors[x][y].end())	//go through the actors
		  {
			  if ((*p)->getiden() == "Anthill_0" && id == "Ant_0")	//if the anthill and the ant matches, return true
				  return true;
			  else if ((*p)->getiden() == "Anthill_1" && id == "Ant_1")
				  return true;
			  else if ((*p)->getiden() == "Anthill_2" && id == "Ant_2")
				  return true;
			  else if ((*p)->getiden() == "Anthill_3" && id == "Ant_3")
				  return true;
			  p++;
		  }
	  }
	  return false;
  }

  bool StudentWorld::checkpheromone(int x, int y)
  {
	  if (!actors[x][y].empty())	//if there is actors at the indicated location
	  {
		  list<Actor*>::iterator p = actors[x][y].begin();	//go through the actors
		  while (p != actors[x][y].end())
		  {
			  if ((*p)->getiden() == "Pheromone_0"|| (*p)->getiden() == "Pheromone_1" || (*p)->getiden() == "Pheromone_2" || (*p)->getiden() == "Pheromone_3")//if there is Pheromone, return true
				  return true;
			  p++;
		  }
	  }
	  return false;
  }

  Actor* StudentWorld::findtarget(int x, int y, Actor* me)
  {
	  int num = 0;
	  list<Actor*>::iterator p = actors[x][y].begin();	//go through the actors
	  while (p != actors[x][y].end())
	  {
		  if (((*p)->getiden() == "Ant_3" || (*p)->getiden() == "Ant_2" || (*p)->getiden() == "Ant_1" || (*p)->getiden() == "Ant_0" || (*p)->getiden() == "BabyGrassHopper" || (*p)->getiden() == "AdultGrassHopper") && (*p) != me)	//count the number of potential targets
			  num++;
		  p++;
	  }

	  if (num > 0)	//if there is target
	  {
		  int theTarget = randInt(1, num);	//generate a random number according to the number of potential target
		  list<Actor*>::iterator p = actors[x][y].begin();
		  while (1)
		  {
			  if (((*p)->getiden() == "Ant_3" || (*p)->getiden() == "Ant_2" || (*p)->getiden() == "Ant_1" || (*p)->getiden() == "Ant_0" || (*p)->getiden() == "BabyGrassHopper" || (*p)->getiden() == "AdultGrassHopper") && (*p) != me)	//if we go over one target
			  {
				  theTarget--;	//decrease the random number
				  if (theTarget == 0)	//if random number reach 0, we find the target
					  return (*p);
				  p++;
			  }
			  else
				  p++;
		  }
	  }
	  else
	  {
		  return nullptr;	//if we can't find target, return nullptr
	  }
  }

  Actor* StudentWorld::findtargetant(int x, int y, string id)
  {
	  int num = 0;
	  list<Actor*>::iterator p = actors[x][y].begin();
	  while (p != actors[x][y].end())	//go through the actors
	  {
		  if(!(*p)->ally(id))	//count the number of actors that is not ally
			  num++;
		  p++;
	  }

	  if (num > 0)	//if there is actors that is not ally
	  {
		  int theTarget = randInt(1, num);	//generate a random number according to the number of actors that is not ally
		  list<Actor*>::iterator p = actors[x][y].begin();
		  while (1)
		  {
			  if (!(*p)->ally(id))	//if we go over one actors that is not ally
			  {
				  theTarget--;	//decrease the random number
				  if (theTarget == 0)	//if random number reach 0, we find the target
					  return (*p);
				  p++;
			  }
			  else
				  p++;
		  }
	  }
	  else
	  {
		  return nullptr;	//if we can't find target, return nullptr
	  }
  }


  void StudentWorld::stunactors(int x, int y, list<Actor*>& old)
  {
	  list<Actor*> current;	//create a list for the actors that are currently on the water
	  if (!actors[x][y].empty())
	  {
		  list<Actor*>::iterator p = actors[x][y].begin();
		  while (p != actors[x][y].end())	//go over the actors on the water
		  {
			  if ((*p)->getiden() == "Ant_3" || (*p)->getiden() == "Ant_2" || (*p)->getiden() == "Ant_1" || (*p)->getiden() == "Ant_0" || (*p)->getiden() == "BabyGrassHopper" || (*p)->getiden() == "AdultGrassHopper")	//if the actor can be stunned
			  {
				  current.push_front((*p));	//add the actor to the list
			  }
			  p++;
		  }
	  }
	  else
	  {
		  old = list<Actor*>();	//if there is no actor on the water, empty the old list that indicate the actors that are previously on the water
		  return;
	  }

	  bool have = false;
	  if (!current.empty())	//if we collect the actors on water
	  {
		  list<Actor*>::iterator p = current.begin();
		  while (p != current.end())	//go through the actors that are currently on water
		  {		 
			  list<Actor*>::iterator k = old.begin();
			  while (k != old.end())	//match the actor with the old list to see if they were previously on the list
			  {
				  if ((*p) == (*k))
				  {
					  have = true;
				  }
				  k++;
			  }

			  if (have == false)	//if the actor was not on the list, stun it, otherwise, don't 
				  (*p)->setStun(2);
			  else
				  have = false;
			  p++;
		  }
		  old = current;
	  }
	  else
	  {
		  old = list<Actor*>();
		  return;
	  }
  }

  void StudentWorld::poisonactors(int x, int y)
  {
	  if (!actors[x][y].empty())
	  {
		  list<Actor*>::iterator p = actors[x][y].begin();
		  while (p != actors[x][y].end())	//go through the actors that are on the posion
		  {
			  if ((*p)->getiden() == "Ant_3" || (*p)->getiden() == "Ant_2" || (*p)->getiden() == "Ant_1" || (*p)->getiden() == "Ant_0" || (*p)->getiden() == "BabyGrassHopper" )	//if they can be posioned
			  {
				  (*p)->changeHP(-150);	//posion them
				  if ((*p)->die())	//if they die, change them to food
					  changefood(x, y, 100);
			  }
			  p++;
		  }
	  }
  }

  void StudentWorld::addcount(string colony)
  {
	  if (colony == "Ant_0")
	  {
		  playerCount[0]++;
	  }
	  else if (colony == "Ant_1")
	  {
		  playerCount[1]++;
	  }
	  else if (colony == "Ant_2")
	  {
		  playerCount[2]++;
	  }
	  else if (colony == "Ant_3")
	  {
		  playerCount[3]++;
	  }
  }

  StudentWorld::~StudentWorld()
  {
	  cleanUp();
  }
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
