//일단 메인
#include <gl/glut.h>

void MyDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 600, 400);
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	glVertex3f(0.5, -0.5, 0.0);
	glVertex3f(-0.5, 0.5, 0.0);
	glEnd();
	glFlush();
}

void MyMouseClick(GLint button, GLint state, int x, int y)
{

}

int main(int a, char **pp)
{
	glutInit(&a, pp);     // 초기화
	glutInitDisplayMode(GLUT_RGB); //디스플레이 출력 모드 초기화
	glutInitWindowSize(900, 600); //윈도우 사이즈
	glutInitWindowPosition(300, 50);
	glutCreateWindow("Football Manager"); //~제목인 윈도우창 만들기

	//클리어 색 [배경색]
	glClearColor(1.0, 1.0, 0.0, 1.0);

	glutDisplayFunc(MyDisplay);
	glutMouseFunc(MyMouseClick);

	glutMainLoop(); 
	return 0;
}