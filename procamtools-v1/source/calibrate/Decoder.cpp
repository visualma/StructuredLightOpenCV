#include "stdafx.h"
#include "Decoder.h"
#include "Capturador.h"
#include "MatrixUtil.h"
#define EPSILON 3.1281928192

CDecoder::CDecoder(COptions* Options) : m_vCaptures(0), m_Info(Options)
{

}

bool CDecoder::Decode(float thres, vector<Mat>& vCaptures)
{
	m_vCaptures = vCaptures;
	CCapturador cap;
	vector<Mat> temp;
	if (m_Info->m_bHorizontal)
	{
		DecodeGray(0, 0.1);
		//Mascarear
		m_mMask[0] = Mat::zeros(m_mGray[0].rows,m_mGray[0].cols, CV_8UC1);
		cv::threshold(m_mGrayError[0], m_mMask[0], thres, 255, CV_THRESH_BINARY);
		m_mMask[0].convertTo(m_mMask[0], CV_16UC1);
		/*
		if (m_Info->m_bPhase)
		{
            vector<Mat>::iterator begin = m_vCaptures.begin() + m_Info->m_nNumPatterns / 2;
            vector<Mat>::iterator end = m_vCaptures.begin() + m_Info->m_nNumPatterns / 2 + m_Info->m_nNumFringes;

			vector<Mat> phaseImgs(begin, end);
			m_mPhaseMap[0] = DecodePhaseImages(phaseImgs, 0);
			UnwrapPhase(m_mPhaseMap[0], m_Info->m_nFringeInterval*m_Info->m_nNumFringes, m_mGray[0], m_mPhaseMap[0], m_mPhaseError[0]);
		}
		else
			*/
		m_mPhaseError[0] = Mat(m_mGray[0].rows, m_mGray[0].cols, CV_32FC1);
		CreateReliableMap(0);
		m_mGray[0] = MaskMat(m_mGray[0], m_mMask[0],true);
		temp.push_back(m_mGrayError[0]);
		temp.push_back(m_mMask[0]);
		//Mat temp1 = Mat(m_mGray[0].rows,m_mGray[0].cols,CV_8UC1);
		//m_mGray[0].convertTo(temp1, CV_8UC1, 255 / 1024.0, 0);
		temp.push_back(m_mGray[0]);
		//temp.push_back(temp1);
		temp.push_back(m_mPhaseError[0]);
		//imshow("Correspondence X", temp1);
	}
	if (m_Info->m_bVertical)
	{
		DecodeGray(1, 0.1);
		//Mascarear
		m_mMask[1] = Mat::zeros(m_mGray[1].rows, m_mGray[1].cols, CV_8UC1);
		cv::threshold(m_mGrayError[1], m_mMask[1], thres, 255, CV_THRESH_BINARY);
		m_mMask[1].convertTo(m_mMask[1], CV_16UC1);
		/*
		if (m_Info->m_bPhase)
		{
            vector<Mat>::iterator begin = m_vCaptures.begin() + m_Info->m_nBasePatterns + m_Info->m_nNumPatterns / 2;
            vector<Mat>::iterator end = m_vCaptures.begin() + m_Info->m_nNumPatterns + m_Info->m_nNumFringes;

			vector<Mat> phaseImgs(begin, end);
			m_mPhaseMap[1] = DecodePhaseImages(phaseImgs, 1);
			UnwrapPhase(m_mPhaseMap[1], m_Info->m_nFringeInterval*m_Info->m_nNumFringes, m_mGray[1], m_mPhaseMap[1], m_mPhaseError[1]);
		}
		else*/
		m_mPhaseError[1] = Mat(m_mGray[1].rows, m_mGray[1].cols, CV_32FC1);
		CreateReliableMap(1);
		m_mGray[1] = MaskMat(m_mGray[1], m_mMask[1],true);
		temp.push_back(m_mGrayError[1]);
		temp.push_back(m_mMask[1]);
		//Mat temp1 = Mat(m_mGray[1].rows, m_mGray[1].cols, CV_8UC1);
		//m_mGray[1].convertTo(temp1, CV_8UC1, 255 / 1024.0, 0);
		//imshow("Correspondence Y", temp1);
		temp.push_back(m_mGray[1]);
		//temp.push_back(temp1);
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
	//m_mGray[0] = MaskMat(m_mGray[0], m_mMask[0],false);
	//m_mGray[1] = MaskMat(m_mGray[1], m_mMask[0],false);
	temp.push_back(m_mMask[0]);
	//cap.SerializeCaptures(temp, "mask");
	return true;
}

void CDecoder::DecodeGray(int dir,float int_threshold)
{
 	int nbits = m_Info->GetNumBits(dir);
	vector<Mat> diff;
	m_mGrayError[dir] = Mat::zeros(cvSize(m_vCaptures[0].cols, m_vCaptures[0].rows), CV_8UC1);
	for (int i = 0; i < nbits; i++)
	{
		if (m_Info->m_bComplementary)
		{
			double maxVal, maxValComp, minVal, minValComp;
			Mat m = m_vCaptures[dir*(m_Info->m_nBasePatterns * 2) + 2 * i].clone();
			Mat m1 = m_vCaptures[dir*(m_Info->m_nBasePatterns * 2) + 2 * i + 1].clone();
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
		else
		{
			double maxVal, minVal;
			Mat m = m_vCaptures[dir*(m_Info->m_nBasePatterns) + i].clone();
			minMaxIdx(m, &minVal, &maxVal);
			diff.insert(diff.begin(), m);
			float threshold = maxVal*int_threshold;
			m_fDivisor[dir] = maxVal / m_Info->GetNumBits(dir);
			Mat result;
			cv::threshold(m, result, threshold, m_fDivisor[dir], CV_THRESH_BINARY);
			m_mGrayError[dir] += result;
		}
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
        if (!moire_phase)
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

Mat CDecoder::MaskMat(Mat& img, Mat& mask,bool b16)
{
	//Mat a = img.clone();
	assert(img.size() == mask.size());
	if (b16)
	{
		for (int i = 0; i < img.cols; i++)
		for (int j = 0; j < img.rows; j++)
		if (mask.at<ushort>(j, i) == 0)
			img.at<ushort>(j, i) = 0;
	}
	else
	{
		for (int i = 0; i < img.cols; i++)
		for (int j = 0; j < img.rows; j++)
		if (mask.at<uchar>(j, i) == 0)
			img.at<ushort>(j, i) = 0;
	}
	return img;
}


/////////////////////////////////////////// CALIBRATE
bool CDecoder::CalibrateRadial()
{
	return false;
}

bool CDecoder::Calibrate(Mat& CameraMatrix,Mat& DistMatrix)
{

    cout<<"k_proj: "<<k_proj<<endl;
	BuildCorrespondence(m_mGray);
    double minVal,maxVal;
    cv::minMaxIdx(m_vCorrespondencePoints[0],&minVal,&maxVal);
    vector<uchar> status;
    m_mFundamentalMatrix = findFundamentalMat(m_vCorrespondencePoints[0], m_vCorrespondencePoints[1], FM_RANSAC, 0.006 * maxVal, 0.99, status);
    cout << "Fundamental Matrix: "<<m_mFundamentalMatrix<<endl;
    cout << "F keeping " << countNonZero(status) << " / " << status.size() << endl;

    vector<Point2f> good_points0,good_points1;
    good_points0 = m_vCorrespondencePoints[0];
    good_points1 = m_vCorrespondencePoints[1];
    m_vCorrespondencePoints[0].clear();
    m_vCorrespondencePoints[1].clear();
    Mat img_orig_matches = Mat(768,1024,CV_8UC1);
    for(int i=0;i<status.size();i++)
    {
        if(status[i])
        {
            m_vCorrespondencePoints[1].push_back(good_points0[i]);
            m_vCorrespondencePoints[0].push_back(good_points1[i]);
            img_orig_matches.at<uchar>(good_points1[i].y,good_points1[i].x) = 255;
        }
    }
    cv::imshow("matches",img_orig_matches);
/*
    double data[] = {1000,0,0.5*1024,
                   0, 1000,0.5*768,
                   0, 0, 1};
    k_proj = Mat(3,3,CV_64FC1,data);
    k_cam = CameraMatrix;
    m_mEssentialMatrix = k_cam.t() * m_mFundamentalMatrix * k_cam;
    if(fabsf(determinant(m_mEssentialMatrix)) > 1e-07) {
        cout << "det(E) != 0 : " << determinant(m_mEssentialMatrix) << "\n";
        return false;
    }

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
        return false;
	}
	vector<Vec3f> lines1;
    computeCorrespondEpilines(m_vCorrespondencePoints[0], 0, m_mFundamentalMatrix, lines1);
	Mat c = m_vCaptures[0].clone();
	for (int i = 0; i<lines1.size() ;i+=1000) {
		Vec3f it = lines1[i];
		// draw the epipolar line between first and last column
		cv::line(c, cv::Point(0, -it[2] / it[1]),
		cv::Point(c.cols, -(it[2] + it[0] * c.cols) / it[1]),
		cv::Scalar(255, 0, 0));
	}
	imshow("imagen", c);
    */
    return true;
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

bool CDecoder::Generate3Dpoints(Mat& k,Mat& distCoef)
{
    Mat_<double> R1(3,3);
    Mat_<double> R2(3,3);
    Mat_<double> t1(1,3);
    Mat_<double> t2(1,3);
    vector<Point2f> corresp;
    vector<Point3f> pcloud1,pcloud;
    Matx34d P1,P;

    k_cam = k;

    for(double a = 1000;a<1001.0;a++)
    {
        double data[] = {a,0,0.5*1024,
                         0, a,0.5*768,
                         0, 0, 1};
        k_proj = Mat(3,3,CV_64FC1,data);
        m_mEssentialMatrix = k_cam.t() * m_mFundamentalMatrix * k_proj;
        if(fabsf(determinant(m_mEssentialMatrix)) < 1e-07 && CMatrixUtil::DecomposeEtoRandT(m_mEssentialMatrix,R1,R2,t1,t2)) {
            cout<<"esta";
            break;
        }
        else
            cout << "det(E) != 0 : " << determinant(m_mEssentialMatrix) << " "<<a<<"\n";
    }
/*
      double data[] = {1000,0,0.5*m_Info->m_nWidth,
                 0, 1000,0.5*m_Info->m_nHeight,
                 0, 0, 1};
    k_proj = Mat(3,3,CV_64FC1,data);
    m_mEssentialMatrix = k_proj.t() * m_mFundamentalMatrix * k_cam;
    if(fabsf(determinant(m_mEssentialMatrix)) > 1e-07) {
        cout << "det(E) != 0 : " << determinant(m_mEssentialMatrix) << "\n";
        return false;
    }
    if (!CMatrixUtil::DecomposeEtoRandT(m_mEssentialMatrix,R1,R2,t1,t2)) return false;
*/
    if(determinant(R1)+1.0 < 1e-09)
    {
        //according to http://en.wikipedia.org/wiki/Essential_matrix#Showing_that_it_is_valid
        cout << "det(R) == -1 ["<<determinant(R1)<<"]: flip E's sign" << endl;
        m_mEssentialMatrix = -m_mEssentialMatrix;
        CMatrixUtil::DecomposeEtoRandT(m_mEssentialMatrix,R1,R2,t1,t2);
    }
    cout<<"R1"<<R1<<endl<<"R2"<<R2<<endl<<"t2"<<t1<<endl<<"t2"<<t2<<endl;
    if (!CMatrixUtil::CheckCoherentRotation(R1))
    {
        cout << "resulting rotation is not coherent\n";
        return false;
    }
    P = Matx34d
            (1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0);
    P1 = Matx34d(R1(0,0),	R1(0,1),	R1(0,2),	t1(0),
    R1(1,0),	R1(1,1),	R1(1,2),	t1(1),
    R1(2,0),	R1(2,1),	R1(2,2),	t1(2));
    vector<Point2f> newPoints0,newPoints1;
    correctMatches(m_mFundamentalMatrix,m_vCorrespondencePoints[1],m_vCorrespondencePoints[0],newPoints1,newPoints0);
    m_vCorrespondencePoints[0] = newPoints0;
    m_vCorrespondencePoints[1] = newPoints1;
    vector<Mat> output;
    //triangulatePoints(k_proj,k_cam,m_vCorrespondencePoints[1],m_vCorrespondencePoints[0],output);
    cout << "Testing P1 " << endl << Mat(P1) << endl;
    double reproj_error1 = TriangulatePointsIterative(m_vCorrespondencePoints[0],m_vCorrespondencePoints[1],k,k.inv(),distCoef,P,P1,pcloud,corresp);
    double reproj_error2 = TriangulatePointsIterative(m_vCorrespondencePoints[1],m_vCorrespondencePoints[0],k,k.inv(),distCoef,P1,P,pcloud1,corresp);
    vector<uchar> tmp_status;
    if (!TestTriangulation(pcloud,P1,tmp_status) || !TestTriangulation(pcloud1,P,tmp_status) || reproj_error1 > 100.0 || reproj_error2 > 100.0)
    {
        P1 = Matx34d(R1(0,0),	R1(0,1),	R1(0,2),	t2(0),
        R1(1,0),	R1(1,1),	R1(1,2),	t2(1),
        R1(2,0),	R1(2,1),	R1(2,2),	t2(2));
        cout << "Testing P1 "<< endl << Mat(P1) << endl;

        pcloud.clear(); pcloud1.clear(); corresp.clear();
        reproj_error1 = TriangulatePointsIterative(m_vCorrespondencePoints[0],m_vCorrespondencePoints[1],k,k.inv(),distCoef,P,P1,pcloud,corresp);
        reproj_error2 = TriangulatePointsIterative(m_vCorrespondencePoints[1],m_vCorrespondencePoints[0],k,k.inv(),distCoef,P1,P,pcloud1,corresp);
        if (!TestTriangulation(pcloud,P1,tmp_status) || !TestTriangulation(pcloud1,P,tmp_status) || reproj_error1 > 100.0 || reproj_error2 > 100.0)
        {
            if (!CMatrixUtil::CheckCoherentRotation(R2))
            {
                cout << "resulting rotation is not coherent\n";
                P1 = 0;
                return false;
            }
            P1 = Matx34d(R2(0,0),	R2(0,1),	R2(0,2),	t1(0),
                            R2(1,0),	R2(1,1),	R2(1,2),	t1(1),
                            R2(2,0),	R2(2,1),	R2(2,2),	t1(2));
            cout << "Testing P1 "<< endl << Mat(P1) << endl;

            pcloud.clear(); pcloud1.clear(); corresp.clear();
            reproj_error1 = TriangulatePointsIterative(m_vCorrespondencePoints[0],m_vCorrespondencePoints[1],k,k.inv(),distCoef,P,P1,pcloud,corresp);
            reproj_error2 = TriangulatePointsIterative(m_vCorrespondencePoints[1],m_vCorrespondencePoints[0],k,k.inv(),distCoef,P1,P,pcloud1,corresp);

            if (!TestTriangulation(pcloud,P1,tmp_status) || !TestTriangulation(pcloud1,P,tmp_status) || reproj_error1 > 100.0 || reproj_error2 > 100.0)
            {
                P1 = Matx34d(R2(0,0),	R2(0,1),	R2(0,2),	t2(0),
                            R2(1,0),	R2(1,1),	R2(1,2),	t2(1),
                            R2(2,0),	R2(2,1),	R2(2,2),	t2(2));
                cout << "Testing P1 "<< endl << Mat(P1) << endl;

                pcloud.clear(); pcloud1.clear(); corresp.clear();
                reproj_error1 = TriangulatePointsIterative(m_vCorrespondencePoints[0],m_vCorrespondencePoints[1],k,k.inv(),distCoef,P,P1,pcloud,corresp);
                reproj_error2 = TriangulatePointsIterative(m_vCorrespondencePoints[1],m_vCorrespondencePoints[0],k,k.inv(),distCoef,P1,P,pcloud1,corresp);
                if (!TestTriangulation(pcloud,P1,tmp_status) || !TestTriangulation(pcloud1,P,tmp_status) || reproj_error1 > 100.0 || reproj_error2 > 100.0) {
                    cout << "No se lograron resolver las ambiguedades, abortando." << endl;
                    return false;
                }
            }
        }
    }
    for (unsigned int i=0; i<pcloud.size(); i++)
        m_vPointCloud.push_back(pcloud[i]);
    return true;
}


double CDecoder::TriangulatePoints(
    const vector<Point2f>& pt_set1,
    const vector<Point2f>& pt_set2,
    const Mat&K,
    const Mat&Kinv,
    const Matx34d& P,
    const Matx34d& P1,
    vector<Point3f>& pointcloud)
{
	vector<double> reproj_error;
	int pts_size = pt_set1.size();
	for (unsigned int i = 0; i<pts_size; i++) {
		//convert to normalized homogeneous coordinates
		Point2f kp = pt_set1[i];
        Point3f u(kp.x, kp.y, 1.0);
		Mat_<double> um = Kinv * Mat_<double>(u);
		u.x = um.at<double>(0);
		u.y = um.at<double>(1);
		u.z = um.at<double>(2);
		Point2f kp1 = pt_set2[i];
        Point3f u1(kp1.x, kp1.y, 1.0);
		Mat_<double> um1 = Kinv * Mat_<double>(u1);
		u1.x = um1.at<double>(0);
		u1.y = um1.at<double>(1);
		u1.z = um1.at<double>(2);

		//triangulate
		Mat_<double> X = LinearLSTriangulation(u, P, u1, P1);

        //calculate reprojection error
        //Mat_<double> xPt_img = K * Mat(P1);
		//xPt_img*=X;
		//Point2f xPt_img_(xPt_img(0) / xPt_img(2), xPt_img(1) / xPt_img(2));
		//reproj_error.push_back(norm(xPt_img_ - kp1));

		//store 3D point
        pointcloud.push_back(Point3f(X(0), X(1), X(2)));
	}

	//return mean reprojection error
	Scalar me = mean(reproj_error);
	return 0;// me[0];
}

bool CDecoder::TestTriangulation(const vector<Point3f>& pcloud, const Matx34d& P, vector<uchar>& status) {
    vector<Point3f> pcloud_pt3d = pcloud;
    vector<Point3f> pcloud_pt3d_projected(pcloud_pt3d.size());
    Matx44d P4x4 = Matx44d::eye();
	for (int i = 0; i<12; i++) P4x4.val[i] = P.val[i];

	perspectiveTransform(pcloud_pt3d, pcloud_pt3d_projected, P4x4);

	status.resize(pcloud.size(), 0);
	for (int i = 0; i<pcloud.size(); i++) {
		status[i] = (pcloud_pt3d_projected[i].z > 0) ? 1 : 0;
	}
	int count = countNonZero(status);

	double percentage = ((double)count / (double)pcloud.size());
	cout << count << "/" << pcloud.size() << " = " << percentage*100.0 << "% are in front of camera" << endl;
	if (percentage < 0.75)
		return false; //less than 75% of the points are in front of the camera

	//check for coplanarity of points
	if (false) //not
	{
		cv::Mat_<double> cldm(pcloud.size(), 3);
		for (unsigned int i = 0; i<pcloud.size(); i++) {
            cldm.row(i)(0) = pcloud[i].x;
			cldm.row(i)(1) = pcloud[i].y;
			cldm.row(i)(2) = pcloud[i].z;
		}
		cv::Mat_<double> mean;
		cv::PCA pca(cldm, mean, CV_PCA_DATA_AS_ROW);

		int num_inliers = 0;
		cv::Vec3d nrm = pca.eigenvectors.row(2); nrm = nrm / norm(nrm);
		cv::Vec3d x0 = pca.mean;
		double p_to_plane_thresh = sqrt(pca.eigenvalues.at<double>(2));

		for (int i = 0; i<pcloud.size(); i++) {
            Vec3d w = Vec3d(pcloud[i].x,pcloud[i].y,pcloud[i].z) - x0;
			double D = fabs(nrm.dot(w));
			if (D < p_to_plane_thresh) num_inliers++;
		}

		cout << num_inliers << "/" << pcloud.size() << " are coplanar" << endl;
		if ((double)num_inliers / (double)(pcloud.size()) > 0.85)
			return false;
	}

	return true;
}

double CDecoder::TriangulatePointsIterative(const vector<Point2f>& pt_set1,
const vector<Point2f>& pt_set2,
const Mat& K,
const Mat& Kinv,
const Mat& distcoeff,
const Matx34d& P,
const Matx34d& P1,
vector<Point3f>& pointcloud,
vector<Point2f>& correspImg1Pt)
{
    #ifdef __SFM__DEBUG__
    vector<double> depths;
    #endif

    // pointcloud.clear();
    correspImg1Pt.clear();

    Matx44d P1_(P1(0,0),P1(0,1),P1(0,2),P1(0,3),
    P1(1,0),P1(1,1),P1(1,2),P1(1,3),
    P1(2,0),P1(2,1),P1(2,2),P1(2,3),
    0,	0,	0,	1);
    Matx44d P1inv(P1_.inv());

    cout << "Triangulating...";
    double t = getTickCount();
    vector<double> reproj_error;
    unsigned int pts_size = pt_set1.size();

#if 0
    //Using OpenCV's triangulation
    //convert to Point2f
    vector<Point2f> _pt_set1_pt,_pt_set2_pt;
    //KeyPointsToPoints(pt_set1,_pt_set1_pt);
    //KeyPointsToPoints(pt_set2,_pt_set2_pt);
    _pt_set1_pt = pt_set1;
    _pt_set2_pt = pt_set2;
    //undistort
    Mat pt_set1_pt,pt_set2_pt;
    undistortPoints(_pt_set1_pt, pt_set1_pt, K, distcoeff);
    undistortPoints(_pt_set2_pt, pt_set2_pt, K, distcoeff);

    //triangulate
    Mat pt_set1_pt_2r = pt_set1_pt.reshape(1, 2);
    Mat pt_set2_pt_2r = pt_set2_pt.reshape(1, 2);
    Mat pt_3d_h(1,pts_size,CV_32FC4);
    cv::triangulatePoints(P,P1,pt_set1_pt_2r,pt_set2_pt_2r,pt_3d_h);

    //calculate reprojection
    vector<Point3f> pt_3d;
    convertPointsHomogeneous(pt_3d_h.reshape(4, 1), pt_3d);
    cv::Mat_<double> R = (cv::Mat_<double>(3,3) << P(0,0),P(0,1),P(0,2), P(1,0),P(1,1),P(1,2), P(2,0),P(2,1),P(2,2));
    Vec3d rvec; Rodrigues(R ,rvec);
    Vec3d tvec(P(0,3),P(1,3),P(2,3));
    vector<Point2f> reprojected_pt_set1;
    projectPoints(pt_3d,rvec,tvec,K,distcoeff,reprojected_pt_set1);

    for (unsigned int i=0; i<pts_size; i++) {
        Point3f cp;
        cp = pt_3d[i];
        pointcloud.push_back(cp);
        reproj_error.push_back(norm(_pt_set1_pt[i]-reprojected_pt_set1[i]));
    }

#else
    Mat_<double> KP1 = K * Mat(P1);

#pragma omp parallel for num_threads(1)
    for (int i=0; i<pts_size; i++)
    {
        Point2f kp = pt_set1[i];
        Point3f u(kp.x,kp.y,1.0);
        Mat_<double> um = Kinv * Mat_<double>(u);
        u.x = um(0); u.y = um(1); u.z = um(2);

        Point2f kp1 = pt_set2[i];
        Point3f u1(kp1.x,kp1.y,1.0);
        Mat_<double> um1 = Kinv * Mat_<double>(u1);
        u1.x = um1(0); u1.y = um1(1); u1.z = um1(2);

        Mat_<double> X = IterativeLinearLSTriangulation(u,P,u1,P1);

        // cout << "3D Point: " << X << endl;
        // Mat_<double> x = Mat(P1) * X;
        // cout << "P1 * Point: " << x << endl;
        // Mat_<double> xPt = (Mat_<double>(3,1) << x(0),x(1),x(2));
        // cout << "Point: " << xPt << endl;
        Mat_<double> xPt_img = KP1 * X;	//reproject
        // cout << "Point * K: " << xPt_img << endl;
        Point2f xPt_img_(xPt_img(0)/xPt_img(2),xPt_img(1)/xPt_img(2));

    #pragma omp critical
        {
            double reprj_err = norm(xPt_img_-kp1);
            reproj_error.push_back(reprj_err);

            Point3f cp = Point3f(X(0),X(1),X(2));

            pointcloud.push_back(cp);
            correspImg1Pt.push_back(pt_set1[i]);

    #ifdef __SFM__DEBUG__
            depths.push_back(X(2));

    #endif
        }
    }

#endif

    Scalar mse = mean(reproj_error);
    t = ((double)getTickCount() - t)/getTickFrequency();
    cout << "Done. ("<<pointcloud.size()<<"points, " << t <<"s, mean reproj err = " << mse[0] << ")"<< endl;

    //show "range image"
    #ifdef __SFM__DEBUG__
    {
        double minVal,maxVal;
        minMaxLoc(depths, &minVal, &maxVal);
        Mat tmp(240,320,CV_8UC3,Scalar(0,0,0)); //cvtColor(img_1_orig, tmp, CV_BGR2HSV);
        for (unsigned int i=0; i<pointcloud.size(); i++) {
        double _d = MAX(MIN((pointcloud[i].z-minVal)/(maxVal-minVal),1.0),0.0);
        circle(tmp, correspImg1Pt[i].pt, 1, Scalar(255 * (1.0-(_d)),255,255), CV_FILLED);
    }
    cvtColor(tmp, tmp, CV_HSV2BGR);
    imshow("Depth Map", tmp);
    waitKey(0);
    destroyWindow("Depth Map");
    }

#endif

return mse[0];
}

Mat_<double> CDecoder::IterativeLinearLSTriangulation(Point3f u,	//homogenous image point (u,v,1)
	Matx34d P,			//camera 1 matrix
    Point3f u1,			//homogenous image point in 2nd camera
	Matx34d P1			//camera 2 matrix
	) {
	
	double wi = 1, wi1 = 1;
	Mat_<double> X(4, 1);
	for (int i = 0; i<10; i++) { //Hartley suggests 10 iterations at most
		Mat_<double> X_ = LinearLSTriangulation(u, P, u1, P1);
		X(0) = X_(0); X(1) = X_(1); X(2) = X_(2); X(3) = 1.0;

		//recalculate weights
		double p2x = Mat_<double>(Mat_<double>(P).row(2)*X)(0);
		double p2x1 = Mat_<double>(Mat_<double>(P1).row(2)*X)(0);

		//breaking point
		if (fabsf(wi - p2x) <= EPSILON && fabsf(wi1 - p2x1) <= EPSILON) break;

		wi = p2x;
		wi1 = p2x1;

		//reweight equations and solve
		Matx43d A((u.x*P(2, 0) - P(0, 0)) / wi, (u.x*P(2, 1) - P(0, 1)) / wi, (u.x*P(2, 2) - P(0, 2)) / wi,
			(u.y*P(2, 0) - P(1, 0)) / wi, (u.y*P(2, 1) - P(1, 1)) / wi, (u.y*P(2, 2) - P(1, 2)) / wi,
			(u1.x*P1(2, 0) - P1(0, 0)) / wi1, (u1.x*P1(2, 1) - P1(0, 1)) / wi1, (u1.x*P1(2, 2) - P1(0, 2)) / wi1,
			(u1.y*P1(2, 0) - P1(1, 0)) / wi1, (u1.y*P1(2, 1) - P1(1, 1)) / wi1, (u1.y*P1(2, 2) - P1(1, 2)) / wi1
			);
		Mat_<double> B = (Mat_<double>(4, 1) << -(u.x*P(2, 3) - P(0, 3)) / wi,
			-(u.y*P(2, 3) - P(1, 3)) / wi,
			-(u1.x*P1(2, 3) - P1(0, 3)) / wi1,
			-(u1.y*P1(2, 3) - P1(1, 3)) / wi1
			);

		solve(A, B, X_, DECOMP_SVD);
		X(0) = X_(0); X(1) = X_(1); X(2) = X_(2); X(3) = 1.0;
	}
	return X;
}

Mat_<double> CDecoder::LinearLSTriangulation(Point3f u,		//homogenous image point (u,v,1)
	Matx34d P,		//camera 1 matrix
    Point3f u1,		//homogenous image point in 2nd camera
	Matx34d P1		//camera 2 matrix
	)
{

	//build matrix A for homogenous equation system Ax = 0
	//assume X = (x,y,z,1), for Linear-LS method
	//which turns it into a AX = B system, where A is 4x3, X is 3x1 and B is 4x1
	//	cout << "u " << u <<", u1 " << u1 << endl;
	//	Matx<double,6,4> A; //this is for the AX=0 case, and with linear dependence..
	//	A(0) = u.x*P(2)-P(0);
	//	A(1) = u.y*P(2)-P(1);
	//	A(2) = u.x*P(1)-u.y*P(0);
	//	A(3) = u1.x*P1(2)-P1(0);
	//	A(4) = u1.y*P1(2)-P1(1);
	//	A(5) = u1.x*P(1)-u1.y*P1(0);
	//	Matx43d A; //not working for some reason...
	//	A(0) = u.x*P(2)-P(0);
	//	A(1) = u.y*P(2)-P(1);
	//	A(2) = u1.x*P1(2)-P1(0);
	//	A(3) = u1.y*P1(2)-P1(1);
	Matx43d A(u.x*P(2, 0) - P(0, 0), u.x*P(2, 1) - P(0, 1), u.x*P(2, 2) - P(0, 2),
		u.y*P(2, 0) - P(1, 0), u.y*P(2, 1) - P(1, 1), u.y*P(2, 2) - P(1, 2),
		u1.x*P1(2, 0) - P1(0, 0), u1.x*P1(2, 1) - P1(0, 1), u1.x*P1(2, 2) - P1(0, 2),
		u1.y*P1(2, 0) - P1(1, 0), u1.y*P1(2, 1) - P1(1, 1), u1.y*P1(2, 2) - P1(1, 2)
		);
	Matx41d B(-(u.x*P(2, 3) - P(0, 3)),
		-(u.y*P(2, 3) - P(1, 3)),
		-(u1.x*P1(2, 3) - P1(0, 3)),
		-(u1.y*P1(2, 3) - P1(1, 3)));

	Mat_<double> X;
	solve(A, B, X, DECOMP_SVD);

	return X;
}

