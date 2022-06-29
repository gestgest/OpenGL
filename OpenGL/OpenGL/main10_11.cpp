#include <GL/glut.h>
#include <math.h>
float angle = 0;
void MyIdle()
{
	angle += 1.0f;
	glutPostRedisplay();
}

//K : 물체 1 0 1
//I : 빛 1 1 0
//IK : 1 0 0 : 빨간색

void MyLightInit() {
	GLfloat global_ambient[] = { 0.3, 0.3, 0.3, 1.0 };     //전역 주변반사

	GLfloat light0_ambient[] = { 1,1.0, 0.0,0 };     //0번 광원 특성 [조명]
	GLfloat light0_diffuse[] = { 1, 1.0, 0,0 };
	GLfloat light0_specular[] = { 1, 1,0, 0,0 };

	GLfloat light1_ambient[] = { 0.2,0.2,0.2, 1.0 };     //1번 광원 특성 [스포트라이트]
	GLfloat light1_diffuse[] = { 0.6,0.6,0.6, 1.0 };
	GLfloat light1_specular[] = { 1.0,1.0,1.0, 1.0 };

	GLfloat material_ambient[] = { 1.0, 0.0, 1.0, 1.0 };  //물체 특성 [물체 색]
	GLfloat material_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat material_shininess[] = { 25.0 };

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);    //깊이 버퍼 활성화
	glEnable(GL_LIGHTING);      //조명 활성화

	glEnable(GL_LIGHT0);        //0번 광원 활성화
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);   //0번 광원 특성할당
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	glEnable(GL_LIGHT1);        //1번 광원 활성화
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);   //1번 광원 특성할당
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);   //물체 특성할당
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); //전역주변반사
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);    //근접시점
}

void MyDisplay()
{
	GLfloat LightPosition0[] = { 0.0, 0.0, 2.0, 1.0 };     //0번 광원위치
	GLfloat LightPosition1[] = { 1.0, 1.0, 1.0, 1.0 };     //1번 광원위치
	GLfloat LightDirection1[] = { -0.5, -1.0, -1.0 };       //1번 광원 방향
	GLfloat SpotAngle1[] = { 20.0 };                        //1번 광원 각도
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);                             //모델뷰 행렬
	glLoadIdentity();                                       //항등행렬 로드

	// 조명을 카메라 좌표에 고정/ 머리위에 조명


	gluLookAt(sin(angle * 0.01), 1.5, 1.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);    //시점변환

	// 조명을 world 좌표에 고정
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition0);      //위치성 광원
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition1);      //스포트라이트
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, LightDirection1);   //방향
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, SpotAngle1);       //각도

	glPushMatrix();
	glRotatef(angle, 0, 1, 0);

	// 조명을 한 물체 좌표에 고정

	glutSolidTorus(0.3, 0.6, 100, 100);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.6, -0.5, 0.6);
	glutSolidTeapot(0.2);
	glTranslatef(-0.6, -0.4, -0.6);
	glScalef(2, 0.1, 2);
	glutSolidCube(0.8);
	glPopMatrix();
	glutSwapBuffers();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);                //투상행렬
	glLoadIdentity();                           //항등행렬 로드
	glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);   //평행투상
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //
	glutCreateWindow("openGL Sample Drawing");
	MyLightInit();
	glutIdleFunc(MyIdle);
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMainLoop();
	return 0;
}