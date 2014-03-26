//
// Copyright (c) 2009-2011  Shuntaro Yamazaki (shun-yamazaki (at) aist.go.jp) 
//  and the National Institute of Advanced Industrial Science and Technology   
//
// $Id: main.cpp 4590 2011-05-30 22:13:45Z shun $
//

#include "stdafx.h"

#include "Field.h"
#include "ImageBmpIO.h"

#include "calibrate.h"
#include "Decoder.h"
#include "Capturador.h"
#include "../triangulate/triangulate.cpp"
#undef min
#undef max

int main(int argc, char *argv[]) 
{
	try 
	{
		COptions* options = new COptions(1024, 768, 10, 4, true, true, true, false);
		options_t opt;
		opt.debug = true;
		string ruta = "../resources/Patterns/pattern-0";
		
		CCapturador* cap = new CCapturador(options, ruta);
		cap->CapturePatterns(500);
		string ruta2 = cap->SerializeCaptures(cap->m_vCaptures, "CasaV2");
		//cap->LoadCapturesFromFiles("../resources/Captures/V4GP/Capture-");
 		CDecoder* decoder = new CDecoder(options, cap->m_vCaptures);
		decoder->Decode();
		CProCamCalibrate calib(opt);
		
		slib::Field<2, float> m_phase_map[2];
		for (int k = 0; k < 2; k++)
		{
			m_phase_map[k].Initialize(decoder->m_mGray[k].cols, decoder->m_mGray[k].rows);
			for (int i = 0; i < decoder->m_mGray[k].cols; i++)
			for (int j = 0; j < decoder->m_mGray[k].rows; j++)
				m_phase_map[k].cell(i, j) = decoder->m_mGray[k].at<ushort>(j, i);
		}
		slib::Field<2, float> m_mask;
		m_mask.Initialize(decoder->m_mMask[1].cols, decoder->m_mMask[1].rows);
		for (int i = 0; i < decoder->m_mMask[1].cols; i++)
		for (int j = 0; j < decoder->m_mMask[1].rows; j++)
			m_mask.cell(i, j) = decoder->m_mMask[1].at<ushort>(j, i);
		
		calib.Calibrate(m_phase_map[0], m_phase_map[1], m_mask);
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
		std::ostringstream oss8;
		oss8 << ruta2 << "//mesh.ply";
		makeTriangulation(opt, m_phase_map[0], m_phase_map[1], m_mask, calib.m_pro_int, calib.m_cam_int, calib.m_pro_ext, calib.m_pro_dist, calib.m_cam_dist,oss8.str());
		cvWaitKey();
	} 
	catch (const std::exception& e) 
	{
		fprintf(stderr,"error: %s\n", e.what());
		return -1;
	}

	return 0;
}

