#include <GL/glut.h>
static int Day = 0, Time = 0;
float t = 0;

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // 단위행렬
	glColor3f(1.0, 0.3, 0.3);

	glutWireSphere(0.15, 20, 16); // 태양

	//행렬 스택1
	glPushMatrix();
		//지구공전
		//glRotatef((GLfloat)Day, 0.0, 1.0, 0.0);
		glRotatef(t/10, 0.0, 1.0, 0.0);
		glTranslatef(0.6, 0.0, 0.0);
	
		//지구 자전
		//glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
		glRotatef(t, 0.0, 1.0, 0.0);

		glColor3f(0.5, 0.6, 0.7);
		glutWireSphere(0.08, 10, 8); //지구
		//행렬 스택2
		glPushMatrix();
			//달 공전
			//glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
			glRotatef(t, 0.0, 1.0, 0.0);
			glTranslatef(0.15, 0.0, 0.0);
			glColor3f(0.9, 0.8, 0.2);
			glutWireSphere(0.04, 10, 8);
		glPopMatrix(); //상단의 행렬이 사라짐[지구로 돌아옴]
		glPushMatrix();
			//달 공전
			//glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
			glRotatef(t, 0.0, 1.0, 0.0);
			glTranslatef(-0.15, 0.0, 0.0);
			glColor3f(0, 1, 0);
			glutWireSphere(0.04, 10, 8);
		glPopMatrix(); //상단의 행렬이 사라짐[지구로 돌아옴]
	glPopMatrix(); //[태양으로 돌아옴]
	glPushMatrix();

		glRotatef(t / 10, 0.0, 1.0, 0.0);
		glTranslatef(-0.5, 0.0, 0.0);

		//금성 자전
		//glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
		glRotatef(t, 0.0, 1.0, 0.0);

		glColor3f(1, 1, 0);
		glutWireSphere(0.08, 10, 8); //금성
	glPopMatrix();
	glutSwapBuffers();
}

void MyKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'd':
		Day = (Day + 10) % 360;
		glutPostRedisplay();
		break;
	case 't':
		Time = (Time + 5) % 360;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void MyIdleFunc() {
	t += 0.2;
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);


	glutDisplayFunc(MyDisplay);
	glutIdleFunc(MyIdleFunc);
	//glutKeyboardFunc(MyKeyboard);
	
	glutMainLoop();
	return 0;
}