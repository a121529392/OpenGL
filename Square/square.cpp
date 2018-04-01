/* Recursive subdivision of triangle to form Sierpinski gasket */
//v3 real 3D tetrahedrons

#include <cstdlib> //atoi()

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <ctime> //clock_t

/* initial tetrahedron */
typedef GLfloat point3[3]; // a 3D point data type
point3 v[] = { { 1.0, 1.0, -1.0 },{ -1.0, 1.0, -1.0 },
{ 1.0, -1.0, -1.0 },{ -1.0, -1.0, -1.0 },{ 1.0, 1.0, 1.0 },
{ -1.0, 1.0, 1.0 },{ 1.0, -1.0, 1.0 },{ -1.0, -1.0, 1.0 } };

GLubyte colors[4][3] = { { 80.0, 174.0, 255.0 },{ 116.0,225.0, 120.0 },
{ 255.0, 132.0, 126.0 },{ 235.0, 164.0, 109.0 } };

int n = 1;
double size = 0.5;
float theta = 0; //STEP:5.1 angle of current spinning cube
float theta1 = 0;
bool move = false;
bool circle = false;
void square(point3 a, point3 b, point3 c, point3 d) {
	/* specify one triangle */
	glBegin(GL_QUADS);
	glVertex3fv(a);
	glVertex3fv(b);
	glVertex3fv(c);
	glVertex3fv(d);
	glEnd();
}

void draw(point3 a, point3 b, point3 c, point3 d,
	point3 e, point3 f, point3 g, point3 h)
{
	if (circle == false) {
		square(a, e, f, b);
		square(a, b, d, c);
		square(c, g, e, a);
		square(d, h, g, c);
		square(b, f, h, d);
		square(e, g, h, f);
	}
	else {
		point3 v0;
		for (int j = 0; j < 3; j++) v0[j] = (b[j] + g[j]) / 2;
		glPushMatrix();
		glTranslated(v0[0], v0[1], v0[2]);
		glutSolidSphere(size, 30, 17);
		glPopMatrix();
	}
}

void divide_square(point3 a, point3 b, point3 c, point3 d,
	point3 e, point3 f, point3 g, point3 h, int m)
{
	point3 v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18;
	int j;
	if (m > 0)
	{

		/* compute six midpoints */
		for (j = 0; j < 3; j++) v0[j] = (a[j] + b[j]) / 2;
		for (j = 0; j < 3; j++) v1[j] = (a[j] + c[j]) / 2;
		for (j = 0; j < 3; j++) v2[j] = (b[j] + d[j]) / 2;
		for (j = 0; j < 3; j++) v3[j] = (c[j] + d[j]) / 2;
		for (j = 0; j < 3; j++) v4[j] = (e[j] + f[j]) / 2;
		for (j = 0; j < 3; j++) v5[j] = (e[j] + g[j]) / 2;
		for (j = 0; j < 3; j++) v6[j] = (f[j] + h[j]) / 2;
		for (j = 0; j < 3; j++) v7[j] = (h[j] + g[j]) / 2;
		for (j = 0; j < 3; j++) v8[j] = (f[j] + b[j]) / 2;
		for (j = 0; j < 3; j++) v9[j] = (e[j] + a[j]) / 2;
		for (j = 0; j < 3; j++) v10[j] = (g[j] + c[j]) / 2;
		for (j = 0; j < 3; j++) v11[j] = (h[j] + d[j]) / 2;
		for (j = 0; j < 3; j++) v12[j] = (a[j] + f[j]) / 2;
		for (j = 0; j < 3; j++) v13[j] = (a[j] + g[j]) / 2;
		for (j = 0; j < 3; j++) v14[j] = (a[j] + d[j]) / 2;
		for (j = 0; j < 3; j++) v15[j] = (d[j] + f[j]) / 2;
		for (j = 0; j < 3; j++) v16[j] = (d[j] + g[j]) / 2;
		for (j = 0; j < 3; j++) v17[j] = (a[j] + h[j]) / 2;
		for (j = 0; j < 3; j++) v18[j] = (e[j] + h[j]) / 2;
		/* create 4 tetrahedrons by subdivision */
		glColor3ubv(colors[0]);
		divide_square(v12, v8, v17, v15, v4, f, v18, v6, m - 1);
		glColor3ubv(colors[3]);
		divide_square(v13, v17, v10, v16, v5, v18, g, v7, m - 1);
		glColor3ubv(colors[2]);
		divide_square(v14, v2, v3, d, v17, v15, v16, v11, m - 1);
		glColor3ubv(colors[1]);
		divide_square(a, v0, v1, v14, v9, v12, v13, v17, m - 1);
	}
	else {
		draw(a, b, c, d, e, f, g, h);
	} /* draw tetrahedron at end of recursion */
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(theta, 0.0, -1.0, 0.0); //STEP:5.3 Rotate it!
	glRotatef(theta, 1.0, 0.0, 0.0);

	//glRotatef(theta1, 0.0, -1.0, 1.0); //STEP:5.3 Rotate it!

	divide_square(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], n);
	/*draw(v[0], v[1], v[2], v[3],
	v[4],v[5],v[6],v[7]);*/

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
	glutPostRedisplay();
}
void spinIt() //STEP:5.2 spin it
{
	clock_t now = clock(); //need time.h
	static clock_t lastTime = 0;
	if ((now - lastTime) / (float)CLOCKS_PER_SEC > (1.0f / 60.0f)) { //<60fps

		if (theta > 360.0) theta -= 360.0;
		glutPostRedisplay();

		// Record current time for next time:
		lastTime = now;
	}
}
void myTimer(int value)
{
	if (move) {
		theta += 1.0;
		if (theta > 360.0) theta -= 360.0;
	}
	glutPostRedisplay();
	glutTimerFunc(value, myTimer, value);
}
void myKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'P': case 'p':
		move = !move;
		break;
	case 'S': case 's':
		circle = !circle;
		break;
	case 'Q': case 'q':
		if (n > 1) n--;
		if (size < 0.5) size *= 2;
		break;
	case 'E': case 'e':
		if (n < 10) { n++; size /= 2; }    break;
	default:
		break;
	}
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	if (argc > 1) n = atoi(argv[1]); /* or set number of subdivision steps here */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("3D Gasket");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutTimerFunc(20, myTimer, 20); //~50fps
	glutKeyboardFunc(myKey); //add keyboard function
							 //glutIdleFunc(spinIt); //STEP:5 add spin function
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutMainLoop();
}
