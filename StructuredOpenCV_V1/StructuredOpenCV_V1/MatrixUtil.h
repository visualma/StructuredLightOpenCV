#pragma once
#include "opencv2\opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;
class CMatrixUtil
{
public:
	CMatrixUtil();

	static Mat GetHomogeneousMatrix(const Mat& vec);
	static void EstimateFocalLengthBougnoux(Mat& fundamental, Mat& pp1, Mat& pp2, float f1Squared, float f2Squared);
	static void GetRightNullVector(Mat& input, Mat& output);
	static void PrintFloatMatrix(Mat&);
	static bool CheckCoherentRotation(cv::Mat_<double>& R);
	~CMatrixUtil();
};

