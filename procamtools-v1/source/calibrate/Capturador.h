#pragma once
#include <vector>
#include "opencv2/opencv.hpp"
#include <time.h>
using namespace std;
using namespace cv;

class COptions
{
public:
	int m_nWidth, m_nHeight, m_nNumPatterns,m_nBasePatterns,m_nScreenWidth,m_nScreenHeight,m_nNumFringes, m_nFringeInterval;
	bool m_bHorizontal, m_bVertical, m_bComplementary, m_bPhase;
	float m_fProjectorCenter;
	COptions(int Width, int Height, int numPatterns,int numFringes, bool Horizontal, bool Vertical, bool Complementary,bool fringes);
	int GetNumBits(int dir);
	void GetDesktopResolution(int& horizontal, int& vertical);
};

class CCapturador
{
public:
	CCapturador(int nPatterns,int nFramesPerPattern,vector<Mat>& Patrones);
	CCapturador(COptions* opt, string path);
	bool LoadCapturesFromFiles(string path);
	bool LoadCapturesFromFilesUndisorted(string path,Mat& camera,Mat& dist);
	CCapturador();
	int m_nPatterns, m_nFramesPerPattern;
	vector <Mat> m_vCaptures;
	vector <Mat> m_vPatterns;
	COptions* m_Options;
	VideoCapture m_VideoCapture;
	bool CapturePatterns(int time);
	bool CapturePatternsUndisorted(Mat& CameraMatrix, Mat& DistVect,int time);
	static string SerializeCaptures(vector<Mat> imagenes,string str);
	~CCapturador();
};


