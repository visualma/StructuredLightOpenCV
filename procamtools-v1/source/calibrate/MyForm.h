#pragma once
#include "Field.h"
#include "ImageBmpIO.h"

#include "MathBaseLapack.h"
#include "MiscUtil.h"
#include "Stereo.h"
#include "LeastSquare.h"

#include "../FundamentalMatrix.h"
#include "../Options.h"

#include "calibrate.h"
#include "Decoder.h"
#include "Capturador.h"
#include "Form1.h"
#include "Renderer.h"
#include "opencv2\opencv.hpp"

namespace calibrate {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace cv;
	/// <summary>
	/// Resumen de MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		CCapturador* m_Cap;
		CDecoder* m_decoder;
		COptions* m_options;
		options_t* m_opt;
		IplImage* frame;
		bool m_bShowWebcam;
		bool m_bStartingWorker;
		int camBusy = 0;
		int camDevice = 0;
	private: System::Windows::Forms::ToolStripMenuItem^  startCaptureToolStripMenuItem;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;

	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::PictureBox^  pictureCamera;
	private: System::Windows::Forms::PictureBox^  pictureCapture;
	private: System::Windows::Forms::PictureBox^  pictureMask;
	private: System::Windows::Forms::TabPage^  tabPage4;
	private: System::Windows::Forms::PictureBox^  pictureCorrX;
	private: System::Windows::Forms::TabPage^  tabPage5;
	private: System::Windows::Forms::PictureBox^  pictureCorrY;




	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: agregar código de constructor aquí
			//
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén utilizando.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^  structuredLightToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadCapturesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  calibrateToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  triangulateToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  dVisualizationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  captureToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadCapturesToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  saveCapturesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  testWebcamToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;

	protected:

	private:
		/// <summary>
		/// Variable del diseñador requerida.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido del método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->structuredLightToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadCapturesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->calibrateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->triangulateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->dVisualizationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->captureToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadCapturesToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveCapturesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->testWebcamToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->startCaptureToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->pictureCamera = (gcnew System::Windows::Forms::PictureBox());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->pictureCapture = (gcnew System::Windows::Forms::PictureBox());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->pictureMask = (gcnew System::Windows::Forms::PictureBox());
			this->pictureCorrX = (gcnew System::Windows::Forms::PictureBox());
			this->pictureCorrY = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCamera))->BeginInit();
			this->tabPage3->SuspendLayout();
			this->tabPage2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCapture))->BeginInit();
			this->tabPage4->SuspendLayout();
			this->tabPage5->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureMask))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCorrX))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCorrY))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->structuredLightToolStripMenuItem,
					this->calibrateToolStripMenuItem, this->triangulateToolStripMenuItem, this->dVisualizationToolStripMenuItem, this->captureToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(855, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// structuredLightToolStripMenuItem
			// 
			this->structuredLightToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->loadCapturesToolStripMenuItem });
			this->structuredLightToolStripMenuItem->Name = L"structuredLightToolStripMenuItem";
			this->structuredLightToolStripMenuItem->Size = System::Drawing::Size(104, 20);
			this->structuredLightToolStripMenuItem->Text = L"&Structured Light";
			// 
			// loadCapturesToolStripMenuItem
			// 
			this->loadCapturesToolStripMenuItem->Name = L"loadCapturesToolStripMenuItem";
			this->loadCapturesToolStripMenuItem->Size = System::Drawing::Size(150, 22);
			this->loadCapturesToolStripMenuItem->Text = L"Load Captures";
			// 
			// calibrateToolStripMenuItem
			// 
			this->calibrateToolStripMenuItem->Name = L"calibrateToolStripMenuItem";
			this->calibrateToolStripMenuItem->Size = System::Drawing::Size(66, 20);
			this->calibrateToolStripMenuItem->Text = L"&Calibrate";
			// 
			// triangulateToolStripMenuItem
			// 
			this->triangulateToolStripMenuItem->Name = L"triangulateToolStripMenuItem";
			this->triangulateToolStripMenuItem->Size = System::Drawing::Size(79, 20);
			this->triangulateToolStripMenuItem->Text = L"&Triangulate";
			// 
			// dVisualizationToolStripMenuItem
			// 
			this->dVisualizationToolStripMenuItem->Name = L"dVisualizationToolStripMenuItem";
			this->dVisualizationToolStripMenuItem->Size = System::Drawing::Size(102, 20);
			this->dVisualizationToolStripMenuItem->Text = L"&3D Visualization";
			// 
			// captureToolStripMenuItem
			// 
			this->captureToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->loadCapturesToolStripMenuItem1,
					this->saveCapturesToolStripMenuItem, this->testWebcamToolStripMenuItem, this->startCaptureToolStripMenuItem
			});
			this->captureToolStripMenuItem->Name = L"captureToolStripMenuItem";
			this->captureToolStripMenuItem->Size = System::Drawing::Size(61, 20);
			this->captureToolStripMenuItem->Text = L"Capture";
			// 
			// loadCapturesToolStripMenuItem1
			// 
			this->loadCapturesToolStripMenuItem1->Name = L"loadCapturesToolStripMenuItem1";
			this->loadCapturesToolStripMenuItem1->Size = System::Drawing::Size(150, 22);
			this->loadCapturesToolStripMenuItem1->Text = L"Load Captures";
			this->loadCapturesToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::loadCapturesToolStripMenuItem1_Click);
			// 
			// saveCapturesToolStripMenuItem
			// 
			this->saveCapturesToolStripMenuItem->Name = L"saveCapturesToolStripMenuItem";
			this->saveCapturesToolStripMenuItem->Size = System::Drawing::Size(150, 22);
			this->saveCapturesToolStripMenuItem->Text = L"Save Captures";
			this->saveCapturesToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveCapturesToolStripMenuItem_Click);
			// 
			// testWebcamToolStripMenuItem
			// 
			this->testWebcamToolStripMenuItem->Name = L"testWebcamToolStripMenuItem";
			this->testWebcamToolStripMenuItem->Size = System::Drawing::Size(150, 22);
			this->testWebcamToolStripMenuItem->Text = L"Test Webcam";
			this->testWebcamToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::testWebcamToolStripMenuItem_Click);
			// 
			// startCaptureToolStripMenuItem
			// 
			this->startCaptureToolStripMenuItem->Name = L"startCaptureToolStripMenuItem";
			this->startCaptureToolStripMenuItem->Size = System::Drawing::Size(150, 22);
			this->startCaptureToolStripMenuItem->Text = L"Start Capture";
			this->startCaptureToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::startCaptureToolStripMenuItem_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Controls->Add(this->tabPage4);
			this->tabControl1->Controls->Add(this->tabPage5);
			this->tabControl1->Location = System::Drawing::Point(12, 42);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(648, 506);
			this->tabControl1->TabIndex = 1;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->pictureCamera);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(640, 480);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Camera";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// pictureCamera
			// 
			this->pictureCamera->Location = System::Drawing::Point(1, 2);
			this->pictureCamera->Name = L"pictureCamera";
			this->pictureCamera->Size = System::Drawing::Size(640, 480);
			this->pictureCamera->TabIndex = 1;
			this->pictureCamera->TabStop = false;
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->pictureMask);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Size = System::Drawing::Size(640, 480);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Mask";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->pictureCapture);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(640, 480);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Captures";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// pictureCapture
			// 
			this->pictureCapture->Location = System::Drawing::Point(1, 2);
			this->pictureCapture->Name = L"pictureCapture";
			this->pictureCapture->Size = System::Drawing::Size(640, 480);
			this->pictureCapture->TabIndex = 2;
			this->pictureCapture->TabStop = false;
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerReportsProgress = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MyForm::backgroundWorker1_DoWork);
			this->backgroundWorker1->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &MyForm::backgroundWorker1_ProgressChanged);
			// 
			// tabPage4
			// 
			this->tabPage4->Controls->Add(this->pictureCorrX);
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Size = System::Drawing::Size(640, 480);
			this->tabPage4->TabIndex = 3;
			this->tabPage4->Text = L"Correspondence X";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// tabPage5
			// 
			this->tabPage5->Controls->Add(this->pictureCorrY);
			this->tabPage5->Location = System::Drawing::Point(4, 22);
			this->tabPage5->Name = L"tabPage5";
			this->tabPage5->Size = System::Drawing::Size(640, 480);
			this->tabPage5->TabIndex = 4;
			this->tabPage5->Text = L"Correspondence Y";
			this->tabPage5->UseVisualStyleBackColor = true;
			// 
			// pictureMask
			// 
			this->pictureMask->Location = System::Drawing::Point(1, 1);
			this->pictureMask->Name = L"pictureMask";
			this->pictureMask->Size = System::Drawing::Size(640, 480);
			this->pictureMask->TabIndex = 3;
			this->pictureMask->TabStop = false;
			// 
			// pictureCorrX
			// 
			this->pictureCorrX->Location = System::Drawing::Point(1, 1);
			this->pictureCorrX->Name = L"pictureCorrX";
			this->pictureCorrX->Size = System::Drawing::Size(640, 480);
			this->pictureCorrX->TabIndex = 3;
			this->pictureCorrX->TabStop = false;
			// 
			// pictureCorrY
			// 
			this->pictureCorrY->Location = System::Drawing::Point(1, 1);
			this->pictureCorrY->Name = L"pictureCorrY";
			this->pictureCorrY->Size = System::Drawing::Size(640, 480);
			this->pictureCorrY->TabIndex = 3;
			this->pictureCorrY->TabStop = false;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(855, 587);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"Structured Light Mapper";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCamera))->EndInit();
			this->tabPage3->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCapture))->EndInit();
			this->tabPage4->ResumeLayout(false);
			this->tabPage5->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureMask))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCorrX))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCorrY))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e)
	{
				 m_options = new COptions(1024, 768, 10, 4, true, true, true, false,true);
				 string ruta = "../resources/Patterns/pattern-0";
				 m_Cap = new CCapturador(m_options, ruta);
				 m_bShowWebcam = false;
				 m_bStartingWorker = true;
				 m_opt = new options_t();
				 m_opt->debug = true;
				 m_decoder = new CDecoder(m_options);
	}
	private: System::Void loadCapturesToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 
				 IO::Stream^ myStream;
				 OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

				 openFileDialog1->InitialDirectory = "$(ProjectDir)";
				 openFileDialog1->Filter = "Image Files(*.BMP;*.JPG;*.GIF)|*.BMP;*.JPG;*.GIF|All files (*.*)|*.*";
				 openFileDialog1->FilterIndex = 1;
				 openFileDialog1->RestoreDirectory = true;

				 if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 if ((myStream = openFileDialog1->OpenFile()) != nullptr)
					 {
						 System::String^ ruta = openFileDialog1->FileName;
						 System::String^ ruta1 = ruta->Remove(ruta->Length - 6);
						 array<Char,1>^ delimitador = ruta1->ToCharArray();
						 std::string ruta2;
						 for (int i = 0; i < delimitador->Length; i++)
							 ruta2 += delimitador[i];
						 m_Cap->LoadCapturesFromFiles(ruta2);
						 if (m_Cap->m_vCaptures.size()>0)
						 {
							 Mat b;
							 cvtColor(m_Cap->m_vCaptures[0], b, CV_GRAY2RGB);
							 DrawCvImage(&(IplImage)b, pictureCapture);

							 bool captura = m_decoder->Decode(25, m_Cap->m_vCaptures);
							 if (captura)
							 {
								 Mat b;
								 cvtColor(m_decoder->m_mMask[0], b, CV_GRAY2RGB);
								 DrawCvImage(&(IplImage)b, pictureMask);

								 Mat temp1 = Mat(m_decoder->m_mGray[0].rows, m_decoder->m_mGray[0].cols, CV_8UC1);
								 m_decoder->m_mGray[0].convertTo(temp1, CV_8UC1, 255 / 1024.0, 0);
								 cvtColor(temp1, b, CV_GRAY2RGB);
								 DrawCvImage(&(IplImage)b, pictureCorrX);
								 m_decoder->m_mGray[1].convertTo(temp1, CV_8UC1, 255 / 1024.0, 0);
								 cvtColor(temp1, b, CV_GRAY2RGB);
								 DrawCvImage(&(IplImage)b, pictureCorrY);
							 }

						 }
						 
						 myStream->Close();
					 }
				 }
				 
				 /*
				 try
				 {
					 Renderer a;
					 //a.render("mesh1.ply");
					 COptions* options = new COptions(1024, 768, 10, 4, true, true, true, false,true);
					 options_t opt;
					 opt.debug = true;
					 string ruta = "../resources/Patterns/pattern-0";

					 CCapturador* cap = new CCapturador(options, ruta);
					 cap->CapturePatterns(750,0);
					 //cap->SerializeCaptures(cap->m_vCaptures, "CasaV3-");
					 //string ruta2 = cap->SerializeCaptures(cap->m_vCaptures, "CasaV2");
					 cap->LoadCapturesFromFiles("../resources/Captures/CasaV3--1396243862/Capture-");
					 CDecoder* decoder = new CDecoder(options, cap->m_vCaptures);
					 decoder->Decode(75);
					 CProCamCalibrate calib(opt);

					 cvWaitKey();
					 cvDestroyAllWindows();
					 slib::Field<2, float> m_mask;
					 
					 //Mat mask = imread("mascara.bmp", CV_LOAD_IMAGE_GRAYSCALE);
					 //mask.convertTo(mask, CV_8U);
					 //printf("chanesl %d %d", mask.channels(), decoder->m_mGray[0].channels());

					 Mat b = decoder->m_mMask[1].clone();// Mat(640, 480, CV_16UC1);
					 //cv::resize(decoder->m_mMask[1], b, cv::Size(640, 480), 0, 0, cv::INTER_CUBIC);
					 m_mask.Initialize(b.cols, decoder->m_mMask[1].rows);
					 for (int i = 0; i < b.cols; i++)
					 for (int j = 0; j < b.rows; j++)
						 m_mask.cell(i, j) = b.at<ushort>(j, i);

					 
					 slib::Field<2, float> m_phase_map[2];
					 for (int k = 0; k < 2; k++)
					 {

//						 Mat a = Mat(640, 480, CV_16UC1);
//						 cv::resize(decoder->m_mGray[k], a, cv::Size(640, 480), 0, 0, cv::INTER_CUBIC);
						 //m_phase_map[k].Initialize(decoder->m_mGray[k].cols, decoder->m_mGray[k].rows);
						 Mat a = decoder->m_mGray[k].clone();
						 m_phase_map[k].Initialize(a.cols, a.rows);
						 for (int i = 0; i < a.cols; i++)
						 for (int j = 0; j < a.rows; j++)
						 //m_phase_map[k].cell(i, j) = decoder->m_mGray[k].at<ushort>(j, i);
						 m_phase_map[k].cell(i, j) = a.at<ushort>(j, i);
					 }
					 //Mat temp1 = Mat(decoder->m_mGray[1].rows, decoder->m_mGray[1].cols, CV_8UC1);
					 //decoder->m_mGray[1].convertTo(temp1, CV_8UC1, 255 / 1024.0, 0);
					 //imshow("mascara1", temp1);
					 //imshow("mascara2", mask);
					 cvWaitKey();
					 cvDestroyAllWindows();
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
					 oss8 << "mesh1.ply";

					 //Renderer a;

					 a.makeTriangulation(opt, m_phase_map[0], m_phase_map[1], m_mask, calib.m_pro_int, calib.m_cam_int, calib.m_pro_ext, calib.m_pro_dist, calib.m_cam_dist,oss8.str());
					 a.render("mesh1.ply");
					 cvWaitKey();
					 }
					 catch (const std::exception& e)
					 {
					 fprintf(stderr,"error: %s\n", e.what());
					 
					 }
					 */
	}
	private: System::Void testWebcamToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 if (!m_bShowWebcam)
				 {

					 m_bShowWebcam = true;
					 if (!backgroundWorker1->IsBusy)
						backgroundWorker1->RunWorkerAsync(10);
				 }
				 else
				 {
					 m_bShowWebcam = false;
				 }
		//m_Cap->tryCamera(0);
	}
	private: System::Void saveCapturesToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (m_Cap->m_vCaptures.size() != 0)
				 {
					 SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog;
					 saveFileDialog->InitialDirectory = "$(ProjectDir)";
					 //saveFileDialog->Filter = "Image Files(*.BMP;*.JPG;*.GIF)|*.BMP;*.JPG;*.GIF|All files (*.*)|*.*";
					 //saveFileDialog->FilterIndex = 1;
					 saveFileDialog->RestoreDirectory = true;
					 if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
						 {
							 System::String^ ruta = saveFileDialog->FileName;
							 array<Char, 1>^ delimitador = ruta->ToCharArray();
							 std::string ruta2;
							 for (int i = 0; i < delimitador->Length; i++)
								 ruta2 += delimitador[i];
							 m_Cap->SerializeCaptures(m_Cap->m_vCaptures, ruta2);
						 }
				 }
				 else
					 MessageBox::Show("No hay capturas que guardar, genera una serie de capturas.",
					 "Error", MessageBoxButtons::OK,
					 MessageBoxIcon::Error);
	}
	private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
	{
				 m_bStartingWorker = true;
				 BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
				 if (!m_Cap->m_VideoCapture.isOpened())  // check if we succeeded
					 m_Cap->m_VideoCapture = VideoCapture(camDevice);
				 m_bStartingWorker = false;
				 Mat mFrame;
				 while (1){
					 if (!m_Cap->m_VideoCapture.isOpened())
					 {
						 m_bShowWebcam = false;
						 break;
					 }
					 if (camBusy) continue;
					 m_Cap->m_VideoCapture >> mFrame;
					 if (mFrame.empty()) continue;
					 frame = cvCloneImage(&(IplImage)mFrame);
					 worker->ReportProgress(1);
				 }
				 mFrame.release();
				 //cvReleaseImage(&frame);
				 m_Cap->m_VideoCapture.release();
	}
	private: System::Void backgroundWorker1_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) {
				 if (!camBusy&&m_bShowWebcam){
					 camBusy = 1;
					 IplImage *destination = cvCreateImage(cvSize(640, 480), frame->depth, frame->nChannels);
					 cvResize(frame, destination);
					 DrawCvImage(destination, pictureCamera);
					 cvReleaseImage(&destination);
					 camBusy = 0;
				 }
	}
	private: System::Void DrawCvImage(IplImage *CvImage, System::Windows::Forms::PictureBox^ pbx) {
				// typecast IplImage to Bitmap
				if ((pbx->Image == nullptr) || (pbx->Width != CvImage->width) || (pbx->Height != CvImage->height)){
					pbx->Width = CvImage->width;
					pbx->Height = CvImage->height;
					Bitmap^ bmpPicBox = gcnew Bitmap(pbx->Width, pbx->Height);
					pbx->Image = bmpPicBox;
				}

				Graphics^g = Graphics::FromImage(pbx->Image);

				Bitmap^ bmp = gcnew Bitmap(CvImage->width, CvImage->height, CvImage->widthStep,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb, IntPtr(CvImage->imageData));

				g->DrawImage(bmp, 0, 0, CvImage->width, CvImage->height);
				pbx->Refresh();

				delete g;
	}
	private: System::Void startCaptureToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 if (!m_bShowWebcam)
				 {
					 m_bShowWebcam = true;
					 backgroundWorker1->RunWorkerAsync(10);
				 }
				 while (m_bStartingWorker)
				 {
				 }
				 printf("hola");
				 bool captura = m_Cap->CapturePatterns(500, 0);
				 if (captura)
				 {
					 if (m_Cap->m_vCaptures.size()>0)
					 {
						 Mat b;
						 cvtColor(m_Cap->m_vCaptures[0], b, CV_GRAY2RGB);
						 DrawCvImage(&(IplImage)b, pictureCapture);
					 }
					 captura = m_decoder->Decode(25, m_Cap->m_vCaptures);
					 if (captura)
					 {
						 Mat b;
						 cvtColor(m_decoder->m_mMask[0], b, CV_GRAY2RGB);
						 DrawCvImage(&(IplImage)b, pictureMask);
						 
						 Mat temp1 = Mat(m_decoder->m_mGray[0].rows, m_decoder->m_mGray[0].cols, CV_8UC1);
						 m_decoder->m_mGray[0].convertTo(temp1, CV_8UC1, 255 / 1024.0, 0);
						 cvtColor(temp1, b, CV_GRAY2RGB);
						 DrawCvImage(&(IplImage)b, pictureCorrX);
						 m_decoder->m_mGray[1].convertTo(temp1, CV_8UC1, 255 / 1024.0, 0);
						 cvtColor(temp1, b, CV_GRAY2RGB);
						 DrawCvImage(&(IplImage)b, pictureCorrY);
					 }
				 }
				 
	}
};
}
