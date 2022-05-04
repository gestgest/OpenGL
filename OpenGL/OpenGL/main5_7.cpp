#include <GL/glut.h>
#define SCREEN_X 300
#define SCREEN_Y 300

GLint TopLeftX, TopLeftY, BottomRightX, BottomRightY;
float r = 0, g = 0, b = 0;
bool clearScreen = false;

void MyDisplay() {
    glViewport(0, 0, SCREEN_X, SCREEN_Y);
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    //clear했는데 점 남아있는거 방지
    if (!clearScreen)
    {
        glVertex3f(TopLeftX / (float)SCREEN_X, (SCREEN_Y - TopLeftY) / (float)SCREEN_Y, 0.0);
        //glVertex3f(TopLeftX / 300.0, (300 - BottomRightY) / 300.0, 0.0);
        glVertex3f(BottomRightX / (float)SCREEN_X, (SCREEN_Y - BottomRightY) / (float)SCREEN_Y, 0.0);
        //glVertex3f(BottomRightX / 300.0, (SCREEN_X - TopLeftY) / 300.0, 0.0);
    }
    glEnd();
    glFlush();
}

//마우스
void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) {
    //마우스를 누른상태
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {
        clearScreen = false;
        //시작점
        TopLeftX = X;
        TopLeftY = Y;
        BottomRightX = X;
        BottomRightY = Y;
    }
}

//드래그
void MyMouseMove(GLint X, GLint Y) {
    TopLeftX = BottomRightX;
    TopLeftY = BottomRightY;
    BottomRightX = X;
    BottomRightY = Y;
    glutPostRedisplay();
}

//키보드
void myKeyboardButton(unsigned char keyPressed, int x, int y)
{
    switch (keyPressed){
        //클리어
    case 'c':
        r = 0.0; b = 0.0; g = 0.0;
        glClear(GL_COLOR_BUFFER_BIT);
        clearScreen = true;
        //도형을 그리지 않고 그린다[화면 클리어].
        glutPostRedisplay();
        break;
        //red
    case 'r':
        r = 1.0; b = 0.0; g = 0.0;
        break;
        //그린
    case 'g':
        r = 0.0; b = 0.0; g = 1.0;
        break;
        //블루
    case 'b':
        r = 0.0; b = 1.0; g = 0.0;
        break;

    }
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(SCREEN_X, SCREEN_Y);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL Drawing Example");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    glClear(GL_COLOR_BUFFER_BIT);

    glutDisplayFunc(MyDisplay);

    //마우스 함수
    //드래그 함수
    //키보드 함수
    glutMouseFunc(MyMouseClick);
    glutMotionFunc(MyMouseMove);
    glutKeyboardFunc(myKeyboardButton);
    glutMainLoop();
    return 0;
}
