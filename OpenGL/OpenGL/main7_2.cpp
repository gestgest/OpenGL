#include <GL/glut.h>
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);

	//청색 주전자
	glColor3f(0.3, 0.3, 0.7);
	glutWireTeapot(1.0);

	glFlush();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// 종횡비(세로에 대한 가로의 비율. 4:3 이라면 1.333 입력)
	gluPerspective(15, (GLdouble)w / (GLdouble)h, 1.0, 50.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//카메라는 5, 5, 5 | 바라보는 방향은 0 0 0
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	//싱글버퍼
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");
	glClearColor(1.0, 1.0, 1.0, 1.0); //흰색
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMainLoop();
	return 0;
}