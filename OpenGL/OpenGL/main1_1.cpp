//c++
#include <gl/glut.h>

void MyDisplay() {
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

//무조건 파라미터는 int 2개가 있어야 한다.
void MyReShape(int width, int height)
{
	glViewport(0, 0, width, height);
	GLfloat WidthFactor = (GLfloat)width / (GLfloat)300;
	GLfloat HeightFactor = (GLfloat)height / (GLfloat)300;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0 * WidthFactor, 1.0 * WidthFactor,
		-1.0 * HeightFactor, 1.0 * HeightFactor, -1.0, 1.0);
}

int main(int a, char** pp) { 
	glutInit(&a, pp);     // 초기화
	glutInitDisplayMode(GLUT_RGB); //디스플레이 출력 모드 초기화
	glutInitWindowSize(600, 400); //윈도우 사이즈
	glutInitWindowPosition(300, 50);
	glutCreateWindow("OpenGL Drawing Example"); //~제목인 윈도우창 만들기

	glClearColor(1.0, 1.0, 0.0, 1.0);

	//callback, callback(함수) => 함수 실행이 아니라 opengl이 이 함수를 기억하는 거다.
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReShape);
	//마치 자바의 repaint느낌이랑 비슷하다.

	glutMainLoop(); //그리고 그 do while구조로 이루어진 디바이스 함수를 실행하고.
	//그 이벤트 큐에 일단 Display함수를 실행해야하니 callback된 MyDisplay를 실행한다.
	return 0;
}
