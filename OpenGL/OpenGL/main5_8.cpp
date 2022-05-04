#include <GL/glut.h>
int number = 1;

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.5, 0.0, 0.5);
    switch (number){
    case 1:
        glutWireSphere(0.2, 15, 15);
        break;
    case 2:
        glutWireTorus(0.3, 0.9, 40, 20);
        break;
    case 3:
        glutWireCone(0.9, 0.1, 20, 10);
        break;
    }
    glFlush();
}

void MyMainMenu(int entryID) {
    if (entryID == 4)
        exit(0);
    else
        number = entryID;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL Example Drawing");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
    glutAddMenuEntry("Draw Sphere", 1);
    glutAddMenuEntry("Draw Torus", 2);
    glutAddMenuEntry("Draw Cone", 3);
    glutAddMenuEntry("Exit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutDisplayFunc(MyDisplay);
    glutMainLoop();
    return 0;
}