//
// Copyright (c) 2009-2011  Shuntaro Yamazaki (shun-yamazaki (at) aist.go.jp) 
//  and the National Institute of Advanced Industrial Science and Technology   
//
// $Id: main.cpp 4590 2011-05-30 22:13:45Z shun $
//

#include "stdafx.h"

#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <System.Data.dll>
using namespace System::Windows::Forms;

#include "MyForm.h"
using namespace calibrate;

[STAThreadAttribute]
int WinMain() 
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Crear la ventana principal y ejecutarla
	Application::Run(gcnew calibrate::MyForm());
	/*
	try 
	{
		COptions* options = new COptions(1024, 768, 10, 4, true, true, true, false);
		options_t opt;
		opt.debug = true;
		string ruta = "../resources/Patterns/pattern-0";
		
		CCapturador* cap = new CCapturador(options, ruta);
		//cap->CapturePatterns(500);
		//string ruta2 = cap->SerializeCaptures(cap->m_vCaptures, "CasaV2");
		cap->LoadCapturesFromFiles("../resources/Captures/CasaV2-1395819069/Capture-");
 		CDecoder* decoder = new CDecoder(options, cap->m_vCaptures);
		decoder->Decode();
		CProCamCalibrate calib(opt);
		
		cvWaitKey();
		cvDestroyAllWindows();
		slib::Field<2, float> m_mask;
		//Mat mask = imread("mascara.bmp", CV_LOAD_IMAGE_GRAYSCALE);
		//mask.convertTo(mask, CV_8U);
		//printf("chanesl %d %d", mask.channels(), decoder->m_mGray[0].channels());
		
		m_mask.Initialize(decoder->m_mMask[1].cols, decoder->m_mMask[1].rows);
		for (int i = 0; i < decoder->m_mMask[1].cols; i++)
		for (int j = 0; j < decoder->m_mMask[1].rows; j++)
		m_mask.cell(i, j) = decoder->m_mMask[1].at<ushort>(j, i);
		
		/*
		m_mask.Initialize(mask.cols, mask.rows);
		for (int i = 0; i < mask.cols; i++)
		for (int j = 0; j < mask.rows; j++)
			m_mask.cell(i, j) = mask.at<uchar>(j, i);
		*/
	/*
	slib::Field<2, float> m_phase_map[2];
		for (int k = 0; k < 2; k++)
		{
			//Mat temp;
			//decoder->m_mGray[k].copyTo(temp, mask);
			//decoder->m_mGray[k] = temp.clone();
			m_phase_map[k].Initialize(decoder->m_mGray[k].cols, decoder->m_mGray[k].rows);
			for (int i = 0; i < decoder->m_mGray[k].cols; i++)
			for (int j = 0; j < decoder->m_mGray[k].rows; j++)
				m_phase_map[k].cell(i, j) = decoder->m_mGray[k].at<ushort>(j, i);
		}
		//Mat temp1 = Mat(decoder->m_mGray[1].rows, decoder->m_mGray[1].cols, CV_8UC1);
		//decoder->m_mGray[1].convertTo(temp1, CV_8UC1, 255 / 1024.0, 0);
		//imshow("mascara1", temp1);
		//imshow("mascara2", mask);
		cvWaitKey();
		cvDestroyAllWindows();
		calib.Calibrate(m_phase_map[0], m_phase_map[1], m_mask);
		*/
		/*
		std::ostringstream oss3;
		oss3 << ruta2 << "//cam-intrinsic.txt";
		calib.WriteCamIntrinsic(oss3.str());
		std::ostringstream oss4;
		oss4 << ruta2 << "//cam-distortion.txt";
		calib.WriteCamDistortion(oss4.str());
		std::ostringstream oss5;
		oss5 <<  ruta2 << "//pro-intrinsic.txt";
		calib.WriteProIntrinsic(oss5.str());
		std::ostringstream oss6;
		oss6 <<  ruta2 << "//pro-distortion.txt";
		calib.WriteProDistortion(oss6.str());
		std::ostringstream oss7;
		oss7 << ruta2 << "//pro-extrinsic.txt";
		calib.WriteProExtrinsic(oss7.str());
		*/
	/*
		std::ostringstream oss8;
		oss8 << "mesh1.ply";
		
		makeTriangulation(opt, m_phase_map[0], m_phase_map[1], m_mask, calib.m_pro_int, calib.m_cam_int, calib.m_pro_ext, calib.m_pro_dist, calib.m_cam_dist,oss8.str());
		Renderer a;
		a.render("mesh1.ply");
		cvWaitKey();
	} 
	catch (const std::exception& e) 
	{
		fprintf(stderr,"error: %s\n", e.what());
		return -1;
	}
	return 0;
	*/
}


