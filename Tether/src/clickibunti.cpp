#include "clickibunti.h"
#include <GL/glew.h>
#include "Graphics2D.h"
#include "PerformanceMeter.h"
#include <iostream>
#include <iomanip>
#include "TexParamSet.h"
#include "TextureStatic2D.h"
#include "MatrixLib.h"

PerformanceMeter pm(6);
TexParamSet tps(2,2);
TextureStatic2D * t2D;
//Graphics2D * g;


#include "Zombie_KeyInput.h"
#include "Zombie_MouseInput.h"
Zombie_KeyInput * keyInput=0;
Zombie_MouseInput * mouseInput=0;

int enabledProgram = 1;
int maxPrograms=2;


#include "Zombie_World.h"
Zombie_World * world;


int test1 = 0;
void clickibunti()
{
	glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
	

	enabledProgram%=maxPrograms;

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


	sf::Time t = sf::microseconds(1000);//TODO enable/disable depending on framerate
	sf::sleep(t);
	glFlush();

}
