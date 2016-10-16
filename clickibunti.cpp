#include "clickibunti.h"
#include <GL\glew.h>
#include "Graphics2D.h"
#include "PerformanceMeter.h"

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
		test = 1;
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
	}


	glTranslatef(0.1f, 0.1f, 0.0f);
	glScalef(0.8f, 0.8f, 1.0f);
	glRotatef(10.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);            // These vertices form a closed polygon

	glColor4f(1.0f, 1.0f, 0.0f, 0.5f); // Yellow
	glVertex2f(0.4f, 0.2f);
	glVertex2f(0.6f, 0.2f);
	glVertex2f(0.7f, 0.4f);
	glVertex2f(0.6f, 0.6f);
	glVertex2f(0.4f, 0.6f);
	glVertex2f(0.3f, 0.4f);
	glEnd();

	if (test == 0)
	{
		test = 1;

	}

	pm.registerTime(4);
	g.endDraw();
	pm.registerTime(5);
}
