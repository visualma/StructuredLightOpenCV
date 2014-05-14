#include "stdafx.h"
#include "Renderer.h"
#include <stdio.h>
Renderer* g_renderer;
extern "C"
void drawCallback()
{
	g_renderer->display();
}

void Renderer::setupDrawCallback()
{
	g_renderer = this;
	glutDisplayFunc(::drawCallback);
}

void mouseCallback(int x,int y)
{
	g_mX = x;
	g_mY = y;
}

void Renderer::setupMouseFunction()
{
	g_renderer = this;

	glutPassiveMotionFunc(::mouseCallback);
}

void keyboardCallback(unsigned char key, int x, int y)
{
	if (key == 'a')
		g_s +=0.1;
	if (key == 'z')
		g_s -=0.1;
	printf("Tecla %c apretada", key);
}
void Renderer::setupKeyboarFunction()
{
	g_renderer = this;
	glutKeyboardFunc(::keyboardCallback);
}