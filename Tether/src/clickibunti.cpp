#include "clickibunti.h"

#include "Zombie_KeyInput.h"
#include "Zombie_MouseInput.h"
Zombie_KeyInput * keyInput=0;
Zombie_MouseInput * mouseInput=0;

int enabledProgram = 1;

//#include "Zombie_World.h"
//Zombie_World * world;

#include "Simulation_World.h"
Simulation_World * world;

int test1 = 0;
void clickibunti()
{
	enabledProgram%=2;
	if (enabledProgram==1)//zombie
	{
		if (test1 == 0)
		{
			test1 = 1;
			world->loadStandardTex();
			srand(time(0));
		}
		world->loop();
	}
}
