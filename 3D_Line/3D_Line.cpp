#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <GL/glut.h>
#include "objloader.hpp"
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SIZE 512

GLfloat BodyAngle = 0.0;
GLfloat CameraTRotX = 0.0, CameraTRotY = 0.0;

int lastPosX = -1, lastPosY = -1;
GLfloat ScaleX = 1.0f, ScaleY = 1.0f;
GLfloat CamShiftFB = 0.0f, CamShiftLR = 0.0f;
bool inSelection = false;

bool MotionPlay = false;
bool ActiveMouse = false;
bool Ctrl_actived = false;

std::vector<vec3> vertices;
std::vector<vec2> uvs;
std::vector<vec3> normals;
std::vector<int> color;
std::vector<bool> ishidden;


void DrawAxis(GLfloat length = 30.0f)
{
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0); //X-Axis (Red)
	glVertex3f(0, 0, 0); glVertex3f(length, 0, 0);
	glColor3ub(0, 255, 0); //Y-Axis (Green)
	glVertex3f(0, 0, 0); glVertex3f(0, length, 0);
	glColor3ub(0, 0, 255); //Z-Axis (Blue)
	glVertex3f(0, 0, 0); glVertex3f(0, 0, length);
	glEnd();
}


void processhit(GLuint hits, GLuint buffer[])
{
	GLuint i, names, *ptr;
	ptr = (GLuint *)buffer;


	GLuint nearestID = -1, nearestDistance = 4294967295, currentDist;
	for (i = 0; i < hits; i++) {
		names = *ptr;
		currentDist = (*(ptr + 1) + *(ptr + 2)) / 2;
		ptr += 3;
		if (currentDist < nearestDistance) {
			nearestDistance = currentDist;
			nearestID = *ptr;
		}
		ptr++;
	}
	if (nearestID != -1) {
		if (ishidden[nearestID] == false) ishidden[nearestID] = true;
		else ishidden[nearestID] = false;
	}
}


void UniObj(void)
{
	float ux = FLT_MIN, uy = FLT_MIN, uz = FLT_MIN;
	float lx = FLT_MAX, ly = FLT_MAX, lz = FLT_MAX;
	for (unsigned int i = 0; i < vertices.size(); i++) {
		if (vertices[i].x > ux) ux = vertices[i].x;
		if (vertices[i].y > uy) uy = vertices[i].y;
		if (vertices[i].z > uz) uz = vertices[i].z;

		if (vertices[i].x < lx) lx = vertices[i].x;
		if (vertices[i].y < ly) ly = vertices[i].y;
		if (vertices[i].z < lz) lz = vertices[i].z;
	}
	float sx = abs(ux - lx), sy = abs(uy - ly), sz = abs(uz - lz);
	float cx = (ux + lx) / 2.0f, cy = (uy + ly) / 2.0f, cz = (uz + lz) / 2.0f;
	float scale = MAX(sx, MAX(sy, sz));
	for (unsigned int i = 0; i < vertices.size(); i++) {
		vertices[i].x = (vertices[i].x - cx) / scale;
		vertices[i].y = (vertices[i].y - cy) / scale;
		vertices[i].z = (vertices[i].z - cz) / scale;
	}
}

void loadModel()
{

	if (loadOBJ(".\\data\\icosphere.obj", vertices, uvs, normals)) {
		UniObj();
		ishidden.assign(vertices.size() / 3, false);
	}
}

void DrawObj(void)
{
	for (unsigned int i = 0; i < vertices.size(); i += 3) {
		if (i % 2 == 0) {//偶數不畫
			if (ishidden[i / 3] == 0) {
				if (inSelection) glPushName(i / 3);

				glBegin(GL_TRIANGLES);

				glColor3ub(116, 40, 148);
				glNormal3f(normals[i].x, normals[i].y, normals[i].z);
				glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);

				glNormal3f(normals[i + 1].x, normals[i + 1].y, normals[i + 1].z);
				glVertex3f(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z);

				glNormal3f(normals[i + 2].x, normals[i + 2].y, normals[i + 2].z);
				glVertex3f(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z);
				glEnd();

				if (inSelection) glPopName();
			}
		}
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(75.0, 75.0, 75.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(CameraTRotX, 1.0, 0.0, 0.0);
	glRotatef(CameraTRotY, 0.0, 1.0, 0.0);

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glEnable(GL_COLOR_MATERIAL);
	glColor3ub(116, 40, 148);


	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	GLfloat zero[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.8, 0.0, 1.0 };
	GLfloat specular[] = { 0.5, 0.0, 0.0, 1.0 };
	GLfloat shine = 100.0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);

	GLfloat back_specular[] = { 0.5, 1.0, 0.0, 1.0 };
	GLfloat back_shine = 1.0;
	glMaterialfv(GL_BACK, GL_SPECULAR, back_specular);
	glMaterialf(GL_BACK, GL_SHININESS, back_shine);
	glScalef(60.0f, 60.0f, 60.0f);
	glRotatef(BodyAngle, 0.0f, 1.0f, 0.0f);
	DrawAxis(50.0f);
	DrawObj();
	glDisable(GL_LIGHTING);

	if (inSelection)
		glFlush();
	else
		glutSwapBuffers();
}


void myTimer(int value)
{
	if (MotionPlay) {
		BodyAngle++;
		glutPostRedisplay();
		glutTimerFunc(value, myTimer, value);
	}
}
void keys(unsigned char key, int x, int y)
{
	if (key == 'p' || key == 'P') {
		MotionPlay = !MotionPlay;
		glutTimerFunc(20, myTimer, 20);
	}
}

void myMouseMotion(int x, int y)
{
	if (ActiveMouse) {
		int dx, dy;
		dx = x - lastPosX;
		dy = y - lastPosY;
		CameraTRotY += dx*ScaleX;
		CameraTRotX += dy*ScaleY;
		glutPostRedisplay();
		lastPosX = x;
		lastPosY = y;
	}
}

void myMouseButton(int button, int state, int x, int y)
{
	GLint viewport[4];
	GLuint selectbuf[SIZE];
	GLuint hits;

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		glGetIntegerv(GL_VIEWPORT, viewport);

		glSelectBuffer(SIZE, selectbuf);
		glRenderMode(GL_SELECT);

		glInitNames();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 0.5, 0.5, viewport);
		gluPerspective(45.0, (GLdouble)viewport[2] / (GLdouble)viewport[3], 1.0f, 500.0f);
		glMatrixMode(GL_MODELVIEW);

		inSelection = true;
		display();
		inSelection = false;

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		hits = glRenderMode(GL_RENDER);
		processhit(hits, selectbuf);

		glutPostRedisplay();

	}
	if (button == GLUT_LEFT_BUTTON)
		switch (state) {
		case GLUT_DOWN:
			lastPosX = x;
			lastPosY = y;
			ActiveMouse = true;
			break;
		case GLUT_UP:
			ActiveMouse = false;
			break;
		}
}

void initLight(void)
{
	GLfloat  ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat  diffuseLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat  specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat  Lposition[] = { 100.0f, 100.0f, 100.0f, 1.0f };

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, Lposition);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE); // normalize the input normal (important step), it requires OpenGL to rescale all of the normal vectors to have a magnitude of 1.

	glClearColor(0.2, 0.2, 0.2, 1.0);
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 1.0f, 500.0f);

	ScaleX = 180.0f / w;
	ScaleY = 180.0f / h;

	glMatrixMode(GL_MODELVIEW);
	initLight();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Object Picker+Lighter MODE:A");

	loadModel();
	glEnable(GL_DEPTH_TEST);

	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);

	glutMouseFunc(myMouseButton);
	glutMotionFunc(myMouseMotion);
	glutTimerFunc(20, myTimer, 20);
	glutMainLoop();
}