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
		Renderer* m_renderer;
		options_t* m_opt;
		IplImage* frame;
		CProCamCalibrate* m_calib;
		bool m_bShowWebcam;
		volatile bool m_bStartingWorker;
		bool m_bMatrixReady = false;
		bool m_bCapturating = false;
		int camBusy = 0;
		int camDevice = 0;
		slib::CMatrix<3, 3, double>* m_cam_int, *m_proj_int;
		slib::CMatrix<3, 4, double> *m_proj_ext;
		double m_cam_dist, m_proj_dist;
		Mat* m_frame;

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
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBoxProyY;

	private: System::Windows::Forms::TextBox^  textBoxProyX;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::CheckBox^  checkBoxComplementary;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ToolStripMenuItem^  calibrateCameraProjectorToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadCalibrationMatricesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveCalibrationMatrixToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  triangulateToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  exportAsOBJToolStripMenuItem;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Label^  labelProggres;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  textBoxTime;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::ComboBox^  comboBoxThress;





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
			this->calibrateCameraProjectorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadCalibrationMatricesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveCalibrationMatrixToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->triangulateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->triangulateToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exportAsOBJToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->captureToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadCapturesToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveCapturesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->testWebcamToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->startCaptureToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->pictureCamera = (gcnew System::Windows::Forms::PictureBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->pictureCapture = (gcnew System::Windows::Forms::PictureBox());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->pictureMask = (gcnew System::Windows::Forms::PictureBox());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->pictureCorrX = (gcnew System::Windows::Forms::PictureBox());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->pictureCorrY = (gcnew System::Windows::Forms::PictureBox());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->comboBoxThress = (gcnew System::Windows::Forms::ComboBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBoxTime = (gcnew System::Windows::Forms::TextBox());
			this->checkBoxComplementary = (gcnew System::Windows::Forms::CheckBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBoxProyY = (gcnew System::Windows::Forms::TextBox());
			this->textBoxProyX = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->labelProggres = (gcnew System::Windows::Forms::Label());
			this->menuStrip1->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCamera))->BeginInit();
			this->tabPage2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCapture))->BeginInit();
			this->tabPage3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureMask))->BeginInit();
			this->tabPage4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCorrX))->BeginInit();
			this->tabPage5->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCorrY))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->structuredLightToolStripMenuItem,
					this->calibrateToolStripMenuItem, this->triangulateToolStripMenuItem, this->captureToolStripMenuItem
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
			this->calibrateToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->calibrateCameraProjectorToolStripMenuItem,
					this->loadCalibrationMatricesToolStripMenuItem, this->saveCalibrationMatrixToolStripMenuItem
			});
			this->calibrateToolStripMenuItem->Name = L"calibrateToolStripMenuItem";
			this->calibrateToolStripMenuItem->Size = System::Drawing::Size(66, 20);
			this->calibrateToolStripMenuItem->Text = L"&Calibrate";
			// 
			// calibrateCameraProjectorToolStripMenuItem
			// 
			this->calibrateCameraProjectorToolStripMenuItem->Name = L"calibrateCameraProjectorToolStripMenuItem";
			this->calibrateCameraProjectorToolStripMenuItem->Size = System::Drawing::Size(218, 22);
			this->calibrateCameraProjectorToolStripMenuItem->Text = L"&Calibrate Camera/Projector";
			this->calibrateCameraProjectorToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::calibrateCameraProjectorToolStripMenuItem_Click);
			// 
			// loadCalibrationMatricesToolStripMenuItem
			// 
			this->loadCalibrationMatricesToolStripMenuItem->Name = L"loadCalibrationMatricesToolStripMenuItem";
			this->loadCalibrationMatricesToolStripMenuItem->Size = System::Drawing::Size(218, 22);
			this->loadCalibrationMatricesToolStripMenuItem->Text = L"Load Calibration Matrices";
			this->loadCalibrationMatricesToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::loadCalibrationMatricesToolStripMenuItem_Click);
			// 
			// saveCalibrationMatrixToolStripMenuItem
			// 
			this->saveCalibrationMatrixToolStripMenuItem->Name = L"saveCalibrationMatrixToolStripMenuItem";
			this->saveCalibrationMatrixToolStripMenuItem->Size = System::Drawing::Size(218, 22);
			this->saveCalibrationMatrixToolStripMenuItem->Text = L"Save Calibration Matrix";
			this->saveCalibrationMatrixToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveCalibrationMatrixToolStripMenuItem_Click);
			// 
			// triangulateToolStripMenuItem
			// 
			this->triangulateToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->triangulateToolStripMenuItem1,
					this->exportAsOBJToolStripMenuItem
			});
			this->triangulateToolStripMenuItem->Name = L"triangulateToolStripMenuItem";
			this->triangulateToolStripMenuItem->Size = System::Drawing::Size(79, 20);
			this->triangulateToolStripMenuItem->Text = L"&Triangulate";
			// 
			// triangulateToolStripMenuItem1
			// 
			this->triangulateToolStripMenuItem1->Name = L"triangulateToolStripMenuItem1";
			this->triangulateToolStripMenuItem1->Size = System::Drawing::Size(152, 22);
			this->triangulateToolStripMenuItem1->Text = L"&Triangulate";
			this->triangulateToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::triangulateToolStripMenuItem1_Click);
			// 
			// exportAsOBJToolStripMenuItem
			// 
			this->exportAsOBJToolStripMenuItem->Name = L"exportAsOBJToolStripMenuItem";
			this->exportAsOBJToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->exportAsOBJToolStripMenuItem->Text = L"&Export as OBJ";
			this->exportAsOBJToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::exportAsOBJToolStripMenuItem_Click);
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
			this->loadCapturesToolStripMenuItem1->Size = System::Drawing::Size(152, 22);
			this->loadCapturesToolStripMenuItem1->Text = L"Load Captures";
			this->loadCapturesToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::loadCapturesToolStripMenuItem1_Click);
			// 
			// saveCapturesToolStripMenuItem
			// 
			this->saveCapturesToolStripMenuItem->Name = L"saveCapturesToolStripMenuItem";
			this->saveCapturesToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->saveCapturesToolStripMenuItem->Text = L"Save Captures";
			this->saveCapturesToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveCapturesToolStripMenuItem_Click);
			// 
			// testWebcamToolStripMenuItem
			// 
			this->testWebcamToolStripMenuItem->Name = L"testWebcamToolStripMenuItem";
			this->testWebcamToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->testWebcamToolStripMenuItem->Text = L"Test Webcam";
			this->testWebcamToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::testWebcamToolStripMenuItem_Click);
			// 
			// startCaptureToolStripMenuItem
			// 
			this->startCaptureToolStripMenuItem->Name = L"startCaptureToolStripMenuItem";
			this->startCaptureToolStripMenuItem->Size = System::Drawing::Size(152, 22);
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
			// pictureMask
			// 
			this->pictureMask->Location = System::Drawing::Point(1, 1);
			this->pictureMask->Name = L"pictureMask";
			this->pictureMask->Size = System::Drawing::Size(640, 480);
			this->pictureMask->TabIndex = 3;
			this->pictureMask->TabStop = false;
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
			// pictureCorrX
			// 
			this->pictureCorrX->Location = System::Drawing::Point(1, 1);
			this->pictureCorrX->Name = L"pictureCorrX";
			this->pictureCorrX->Size = System::Drawing::Size(640, 480);
			this->pictureCorrX->TabIndex = 3;
			this->pictureCorrX->TabStop = false;
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
			// pictureCorrY
			// 
			this->pictureCorrY->Location = System::Drawing::Point(1, 1);
			this->pictureCorrY->Name = L"pictureCorrY";
			this->pictureCorrY->Size = System::Drawing::Size(640, 480);
			this->pictureCorrY->TabIndex = 3;
			this->pictureCorrY->TabStop = false;
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerReportsProgress = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MyForm::backgroundWorker1_DoWork);
			this->backgroundWorker1->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &MyForm::backgroundWorker1_ProgressChanged);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label7);
			this->groupBox1->Controls->Add(this->comboBoxThress);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->textBoxTime);
			this->groupBox1->Controls->Add(this->checkBoxComplementary);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->textBoxProyY);
			this->groupBox1->Controls->Add(this->textBoxProyX);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->groupBox1->Location = System::Drawing::Point(662, 67);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(193, 481);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(10, 95);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(59, 13);
			this->label7->TabIndex = 10;
			this->label7->Text = L"Thresshold";
			// 
			// comboBoxThress
			// 
			this->comboBoxThress->FormattingEnabled = true;
			this->comboBoxThress->Items->AddRange(gcnew cli::array< System::Object^  >(10) {
				L"10%", L"20%", L"30%", L"40%", L"50%", L"60%",
					L"70%", L"80%", L"90%", L"100%"
			});
			this->comboBoxThress->Location = System::Drawing::Point(93, 95);
			this->comboBoxThress->Name = L"comboBoxThress";
			this->comboBoxThress->Size = System::Drawing::Size(57, 21);
			this->comboBoxThress->TabIndex = 9;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(147, 68);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(12, 13);
			this->label6->TabIndex = 8;
			this->label6->Text = L"s";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(10, 66);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(82, 13);
			this->label5->TabIndex = 7;
			this->label5->Text = L"Capture Interval";
			// 
			// textBoxTime
			// 
			this->textBoxTime->Location = System::Drawing::Point(93, 63);
			this->textBoxTime->Name = L"textBoxTime";
			this->textBoxTime->Size = System::Drawing::Size(53, 20);
			this->textBoxTime->TabIndex = 6;
			this->textBoxTime->Text = L"500";
			// 
			// checkBoxComplementary
			// 
			this->checkBoxComplementary->AutoSize = true;
			this->checkBoxComplementary->Checked = true;
			this->checkBoxComplementary->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBoxComplementary->Location = System::Drawing::Point(13, 116);
			this->checkBoxComplementary->Name = L"checkBoxComplementary";
			this->checkBoxComplementary->Size = System::Drawing::Size(162, 17);
			this->checkBoxComplementary->TabIndex = 5;
			this->checkBoxComplementary->Text = L"Use Complementary Patterns";
			this->checkBoxComplementary->UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(10, 36);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(14, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"X";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(78, 36);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(14, 13);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Y";
			// 
			// textBoxProyY
			// 
			this->textBoxProyY->Location = System::Drawing::Point(93, 33);
			this->textBoxProyY->Name = L"textBoxProyY";
			this->textBoxProyY->Size = System::Drawing::Size(53, 20);
			this->textBoxProyY->TabIndex = 2;
			this->textBoxProyY->Text = L"0";
			// 
			// textBoxProyX
			// 
			this->textBoxProyX->Location = System::Drawing::Point(25, 33);
			this->textBoxProyX->Name = L"textBoxProyX";
			this->textBoxProyX->Size = System::Drawing::Size(53, 20);
			this->textBoxProyX->TabIndex = 1;
			this->textBoxProyX->Text = L"1920";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(8, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(108, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Projector Coordinates";
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(12, 550);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(162, 22);
			this->progressBar1->TabIndex = 6;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(728, 48);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(64, 16);
			this->label4->TabIndex = 3;
			this->label4->Text = L"Settings";
			// 
			// labelProggres
			// 
			this->labelProggres->AutoSize = true;
			this->labelProggres->Location = System::Drawing::Point(183, 553);
			this->labelProggres->Name = L"labelProggres";
			this->labelProggres->Size = System::Drawing::Size(0, 13);
			this->labelProggres->TabIndex = 7;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Snow;
			this->ClientSize = System::Drawing::Size(855, 587);
			this->Controls->Add(this->labelProggres);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"Structured Light Mapper";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCamera))->EndInit();
			this->tabPage2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCapture))->EndInit();
			this->tabPage3->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureMask))->EndInit();
			this->tabPage4->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCorrX))->EndInit();
			this->tabPage5->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureCorrY))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e)
	{
				 m_options = new COptions(1024, 768, 10, 4, true, true, true, false, true);
				 string ruta = "../resources/Patterns/pattern-0";
				 m_Cap = new CCapturador(m_options, ruta);
				 m_renderer = new Renderer();
				 m_bShowWebcam = false;
				 m_bStartingWorker = true;
				 m_opt = new options_t();
				 m_opt->debug = true;
				 m_decoder = new CDecoder(m_options);
				 m_calib = new CProCamCalibrate(*m_opt);
				 m_cam_int = new slib::CMatrix<3, 3, double>;
				 m_proj_int = new slib::CMatrix<3, 3, double>;
				 m_proj_ext = new slib::CMatrix<3, 4, double>;
				 m_frame = new Mat();
				 comboBoxThress->SelectedIndex = 2;
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
						 array<Char, 1>^ delimitador = ruta1->ToCharArray();
						 std::string ruta2;
						 for (int i = 0; i < delimitador->Length; i++)
							 ruta2 += delimitador[i];
						 m_Cap->LoadCapturesFromFiles(ruta2);
						 if (m_Cap->m_vCaptures.size()>0)
						 {
							 Mat b;
							 cvtColor(m_Cap->m_vCaptures[0], b, CV_GRAY2RGB);
							 DrawCvImage(&(IplImage)b, pictureCapture);
							 m_decoder->m_Info->m_bComplementary = true;
							 int thress = 25 * comboBoxThress->SelectedIndex;
							 bool captura = m_decoder->Decode(thress, m_Cap->m_vCaptures);
							 if (captura)
							 {
								 Mat b;
								 m_decoder->m_mMask[0].convertTo(b, CV_8UC1);
								 cvtColor(b, b, CV_GRAY2RGB);
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

	private: System::Void startCaptureToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 captureToolStripMenuItem->Enabled = false;
				 if (!m_bShowWebcam)
				 {
					 m_bShowWebcam = true;
					 backgroundWorker1->RunWorkerAsync(10);
				 }
				 while (m_bStartingWorker)
				 {
				 }
				 m_bCapturating = true;
				 bool captura = m_Cap->CapturePatterns(Convert::ToDouble(textBoxTime->Text), 0, Convert::ToInt32(textBoxProyX->Text, 16), Convert::ToInt32(textBoxProyY->Text, 16), true);
				 m_bCapturating = false;
				 if (captura)
				 {
					 if (m_Cap->m_vCaptures.size() > 0)
					 {
						 Mat b;
						 cvtColor(m_Cap->m_vCaptures[0], b, CV_GRAY2RGB);
						 DrawCvImage(&(IplImage)b, pictureCapture);
					 }
					// if (checkBoxComplementary->Checked)
						 m_decoder->m_Info->m_bComplementary = true;
					// else
					//	 m_decoder->m_Info->m_bComplementary = false;
						 int thress = 25 * comboBoxThress->SelectedIndex;
					 captura = m_decoder->Decode(thress, m_Cap->m_vCaptures);
					 if (captura)
					 {
						 Mat b,c;
						 m_decoder->m_mMask[0].convertTo(c, CV_8UC1);
						 cvtColor(c, b, CV_GRAY2RGB);
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
				 captureToolStripMenuItem->Enabled = true;

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
					 try
					 {
						 m_Cap->m_VideoCapture >> mFrame;
						 if (mFrame.empty()) continue;
						 m_frame = &mFrame;
						 //frame = cvCloneImage(&(IplImage)mFrame);
						 worker->ReportProgress(1);
						 //mFrame.release();
					 }
					 catch (std::exception e)
					 {
						 printf("Error: %s", e.what());
					 }
				 }
				 mFrame.release();

				 //pin_ptr<IplImage*> p;
				 //p = &frame;
				 //cvReleaseImage(p);
				 m_Cap->m_VideoCapture.release();
	}

	private: System::Void backgroundWorker1_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) {
				 if (!camBusy&&m_bShowWebcam){
					 camBusy = 1;
					 //IplImage *destination = cvCreateImage(cvSize(640, 480), m_frame->depth(), m_frame->channels());
					 //cvResize(frame, destination);
					 if (m_frame->cols>0 && m_frame->rows>0)
						DrawCvImage(&(IplImage)(*m_frame), pictureCamera);
					 //if (m_frame->cols>0&&m_frame->rows>0)
					 //imshow("camara", *m_frame);
					 m_frame->release();
					 //cvReleaseImage(&destination);
					 /*pin_ptr<IplImage*> p;
					 p = &frame;
					 cvReleaseImage(p);*/
					 camBusy = 0;
				 }
				 if (m_bCapturating)
				 {
					 progressBar1->Value = 100 * (float)((float)m_Cap->m_vCaptures.size() / (float)m_Cap->m_Options->m_nNumPatterns);
					 labelProggres->Text = m_Cap->m_vCaptures.size().ToString() + "/" + m_Cap->m_Options->m_nNumPatterns;
				 }
				 else
				 {
					 progressBar1->Value = 0;
					 labelProggres->Text = " ";
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

	private: System::Void calibrateCameraProjectorToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 if (m_decoder->m_mGray[0].empty())
				 {
					 MessageBox::Show("No hay información para calibrar. Carge capturas para continuar.",
						 "Error", MessageBoxButtons::OK,
						 MessageBoxIcon::Error);
					 return;
				 }

				 Mat b = m_decoder->m_mMask[1].clone();
				 slib::Field<2, float> m_mask;
				 m_mask.Initialize(b.cols, m_decoder->m_mMask[1].rows);
				 for (int i = 0; i < b.cols; i++)
				 for (int j = 0; j < b.rows; j++)
					 m_mask.cell(i, j) = b.at<ushort>(j, i);


				 slib::Field<2, float> m_phase_map[2];
				 for (int k = 0; k < 2; k++)
				 {
					 Mat a = m_decoder->m_mGray[k].clone();
					 m_phase_map[k].Initialize(a.cols, a.rows);
					 for (int i = 0; i < a.cols; i++)
					 for (int j = 0; j < a.rows; j++)
						 m_phase_map[k].cell(i, j) = a.at<ushort>(j, i);
				 }
				 m_calib->Calibrate(m_phase_map[0], m_phase_map[1], m_mask);
				 *m_cam_int = m_calib->m_cam_int;
				 m_cam_dist = m_calib->m_cam_dist;
				 *m_proj_int = m_calib->m_pro_int;
				 m_proj_dist = m_calib->m_pro_dist;
				 *m_proj_ext = m_calib->m_pro_ext;
				 m_bMatrixReady = true;
	}

	private: System::Void saveCalibrationMatrixToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog;
				 saveFileDialog->InitialDirectory = "$(ProjectDir)";
				 saveFileDialog->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
				 //saveFileDialog->FilterIndex = 1;
				 saveFileDialog->RestoreDirectory = true;
				 if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 System::String^ filename = saveFileDialog->FileName;
					 System::IO::StreamWriter^ file = gcnew System::IO::StreamWriter(filename);
					 file->WriteLine("MatJPV");
					 //Write camera-int
					 int sizeX = m_cam_int->GetNumCols();
					 int sizeY = m_cam_int->GetNumRows();
					 for (int i = 0; i < sizeX; i++)
					 for (int j = 0; j < sizeY; j++)
					 {
						 file->Write((*m_cam_int)(i, j).ToString());
						 file->Write(" ");
					 }
					 file->Write("\n");
					 //Write camera-dist
					 file->Write(m_cam_dist.ToString());
					 file->Write("\n");

					 //Write proj-int
					 sizeX = m_proj_int->GetNumCols();
					 sizeY = m_proj_int->GetNumRows();
					 for (int i = 0; i < sizeX; i++)
					 for (int j = 0; j < sizeY; j++)
					 {
						 file->Write((*m_proj_int)(i, j).ToString());
						 file->Write(" ");
					 }
					 file->Write("\n");
					 //Write proj-dist
					 file->Write(m_proj_dist.ToString());
					 file->Write("\n");

					 //Write proj-ext
					 sizeX = m_proj_ext->GetNumCols();
					 sizeY = m_proj_ext->GetNumRows();
					 for (int r = 0; r < sizeY; r++)
					 for (int c = 0; c < sizeX; c++)
					 {
						 file->Write((*m_proj_ext)(r, c).ToString());
						 file->Write(" ");
					 }
					 file->Close();
				 }
	}

	private: System::Void loadCalibrationMatricesToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

				 openFileDialog1->InitialDirectory = "$(ProjectDir)";
				 openFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
				 openFileDialog1->FilterIndex = 1;
				 openFileDialog1->RestoreDirectory = true;
				 if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 System::IO::StreamReader^ streamReader = gcnew System::IO::StreamReader(openFileDialog1->FileName);
					 System::String^ output = streamReader->ReadLine();
					 if (output != "MatJPV")
					 {
						 MessageBox::Show("El archivo seleccionado no es un archivo de matrices valido.",
							 "Error", MessageBoxButtons::OK,
							 MessageBoxIcon::Error);
						 streamReader->Close();
						 return;
					 }
					 //Camera intrinsics
					 output = streamReader->ReadLine();
					 array<System::String^>^ camera_int = output->Split(' ');

					 slib::CMatrix<3, 3, double> cam_int = slib::make_matrix<double>(
						 Convert::ToDouble(camera_int[0]), Convert::ToDouble(camera_int[1]), Convert::ToDouble(camera_int[2]),
						 Convert::ToDouble(camera_int[3]), Convert::ToDouble(camera_int[4]), Convert::ToDouble(camera_int[5]),
						 Convert::ToDouble(camera_int[6]), Convert::ToDouble(camera_int[7]), 1);

					 //Camera dist
					 output = streamReader->ReadLine();
					 m_cam_dist = Convert::ToDouble(output);

					 //Projector intrinsics
					 output = streamReader->ReadLine();
					 camera_int = output->Split(' ');

					 slib::CMatrix<3, 3, double> proj_int = slib::make_matrix<double>(
						 Convert::ToDouble(camera_int[0]), Convert::ToDouble(camera_int[1]), Convert::ToDouble(camera_int[2]),
						 Convert::ToDouble(camera_int[3]), Convert::ToDouble(camera_int[4]), Convert::ToDouble(camera_int[5]),
						 Convert::ToDouble(camera_int[6]), Convert::ToDouble(camera_int[7]), 1);

					 //Projector dist
					 output = streamReader->ReadLine();
					 m_proj_dist = Convert::ToDouble(output);

					 //Proj extrinsics
					 output = streamReader->ReadLine();
					 array<System::String^>^ camera_ext = output->Split(' ');

					 slib::CMatrix<3, 4, double> proj_ext;
					 int sizeX = 3;
					 int sizeY = 4;
					 for (int c = 0; c < sizeY; c++)
					 for (int r = 0; r < sizeX; r++)
						 proj_ext(r, c) = Convert::ToDouble(camera_ext[r*sizeY+c]);


					 *m_cam_int = cam_int;
					 *m_proj_int = proj_int;
					 *m_proj_ext = proj_ext;
					 streamReader->Close();
					 m_bMatrixReady = true;
				 }
	}

	private: System::Void triangulateToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 if (!m_bMatrixReady || m_decoder->m_mGray[0].empty())
				 {
					 MessageBox::Show("No hay matrices o mapas de correspondencia para hacer la triangulacion.",
						 "Error", MessageBoxButtons::OK,
						 MessageBoxIcon::Error);
					 return;
				 }
				 Mat b = m_decoder->m_mMask[1].clone();
				 slib::Field<2, float> m_mask;
				 m_mask.Initialize(b.cols, m_decoder->m_mMask[1].rows);
				 for (int i = 0; i < b.cols; i++)
				 for (int j = 0; j < b.rows; j++)
					 m_mask.cell(i, j) = b.at<ushort>(j, i);


				 slib::Field<2, float> m_phase_map[2];
				 for (int k = 0; k < 2; k++)
				 {
					 Mat a = m_decoder->m_mGray[k].clone();
					 m_phase_map[k].Initialize(a.cols, a.rows);
					 for (int i = 0; i < a.cols; i++)
					 for (int j = 0; j < a.rows; j++)
						 m_phase_map[k].cell(i, j) = a.at<ushort>(j, i);
				 }

				 m_renderer->makeTriangulation(*m_opt, m_phase_map[0], m_phase_map[1], m_mask, *m_proj_int, *m_cam_int, *m_proj_ext, m_proj_dist, m_cam_dist);
				// m_renderer->render("meshTemp.ply");
	}

	private: System::Void MyForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				 if (m_Cap->m_VideoCapture.isOpened())
					 m_Cap->m_VideoCapture.release();
	}
	private: System::Void exportAsOBJToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 if (m_renderer->vertices.size()!=0)
				 {
					 SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog;
					 saveFileDialog->InitialDirectory = "$(ProjectDir)";
					 saveFileDialog->Filter = "Waveform 3D Object(*.OBJ)|*.OBJ";
					 //saveFileDialog->FilterIndex = 1;
					 saveFileDialog->RestoreDirectory = true;
					 if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					 {
						 System::String^ ruta = saveFileDialog->FileName;
						 array<System::String^>^ rutas = ruta->Split('\\');
						 array<Char, 1>^ delimitador = ruta->ToCharArray();
						 std::string ruta2;
						 for (int i = 0; i < delimitador->Length; i++)
							 ruta2 += delimitador[i];

						 array<Char, 1>^ delimitadorPath = rutas[rutas->Length - 1]->ToCharArray();
						 std::string path;
						 for (int i = 0; i < delimitadorPath->Length; i++)
							 path += delimitador[i];
						 
						 m_renderer->WriteObj(ruta2,path);
					 }
				 }
				 else
					 MessageBox::Show("No hay capturas que guardar, genera una serie de capturas.",
					 "Error", MessageBoxButtons::OK,
					 MessageBoxIcon::Error);
	}
};
}
