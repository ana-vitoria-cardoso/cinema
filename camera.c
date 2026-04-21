// Ana Vitória Cardoso Queiroz - 172706
// Maria Clara Muharem - 266864

#include "camera.h"
#include "textura.h"
#include <GL/glut.h>
#include <stdio.h>

//camera
float anguloY = 0.0;
float anguloX = 0.0;
float distancia = 22.0;

int ultimoX, ultimoY;
int botaoPressionado = 0;

void initCamera() {
    anguloY = 0.0;
    anguloX = 0.0;
    distancia = 22.0;
}

void controlarCameraTeclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W':
            distancia -= 1.0f;
            break;
        case 's': case 'S':
            distancia += 1.0f;
            break;
        case 't': case 'T':  
            toggleTexturas();//liga/desliga texturas
            break;
    }
    glutPostRedisplay();
}

void controlarCameraEspecial(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:    anguloX -= 2.0f; break;
        case GLUT_KEY_DOWN:  anguloX += 2.0f; break;
        case GLUT_KEY_LEFT:  anguloY -= 2.0f; break;
        case GLUT_KEY_RIGHT: anguloY += 2.0f; break;
    }
    
    glutPostRedisplay();
}

void controlarCameraMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            botaoPressionado = 1;
            ultimoX = x;
            ultimoY = y;
        } else {
            botaoPressionado = 0;
        }
    }
    
    if (button == 3) { //scroll up
        distancia -= 1.2;
    }
    if (button == 4) {//ccroll down
        distancia += 1.2;
    }
    
    glutPostRedisplay();
}

void controlarCameraMotion(int x, int y) {
    if (botaoPressionado) {
        anguloY += (x - ultimoX) * 0.5;
        anguloX += (y - ultimoY) * 0.4;
        
        ultimoX = x;
        ultimoY = y;
        
        glutPostRedisplay();
    }
}