// StructuredOpenCV_V1.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include "Capturador.h"
#include "Decoder.h"
#include "CameraCalibrator.h"
#include "MatrixUtil.h"
using namespace cv;
int main123(int, char**)
{
	COptions* options = new COptions(1024, 768, 10,4,true,true,true,false);

	string ruta = "Patterns/pattern-0";
	CCameraCalibrator* cal = new CCameraCalibrator();
	Mat dist, camera;
	//cal->Calibrate("in_calib.xml", camera, dist);
	cal->GetCalibrationFromFile("out_camera_data3.xml", camera, dist);
	
	CCapturador* cap = new CCapturador(options,ruta);
	
	//cap->CapturePatternsUndisorted(camera,dist,250);
	cap->LoadCapturesFromFilesUndisorted("Captures/V4GP/Capture-",camera,dist);
	//cap->LoadCapturesFromFiles("Patterns/pattern-0");
	CDecoder* decoder = new CDecoder(options, cap->m_vCaptures);
	decoder->Decode();
	/*
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
	calibrator.Calibrate(m_phase_map[0], m_phase_map[1], m_mask);
	*/
	//decoder->Calibrate(camera,dist);
	//decoder->Generate3Dpoints(camera);
	
	cvWaitKey();
	
	//cap->SerializeCaptures(cap->m_vCaptures,"CapturasGP_V1",true);

	return 0;
}