#include "Zombie_Projectile.h"

#include "MatrixLib2.h"
#include <cstdlib>

Zombie_Projectile::Zombie_Projectile(ICamera3D * cam, ITexture * t,Zombie_AmmoType * type):
pType(type), tex(t)
{
	pos.x = cam->posX;
	pos.y = cam->posY;
	pos.z = cam->posZ;
	posOld = pos;
	float velZ = -type->speed;
	float velY = 0;
	float velX = 0;
	float rand3=(rand()%1024)/512.0f-512.0f;//TODO remove
	MatrixLib2 ml(2);
	ml.loadIdentity();

	ml.rotatef(rand3*180, 0, 0, 1);//TODO remove, test only

	ml.rotatef(-cam->beta, 0, 1, 0);
	ml.rotatef(-cam->alpha, 1, 0, 0);
	ml.rotatef(-cam->gamma, 0, 0, 1);
	v.x = ml.curMatrix[0] * velX + ml.curMatrix[4] * velY + ml.curMatrix[8] * velZ;
	v.y = ml.curMatrix[1] * velX + ml.curMatrix[5] * velY + ml.curMatrix[9] * velZ;
	v.z = ml.curMatrix[2] * velX + ml.curMatrix[6] * velY + ml.curMatrix[10] * velZ;

}


Zombie_Projectile::~Zombie_Projectile()
{

}

bool Zombie_Projectile::move(float sec,ChunkManager * cm)
{
	bool destroy = posOld.y < cm->getHeight(posOld.x,posOld.z);
	posOld = pos;
	vec3 vOld=v;
	v.y -= 9.81f * sec;
	flt spdSq=v.lengthSq();
	flt drag=spdSq*pType->drag;
	vec3 dragDir=v;
	dragDir.normalize();
	v-=dragDir*drag*sec;
	pos+=(v+vOld)*sec*0.5f;
	return destroy;
}

void Zombie_Projectile::draw()
{
	tex->bind();
	glColor3f(1.0f,1.0f, 0.1f);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glBegin(GL_QUADS);


	glTexCoord2f(0, 0);
	glVertex3f(0, -0.1f, -0.1f);

	glTexCoord2f(1, 0);
	glVertex3f(0, 0.1f, -0.1f);

	glTexCoord2f(1, 1);
	glVertex3f(0,  0.1f,  0.1f);

	glTexCoord2f(0, 1);
	glVertex3f(0, -0.1f,  0.1f);


	glTexCoord2f(0, 0);
	glVertex3f(-0.1f, 0, -0.1f);

	glTexCoord2f(1, 0);
	glVertex3f(0.1f, 0, -0.1f);

	glTexCoord2f(1, 1);
	glVertex3f(0.1f, 0, 0.1f);

	glTexCoord2f(0, 1);
	glVertex3f(-0.1f, 0, 0.1f);


	glTexCoord2f(0, 0);
	glVertex3f(-0.1f, -0.1f,0);

	glTexCoord2f(1, 0);
	glVertex3f(0.1f, -0.1f,0);

	glTexCoord2f(1, 1);
	glVertex3f(0.1f, 0.1f,0);

	glTexCoord2f(0, 1);
	glVertex3f(-0.1f, 0.1f,0);


	glEnd();
	glPopMatrix();
}
