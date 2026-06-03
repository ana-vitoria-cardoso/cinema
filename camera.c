// Ana Vitória Cardoso Queiroz - 172706
// Maria Clara Muharem - 266864

#include "camera.h"
#include "textura.h"
#include <GL/glut.h>
#include <stdio.h>

static float camX = 0.0, camY = 5.0, camZ = 22.0;
static float lookX = 0.0, lookY = 3.5, lookZ = 0.0;

//camera
float anguloY = 0.0;
float anguloX = 0.0;
float distancia = 22.0;

int ultimoX, ultimoY;
int botaoPressionado = 0;

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
    if (button == 4) {//scroll down
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

void camera_set_posicao(float x, float y, float z)
{
    camX = x;
    camY = y;
    camZ = z;
}

void camera_set_alvo(float x, float y, float z)
{
    lookX = x;
    lookY = y;
    lookZ = z;
}

void camera_get_posicao(float *x, float *y, float *z)
{
    *x = camX;
    *y = camY;
    *z = camZ;
}

float camera_get_camX(void)
{
    return camX;
}

float camera_get_camY(void)
{
    return camY;
}

float camera_get_camZ(void)
{
    return camZ;
}

float camera_get_lookX(void)
{
    return lookX;
}

float camera_get_lookY(void)
{
    return lookY;
}

float camera_get_lookZ(void)
{
    return lookZ;
}