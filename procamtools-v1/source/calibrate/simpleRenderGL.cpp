#include "stdafx.h"
#include <Windows.h>
#include "simpleRenderGL.h"


//#include <stdio.h>
//
//simpleRenderer* g_renderer;
//extern "C"
//void DrawCallback()
//{
//	g_renderer->display();
//}
//
//void simpleRenderer::setupDrawCallback()
//{
//	g_renderer = this;
//	glutDisplayFunc(::DrawCallback);
//}
//
//void MouseCallback(int x, int y)
//{
//	g_X = x;
//	g_Y = y;
//}
//
//void simpleRenderer::setupMouseFunction()
//{
//	g_renderer = this;
//
//	glutPassiveMotionFunc(::MouseCallback);
//}
//
//void KeyboardCallback(unsigned char key, int x, int y)
//{
//	if (key == 'a')
//		g_ms += 0.1;
//	if (key == 'z')
//		g_ms -= 0.1;
//	printf("Tecla %c apretada\n", key);
//}
//void simpleRenderer::setupKeyboarFunction()
//{
//	g_renderer = this;
//	glutKeyboardFunc(::KeyboardCallback);
//}