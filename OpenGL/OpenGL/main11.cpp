#include <GL/glut.h>
#include <cmath>
#include <vector>
int MX = 300, MY = 300;
int hp;
float time = 0;
float x = 0, y = 0; // 눈사람 좌표 전역변수

class Vec3 {
public:
	float m[3];
	Vec3(float x = 0, float y = 0, float z = 0) {
		m[0] = x;
		m[1] = y;
		m[2] = z;
	}

	Vec3 operator+ (Vec3 v) {
		Vec3 ret;
		for (int i = 0; i < 3; i++)
			ret.m[i] = m[i] + v.m[i];

		return ret;
	}

	Vec3 operator- (Vec3 v) {
		Vec3 ret;
		for(int i = 0; i < 3; i++)
			ret.m[i] = m[i] - v.m[i];

		return ret;
	}
	Vec3 operator* (float ppap) {
		Vec3 ret;
		for (int i = 0; i < 3; i++)
			ret.m[i] = m[i] * ppap;

		return ret;
	}


	float length() {
		float len = 0;
		for (int i = 0; i < 3; i++)
			len += m[i] * m[i];
		return sqrtf(len);
	}
};

class SnowBall {
public:
	//속도
	Vec3 v;
	Vec3 pos;
};

std::vector<SnowBall> snowballs;

bool HitCheck() { // 공과 big snow의 충돌이면 true리턴
	Vec3 big_snow(sin(time * 0.0003) * 8, 1.0, -8);
	int size = snowballs.size();
	for (int i = 0; i < size; i++)
	{
		Vec3 diff = big_snow - snowballs[i].pos;
		if (diff.length() < 1.5)
		{
			return true;
		}
	}
	return false;
}


void UpdateSnowballs()
{
	Vec3 earthG(0, -0.98, 0);
	float snowTime = 0.001;

	int size = snowballs.size();
	for (int i = 0; i < size; i++) {
		snowballs[i].v = snowballs[i].v + earthG * snowTime;
		snowballs[i].pos = snowballs[i].pos + snowballs[i].v * 0.005;
		
		//바닥에 팅기는
			// 공의 반지름 + 바닥판의 위치
		if (snowballs[i].pos.m[1] < 0.5 + 0.25)
		{
			snowballs[i].v.m[1] = fabs(snowballs[i].v.m[1]);
			snowballs[i].pos.m[1] = 0.75;
			//snowballs[i].v = snowballs[i].v * 0.4;
			// ㄴ 이거 넣으면 마지막에 멈춤
		}

	}

	//빠져 나가면 제거
	for (int i = 0; i < snowballs.size(); )
	{
		if (snowballs[i].pos.m[2] > 10 || snowballs[i].pos.m[2] < -10)
			snowballs.erase(snowballs.begin() + i);//이렇게 제거하는구나
		//else if (snowballs[i].v.length() < 0.0001)
			//snowballs.erase(snowballs.begin() + i); 
		else
			i++;
	}


}

void DrawSnowballs()
{
	UpdateSnowballs(); // 눈덩이 좌표의 update
	int size = snowballs.size();
	for (int i = 0; i < size; i++) {
		glPushMatrix();
		glTranslatef(snowballs[i].pos.m[0], snowballs[i].pos.m[1], snowballs[i].pos.m[2]);
		glutSolidSphere(0.5, 20, 20);
		glPopMatrix();
	}
}


//조명
void InitLight() {
	//조명 속성 배열
	GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 15.0 };
	GLfloat light_specular[] = { 1.0, 0.8, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_position[] = { -3, 6, 3.0, 0.0 };

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
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
	glEnable(GL_NORMALIZE); //조명 계산할때 법선 벡터를 정확히 다시 계산해라
}

void drawSnowman() {

	glPushMatrix();
	glColor3f(0, 0, 0);

	//몸통
	glTranslatef(0, 3, 0); //바닥위치
	glutSolidSphere(3, 20, 20);

	//팔
	glPushMatrix();
		glTranslatef(2.5, 0, 0);
		//3d축은 ppt에 나와있는 y가 위 아래 축인 경우다.
		// [2 2 2 큐브하기전 뷰 축 기준]
		glRotatef(sin(time * 0.0005) * 30 - 30, 0, 0, 1);
		glTranslatef(0, 2.5, 0);
		glPushMatrix();
			glScalef(0.1, 1, 0.1);
			glutSolidCube(5);
		glPopMatrix();

		//팔목
		glTranslatef(0, 2.5, 0);
		glPushMatrix();
			glRotatef(sin(time * 0.0005) * 15 + 15, 0, 0, 1);
			glTranslatef(0, 1.5, 0);
			glScalef(0.1, 1, 0.1);
			glutSolidCube(3);
		glPopMatrix();

	glPopMatrix();
	


	//y축으로 하면 자전하는 느낌임
	glRotatef(sin(time * 0.0005) * 15, 1, 0, 0);
	glTranslatef(0, 6, 0);
	glutSolidSphere(3, 20, 20); //머리
	glPopMatrix();
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//판떼기
	glPushMatrix();
		glColor3f(1, 1, 0);
		glScalef(20, 0.5, 20);
		glutSolidCube(1.0);
	glPopMatrix();

	//눈사람1, 내 눈사람
	glPushMatrix();
		glTranslatef(0, 0, 8);
		glTranslatef(x, 0, y);
		glScalef(0.2, 0.2, 0.2);
		drawSnowman();
	glPopMatrix();
	
	//눈사람2, AI 눈사람
	glPushMatrix();
		glTranslatef(0, 0, -8); //스타팅 위치
		glTranslatef(sin(time * 0.0003) * 8, 0, 0);
		glScalef(0.3, 0.3, 0.3);
		drawSnowman();
	glPopMatrix();

	DrawSnowballs();

	bool hit = HitCheck();
	//아직 10번의 목숨 구현 안함
	if (hit)
	{
		hp--;
		if(hp <= 0)
			exit(0);
	}
	glFlush();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//원근법이 없는 일반투영
	glOrtho(-15.0, 15.0, -15.0, 15.0, -100.0, 100.0); // 15가 좋은 view
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(MX / 300.0, MY / 300.0, 1.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);  //시점변환
}

//마우스 드래그
void MyMouseMove(GLint X, GLint Y)
{
	MX = X;
	MY = Y;	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(MX / 300.0, MY / 300.0, 1.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);  //시점변환

	glutPostRedisplay();
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
	if (KeyPressed == ' ') { //spacebar 
		SnowBall sb;
		sb.pos.m[0] = x + 1.2; // 눈사람의 좌표 x, y
		sb.pos.m[1] = 2.1;
		sb.pos.m[2] = y + 7;
		sb.v.m[0] = 0;
		sb.v.m[1] = 0;
		sb.v.m[2] = -1.5;
		snowballs.push_back(sb);
		glutPostRedisplay();
	}
}


void MySpecial(int key, int X, int Y) { //x,y는 눈사람의 현재 위치
	switch (key) {
	case  GLUT_KEY_UP:
		y -= 0.5f;
		glutPostRedisplay();
		break;
	case  GLUT_KEY_DOWN:
		y += 0.5f;
		glutPostRedisplay();
		break;
	case  GLUT_KEY_LEFT:
		x -= 0.5f;
		glutPostRedisplay();
		break;
	case  GLUT_KEY_RIGHT:
		x += 0.5f;
		glutPostRedisplay();
		break;
	}
}

void MyIdle() {
	time += 3;
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);               //GLUT 윈도우 함수
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("OpenGL Drawing Example");
	glClearColor(0.4, 0.4, 0.4, 1);

	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutIdleFunc(MyIdle);
	glutKeyboardFunc(MyKeyboard);
	glutSpecialFunc(MySpecial);
	glutMotionFunc(MyMouseMove);

	hp = 10;
	InitLight();
	glutMainLoop();
	return 0;
}