//롤 FM 게임

#include <GL/glut.h>
//해상도에서 4/5를 해야 현 해상도가 나온다.
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define GAME_WIDTH 800
#define GAME_HEIGHT 600

//대충 능력치
//1~5

void MyDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	glEnd();
	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);               //GLUT 윈도우 함수
	glutInitWindowSize(GAME_WIDTH, GAME_HEIGHT);
	glutInitWindowPosition((WINDOW_WIDTH * 4 / 5 - GAME_WIDTH) / 2, (WINDOW_HEIGHT * 4 / 5 - GAME_HEIGHT) / 2);
	glutCreateWindow("OpenGL Drawing Example");

	glutDisplayFunc(MyDisplay);
	glutMainLoop();
	return 0;
}