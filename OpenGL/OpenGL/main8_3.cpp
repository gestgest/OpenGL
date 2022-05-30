#include <GL/glut.h>
// 조명은 10장에서

void InitLight() {
	GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_position[] = { -3, 2, 3.0, 0.0 };

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}


//TMI, ccw : 반시계[표준] ,cw : 시계방향 
//back face culling : 뒤 모니터 짤라내기 
//시계방향으로 그리는 도형을 보고 알수있음 [a,b벡터 내적보고 알 수 있음]
void InitVisibility()
{
	//glEnable(GL_CULL_FACE);	// 1. culling으로 뒷면이 사라짐 [함부로 culling을 하지마라]
	glFrontFace(GL_CW);			// 2. 주전자 데이터는 CW로 만들어짐. 표준은 CCW [부등호 판정만 바꾼거]
	glCullFace(GL_BACK);		// 3. GL_FRONT로 변경 [CCW, Back이 표준]
	glPolygonMode(GL_FRONT, GL_FILL);	// 4. 앞은 채워진 모드 설정
	glPolygonMode(GL_BACK, GL_LINE);		// 4. 뒤는 선 모드 설정
	glEnable(GL_DEPTH_TEST);	// 5. 모든 자료를 초기화한 후, depth test를 제거. 그리는 순서대로 누적됨 [딥버퍼 기능 활성화]
	//Depth 버퍼, 없으면 뒤에있는 그림도 그려짐 [무조건 넣는 느낌]
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //depth 버퍼 초기화 [2]
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.4, 0.5, 0.0, -0.5, -1.0, 0.0, 1.0, 0.0);
	glutSolidTeapot(0.55f);
	glFlush();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 2.0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); //depth 버퍼 준비 [1]
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Sample Drawing");
	glClearColor(0.5, 0.5, 0.5, 0.0);
	InitLight();
	InitVisibility();
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMainLoop();
	return 0;
}