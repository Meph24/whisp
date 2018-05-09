#include "Zombie_Physics.h"


Zombie_Physics::Zombie_Physics(int maxObjects):
curMax(0),totalMax(maxObjects)
{
	xPush = new float[totalMax * 6];
	zPush = xPush + totalMax;
	xObj = xPush + totalMax*2;
	zObj = xPush + totalMax * 3;
	force = xPush + totalMax * 4;
	radius = xPush + totalMax * 5;
	pos = new int[totalMax];
}


Zombie_Physics::~Zombie_Physics()
{
	delete[] xPush;
	delete[] pos;
}

bool Zombie_Physics::registerObject(int ID, float objForce, float xO, float zO,float rad)
{
	if (curMax == totalMax) return false;
	pos[ID] = curMax;
	force[curMax] = objForce;
	xObj[curMax] = xO;
	zObj[curMax] = zO;
	radius[curMax] = rad;
	curMax++;
	return true;
}

Zombie_Physics::motion Zombie_Physics::getMotion(int ID, float time)
{
	int index = pos[ID];
	motion m;
	m.x = xPush[index]*time;
	m.z = zPush[index]*time;
	return m;
}

#include <cmath>

#include <iostream>
void Zombie_Physics::doPushPhysics()
{
	for (int i = 0; i < curMax; i++)
	{
		xPush[i] = 0;
		zPush[i] = 0;
	}

#pragma omp parallel for schedule(dynamic, 1)
	for (int i = 0; i < curMax; i++)
	{
		for (int j = i+1; j < curMax; j++)
		{
			float difX = xObj[i] - xObj[j];
			float difZ = zObj[i] - zObj[j];
			float dist;

			if ((dist = (difX*difX + difZ*difZ)) < (radius[i] + radius[j])*(radius[i] + radius[j]))
			{
				//std::cout <<"i"<<i<<"j"<<j<< " dist: " << dist << " difX: " << difX << " difZ: " << difZ << std::endl;
				float minR = radius[i] < radius[j] ? radius[i] : radius[j];
				dist = sqrt(dist);

				float totRad = radius[i] + radius[j];
				float f;
				if (dist < (totRad - minR*0.975f)) f = 40 * (force[i] + force[j]);
				else f = minR / (dist - (totRad - minR))*(force[i] + force[j]);
				//std::cout << "f: " << f << std::endl;
				if (dist < (totRad / 1024.0f))
				{
					difX = (totRad / 1024.0f);
					dist = (totRad / 1024.0f);
				}
				float xP = f*difX / dist;
				float zP = f*difZ / dist;
				xPush[i] += xP;
				xPush[j] -= xP;
				zPush[i] += zP;
				zPush[j] -= zP;
			}
		}
	}
	curMax = 0;
}
#include "MatrixLib2.h"
Zombie_Physics::hit Zombie_Physics::testHitbox(MatrixLib2 * ml,float xFrom, float xTo, float yFrom, float yTo, float zFrom, float zTo,spacevec relPos)
{
	hit ret;
	ret.projectileIndex = -1;
	ml->pushMatrix();
	ml->translatef(xFrom, yFrom, zFrom);
	ml->scalef(xTo - xFrom, yTo - yFrom, zTo - zFrom);
	mat4 matOut;
	ml->invertMatrix(ml->curMatrix, matOut.mat);
	for (int i = 0; i < pCount[0]; i++)
	{
		if (projectiles[i])
		{
			vec3 p1 = matOut* (cm->toMeters(projectiles[i]->posOld-relPos));
			vec3 p2 = matOut*(cm->toMeters(projectiles[i]->pos-relPos));
			//projectile now relative to cube, where cube is at 0-1 on all 3 axis
			float vx = p2.x - p1.x;//TODO check for 0
			float vy = p2.y - p1.y;
			float vz = p2.z - p1.z;
			float x0 = -p1.x / vx;
			float x1 = (1 - p1.x) / vx;
			if (x1 < x0)
			{
				float temp = x0;
				x0 = x1;
				x1 = temp;
			}
			float y0 = -p1.y / vy;
			float y1 = (1 - p1.y) / vy;
			if (y1 < y0)
			{
				float temp = y0;
				y0 = y1;
				y1 = temp;
			}
			float z0 = -p1.z / vz;
			float z1 = (1 - p1.z) / vz;
			if (z1 < z0)
			{
				float temp = z0;
				z0 = z1;
				z1 = temp;
			}
			
			float minC = x0 > y0 ? x0 : y0;
			minC = minC > z0 ? minC : z0;// max(x0, y0, z0);
			float maxC = x1 < y1 ? x1 : y1;
			maxC = maxC < z1 ? maxC : z1;// min(x1, y1, z1);
			if ((minC<1)&&(maxC>0)&&(minC < maxC))
			{
				//TODO multiple projectiles, projectile sorting
				ret.projectileIndex = i;
				ret.time = minC;
				break;
			}
		}
	}

	ml->popMatrix();
	return ret;
}

void pushEntities(Pushable* a, Pushable* b, float time,ChunkManager * cm)
{
	spacevec dif=a->getPos()-b->getPos();
	float difX = cm->toMeters(dif.x);
	float difZ = cm->toMeters(dif.z);
	float dist;

	float totRad =  a->pushRadius + b->pushRadius;
	float totForce= a->pushForce + b->pushForce;

	if ((dist = (difX*difX + difZ*difZ)) < totRad*totRad)
	{
		float minR = a->pushRadius < b->pushRadius ?  a->pushRadius : b->pushRadius;
		dist = sqrt(dist);

		float f;
		if (dist < (totRad - minR*0.975f)) f = 40 * totForce;
		else f = minR / (dist - (totRad - minR))*totForce;
		if (dist < (totRad / 1024.0f))
		{
			difX = (totRad / 1024.0f);
			dist = (totRad / 1024.0f);
		}
		f/=dist;
		float xP = f*difX;
		float zP = f*difZ;

		spacevec move=cm->fromMeters(vec3(xP,0,zP)*(time*1));//amplification factor here
		a->push(move);
		b->push(-move);
	}
}
