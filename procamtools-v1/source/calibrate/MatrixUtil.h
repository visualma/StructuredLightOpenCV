#pragma once
#include <opencv2/opencv.hpp>
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
    static void DecomposeEssentialUsingHorn90(double _E[9], double _R1[9], double _R2[9], double _t1[3], double _t2[3]);
    static bool DecomposeEtoRandT(Mat_<double>& E,Mat_<double>& R1,Mat_<double>& R2,Mat_<double>& t1,Mat_<double>& t2);
    static void TakeSVDOfE(Mat_<double>& E, Mat& svd_u, Mat& svd_vt, Mat& svd_w);
    static void drawArrows(Mat& frame, const vector<Point2f>& prevPts, const vector<Point2f>& nextPts, const vector<uchar>& status, const vector<float>& verror, const Scalar& _line_color);
	~CMatrixUtil();
};

