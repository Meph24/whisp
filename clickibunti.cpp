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
PerformanceMeter pm(6);
TexParamSet tps(2,2);
TextureStatic2D * t2D;
Graphics2D * g;
MathModel * model=new MathModel();
RobonovaGraphics rob(model);

int test = 0;
void clickibunti()
{
	glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
	g->beginDraw();
	

	/*glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex2f(-0.8f, 0.1f);     // Define vertices in counter-clockwise (CCW) order
	glVertex2f(-0.2f, 0.1f);     //  so that the normal (front-face) is facing you
	glVertex2f(-0.2f, 0.7f);
	glVertex2f(-0.8f, 0.7f);

	glColor3f(0.0f, 1.0f, 0.0f); // Green
	glVertex2f(-0.7f, -0.6f);
	glVertex2f(-0.1f, -0.6f);
	glVertex2f(-0.1f, 0.0f);
	glVertex2f(-0.7f, 0.0f);

	glColor3f(0.2f, 0.2f, 0.2f); // Dark Gray
	glVertex2f(-0.9f, -0.7f);
	glColor3f(1.0f, 1.0f, 1.0f); // White
	glVertex2f(-0.5f, -0.7f);
	glColor3f(0.2f, 0.2f, 0.2f); // Dark Gray
	glVertex2f(-0.5f, -0.3f);
	glColor3f(1.0f, 1.0f, 1.0f); // White
	glVertex2f(-0.9f, -0.3f);
	glEnd();

	glBegin(GL_TRIANGLES);          // Each set of 3 vertices form a triangle
	//glColor3f(0.0f, 0.0f, 1.0f); // Blue
	glVertex2f(0.1f, -0.6f);
	glVertex2f(0.7f, -0.6f);
	glVertex2f(0.4f, -0.1f);

	//glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex2f(0.3f, -0.4f);
	//glColor3f(0.0f, 1.0f, 0.0f); // Green
	glVertex2f(0.9f, -0.4f);
	//glColor3f(0.0f, 0.0f, 1.0f); // Blue
	glVertex2f(0.6f, -0.9f);
	glEnd();

	glBegin(GL_POLYGON);            // These vertices form a closed polygon
	glColor3f(1.0f, 1.0f, 0.0f); // Yellow
	glVertex2f(0.4f, 0.2f);
	glVertex2f(0.6f, 0.2f);
	glVertex2f(0.7f, 0.4f);
	glVertex2f(0.6f, 0.6f);
	glVertex2f(0.4f, 0.6f);
	glVertex2f(0.3f, 0.4f);
	glEnd();

	glFlush();  // Render now*/
	if (test == 0)
	{
		g = new Graphics2D(256);

		if(!tps.addI(GL_TEXTURE_WRAP_S, GL_REPEAT)) std::cout<<"fuckshit"<<std::endl;
		if (!tps.addI(GL_TEXTURE_WRAP_T, GL_REPEAT)) std::cout << "fuckshit" << std::endl;
		if (!tps.addF(GL_TEXTURE_MIN_FILTER, GL_NEAREST)) std::cout << "fuckshit" << std::endl;
		if (!tps.addF(GL_TEXTURE_MAG_FILTER, GL_NEAREST)) std::cout << "fuckshit" << std::endl;

		t2D = new TextureStatic2D(&tps, "./res/robonova.png");
		t2D->update();

		rob.tex = t2D;
		//g.font->update();
		//g.font = t2D;
		//test = 1;
		//glViewport(0,0,320,320);
		test = 1;
	}
	rob.draw();
	int mov = (rand() % 2)*2-1;
	int r=rand()%16;
	switch (r)
	{
	case 0:
		model->left.a0 += mov;
		if (model->left.a0 < -2) model->left.a0 = -2;
		break;
	case 1:
		model->left.a1 += mov;
		break;
	case 2:
		model->left.a2 += mov;
		break;
	case 3:
		model->left.a3 += mov;
		break;
	case 4:
		model->left.a4 += mov;
		break;
	case 5:
		model->right.a0 += mov;
		if (model->right.a0 > 2) model->right.a0 = 2;
		break;
	case 6:
		model->right.a1 += mov;
		break;
	case 7:
		model->right.a2 += mov;
		break;
	case 8:
		model->right.a3 += mov;
		break;
	case 9:
		model->right.a4 += mov;
		break;
	case 10:
		model->rightArm.a0 += mov;
		break;
	case 11:
		model->leftArm.a0 += mov;
		break;
	case 12:
		model->rightArm.a1 += mov;
		break;
	case 13:
		model->leftArm.a1 += mov;
		break;
	case 14:
		model->rightArm.a2 += mov;
		break;
	case 15:
		model->leftArm.a2 += mov;
		break;
	}
	glLoadIdentity();
	float aspect = 1920 / 1080.0f;
	glScalef(1 / aspect, 1, 1);
	g->drawString("test", 4, -0.8f, 0.9f, 0.1f);

	sf::Time t = sf::microseconds(6000);
	sf::sleep(t);




	//pm.registerTime(0);
	//sf::Time t = sf::microseconds(1000);
	//sf::sleep(t);
	//g.beginDraw();
	//pm.registerTime(1);
	//g.setColor(0.9f, 0.4f, 0.0f);
	//pm.registerTime(2);
	//g.fillOval(0.5f, 0.5f, 0.2f, 0.2f);
	//pm.registerTime(3);
	//pm.setSpikeHalfLifeTime(0.5f);
	//pm.useFastApproximation = false;
	//float posx = 0;
	//for (int i = 0; i < 6; i++)
	//{
	//	float now = pm.getSpikes(i)*144.0f/1000000.0f;
	//	color c;
	//	c.r = (i + 1) % 2;
	//	c.g = ((i + 1) / 2) % 2;
	//	c.b = ((i + 1) / 4) % 2;
	//	g.fillRect(posx-0.5f, 0, now, 0.2f,c);
	//	posx += now;
	//}posx = 0;
	//for (int i = 0; i < 6; i++)
	//{
	//	float now = pm.getTotalAVG(i)*144.0f / 1000000.0f;
	//	color c;
	//	c.r = (i + 1) % 2;
	//	c.g = ((i + 1) / 2) % 2;
	//	c.b = ((i + 1) / 4) % 2;
	//	g.fillRect(posx - 0.5f, 0.2f, now, 0.2f, c);
	//	posx += now;
	//}posx = 0;
	//for (int i = 0; i < 6; i++)
	//{
	//	float now = pm.getTime(i)*144.0f / 1000000.0f;
	//	color c;
	//	c.r = (i + 1) % 2;
	//	c.g = ((i + 1) / 2) % 2;
	//	c.b = ((i + 1) / 4) % 2;
	//	g.fillRect(posx - 0.5f, 0.4f, now, 0.2f, c);
	//	posx += now;
	//}posx = 0;
	//for (int i = 0; i < 1; i++)
	//{
	//	float now = pm.roundtriptime*144.0f / 1000000.0f;
	//	color c;
	//	c.r = (i + 1) % 2;
	//	c.g = ((i + 1) / 2) % 2;
	//	c.b = ((i + 1) / 4) % 2;
	//	g.fillRect(posx - 0.5f, 0.6f, now, 0.2f, c);
	//	posx += now;
	//}
	//
	//
	//
	//
	//subsection s = g.generateSubsection(0, 0, 0.4f, 0.4f, SNAP_MID,30);
	//g.setSubsection(&s);
	//glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	//g.fillOval(0, 0, 1, 1);
	//
	//t2D->bind();
	//glEnable(GL_TEXTURE_2D);
	//glBegin(GL_POLYGON);            // These vertices form a closed polygon
	//
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glTexCoord2f(1.0, 1.0);
	//glVertex2f(0.2f, 1);
	//glTexCoord2f(0.0, 1.0);
	//glVertex2f(-0.2f, 1);
	//glTexCoord2f(0.0, 0.0);
	//glVertex2f(-1, -1);
	//glTexCoord2f(1.0, 0.0);
	//glVertex2f(1,-1);
	//glEnd();
	//glDisable(GL_TEXTURE_2D);
	//subsection s2 = g.generateSubsection(0, 0, 0.8f, 0.8f, SNAP_W,-30);
	//
	//if (test == 245562)
	//{
	//	float * mat = s2.mat;
	//	test = 1;
	//	for (int i = 0; i < 4; i++)
	//	{
	//		std::cout << std::fixed << std::setprecision(3) << mat[i * 4] << "		" << mat[i * 4 + 1] << "		" << mat[i * 4 + 2] << "		" << mat[i * 4 + 3] << std::endl << std::endl;
	//	}
	//	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
	//}
	//g.setSubsection(&s2);
	//glColor4f(1.0f, 1.0f, 0.0f, 0.75f);
	//g.fillOval(0, 0, 1, 1);
	//
	//glEnable(GL_TEXTURE_2D);
	//glBegin(GL_QUADS);            // These vertices form a closed polygon
	//
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glTexCoord2f(1.0, 1.0);
	//glVertex2f(1.0f, 1);
	//glTexCoord2f(0.0, 1.0);
	//glVertex2f(-1.0f, 1);
	//glTexCoord2f(0.0, 0.0);
	//glVertex2f(-1, -1);
	//glTexCoord2f(1.0, 0.0);
	//glVertex2f(1, -1);
	//glEnd();
	//glDisable(GL_TEXTURE_2D);
	//g.resetLastSubsection();
	//g.resetLastSubsection();
	//
	//
	//
	////glTranslatef(0.2f, 0.2f, 0.0f);
	////glScalef(0.8f, 0.8f, 8.0f);
	//glRotatef(10.0f, 0.0f, 0.0f, 1.0f);
	//
	//
	///*float mat[16];
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		if ((j == i)&&(i<2)) mat[i * 4 + j] = 2;
	//		else if (j == i) mat[i * 4 + j] = 1;
	//		else mat[i * 4 + j] = 0;
	//	}
	//}
	//glLoadMatrixf(mat);*/
	//
	//
	///*GLfloat matrix[16];
	//glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	//if (test == 0)
	//{
	//	test = 1;
	//	for (int i = 0; i < 4; i++)
	//	{
	//		std::cout << std::fixed << std::setprecision(3) << matrix[i*4] << "		" << matrix[i*4 + 1] << "		" << matrix[i*4 + 2] << "		" << matrix[i*4 + 3] << std::endl << std::endl;
	//	}
	//	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
	//}*/
	//
	//
	//glBegin(GL_POLYGON);            // These vertices form a closed polygon
	//
	//glColor4f(1.0f, 1.0f, 0.0f, 0.5f); // Yellow
	//glVertex2f(0.4f, 0.2f);
	//glVertex2f(0.6f, 0.2f);
	//glVertex2f(0.7f, 0.4f);
	//glVertex2f(0.6f, 0.6f);
	//glVertex2f(0.4f, 0.6f);
	//glVertex2f(0.3f, 0.4f);
	//glEnd();
	//
	//
	//glLoadIdentity();
	//g.drawString("test", 4, -0.5f, -0.5f, 0.2f);
	//
	//
	//
	//float matrix[16];
	//glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	//if (test == 0)
	//{
	//	test = 1;
	//
	//	MathModel model;
	//	model.update();
	//	model.ml.printMatrix();
	//}
	//
	//
	//
	//pm.registerTime(4);
	g->endDraw();
	//pm.registerTime(5);
}
