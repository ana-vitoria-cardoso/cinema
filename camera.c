#include "camera.h"
#include <GL/glut.h>

float anguloY = 0.0;
float anguloX = 0.0;
float distancia = 22.0;
float distanciaMin = 3.0;
float distanciaMax = 45.0;

int ultimoX, ultimoY;
int botaoPressionado = 0;

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
    
    if (button == 3) {
        distancia -= 1.2;
        if(distancia < distanciaMin) distancia = distanciaMin;
    }
    if (button == 4) {
        distancia += 1.2;
        if(distancia > distanciaMax) distancia = distanciaMax;
    }
    
    glutPostRedisplay();
}

void controlarCameraMotion(int x, int y) {
    if (botaoPressionado) {
        anguloY += (x - ultimoX) * 0.5;
        anguloX += (y - ultimoY) * 0.4;
        
        if(anguloX > 50) anguloX = 50;
        if(anguloX < -40) anguloX = -40;
        
        ultimoX = x;
        ultimoY = y;
        
        glutPostRedisplay();
    }
}

void controlarCameraTeclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W':
            distancia -= 1.0f;
            break;
        case 's': case 'S':
            distancia += 1.0f;
            break;
    }
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