// StructuredOpenCV_V1.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include "Capturador.h"
#include "Decoder.h"
#include "CameraCalibrator.h"
#include "MatrixUtil.h"
using namespace cv;

int main(int, char**)
{
	COptions* options = new COptions(1024, 768, 10,4,true,true,true,false);
	string ruta = "Patterns/pattern-0";
	CCameraCalibrator* cal = new CCameraCalibrator();
	Mat dist, camera;
	//cal->Calibrate("in_calib.xml", camera, dist);
	cal->GetCalibrationFromFile("out_camera_data3.xml", camera, dist);
	
	CCapturador* cap = new CCapturador(options,ruta);
	
	cap->CapturePatternsUndisorted(camera,dist,250);
	
	//cap->LoadCapturesFromFilesUndisorted("Captures/V4GP/Capture-",camera,dist);
	//cap->LoadCapturesFromFiles("Patterns/pattern-0");
	CDecoder* decoder = new CDecoder(options, cap->m_vCaptures);
	decoder->Decode();
	decoder->Calibrate(camera,dist);
	//decoder->Generate3Dpoints(camera);
	cvWaitKey();
	
	//cap->SerializeCaptures(cap->m_vCaptures,"CapturasGP_V1",true);

	return 0;
}