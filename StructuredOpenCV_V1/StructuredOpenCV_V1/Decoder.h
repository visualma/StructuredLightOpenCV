#include "Capturador.h"
#pragma once

#define M_E        2.71828182845904523536
#define M_LOG2E    1.44269504088896340736
#define M_LOG10E   0.434294481903251827651
#define M_LN2      0.693147180559945309417
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
#define M_1_PI     0.318309886183790671538
#define M_2_PI     0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880
#define M_SQRT1_2  0.707106781186547524401

class CDecoder
{
	COptions* m_Info;
	vector<Mat>& m_vCaptures;
	vector<Mat> m_vGrayError[2];
	vector<Mat> m_vGrayMap;
	vector<Point2f> m_vCorrespondencePoints[2];
	vector<Point3d> m_vPointCloud;
	Mat m_mMask[2];
	Mat m_mGrayError[2];
	Mat m_mGray[2];
	Mat m_mPhaseError[2];
	Mat m_mPhaseMap[2];
	Mat m_mFundamentalMatrix;
	Mat m_mEssentialMatrix;
	float m_fDivisor[2];
	Matx34d m_mProjectorProjection;
	Matx34d m_mCameraProjection;
public:
	CDecoder(COptions* Options, vector<Mat>& vCaptures);
	bool Decode();
	void DecodeGray(int direction,float int_threashold);
	Mat DecodeBinToGray(vector<Mat> diff);
	int ConvertGrayToBinary(const unsigned long graycode);
	Mat MaskMat(Mat& img, Mat& mask);
	Mat DecodePhaseImages(vector<Mat>&, int);
	void UnwrapPhase(Mat& phase, int period, Mat& reference, Mat& result, Mat& unwrap_error);
	void CreateReliableMap(int dir);
	void BuildCorrespondence(Mat* grays);
	void Calibrate(Mat& CameraMatrix, Mat& DistMatrix);
	void EstimateIntrinsics(Mat& fundamental);
	void Generate3Dpoints(Mat& output);
	Mat_<double> LinearLSTriangulation(
		Point3d u,//homogenous image point (u,v,1)
		Matx34d P,//camera 1 matrix
		Point3d u1,//homogenous image point in 2nd camera
		Matx34d P1//camera 2 matrix
		);
	double TriangulatePoints(
		const vector<Point2f>& pt_set1,
		const vector<Point2f>& pt_set2,
		const Mat&K,
		const Mat&Kinv,
		const Matx34d& P,
		const Matx34d& P1,
		vector<Point3d>& pointcloud);
	Mat m_m3DPoints;


	~CDecoder();
};

