#include <GL/glut.h>
GLfloat MyVertices[4][3] = {
	{ 0, 0, 0 },
	{ 0.25, 0, 0 },
	{ 0, 0.25, 0 },
	{ 0, 0, 0.25 }
};
GLfloat MyColors[4][3] = {
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 0.0, 1.0, 1.0 }
};
GLubyte MyVertexList[12] = {
	0, 1, 2,
	2, 0, 3,
	3, 0, 1,
	1, 2, 3
};

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glFrontFace(GL_CCW); // 깊이검사(8장)를 하지 않고, front face사용 [어려우니 넘어가자]
	glEnable(GL_CULL_FACE); //[어려우니 넘어가자]
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY); //정점배열 사용
	glColorPointer(3, GL_FLOAT, 0, MyColors);
	glVertexPointer(3, GL_FLOAT, 0, MyVertices); //3개=>x,y,z 타입, ?, 배열

	//화면 전환
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(260.0, 1.0, 1.0, 1.0);

	//폴리곤, 4개를 한번에 읽어라, ? ,시작 위치는 여기
	for (GLint i = 0; i < 4; i++)
		glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_BYTE, &MyVertexList[3 * i]);
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Drawing Example");
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glutDisplayFunc(MyDisplay);
	glutMainLoop();
	return 0;
}