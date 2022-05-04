#include <GL/glut.h>

bool color = false;
float dx,dy = 0;

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (color)
        glColor3f(1, 0, 0);
    else
        glColor3f(0, 0, 1);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5 + dx, -0.5 + dy, 0.0);
    glVertex3f(0.5 + dx, -0.5 + dy, 0.0);
    glVertex3f(0.5 + dx, 0.5 + dy, 0.0);
    glVertex3f(-0.5 + dx, 0.5 + dy, 0.0);
    glEnd();
    glFlush();
}

void MyKeyboard(unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
    case 'r':
        color = true;
        glutPostRedisplay();
        break;
    case 'b':
        color = false;
        glutPostRedisplay();
        break;
    case 'Q':
        exit(0); break;
    case 'q':
        exit(0); break;
    case 27:                //'esc' 키의 아스키 코드 값
        exit(0); break;
    }
}
void mySpecialKey(int keyPressed,int x, int y)
{
    switch (keyPressed) {
    case GLUT_KEY_DOWN:
        dy -= 0.1;
        glutPostRedisplay();
        break;
    case GLUT_KEY_UP:
        dy += 0.1;
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:
        dx -= 0.1;
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT:
        dx += 0.1;
        glutPostRedisplay();
        break;
    }
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
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    glutSpecialFunc(mySpecialKey);

    glutMainLoop();
    return 0;
}