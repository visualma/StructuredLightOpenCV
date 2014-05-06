
#include <stdio.h>
#include <GL/glut.h>
#include "MathBaseLapack.h"
#include "MiscUtil.h"
#include "Stereo.h"
#include "LeastSquare.h"
#include "../FundamentalMatrix.h"
#include "../Options.h"
#include "Field.h"
#include "ImageBmpIO.h"
#include <vector>
#include "opencv2\opencv.hpp"

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

using namespace slib;

static int g_X;
static int g_Y;
static float g_ms;
class simpleRenderer
{
public:
	simpleRenderer()
	{
		angle = 0.0f;
		g_ms = 1.0f;
	}
	~simpleRenderer()
	{}
private:
	void setupDrawCallback();
	void setupMouseFunction();
	void setupKeyboarFunction();
public:
	float angle;
	vector<CVector<3, double>> vertex_normals, vertices;
	vector<CVector<2, double>> texture_coords;
	vector<CVector<3, int>> face;

	CVector<3, double> m_sceneMin, m_sceneMax, m_sceneCenter;

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
		g_X = x;
		g_Y = 768 - y;
		printf("%d", g_X);
		glutPostRedisplay();
	}

	void getBoundingBox()
	{
		m_sceneMin = make_vector(20000, 20000, 20000);
		m_sceneMax = make_vector(-20000, -20000, -20000);
		for (int i = 0; i < vertices.size(); i++)
		{
			m_sceneMin[0] = min(m_sceneMin[0], vertices[i][0]);
			m_sceneMin[1] = min(m_sceneMin[1], vertices[i][1]);
			m_sceneMin[2] = min(m_sceneMin[2], vertices[i][2]);

			m_sceneMax[0] = max(m_sceneMax[0], vertices[i][0]);
			m_sceneMax[1] = max(m_sceneMax[1], vertices[i][1]);
			m_sceneMax[2] = max(m_sceneMax[2], vertices[i][2]);
		}
		m_sceneCenter[0] = (m_sceneMax[0] + m_sceneMin[0]) / 2.0f;
		m_sceneCenter[1] = (m_sceneMax[1] + m_sceneMin[1]) / 2.0f;
		m_sceneCenter[2] = (m_sceneMax[2] + m_sceneMin[2]) / 2.0f;
	}

	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.f, 0.f, 3.f, 0.f, 0.f, -5.f, 0.f, 1.f, 0.f);

		float width = glutGet(GLUT_WINDOW_WIDTH);
		float height = glutGet(GLUT_WINDOW_HEIGHT);
		float rX = (g_mX / width) * 90 - 45;
		float rY = (g_mY / height) * 90 - 45 + 180;
		glRotatef(rY, 1.0, 0.f, 0.f);
		glRotatef(-rX, 0.0f, 1.0f, 0.0f);

		float tmp;
		tmp = m_sceneMax[0] - m_sceneMin[0];
		tmp = aisgl_max(m_sceneMax[1] - m_sceneMin[1], tmp);
		tmp = aisgl_max(m_sceneMax[2] - m_sceneMin[2], tmp);
		tmp = 1.f / tmp;
		glScalef(tmp*g_s, tmp*g_s, tmp*g_s);

		glTranslatef(-m_sceneCenter[0], -m_sceneCenter[1], -m_sceneCenter[2]);
		*/

		glBegin(GL_TRIANGLES);
		glVertex3f(-0.5, -0.5, 0.0);
		glVertex3f(0.5, 0.0, 0.0);
		glVertex3f(0.0, 0.5, 0.0);
		glEnd();

		glutSwapBuffers();
	}

	void init()
	{
		
		char *myargv[1];
		int myargc = 1;
		myargv[0] = strdup("calibrate");
		glutInit(&myargc, myargv);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(900, 600);
		glutCreateWindow("Structured Light point cloud viewer.");

		setupDrawCallback();
		glutReshapeFunc(&simpleRenderer::reshape);
		setupMouseFunction();
		setupKeyboarFunction();

		glutMainLoop();
	}










	//////////////////

	// filename of optional vertical correspondence 
	char *m_vmapfilename = "123";

	float m_max_edge_length = 0.1;
	float m_distortion_angle = 1;
	bool m_debug = true;
	std::string m_plyfilename = "mesh.ply";

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
		for (int y = 0, idx = 0; y < mask.size(1); y++)
		for (int x = 0; x < mask.size(0); x++)
		if (mask.cell(x, y))
			index.cell(x, y) = idx++;
		else
			index.cell(x, y) = -1;

		for (int y = 0; y < mask.size(1) - 1; y++)
		{
			for (int x = 0; x < mask.size(0) - 1; x++)
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
		for (int i = 0; i < result.size(); i++)
		{
			CVector<3, float> p = make_vector(result[i][0], result[i][1], result[i][2]);
			fwrite(&p, 12, 1, fw);
		}
		for (int i = 0; i < face.size(); i++)
		{
			fputc(3, fw);
			fwrite(&face[i], 12, 1, fw);
		}

		fclose(fw);
	}

	void WriteObj(std::string path, std::string filename)
	{
		TRACE("obj => %s\n", filename.c_str());
		FILE *fl = fopen(path.c_str(), "w");
		fprintf(fl, "g %s\n", filename.c_str());
		/*respaldo
		for (int i = 0; i<vertices.size(); i++)
		{
		fprintf(fl, "v %g %g %g\n", vertices[i][0], vertices[i][1], vertices[i][2]);
		}
		for (int i = 0; i < vertex_normals.size(); i++)
		{
		fprintf(fl, "vn %g %g %g\n", vertex_normals[i][0], vertex_normals[i][1], vertex_normals[i][2]);
		}
		for (int i = 0; i < face.size(); i++)
		{
		fprintf(fl, "f %d", face[i][0] + 1);
		fprintf(fl, " %d", face[i][1] + 1);
		fprintf(fl, " %d\n", face[i][2] + 1);
		}
		*/
		for (int i = 0; i < vertices.size(); i++)
		{
			fprintf(fl, "v %g %g %g\n", vertices[i][0], vertices[i][1], vertices[i][2]);
		}
		for (int i = 0; i < vertex_normals.size(); i++)
		{
			fprintf(fl, "vn %g %g %g\n", vertex_normals[i][0], vertex_normals[i][1], vertex_normals[i][2]);
		}
		for (int i = 0; i < texture_coords.size(); i++)
		{
			fprintf(fl, "vt %g %g\n", texture_coords[i][0], texture_coords[i][1]);
		}
		for (int i = 0; i < face.size(); i++)
		{
			fprintf(fl, "f %d/%d", face[i][0] + 1, i);
			fprintf(fl, " %d/%d", face[i][1] + 1, i);
			fprintf(fl, " %d/%d\n", face[i][2] + 1, i);
		}
		fclose(fl);
	}
	// print usage and exit

	// entry point
	int makeTriangulation(options_t options, Field<2, float> horizontal, Field<2, float> vertical, Field<2, float> mask, CMatrix<3, 3,
		double> matKpro, CMatrix<3, 3, double>  matKcam,
		CMatrix<3, 4, double> proRt, double xi1, double xi2)
	{
		face.clear();
		vertices.clear();
		vertex_normals.clear();
		texture_coords.clear();
		std::vector<CVector<3, double>> result;
		//printf("Angulo de distorcion: %f, se guadara en %s", m_distortion_angle, m_plyfilename);
		try
		{
			CVector<2, double>
				cod1 = make_vector<double>((options.projector_width + 1) / 2.0, options.projector_height*options.projector_horizontal_center),
				cod2 = (make_vector(1.0, 1.0) + mask.size()) / 2;

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
			for (int y = 0; y < horizontal.size(1); y++)
			{
				if (y % (horizontal.size(1) / 100) == 0)
					printf("\rtriangulation: %d%% done", 100 * y / horizontal.size(1));

				int nbehind = 0;
				for (int x = 0; x < horizontal.size(0); x++)
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
						if (p3d[2] < 0)
							nbehind++;
					}
				}
				if (m_debug && nbehind)
					TRACE("\rfound %d points behind viewpoint.\n", nbehind);
			}
			printf("\n");
			// export triangular mesh in PLY format
		}
		catch (const std::exception& e)
		{
			TRACE("error: %s\n", e.what());
			return -1;
		}


		Field<2, int> index(mask.size());
		for (int y = 0, idx = 0; y < mask.size(1); y++)
		for (int x = 0; x < mask.size(0); x++)
		if (mask.cell(x, y))
			index.cell(x, y) = idx++;
		else
			index.cell(x, y) = -1;

		for (int y = 0; y < mask.size(1) - 1; y++)
		{
			for (int x = 0; x < mask.size(0) - 1; x++)
			{
				// sore in CCW order
				if (mask.cell(x, y) && mask.cell(x + 1, y) && mask.cell(x + 1, y + 1) &&
					!distorted(index.cell(x, y), index.cell(x + 1, y), index.cell(x + 1, y + 1), result))
				{
					face.push_back(make_vector(index.cell(x, y), index.cell(x + 1, y + 1), index.cell(x + 1, y)));
					texture_coords.push_back(make_vector((double)x / mask.size(0), (double)(mask.size(1) - y) / mask.size(1)));
				}

				if (mask.cell(x, y) && mask.cell(x + 1, y + 1) && mask.cell(x, y + 1) &&
					!distorted(index.cell(x, y), index.cell(x + 1, y + 1), index.cell(x, y + 1), result))
				{
					face.push_back(make_vector(index.cell(x, y), index.cell(x, y + 1), index.cell(x + 1, y + 1)));
					texture_coords.push_back(make_vector((double)x / mask.size(0), (double)(mask.size(1) - y) / mask.size(1)));
				}

			}
		}
		vertex_normals = GenerateVertexNormalsFromVertices(face, result);
		vertices = result;


		//Rotate X for OBJ model;
		for (int i = 0; i < vertices.size(); i++)
		{
			double tempY = vertices[i][1] * cos(M_PI) - vertices[i][2] * sin(M_PI);
			double tempZ = vertices[i][1] * sin(M_PI) + vertices[i][2] * cos(M_PI);
			vertices[i][1] = tempY;
			vertices[i][2] = tempZ;
		}

		return 0;
	}

	vector<CVector<3, double>> GenerateVertexNormalsFromVertices(vector<CVector<3, int>> triangles, vector<CVector<3, double>> vertices)

	{
		vector<CVector<3, double>> face_normals(triangles.size()), vertex_normals(vertices.size());
		try
		{
			//face_normals.reserve(triangles.size());
			//face_normals.resize(triangles.size());
		}
		catch (bad_alloc)
		{
			throw runtime_error("Memory allocation failure.");
		}

		for (size_t i = 0; i < triangles.size(); i++)
		{
			CVector<3, float> v[3];

			// for each vertex in the triangle
			for (unsigned char j = 0; j < 3; j++)
			{
				// else, extract position data
				v[j][0] = vertices[triangles[i][j] + 1][0];
				v[j][1] = vertices[triangles[i][j] + 1][1];
				v[j][2] = vertices[triangles[i][j] + 1][2];

			}

			// calculate vectors along two triangle edges
			double x1 = v[0][0] - v[1][0];
			double y1 = v[0][1] - v[1][1];
			double z1 = v[0][2] - v[1][2];

			double x2 = v[1][0] - v[2][0];
			double y2 = v[1][1] - v[2][1];
			double z2 = v[1][2] - v[2][2];

			// calculate face normal through cross product
			face_normals[i][0] = (y1 * z2) - (z1 * y2);
			face_normals[i][1] = (z1 * x2) - (x1 * z2);
			face_normals[i][2] = (x1 * y2) - (y1 * x2);

			double len = sqrt(face_normals[i][0] * face_normals[i][0] + face_normals[i][1] * face_normals[i][1] + face_normals[i][2] * face_normals[i][2]);
			if (len != 1.0)
			{
				face_normals[i][0] /= len;
				face_normals[i][1] /= len;
				face_normals[i][2] /= len;
			}
		}
		// resize vertex normals
		try
		{
			//vertex_normals.reserve(vertices.size());
			//vertex_normals.resize(vertices.size());
		}
		catch (bad_alloc)
		{
			throw runtime_error("Memory allocation failure");
		}
		vector<vector<CVector<3, double>>> temp_normals(vertices.size());
		temp_normals.resize(vertex_normals.size());
		size_t normal_index = 0;
		for (size_t i = 0; i < triangles.size(); i++)
		{
			temp_normals[triangles[i][0] + 1].push_back(face_normals[normal_index]);
			temp_normals[triangles[i][1] + 1].push_back(face_normals[normal_index]);
			temp_normals[triangles[i][2] + 1].push_back(face_normals[normal_index]);
			normal_index++;
		}
		for (size_t i = 0; i < vertex_normals.size(); i++)
		{
			double temp_x = 0.0f;
			double temp_y = 0.0f;
			double temp_z = 0.0f;
			// add up all face normals associated with this vertex
			for (size_t j = 0; j < temp_normals[i].size(); j++)
			{
				double local_temp_x = temp_normals[i][j][0];
				double local_temp_y = temp_normals[i][j][1];
				double local_temp_z = temp_normals[i][j][2];
				double local_len = sqrt(local_temp_x*local_temp_x + local_temp_y*local_temp_y + local_temp_z*local_temp_z);

				if (local_len != 1.0f)
				{
					local_temp_x /= local_len;
					local_temp_y /= local_len;
					local_temp_z /= local_len;
				}
				temp_x += local_temp_x;
				temp_y += local_temp_y;
				temp_z += local_temp_z;
			}

			// average them using a flat linear
			temp_x /= temp_normals[i].size();
			temp_y /= temp_normals[i].size();
			temp_z /= temp_normals[i].size();

			// normalize the final result

			double len = sqrt(temp_x*temp_x + temp_y*temp_y + temp_z*temp_z);
			if (len != 1.0f)
			{
				temp_x /= len;
				temp_y /= len;
				temp_z /= len;
			}

			vertex_normals[i][0] = temp_x;
			vertex_normals[i][1] = temp_y;
			vertex_normals[i][2] = temp_z;

		}
		return vertex_normals;
	}

	GLuint matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter)
	{
		// Generate a number for our textureID's unique handle
		GLuint textureID;
		glGenTextures(1, &textureID);

		// Bind to our texture handle
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Catch silly-mistake texture interpolation method for magnification
		if (magFilter == GL_LINEAR_MIPMAP_LINEAR ||
			magFilter == GL_LINEAR_MIPMAP_NEAREST ||
			magFilter == GL_NEAREST_MIPMAP_LINEAR ||
			magFilter == GL_NEAREST_MIPMAP_NEAREST)
		{
			cout << "You can't use MIPMAPs for magnification - setting filter to GL_LINEAR" << endl;
			magFilter = GL_LINEAR;
		}

		// Set texture interpolation methods for minification and magnification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

		// Set incoming texture format to:
		// GL_BGR       for CV_CAP_OPENNI_BGR_IMAGE,
		// GL_LUMINANCE for CV_CAP_OPENNI_DISPARITY_MAP,
		// Work out other mappings as required ( there's a list in comments in main() )
		GLenum inputColourFormat = GL_BGR_EXT;
		if (mat.channels() == 1)
		{
			inputColourFormat = GL_LUMINANCE;
		}

		// Create the texture
		glTexImage2D(GL_TEXTURE_2D,     // Type of texture
			0,                 // Pyramid level (for mip-mapping) - 0 is the top level
			GL_RGB,            // Internal colour format to convert to
			mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
			mat.rows,          // Image height i.e. 480 for Kinect in standard mode
			0,                 // Border width in pixels (can either be 1 or 0)
			inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
			GL_UNSIGNED_BYTE,  // Image data type
			mat.ptr());        // The actual image data itself

		// If we're using mipmaps then generate them. Note: This requires OpenGL 3.0 or higher
		/*
		if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
		{
		glGenerateMipmap(GL_TEXTURE_2D);
		}
		*/
		return textureID;
	}
};