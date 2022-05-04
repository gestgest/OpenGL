#include <GL/glut.h>
GLint TopLeftX, TopLeftY, BottomRightX, BottomRightY;

//행렬계산이 매우 효율적이다.
void MyDisplay() {
    //glViewport(0, 0, 300, 300);

    glMatrixMode(GL_MODELVIEW); //행열 건드릴 예쩡
    glLoadIdentity(); //행렬 초기화
    glTranslatef(TopLeftX / 300.0, (300 - TopLeftY) / 300.0, 0.0); //+x,y위치에 그리기

    glColor3f(0.5, 0.5, 0.5);
    glutWireTeapot(0.1);

    //glBegin(GL_LINES);
    //glVertex3f(TopLeftX / 300.0, (300 - TopLeftY) / 300.0, 0.0);
    //glVertex3f(BottomRightX / 300.0, (300 - BottomRightY) / 300.0, 0.0);
    //glEnd();
    glFlush();
}

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) {
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {
        TopLeftX = X;
        TopLeftY = Y;
        BottomRightX = X;
        BottomRightY = Y;
    }
}

void MyMouseMove(GLint X, GLint Y) {
    TopLeftX = BottomRightX;
    TopLeftY = BottomRightY;

    BottomRightX = X;
    BottomRightY = Y;

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL Drawing Example");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //0~1 0~1  [8장 내용]
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glutDisplayFunc(MyDisplay);
    glutMouseFunc(MyMouseClick);
    glutMotionFunc(MyMouseMove);
    glutMainLoop();
    return 0;
}