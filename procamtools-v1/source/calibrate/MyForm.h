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
		COptions* m_options;
		bool m_bShowWebcam;
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
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->menuStrip1->SuspendLayout();
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
			this->menuStrip1->Size = System::Drawing::Size(599, 24);
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
			this->captureToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->loadCapturesToolStripMenuItem1,
					this->saveCapturesToolStripMenuItem, this->testWebcamToolStripMenuItem
			});
			this->captureToolStripMenuItem->Name = L"captureToolStripMenuItem";
			this->captureToolStripMenuItem->Size = System::Drawing::Size(61, 20);
			this->captureToolStripMenuItem->Text = L"Capture";
			// 
			// loadCapturesToolStripMenuItem1
			// 
			this->loadCapturesToolStripMenuItem1->Name = L"loadCapturesToolStripMenuItem1";
			this->loadCapturesToolStripMenuItem1->Size = System::Drawing::Size(152, 22);
			this->loadCapturesToolStripMenuItem1->Text = L"Load Captures";
			this->loadCapturesToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::loadCapturesToolStripMenuItem1_Click);
			// 
			// saveCapturesToolStripMenuItem
			// 
			this->saveCapturesToolStripMenuItem->Name = L"saveCapturesToolStripMenuItem";
			this->saveCapturesToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->saveCapturesToolStripMenuItem->Text = L"Save Captures";
			// 
			// testWebcamToolStripMenuItem
			// 
			this->testWebcamToolStripMenuItem->Name = L"testWebcamToolStripMenuItem";
			this->testWebcamToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->testWebcamToolStripMenuItem->Text = L"Test Webcam";
			this->testWebcamToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::testWebcamToolStripMenuItem_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(599, 419);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"Structured Light Mapper";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e)
	{
				 //m_options = new COptions(1024, 768, 10, 4, true, true, true, false);
				 //string ruta = "../resources/Patterns/pattern-0";
				 //m_Cap = new CCapturador(m_options, ruta);
				 //m_bShowWebcam = false;
	}
	private: System::Void loadCapturesToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 /*
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
						 if ((MessageBox::Show(
							 m_Cap->m_vCaptures.size().ToString(),
							 "Form Closing", MessageBoxButtons::YesNo,
							 MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::No))
						 {
							 // cancel the closure of the form.
						 }
						 myStream->Close();
					 }
				 }
				 */
				 try
				 {
					 Renderer a;
					 a.render("mesh1.ply");
					 COptions* options = new COptions(1024, 768, 10, 4, true, true, true, false,true);
					 options_t opt;
					 opt.debug = true;
					 string ruta = "../resources/Patterns/pattern-0";

					 CCapturador* cap = new CCapturador(options, ruta);
					 //cap->CapturePatterns(750);
					 //cap->SerializeCaptures(cap->m_vCaptures, "CasaV3-");
					 //string ruta2 = cap->SerializeCaptures(cap->m_vCaptures, "CasaV2");
					 cap->LoadCapturesFromFiles("../resources/Captures/CasaV3--1396243862/Capture-");
					 CDecoder* decoder = new CDecoder(options, cap->m_vCaptures);
					 decoder->Decode(0);
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
	}
	private: System::Void testWebcamToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 m_bShowWebcam = true;
	}
};
}
