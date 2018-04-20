#include "clickibunti.h"
#include <GL/glew.h>
#include "Graphics2D.h"
#include "PerformanceMeter.h"
#include <iostream>
#include <iomanip>
#include "TexParamSet.h"
#include "TextureStatic2D.h"
#include "MatrixLib.h"
#include "MathModel.h"
#include "RobonovaGraphics.h"
#include <SFML/Network.hpp>

PerformanceMeter pm(6);
TexParamSet tps(2,2);
TextureStatic2D * t2D;
//Graphics2D * g;
MathModel * myModel=new MathModel();
RobonovaGraphics rob(myModel);
RobonovaGraphics * robG;
sf::UdpSocket * socket;


#include "Zombie_KeyInput.h"
#include "Zombie_MouseInput.h"
Zombie_KeyInput * keyInput=0;
Zombie_MouseInput * mouseInput=0;

int enabledProgram = 1;
int maxPrograms=2;


#include "Zombie_World.h"
Zombie_World * world;

//this is a test-everything-file, chaos is normal


void setAxis(int axis, float degreeValue)
{
	switch (axis)
	{
	case 0:
		myModel->left.a0 = degreeValue;
		break;
	case 1:
		myModel->left.a1 = degreeValue;
		break;
	case 2:
		myModel->left.a2 = degreeValue - 30;
		break;
	case 3:
		myModel->left.a3 = degreeValue;
		break;
	case 4:
		myModel->left.a4 = -90-degreeValue;
		break;
	case 5:
		myModel->right.a0 = degreeValue;
		break;
	case 6:
		myModel->right.a1 = degreeValue;
		break;
	case 7:
		myModel->right.a2 = degreeValue - 30;
		break;
	case 8:
		myModel->right.a3 = degreeValue;
		break;
	case 9:
		myModel->right.a4 = -90-degreeValue;
		break;
	case 10:
		myModel->leftArm.a0 = degreeValue;
		break;
	case 11:
		myModel->leftArm.a1 = degreeValue;
		break;
	case 12:
		myModel->leftArm.a2 = degreeValue;
		break;
	case 13:
		myModel->rightArm.a0 = degreeValue-90;
		break;
	case 14:
		myModel->rightArm.a1 = degreeValue;
		break;
	case 15:
		myModel->rightArm.a2 = degreeValue;
		break;
	}
}

int getAxisID(int servoID)
{
	switch (servoID)
	{
	case 0x0C:
		return 0;
	case 0x1E:
		return 1;
	case 0x01:
		return 2;
	case 0x03:
		return 3;
	case 0x17:
		return 4;
	case 0x08:
		return 5;
	case 0x0E:
		return 6;
	case 0x0A:
		return 7;
	case 0x20:
		return 8;
	case 0x10:
		return 9;
	case 0x19:
		return 10;
	case 0x1F:
		return 11;
	case 0x12:
		return 12;
	case 0x04:
		return 13;
	case 0x05:
		return 14;
	case 0x15:
		return 15;
	
	}
	return -1;
}


int test1 = 0;
int test2 = 0;
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
	else if(enabledProgram==0)//robot
	{
		if (test2 == 0)
		{
			tps.addI(GL_TEXTURE_WRAP_S, GL_REPEAT);
			tps.addI(GL_TEXTURE_WRAP_T, GL_REPEAT);
			tps.addF(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			tps.addF(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			t2D = new TextureStatic2D(&tps, "./res/robonova.png");
			t2D->update();
			rob.tex = t2D;
			test2 = 1;
			socket = new sf::UdpSocket();
			if (socket->bind(54000) != sf::Socket::Done)
			{
				// error...
				std::cout << "socket.bind error" << std::endl;
			}
			else
			{
				socket->setBlocking(false);
				std::cout << "socket.bind successful" << std::endl;
			}
			robG = &rob;
		}
		char * data = new char[300];
		std::size_t received;

		sf::IpAddress sender;
		unsigned short port;

		int datasize = 200;
		if (socket->receive(data, datasize, received, sender, port) != sf::Socket::Done)
		{
			// error...
		}
		if (received)
		{
			for (int i = 0; i < (int)(received - 8); i++)
			{

				if ((data[i + 0] == 'P') && (data[i + 1] == 'O') && (data[i + 2] == 'S'))
				{
					char idA = data[i + 3];
					char idB = data[i + 4];
					char posM = data[i + 6];
					char pos2 = data[i + 7];
					char posL = data[i + 8];
					int ID = 0;
					if ((idA >= '0') && (idA <= '9')) ID += (idA - '0') << 4;
					if ((idA >= 'A') && (idA <= 'F')) ID += (idA - 'A' + 10) << 4;
					if ((idB >= '0') && (idB <= '9')) ID += (idB - '0');
					if ((idB >= 'A') && (idB <= 'F')) ID += (idB - 'A' + 10);
					int pos = 0;
					if ((posM >= '0') && (posM <= '9')) pos += (posM - '0') << 8;
					if ((posM >= 'A') && (posM <= 'F')) pos += (posM - 'A' + 10) << 8;
					if ((pos2 >= '0') && (pos2 <= '9')) pos += (pos2 - '0') << 4;
					if ((pos2 >= 'A') && (pos2 <= 'F')) pos += (pos2 - 'A' + 10) << 4;
					if ((posL >= '0') && (posL <= '9')) pos += (posL - '0');
					if ((posL >= 'A') && (posL <= 'F')) pos += (posL - 'A' + 10);

					std::cout << "update: ID=" << ID << "; pos=" << pos << std::endl;

					setAxis(getAxisID(ID), pos*180.0f / 1024.0f - 90);
				}
			}
			data[99] = 0;
			std::cout << "Received " << received << " bytes: " << data << std::endl;
		}

		delete data;
		rob.draw();
	}

	sf::Time t = sf::microseconds(1000);//TODO 1ms
	sf::sleep(t);
	glFlush();

}
