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

#undef min
#undef max

void print_usage(const char *argv0)
{
	const char *prog = strrchr(argv0, '\\');
	if (prog)
		prog++;
	else
		prog = argv0;
	printf("Usage: %s <options> <h.map> <v.map> <mask.bmp>\n",prog);
	exit(-1);
}

int main(int argc, char *argv[]) 
{
	try 
	{
		// parse commandline options
		//if (argc!=5) print_usage(argv[0]);
		COptions* options = new COptions(1024, 768, 10, 4, true, true, true, false);
		options_t opt;
		opt.debug = true;
		string ruta = "../resources/Patterns/pattern-0";
		
		CCapturador* cap = new CCapturador(options, ruta);
		//cap->CapturePatterns(500);
		cap->LoadCapturesFromFiles("../resources/Captures/V4GP/Capture-");
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
		
		slib::Field<2, float> horizontal("gray-h.bmp");
		slib::Field<2, float> vertical("gray-v.bmp");
		slib::Field<2, float> mask;
		slib::image::Read(mask, "mask.bmp");
		//calib.Calibrate(horizontal, vertical, mask);
		calib.Calibrate(m_phase_map[0], m_phase_map[1], m_mask);
		calib.WriteCamIntrinsic("cam-intrinsic.txt");
		calib.WriteCamDistortion("cam-distortion.txt");
		calib.WriteProIntrinsic("pro-intrinsic.txt");
		calib.WriteProDistortion("pro-distortion.txt");
		calib.WriteProExtrinsic("pro-extrinsic.txt");

		cvWaitKey();
	} 
	catch (const std::exception& e) 
	{
		fprintf(stderr,"error: %s\n", e.what());
		return -1;
	}

	return 0;
}

