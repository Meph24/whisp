#include "clickibunti.h"
#include <GL\glew.h>
#include "Graphics2D.h"
#include "PerformanceMeter.h"
#include <iostream>
#include <iomanip>

Graphics2D g(256);
PerformanceMeter pm(6);

int test = 0;
void clickibunti()
{
	glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
	glLoadIdentity();
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
		//test = 1;
		//glViewport(0,0,320,320);
	}
	pm.registerTime(0);
	sf::Time t = sf::microseconds(1000);
	sf::sleep(t);
	g.beginDraw();
	pm.registerTime(1);
	g.setColor(0.9f, 0.4f, 0.0f);
	pm.registerTime(2);
	g.fillOval(0.5f, 0.5f, 0.2f, 0.2f);
	pm.registerTime(3);
	pm.setSpikeHalfLifeTime(0.5f);
	pm.useFastApproximation = false;
	float posx = 0;
	for (int i = 0; i < 6; i++)
	{
		float now = pm.getSpikes(i)*144.0f/1000000.0f;
		color c;
		c.r = (i + 1) % 2;
		c.g = ((i + 1) / 2) % 2;
		c.b = ((i + 1) / 4) % 2;
		g.fillRect(posx-0.5f, 0, now, 0.2f,c);
		posx += now;
	}posx = 0;
	for (int i = 0; i < 6; i++)
	{
		float now = pm.getAVG(i)*144.0f / 1000000.0f;
		color c;
		c.r = (i + 1) % 2;
		c.g = ((i + 1) / 2) % 2;
		c.b = ((i + 1) / 4) % 2;
		g.fillRect(posx - 0.5f, 0.2f, now, 0.2f, c);
		posx += now;
	}posx = 0;
	for (int i = 0; i < 6; i++)
	{
		float now = pm.getTime(i)*144.0f / 1000000.0f;
		color c;
		c.r = (i + 1) % 2;
		c.g = ((i + 1) / 2) % 2;
		c.b = ((i + 1) / 4) % 2;
		g.fillRect(posx - 0.5f, 0.4f, now, 0.2f, c);
		posx += now;
	}posx = 0;
	for (int i = 0; i < 1; i++)
	{
		float now = pm.roundtriptime*144.0f / 1000000.0f;
		color c;
		c.r = (i + 1) % 2;
		c.g = ((i + 1) / 2) % 2;
		c.b = ((i + 1) / 4) % 2;
		g.fillRect(posx - 0.5f, 0.6f, now, 0.2f, c);
		posx += now;
	}




	subsection s = g.generateSubsection(0, 0, 0.4f, 0.4f, SNAP_MID,30);
	g.setSubsection(&s);
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	g.fillOval(0, 0, 1, 1);

	glBegin(GL_POLYGON);            // These vertices form a closed polygon

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex2f(0.2f,1);
	glVertex2f(-0.2f,1);
	glVertex2f(-1,-1);
	glVertex2f(1,-1);
	glEnd();
	subsection s2 = g.generateSubsection(0, 0, 0.8f, 0.8f, SNAP_W,-30);

	if (test == 0)
	{
		float * mat = s2.mat;
		test = 1;
		for (int i = 0; i < 4; i++)
		{
			std::cout << std::fixed << std::setprecision(3) << mat[i * 4] << "		" << mat[i * 4 + 1] << "		" << mat[i * 4 + 2] << "		" << mat[i * 4 + 3] << std::endl << std::endl;
		}
		std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
	}
	g.setSubsection(&s2);
	glColor4f(1.0f, 1.0f, 1.0f, 0.75f);
	g.fillOval(0, 0, 1, 1);

	glBegin(GL_POLYGON);            // These vertices form a closed polygon

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.2f, 1);
	glVertex2f(-0.2f, 1);
	glVertex2f(-1, -1);
	glVertex2f(1, -1);
	glEnd();
	g.resetLastSubsection();
	g.resetLastSubsection();



	//glTranslatef(0.2f, 0.2f, 0.0f);
	//glScalef(0.8f, 0.8f, 8.0f);
	glRotatef(10.0f, 0.0f, 0.0f, 1.0f);

	
	/*float mat[16];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((j == i)&&(i<2)) mat[i * 4 + j] = 2;
			else if (j == i) mat[i * 4 + j] = 1;
			else mat[i * 4 + j] = 0;
		}
	}
	glLoadMatrixf(mat);*/


	/*GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	if (test == 0)
	{
		test = 1;
		for (int i = 0; i < 4; i++)
		{
			std::cout << std::fixed << std::setprecision(3) << matrix[i*4] << "		" << matrix[i*4 + 1] << "		" << matrix[i*4 + 2] << "		" << matrix[i*4 + 3] << std::endl << std::endl;
		}
		std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
	}*/


	glBegin(GL_POLYGON);            // These vertices form a closed polygon

	glColor4f(1.0f, 1.0f, 0.0f, 0.5f); // Yellow
	glVertex2f(0.4f, 0.2f);
	glVertex2f(0.6f, 0.2f);
	glVertex2f(0.7f, 0.4f);
	glVertex2f(0.6f, 0.6f);
	glVertex2f(0.4f, 0.6f);
	glVertex2f(0.3f, 0.4f);
	glEnd();


	pm.registerTime(4);
	g.endDraw();
	pm.registerTime(5);
}
