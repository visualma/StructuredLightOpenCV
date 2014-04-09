#include "stdafx.h"
#include "Capturador.h"
#include <string>
#include <sstream>
#include "wtypes.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <io.h>
#include <fcntl.h>

void delay(int secs) {
	for (int i = (time(NULL) + secs); time(NULL) != i; time(NULL));
}

CCapturador::CCapturador(int nPatterns,int nFrames, vector<Mat>& Patrones) :
m_vPatterns(Patrones),
m_nPatterns(nPatterns),
m_nFramesPerPattern(nFrames)
{

}

CCapturador::CCapturador()
{}
CCapturador::CCapturador(COptions* opt, string ruta) :  m_Options(opt)
{
	m_nPatterns = m_Options->m_nNumPatterns;
	if (m_Options->m_bVertical)
		m_Options->m_nNumPatterns *= 2;
	if (m_Options->m_bComplementary)
		m_Options->m_nNumPatterns *= 2;
	m_nPatterns = m_Options->m_nNumPatterns;
	for (int i = 0; i < m_nPatterns; i++)
	{
		std::ostringstream oss;
		oss << ruta;
		if (i < 10)
			oss << "0";
		oss << i << ".bmp";
		string temp = oss.str();
		Mat pattern = imread(oss.str(), 1);
		m_vPatterns.push_back(pattern);
		oss.clear();
	}

}

bool CCapturador::CapturePatterns(int time,int device,int posX,int posY,bool useComp)
{
	m_vCaptures.clear();
	
	//VideoCapture cap(0); // open the default camera
	if (!m_VideoCapture.isOpened())  // check if we succeeded
		m_VideoCapture = VideoCapture(device);
	if (!m_VideoCapture.isOpened())
		return false;
		
	//VideoCapture cap(0);
	//if (!cap.isOpened())
	//	return -1;
	bool bMakeCapture = false;
	int nPatterns = 0;
	//namedWindow("Camera", 1);
	cvWaitKey(500);
	m_VideoCapture >> m_mTextura;
	imwrite("Textura.bmp", 0);
	namedWindow("Patrones");

	HWND win_handle = FindWindow(0, "Patrones");
	if (!win_handle)
	{
		printf("Failed FindWindow\n");
	}

	// Resize
	unsigned int flags = (SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
	flags &= ~SWP_NOSIZE;
	unsigned int x = posX;
	unsigned int y = posY;
	unsigned int w = m_Options->m_nWidth;
	unsigned int h = m_Options->m_nHeight;
	SetWindowPos(win_handle, HWND_NOTOPMOST, x, y, w, h, flags);

	// Borderless
	SetWindowLong(win_handle, GWL_STYLE, GetWindowLong(win_handle, GWL_EXSTYLE) | WS_EX_TOPMOST);
	ShowWindow(win_handle, SW_SHOW);
	cvMoveWindow("Patrones", posX, posY);
	cvWaitKey(2000);
	auto A = GetTickCount();
	auto B = GetTickCount();
	for (int i = 0;;)
	{
		imshow("Patrones", m_vPatterns[i]);
		Mat frame;
		m_VideoCapture >> frame;
		//imshow("Camera", frame);
		B = GetTickCount();
		int C = B - A;
		if (C>time || waitKey(30) >= 0)
		{
			if (!frame.empty())
			if (useComp)
			{
				i++;
				Mat capture = frame.clone();
				Mat gray;
				cv::cvtColor(capture, gray, CV_BGR2GRAY);
				m_vCaptures.push_back(gray);
				if (++nPatterns >= m_nPatterns)
					break;
			}
			else
			{
				i += 2;
				Mat capture = frame.clone();
				Mat gray;
				cv::cvtColor(capture, gray, CV_BGR2GRAY);
				m_vCaptures.push_back(gray);
				nPatterns += 2;
				if (nPatterns >= m_nPatterns)
					break;
			}
			else
				printf("Error: no caputre info.\n");
			A = GetTickCount();
		};
	}
	cout << "Patrones capturados." << endl;
	cvDestroyWindow("Patrones");
	//cvDestroyWindow("Camera");
	return true;
}

bool CCapturador::CapturePatternsUndisorted(Mat& CameraMatrix,Mat& DistMatrix,int time)
{
	m_vCaptures.clear();
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	bool bMakeCapture = false;
	int nPatterns = 0;
	namedWindow("Camera", 1);
	namedWindow("Patrones");
/*
	HWND win_handle = FindWindow(0, L"Patrones");
	if (!win_handle)
	{
		printf("Failed FindWindow\n");
	}

	// Resize
	unsigned int flags = (SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
	flags &= ~SWP_NOSIZE;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int w = m_Options->m_nWidth;
	unsigned int h = m_Options->m_nHeight;
	SetWindowPos(win_handle, HWND_NOTOPMOST, x, y, w, h, flags);

	// Borderless
	SetWindowLong(win_handle, GWL_STYLE, GetWindowLong(win_handle, GWL_EXSTYLE) | WS_EX_TOPMOST);
	ShowWindow(win_handle, SW_SHOW);
	cvMoveWindow("Patrones", 0, 0);
    */
    long A = getTickCount();
    long B = getTickCount();
	bool start = false;
	for (int i = 0;;)
	{
		imshow("Patrones", m_vPatterns[i]);
		Mat frame;
		cap >> frame;
		if (frame.empty())
			return false;
		Mat view, rview, map1, map2;
		initUndistortRectifyMap(CameraMatrix, DistMatrix, Mat(),
			getOptimalNewCameraMatrix(CameraMatrix, DistMatrix, frame.size(), 1, frame.size(), 0),
			frame.size(), CV_16SC2, map1, map2);
		remap(frame, rview, map1, map2, INTER_LINEAR);
		imshow("Camera", rview);
        B = getTickCount();
		int C = B - A;
		if ((C>time&&start) || waitKey(30) >= 0)
		{
			start = true;
			cout << "time = " << C << endl;
            A = getTickCount();
			i++;
			Mat capture = frame.clone();
			Mat gray;
			cv::cvtColor(capture, gray, CV_BGR2GRAY);
			m_vCaptures.push_back(gray);
			if (++nPatterns >= m_nPatterns)
				break;
		};
	}
	cout << "Patrones capturados." << endl;
	cvDestroyWindow("Patrones");
	return true;
}

bool CCapturador::SerializeCaptures(vector<Mat> imagenes, string str)
{
	for (int i = 0; i < imagenes.size(); i++)
	{
		std::ostringstream oss;
		if (i<10)
			oss << str << "-" << "0" << i << ".bmp";
		else
			oss << str << "-" << i << ".bmp";
		string	ruta = oss.str();
		imagenes[i].convertTo(imagenes[i], CV_8UC1);
		imwrite(oss.str(), imagenes[i]);
		oss.clear();
	}
	return true;
}

string CCapturador::SerializeCapturesDefault(vector<Mat> imagenes, string str)
{
	int tiempo = time(NULL);
	std::ostringstream oss1;
	oss1 << "cd .. & cd resources & cd Captures & mkdir " << str << "-" << tiempo;
	system(oss1.str().c_str());
	string rutafinal;
	std::ostringstream oss3;
	oss3 << "..//resources//Captures//" << str << "-" << tiempo;
	for (int i = 0; i < imagenes.size(); i++)
	{
		std::ostringstream oss;
		string ruta = "..//resources//Captures//";
		if (i<10)
			oss << ruta << str << "-" << tiempo << "//Capture-0" << i << ".bmp";
		else
			oss << ruta << str << "-" << tiempo << "//Capture-" << i << ".bmp";
		ruta = oss.str();
		//if (b16)
		//	cvSave(oss.str().c_str(),  imagenes[i].data);
		imagenes[i].convertTo(imagenes[i], CV_8UC1);
		imwrite(oss.str(), imagenes[i]);
		oss.clear();
	}
	return oss3.str();
}

bool CCapturador::LoadCapturesFromFiles(string ruta)
{
	m_vCaptures.clear();
	m_nPatterns = m_Options->m_nNumPatterns;
	for (int i = 0; i < m_nPatterns + m_Options->m_nNumFringes * 2; i++)
	{
		std::ostringstream oss;
		oss << ruta;
		if (i < 10)
			oss << "0";
		oss << i << ".bmp";
		string temp = oss.str();
		Mat capture = imread(oss.str(), 1);
		if (capture.empty())
			return false;
		Mat gray;
		cv::cvtColor(capture, gray, CV_BGR2GRAY);
		m_vCaptures.push_back(gray);
		oss.clear();
	}
	return true;
}
bool CCapturador::LoadCapturesFromFilesUndisorted(string ruta,Mat& CameraMatrix,Mat& DistMatrix)
{
	m_vCaptures.clear();
	m_nPatterns = m_Options->m_nNumPatterns;
	for (int i = 0; i < m_nPatterns + m_Options->m_nNumFringes * 2; i++)
	{
		std::ostringstream oss;
		oss << ruta;
		if (i < 10)
			oss << "0";
		oss << i << ".jpg";
		string temp = oss.str();
		Mat capture = imread(oss.str(), 1);
		if (capture.empty())
			return false;
		Mat gray;
		cv::cvtColor(capture, gray, CV_BGR2GRAY);
		Mat view, rview, map1, map2;
		initUndistortRectifyMap(CameraMatrix, DistMatrix, Mat(),
			getOptimalNewCameraMatrix(CameraMatrix, DistMatrix, gray.size(), 1, gray.size(), 0),
			gray.size(), CV_16SC2, map1, map2);
		remap(gray, rview, map1, map2, INTER_LINEAR);
		m_vCaptures.push_back(rview);
		oss.clear();
	}
	return true;
}

bool CCapturador::tryCamera(int device)
{
	if (!m_VideoCapture.isOpened())  // check if we succeeded
		m_VideoCapture = VideoCapture(device);
	if (!m_VideoCapture.isOpened())
		return false;
	char key = 0;
	Mat frame;
	while (key==-1||key==0)
	{
		m_VideoCapture >> frame;
		if (!frame.empty())
			imshow("Camara", frame);
		key = cvWaitKey(30);
	}
	cvvDestroyWindow("Camara");
}

CCapturador::~CCapturador()
{
}

///////////////////////////////////////////////////

COptions::COptions(int Width, int Height, int numPatterns, int numFringes, bool Horizontal, bool Vertical, bool Complementary, bool fringes,bool useConsole) :
m_nWidth(Width),
m_nHeight(Height),
m_nNumPatterns(numPatterns),
m_bVertical(Vertical),
m_bHorizontal(Horizontal),
m_bComplementary(Complementary),
m_nNumFringes(numFringes),
m_bPhase(fringes)
{
	m_nBasePatterns = numPatterns;
    //GetDesktopResolution(m_nScreenWidth, m_nScreenHeight);
	m_nFringeInterval = 1;
	m_fProjectorCenter = 0.5;
	if (useConsole)
	{
		BOOL bOk = AllocConsole();
		if (bOk)
		{
			int fd;
			HANDLE hOutput;
			FILE* fp;
			hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
			fd = _open_osfhandle((intptr_t)hOutput, _O_TEXT);
			fp = _fdopen(fd, "w");
			*stdout = *fp;
			char *pBuffer = (char*)malloc(32);
			setvbuf(stdout, pBuffer, _IOFBF, 32);
			SetConsoleTitle(TEXT("Structured Light Mapping V1 Josue Page Vizcaino"));
		}
	}
}

int COptions::GetNumBits(int dir)
{
	if (dir)
		return  ceilf(logf(m_nHeight) / logf(2));
	else
		return  ceilf(logf(m_nWidth) / logf(2));
}
/*
void COptions::GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}
*/
