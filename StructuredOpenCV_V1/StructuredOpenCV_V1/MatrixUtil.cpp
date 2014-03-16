#include "stdafx.h"
#include "MatrixUtil.h"


CMatrixUtil::CMatrixUtil()
{
}


CMatrixUtil::~CMatrixUtil()
{
}

void CMatrixUtil::EstimateFocalLengthBougnoux(Mat& fundamental, Mat& pp1, Mat& pp2, float f1Squared, float f2Squared)
{

}

Mat CMatrixUtil::GetHomogeneousMatrix(const Mat& vec)
{
	float a1 = vec.at<float>(0, 0);
	Mat a = Mat(1, vec.cols + 1, vec.type());
	for (int i = 0; i < vec.cols; i++)
		a.at<float>(0, i) = vec.at<float>(0, i);
	a.at<float>(0, vec.cols) = 1;
	return a;
}

void CMatrixUtil::GetRightNullVector(Mat& in, Mat& out)
{
	Mat vecW = Mat::zeros(in.rows, in.cols, CV_32FC1);
	Mat vecO = Mat::zeros(in.rows, in.cols, CV_32FC1);
	solve(in, vecW, vecO);
	out = Mat(1, vecO.cols, CV_32FC1);
	int r = vecO.rows - 1;
	for (int c = 0; c < in.cols; c++)
		out.at<float>(0, c) = vecO.at<float>(r, c);
}

void CMatrixUtil::PrintFloatMatrix(Mat& mat)
{
	cout << endl << "Mat:" << endl;
	for (int i = 0; i < mat.rows; i++)
	{
		for (int j = 0; j < mat.cols; j++)
		{
			cout << mat.at<float>(i, j) << "\t";
		}
		cout << endl;
	}
	
}

bool CMatrixUtil::CheckCoherentRotation(cv::Mat_<double>& R) {
	if (fabsf(determinant(R)) - 1.0 > 1e-07) {
		cerr << "det(R) != +-1.0, this is not a rotation matrix" << endl;
		return false;
	}
	return true;
}