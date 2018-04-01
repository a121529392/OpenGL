#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <GL/glut.h>
#define SIZE 512

struct Point3D {
	GLfloat x, y, z;
};

const GLfloat Scale = 10.0;
double theta = 0.0;
bool MotionPlay = false;
bool inSelection = false;
GLuint face[4] = { 5,5,5,5 };

Point3D point[8] = { { -1.0, -1.0, 1.0 },{ -1.0, -1.0, -1.0 },{ 1.0, -1.0, -1.0 },{ 1.0, -1.0, 1.0 },
{ -1.0, 1.0, 1.0 },{ -1.0, 1.0, -1.0 },{ 1.0, 1.0, -1.0 },{ 1.0, 1.0, 1.0 } };

const GLubyte colors[6][3] = { { 0, 255, 0 },{ 0, 0, 255 },
{ 255, 0, 255 },{ 255, 255, 0 },{ 255, 0, 0 },{ 0, 255, 255 } };

const GLfloat text[4][2] = { { 0.0, 1.0 },
{ 0.0, 0.0 },{ 1.0, 0.0 },{ 1.0, 1.0 } };
const GLfloat cubepos[4][3] = { { 1.0, -1.0, -1.0 },
{ -1.0, 1.0, -1.0 },{ -1.0, -1.0, 1.0 },{ 1.0, 1.0, 1.0 }, };

const GLubyte Indices[] = {
	0, 1, 2, 3,
	3, 2, 6, 7,
	7, 6, 5, 4,
	4, 5, 1, 0,
	0, 3, 7, 4,
	1, 5, 6, 2
};

// texture ID

GLuint setup_texture(unsigned char * data, int width, int height)
{
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL GL_BGR_EXT=0x80E0
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);


	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data); //Legacy openGL

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint loadBMP_custom(const char* imagepath)
{

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath, "rb");
	if (!file) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
		getchar(); return 0;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return 0;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) {
		printf("Not a correct BMP file\n");    return 0;
	}
	if (*(int*)&(header[0x1C]) != 24) {
		printf("Not a correct BMP file\n");    return 0;
	}

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

										 // Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file wan be closed
	fclose(file);

	//Setup OpenGL texture
	GLuint textureID = setup_texture(data, width, height);

	// OpenGL has now copied the data. Free our own version
	delete[] data;

	// Return the ID of the texture we just created
	return textureID;
}
GLuint dice[7];
void loadTexture()
{
	dice[1] = loadBMP_custom(".\\data\\one.bmp");
	dice[2] = loadBMP_custom(".\\data\\two.bmp");
	dice[3] = loadBMP_custom(".\\data\\three.bmp");
	dice[4] = loadBMP_custom(".\\data\\four.bmp");
	dice[5] = loadBMP_custom(".\\data\\five.bmp");
	dice[6] = loadBMP_custom(".\\data\\six.bmp");
}
void drawCube(Point3D cube[], GLuint id)
{
	if (inSelection) {
		glPushName(id);
	}
	glBindTexture(GL_TEXTURE_2D, dice[face[id]]);
	for (int i = 0; i < 6; i++) {
		glColor3ubv(colors[i]);
		glBegin(GL_POLYGON);
		for (int j = 0; j < 4; j++) {
			glTexCoord2fv(text[j]);
			glVertex3f(cube[Indices[i * 4 + j]].x, cube[Indices[i * 4 + j]].y, cube[Indices[i * 4 + j]].z);
		}
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	if (inSelection) {
		glPopName();
	}
}
void DrawAxis(GLfloat length = 30.0f)
{ // a little tool for drawing local axis
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0); //X-Axis (Red)
	glVertex3f(0, 0, 0); glVertex3f(length, 0, 0);
	glColor3ub(0, 255, 0); //Y-Axis (Green)
	glVertex3f(0, 0, 0); glVertex3f(0, length, 0);
	glColor3ub(0, 0, 255); //Z-Axis (Blue)
	glVertex3f(0, 0, 0); glVertex3f(0, 0, length);
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(75.0, 75.0, 75.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);

	glRotatef(theta, 0.0, 0.0, 1.0);
	glScalef(Scale, Scale, Scale);

	// 骰子 1~4
	for (int i = 0; i < 4; ++i) {
		glPushMatrix();
		glTranslated(cubepos[i][0], cubepos[i][1], cubepos[i][2]);
		drawCube(point, i);
		glPopMatrix();
	}

	if (!inSelection)
		glutSwapBuffers();
	else
		glFlush();
}

void processHits(GLint hits, GLuint buffer[])
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
		face[nearestID] < 6 ? ++face[nearestID] : 6;
		glutPostRedisplay();
	}
}

void myMotion(int value)
{
	if (MotionPlay) {
		theta++;
		glutPostRedisplay();
		glutTimerFunc(20, myMotion, value);
	}
}

void change(int value)
{
	if (MotionPlay) {
		for (int i = 0; i < 4; ++i) {
			if (face[i] > 1) {
				face[i]--;
			}
		}
		glutTimerFunc(1000, change, value);
	}
}

void myMouse(int button, int state, int x, int y)
{
	GLuint selectBuf[SIZE];
	GLint hits;
	GLint viewport[4];

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		glGetIntegerv(GL_VIEWPORT, viewport);

		glSelectBuffer(SIZE, selectBuf);
		glRenderMode(GL_SELECT);
		glInitNames();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 0.5, 0.5, viewport);

		gluPerspective(45.0f, (GLfloat)viewport[2] / (GLfloat)viewport[3], 1.0, 600.0);

		glMatrixMode(GL_MODELVIEW);
		inSelection = true;
		display();
		inSelection = false;

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glFlush();

		hits = glRenderMode(GL_RENDER);
		processHits(hits, selectBuf);

		glutPostRedisplay();
	}
}

void keys(unsigned char key, int x, int y)
{
	if (key == 'p' || key == 'P') {
		MotionPlay = !MotionPlay;
		glutTimerFunc(20, myMotion, 20);
		glutTimerFunc(1000, change, 30);
	}

	glutPostRedisplay();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 1.0f, 500.0f);

	glMatrixMode(GL_MODELVIEW);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Countdown Cube");

	loadTexture();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glutMouseFunc(myMouse);

	glutMainLoop();
}
