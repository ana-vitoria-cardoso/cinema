#include <GL/glut.h>
#include <stdio.h>

// ============================
// INTEGRANTES
// ============================
// Nome 1: SEU NOME
// Nome 2: NOME DA DUPLA

int modoWireframe = 0;

// ============================
// COLOQUE SEUS PONTOS AQUI
// ============================
GLfloat pontos[][2] = {
    // SUBSTITUA PELOS SEUS PONTOS
    {-0.5, -0.5},
    {-0.3,  0.5},
    { 0.0, -0.5},
    { 0.3,  0.5},
    { 0.5, -0.5}
};

int numPontos = sizeof(pontos) / sizeof(pontos[0]);

// ============================
// DESENHO
// ============================
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (modoWireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < numPontos; i++) {
        glVertex2f(pontos[i][0], pontos[i][1]);
    }
    glEnd();

    glFlush();
}

// ============================
// CLIQUE DO MOUSE
// ============================
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        modoWireframe = !modoWireframe;
        glutPostRedisplay();
    }
}

// ============================
// INICIALIZAÇÃO
// ============================
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

// ============================
// MAIN
// ============================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(800, 600);
    glutCreateWindow("Ferramenta Mecânica - Malha Triangular");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}