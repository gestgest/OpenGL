#include <GL/glut.h>
#include <math.h>
GLfloat Delta = 0.0;

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.5, 0.8);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glTranslatef(sin(Delta * 10), cos(Delta*10), 0);

    glutWireTeapot(0.1);

    /*
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.5, 0.8);
    glVertex3f(-1.0 + Delta, -0.5, 0.0);
    glVertex3f(0.0 + Delta, -0.5, 0.0);
    glVertex3f(0.0 + Delta, 0.5, 0.0);
    glVertex3f(-1.0 + Delta, 0.5, 0.0);
    glEnd();
    */
    glutSwapBuffers(); //GLUT_DOUBLE :이랑 있어야함
}

void MyIdle() {
    Delta = Delta + 0.0001;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //더블 버퍼링
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL Drawing Example");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0);
    glutDisplayFunc(MyDisplay);
    glutIdleFunc(MyIdle); //가만히 있어도 실행해라 함수 : cpu가 가만히 있으면 실행되는 함수? 빠를수록 많이 됨
    glutMainLoop();
    return 0;
}