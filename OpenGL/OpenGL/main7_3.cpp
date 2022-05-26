#include <GL/glut.h>
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.3, 0.3, 0.7);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 1, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(-1, 0, 0);
	glEnd();
	glFlush();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//자신의 눈 기준, 거리 5에 삼각형이 있다.
	//그냥 near는 최대한 작게[0보다는 큰], far는 최대한 크게
	//0은 왜 안되는가 : z를 나눌때 오류가 생길 수 있음
	gluPerspective(30, (GLdouble)w / (GLdouble)h, 0.0, 5.0);
	//glOrtho(-1, 1, -1, 1, 1.0, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//거리가 5인 카메라
	gluLookAt(0.0, 0.0, 5.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMainLoop();
	return 0;
}