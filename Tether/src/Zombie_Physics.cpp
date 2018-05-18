#include "Zombie_Physics.h"


Zombie_Physics::Zombie_Physics(int maxObjects)
{
}


Zombie_Physics::~Zombie_Physics()
{
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

