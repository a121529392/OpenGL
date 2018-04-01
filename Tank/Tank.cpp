#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.141592654f

GLfloat CarBodySize = 10.0f;
GLfloat CarWheelSize = 3.5f;
GLfloat CarWheelAngleL = 0.0f;
GLfloat CarWheelAngleR = 0.0f;
GLfloat CarBodyAngle = 0.0f;

GLfloat CarTurnSpeed = 3.0f;
GLfloat WheelRotSpeed = 10.0f;

GLfloat CarPosX = 0.0f;
GLfloat CarPosZ = 0.0f;
GLfloat CameraDist = 60.0f;

void DrawAxis(GLfloat length = 30.0f)
{
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0); //X-Axis  red
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(length, 0.0f, 0.0f);
	glColor3ub(0, 255, 0); //Y-Axis  green
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, length, 0.0f);
	glColor3ub(0, 0, 255); //Z-Axis  blue
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, length);
	glEnd();
}

//DO NOT Modify this function!
void DrawGrids(void)
{
	//DrawAxis(30.0f);
	glBegin(GL_LINES);
	glColor3f(200, 200, 200);
	for (int i = -1000; i <= 1000; i += 10) {
		glVertex3f(i, -2.0f, 1000.0f); glVertex3f(i, -2.0f, -1000.0f);
		glVertex3f(1000.0f, -2.0f, i); glVertex3f(-1000.0f, -2.0f, i);
	}
	glEnd();
}

void DrawCar(void)
{
	glTranslatef(CarPosX, 0.0, CarPosZ);
	//The turret
	glColor3ub(0, 0, 90);
	glPushMatrix();
	glScalef(0.2, 0.2, 1.2);
	glTranslatef(0.0, 20.0, -2.0);
	glutSolidCube(CarBodySize);
	glPopMatrix();
	//	glPopMatrix();

	glRotatef(CarBodyAngle, 0.0, 1.0, 0.0);
	// tank system rotated
	//Tank body
	glPushMatrix();
	glColor3ub(0, 150, 0);
	glScalef(1.0, 0.5, 1.0);
	glutSolidCube(CarBodySize);
	glPopMatrix();

	DrawAxis(15.0f);

	//Right wheel(1)
	glPushMatrix();
	glColor3ub(150, 100, 0);
	glTranslatef(CarBodySize*0.5 + CarWheelSize*0.5, 0.0, -CarBodySize*0.5 + CarWheelSize*0.25);
	glRotatef(-CarWheelAngleR, 1.0, 0.0, 0.0);	//wheel rotate (How???)
	glPushMatrix();
	glScalef(0.5, 1.0, 1.0);
	glutSolidSphere(CarWheelSize, 15.0, 15.0);
	glPopMatrix();
	glTranslatef(0, CarWheelSize, 0);
	glColor3ub(0, 100, 0);	//Little Green dot
	glutSolidSphere(CarWheelSize*0.1, 5.0, 5.0);
	glPopMatrix();
	//glPopMatrix();

	//left wheel(1)
	glPushMatrix();
	glColor3ub(150, 100, 0);
	glTranslatef(-CarBodySize*0.5 - CarWheelSize*0.5, 0.0, -CarBodySize*0.5 + CarWheelSize*0.25);
	glRotatef(-CarWheelAngleL, 1.0, 0.0, 0.0);	//wheel rotate (How???)
	glPushMatrix();
	glScalef(0.5, 1.0, 1.0);
	glutSolidSphere(CarWheelSize, 15.0, 15.0);
	glPopMatrix();
	glTranslatef(0, CarWheelSize, 0);
	glColor3ub(0, 100, 0);	//Little Green dot
	glutSolidSphere(CarWheelSize*0.1, 5.0, 5.0);
	glPopMatrix();
	glPopMatrix();

	//Left wheel(2)
	glPushMatrix();
	glColor3ub(150, 100, 0);
	glTranslatef(-CarBodySize*0.5 - CarWheelSize*0.5, 0.0, CarBodySize*0.5 - CarWheelSize*0.25);
	glRotatef(-CarWheelAngleL, 1.0, 0.0, 0.0);	//wheel rotate (How???)
	glPushMatrix();
	glScalef(0.5, 1.0, 1.0);
	glutSolidSphere(CarWheelSize, 15.0, 15.0);
	glPopMatrix();
	glTranslatef(0, CarWheelSize, 0);
	glColor3ub(0, 100, 0);	//Little Green dot
	glutSolidSphere(CarWheelSize*0.1, 5.0, 5.0);
	glPopMatrix();

	//Right wheel(2)
	glPushMatrix();
	glColor3ub(150, 100, 0);
	glTranslatef(CarBodySize*0.5 + CarWheelSize*0.5, 0.0, +CarBodySize*0.5 - CarWheelSize*0.25);
	glRotatef(-CarWheelAngleR, 1.0, 0.0, 0.0);	
	glPushMatrix();
	glScalef(0.5, 1.0, 1.0);
	glutSolidSphere(CarWheelSize, 15.0, 15.0);
	glPopMatrix();
	glTranslatef(0, CarWheelSize, 0);
	glColor3ub(0, 100, 0);	//Little Green dot
	glutSolidSphere(CarWheelSize*0.1, 5.0, 5.0);
	glPopMatrix();
	glPopMatrix();

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0 + CarPosX, CameraDist, CameraDist + CarPosZ, CarPosX, 0.0, CarPosZ, 0.0, 1.0, 0.0);


	DrawGrids();
	DrawCar();

	glFlush();
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
	switch (key) {
	case 'A': case 'a':
		if (CarBodyAngle == 360.0) CarBodyAngle = 0.0;
		else  CarBodyAngle += CarTurnSpeed;
		if (CarWheelAngleR == 360)	CarWheelAngleR = 0.0;
		else	CarWheelAngleR += WheelRotSpeed;
		if (CarWheelAngleL == 360)	CarWheelAngleL = 0.0;
		else	CarWheelAngleL += WheelRotSpeed;
		break;
	case 'S': case 's':
		CarPosX += 1 * sin(CarBodyAngle / 180.0 * PI);
		CarPosZ += 1 * cos(CarBodyAngle / 180.0 * PI);
		if (CarWheelAngleR == -360)	CarWheelAngleR = 0.0;
		else	CarWheelAngleR -= WheelRotSpeed;
		if (CarWheelAngleL == -360)	CarWheelAngleL = 0.0;
		else	CarWheelAngleL -= WheelRotSpeed;
		break;

	case 'W': case 'w':
		CarPosX -= 1 * sin(CarBodyAngle / 180.0 * PI);
		CarPosZ -= 1 * cos(CarBodyAngle / 180.0 * PI);
		if (CarWheelAngleL < 360)
			CarWheelAngleL += WheelRotSpeed;
		else
			CarWheelAngleL = 0;
		if (CarWheelAngleR < 360)
			CarWheelAngleR += WheelRotSpeed;
		else
			CarWheelAngleR = 0;
		break;
	case 'D': case 'd':
		if (CarBodyAngle == 360.0) CarBodyAngle = 0.0;
		else  CarBodyAngle -= CarTurnSpeed;
		if (CarWheelAngleL > 0)
			CarWheelAngleL -= WheelRotSpeed;
		else
			CarWheelAngleL = 360;
		if (CarWheelAngleR > 0)
			CarWheelAngleR -= WheelRotSpeed;
		else
			CarWheelAngleR = 360;
		break;

	case 'E': case 'e':
		if (CarBodyAngle == 0.0) {
			CarBodyAngle = 360.0;
			CarPosX -= 1 * sin(CarBodyAngle / 180.0 * PI);
			CarPosZ -= 1 * cos(CarBodyAngle / 180.0 * PI);
		}
		else {
			CarBodyAngle -= CarTurnSpeed;
			CarPosX -= 1 * sin(CarBodyAngle / 180.0 * PI);
			CarPosZ -= 1 * cos(CarBodyAngle / 180.0 * PI);
		}

		if (CarWheelAngleR == 360)	CarWheelAngleR = 0.0;
		else	CarWheelAngleR += 5.0;
		if (CarWheelAngleL == 360)	CarWheelAngleL = 0.0;
		else	CarWheelAngleL += 10.0;

		break;
	case 'Q': case 'q':
		if (CarBodyAngle == 360.0) {
			CarBodyAngle = 0.0;
			CarPosX -= 1 * sin(CarBodyAngle / 180.0 * PI);
			CarPosZ -= 1 * cos(CarBodyAngle / 180.0 * PI);
		}
		else {
			CarBodyAngle += CarTurnSpeed;
			CarPosX -= 1 * sin(CarBodyAngle / 180.0 * PI);
			CarPosZ -= 1 * cos(CarBodyAngle / 180.0 * PI);
		}

		if (CarWheelAngleR <= 0.0)	CarWheelAngleR = 360.0;
		else	CarWheelAngleR += 10.0;
		if (CarWheelAngleL <= 0.0)	CarWheelAngleL = 360.0;
		else	CarWheelAngleL += 5.0;

		break;
	case 'F': case 'f':
		if (CameraDist < 200)
			CameraDist += 10;
		break;

	case 'R': case 'r':
		if (CameraDist > 10)
			CameraDist -= 10;
		break;

	default:
		return;
	}
	glutPostRedisplay();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 1.0f, 750.0f);

	glMatrixMode(GL_MODELVIEW);
}

void initLight(void)
{
	GLfloat  ambientLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat  diffuseLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat  specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat  Lposition[] = { 10.0f, 10.0f, 10.0f, 0.0f }; 

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, Lposition);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0.5, 0.5, 0.5, 1.0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Running Tank");
	initLight();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);

	glutMainLoop();
}