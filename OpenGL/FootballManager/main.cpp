#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int MX = 300;
int MY = 300;
float timer = 0;
float x = 0;
float throwBall = 0;
float walk = 0;
float drive = 0;
int flag = 0;
int countNum = 20;
int direction = 0;
int focus = 0;
int elbowAngle = 45, shoulderAngle = 45;
int countBomb = 0, countMissile = 0;
int humanLife = 5, truckLife = 10, droneLife = 3;

class vec3 {
public:
	float m[3];
	vec3(float x = 0, float y = 0, float z = 0) {
		m[0] = x; m[1] = y; m[2] = z;
	}
	vec3 operator+(vec3 x) {
		return vec3(m[0] + x.m[0], m[1] + x.m[1], m[2] + x.m[2]);
	}
	vec3 operator-(vec3 x) {
		return vec3(m[0] - x.m[0], m[1] - x.m[1], m[2] - x.m[2]);
	}
	vec3 operator*(float x) {
		return vec3(m[0] * x, m[1] * x, m[2] * x);
	}
	float Length() {
		return sqrtf(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
	}
};

class Attack {
public:
	vec3 v;
	vec3 pos;
};
vector<Attack> fireballs;
vector<Attack> bombs;
vector<Attack> missiles;

void MySpecial(int key, int X, int Y) {
	if (key == GLUT_KEY_LEFT) { // 왼쪽으로 움직임
		if (x > -7)	x -= 0.2f;
	}
	if (key == GLUT_KEY_RIGHT) { // 오른쪽으로 움직임
		if (x < 7) x += 0.2f;
	}
	if (key == GLUT_KEY_UP) { // 포커스를 위로 향해서 드론을 맞춘다
		focus = 1;
		shoulderAngle = 120; //오른 팔의 각도 변경
		elbowAngle = 30;
		throwBall = 4.5; //파이어볼의 높이 변경
	}
	if (key == GLUT_KEY_DOWN) { // 포커스를 아래로 향해서 트럭을 맞춘다
		focus = 0;
		shoulderAngle = 45;
		elbowAngle = 45;
		throwBall = 0;
	}
	glutPostRedisplay();
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
	if (KeyPressed == ' ' && fireballs.size() < 3) { //파이어 볼은 한번에 3개 까지 밖에 존재할 수 없다
		Attack f;
		f.pos.m[0] = x + 1.2; //사람의 좌표 x
		f.pos.m[1] = throwBall + 2.4;
		f.pos.m[2] = 10;
		f.v.m[0] = 0;
		f.v.m[1] = 0;
		f.v.m[2] = -1.5;
		fireballs.push_back(f);
		glutPostRedisplay();
	}

	//a d w s로 카메라 위치 변경
	if (KeyPressed == 'a') {
		MX = MX - 10;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(MX / 300.0, MY / 300.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glutPostRedisplay();
	}

	if (KeyPressed == 'd') {
		MX = MX + 10;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(MX / 300.0, MY / 300.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glutPostRedisplay();
	}

	if (KeyPressed == 'w') {
		MY = MY + 10;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(MX / 300.0, MY / 300.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glutPostRedisplay();
	}

	if (KeyPressed == 's') {
		MY = MY - 10;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(MX / 300.0, MY / 300.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glutPostRedisplay();
	}
}

void MyTimer(int v) {
	timer += 8.0; //바퀴를 굴리거나 프로펠러를 돌리기 위한 변수 증가

	// 팔과 다리는 일정 각도 이상 증가하거나 감소하지 않음
	if (walk >= 30) {
		flag = 1;
	}
	if (walk <= -30) {
		flag = 0;
	}

	if (flag == 0) {
		walk += 1.0;
	}
	else {
		walk -= 1.0;
	}

	glutPostRedisplay();
	glutTimerFunc(10, MyTimer, 0);
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-18.0, 18.0, -18.0, 18.0, -200.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(MX / 300.0, MY / 300.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void InitLight(int n) {
	GLfloat mat_diffuse_ground[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_specular_ground[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient_ground[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_shininess_ground[] = { 15.0 };

	GLfloat mat_diffuse_human[] = { 0.0, 0.0, 0.8, 1.0 };
	GLfloat mat_specular_human[] = { 0.0, 0.0, 0.8, 1.0 };
	GLfloat mat_ambient_human[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat mat_shininess_human[] = { 15.0 };

	GLfloat mat_diffuse_fireball[] = { 0.7, 0.0, 0.0, 1.0 };
	GLfloat mat_specular_fireball[] = { 0.4, 0.0, 0.0, 1.0 };
	GLfloat mat_ambient_fireball[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat mat_shininess_fireball[] = { 15.0 };

	GLfloat mat_diffuse_truck[] = { 0.8, 0.7, 0.2, 1.0 };
	GLfloat mat_specular_truck[] = { 0.5, 0.4, 0.1, 1.0 };
	GLfloat mat_ambient_truck[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat mat_shininess_truck[] = { 15.0 };

	GLfloat mat_diffuse_bomb[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat mat_specular_bomb[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat mat_ambient_bomb[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat mat_shininess_bomb[] = { 15.0 };

	GLfloat mat_diffuse_drone[] = { 0.4, 0.0, 0.7, 1.0 };
	GLfloat mat_specular_drone[] = { 0.2, 0.0, 0.3, 1.0 };
	GLfloat mat_ambient_drone[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_shininess_drone[] = { 15.0 };

	GLfloat mat_diffuse_missile[] = { 0.8, 0.0, 0.3, 1.0 };
	GLfloat mat_specular_missile[] = { 0.7, 0.0, 0.2, 1.0 };
	GLfloat mat_ambient_missile[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_shininess_missile[] = { 30.0 };

	GLfloat mat_diffuse_lifebar[] = { 0.0, 0.7, 0.0, 1.0 };
	GLfloat mat_specular_lifebar[] = { 0.0, 0.6, 0.0, 1.0 };
	GLfloat mat_ambient_lifebar[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_shininess_lifebar[] = { 15.0 };

	GLfloat light0_specular[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light0_diffuse[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat light0_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat light0_position[] = { -3, 6, 3, 0.0 };

	GLfloat light1_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light1_diffuse[] = { 0.2, 0.3, 0.2, 1.0 };
	GLfloat light1_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat light1_position[] = { -3, 6, -5, 0.0 };
	GLfloat Light1_direction[] = { -0.5, -1.0, -1.0 };
	GLfloat SpotAngle[] = { 20.0 };

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);

	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Light1_direction);
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, SpotAngle);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

	//물체마다 다른 색을 가짐
	switch (n)
	{
	case 0:
	default:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_ground);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_ground);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_ground);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_ground);
		break;
	case 1:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_human);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_human);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_human);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_human);
		break;
	case 2:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_fireball);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_fireball);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_fireball);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_fireball);
		break;
	case 3:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_truck);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_truck);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_truck);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_truck);
		break;
	case 4:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_bomb);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_bomb);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_bomb);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_bomb);
		break;
	case 5:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_drone);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_drone);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_drone);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_drone);
		break;
	case 6:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_missile);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_missile);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_missile);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_missile);
		break;
	case 7:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_lifebar);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_lifebar);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_lifebar);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_lifebar);
		break;

	}
	glEnable(GL_NORMALIZE);
}

bool HitCheckTruck() { //파이어볼이 트럭에 맞았는지 검사해 결과 리턴
	vec3 truck(drive, 2.0, -12);
	int size = fireballs.size();
	for (int i = 0; i < size; i++) {
		vec3 diff = truck - fireballs[i].pos;
		if (diff.Length() < 2) {
			fireballs.erase(fireballs.begin() + i); //트럭을 맞추면 파이어볼 소멸 - 바로 소멸해 주지 않으면 범위 내에 파이어볼이 있다면 여러번 타격해버림
			return true;
		}
	}
	return false;
}

bool HitCheckDrone() { //파이어 볼이 드론에 맞았는지 검사해 결과 리턴
	vec3 drone(-5, 6, -12);
	int size = fireballs.size();
	for (int i = 0; i < size; i++) {
		vec3 diff = drone - fireballs[i].pos;
		if (diff.Length() < 1) {
			fireballs.erase(fireballs.begin() + i);
			return true;
		}
	}
	return false;
}

bool HitCheckHumanByBomb() { // 사람이 폭탄에 맞았는지 결과 리턴
	vec3 human(x, 3, 12);
	int size = bombs.size();
	for (int i = 0; i < size; i++) {
		vec3 diff = human - bombs[i].pos;
		if (diff.Length() < 1.5) {
			bombs.erase(bombs.begin() + i);
			return true;
		}
	}
	return false;
}

bool HitCheckHumanByMissile() { //미사일이 사람에 맞았는지 결과 리턴
	vec3 human(x, 3, 12);
	int size = missiles.size();
	for (int i = 0; i < size; i++) {
		vec3 diff = human - missiles[i].pos;
		if (diff.Length() < 1.5) {
			missiles.erase(missiles.begin() + i);
			return true;
		}
	}
	return false;
}

void UpdateFireballs() {
	int size = fireballs.size();
	float fireBallTime = 0.1;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 3; j++)
			fireballs[i].pos.m[j] += fireballs[i].v.m[j] * fireBallTime;
	}

	for (int i = 0; i < fireballs.size(); ) { //일정 범위 이상 넘어가면 소멸
		if (fireballs[i].pos.m[2] > 20 || fireballs[i].pos.m[2] < -20)
			fireballs.erase(fireballs.begin() + i);
		else
			i++;
	}
}

void DrawFireballs() {
	UpdateFireballs();
	int size = fireballs.size();
	for (int i = 0; i < size; i++) {
		glPushMatrix();
		InitLight(2);
		glTranslatef(fireballs[i].pos.m[0], fireballs[i].pos.m[1], fireballs[i].pos.m[2]);
		glScalef(0.3, 0.3, 0.3);
		glutSolidDodecahedron();
		glPopMatrix();
	}
}

void UpdateBomb() {
	int size = bombs.size();
	float bombTime = 0.05;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 3; j++)
			bombs[i].pos.m[j] += bombs[i].v.m[j] * bombTime;
	}

	for (int i = 0; i < bombs.size(); ) {
		if (bombs[i].pos.m[2] > 20 || bombs[i].pos.m[2] < -20)
			bombs.erase(bombs.begin() + i);
		else
			i++;
	}
}

void DrawBomb() {
	UpdateBomb();
	int size = bombs.size();
	for (int i = 0; i < size; i++) {
		glPushMatrix();
		InitLight(4);
		glTranslatef(bombs[i].pos.m[0], bombs[i].pos.m[1], bombs[i].pos.m[2]);
		glutSolidSphere(1, 12, 12);
		glPopMatrix();
	}
}

void CreateBomb() { //폭탄의 좌표를 생성한다
	countBomb = 0;
	Attack b;
	b.pos.m[0] = drive;
	b.pos.m[1] = 2.4;
	b.pos.m[2] = -10;
	b.v.m[0] = 0;
	b.v.m[1] = 0;
	b.v.m[2] = 1.5; //폭탄은 파이어볼과 반대 방향을 향해 굴러온다
	bombs.push_back(b);
}

void UpdateMissile() {
	int size = missiles.size();
	float missileTime = 0.2;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 3; j++)
			missiles[i].pos.m[j] += missiles[i].v.m[j] * missileTime;
	}

	for (int i = 0; i < missiles.size(); ) {
		if (missiles[i].pos.m[2] > 20 || missiles[i].pos.m[2] < -20)
			missiles.erase(missiles.begin() + i);
		else
			i++;
	}
}

void DrawMissile() {
	UpdateMissile();
	int size = missiles.size();
	for (int i = 0; i < size; i++) {
		glPushMatrix();
		InitLight(6);
		glTranslatef(missiles[i].pos.m[0], missiles[i].pos.m[1], missiles[i].pos.m[2]);
		glutSolidCone(0.25, 0.25, 12, 12);
		glPopMatrix();
	}
}

void CreateMissile() {
	countMissile = 0;
	Attack m;
	m.pos.m[0] = -5;
	m.pos.m[1] = 6;
	m.pos.m[2] = -12;
	m.v.m[0] = 0;
	m.v.m[1] = -0.25; //미사일은 점점 내려옴
	m.v.m[2] = 1.5;
	missiles.push_back(m);
}

void DrawLifeBoxDrone() {
	glPushMatrix();

	InitLight(7);

	glTranslatef(-0.5, 4.0, 0.0);
	if (droneLife >= 1) {
		glutSolidCube(0.5);
	}
	else {
		glutWireCube(0.5);
	}
	glTranslatef(0.5, 0.0, 0.0);
	if (droneLife >= 2) {
		glutSolidCube(0.5);
	}
	else {
		glutWireCube(0.5);
	}
	glTranslatef(0.5, 0.0, 0.0);
	if (droneLife >= 3) {
		glutSolidCube(0.5);
	}
	else {
		glutWireCube(0.5);
	}

	glPopMatrix();
}

void DrawLifeBoxTruck() {
	glPushMatrix();

	InitLight(7);

	glTranslatef(-9, 15.0, 0.0);
	if (truckLife >= 1) {
		glutSolidCube(2);
	}
	else {
		glutWireCube(2);
	}
	glTranslatef(2.0, 0.0, 0.0);
	if (truckLife >= 2) {
		glutSolidCube(2);
	}
	else {
		glutWireCube(2);
	}
	glTranslatef(2.0, 0.0, 0.0);
	if (truckLife >= 3) {
		glutSolidCube(2);
	}
	else {
		glutWireCube(2);
	}
	glTranslatef(2.0, 0.0, 0.0);
	if (truckLife >= 4) {
		glutSolidCube(2);
	}
	else {
		glutWireCube(2);
	}
	glTranslatef(2.0, 0.0, 0.0);
	if (truckLife >= 5) {
		glutSolidCube(2);
	}
	else {
		glutWireCube(2);
	}
	glTranslatef(2.0, 0.0, 0.0);
	if (truckLife >= 6) {
		glutSolidCube(2);
	}
	else {
		glutWireCube(2);
	}
	glTranslatef(2.0, 0.0, 0.0);
	if (truckLife >= 7) {
		glutSolidCube(2);
	}
	else {
		glutWireCube(2);
	}
	glTranslatef(2.0, 0.0, 0.0);
	if (truckLife >= 8) {
		glutSolidCube(2);
	}
	else {
		glutWireCube(2);
	}
	glTranslatef(2.0, 0.0, 0.0);
	if (truckLife >= 9) {
		glutSolidCube(2);
	}
	else {
		glutWireCube(2);
	}
	glTranslatef(2.0, 0.0, 0.0);
	if (truckLife >= 10) {
		glutSolidCube(2);
	}
	else {
		glutWireCube(2);
	}

	glPopMatrix();
}

void DrawLifeBoxHuman() {
	glPushMatrix();

	InitLight(7);

	glTranslatef(-10.0, 45.0, 0.0);
	if (humanLife >= 1) {
		glutSolidCube(5);
	}
	else {
		glutWireCube(5);
	}
	glTranslatef(5.0, 0.0, 0.0);
	if (humanLife >= 2) {
		glutSolidCube(5);
	}
	else {
		glutWireCube(5);
	}
	glTranslatef(5.0, 0.0, 0.0);
	if (humanLife >= 3) {
		glutSolidCube(5);
	}
	else {
		glutWireCube(5);
	}
	glTranslatef(5.0, 0.0, 0.0);
	if (humanLife >= 4) {
		glutSolidCube(5);
	}
	else {
		glutWireCube(5);
	}
	glTranslatef(5.0, 0.0, 0.0);
	if (humanLife >= 5) {
		glutSolidCube(5);
	}
	else {
		glutWireCube(5);
	}

	glPopMatrix();
}

void DrawHuman() {
	//인간
	glPushMatrix();

	DrawLifeBoxHuman();

	InitLight(1);

	glScalef(0.5, 1.0, 0.5);
	glColor3f(0.8, 0.0, 0.0);
	glutSolidCube(15); //몸통
	glScalef(2.0, 1.0, 2.0);

	//머리 부분
	glPushMatrix(); //머리
	glTranslatef(0.0, 12, 0.0);
	glColor3f(0.8, 0.0, 0.0);
	glutSolidSphere(5, 24, 24);
	glPopMatrix();

	//팔 부분
	glPushMatrix(); //팔1 윗부분
	glTranslatef(-4.5, 5, 0.0);
	glRotatef((GLfloat)(walk), 1.0, 0.0, 0.0);
	glTranslatef(0.0, -5, 0.0);
	glScalef(1.0, 5.0, 1.0);
	glColor3f(0.8, 0.0, 0.0);
	glutSolidCube(2);
	glScalef(1.0, 0.2, 1.0);
	glPushMatrix(); //팔1 아랫부분
	glTranslatef(0.0, -5, 0.0);
	glRotatef((GLfloat)(walk), 1.0, 0.0, 0.0);
	glTranslatef(0.0, -3, 0.0);
	glScalef(1.0, 5.0, 1.0);
	glutSolidCube(1.5);
	glScalef(1.0, 0.2, 1.0);
	glPushMatrix(); //손1 부분
	glTranslatef(0.0, -5, 0.0);
	glutSolidCube(2);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//오른 팔의 상태에 따라 포커싱이 변경됨
	glPushMatrix(); //팔2 윗부분
	glTranslatef(4.5, 5, 0.0);
	glRotatef(shoulderAngle, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -5, 0.0);
	glScalef(1.0, 5.0, 1.0);
	glColor3f(0.8, 0.0, 0.0);
	glutSolidCube(2);
	glScalef(1.0, 0.2, 1.0);
	glPushMatrix(); //팔2 아랫부분
	glTranslatef(0.0, -5, 0.0);
	glRotatef(elbowAngle, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -3, 0.0);
	glScalef(1.0, 5.0, 1.0);
	glutSolidCube(1.5);
	glScalef(1.0, 0.2, 1.0);
	glPushMatrix(); //손2 부분
	glTranslatef(0.0, -5, 0.0);
	glutSolidCube(2);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//다리 부분
	glPushMatrix();//허벅지1
	glTranslatef(-2.5, -5, 0.0);
	glRotatef((GLfloat)(-walk), 1.0, 0.0, 0.0);
	glTranslatef(0.0, -8, 0.0);
	glScalef(1.0, 5.0, 1.0);
	glColor3f(0.3, 0.8, 0.3);
	glutSolidCube(2.5);
	glScalef(1.0, 0.2, 1.0);
	glPushMatrix(); //종아리1 부분
	glTranslatef(0.0, -5, 0.0);
	glRotatef((GLfloat)(-walk), 1.0, 0.0, 0.0);
	glTranslatef(0.0, -6, 0.0);
	glScalef(1.0, 5.0, 1.0);
	glutSolidCube(2);
	glScalef(1.0, 0.2, 1.0);
	glPushMatrix(); //발1 부분
	glTranslatef(0.0, -5, -2.0);
	glScalef(1.5, 0.2, 2.0);
	glutSolidCube(2.5);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();//허벅지2
	glTranslatef(2.5, -5, 0.0);
	glRotatef((GLfloat)(walk), 1.0, 0.0, 0.0);
	glTranslatef(0.0, -8, 0.0);
	glScalef(1.0, 5.0, 1.0);
	glColor3f(0.3, 0.8, 0.3);
	glutSolidCube(2.5);
	glScalef(1.0, 0.2, 1.0);
	glPushMatrix(); //종아리2 부분
	glTranslatef(0.0, -5, 0.0);
	glRotatef((GLfloat)(walk), 1.0, 0.0, 0.0);
	glTranslatef(0.0, -6, 0.0);
	glScalef(1.0, 5.0, 1.0);
	glutSolidCube(2);
	glScalef(1.0, 0.2, 1.0);
	glPushMatrix(); //발2 부분
	glTranslatef(0.0, -5, -2.0);
	glScalef(1.5, 0.2, 2.0);
	glutSolidCube(2.5);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void DrawTruck() {
	glPushMatrix();

	DrawLifeBoxTruck();

	InitLight(3);

	glutSolidCube(8); //윗 몸체
	glPushMatrix();
	glScalef(1.0, 0.5, 2.5);
	glTranslatef(0.0, -9.0, 2.3);
	glutSolidCube(8); //아랫 몸체
	glScalef(1.0, 2, 0.4);
	glTranslatef(0.0, -3.0, 0.0);
	glPushMatrix(); //바퀴1
	glTranslatef(-3.0, 0.0, -8.0);
	glRotatef(timer, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glutWireTorus(0.8, 1, 12, 12);
	glPopMatrix();
	glPushMatrix(); //바퀴2
	glTranslatef(3.0, 0.0, -8.0);
	glRotatef(timer, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glutWireTorus(0.8, 1, 12, 12);
	glPopMatrix();
	glPushMatrix(); //바퀴3
	glTranslatef(-3.0, 0.0, 8.0);
	glRotatef(timer, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glutWireTorus(0.8, 1, 12, 12);
	glPopMatrix();
	glPushMatrix(); //바퀴4
	glTranslatef(3.0, 0.0, 8.0);
	glRotatef(timer, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glutWireTorus(0.8, 1, 12, 12);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void DrawDrone() {
	glPushMatrix();

	DrawLifeBoxDrone();

	InitLight(5);

	glutSolidOctahedron(); //몸체
	glPushMatrix();
	glTranslatef(0.0, 1.0, 0.0);
	glScalef(1.0, 5.0, 1.0);
	glutSolidCube(0.2); //날개 기둥?
	glScalef(1.0, 0.2, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.5, 0.0);
	glutSolidSphere(0.2, 12, 12);
	glPushMatrix(); //날개 1
	glTranslatef(-0.1, 0.0, 0.0);
	glRotatef(timer, 0.0, 1.0, 0.0);
	glTranslatef(-0.7, 0.0, 0.0);
	glScalef(10.0, 0.2, 2.5);
	glutSolidCube(0.2);
	glPopMatrix();
	glPushMatrix(); //날개 2
	glTranslatef(-0.1, 0.0, 0.0);
	glRotatef(timer + 90, 0.0, 1.0, 0.0);
	glTranslatef(-0.7, 0.0, 0.0);
	glScalef(10.0, 0.2, 2.5);
	glutSolidCube(0.2);
	glPopMatrix();
	glPushMatrix(); //날개 3
	glTranslatef(-0.1, 0.0, 0.0);
	glRotatef(timer + 180, 0.0, 1.0, 0.0);
	glTranslatef(-0.7, 0.0, 0.0);
	glScalef(10.0, 0.2, 2.5);
	glutSolidCube(0.2);
	glPopMatrix();
	glPushMatrix(); //날개 4
	glTranslatef(-0.1, 0.0, 0.0);
	glRotatef(timer + 270, 0.0, 1.0, 0.0);
	glTranslatef(-0.7, 0.0, 0.0);
	glScalef(10.0, 0.2, 2.5);
	glutSolidCube(0.2);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();


	glPopMatrix();
}

void SetDirection() { //트럭의 방향을 랜덤으로 바꿔주는 함수
	if (truckLife < 0) { //트럭의 라이프가 없으면 움직이지 않음
		direction = 0;
		return;
	}

	if (countNum >= 40) { //일정 시간이 지나면 트럭이 방향을 변환함 좌, 정면, 우
		countNum = 0;
		int randNum = rand() % 3;
		direction = randNum;
	}

	else countNum++;
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetDirection();
	if (direction == 1) { //변환한 방향으로 서서히 이동(왼쪽) - 일정 범위 내에서만 이동
		if (drive >= -7) drive -= 0.1;
	}
	else if (direction == 2) { //변환한 방향으로 서서히 이동(오른쪽)
		if (drive <= 7) drive += 0.1;
	}

	//땅
	InitLight(0);
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -20);
	glScalef(25, 0.5, 200);
	glutSolidCube(1.0);
	glPopMatrix();

	//사람
	glPushMatrix();
	glTranslatef(0, 3, 12); // 기본 위치
	glTranslatef(x, 0, 0); // 키보드 조작
	glScalef(0.1, 0.1, 0.1);
	DrawHuman();
	glPopMatrix();

	//자동차
	glPushMatrix();
	glTranslatef(0, 3, -12); // 기본 위치
	glTranslatef(drive, 0.0, 0.0);
	glScalef(0.3, 0.3, 0.3);
	DrawTruck();
	glPopMatrix();

	//드론
	glPushMatrix();
	glTranslatef(-5, 6, -12); // 기본 위치
	glScalef(0.8, 0.8, 0.8);
	DrawDrone();
	glPopMatrix();

	// 눈덩이들

	if (countBomb > 500 && truckLife >= 0) { //일정 시간마다 폭탄 굴리기
		CreateBomb();
	}
	if (countMissile > 200 && droneLife >= 0) { //일정 시간마다 미사일 발사
		CreateMissile();
	}
	countBomb++;
	countMissile++;

	DrawFireballs();
	DrawBomb();
	DrawMissile();

	glFlush();

	bool hitTruck = HitCheckTruck();
	if (hitTruck)
		truckLife--;

	bool hitDrone = HitCheckDrone();
	if (hitDrone)
		droneLife--;

	bool hitHumanByBomb = HitCheckHumanByBomb();
	if (hitHumanByBomb)
		humanLife -= 2;

	bool hitHumanByMissile = HitCheckHumanByMissile();
	if (hitHumanByMissile)
		humanLife--;

	// 트럭과 드론을 모두 처리했거나 플레이어가 죽었다면 중단
	if (truckLife < 0 && droneLife < 0) exit(0);
	if (humanLife < 0) exit(0);

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Truck Destroy");
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutTimerFunc(10, MyTimer, 0);
	glutSpecialFunc(MySpecial);
	glutKeyboardFunc(MyKeyboard);
	glClearColor(0.4, 0.4, 0.4, 1);
	InitLight(0);

	glutMainLoop();
	return 0;
}