#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <cstdlib>


#define LEG_SIZE 1.0
#define THIGH_SIZE 1.0
#define BODY_SIZE 2.0

float debug_x = 1, debug_y = 1;
float mytime = 0;

class Vec3 {
public:
	float m[3];
	Vec3(float x = 0, float y = 0, float z = 0) {
		m[0] = x;
		m[1] = y;
		m[2] = z;
	}
	void setPos(float x, float y, float z) {
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
		for (int i = 0; i < 3; i++)
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



//플레이어
//현재 좌표 + HP
class Player{
public:
	Vec3 pos;
	int hp;
	float time; //던지는 볼 상태
};

Player players[4];


//조명
void InitLight(int n)
{
	//조명 속성 배열
	GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 15.0 };
	GLfloat light_specular[] = { 1.0, 0.8, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_position[] = { -3, 6, 3.0, 0.0 };

	GLfloat mat_diffuse_human[] = { 255.0, 195.0 / 255, 150.0 / 255, 1.0 };
	GLfloat mat_diffuse_bus[] = { 0.0, 0.0, 0.8, 1.0 };
	GLfloat mat_diffuse_bus1[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse_hpbar[] = { 0.0, 0.9, 0.0, 1.0 };
	GLfloat mat_diffuse_snow[] = { 0.9, 0.9, 0.9, 1.0 };

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


	switch (n) {
	case 1:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_human);
		break;
	case 2: //버스
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_bus);
		break;
	case 3: //바퀴
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_bus1);
		break;
	case 4: //눈
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_snow);
		break;
	case 5: //hp바
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_hpbar);
		break;
	}
}

void UpdateSnowballs()
{
	Vec3 earthG(0, -0.98, 0);
	float snowTime = 0.1;

	int size = snowballs.size();
	for (int i = 0; i < size; i++) {
		snowballs[i].v = snowballs[i].v + earthG * snowTime;
		snowballs[i].pos = snowballs[i].pos + snowballs[i].v * 0.2;

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
		if (snowballs[i].pos.m[2] > 15 || snowballs[i].pos.m[2] < -15)
			snowballs.erase(snowballs.begin() + i);//이렇게 제거하는구나
		//else if (snowballs[i].v.length() < 0.0001)
			//snowballs.erase(snowballs.begin() + i); 
		else
			i++;
	}

	//맞는다면?
	for (int i = 0; i < 4; i++)
	{
		if (0 < players[i].hp)
			for (int j = 0; j < snowballs.size();)
			{
				Vec3 vec = players[i].pos - snowballs[j].pos;
				if (vec.length() < 2.8)
				{
					players[i].hp--;
					snowballs.erase(snowballs.begin() + j);
				}
				else
					j++;
			}
	}
	for (int j = 0; j < snowballs.size();)
	{
		Vec3 myvec;
		myvec.setPos(cos(mytime * 0.5) * -12, 3.05, 0);
		Vec3 vec = myvec - snowballs[j].pos;
		if (vec.length() <= 3)
			snowballs.erase(snowballs.begin() + j);
		else
			j++;
	}
}

void DrawSnowballs()
{
	UpdateSnowballs(); // 눈덩이 좌표의 update
	int size = snowballs.size();
	for (int i = 0; i < size; i++)
	{
		glPushMatrix();
		InitLight(4);
		glTranslatef(snowballs[i].pos.m[0], snowballs[i].pos.m[1], snowballs[i].pos.m[2]);
		glutSolidSphere(0.3, 20, 20);
		glPopMatrix();
	}

}



void drawBus()
{
	glPushMatrix();
	InitLight(2);
	//y는 버스높이/2
	glTranslatef(cos(mytime * 0.5) * -12, 3.05, 0);

	//버스 몸통
	glPushMatrix();
		glScalef(1, 0.6, 0.6);
		glutSolidCube(6);
	glPopMatrix();


	InitLight(3);
	glPushMatrix();
	glTranslatef(-1.5, -2, 2.3);
	glRotatef(cos(mytime * 0.5) * 100, 0, 0, 1);
	glutSolidTorus(0.2, 0.7, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -2, -2.3);
	glRotatef(cos(mytime * 0.5) * 100, 0, 0, 1);
	glutSolidTorus(0.2, 0.7, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -2, 2.3);
	glRotatef(cos(mytime * 0.5) * 100, 0, 0, 1);
	glutSolidTorus(0.2, 0.7, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -2, -2.3);
	glRotatef(cos(mytime * 0.5) * 100, 0, 0, 1);
	glutSolidTorus(0.2, 0.7, 20, 20);
	glPopMatrix();

	glPopMatrix();
}

//사람 그리는거
void drawPlayer(int i)
{
	
	glPushMatrix(); //몸 [0,3,0]
	glTranslatef(0, (float)(LEG_SIZE + THIGH_SIZE + (float)BODY_SIZE / 2), 0);

	InitLight(2);
	//몸통 [size 1,2]
	glPushMatrix();
		glScalef(0.6, 1, 0.6);
		glutSolidCube(BODY_SIZE);
	glPopMatrix(); //몸통 끝
	InitLight(1);

	//머리
	glPushMatrix();

	glTranslatef(0, 2, 0);
	//진짜 생 머리
	glPushMatrix();
		glutSolidSphere(1, 20, 20);
	glPopMatrix(); //진짜 생 머리

	glPushMatrix();//hp
		InitLight(5);
		glTranslatef((float)players[i].hp * 3 / 20 - 1.5, 1.5, 0);
		glScalef((float)players[i].hp / 10, 0.2, 0.2);
		glutSolidCube(3);
	glPopMatrix();
	InitLight(1);

	glPopMatrix(); //머리
	
	//-x 왼 팔
	glPushMatrix();

		glTranslatef(-0.8, 0, 0);
		//-x 왼쪽 진짜팔
		glPushMatrix();

		glScalef(0.4, 1, 0.4);
		glutSolidCube(THIGH_SIZE);
		glPopMatrix(); //진짜팔

		glPushMatrix();
		glTranslatef(0, -1, 0);
		glScalef(0.2, 1, 0.2);
		glutSolidCube(LEG_SIZE);
		glPopMatrix();

	glPopMatrix(); //왼쪽 팔

	//x 오른쪽팔
	glPushMatrix();
		glTranslatef(0.8, 0, 0);
		float deltatime = mytime - players[i].time;
		if(deltatime <= 0.5 && players[i].time != 0.0)
			glRotatef(cos(deltatime * 10) * 90 + 180, 1, 0, 0) ;

		glPushMatrix();
		glScalef(0.4, 1, 0.4);
		glutSolidCube(THIGH_SIZE);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, -1, 0);
		glScalef(0.2, 1, 0.2);
		if (deltatime <= 0.5 && players[i].time != 0.0)
			glRotatef(cos(deltatime * 30) * 45 + 180, 1, 0, 0);
		glutSolidCube(LEG_SIZE);
		glPopMatrix();

	glPopMatrix(); //오른쪽팔
	

	//허벅지 + 다리
	glPushMatrix();
		glTranslatef(0, -(float)BODY_SIZE / 2 - (float)THIGH_SIZE / 2, 0);
		//여기에 다리 운동 관련 함수 넣으면 됨
		glPushMatrix(); //허벅지
			glScalef(0.4, 1, 0.4);

		glPushMatrix();
			glTranslatef(-0.3 / 0.4 ,0, 0);
			glutSolidCube(THIGH_SIZE);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.3 / 0.4, 0, 0);
			glutSolidCube(THIGH_SIZE);
		glPopMatrix();

		glPopMatrix(); //허벅지

		//다리
		glPushMatrix();
			glTranslatef(0, - (float)(THIGH_SIZE + LEG_SIZE) / 2, 0);

			glPushMatrix();
				glTranslatef(-0.3, 0, 0);
				glScalef(0.2, 1, 0.2);
				glutSolidCube(LEG_SIZE);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(0.3, 0, 0);
				glScalef(0.2, 1, 0.2);
				glutSolidCube(LEG_SIZE);
			glPopMatrix();

		glPopMatrix();//다리

	glPopMatrix();//허벅지 + 다리

	glPopMatrix(); //몸
}

//물체를 적는다.
void MyDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//판떼기
	glPushMatrix();
		InitLight(0);
		glColor3f(1, 1, 0);
		glTranslatef(0, -0.5, 0); //이래야 y값이 0이 땅좌표가 된다.
		glScalef(30, 1, 30);
		glutSolidCube(1.0);
	glPopMatrix();

	for (int i = 0; i < 4; i++)
	{
		if (0 < players[i].hp)
		{
			glPushMatrix();
			glTranslatef(players[i].pos.m[0], players[i].pos.m[1], players[i].pos.m[2]);
			drawPlayer(i);
			glPopMatrix();
		}
	}

	drawBus();
	DrawSnowballs();

	//디버그 테스트 끝내면 없앨거임
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(debug_x, debug_y, 1.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);  //시점변환
	glFlush();
}

//화면 해상도가 바뀌면
//화면 보는 시점만 수정
void MyReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//원근법이 없는 일반투영
	glOrtho(-20.0, 20.0, -20.0, 20.0, -100.0, 100.0); // 15가 좋은 view
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.0, 1.0, 1.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);  //시점변환
}

void mySpecialKeyEvent(int key, int mouseX, int mouseY)
{
	switch (key) {
	case  GLUT_KEY_UP:
		players->pos.m[2] -= 0.5f;
		glutPostRedisplay();
		break;
	case  GLUT_KEY_DOWN:
		players->pos.m[2] += 0.5f;
		glutPostRedisplay();
		break;
	case  GLUT_KEY_LEFT:
		players->pos.m[0] -= 0.5f;
		glutPostRedisplay();
		break;
	case  GLUT_KEY_RIGHT:
		players->pos.m[0] += 0.5f;
		glutPostRedisplay();
		break;
	}

}

void myKeyboardEvent(unsigned char key, int mouseX, int mouseY)
{
	switch (key) {
		//e를 누르면 높이를 볼 수 있음
	case 'e':
		if (debug_y == 1.0)
		{
			debug_x = 0;
			debug_y = 0;
		}
		else
		{
			debug_x = 1.0;
			debug_y = 1.0;
		}
		glutPostRedisplay();
		break;
	case ' ': //스페이스바		
		SnowBall sb;
		sb.pos.m[0] = players[0].pos.m[0] + 1.5; // 눈사람의 좌표 x, y
		sb.pos.m[1] = 3;
		sb.pos.m[2] = players[0].pos.m[2] - 3;

		sb.v.m[0] = 0;
		sb.v.m[1] = 0;
		sb.v.m[2] = -1.5;

		snowballs.push_back(sb);
		players[0].time = mytime;
		glutPostRedisplay();
		break;
	}
}
void myTimerCheck(int timeValue)
{
	int randNumber;
	mytime += 0.01;

	for (int i = 1; i < 4; i++)
	{
		randNumber = rand() % 300;
		if (randNumber == 203)
		{
			SnowBall sb;
			sb.pos.m[0] = players[i].pos.m[0] + 1.5;
			sb.pos.m[1] = 3;
			sb.pos.m[2] = players[i].pos.m[2] + 3.5;

			sb.v.m[0] = 0;
			sb.v.m[1] = 0;
			sb.v.m[2] = +1.5;
			snowballs.push_back(sb);
			players[i].time = mytime;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(10, myTimerCheck, 1);
}

int main(int argc, char** argv)
{
	srand(NULL);
	glutInit(&argc, argv);               //GLUT 윈도우 함수
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Snowball Game");
	glClearColor(0.4, 0.4, 0.4, 1);

	for (int i = 0; i < 4; i++)
	{
		players[i].hp = 10;
		players[i].time = 0;
	}
	
	//플레이어
	players[0].pos.setPos(0,0,10);

	for (int i = 1; i < 4; i++)
		players[i].pos.setPos((i - 1)* 10 - 10,0,-10);
	

	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutKeyboardFunc(myKeyboardEvent);
	glutSpecialFunc(mySpecialKeyEvent);
	glutTimerFunc(10, myTimerCheck, 1);

	InitLight(0);
	glutMainLoop();
	return 0;
}