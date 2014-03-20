//#include "stdafx.h"
#include "MatrixUtil.h"

//#define DECOMPOSE_SVD
#define intrpmnmx(val,min,max) (max==min ? 0.0 : ((val)-min)/(max-min))
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

void CMatrixUtil::DecomposeEssentialUsingHorn90(double _E[9], double _R1[9], double _R2[9], double _t1[3], double _t2[3]) {
//from : http://people.csail.mit.edu/bkph/articles/Essential.pdf
    #ifdef USE_EIGEN
    using namespace Eigen;

    Matrix3d E = Map<Matrix<double,3,3,RowMajor> >(_E);
    Matrix3d EEt = E * E.transpose();
    Vector3d e0e1 = E.col(0).cross(E.col(1)),e1e2 = E.col(1).cross(E.col(2)),e2e0 = E.col(2).cross(E.col(2));
    Vector3d b1,b2;

    #if 1
    //Method 1
    Matrix3d bbt = 0.5 * EEt.trace() * Matrix3d::Identity() - EEt; //Horn90 (12)
    Vector3d bbt_diag = bbt.diagonal();
    if (bbt_diag(0) > bbt_diag(1) && bbt_diag(0) > bbt_diag(2))
    {
        b1 = bbt.row(0) / sqrt(bbt_diag(0));
        b2 = -b1;
    }
    else
        if (bbt_diag(1) > bbt_diag(0) && bbt_diag(1) > bbt_diag(2))
        {
            b1 = bbt.row(1) / sqrt(bbt_diag(1));
            b2 = -b1;
        }
        else
        {
            b1 = bbt.row(2) / sqrt(bbt_diag(2));
            b2 = -b1;
        }
    #else
    //Method 2
    if (e0e1.norm() > e1e2.norm() && e0e1.norm() > e2e0.norm())
    {
        b1 = e0e1.normalized() * sqrt(0.5 * EEt.trace()); //Horn90 (18)
        b2 = -b1;
    }
    else
        if (e1e2.norm() > e0e1.norm() && e1e2.norm() > e2e0.norm())
        {
            b1 = e1e2.normalized() * sqrt(0.5 * EEt.trace()); //Horn90 (18)
            b2 = -b1;
        }
        else
        {
            b1 = e2e0.normalized() * sqrt(0.5 * EEt.trace()); //Horn90 (18)
            b2 = -b1;
        }
    #endif

    //Horn90 (19)
    Matrix3d cofactors; cofactors.col(0) = e1e2; cofactors.col(1) = e2e0; cofactors.col(2) = e0e1;
    cofactors.transposeInPlace();

    //B = [b]_x , see Horn90 (6) and http://en.wikipedia.org/wiki/Cross_product#Conversion_to_matrix_multiplication
    Matrix3d B1; B1 <<	0,-b1(2),b1(1),
    b1(2),0,-b1(0),
    -b1(1),b1(0),0;

    Matrix3d B2; B2 <<	0,-b2(2),b2(1),
    b2(2),0,-b2(0),
    -b2(1),b2(0),0;

    Map<Matrix<double,3,3,RowMajor> > R1(_R1),R2(_R2);

    //Horn90 (24)
    R1 = (cofactors.transpose() - B1*E) / b1.dot(b1);
    R2 = (cofactors.transpose() - B2*E) / b2.dot(b2);
    Map<Vector3d> t1(_t1),t2(_t2);
    t1 = b1; t2 = b2;

    cout << "Horn90 provided " << endl << R1 << endl << "and" << endl << R2 << endl;
    #endif
}



bool CMatrixUtil::DecomposeEtoRandT(
Mat_<double>& E,
Mat_<double>& R1,
Mat_<double>& R2,
Mat_<double>& t1,
Mat_<double>& t2)
{

#ifdef DECOMPOSE_SVD
    //Using HZ E decomposition
    Mat svd_u, svd_vt, svd_w;
    TakeSVDOfE(E,svd_u,svd_vt,svd_w);
    cout<<"svd_w = "<<svd_w<<endl;
    //check if first and second singular values are the same (as they should be)
   // svd_w.at<double>(1) = svd_w.at<double>(0);
    double singular_values_ratio = fabsf(svd_w.at<double>(0) / svd_w.at<double>(1));

    if(singular_values_ratio>1.0) singular_values_ratio = 1.0/singular_values_ratio; // flip ratio to keep it [0,1]

    if (singular_values_ratio < 0.7)
    {
        cout << "singular values are too far apart" <<singular_values_ratio<<" \n";
        return false;
    }

    Matx33d W(0,-1,0,	//HZ 9.13
            1,0,0,
            0,0,1);

    Matx33d Wt(0,1,0,
            -1,0,0,
            0,0,1);
    R1 = svd_u * Mat(W) * svd_vt; //HZ 9.19
    R2 = svd_u * Mat(Wt) * svd_vt; //HZ 9.19
    t1 = svd_u.col(2); //u3
    t2 = -svd_u.col(2); //u3

#else
    //Using Horn E decomposition
    DecomposeEssentialUsingHorn90(E[0],R1[0],R2[0],t1[0],t2[0]);

#endif
    return true;
}

void CMatrixUtil::TakeSVDOfE(Mat_<double>& E, Mat& svd_u, Mat& svd_vt, Mat& svd_w) {
#if 1
    //Using OpenCV's SVD
    SVD svd(E,SVD::MODIFY_A);
    svd_u = svd.u;
    svd_vt = svd.vt;
    svd_w = svd.w;
#else
    //Using Eigen's SVD
    cout << "Eigen3 SVD..\n";
    Eigen::Matrix3f e = Eigen::Map<Eigen::Matrix<double,3,3,Eigen::RowMajor> >((double*)E.data).cast<float>();
    Eigen::JacobiSVD<Eigen::MatrixXf> svd(e, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::MatrixXf Esvd_u = svd.matrixU();
    Eigen::MatrixXf Esvd_v = svd.matrixV();
    svd_u = (Mat_<double>(3,3) << Esvd_u(0,0), Esvd_u(0,1), Esvd_u(0,2),
    Esvd_u(1,0), Esvd_u(1,1), Esvd_u(1,2),
    Esvd_u(2,0), Esvd_u(2,1), Esvd_u(2,2));
    Mat_<double> svd_v = (Mat_<double>(3,3) << Esvd_v(0,0), Esvd_v(0,1), Esvd_v(0,2),
    Esvd_v(1,0), Esvd_v(1,1), Esvd_v(1,2),
    Esvd_v(2,0), Esvd_v(2,1), Esvd_v(2,2));
    svd_vt = svd_v.t();
    svd_w = (Mat_<double>(1,3) << svd.singularValues()[0] , svd.singularValues()[1] , svd.singularValues()[2]);
#endif

    //cout << "----------------------- SVD ------------------------\n";
   // cout << "U:\n"<<svd_u<<"\nW:\n"<<svd_w<<"\nVt:\n"<<svd_vt<<endl;
   // cout << "----------------------------------------------------\n";
}

void CMatrixUtil::drawArrows(Mat& frame, const vector<Point2f>& prevPts, const vector<Point2f>& nextPts, const vector<uchar>& status, const vector<float>& verror, const Scalar& _line_color)
{
double minVal,maxVal; minMaxIdx(verror,&minVal,&maxVal,0,0,status);
int line_thickness = 1;

    for (size_t i = 0; i < prevPts.size(); ++i)
    {
        if (status[i])
        {
            double alpha = intrpmnmx(verror[i],minVal,maxVal); alpha = 1.0 - alpha;
            Scalar line_color(alpha*_line_color[0],
            alpha*_line_color[1],
            alpha*_line_color[2]);

            Point p = prevPts[i];
            Point q = nextPts[i];

            double angle = atan2((double) p.y - q.y, (double) p.x - q.x);

            double hypotenuse = sqrt( (double)(p.y - q.y)*(p.y - q.y) + (double)(p.x - q.x)*(p.x - q.x) );

            if (hypotenuse < 1.0)
                continue;

            // Here we lengthen the arrow by a factor of three.
            q.x = (int) (p.x - 3 * hypotenuse * cos(angle));
            q.y = (int) (p.y - 3 * hypotenuse * sin(angle));

            // Now we draw the main line of the arrow.
            line(frame, p, q, line_color, line_thickness);

            // Now draw the tips of the arrow. I do some scaling so that the
            // tips look proportional to the main line of the arrow.

            p.x = (int) (q.x + 9 * cos(angle + CV_PI / 4));
            p.y = (int) (q.y + 9 * sin(angle + CV_PI / 4));
            line(frame, p, q, line_color, line_thickness);

            p.x = (int) (q.x + 9 * cos(angle - CV_PI / 4));
            p.y = (int) (q.y + 9 * sin(angle - CV_PI / 4));
            line(frame, p, q, line_color, line_thickness);
        }
    }
}
