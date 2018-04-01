

#include <cstdlib> 
#include <cstdio>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


typedef GLfloat point3[3]; 
point3 v[] = { { 0.0, 0.0, 1.15 },{ -1.0, -0.58, -0.58 },
{ 1.0, -0.58, -0.58 },{ 0.0, 1.15, -0.58 } };

GLubyte colors[4][3] = { { 241, 249, 109 },{ 58, 102, 65 },
{ 218, 252, 227 },{ 150, 190, 89 } };
GLfloat whiteLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat	lightPos[] = { 0.0f, 0.0f, 1.0f, 1.0f };
int n = 3;
double size = 0.5 / (2*2);
float amove = 1.0; 
float bmove = 1.0;
float cmove = 1.0;
bool move1 = false;
bool move2 = false;
bool move3 = false;

void cube(point3 a) {

	glTranslated(a[0], a[1], a[2]);

	glutSolidCube(size);
}

void tetrahedron(point3 a)
{
	glPushMatrix();
	cube(a);
	glPopMatrix();
}

void divide_tetra(point3 a, point3 b, point3 c, point3 d, int m)
{
	point3 v0, v1, v2, v3, v4, v5;
	int j;
	if (m>0)
	{
		for (j = 0; j<3; j++) v0[j] = (a[j] + b[j]) / 2;
		for (j = 0; j<3; j++) v1[j] = (a[j] + c[j]) / 2;
		for (j = 0; j<3; j++) v2[j] = (a[j] + d[j]) / 2;
		for (j = 0; j<3; j++) v3[j] = (b[j] + c[j]) / 2;
		for (j = 0; j<3; j++) v4[j] = (c[j] + d[j]) / 2;
		for (j = 0; j<3; j++) v5[j] = (b[j] + d[j]) / 2;
		glDisable(GL_LIGHTING);

		glColor3ubv(colors[1]);
		divide_tetra(a, v0, v1, v2, m - 1);
		glColor3ubv(colors[2]);
		divide_tetra(v0, b, v3, v5, m - 1);
		glColor3ubv(colors[3]);
		divide_tetra(v1, v3, c, v4, m - 1);
		glColor3ubv(colors[0]);
		divide_tetra(v2, v5, v4, d, m - 1);
	}
	else {
		glEnable(GL_LIGHTING);
		point3 ans;
		for (j = 0; j<3; j++) ans[j] = (a[j] + b[j]+c[j]+d[j]) / 4;
		tetrahedron(ans);
	}
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(amove, 1.0, 0.0, 0.0);
	glRotatef(bmove, 0.0, 1.0, 0.0);
	glRotatef(cmove, 0.0, 0.0, 1.0);
	glEnable(GL_LIGHTING);
	divide_tetra(v[0], v[1], v[2], v[3], n);

	glFlush();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
			2.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
			2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}
void myTimer(int value)
{
	if (move1) {

		amove += 1.0;
		if (amove > 360.0) amove -= 360.0;
	}
	if (move2) {

		bmove += 1.0;
		if (bmove > 360.0) bmove -= 360.0;
	}
	if (move3) {
		cmove += 1.0;
		if (cmove > 360.0) cmove -= 360.0;
	}
	glutPostRedisplay();
	glutTimerFunc(value, myTimer, value);

}
void myKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'A': case 'a':
		move1 = !move1; 
		move2 = false;
		move3 = false;
		break;
	case 'S': case 's':
		move2 = !move2;
		move1 = false;
		move3 = false;
		break;
	case 'D': case 'd':
		move3 = !move3;		
		move2 = false;
		move1 = false;
		break;
	case 'Q': case 'q':
		if (n > 0) n--, size *=2; break;
	case 'E': case 'e':
		n++; size /= 2; break;
	default:
		break;
	}
	glutPostRedisplay();
}
void init()
{

	glEnable(GL_DEPTH_TEST);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(int argc, char **argv)
{
	if (argc > 1) n = atoi(argv[1]);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("3D Gasket");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutTimerFunc(15, myTimer, 15); 
	glutKeyboardFunc(myKey); 
	init();
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutMainLoop();
}
