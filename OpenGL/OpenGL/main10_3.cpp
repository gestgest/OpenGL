#include <GL/glut.h>


//조명
void InitLight() {

	//Light
	GLfloat MyLightPosition[] = { 1.0, 0.0, 0.0, 1.0 }; // 10, 0, 0, 1 [위치성 광원]
	GLfloat MyLightDirection[] = { 3.0, 4.0, 3.0, 0.0 }; // 30, 40, 30, 0 [방향성 광원, 벡터] : 1이면 직선
	GLfloat MySpotDirection[] = { -1.0, 0.0, 0.0, 0.0 }; //	[스팟]벡터
	GLfloat MySpotAngle = 10; // 10


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	int mode = 1;

	switch (mode) {
	case 1:	// 방향성 광원 [태양]
		//GL_LIGHT0 ~ 7까지 있음
		//조명1~7
		glLightfv(GL_LIGHT0, GL_POSITION, MyLightDirection); 
		break;

	case 2: // 위치성 광원 [조명]
		glLightfv(GL_LIGHT0, GL_POSITION, MyLightPosition);
		break;

	case 3: // 스폿 광원
		glLightfv(GL_LIGHT0, GL_POSITION, MyLightPosition);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, MySpotDirection);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, MySpotAngle);
		break;
	}
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);    //근접시점
}
void InitMaterial() {
	GLfloat mAmbient[] = { 0.3, 0.3, 0.3, 1 };
	GLfloat mDiffuse[] = { 0.8, 0.8, 0.8, 1 };
	GLfloat mSpecular[] = { 0.1, 0.1, 0.1, 1 };
	GLfloat power = 25;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, power);
	glEnable(GL_NORMALIZE);
}
void MyDisplay() {                      //디스플레이 콜백함수
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       //GL 상태변수 설정
	glShadeModel(GL_SMOOTH); // GL_FLAT, 마지막 정점의 색으로 칠함 [단색으로 칠해줘]
	glColor3f(1, 0, 0);
	//glutSolidTeapot(0.5);
	glutSolidSphere(0.5, 200, 200);
	// 방향성 광원, MyLightDirection 8,4,1
	//glBegin(GL_TRIANGLES);
	//	glNormal3f(0.0, 0.0, 1.0);
	//	glVertex3f(0.0, 0.0, 1.0);
	//
	//	glNormal3f(0.0, 1.0, 0.0);
	//	glVertex3f(0.0, 1.0, 0.0);
	//
	//	glNormal3f(1.0, 0.0, 0.0);
	//	glVertex3f(1.0, 0.0, 0.0);
	//glEnd();

	glFlush();
}

int main(int argc, char** argv) 
{
	glutInit(&argc, argv);               //GLUT 윈도우 함수
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("OpenGL Sample Drawing");

	glEnable(GL_DEPTH_TEST); //앞면 + 뒷면 

	glClearColor(1.0, 1.0, 1.0, 1.0);   //GL 상태변수 설정
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//view
	glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2, 2, 2, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);    //시점변환

	InitLight();
	InitMaterial(); //재질 초기화
	glutDisplayFunc(MyDisplay);         //GLUT 콜백함수 등록
	glutMainLoop();                     //이벤트 루프 진입
	return 0;
}