// Ana Vitória Cardoso Queiroz - 172706
// Maria Clara Muharem - 266864

#include <GL/glut.h>

// CONTROLE DA CÂMERA
float anguloY = 0.0;
float distancia = 15.0;

int ultimoX;
int botaoPressionado = 0;


// OBJETOS
// CHÃO
void desenharChao() {
    glColor3f(0.2, 0.2, 0.2);

    glPushMatrix();
    glScalef(12, 0.1, 12);
    glutSolidCube(1);
    glPopMatrix();
}

// TETO
void desenharTeto() {
    glColor3f(0.15, 0.15, 0.15);

    glPushMatrix();
    glTranslatef(0, 6, 0);
    glScalef(12, 0.1, 12);
    glutSolidCube(1);
    glPopMatrix();
}

// PAREDES
void desenharParedes() {
    glColor3f(0.1, 0.1, 0.1);

    // fundo
    glPushMatrix();
    glTranslatef(0, 3, -6);
    glScalef(12, 6, 0.2);
    glutSolidCube(1);
    glPopMatrix();

    // esquerda
    glPushMatrix();
    glTranslatef(-6, 3, 0);
    glScalef(0.2, 6, 12);
    glutSolidCube(1);
    glPopMatrix();

    // direita
    glPushMatrix();
    glTranslatef(6, 3, 0);
    glScalef(0.2, 6, 12);
    glutSolidCube(1);
    glPopMatrix();
}

// TELA
void desenharTela() {
    glColor3f(0.9, 0.9, 0.9);

    glPushMatrix();
    glTranslatef(0, 4, -5);
    glScalef(8, 4, 0.2);
    glutSolidCube(1);
    glPopMatrix();
}

// CADEIRA
void desenharCadeira() {
    // assento
    glColor3f(0.6, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.8, 0.3, 0.8);
    glutSolidCube(1);
    glPopMatrix();

    // encosto
    glPushMatrix();
    glTranslatef(0, 0.5, -0.3);
    glScalef(0.8, 1.0, 0.2);
    glutSolidCube(1);
    glPopMatrix();
}

// FILEIRAS
void desenharCadeiras() {
    for(int z = -2; z <= 4; z += 2) {
        for(int x = -4; x <= 4; x += 2) {
            glPushMatrix();
            glTranslatef(x, 0.5, z);
            desenharCadeira();
            glPopMatrix();
        }
    }
}

// LUZES
void desenharLuzes() {
    glColor3f(1.0, 1.0, 0.0);

    for(int i = -4; i <= 4; i += 2) {
        glPushMatrix();
        glTranslatef(-5.5, 3, i);
        glutSolidSphere(0.3, 20, 20);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(5.5, 3, i);
        glutSolidSphere(0.3, 20, 20);
        glPopMatrix();
    }
}

// DISPLAY
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // câmera com zoom
    gluLookAt(0, 5, distancia,
              0, 0, 0,
              0, 1, 0);

    // rotação com mouse
    glRotatef(anguloY, 0, 1, 0);

    // cena
    desenharChao();
    desenharTeto();
    desenharParedes();
    desenharTela();
    desenharCadeiras();
    desenharLuzes();

    glutSwapBuffers();
}

// MOUSE 
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            botaoPressionado = 1;
            ultimoX = x;
        } else {
            botaoPressionado = 0;
        }
    }

    // zoom
    if (button == 3) distancia -= 1.0; // scroll up
    if (button == 4) distancia += 1.0; // scroll down

    glutPostRedisplay();
}

// MOUSE (movimento)
void motion(int x, int y) {
    if (botaoPressionado) {
        anguloY += (x - ultimoX) * 0.5;
        ultimoX = x;
        glutPostRedisplay();
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.0, 1.0, 50.0);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 800);
    glutCreateWindow("Sala de Cinema 3D");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}