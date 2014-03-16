#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "Capturador.h"
#include "Decoder.h"
#include "CameraCalibrator.h"
#include "MatrixUtil.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    decoder->Calibrate(camera,dist);
//	decoder->Generate3Dpoints(camera);
    cvWaitKey();
}

MainWindow::~MainWindow()
{
    delete ui;
}
