#include "RobonovaGraphics.h"
#include "CameraTP.h"

RobonovaGraphics::RobonovaGraphics(MathModel * myModel):
model(myModel)
{
	pm = new PerformanceMeter(1);
	cam = new CameraTP();
	cam->aspect = 1920.0f / 1080;
	//cam->posZ = 0;
	((CameraTP *)cam)->dist = 250;
	pm->reset();
}


RobonovaGraphics::~RobonovaGraphics()
{

}


void RobonovaGraphics::drawServo()
{
	glBegin(GL_QUADS);
	glColor3f(0.15f, 0.15f, 0.2f);
	glVertex3f(-10, 10, 23);
	glVertex3f(-10, 10, -23);//servo
	glVertex3f(10, 10, -23);
	glVertex3f(10, 10, 23);

	glVertex3f(-10, -30, 23);
	glVertex3f(-10, -30, -23);//servo
	glVertex3f(10, -30, -23);
	glVertex3f(10, -30, 23);


	glVertex3f(10, 10, 23);
	glVertex3f(-10, 10, 23);//servo
	glVertex3f(-10, -30, 23);
	glVertex3f(10, -30, 23);

	glVertex3f(10, 10, -23);
	glVertex3f(-10, 10, -23);//servo
	glVertex3f(-10, -30, -23);
	glVertex3f(10, -30, -23);
	glEnd();

	tex->bind();
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2f(1, 0);
	glVertex3f(10, 10, 23);
	glTexCoord2f(0, 0);
	glVertex3f(10, 10, -23);//servo
	glTexCoord2f(0, 1);
	glVertex3f(10, -30, -23);
	glTexCoord2f(1, 1);
	glVertex3f(10, -30, 23);

	glTexCoord2f(1, 0);
	glVertex3f(-10, 10, 23);
	glTexCoord2f(0, 0);
	glVertex3f(-10, 10, -23);//servo
	glTexCoord2f(0, 1);
	glVertex3f(-10, -30, -23);
	glTexCoord2f(1, 1);
	glVertex3f(-10, -30, 23);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void RobonovaGraphics::drawArm(bool left, arm armAngleData)
{
	glPushMatrix();
	glTranslatef(0, 46, 0);
	if (left) glRotatef(90, 1, 0, 0);
	else glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 49 - 20, 0);
	drawServo();
	//z 49mm
	//y 46mm
	glTranslatef(0,32, 0);
	glPopMatrix();

	//a0
	glTranslatef(0, 46, 0);
	if (left) glRotatef(90, 1, 0, 0);
	else glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 49, 0);
	glRotatef(left ? armAngleData.a0 : -armAngleData.a0, 0, 1, 0);

	glPushMatrix();
	glTranslatef(0, 15, 0);

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
	glPopMatrix();



	glTranslatef(0, 20, 0);//TODO check with robonova
	//a1
	glRotatef(left ? armAngleData.a1 : -armAngleData.a1, 1, 0, 0);


	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 15, 0);
	drawServo();
	glPopMatrix();



	glPushMatrix();
	//glRotatef(180, 0, 0, 1);
	glTranslatef(0.0f, 47.75, 0);
	glBegin(GL_QUADS);
	glColor3f(233 / 255.0, 140 / 255.0, 0);
	glVertex3f(25, -35, 10);
	glVertex3f(25, 10, 10);//crossA
	glVertex3f(25, 10, -10);
	glVertex3f(25, -35, -10);

	glVertex3f(-25, -35, 10);
	glVertex3f(-25, 10, 10);//crossB
	glVertex3f(-25, 10, -10);
	glVertex3f(-25, -35, -10);
	glEnd();
	glPopMatrix();

	//translate, but not rotate
	glTranslatef(0.0f, 47.75f, 0);
	//a2
	glRotatef(left ? armAngleData.a2 : -armAngleData.a2, 1, 0, 0);






	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	drawServo();
	glPopMatrix();




	//z 25mm bis a1

	//z 4xmm wie a23



	//body platte 45mm y

	//50mm hand z
	//39mm hand y

}


void RobonovaGraphics::drawFoot(bool left, foot footAngleData)
{

	glPushMatrix();
	glTranslatef(0, 0, left?28.25f:-28.25f);
	glRotatef(180, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	drawServo();
	glPopMatrix();

	//a4 nope a0
	glTranslatef(0.0f, 0.0f, left ? -28.25f : 28.25f);//body->right leg
	glRotatef(footAngleData.a0, 1.0f, 0.0f, 0.0f);

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

	glTranslatef(0, -40, 0);
	glRotatef(180, 1, 0, 0);
	glRotatef(90, 0, 1, 0);

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
	glPopMatrix();

	//a3 nope a1
	glTranslatef(0.0f, -40.63f, 0);
	glRotatef(footAngleData.a1, 0.0f, 0.0f, 1.0f);
	
	drawServo();
	glBegin(GL_QUADS);
	glColor3f(233 / 255.0, 140 / 255.0, 0);
	glVertex3f(10.1, -50, 25);
	glVertex3f(10.1, -50, -25);//kneeA
	glVertex3f(10.1, -20, -25);
	glVertex3f(10.1, -20, 25);


	glVertex3f(10.1, -50, 25);
	glVertex3f(10.1, -15, 25);//kneeB
	glVertex3f(-10, -15, 25);
	glVertex3f(-10, -50, 25);

	glVertex3f(10.1, -50, -25);
	glVertex3f(10.1, -15, -25);//kneeC
	glVertex3f(-10, -15, -25);
	glVertex3f(-10, -50, -25);
	glEnd();

	
	//translate, but not rotate
	glRotatef(-28, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, -57.0f, 0);
	glRotatef(-32, 0.0f, 0.0f, 1.0f);



	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	drawServo();
	glPopMatrix();


	//a2
	glRotatef(footAngleData.a2, 0.0f, 0.0f, 1.0f);

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glBegin(GL_QUADS);
	glColor3f(233 / 255.0, 140 / 255.0, 0);
	glVertex3f(25, -35, 10);
	glVertex3f(25, 10, 10);//crossA
	glVertex3f(25, 10, -10);
	glVertex3f(25, -35, -10);

	glVertex3f(-25, -35, 10);
	glVertex3f(-25, 10, 10);//crossB
	glVertex3f(-25, 10, -10);
	glVertex3f(-25, -35, -10);
	glEnd();
	glPopMatrix();

	//translate, but not rotate
	glTranslatef(0.0f, -47.75f, 0);



	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	drawServo();
	glPopMatrix();

	//a1 nope a3
	glRotatef(footAngleData.a3, 0.0f, 0.0f, 1.0f);


	glPushMatrix();
	glRotatef(90, 0, 1, 0);


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

	glTranslatef(0, -40, 0);
	glRotatef(180, 1, 0, 0);
	glRotatef(90, 0, 1, 0);

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
	glPopMatrix();
	glPopMatrix();

	//a0 nope a4
	glTranslatef(0.0f, -40.5f, 0);
	glRotatef(footAngleData.a4, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, -11.75f, 0);

	glBegin(GL_QUADS);
	glColor3f(233 / 255.0, 140 / 255.0, 0);
	glVertex3f(-50, -11.75f, left ? 22.5f : -22.5f);//left foot, otherwise invert z
	glVertex3f(-50, -11.75f, left ? -37.5f : 37.5f);//crossC
	glVertex3f(50, -11.75f, left ? -37.5f : 37.5f);
	glVertex3f(50, -11.75f, left ? 22.5f : -22.5f);
	glEnd();

	glPushMatrix();
	glRotatef(left ? 90:-90, 1, 0, 0);//right foot, otherwise invert angle
	glRotatef(90, 0, 1, 0);
	drawServo();
	glPopMatrix();
	
}
void RobonovaGraphics::draw()
{
	static float rot = 0;

	/*float aspect = 1920 / 1080.0f;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-aspect,aspect,-1,1,1,1024*1024);

	glMatrixMode(GL_MODELVIEW);

	//glTranslatef(0, 0.3f, 0.5f);
	//glScalef(0.003f, aspect*0.003f, 0.00001f); //cheaty almost-camera
	glTranslatef(0, 0, -180);
	glRotatef(20, 1, 0, 0);
	glRotatef(rot, 0, 1, 0);*/

	if (rotEn)
	{
		cam->alpha = 20;
		cam->beta = rot;
		cam->gamma = 0;

		pm->registerTime(0);
		float time = pm->getTime(0);
		rot += 0.00006f*time;

	}

	cam->applyFresh();

	glPushMatrix();



	glBegin(GL_QUADS);
	glColor3f(210 / 255.0, 120 / 255.0, 0);
	glVertex3f(24, -10, 40);
	glVertex3f(24, -10, -40);//body1
	glVertex3f(24, 80, -40);
	glVertex3f(24, 80, 40);


	glVertex3f(-24, -10, 40);
	glVertex3f(-24, -10, -40);//body2
	glVertex3f(-24, 80, -40);
	glVertex3f(-24, 80, 40);

	glColor3f(248 / 255.0, 158 / 255.0, 0);
	glVertex3f(28, -10, 15);
	glVertex3f(28, -10, -15);//body3
	glVertex3f(28, 80, -15);
	glVertex3f(28, 80, 15);

	glColor3f(0.1f, 0.4f, 0.2f);
	glVertex3f(-30, 0, 30);
	glVertex3f(-30, 0, -30);//circuit board
	glVertex3f(-30, 60, -30);
	glVertex3f(-30, 60, 30);

	glEnd();
	drawFoot(true, model->left);
	glPopMatrix();
	glPushMatrix();
	drawFoot(false, model->right);
	glPopMatrix();
	glEnd();
	glPushMatrix();
	drawArm(false, model->leftArm);
	glPopMatrix();
	glPushMatrix();
	drawArm(true, model->rightArm);
	glPopMatrix();
}
