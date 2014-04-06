#pragma once
#pragma comment(lib,"assimp.lib")
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
//#include <GL\freeglut.h>
#include "MathBaseLapack.h"
#include "MiscUtil.h"
#include "Stereo.h"
#include "LeastSquare.h"
#include "../FundamentalMatrix.h"
#include "../Options.h"
#include "Field.h"
#include "ImageBmpIO.h"
#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

using namespace slib;

static int g_mX;
static int g_mY;
static float g_s;
class Renderer
{
public:
	Renderer()
	{
		angle = 0.0f;
		g_s = 1.0f;
	}
	~Renderer()
	{}
private:
	void setupDrawCallback();
	void setupMouseFunction();
	void setupKeyboarFunction();
public:
	const aiScene* scene = NULL;
	GLuint scene_list = 0;
	aiVector3D scene_min, scene_max, scene_center;
	// current rotation angle
	float angle;
	static void reshape(int width, int height)
	{
		const double aspectRatio = (float)width / height, fieldOfView = 45.0;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fieldOfView, aspectRatio,
			1.0, 1000.0);  // Znear and Zfar 
		glViewport(0, 0, width, height);
	}

	void myMouseMove(int x, int y)
	{
		g_mX = x;
		g_mY = 768 - y;
		printf("%d", g_mX);
		glutPostRedisplay();
	}
//---------------------------------------------------------------------------
	void get_bounding_box_for_node(const aiNode* nd,
	aiVector3D* min,
	aiVector3D* max,
	aiMatrix4x4* trafo
		){
		aiMatrix4x4 prev;
		unsigned int n = 0, t;

		prev = *trafo;
		aiMultiplyMatrix4(trafo, &nd->mTransformation);

		for (; n < nd->mNumMeshes; ++n) {
			const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
			for (t = 0; t < mesh->mNumVertices; ++t) {

				aiVector3D tmp = mesh->mVertices[t];
				aiTransformVecByMatrix4(&tmp, trafo);

				min->x = aisgl_min(min->x, tmp.x);
				min->y = aisgl_min(min->y, tmp.y);
				min->z = aisgl_min(min->z, tmp.z);

				max->x = aisgl_max(max->x, tmp.x);
				max->y = aisgl_max(max->y, tmp.y);
				max->z = aisgl_max(max->z, tmp.z);
			}
		}

		for (n = 0; n < nd->mNumChildren; ++n) {
			get_bounding_box_for_node(nd->mChildren[n], min, max, trafo);
		}
		*trafo = prev;
	}

	// ----------------------------------------------------------------------------
	void get_bounding_box(aiVector3D* min, aiVector3D* max)
	{
		aiMatrix4x4 trafo;
		aiIdentityMatrix4(&trafo);

		min->x = min->y = min->z = 1e10f;
		max->x = max->y = max->z = -1e10f;
		get_bounding_box_for_node(scene->mRootNode, min, max, &trafo);
	}

	// ----------------------------------------------------------------------------
	void color4_to_float4(const aiColor4D *c, float f[4])
	{
		f[0] = c->r;
		f[1] = c->g;
		f[2] = c->b;
		f[3] = c->a;
	}

	// ----------------------------------------------------------------------------
	void set_float4(float f[4], float a, float b, float c, float d)
	{
		f[0] = a;
		f[1] = b;
		f[2] = c;
		f[3] = d;
	}

	// ----------------------------------------------------------------------------
	void apply_material(const aiMaterial *mtl)
	{
		float c[4];

		GLenum fill_mode;
		int ret1, ret2;
		aiColor4D diffuse;
		aiColor4D specular;
		aiColor4D ambient;
		aiColor4D emission;
		float shininess, strength;
		int two_sided;
		int wireframe;
		unsigned int max;

		set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
			color4_to_float4(&diffuse, c);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
			color4_to_float4(&specular, c);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

		set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
			color4_to_float4(&ambient, c);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
			color4_to_float4(&emission, c);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

		max = 1;
		ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
		if (ret1 == AI_SUCCESS) {
			max = 1;
			ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
			if (ret2 == AI_SUCCESS)
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
			else
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		}
		else {
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
			set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
		}

		max = 1;
		if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
			fill_mode = wireframe ? GL_LINE : GL_FILL;
		else
			fill_mode = GL_FILL;
		glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

		max = 1;
		if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);
	}

	// ----------------------------------------------------------------------------
	void recursive_render(const aiScene *sc, const aiNode* nd)
	{
		unsigned int i;
		unsigned int n = 0, t;
		aiMatrix4x4 m = nd->mTransformation;

		// update transform
		aiTransposeMatrix4(&m);
		glPushMatrix();
		glMultMatrixf((float*)&m);

		// draw all meshes assigned to this node
		for (; n < nd->mNumMeshes; ++n) {
			const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

			apply_material(sc->mMaterials[mesh->mMaterialIndex]);

			if (mesh->mNormals == NULL) {
				glDisable(GL_LIGHTING);
			}
			else {
				glEnable(GL_LIGHTING);
			}

			for (t = 0; t < mesh->mNumFaces; ++t) {
				const aiFace* face = &mesh->mFaces[t];
				GLenum face_mode;

				switch (face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
				}

				glBegin(face_mode);

				for (i = 0; i < face->mNumIndices; i++) {
					int index = face->mIndices[i];
					if (mesh->mColors[0] != NULL)
						glColor4fv((GLfloat*)&mesh->mColors[0][index]);
					if (mesh->mNormals != NULL)
						glNormal3fv(&mesh->mNormals[index].x);
					glVertex3fv(&mesh->mVertices[index].x);
				}

				glEnd();
			}

		}

		// draw all children
		for (n = 0; n < nd->mNumChildren; ++n) {
			recursive_render(sc, nd->mChildren[n]);
		}

		glPopMatrix();
	}

	// ----------------------------------------------------------------------------
	void do_motion(void)
	{
		static GLint prev_time = 0;
		static GLint prev_fps_time = 0;
		static int frames = 0;

		int time = glutGet(GLUT_ELAPSED_TIME);
		angle += (time - prev_time)*0.01;
		prev_time = time;

		frames += 1;
		if ((time - prev_fps_time) > 1000) // update every seconds
		{
			int current_fps = frames * 1000 / (time - prev_fps_time);
			printf("%d fps\n", current_fps);
			frames = 0;
			prev_fps_time = time;
		}


		glutPostRedisplay();
	}

	// ----------------------------------------------------------------------------
	void display(void)
	{
		float tmp;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.f, 0.f, 3.f, 0.f, 0.f, -5.f, 0.f, 1.f, 0.f);



		// rotate it around the y axis
		float width = glutGet(GLUT_WINDOW_WIDTH);
		float height = glutGet(GLUT_WINDOW_HEIGHT);
		float rX = (g_mX / width)*90-45;
		float rY = (g_mY / height)*90-45+180;
		glRotatef(rY, 1.0, 0.f, 0.f);
		glRotatef(-rX, 0.0f, 1.0f, 0.0f);

		// scale the whole asset to fit into our view frustum 
		tmp = scene_max.x - scene_min.x;
		tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
		tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
		tmp = 1.f / tmp;
		glScalef(tmp*g_s, tmp*g_s, tmp*g_s);


		// center the model
		glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);

		// if the display list has not been made yet, create a new one and
		// fill it with scene contents
		if (scene_list == 0) {
			scene_list = glGenLists(1);
			glNewList(scene_list, GL_COMPILE);
			// now begin at the root node of the imported data and traverse
			// the scenegraph by multiplying subsequent local transforms
			// together on GL's matrix stack.
			recursive_render(scene, scene->mRootNode);
			glEndList();
		}

		glCallList(scene_list);

		glutSwapBuffers();

		do_motion();
	}
	// -
	// ----------------------------------------------------------------------------
	int loadasset(const char* path)
	{
		// we are taking one of the postprocessing presets to avoid
		// spelling out 20+ single postprocessing flags here.
		scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

		if (scene) {
			get_bounding_box(&scene_min, &scene_max);
			scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
			scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
			scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
			return 0;
		}
		return 1;
	}
	int render(char* plyModelStr)
	{
		struct aiLogStream stream;
		
		glutInitWindowSize(900, 600);
		glutInitWindowPosition(100, 100);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		char *myargv[1];
		int myargc = 1;
		myargv[0] = strdup("calibrate");
		glutInit(&myargc, myargv);

		glutCreateWindow("Structured Light point cloud viewer.");
		setupDrawCallback();
		glutReshapeFunc(&Renderer::reshape);
		setupMouseFunction();
		setupKeyboarFunction();
		// get a handle to the predefined STDOUT log stream and attach
		// it to the logging system. It remains active for all further
		// calls to aiImportFile(Ex) and aiApplyPostProcessing.
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
		aiAttachLogStream(&stream);

		// ... same procedure, but this stream now writes the
		// log messages to assimp_log.txt
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE, "assimp_log.txt");
		aiAttachLogStream(&stream);

		// the model name can be specified on the command line. If none
		// is specified, we try to locate one of the more expressive test 
		// models from the repository (/models-nonbsd may be missing in 
		// some distributions so we need a fallback from /models!).
		if (0 != loadasset(plyModelStr)) {
			return -1;
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);    // Uses default lighting parameters

		glEnable(GL_DEPTH_TEST);

		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glEnable(GL_NORMALIZE);

		// XXX docs say all polygons are emitted CCW, but tests show that some aren't.
		if (getenv("MODEL_IS_BROKEN"))
			glFrontFace(GL_CW);
		glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

		glutGet(GLUT_ELAPSED_TIME);
		glutMainLoop();

		// cleanup - calling 'aiReleaseImport' is important, as the library 
		// keeps internal resources until the scene is freed again. Not 
		// doing so can cause severe resource leaking.
		aiReleaseImport(scene);
		// We added a log stream to the library, it's our job to disable it
		// again. This will definitely release the last resources allocated
		// by Assimp.
		aiDetachAllLogStreams();
		return 0;
	}
	

	////////////////////////TRIANGULATION/////////////////////////////
	std::string m_plyfilename = "mesh.ply";

	// filename of optional vertical correspondence 
	char *m_vmapfilename = "123";

	float m_max_edge_length = 0.1;
	float m_distortion_angle = 1;
	bool m_debug = true;

	bool distorted(const int v1, const int v2, const int v3, const std::vector<CVector<3, double>>& result)
	{
		if (result[v1][2]<0 || result[v2][2]<0 || result[v3][2]<0)
			return true;

		CVector<3, double> d12 = result[v2] - result[v1];
		CVector<3, double> d23 = result[v3] - result[v2];
		CVector<3, double> d31 = result[v1] - result[v3];
		double n12 = GetNorm2(d12);
		double n23 = GetNorm2(d23);
		double n31 = GetNorm2(d31);

		if (n12>m_max_edge_length || n23>m_max_edge_length || n31>m_max_edge_length)
			return true;

		double cos1 = dot(d12, -d31) / (n12*n31);
		double cos2 = dot(-d12, d23) / (n12*n23);
		double cos3 = dot(-d23, d31) / (n23*n31);
		double maxcos = std::max(std::max(cos1, cos2), cos3);

		double t = cos(m_distortion_angle*M_PI / 180);
		return maxcos > t;
	}

	void WritePly(const std::vector<CVector<3, double>>& result, const Field<2, float>& mask, std::string filename)
	{
		// generate face indices
		Field<2, int> index(mask.size());
		for (int y = 0, idx = 0; y<mask.size(1); y++)
		for (int x = 0; x<mask.size(0); x++)
		if (mask.cell(x, y))
			index.cell(x, y) = idx++;
		else
			index.cell(x, y) = -1;

		std::vector<CVector<3, int>> face;
		for (int y = 0; y<mask.size(1) - 1; y++)
		{
			for (int x = 0; x<mask.size(0) - 1; x++)
			{
				// sore in CCW order
				if (mask.cell(x, y) && mask.cell(x + 1, y) && mask.cell(x + 1, y + 1) &&
					!distorted(index.cell(x, y), index.cell(x + 1, y), index.cell(x + 1, y + 1), result))
					face.push_back(make_vector(index.cell(x, y), index.cell(x + 1, y + 1), index.cell(x + 1, y)));
				if (mask.cell(x, y) && mask.cell(x + 1, y + 1) && mask.cell(x, y + 1) &&
					!distorted(index.cell(x, y), index.cell(x + 1, y + 1), index.cell(x, y + 1), result))
					face.push_back(make_vector(index.cell(x, y), index.cell(x, y + 1), index.cell(x + 1, y + 1)));
			}
		}

		TRACE("ply => %s\n", filename.c_str());
		FILE *fw = fopen(filename.c_str(), "wb");
		fprintf(fw,
			"ply\n"
			"format binary_little_endian 1.0\n"
			"element vertex %d\n"
			"property float x\n"
			"property float y\n"
			"property float z\n"
			"element face %d\n"
			"property list uchar int vertex_indices\n"
			"end_header\n", result.size(), face.size());
		for (int i = 0; i<result.size(); i++)
		{
			CVector<3, float> p = make_vector(result[i][0], result[i][1], result[i][2]);
			fwrite(&p, 12, 1, fw);
		}
		for (int i = 0; i<face.size(); i++)
		{
			fputc(3, fw);
			fwrite(&face[i], 12, 1, fw);
		}

		fclose(fw);
	}

	// print usage and exit

	// entry point
	int makeTriangulation(options_t options, Field<2, float> horizontal, Field<2, float> vertical, Field<2, float> mask, CMatrix<3, 3,
		double> matKpro, CMatrix<3, 3, double>  matKcam,
		CMatrix<3, 4, double> proRt, double xi1, double xi2, std::string meshName)
	{
		m_plyfilename = meshName;
		//printf("Angulo de distorcion: %f, se guadara en %s", m_distortion_angle, m_plyfilename);
		try
		{
			/*
			// parse commandline options
			int argi = set_options(argc, argv);

			// horizontal and vertical correspondences between projector and camera
			Field<2,float> horizontal, vertical, mask;
			horizontal.Read(argv[argi++]);
			if (m_vmapfilename)
			vertical.Read(m_vmapfilename);
			image::Read(mask, argv[argi++]);
			options.load(argv[argi++]);
			*/
			CVector<2, double>
				cod1 = make_vector<double>((options.projector_width + 1) / 2.0, options.projector_height*options.projector_horizontal_center),
				cod2 = (make_vector(1.0, 1.0) + mask.size()) / 2;

			// intrinsic matrices of projector and camera
			//CMatrix<3,3,double> matKpro, matKcam;
			//double xi1,xi2;
			//FILE*fr;
			//matKcam.Read(argv[argi++]);
			//fr=fopen(argv[argi++],"rb");
			//if (!fr) throw std::runtime_error("failed to open camera distortion");
			//fscanf(fr,"%lf",&xi2);
			//fclose(fr);
			//matKpro.Read(argv[argi++]);
			//fr=fopen(argv[argi++],"rb");
			//if (!fr) throw std::runtime_error("failed to open projector distortion");
			//fscanf(fr,"%lf",&xi1);
			//fclose(fr);

			// extrinsic matrices of projector and camera
			CMatrix<3, 4, double> camRt;
			//proRt.Read(argv[argi++]);
			camRt = make_diagonal_matrix(1, 1, 1).AppendCols(make_vector(0, 0, 0));//CMatrix<3,4,double>::GetIdentity(); // reconstruction is in camera coordinate frame

			// compute projection matrices of projector and camera
			std::vector<CMatrix<3, 4, double>> matrices(2);
			matrices[0] = matKcam * camRt; // camera
			matrices[1] = matKpro * proRt; // projector

			// fundamental matrix
			CMatrix<3, 3, double> matR;
			CVector<3, double> vecT;
			matR.Initialize(proRt.ptr());
			vecT.Initialize(proRt.ptr() + 9);
			CMatrix<3, 3, double> matF = transpose_of(inverse_of(matKpro)) * GetSkewSymmetric(vecT) * matR * inverse_of(matKcam);

			// triangulate 3d points
			std::vector<CVector<3, double>> result;
			for (int y = 0; y<horizontal.size(1); y++)
			{
				if (y % (horizontal.size(1) / 100) == 0)
					printf("\rtriangulation: %d%% done", 100 * y / horizontal.size(1));

				int nbehind = 0;
				for (int x = 0; x<horizontal.size(0); x++)
				{
					if (mask.cell(x, y))
					{
						// 2D correspondence
						std::vector<CVector<2, double>> p2d(2);

						// camra coordinate
						slib::fmatrix::CancelRadialDistortion(xi2, cod2, make_vector<double>(x, y), p2d[0]);

						// projector coordinate
						double proj_y;
						if (m_vmapfilename)
						{
							proj_y = vertical.cell(x, y);
						}
						else
						{
							CVector<3, double> epiline = matF * GetHomogeneousVector(p2d[0]);
							proj_y = -(epiline[0] * horizontal.cell(x, y) + epiline[2]) / epiline[1];
						}
						slib::fmatrix::CancelRadialDistortion(xi1, cod1, make_vector<double>(horizontal.cell(x, y), proj_y), p2d[1]);

						// triangulate
						CVector<3, double> p3d;
						SolveStereo(p2d, matrices, p3d);

						// save
						result.push_back(p3d);
						if (p3d[2]<0)
							nbehind++;
					}
				}
				if (m_debug && nbehind)
					TRACE("\rfound %d points behind viewpoint.\n", nbehind);
			}
			printf("\n");
			// export triangular mesh in PLY format
			WritePly(result, mask, m_plyfilename);
		}
		catch (const std::exception& e)
		{
			TRACE("error: %s\n", e.what());
			return -1;
		}
		return 0;
	}

};
