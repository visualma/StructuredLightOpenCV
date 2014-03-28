#include "stdafx.h"
#include "Renderer.h"

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

