#include "RobonovaGraphics.h"


RobonovaGraphics::RobonovaGraphics(MathModel * myModel):
model(myModel)
{

}


RobonovaGraphics::~RobonovaGraphics()
{

}


void RobonovaGraphics::drawServo()
{
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex3f(-10, 10, 23);
	glVertex3f(-10, 10, -23);//servo
	glVertex3f(10, 10, -23);
	glVertex3f(10, 10, 23);

	glVertex3f(-10, -20, 23);
	glVertex3f(-10, -20, -23);//servo
	glVertex3f(10, -20, -23);
	glVertex3f(10, -20, 23);


	glVertex3f(10, 10, 23);
	glVertex3f(-10, 10, 23);//servo
	glVertex3f(-10, -20, 23);
	glVertex3f(10, -20, 23);

	glVertex3f(10, 10, -23);
	glVertex3f(-10, 10, -23);//servo
	glVertex3f(-10, -20, -23);
	glVertex3f(10, -20, -23);
	glEnd();

	tex->bind();
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex3f(10, 10, 23);
	glTexCoord2f(0, 0);
	glVertex3f(10, 10, -23);//servo
	glTexCoord2f(0, 1);
	glVertex3f(10, -20, -23);
	glTexCoord2f(1, 1);
	glVertex3f(10, -20, 23);
	glTexCoord2f(1, 0);

	glVertex3f(-10, 10, 23);
	glTexCoord2f(0, 0);
	glVertex3f(-10, 10, -23);//servo
	glTexCoord2f(0, 1);
	glVertex3f(-10, -20, -23);
	glTexCoord2f(1, 1);
	glVertex3f(-10, -20, 23);
	glTexCoord2f(1, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void RobonovaGraphics::drawFoot(bool left, foot footAngleData)
{

	glPushMatrix();
	glTranslatef(0, 0, 28.25f);
	glRotatef(180, 1, 0, 0);
	drawServo();
	glPopMatrix();

	//a4

	glBegin(GL_QUADS);
	glColor3f(233 / 255.0, 140 / 255.0, 0);
	glVertex3f(25, -20, 10);
	glVertex3f(25, 10, 10);//crossA
	glVertex3f(25, 10, -10);
	glVertex3f(25, -20, -10);

	glVertex3f(-25, -20, 10);
	glVertex3f(-25, 10, 10);//crossB
	glVertex3f(-25, 10, -10);
	glVertex3f(-25, -20, -10);

	glVertex3f(-25, -20, 10);
	glVertex3f(-25, -20, -10);//crossC
	glVertex3f(25, -20, -10);
	glVertex3f(25, -20, 10);
	glEnd();

	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(233 / 255.0, 140 / 255.0, 0);
	glTranslatef(0, -20, 0);
	glRotatef(180, 1, 0, 0);
	glRotatef(90, 0, 1, 0);

	glVertex3f(25, -20, 10);
	glVertex3f(25, 10, 10);//crossA
	glVertex3f(25, 10, -10);
	glVertex3f(25, -20, -10);

	glVertex3f(-25, -20, 10);
	glVertex3f(-25, 10, 10);//crossB
	glVertex3f(-25, 10, -10);
	glVertex3f(-25, -20, -10);

	glVertex3f(-25, -20, 10);
	glVertex3f(-25, -20, -10);//crossC
	glVertex3f(25, -20, -10);
	glVertex3f(25, -20, 10);
	glEnd();
	glPopMatrix();

	//a3

	drawServo();
	glBegin(GL_QUADS);
	glColor3f(233 / 255.0, 140 / 255.0, 0);
	glVertex3f(10, -50, 25);
	glVertex3f(10, -50, -25);//kneeA
	glVertex3f(10, -20, -25);
	glVertex3f(10, -20, 25);


	glVertex3f(10, -50, 25);
	glVertex3f(10, -15, 25);//kneeB
	glVertex3f(-10, -15, 25);
	glVertex3f(-10, -50, 25);

	glVertex3f(10, -50, -25);
	glVertex3f(10, -15, -25);//kneeC
	glVertex3f(-10, -15, -25);
	glVertex3f(-10, -50, -25);
	glEnd();


	//translate, but not rotate
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	drawServo();
	glPopMatrix();
	//a2


	glBegin(GL_QUADS);
	glColor3f(233 / 255.0, 140 / 255.0, 0);
	glVertex3f(25, -20, 10);
	glVertex3f(25, 10, 10);//crossA
	glVertex3f(25, 10, -10);
	glVertex3f(25, -20, -10);

	glVertex3f(-25, -20, 10);
	glVertex3f(-25, 10, 10);//crossB
	glVertex3f(-25, 10, -10);
	glVertex3f(-25, -20, -10);
	glEnd();


	//translate, but not rotate
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	drawServo();
	glPopMatrix();
	//a1


	glBegin(GL_QUADS);
	glColor3f(233 / 255.0, 140 / 255.0, 0);
	glVertex3f(25, -20, 10);
	glVertex3f(25, 10, 10);//crossA
	glVertex3f(25, 10, -10);
	glVertex3f(25, -20, -10);

	glVertex3f(-25, -20, 10);
	glVertex3f(-25, 10, 10);//crossB
	glVertex3f(-25, 10, -10);
	glVertex3f(-25, -20, -10);

	glVertex3f(-25, -20, 10);
	glVertex3f(-25, -20, -10);//crossC
	glVertex3f(25, -20, -10);
	glVertex3f(25, -20, 10);
	glEnd();

	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(233 / 255.0, 140 / 255.0, 0);
	glTranslatef(0, -20, 0);
	glRotatef(180, 1, 0, 0);
	glRotatef(90, 0, 1, 0);

	glVertex3f(25, -20, 10);
	glVertex3f(25, 10, 10);//crossA
	glVertex3f(25, 10, -10);
	glVertex3f(25, -20, -10);

	glVertex3f(-25, -20, 10);
	glVertex3f(-25, 10, 10);//crossB
	glVertex3f(-25, 10, -10);
	glVertex3f(-25, -20, -10);

	glVertex3f(-25, -20, 10);
	glVertex3f(-25, -20, -10);//crossC
	glVertex3f(25, -20, -10);
	glVertex3f(25, -20, 10);
	glEnd();
	glPopMatrix();

	//a0

	glBegin(GL_QUADS);
	glColor3f(233 / 255.0, 140 / 255.0, 0);
	glVertex3f(-50, -11.75f, 22.5);//left foot, otherwise invert z
	glVertex3f(-50, -11.75f, -37.5);//crossC
	glVertex3f(50, -11.75f, -37.5);
	glVertex3f(50, -11.75f, 22.5);
	glEnd();

	glPushMatrix();
	glRotatef(90, 1, 0, 0);//right foot, otherwise invert angle
	drawServo();
	glPopMatrix();
}

void RobonovaGraphics::draw()
{
	//TODO apply camera


	glBegin(GL_QUADS);
	glColor3f(233 / 255.0, 140 / 255.0, 0);
	glVertex3f(20, -10, 40);
	glVertex3f(20, -10, -40);//body1
	glVertex3f(20, 80, -40);
	glVertex3f(20, 80, 40);

	glVertex3f(-20, -10, 40);
	glVertex3f(-20, -10, -40);//body2
	glVertex3f(-20, 80, -40);
	glVertex3f(-20, 80, 40);
	glEnd();
	drawFoot(true, model->left);
	drawFoot(false, model->right);
}
