#include <GL/glut.h>
float r = 0;

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 600, 600);
	glColor3f(1.0, 0.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	// model 행렬  반영
	glLoadIdentity();
	//현재 행렬을 단위 행렬로 초기화 : 1 0 0 0, 대각선에 1 있는거
	//왼쪽 아래를 기준으로 돌리고 싶다면
	glTranslatef(-0.15, -0.15, -0.15);
	glRotatef(r, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.15, 0.15);
	glutSolidCube(0.3);
	// 가운데 점이 0, 0
	glFlush();
}
void MyIdle() {
	r+= 0.01f;
	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glutDisplayFunc(MyDisplay);
	glutIdleFunc(MyIdle);
	glutMainLoop();
	return 0;
}