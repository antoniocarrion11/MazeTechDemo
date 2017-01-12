#include "imathvec.h"
#include "glut.h"
#include "Material.h"
#include "Noise.h"
#include <windows.h>
#include <wingdi.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#define PI 3.1415926

using namespace std;
using namespace Imath;

// mesh
typedef Vec3<float> Vec3f;
typedef Vec2<float> Vec2f;

struct Mesh {

	// vertex
	vector<Vec3f> dot_vertex;
	vector<Vec3f> dot_normal;
	vector<Vec2f> dot_texture;

	// faces
	vector<int> face_index_vertex;
	vector<int> face_index_normal;
	vector<int> face_index_texture;

};


// global
int width = 1200;
int height = 600;
float ratio = 1.0;
GLuint textureArray[8];
GLuint displayPlaneFace, displayPlaneVer, displayCube, displayBorderFace, displayBorder, displayGoal, displayBlade, displayPlaneNoText, displayBorderNoText, displayBladeNoText, displayGoalNoText;
GLuint  displayPlaneJade, displayBorderJade, displayBladeJade, displayGoalJade, displayFlatPlane, displayCubeJade, displayCubeNoText, displayMarblePlane;
GLuint displayCylinder, displayCylinderFace, displayCylinderJade, displayCylinderNoText, displaySkyBox;
vector<GLuint> activeMats;

// controling parameters
int mouse_button;
int mouse_x = 0;
int mouse_y = 0;
float scale = 1.0;
float x_angle = 1.5;
float y_angle = 45.0;
float z_angle = -70.0;
float view = 0.0;
float view2 = 0.0;
float x_trans = 425.0;
float y_trans = -425.0;
float x_temp = x_trans;
float y_temp = y_trans;
float x_width = 1800;
float xkey_trans = -420.0;
float zkey_trans = 400.0;

int playerX = static_cast<int>(x_trans + 500);
int playerY = static_cast<int>(y_trans + 500);

//Rotate
float moveX = 0;
float moveZ = 0;
float tick = 1.6;
float radius = 9;
float angle_animated_object=0;

//Timer

float angle = 0;
Vec3f moving_position = Vec3f(0, 0, 0);
Vec3f center_position = Vec3f(0, 0, 0);

//Menu Options
int normalBool = 1;
int cameraBool = 1;
int lightBool = 1;
int plane = 1;
bool collision = true;
bool fired = false;
bool turnLeft = false;
bool turnRight = false;

char instruct[100];
char time[100];

int maze[20][20] = {

	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	{ 2, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 2 },
	{ 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 2 },
	{ 2, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 2 },
	{ 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 2 },
	{ 2, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 2 },
	{ 2, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 2 },
	{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 2 },
	{ 2, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 2 },
	{ 2, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 2 },
	{ 2, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 2 },
	{ 2, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 2 },
	{ 2, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 2 },
	{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 2 },
	{ 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 2 },
	{ 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2 },
	{ 2, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 2 },
	{ 2, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2 },
	{ 2, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2 },
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }

};
/* ---------------------------------------- BEGIN:MESH CODE  ---------------------------------------- */


// str to int
int StrToInt(const string &str) {

	int i;

	if (sscanf_s(str.c_str(), "%i", &i) == 1) return i;

	else return 0;
}

// split string

vector<string> split_string(const string& str, const string& split_str) {

	vector<string> stlv_string;
	string part_string("");
	string::size_type i;

	i = 0;

	while (i < str.size()) {

		if (split_str.find(str[i]) != string::npos) {
			stlv_string.push_back(part_string);
			part_string = "";

			while (split_str.find(str[i]) != string::npos) {

				++i;

			}

		}

		else {
			part_string += str[i];
			++i;
		}
	}

	if (!part_string.empty()) {

		stlv_string.push_back(part_string);

	}

	return stlv_string;
}

// load file
Mesh* loadFile(const char* file) {

	Mesh *m = new Mesh;

	m->dot_vertex.clear();

	m->face_index_vertex.clear();

	ifstream infile(file);



	if (infile.fail()) {
		cout << "Error opening file " << file;
		return NULL;

	}
	char current_line[1024];

	while (!infile.eof()) {
		infile.getline(current_line, 1024);

		switch (current_line[0]) {
		case'v':
			float x, y, z;
			switch (current_line[1]) {

			case 'n':
				sscanf_s(current_line, "vn %f %f %f", &x, &y, &z);
				m->dot_normal.push_back(Vec3f(x, y, z));
				break;

			case 't':
				sscanf_s(current_line, "vt %f %f", &x, &y);
				m->dot_texture.push_back(Vec2f(x, y));
				break;

			default:
				sscanf_s(current_line, "v %f %f %f", &x, &y, &z);
				m->dot_vertex.push_back(Vec3f(x, y, z));
				break;
			}
			break;

		case 'f': {
			vector<string> faces = split_string(current_line, " \t.\r\n");
			int vnt[3][3] = { { -1, -1, -1 },{ -1, -1, -1 },{ -1, -1, -1 } };

			string::size_type begin, end;

			for (int i = 0; i < 2; i++) {
				begin = 0;
				int j = 0;
				do {
					end = faces[i + 1].find_first_of("/", begin);
					if (begin < end) {
						vnt[i][j] = StrToInt(faces[i + 1].substr(begin, end - begin)) - 1;
					}
					else {
						vnt[i][j] = -1;
					}
					begin = end + 1;
					j++;

				} while (end != string::npos);
			}

			for (unsigned int i = 3; i < faces.size(); i++) {
				begin = 0;
				int j = 0;

				do {
					end = faces[i].find_first_of("/", begin);
					if (begin < end) {
						vnt[2][j] = StrToInt(faces[i].substr(begin, end - begin)) - 1;
					}
					else {
						vnt[2][j] = -1;
					}
					begin = end + 1;
					j++;

				} while (end != string::npos);

				for (int j = 0; j < 3; j++) {
					m->face_index_vertex.push_back(vnt[j][0]);
					if (vnt[j][1] != -1) m->face_index_texture.push_back(vnt[j][1]);
					if (vnt[j][2] != -1) m->face_index_normal.push_back(vnt[j][2]);
				}


				memcpy(&vnt[1], &vnt[2], sizeof(int) * 3);
			}

		}
				  break;

		default:
			break;

		}
	}

	return m;
}





float areaOfTri(Vec3<float> v1, Vec3<float> v2, Vec3<float> v3){

	float tempValue = (v2 - v1).dot((v3 - v1));
	return tempValue / 2;
}

void calculateNormalPerFace(Mesh* mesh){

	Vec3<float> v1, v2, v3, v4, v5;
	mesh->dot_normal.clear();
	mesh->face_index_normal.clear();

	for (float i = 0; i < mesh->face_index_vertex.size(); i += 3){

		v1 = mesh->dot_vertex[mesh->face_index_vertex[i]];
		v2 = mesh->dot_vertex[mesh->face_index_vertex[i + 1]];
		v3 = mesh->dot_vertex[mesh->face_index_vertex[i + 2]];
		v4 = (v2 - v1);
		v5 = (v3 - v1);
		v4 = v4.cross(v5);
		v4.normalize();

		mesh->dot_normal.push_back(v4);

		float pos = mesh->dot_normal.size() - 1;

		mesh->face_index_normal.push_back(pos);
		mesh->face_index_normal.push_back(pos);
		mesh->face_index_normal.push_back(pos);
	}

}


void calculateNormalPerVertex(Mesh* mesh){

	vector<int> face_index_normalPerFace = mesh->face_index_normal;
	vector<Vec3f> dot_normalPerFace = mesh->dot_normal;

	mesh->dot_normal.clear();
	mesh->face_index_normal.clear();

	Vec3<float> suma; suma.x = 0; suma.y = 0; suma.z = 0;

	for (unsigned int val = 0; val < mesh->dot_vertex.size(); val++){
		mesh->dot_normal.push_back(suma);
	}

	for (long pos = 0; pos < mesh->face_index_vertex.size(); pos++){
		mesh->dot_normal[mesh->face_index_vertex[pos]] += dot_normalPerFace[face_index_normalPerFace[pos]];
	}

	for (unsigned int val = 0; val < mesh->dot_normal.size(); val++){
		int i = 0;
		mesh->dot_normal[val] = mesh->dot_normal[val].normalize();

		if (normalBool == 4) {
			mesh->dot_normal[val] = mesh->dot_normal[val] * areaOfTri(dot_normalPerFace[face_index_normalPerFace[i]], dot_normalPerFace[face_index_normalPerFace[i + 1]], dot_normalPerFace[face_index_normalPerFace[i + 2]]);
			i += 3;
		}
	}

	for (unsigned int pos = 0; pos < mesh->face_index_vertex.size(); pos++){
		mesh->face_index_normal.push_back(mesh->face_index_vertex[pos]);
	}

}


int p[] = { 151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};


double fade(double t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(double t, double a, double b)
{
	return a + t * (b - a);
}

double grad(int hash, double x, double y, double z)
{
	int h = hash & 15;
	double u = h<8 ? x : y, v = h<4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double noise(double x, double y, double z)
{
	// find unit cube that contains point
	int X = (int)floor(x) & 255;
	int	Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;
	// find relative x,y,z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);
	//compute fade curves for each x,y,z
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);
	// hash coordinates of the 8 cube corners
int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
	int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;
	// add blended results from 8 corners of cube
	return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
		grad(p[BA], x - 1, y, z)),
		lerp(u, grad(p[AB], x, y - 1, z),
			grad(p[BB], x - 1, y - 1, z))),
		lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
			grad(p[BA + 1], x - 1, y, z - 1)),
			lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
				grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

double perlinMarble(float x, float y)
{
	double total = 0;
	double scale = 0.005;
	double p = 2.0; //0.1; //persistence 
	double n = 4.0; // number of octaves
	double frequency, amplitude;
	for (int i = 0; i<n; i++) {
		frequency = pow(2, (double)i);
		amplitude = pow(p, (double)-i) * 20;
		total = total + noise(scale*frequency* x, scale*frequency* y, 11.5) * amplitude;
	}
	return sin(2 * (x*scale) + total);

}

// creating a triangulated plane
Mesh* createMarblePlane(int arena_width, int arena_depth, int arena_cell) {

	Mesh *me = new Mesh;

	int n = arena_width / arena_cell;
	int m = arena_depth / arena_cell;

	double perlinValue;

	// vertices
	for (int i = 0; i<n; i++) {
		for (int j = 0; j < m; j++) {

			perlinValue = perlinMarble(i*arena_cell, arena_cell);
			me->dot_vertex.push_back(Vec3<GLfloat>(i*arena_cell, 25*perlinValue, j*arena_cell));

		}

	}


	me->dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));
	me->dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	me->dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	me->dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));

	for (int i = 0; i<(n*m) - m; i++) {
		if ((i + 1) % n == 0) continue;
		me->face_index_vertex.push_back(i); me->face_index_vertex.push_back(i + 1);
		me->face_index_vertex.push_back(i + n);
		me->face_index_vertex.push_back(i + 1); me->face_index_vertex.push_back(i + n + 1);
		me->face_index_vertex.push_back(i + n);

		for (int t = 0; t<6; t++) {
			me->face_index_texture.push_back(3);//0
			me->face_index_texture.push_back(2);//2
			me->face_index_texture.push_back(1);//1
			me->face_index_texture.push_back(2);//0
			me->face_index_texture.push_back(0);//3
			me->face_index_texture.push_back(1);//2
		}
	}

	return me;

}

// creating a triangulated plane
Mesh* createPlane(int arena_width, int arena_depth, int arena_cell) {

	Mesh *me = new Mesh;

	int n = arena_width / arena_cell;
	int m = arena_depth / arena_cell;

	float perlinValue;
	int flat = 0;

	// vertices
	for (int i = 0; i<n; i++) {
		for (int j = 0; j < m; j++) {

			perlinValue = ((rand() % 10) * pow(sin(j*300), 3)) + ((rand() % 5) * pow(cos(i*300),3));
			me->dot_vertex.push_back(Vec3<GLfloat>(i*arena_cell, perlinValue, j*arena_cell));

		}

	}


	me->dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));
	me->dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	me->dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	me->dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));

	for (int i = 0; i<(n*m) - m; i++) {
		if ((i + 1) % n == 0) continue;
		me->face_index_vertex.push_back(i); me->face_index_vertex.push_back(i + 1);
		me->face_index_vertex.push_back(i + n);
		me->face_index_vertex.push_back(i + 1); me->face_index_vertex.push_back(i + n + 1);
		me->face_index_vertex.push_back(i + n);

		for (int t = 0; t<6; t++) {
			me->face_index_texture.push_back(3);//0
			me->face_index_texture.push_back(2);//2
			me->face_index_texture.push_back(1);//1
			me->face_index_texture.push_back(2);//0
			me->face_index_texture.push_back(0);//3
			me->face_index_texture.push_back(1);//2
		}
	}

	return me;

}

Mesh* createFlatPlane(int arena_width, int arena_depth, int arena_cell) {

	Mesh *me = new Mesh;

	int n = arena_width / arena_cell;
	int m = arena_depth / arena_cell;

	int flat = 0;

	// vertices
	for (int i = 0; i<n; i++) {
		for (int j = 0; j < m; j++) {

			me->dot_vertex.push_back(Vec3<GLfloat>(i*arena_cell, flat, j*arena_cell));

		}

	}


	me->dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));
	me->dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	me->dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	me->dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));

	for (int i = 0; i<(n*m) - m; i++) {
		if ((i + 1) % n == 0) continue;
		me->face_index_vertex.push_back(i); me->face_index_vertex.push_back(i + 1);
		me->face_index_vertex.push_back(i + n);
		me->face_index_vertex.push_back(i + 1); me->face_index_vertex.push_back(i + n + 1);
		me->face_index_vertex.push_back(i + n);

		for (int t = 0; t<6; t++) {
			me->face_index_texture.push_back(3);//0
			me->face_index_texture.push_back(2);//2
			me->face_index_texture.push_back(1);//1
			me->face_index_texture.push_back(2);//0
			me->face_index_texture.push_back(0);//3
			me->face_index_texture.push_back(1);//2
		}
	}

	return me;

}

Mesh* createSkyBox(int h, int radio) {
  int n = 100;
  int max = n + n;
  int pop = 1;
  double angle;
  Mesh *m = new Mesh;

  // Generate vertices on top
  m->dot_vertex.push_back(Vec3<GLfloat>(0.0, h + 10, 0.0));
  for (int i = 0; i < n; i++) {
    angle = (M_PI / 180)*(1.0*i * 360) / n;
    m->dot_vertex.push_back(Vec3<GLfloat>(cos(angle)* radio, h, sin(angle)* radio));
  }

  //Ring
  for (int i = 0; i<n; i++) {
    angle = (M_PI / 180)*(i * 360) / n;
	m->dot_vertex.push_back(Vec3<GLfloat>(cos(angle)* (radio + 1000), 1000.0, sin(angle)*(radio + 1000)));
  }
  for (int i = 0; i<n; i++) {
    angle = (M_PI / 180)*(i * 360) / n;
	m->dot_vertex.push_back(Vec3<GLfloat>(cos(angle)* (radio + 1500), 5.0, sin(angle)*(radio + 1500)));
  }

  for (int i = n; i >= 0; i--) {
    m->face_index_vertex.push_back(0); 
    if (i == 0) {
		m->face_index_vertex.push_back(n);
      m->face_index_vertex.push_back(1);
    }
    else {
	  m->face_index_vertex.push_back(i);
      m->face_index_vertex.push_back(i - 1);
    }
  }


  // 2nd faces
  for (pop = 1; pop <= n; pop++) {
    m->face_index_vertex.push_back(pop); m->face_index_vertex.push_back(n + pop);
    if (pop != n) {
      m->face_index_vertex.push_back(n + pop + 1);
    }
    else { m->face_index_vertex.push_back(n + 1); }
    m->face_index_vertex.push_back(pop);
    if (pop != n) {
      m->face_index_vertex.push_back(pop + 1); m->face_index_vertex.push_back(n + pop + 1);
    }
    else {
      m->face_index_vertex.push_back(1); m->face_index_vertex.push_back(n + 1);
    }
  }

  for (pop;  pop <= max; pop++) {
    m->face_index_vertex.push_back(pop); m->face_index_vertex.push_back(n + pop);
    if (pop != max) {
      m->face_index_vertex.push_back(n + pop + 1);
   }

    else { m->face_index_vertex.push_back(max + 1); }
    m->face_index_vertex.push_back(pop);
	if (pop != max) {
      m->face_index_vertex.push_back(pop + 1); m->face_index_vertex.push_back(n + pop + 1);
    }
    else {
      m->face_index_vertex.push_back(n+1); m->face_index_vertex.push_back(max + 1);
    }
  }

  m->dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	m->dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));
	m->dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	m->dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));



	for (int t = 0; t<300; t++) {

		m->face_index_texture.push_back(0);//0
		m->face_index_texture.push_back(2);//1
		m->face_index_texture.push_back(1);//2
		m->face_index_texture.push_back(0);//0
		m->face_index_texture.push_back(3);//2
		m->face_index_texture.push_back(2);//3

	}

  return m;
}

Mesh* createCylinder(int h, int radio) {

	int n = 50;
	double angle;
	Mesh *m = new Mesh;

	m->dot_vertex.push_back(Vec3<GLfloat>(0.0, h, 0.0));
	for (int i = 0; i<n; i++) {
		angle = (M_PI / 180)*(1.0*i * 360) / n;
		m->dot_vertex.push_back(Vec3<GLfloat>(cos(angle)* radio, h, sin(angle)* radio));
	}

	for (int i = 0; i<n; i++) {
		angle = (M_PI / 180)*(i * 360) / n;
		m->dot_vertex.push_back(Vec3<GLfloat>(cos(angle)* radio, 0.0, sin(angle)* radio));
	}
	for (int i = 1; i <= n; i++) {
		m->face_index_vertex.push_back(0); m->face_index_vertex.push_back(i);
		if (i == n) {
			m->face_index_vertex.push_back(1);
		}
		else {
			m->face_index_vertex.push_back(i + 1);
		}
	}

	for (int i = 1; i <= n; i++) {
		m->face_index_vertex.push_back(i); m->face_index_vertex.push_back(n + i);
		if (i != n) {
			m->face_index_vertex.push_back(n + i + 1);
		}
		else { m->face_index_vertex.push_back(n + 1); }
		m->face_index_vertex.push_back(i);
		if (i != n) {
			m->face_index_vertex.push_back(i + 1); m->face_index_vertex.push_back(n + i + 1);
		}
		else {
			m->face_index_vertex.push_back(1); m->face_index_vertex.push_back(n + 1);
		}
	}

	
	m->dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	m->dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));
	m->dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	m->dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));



	for (int t = 0; t<75; t++) {

		m->face_index_texture.push_back(0);//0
		m->face_index_texture.push_back(2);//1
		m->face_index_texture.push_back(1);//2
		m->face_index_texture.push_back(0);//0
		m->face_index_texture.push_back(3);//2
		m->face_index_texture.push_back(2);//3

	}

	return m;

}


Mesh* createCube() {

	Mesh *mesh = new Mesh;

	// Vertexe
	int BOX_WIDTH = 50;
	int BOX_HEIGHT = 50;
	int BOX_DEPTH = 50;

	mesh->dot_vertex.push_back(Vec3<GLfloat>(0.0, BOX_HEIGHT, BOX_DEPTH));
	mesh->dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH));
	mesh->dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, 0.0, BOX_DEPTH));
	mesh->dot_vertex.push_back(Vec3<GLfloat>(0.0, 0.0, BOX_DEPTH));

	mesh->dot_vertex.push_back(Vec3<GLfloat>(0.0, BOX_HEIGHT, 0.0));
	mesh->dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, BOX_HEIGHT, 0.0));
	mesh->dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, 0.0, 0.0));
	mesh->dot_vertex.push_back(Vec3<GLfloat>(0.0, 0.0, 0.0));



	// face 1
	mesh->face_index_vertex.push_back(0);//0
	mesh->face_index_vertex.push_back(2);//1
	mesh->face_index_vertex.push_back(1);//2
	mesh->face_index_vertex.push_back(0);//0
	mesh->face_index_vertex.push_back(3);//2
	mesh->face_index_vertex.push_back(2);//3



	// face 2
	mesh->face_index_vertex.push_back(0);//0
	mesh->face_index_vertex.push_back(5);//1
	mesh->face_index_vertex.push_back(4);//5
	mesh->face_index_vertex.push_back(0);//0
	mesh->face_index_vertex.push_back(1);//5
	mesh->face_index_vertex.push_back(5);//4


	// face 3
	mesh->face_index_vertex.push_back(1);//1
	mesh->face_index_vertex.push_back(6);//2
	mesh->face_index_vertex.push_back(5);//6
	mesh->face_index_vertex.push_back(1);//1
	mesh->face_index_vertex.push_back(2);//6
	mesh->face_index_vertex.push_back(6);//5
	mesh->face_index_vertex.push_back(2);//2
	mesh->face_index_vertex.push_back(7);//3
	mesh->face_index_vertex.push_back(6);//7
	mesh->face_index_vertex.push_back(2);//2
	mesh->face_index_vertex.push_back(3);//7
	mesh->face_index_vertex.push_back(7);//6



	mesh->face_index_vertex.push_back(4);//0
	mesh->face_index_vertex.push_back(3);//3
	mesh->face_index_vertex.push_back(0);//7
	mesh->face_index_vertex.push_back(4);//0
	mesh->face_index_vertex.push_back(7);//7
	mesh->face_index_vertex.push_back(3);//4



	mesh->face_index_vertex.push_back(5);//4 
	mesh->face_index_vertex.push_back(7);//5
	mesh->face_index_vertex.push_back(4);//6
	mesh->face_index_vertex.push_back(5);//4
	mesh->face_index_vertex.push_back(6);//6
	mesh->face_index_vertex.push_back(7);//7



	mesh->dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	mesh->dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));
	mesh->dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	mesh->dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));



	for (int t = 0; t<6; t++) {

		mesh->face_index_texture.push_back(0);//0
		mesh->face_index_texture.push_back(2);//1
		mesh->face_index_texture.push_back(1);//2
		mesh->face_index_texture.push_back(0);//0
		mesh->face_index_texture.push_back(3);//2
		mesh->face_index_texture.push_back(2);//3

	}

	return mesh;
}


Vec3f centerAverage(Vec3f pointA, Vec3f pointB, Vec3f pointC) {
	Vec3f center;
	
	center[0] = (pointA[0] + pointB[0] + pointC[0]) / 3;
	center[1] = (pointA[1] + pointB[1] + pointC[1]) / 3;
	center[2] = (pointA[2] + pointB[2] + pointC[2]) / 3;

	return center;
}

// draw

GLuint meshToDisplayList(Mesh* m, int id, GLuint mat = NULL, int texture = NULL, int normals = 0) {

	GLuint listID = glGenLists(id);

	glNewList(listID, GL_COMPILE);

	if (mat != NULL)
		glCallList(mat);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if (texture != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, textureArray[texture]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
	}

	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < m->face_index_vertex.size(); i++) {

		if (!m->dot_normal.empty() && !m->face_index_normal.empty())
			glNormal3fv(&m->dot_normal[m->face_index_normal[i]].x);

		if (!m->dot_texture.empty() && !m->face_index_texture.empty())
			glTexCoord2fv(&m->dot_texture[m->face_index_texture[i]].x);



		// color

		Vec3f offset = (m->dot_vertex[m->face_index_vertex[i]]);

		glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));

		glVertex3fv(&m->dot_vertex[m->face_index_vertex[i]].x);

	}

	glEnd();

	glDisable(GL_TEXTURE_2D);

	if (normals > 0) {
		glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		for (int i = 0; i < m->face_index_vertex.size(); i += 3) {
			Vec3f center = centerAverage(m->dot_vertex[m->face_index_vertex[i]], m->dot_vertex[m->face_index_vertex[i + 1]], m->dot_vertex[m->face_index_vertex[i + 2]]);
			glVertex3f(center[0], center[1], center[2]);
			glVertex3f(m->dot_normal[m->face_index_normal[i]].x * 0.5 + center[0], m->dot_normal[m->face_index_normal[i]].y * 0.5 + center[1], m->dot_normal[m->face_index_normal[i]].z * 0.5 + center[2]);	
		}
		glEnd();
	}

	glEndList();

	return listID;
}


/* ----------------------------------------- END:MESH CODE  ----------------------------------------- */

/* ------------------------------------------- BEGIN::TEXTURES -------------------------------------- */
// Create from algorithm
void codedTexture(UINT textureArray[], int n) {
	const int TexHeight = 128;
	const int TexWidth = 128;
	// create texture in memory

	Noise planeNoise;
	
	double RGBA;

	GLubyte textureImage[TexHeight][TexWidth][4];
	for (int i = 0; i < TexHeight; i++)
		for (int j = 0; j < TexWidth; j++) {

			RGBA = 20 * (planeNoise.perlinMarble(i * 7, j * 5));

			textureImage[i][j][0] =   255 + RGBA; // red value from 0 to 255
			textureImage[i][j][1] =  255 + RGBA; // green value from 0 to 255 
			textureImage[i][j][2] =  10 + RGBA; // blue value from 0 to 255 
			textureImage[i][j][3] =  100; // alpha value from 0 to 255 
		}
	// setup texture
	glGenTextures(1, &textureArray[n]);
	glBindTexture(GL_TEXTURE_2D, textureArray[n]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // must set to 1 for compact data
										   // glTexImage2D Whith size and minification
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TexWidth, TexHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureImage); // BGRA to include alpha

}


/* ------------------------------------------- END::TEXTURES ----------------------------------------- */

GLubyte *LoadDIBitmap(const char *filename, BITMAPINFO **info) {
	FILE *fp;      // open file pointer
	GLubyte * bits; // bitmap pixel bits
	int bitsize;   // Size of bitmap
	int infosize;  // Size of header information
	BITMAPFILEHEADER header; // File header
							 // try opening the file; use "rb" mode to read this *binary* file.
	if ((fp = fopen(filename, "rb")) == NULL)
		return (NULL);
	// read the file header and any following bitmap information.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		// Couldn't read the file header - return NULL.
		fclose(fp);
		return (NULL);
	}
	// Check for BM reversed.
	if (header.bfType != 'MB') {
		// not a bitmap file - return NULL.
		fclose(fp);
		return (NULL);
	}
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
		// couldn't allocate memory for bitmap info - return NULL.
		fclose(fp);
		return (NULL);
	}
	if (fread(*info, 1, infosize, fp) < infosize) {
		// Couldn't read the bitmap header - return NULL.
		free(*info);
		fclose(fp);
		return (NULL);
	}
	// Now that we have all the header info read in, allocate memory for the bitmap and read *it* in.
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth*(*info)->bmiHeader.biBitCount + 7) / 8 * abs((*info)->bmiHeader.biHeight);
	if ((bits = (GLubyte *)malloc(bitsize)) == NULL) {
		// Couldn't allocate memory - return NULL!
		free(*info);
		fclose(fp);
		return (NULL);
	}
	if (fread(bits, 1, bitsize, fp) < bitsize) {
		// couldn't read bitmap - free memory and return NULL!
		free(*info);
		free(bits);
		fclose(fp);
		return (NULL);
	}
	// OK, everything went fine - return the allocated bitmap.
	fclose(fp);
	return (bits);
}


void bmpTexture(UINT textureArray[], LPSTR file, int n) {
	BITMAPINFO *bitmapInfo; // Bitmap information
	GLubyte    *bitmapBits; // Bitmap data
	if (!file) {
		cout << "texture file not found!" << endl;
		return;
	}
	bitmapBits = LoadDIBitmap(file, &bitmapInfo);
	glGenTextures(1, &textureArray[n]);
	glBindTexture(GL_TEXTURE_2D, textureArray[n]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // must set to 1 for compact data
										   // glTexImage2D Whith size and minification
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bitmapInfo->bmiHeader.biWidth, bitmapInfo->bmiHeader.biHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmapBits);
}
/////////////////////////////////////////////////////////INIT////////////////////////////////////////////////////////////////////////////////////

Mesh* mesh1 = createPlane(1000, 1000, 40);
Mesh* mesh2 = createPlane(1000, 1000, 40);
Mesh* mesh3 = createCube();
Mesh* mesh4 = loadFile("lowpoyltree.obj");
Mesh* mesh5 = loadFile("deer-obj.obj");
Mesh* mesh6 = createCube();
Mesh* mesh7 = loadFile("KeyBlade.obj");
Mesh* mesh8 = createFlatPlane(1000, 1000, 40);
Mesh* mesh9 = createMarblePlane(1000, 1000, 25);
Mesh* mesh10 = createCylinder(40, 10);
Mesh* mesh11 = createCylinder(40, 10);
Mesh* mesh12 = createSkyBox(1000, 15);
Mesh* mesh13 = loadFile("ball.obj");

// init
void init() {

	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);

	activeMats = loadMat();

	calculateNormalPerFace(mesh1);
	calculateNormalPerFace(mesh2);
	calculateNormalPerVertex(mesh2);
	calculateNormalPerFace(mesh3);
	calculateNormalPerVertex(mesh3);
	calculateNormalPerFace(mesh6);
	calculateNormalPerFace(mesh8);
	calculateNormalPerVertex(mesh8);
	calculateNormalPerFace(mesh9);
	calculateNormalPerVertex(mesh9);
	calculateNormalPerFace(mesh10);
	calculateNormalPerVertex(mesh10);
	calculateNormalPerFace(mesh11);
	calculateNormalPerFace(mesh12);
	calculateNormalPerVertex(mesh12);

	bmpTexture(textureArray, "grass.bmp", 0);
	bmpTexture(textureArray, "stones.bmp", 1);
	bmpTexture(textureArray, "treeLeaves.bmp", 2);
	bmpTexture(textureArray, "leopard.bmp", 3);
	bmpTexture(textureArray, "rocks.bmp", 4);
	bmpTexture(textureArray, "deer.bmp", 5);
	codedTexture(textureArray, 6);


	displayPlaneFace = meshToDisplayList(mesh2, 6, activeMats[4], 4);
	displayPlaneVer = meshToDisplayList(mesh2, 7, activeMats[4], 4);
	displayCube = meshToDisplayList(mesh4, 8, activeMats[2], 2);
	displayBorderFace = meshToDisplayList(mesh6, 9, activeMats[1], 1);
	displayBorder = meshToDisplayList(mesh3, 10, activeMats[1], 1);
	displayGoal = meshToDisplayList(mesh5, 11, activeMats[0], 5);
	displayBlade = meshToDisplayList(mesh7, 12, activeMats[4], 3);

	displayPlaneNoText = meshToDisplayList(mesh1, 13, activeMats[4], 0);
	displayBorderNoText = meshToDisplayList(mesh6, 14, activeMats[1], 0);
	displayBladeNoText = meshToDisplayList(mesh7, 15, activeMats[4], 0);
	displayGoalNoText = meshToDisplayList(mesh5, 16, activeMats[0], 0);

	displayPlaneJade = meshToDisplayList(mesh2, 17, activeMats[2], 4);
	displayBorderJade = meshToDisplayList(mesh6, 18, activeMats[2], 1);
	displayBladeJade = meshToDisplayList(mesh7, 19, activeMats[2], 3);
	displayGoalJade = meshToDisplayList(mesh5, 20, activeMats[2], 5);

	displayFlatPlane = meshToDisplayList(mesh8, 21, activeMats[4], 4);
	displayMarblePlane = meshToDisplayList(mesh9, 22, activeMats[4], 4);

	displayCubeNoText = meshToDisplayList(mesh4, 23, activeMats[0], 0);
	displayCubeJade = meshToDisplayList(mesh4, 24, activeMats[2], 2);

	displayCylinder = meshToDisplayList(mesh10, 25, activeMats[4], 5);
	displayCylinderFace = meshToDisplayList(mesh11, 26, activeMats[4], 5);
	displayCylinderNoText = meshToDisplayList(mesh10, 27, activeMats[4], 0);
	displayCylinderJade = meshToDisplayList(mesh10, 28, activeMats[2], 5);
	displaySkyBox = meshToDisplayList(mesh12, 29, activeMats[4], 6);

	ratio = (double)width / (double)height;
}



// reshape
void reshape(int w, int h) {

	width = w;
	height = h;

	if (h == 0) h = 1;
	ratio = 1.0f * w / h;

}

// mouse
void mouse(int button, int state, int x, int y) {
	
	mouse_x = x;
	mouse_y = y;
	mouse_button = button;

}


// motion
void motion(int x, int y) {
	if (cameraBool) {
		if (mouse_button == GLUT_LEFT_BUTTON) {
			y_angle -= (float(y - mouse_y) / height) * 360.0;
			
		}
	}
	else {
		if (mouse_button == GLUT_LEFT_BUTTON) {
			view += (float(y - mouse_y) / height)*360.0;
			view2 += (float(x - mouse_x) / x_width) *360.0;
		}
	}

	if (mouse_button == GLUT_RIGHT_BUTTON) {

		scale += (y - mouse_y) / 100.0;
		if (scale < 0.1) scale = 0.1;
		if (scale > 7)	scale = 7;
	}

	mouse_y = y;

	glutPostRedisplay();
}

// moving
void move() {
  angle = angle + 0.1;

  if (angle > 360) 
	  angle -= 360;
  moving_position.x = center_position.x + 8.0f * sin(angle * PI / 180);
  moving_position.z = center_position.z + 8.0f * cos(angle * PI / 180);
}

// text
void renderBitmapString(float x, float y, float z, char *string) {

	char *c;

	glRasterPos3f(x, y, z);   // fonts position

	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

float ylight = 100.0;
float xlight = 100.0;
bool lightleft = false;

void lightRotation() {

	//Sun Rotation
	if (lightleft) {
		xlight += 2;
	}
	else {
		xlight -= 2;
	}

	if (xlight == 100) {
		lightleft = false;
	}
	else if (xlight == -100) {
		lightleft = true;
	}
}

bool checkLocation() {

	bool isSpace = true;
	if (collision) {
		if (maze[playerY / 50][playerX / 50] > 0) {
			cout << "Collision\n";
			isSpace = false;
		}
	}
	return isSpace;
}


// display

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height / 12);
	glOrtho(-10, 10, -10, 10, -10, 10);

	// view
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1, 1, 1);
	glDisable(GL_LIGHTING);
	if (cameraBool) {
		sprintf(instruct, "Use Left Key and Right Key to rotate");
	}
	if (lightBool == 2) {
		sprintf(instruct, "Use left and right keys to rotate the light");
	}
	if (!cameraBool) {
		sprintf(instruct, "Elevate with up and down, Move forward and back using A and S");
	}
	renderBitmapString(-10.0, 5.0f, 0.0f, instruct); 
	renderBitmapString(-10.0, 0.0f, 0.0f, "Move with WASD, explore the maze and have FUN!");

	float timer = glutGet(GLUT_ELAPSED_TIME);
	timer = timer / 1000;
	sprintf(time, "%f",timer);
	renderBitmapString(4, 0.0f, 0.0f, time);
	glEnable(GL_LIGHTING);

  // projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, height / 10, width, height);

	gluPerspective(45, ratio, 1, 10000);


	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	// lookAt
	if (cameraBool == 1) {
		gluLookAt(0.0, 40.0f, 0.0f,
		40*cos(x_angle), y_angle, 40*sin(z_angle),
			0.0f, 1.0f, 0.0f);
	}
	else if (cameraBool == 2) {
		gluLookAt(0.0f, 40.0f, 0.0f,
			-60.0, 0.0, -50.0f,
			0.0f, 1.0f, 0.0f);
	}
	else if(!cameraBool){
		gluLookAt(0.0f, 40.0f, 0.0f,
			0.0, 0.0, -90.0f,
			0.0f, 1.0f, 0.0f);
	}



	// camera
	glScalef(scale, scale, scale);
	glTranslatef(x_trans, -10.0f, y_trans);
	if (!cameraBool) {
		glRotatef(view, 1.0f, 0.0f, 0.0f);
		glRotatef(view2, 0.0f, 1.0f, 0.0f);
	}

	//Plane
	glPushMatrix();
	glTranslatef(-500, 0, -500);
	if ((normalBool == 1 || normalBool == 4 )&& plane == 1) {
		glCallList(displayPlaneVer);
	}
	else if (normalBool == 0 && plane == 1) {
		glCallList(displayPlaneFace);
	}
	else if (normalBool == 2 && plane == 1) {
		glCallList(displayPlaneNoText);
	}
	else if (normalBool == 3 && plane == 1) {
		glCallList(displayPlaneJade);
	}
	else if (plane == 0) {
		glCallList(displayFlatPlane);
	}
	else if (plane == 2) {
		glCallList(displayMarblePlane);
	}
	glPopMatrix();

	//Keyblade
	glPushMatrix();
	glTranslatef(40 * cos(x_angle), y_angle, 40 * sin(z_angle));
	glTranslatef(xkey_trans, 0.0, zkey_trans);
	glScalef(5, 5, 5);
	glRotatef(-110.4, .216, .000029, 0.0f);

	if (normalBool == 2) {
		glCallList(displayBladeNoText);
	}
	else if (normalBool == 3) {
		glCallList(displayBladeJade);
	}
	else {
		glCallList(displayBlade);
	}
	glPopMatrix();

	//Cylinder
	glPushMatrix();
    if (angle_animated_object<=-360)
		angle_animated_object=0; 
    angle_animated_object -=0.9;
	glRotatef(angle_animated_object, 0,1,0);
    glTranslatef(20, 0, 0);
    if (normalBool == 1 || normalBool == 4) {
		glCallList(displayCylinder);
	}
	else if(normalBool == 0){
		glCallList(displayCylinderFace);
	}
	else if(normalBool == 2){
		glCallList(displayCylinderNoText);
	}
	else if (normalBool == 3) {
		glCallList(displayCylinderJade);
	}

	 if (angle_animated_object <=-360)
		angle_animated_object=0; 
    angle_animated_object -=0.0001;
	glTranslatef(50, 0, 0);
	glRotatef(angle_animated_object, 0,1,0);
	if (normalBool == 2) {
		glCallList(displayGoalNoText);
	}
	else if (normalBool == 3) {
		glCallList(displayGoalJade);
	}
	else {
		glCallList(displayGoal);
	}
	glPopMatrix();

	//SkyBox
	glPushMatrix();
	glTranslatef(-100, -400, 300);
	glCallList(displaySkyBox);
	glPopMatrix();

	//Draw Maze
	glPushMatrix();

	glTranslatef(450, 0, 450);

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {

			if (maze[j][i] == 1) {
				glPushMatrix();
				glTranslatef(18, 0, 0);
				glTranslatef(-(i * 50), -21, -(j * 50));
				glScalef(20, 20, 20);
				if(normalBool == 2){
					glCallList(displayCubeNoText);
				}
				else if (normalBool == 3) {
					glCallList(displayCubeJade);
				}
				else{
					glCallList(displayCube);
				}
				glPopMatrix();
			}

			if (maze[j][i] == 2) {
				glPushMatrix();
				glTranslatef(-(i * 50), 0, -(j * 50));
				if (normalBool == 1 || normalBool == 4) {
					glCallList(displayBorder);
				}
				else if(normalBool == 0){
					glCallList(displayBorderFace);
				}
				else if(normalBool == 2){
					glCallList(displayBorderNoText);
				}
				else if (normalBool == 3) {
					glCallList(displayBorderJade);
				}
				glPopMatrix();
			}
		}
	}

	glPopMatrix();

	//Emissive Material Goal
	glPushMatrix();
	glTranslatef(450, 0, 450);
	glTranslatef(-(2 * 50), 0, -(17 * 50));
	if (normalBool == 2) {
		glCallList(displayGoalNoText);
	}
	else if (normalBool == 3) {
		glCallList(displayGoalJade);
	}
	else {
		glCallList(displayGoal);
	}
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// text
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	glEnable(GL_LIGHTING);
	GLfloat light_position[] = { xlight, ylight , 0, 1 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.5, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	if (lightBool == 0) {
		light_position[0] = { 100 };
		light_position[1] = { 100 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	else if (lightBool == 1) {
		lightRotation();
	}
	glutSwapBuffers();
	move();
}

enum MENU_TYPE
{
	PERFACE,
	PERVERTEX,
	STATIC,
	MOVING,
	CONTROL,
	NOTEXT,
	JADE,
	DIFFERENT,
	FIRSTPERSON,
	ANGLED,
	OVERVIEW,
	FLATPLANE,
	NOISEPLANE,
	SMOOTH,
	FLAT,
	MARBLEPLANE,
	PERFACEVISUAL,
	PERVERTEXVISUAL,
	OFF,
	COLLIDE,
	NOCOLLIDE,
	CREASE,
	CREASEON,
	WEIGHT,
	NAVIGATION
};

void processMenu(int item)
{
	switch (item)
	{
	case FLATPLANE:
		plane = 0;
		break;
	case NOISEPLANE:
		plane = 1;
		normalBool = 0;
		break;
	case MARBLEPLANE:
		plane = 2;
		normalBool = 0;
		break;
	default:
	{              }
	break;
	}
}

void processVisualizationMenu(int item) {
	switch (item)
	{
	case PERFACEVISUAL:
		normalBool = 0;
		displayPlaneFace = meshToDisplayList(mesh2, 6, activeMats[4], 4, 1);
		displayCube = meshToDisplayList(mesh4, 8, activeMats[2], 2, 1);
		displayBorderFace = meshToDisplayList(mesh6, 9, activeMats[1], 1, 1);
		displayGoal = meshToDisplayList(mesh5, 11, activeMats[0], 5, 1);
		displayBlade = meshToDisplayList(mesh7, 12, activeMats[4], 3, 1);

		displayPlaneNoText = meshToDisplayList(mesh1, 13, activeMats[4], 0, 1);
		displayBorderNoText = meshToDisplayList(mesh6, 14, activeMats[1], 0, 1);
		displayBladeNoText = meshToDisplayList(mesh7, 15, activeMats[4], 0, 1);
		displayGoalNoText = meshToDisplayList(mesh5, 16, activeMats[0], 0, 1);

		displayFlatPlane = meshToDisplayList(mesh8, 21, activeMats[4], 4, 1);
		displayMarblePlane = meshToDisplayList(mesh9, 22, activeMats[4], 4, 1);

		displayCubeNoText = meshToDisplayList(mesh4, 23, activeMats[0], 0, 1);

		displayCylinderFace = meshToDisplayList(mesh11, 26, activeMats[4], 5, 1);
		displayCylinderNoText = meshToDisplayList(mesh10, 27, activeMats[4], 0, 1);
		break;

	case PERVERTEXVISUAL:
		displayPlaneVer = meshToDisplayList(mesh2, 7, activeMats[4], 4, 1);
		displayCube = meshToDisplayList(mesh4, 8, activeMats[2], 2, 1);
		displayBorder = meshToDisplayList(mesh3, 10, activeMats[1], 1, 1);
		displayGoal = meshToDisplayList(mesh5, 11, activeMats[0], 5, 1);
		displayBlade = meshToDisplayList(mesh7, 12, activeMats[4], 3, 1);

		displayFlatPlane = meshToDisplayList(mesh8, 21, activeMats[4], 4, 1);
		displayMarblePlane = meshToDisplayList(mesh9, 22, activeMats[4], 4, 1);

		displayCubeNoText = meshToDisplayList(mesh4, 23, activeMats[0], 0, 1);

		displayCylinder = meshToDisplayList(mesh10, 25, activeMats[4], 5, 1);
		displayPlaneNoText = meshToDisplayList(mesh1, 13, activeMats[4], 0, 1);
		displayBorderNoText = meshToDisplayList(mesh6, 14, activeMats[1], 0, 1);
		displayBladeNoText = meshToDisplayList(mesh7, 15, activeMats[4], 0, 1);
		displayGoalNoText = meshToDisplayList(mesh5, 16, activeMats[0], 0, 1);

		displayFlatPlane = meshToDisplayList(mesh8, 21, activeMats[4], 4, 1);
		displayMarblePlane = meshToDisplayList(mesh9, 22, activeMats[4], 4, 1);

		displayCubeNoText = meshToDisplayList(mesh4, 23, activeMats[0], 0, 1);

		displayCylinderFace = meshToDisplayList(mesh11, 26, activeMats[4], 5, 1);
		displayCylinderNoText = meshToDisplayList(mesh10, 27, activeMats[4], 0, 1);
		break;

	case OFF:
		displayPlaneFace = meshToDisplayList(mesh2, 6, activeMats[4], 4);
		displayPlaneVer = meshToDisplayList(mesh2, 7, activeMats[4], 4);
		displayCube = meshToDisplayList(mesh4, 8, activeMats[2], 2);
		displayBorderFace = meshToDisplayList(mesh6, 9, activeMats[1], 1);
		displayBorder = meshToDisplayList(mesh3, 10, activeMats[1], 1);
		displayGoal = meshToDisplayList(mesh5, 11, activeMats[0], 5);
		displayBlade = meshToDisplayList(mesh7, 12, activeMats[4], 3);

		displayPlaneNoText = meshToDisplayList(mesh1, 13, activeMats[4], 0);
		displayBorderNoText = meshToDisplayList(mesh6, 14, activeMats[1], 0);
		displayBladeNoText = meshToDisplayList(mesh7, 15, activeMats[4], 0);
		displayGoalNoText = meshToDisplayList(mesh5, 16, activeMats[0], 0);

		displayFlatPlane = meshToDisplayList(mesh8, 21, activeMats[4], 4);
		displayMarblePlane = meshToDisplayList(mesh9, 22, activeMats[4], 4);

		displayCubeNoText = meshToDisplayList(mesh4, 23, activeMats[0], 0);
		displayCubeJade = meshToDisplayList(mesh4, 24, activeMats[2], 2);

		displayCylinder = meshToDisplayList(mesh10, 25, activeMats[4], 5);
		displayCylinderFace = meshToDisplayList(mesh11, 26, activeMats[4], 5);
		displayCylinderNoText = meshToDisplayList(mesh10, 27, activeMats[4], 0);
		displayCylinderJade = meshToDisplayList(mesh10, 28, activeMats[2], 5);
		break;

	default:
		break;
	}
}

void processRenderingMenu(int item) {
	switch (item) {
		case SMOOTH:
		glShadeModel(GL_SMOOTH);
		break;
	case FLAT:
		glShadeModel(GL_FLAT);
		break;
	}
}
void processLightMenu(int item)
{
	switch (item)
	{

	case STATIC:
		lightBool = 0;
		break;
	case MOVING:
		lightBool = 1;
		break;
	case CONTROL:
		lightBool = 2;
		break;
	case NAVIGATION:
		lightBool = 1;
		cameraBool = 1;
		break;
	default:
	{              }
	break;
	}
}

void processNormalMenu(int item)
{
	switch (item)
	{

	case PERFACE:
		normalBool = 0;
		plane = 1;
		break;
	case PERVERTEX:
		normalBool = 1;
		plane = 1;


		calculateNormalPerFace(mesh2);
		calculateNormalPerVertex(mesh2);
		calculateNormalPerFace(mesh3);
		calculateNormalPerVertex(mesh3);
		calculateNormalPerFace(mesh8);
		calculateNormalPerVertex(mesh8);
		calculateNormalPerFace(mesh9);
		calculateNormalPerVertex(mesh9);
		calculateNormalPerFace(mesh10);
		calculateNormalPerVertex(mesh10);
		calculateNormalPerFace(mesh12);
		calculateNormalPerVertex(mesh12);

		displayPlaneFace = meshToDisplayList(mesh2, 6, activeMats[4], 4);
		displayPlaneVer = meshToDisplayList(mesh2, 7, activeMats[4], 4);
		displayBorder = meshToDisplayList(mesh3, 10, activeMats[1], 1);
		displayPlaneNoText = meshToDisplayList(mesh1, 13, activeMats[4], 0);
		displayFlatPlane = meshToDisplayList(mesh8, 21, activeMats[4], 4);
		displayMarblePlane = meshToDisplayList(mesh9, 22, activeMats[4], 4);
		displayCylinder = meshToDisplayList(mesh10, 25, activeMats[4], 5);
		displayCylinderNoText = meshToDisplayList(mesh10, 27, activeMats[4], 0);
		displayCylinderJade = meshToDisplayList(mesh10, 28, activeMats[2], 5);
		displaySkyBox = meshToDisplayList(mesh12, 29, activeMats[4], 6);

		break;
	case NOTEXT:
		normalBool = 2;
		plane = 1;
		break;
	
	case WEIGHT:
		normalBool = 4;
		plane = 1;

		calculateNormalPerFace(mesh2);
		calculateNormalPerVertex(mesh2);
		calculateNormalPerFace(mesh3);
		calculateNormalPerVertex(mesh3);
		calculateNormalPerFace(mesh8);
		calculateNormalPerVertex(mesh8);
		calculateNormalPerFace(mesh9);
		calculateNormalPerVertex(mesh9);
		calculateNormalPerFace(mesh10);
		calculateNormalPerVertex(mesh10);
		calculateNormalPerFace(mesh12);
		calculateNormalPerVertex(mesh12);

		displayPlaneFace = meshToDisplayList(mesh2, 6, activeMats[4], 4);
		displayPlaneVer = meshToDisplayList(mesh2, 7, activeMats[4], 4);
		displayBorder = meshToDisplayList(mesh3, 10, activeMats[1], 1);
		displayPlaneNoText = meshToDisplayList(mesh1, 13, activeMats[4], 0);
		displayFlatPlane = meshToDisplayList(mesh8, 21, activeMats[4], 4);
		displayMarblePlane = meshToDisplayList(mesh9, 22, activeMats[4], 4);
		displayCylinder = meshToDisplayList(mesh10, 25, activeMats[4], 5);
		displayCylinderNoText = meshToDisplayList(mesh10, 27, activeMats[4], 0);
		displayCylinderJade = meshToDisplayList(mesh10, 28, activeMats[2], 5);
		displaySkyBox = meshToDisplayList(mesh12, 29, activeMats[4], 6);

		break;

		case CREASE:
		normalBool = 1;
		break;
		case CREASEON:
		normalBool = 0;
		break;
	default:
	{              }
	break;
	}
}

void processCameraMenu(int item)
{
	switch (item)
	{

	case OVERVIEW:
		cameraBool = 0;
		if (scale == 1) {
			x_temp = x_trans;
			y_temp = y_trans;
			x_trans = 29;
			y_trans = -445;
			scale = 0.1;
		}
		glutPostRedisplay();
		break;
	case FIRSTPERSON:
		cameraBool = 1;
		scale = 1;
		x_trans = x_temp;
		y_trans = y_temp;
		break;
	case ANGLED:
		cameraBool = 2;
		if (scale == 1) {
			x_temp = x_trans;
			y_temp = y_trans;
			x_trans = -400;
			y_trans = -600;
			scale = 0.1;
		}
		glutPostRedisplay();
		break;
	default:
	{              }
	break;
	}
}

void processMaterialMenu(int item){

	switch (item){

	case NOTEXT:
		normalBool = 2;
		break;
	case JADE:
		normalBool = 3;
		break;
	case DIFFERENT:
		normalBool = 1;
		break;
	default:
	{              }
	break;
	}
}

void processCollision(int item) {

	switch (item) {

	case COLLIDE:
		collision = true;
		break;
	case NOCOLLIDE:
		collision = false;
		break;
	default:
	{           }
	break;
	}
}

void createGLUTMenus() {
	int mainMenu;
	int lightMenu;
	int normalMenu;
	int cameraMenu;
	int materialMenu;
	int textureMenu;
	int rendering;
	int visual;
	int collision;
	int plane;
	int crease;
	int navigation;
	// create the menu and
	// "processMenuEvents" will handle the events
	navigation = glutCreateMenu(processLightMenu);
	glutAddMenuEntry("Move Camera", NAVIGATION);
	glutAddMenuEntry("Control Light", CONTROL);

	visual = glutCreateMenu(processVisualizationMenu);
	glutAddMenuEntry("Per Face Visuals", PERFACEVISUAL);
	glutAddMenuEntry("Per Vertex Visuals", PERVERTEXVISUAL);
	glutAddMenuEntry("Visualization off", OFF);

	lightMenu = glutCreateMenu(processLightMenu);
	glutAddMenuEntry("Static", STATIC);
	glutAddMenuEntry("Moving", MOVING);

	normalMenu = glutCreateMenu(processNormalMenu);
	glutAddMenuEntry("Perface normals", PERFACE);
	glutAddMenuEntry("PerVertex normals", PERVERTEX);
	glutAddMenuEntry("VertexPerWeight normals", WEIGHT);
	glutAddSubMenu("Normal Visualization", visual);

	materialMenu = glutCreateMenu(processMaterialMenu);
	glutAddMenuEntry("Same Material", JADE);
	glutAddMenuEntry("Different Materials", DIFFERENT);

	textureMenu = glutCreateMenu(processMaterialMenu);
	glutAddMenuEntry("Off", NOTEXT);

	cameraMenu = glutCreateMenu(processCameraMenu);
	glutAddMenuEntry("Overview", OVERVIEW);
	glutAddMenuEntry("First Person", FIRSTPERSON);
	glutAddMenuEntry("45 Degree Angle", ANGLED);

	rendering = glutCreateMenu(processRenderingMenu);
	glutAddMenuEntry("Smooth", SMOOTH);
	glutAddMenuEntry("Flat", FLAT);

	collision = glutCreateMenu(processCollision);
	glutAddMenuEntry("ON", COLLIDE);
	glutAddMenuEntry("OFF", NOCOLLIDE);

	plane = glutCreateMenu(processMenu);
	glutAddMenuEntry("Flat Plane", FLATPLANE);
	glutAddMenuEntry("Multiscale Plane", NOISEPLANE);
	glutAddMenuEntry("Marble Plane", MARBLEPLANE);

	crease = glutCreateMenu(processNormalMenu);
	glutAddMenuEntry("OFF", CREASE);
	glutAddMenuEntry("35degrees", CREASEON);

	mainMenu = glutCreateMenu(processNormalMenu);
		glutAddSubMenu("Navigation", navigation);
		glutAddSubMenu("Light", lightMenu);
		glutAddSubMenu("Normals", normalMenu);
		glutAddSubMenu("Viewing", cameraMenu);
		glutAddSubMenu("Textures", textureMenu);
		glutAddSubMenu("Materials", materialMenu);
		glutAddSubMenu("Shading", rendering);
		glutAddSubMenu("Collision", collision);
		glutAddSubMenu("Plane", plane);
		glutAddSubMenu("Crease Angles", crease);
		


	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void lightsControl(int key, int x, int y) {
	GLfloat light_position[] = { xlight, ylight , 0, 1 };

	switch (key) {

	case GLUT_KEY_LEFT:
		if (lightBool == 2) {
			xlight += 5.5;
			light_position[0] = { xlight };
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		}
		else if (cameraBool) {
			x_angle += .1;
			z_angle -= .1;
		}
		else if (!cameraBool) {
			view2 += 1;
		}
		break;

	case GLUT_KEY_RIGHT:
		if (lightBool == 2) {
			xlight -= 5.5;
		light_position[0] = { xlight };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		}
		else if (cameraBool) {
			x_angle -= .1;
			z_angle += .1;
		}
		else if (!cameraBool) {
			view2 -= 1;
		}
		break;
	case GLUT_KEY_DOWN:
		if (scale < 1) {
				scale += 0.1;
			}
			
		break;

	case GLUT_KEY_UP:
			if (scale > 0.2) {
				scale -= 0.1;
			}
		break;
	}
	
}

void playerControl(unsigned char key, int x, int y){

	int prevX = playerX;
	int prevY = playerY;

	switch (key){

	case 'a':
		if (cameraBool == 1) {
			playerX += 1;
			if (checkLocation() && scale == 1) {
				x_trans += 1;
				xkey_trans -= 1;
			}
			else {
				playerX = prevX;
			}
		}
		else if (cameraBool == 0) {
			y_trans += 10;
		}
		break;

	case 'd':

		playerX -= 1;
		if (checkLocation() && scale == 1) {
			x_trans -= 1;
			xkey_trans += 1;
		}
		else {
			playerX = prevX;
		}
		break;

	case 'w':

		playerY += 1;
		if (checkLocation() && scale == 1) {
			y_trans += 1;
			zkey_trans -= 1;
		}
		else {
			playerY = prevY;
		}
		break;

	case 's':
		if (cameraBool == 1) {
			playerY -= 1;

			if (checkLocation() && scale == 1) {
				y_trans -= 1;
				zkey_trans += 1;
			}
			else {
				playerY = prevY;
			}
		}
		else if (cameraBool == 0) {
			y_trans -= 10;
		}
		break;


	}

}


// main
void main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Maze");
	glClearColor(0.2, 0.1, 0.7, 1.0);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(playerControl);
	glutSpecialFunc(lightsControl);

	glutMotionFunc(motion);

	createGLUTMenus();

	init();
	glutMainLoop();
}