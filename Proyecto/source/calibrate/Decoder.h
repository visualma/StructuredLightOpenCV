#include "Capturador.h"
#include "stdafx.h"
//#include "pcl/common/common.h"
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

//using namespace pcl;

class CloudPoint : Point3f
{
public:
    float reprojection_error;
    CloudPoint(Point3f a,float r_e)
    {
        this->x = a.x;
        this->y = a.y;
        this->z = a.z;
        this->reprojection_error = r_e;
    }
};


class CDecoder
{
	vector<Mat> m_vCaptures;
	vector<Mat> m_vGrayError[2];
	vector<Mat> m_vGrayMap;
	vector<Point2f> m_vCorrespondencePoints[2];
    vector<Point3f> m_vPointCloud;
	Mat m_mGrayError[2];
	Mat m_mPhaseError[2];
	Mat m_mPhaseMap[2];
	Mat m_mFundamentalMatrix;
    Mat_<double> m_mEssentialMatrix;
	float m_fDivisor[2];
	Matx34d m_mProjectorProjection;
    Matx34d m_mCameraProjection;
    Mat_<double> k_cam,k_proj;
public:
	COptions* m_Info;
	Mat m_mGray[2];
	Mat m_mMask[2];
	CDecoder(COptions* Options);
	bool Decode(float* thres, vector<Mat>& vCaptures);
	void DecodeGray(int direction,float int_threashold);
	Mat DecodeBinToGray(vector<Mat> diff);
	int ConvertGrayToBinary(const unsigned long graycode);
	static Mat MaskMat(Mat& img, Mat& mask,bool);
	Mat DecodePhaseImages(vector<Mat>&, int);
	void UnwrapPhase(Mat& phase, int period, Mat& reference, Mat& result, Mat& unwrap_error);
	void CreateReliableMap(int dir);
	void BuildCorrespondence(Mat* grays);
    bool Calibrate(Mat& CameraMatrix, Mat& DistMatrix);
	bool CalibrateRadial();
	void EstimateIntrinsics(Mat& fundamental);

    bool Generate3Dpoints(Mat& output,Mat& distCoef);
	static Mat_<double> LinearLSTriangulation(
        Point3f u,//homogenous image point (u,v,1)
		Matx34d P,//camera 1 matrix
        Point3f u1,//homogenous image point in 2nd camera
		Matx34d P1//camera 2 matrix
		);
	double TriangulatePoints(
		const vector<Point2f>& pt_set1,
		const vector<Point2f>& pt_set2,
		const Mat&K,
		const Mat&Kinv,
		const Matx34d& P,
		const Matx34d& P1,
        vector<Point3f>& pointcloud);
    bool TestTriangulation(const vector<Point3f>& pcloud, const Matx34d& P, vector<uchar>& status);
    static Mat_<double> IterativeLinearLSTriangulation(Point3f u,	//homogenous image point (u,v,1)
		Matx34d P,			//camera 1 matrix
        Point3f u1,			//homogenous image point in 2nd camera
		Matx34d P1			//camera 2 matrix
		);
    double TriangulatePointsIterative(const vector<Point2f>& pt_set1,
    const vector<Point2f>& pt_set2,
    const Mat& K,
    const Mat& Kinv,
    const Mat& distcoeff,
    const Matx34d& P,
    const Matx34d& P1,
    vector<Point3f>& pointcloud,
    vector<Point2f>& correspImg1Pt);

	Mat m_m3DPoints;


	~CDecoder();
};



