#include "stdafx.h"
#include "Decoder.h"
#include "opencv2\gpu\gpu.hpp"
#include "opencv2\gpu\gpumat.hpp"
#include "Capturador.h"
#include "opencv2\stitching\detail\camera.hpp"
#include "opencv2\stitching\detail\autocalib.hpp"
#include "MatrixUtil.h"
CDecoder::CDecoder(COptions* Options, vector<Mat>& vCaptures) : m_vCaptures(vCaptures), m_Info(Options)
{

}

bool CDecoder::Decode()
{
	CCapturador cap;
	vector<Mat> temp;
	if (m_Info->m_bHorizontal)
	{
		DecodeGray(0, 0.1);
		//Mascarear
		m_mMask[0] = Mat::zeros(m_mGray[0].rows,m_mGray[0].cols, CV_8UC1);
		cv::threshold(m_mGrayError[0], m_mMask[0], 50, 255, CV_THRESH_BINARY);
		m_mMask[0].convertTo(m_mMask[0], CV_16UC1);
		if (m_Info->m_bPhase)
		{
			auto begin = m_vCaptures.begin() + m_Info->m_nNumPatterns / 2;
			auto end = m_vCaptures.begin() + m_Info->m_nNumPatterns / 2 + m_Info->m_nNumFringes;

			vector<Mat> phaseImgs(begin, end);
			m_mPhaseMap[0] = DecodePhaseImages(phaseImgs, 0);
			UnwrapPhase(m_mPhaseMap[0], m_Info->m_nFringeInterval*m_Info->m_nNumFringes, m_mGray[0], m_mPhaseMap[0], m_mPhaseError[0]);
		}
		else
			m_mPhaseError[0] = Mat(m_mGray[0].rows, m_mGray[0].cols, CV_32FC1);
		CreateReliableMap(0);
		m_mGray[0] = MaskMat(m_mGray[0], m_mMask[0]);
		temp.push_back(m_mGrayError[0]);
		temp.push_back(m_mMask[0]);
		Mat temp1 = Mat(m_mGray[0].rows,m_mGray[0].cols,CV_8UC1);
		m_mGray[0].convertTo(temp1, CV_8UC1, 255 / 1024.0, 0);
		temp.push_back(m_mGray[0]);
		temp.push_back(temp1);
		temp.push_back(m_mPhaseError[0]);
		imshow("Correspondence X", temp1);
	}
	if (m_Info->m_bVertical)
	{
		DecodeGray(1, 0.1);
		//Mascarear
		m_mMask[1] = Mat::zeros(m_mGray[1].rows, m_mGray[1].cols, CV_8UC1);
		cv::threshold(m_mGrayError[1], m_mMask[1], 50, 255, CV_THRESH_BINARY);
		m_mMask[1].convertTo(m_mMask[1], CV_16UC1);
		if (m_Info->m_bPhase)
		{
			auto begin = m_vCaptures.begin() + m_Info->m_nBasePatterns + m_Info->m_nNumPatterns / 2;
			auto end = m_vCaptures.begin() + m_Info->m_nNumPatterns + m_Info->m_nNumFringes;

			vector<Mat> phaseImgs(begin, end);
			m_mPhaseMap[1] = DecodePhaseImages(phaseImgs, 1);
			UnwrapPhase(m_mPhaseMap[1], m_Info->m_nFringeInterval*m_Info->m_nNumFringes, m_mGray[1], m_mPhaseMap[1], m_mPhaseError[1]);
		}
		else
			m_mPhaseError[1] = Mat(m_mGray[1].rows, m_mGray[1].cols, CV_32FC1);
		CreateReliableMap(1);
		m_mGray[1] = MaskMat(m_mGray[1], m_mMask[1]);
		temp.push_back(m_mGrayError[1]);
		temp.push_back(m_mMask[1]);
		Mat temp1 = Mat(m_mGray[1].rows, m_mGray[1].cols, CV_8UC1);
		m_mGray[1].convertTo(temp1, CV_8UC1, 255 / 1024.0, 0);
		imshow("Correspondence Y", temp1);
		temp.push_back(m_mGray[1]);
		temp.push_back(temp1);
		temp.push_back(m_mPhaseError[1]);
	}
	if (m_Info->m_bHorizontal&&m_Info->m_bVertical)
	{
		for (int y = 0; y < m_mMask[1].cols;y++)
		for (int x = 0; x < m_mMask[1].rows; x++)
		{
			if (!m_mMask[1].at<ushort>(x, y))
				m_mMask[0].at<ushort>(x, y) = 0;
			m_mPhaseError[0].at<float>(x, y) = std::min(m_mPhaseError[0].at<float>(x, y), m_mPhaseError[1].at<float>(x, y));
		}
	}
	temp.push_back(m_mMask[0]);
	cap.SerializeCaptures(temp, "mask", true);
	return true;
}

void CDecoder::DecodeGray(int dir,float int_threshold)
{
 	int nbits = m_Info->GetNumBits(dir);
	vector<Mat> diff;
	m_mGrayError[dir] = Mat::zeros(cvSize(m_vCaptures[0].cols, m_vCaptures[0].rows), CV_8UC1);
	for (int i = 0; i < nbits; i++)
	{
		double maxVal, maxValComp, minVal, minValComp ;
		Mat m = m_vCaptures[dir*(m_Info->m_nBasePatterns*2+m_Info->m_nNumFringes) + 2 * i].clone();
		Mat m1 = m_vCaptures[dir*(m_Info->m_nBasePatterns * 2 + m_Info->m_nNumFringes) + 2 * i + 1].clone();
		minMaxIdx(m, &minVal, &maxVal);
		minMaxIdx(m1, &minValComp, &maxValComp);
		float BrightesPixel = std::max(maxVal, maxValComp);
		Mat difference;
		difference = m - m1;
		diff.insert(diff.begin(), difference);
		float threshold = BrightesPixel*int_threshold;
		Mat result;
		m_fDivisor[dir] = BrightesPixel / m_Info->GetNumBits(dir);
		cv::threshold(difference, result, threshold, m_fDivisor[dir], CV_THRESH_BINARY);
		m_mGrayError[dir] += result;
	}
	m_mGray[dir] = Mat::zeros(diff[0].rows,diff[0].cols,CV_16UC1);
	m_mGray[dir] = DecodeBinToGray(diff);

}

Mat CDecoder::DecodeBinToGray(vector<Mat> diff)
{
	CV_Assert(diff[0].depth() != sizeof(ushort));
	Mat result = Mat::zeros(diff[0].rows,diff[0].cols,CV_8UC1);
	int niveles = diff.size();
	int tam = result.cols*result.rows;
	unsigned short *Gray = new unsigned short[tam];
	unsigned short *binario = new unsigned short[tam];
	memset(binario, 0,sizeof(ushort)*tam);
	memset(Gray, 0, sizeof(ushort)*tam);
	cv::MatIterator_<uchar> it,end;
	for (int l = 0; l < diff.size(); l++)
	{
		int i = 0;
		/*
		unsigned char *input = (unsigned char*)(diff[l].data);
		for (int i = 0; i < tam; i++)
		{
			int k = input[i];
			if (k>0)
			{
				int t = Gray[i];
				if (t != 0)
				{
					//printf("h");
				}
				if (t > 255)
				{
					t = t;
				}
				t += (1 << l);
				Gray[i] = t;
			}
				 
		}*/
		
		for (it = diff[l].begin<uchar>(), end = diff[l].end<uchar>(); it != end; ++it, i++)
		{
			if (*it>0)
				Gray[i] += (1 << l);
		}
				
	}
	for (int i = 0; i < tam; i++)
	{
		int gray = Gray[i];
		int bina = ConvertGrayToBinary(Gray[i]);
		binario[i] = bina;
	}
		

	result = Mat(result.rows, result.cols, CV_16UC1, binario);
	return result;
}

Mat CDecoder::DecodePhaseImages(vector<Mat>& img, int dir)
{
	Mat result = Mat::zeros(img[0].rows, img[0].cols, CV_32FC1);
	int nPhases = img.size();
	Mat senos = Mat(nPhases, 3,CV_32FC1);
	Mat senos_inv = Mat(3, nPhases, CV_32FC1);
	for (int i = 0; i < nPhases; i++)
	{
		senos.at<float>(i, 0) = cos(2 * M_PI * i / nPhases);
		senos.at<float>(i, 1) = sin(2 * M_PI * i / nPhases);
		senos.at<float>(i, 2) = 1;
	}
	invert(senos, senos_inv, cv::DECOMP_SVD);
	/*
	o
	SVD svd(senos);
	Mat pinvA = svd.vt.t()*Mat::diag(1. / svd.w)*svd.u.t();
	*/
	cout << img[0].type() << endl;
	float a = 0;
	for (int y = 0; y < img[0].cols; y++)
	for (int x = 0; x < img[0].rows; x++)
	{
		Mat vec = Mat(nPhases, 1, CV_32FC1);
		for (int i = 0; i < nPhases; i++)
			vec.at<float>(i, 0) = img[i].at<uchar>(x, y);
		vec = senos_inv * vec;
		float A = sqrt(vec.at<float>(0, 0) *vec.at<float>(0, 0) + vec.at<float>(1, 0) * vec.at<float>(1, 0));
		float phi = atan2(vec.at<float>(0, 0) / A, vec.at<float>(1, 0) / A);
		float m = vec.at<float>(2, 0);
		while (phi < 0)
			phi += 2 * M_PI;
		result.at<float>(x, y) = phi / (2 * M_PI);
		//a += result.at<float>(x, y);
	}
	cout << a << endl;
	return result;
}

void CDecoder::UnwrapPhase(Mat& phase, int period, Mat& reference, Mat& result, Mat& unwrap_error)
{
	float window = 2.0 / period;
	result = Mat(phase);
	unwrap_error = Mat(phase);
	for (int y = 0; y < phase.cols;y++)
	for (int x = 0; x < phase.rows; x++)
	{
		int graycode = reference.at<ushort>(x, y);
		float moire_phase = phase.at<float>(x, y);
		float gray_phase = (float)(graycode % period) / period;
		if (_isnan(moire_phase))
		{
			result.at<float>(x, y) = graycode;
			unwrap_error.at<float>(x, y) = 0.5;
			continue;
		}
		if (moire_phase >= gray_phase + 0.5)
			moire_phase -= 1;
		else if (moire_phase < gray_phase - 0.5)
			moire_phase += 1;

		float diff = abs(gray_phase - moire_phase);
		if (diff < window)
			result.at<float>(x, y) = graycode - (graycode % period) + period * moire_phase;
		else
			result.at<float>(x, y) = graycode;
		unwrap_error.at<float>(x, y) = diff;
	}
}

void CDecoder::CreateReliableMap(int dir)
{
	//float maxError = 2.0 / m_Info->m_nNumFringes;
	Mat& reliable = m_mPhaseError[dir];
	for (int y = 0; y < reliable.cols;y++)
		for (int x = 0; x < reliable.rows; x++)
			if (/*reliable.at<float>(x, y) < maxError  && */m_mGrayError[dir].at<uchar>(x, y) > m_fDivisor[dir]*2)
				reliable.at<float>(x, y) = 255;
			else
				reliable.at<float>(x, y) = 0;
}

CDecoder::~CDecoder()
{
}

int CDecoder::ConvertGrayToBinary(const unsigned long graycode)
{
	int bincode = 0;
	int mask = 1;
	while (mask < graycode)
		mask *= 2;
	for (; mask; mask /= 2)
		bincode += (mask & graycode) ^ (mask & (bincode / 2));
	return bincode;
}

Mat CDecoder::MaskMat(Mat& img, Mat& mask)
{
	Mat a = img.clone();
	for (int i = 0; i < a.cols;i++)
	for (int j = 0; j < a.rows;j++)
	if (mask.at<ushort>(j, i) == 0)
		a.at<ushort>(j, i) = 0;
	return a;
}


/////////////////////////////////////////// CALIBRATE

void CDecoder::Calibrate(Mat& CameraMatrix,Mat& DistMatrix)
{
	BuildCorrespondence(m_mGray);
	m_mFundamentalMatrix = findFundamentalMat(m_vCorrespondencePoints[0], m_vCorrespondencePoints[1], FM_RANSAC, 3.0, 0.99);

	m_mEssentialMatrix = CameraMatrix.t() * m_mFundamentalMatrix * CameraMatrix;

	SVD svd(m_mEssentialMatrix);
	Matx33d W(0, -1, 0,//HZ 9.13
		1, 0, 0,
		0, 0, 1);
	Mat_<double> R = svd.u * Mat(W) * svd.vt; //HZ 9.19
	Mat_<double> t = svd.u.col(2); //u3
	m_mCameraProjection = Matx34d
		(R(0, 0), R(0, 1), R(0, 2), t(0),
		R(1, 0), R(1, 1), R(1, 2), t(1),
		R(2, 0), R(2, 1), R(2, 2), t(2));

	m_mProjectorProjection = Matx34d
		(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0);

	std::cout << "R: " << (cv::Mat)R << std::endl;
	std::cout << "t: " << (cv::Mat)t << std::endl;

	if (!CMatrixUtil::CheckCoherentRotation(R)) {
		cout << "resulting rotation is not coherent\n";
		m_mCameraProjection = 0;
		return;
	}
	vector<Vec3f> lines1;
	computeCorrespondEpilines(m_vCorrespondencePoints[0], 1, m_mFundamentalMatrix, lines1);
	Mat c = m_vCaptures[0].clone();
	for (int i = 0; i<lines1.size() ;i+=1000) {
		Vec3f it = lines1[i];
		// draw the epipolar line between first and last column
		cv::line(c, cv::Point(0, -it[2] / it[1]),
		cv::Point(c.cols, -(it[2] + it[0] * c.cols) / it[1]),
		cv::Scalar(255, 0, 0));
	}
	imshow("imagen", c);
}

void CDecoder::EstimateIntrinsics(Mat& fundamental)
{
	double f1, f2;
	//Mat homoProCod = CMatrixUtil::GetHomogeneousMatrix(m_fProCod);
	//Mat homoCamCod = CMatrixUtil::GetHomogeneousMatrix(m_fCamCod);

	//cout << "alto";
}


void CDecoder::BuildCorrespondence(Mat* grays)
{
	Point2f a, b;
	for (int x = 0; x < grays[0].rows;x++)
	for (int y = 0; y < grays[0].cols; y++)
	{
		if (m_mMask[0].at<ushort>(x, y))
		{
			a = Point2f(x, y);
			b = Point2f(grays[0].at<ushort>(x, y), grays[1].at<ushort>(x, y));
			m_vCorrespondencePoints[0].push_back(a);
			m_vCorrespondencePoints[1].push_back(b);
		}
	}
}

void CDecoder::Generate3Dpoints(Mat& k)
{
	triangulatePoints(m_mProjectorProjection,m_mCameraProjection, m_vCorrespondencePoints[0], m_vCorrespondencePoints[1], m_m3DPoints);
	//double inverse[3][3];
	Mat kInv = k.inv();
	TriangulatePoints(m_vCorrespondencePoints[0], m_vCorrespondencePoints[1],k, kInv, m_mProjectorProjection, m_mCameraProjection, m_vPointCloud);
}

Mat_<double> CDecoder::LinearLSTriangulation(
	Point3d u,//homogenous image point (u,v,1)
	Matx34d P,//camera 1 matrix
	Point3d u1,//homogenous image point in 2nd camera
	Matx34d P1//camera 2 matrix
	)
{
	//build A matrix
	Matx43d A(u.x*P(2, 0) - P(0, 0), u.x*P(2, 1) - P(0, 1), u.x*P(2, 2) - P(0, 2),
		u.y*P(2, 0) - P(1, 0), u.y*P(2, 1) - P(1, 1), u.y*P(2, 2) - P(1, 2),
		u1.x*P1(2, 0) - P1(0, 0), u1.x*P1(2, 1) - P1(0, 1), u1.x*P1(2, 2) - P1(0, 2),
		u1.y*P1(2, 0) - P1(1, 0), u1.y*P1(2, 1) - P1(1, 1), u1.y*P1(2, 2) - P1(1, 2)
		);
	//build B vector
	Matx41d B(-(u.x*P(2, 3) - P(0, 3)),
		-(u.y*P(2, 3) - P(1, 3)),
		-(u1.x*P1(2, 3) - P1(0, 3)),
		-(u1.y*P1(2, 3) - P1(1, 3)));

	//solve for X
	Mat_<double> X;
	solve(A, B, X, DECOMP_SVD);

	return X;
}

double CDecoder::TriangulatePoints(
	const vector<Point2f>& pt_set1,
	const vector<Point2f>& pt_set2,
	const Mat&K,
	const Mat&Kinv,
	const Matx34d& P,
	const Matx34d& P1,
	vector<Point3d>& pointcloud)
{
	vector<double> reproj_error;
	int pts_size = pt_set1.size();
	for (unsigned int i = 0; i<pts_size; i++) {
		//convert to normalized homogeneous coordinates
		Point2f kp = pt_set1[i];
		Point3d u(kp.x, kp.y, 1.0);
		Mat_<double> um = Kinv * Mat_<double>(u);
		u.x = um.at<double>(0);
		u.y = um.at<double>(1);
		u.z = um.at<double>(2);
		//u.x = um.at<Point3d>(0).x;
		Point2f kp1 = pt_set2[i];
		Point3d u1(kp1.x, kp1.y, 1.0);
		Mat_<double> um1 = Kinv * Mat_<double>(u1);
		u1.x = um1.at<double>(0);
		u1.y = um1.at<double>(1);
		u1.z = um1.at<double>(2);

		//triangulate
		Mat_<double> X = LinearLSTriangulation(u, P, u1, P1);

		//calculate reprojection error
		Mat_<double> xPt_img = K * Mat(P1) * X;
		Point2f xPt_img_(xPt_img(0) / xPt_img(2), xPt_img(1) / xPt_img(2));
		reproj_error.push_back(norm(xPt_img_ - kp1));

		//store 3D point
		pointcloud.push_back(Point3d(X(0), X(1), X(2)));
	}

	//return mean reprojection error
	Scalar me = mean(reproj_error);
	return me[0];
}