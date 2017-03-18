#include "Zombie_Projectile.h"

#include "MatrixLib2.h"
Zombie_Projectile::Zombie_Projectile(ICamera3D * cam, Zombie_Gun * gun, ITexture * t) :
firedFrom(gun), tex(t)
{
	pos.x = cam->posX;
	pos.y = cam->posY;
	pos.z = cam->posZ;
	posOld = pos;
	float velZ = -gun->projectileSpeed;// 100000;
	float velY = 0;
	float velX = 0;
	MatrixLib2 ml(2);
	ml.loadIdentity();
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

bool Zombie_Projectile::move(float sec)
{
	bool destroy = posOld.y < 0;
	posOld = pos;
	pos+=v*sec;
	v.y -= 9810 * sec;
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
	glVertex3f(0, -100, -100);

	glTexCoord2f(1, 0);
	glVertex3f(0, 100, -100);

	glTexCoord2f(1, 1);
	glVertex3f(0, 100, 100);

	glTexCoord2f(0, 1);
	glVertex3f(0, -100, 100);


	glTexCoord2f(0, 0);
	glVertex3f(-100, 0, -100);

	glTexCoord2f(1, 0);
	glVertex3f(100, 0, -100);

	glTexCoord2f(1, 1);
	glVertex3f(100, 0, 100);

	glTexCoord2f(0, 1);
	glVertex3f(-100, 0, 100);


	glTexCoord2f(0, 0);
	glVertex3f(-100, -100,0);

	glTexCoord2f(1, 0);
	glVertex3f(100, -100,0);

	glTexCoord2f(1, 1);
	glVertex3f(100, 100,0);

	glTexCoord2f(0, 1);
	glVertex3f(-100, 100,0);


	glEnd();
	glPopMatrix();
}
