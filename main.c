#include <GL/glut.h>
#include "camera.h"
#include "obj.h"

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    gluLookAt(0, 5, distancia,
              0, 3.5, 0,
              0, 1, 0);
    
    glRotatef(anguloX, 1, 0, 0);
    glRotatef(anguloY, 0, 1, 0);
    desenharChao();
    desenharTeto();
    desenharParedes();
    desenharCorredorLateral();
    desenharEscadaria();
    desenharPorta();
    desenharTela();
    desenharPlateia();
    desenharLuzes();
    desenharCaixasSom();
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55, 1.0, 0.8, 60.0);
    
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Cinema 3D - MC.AV");
    
    init();
    
    glutDisplayFunc(display);
    glutMouseFunc(controlarCameraMouse);
    glutMotionFunc(controlarCameraMotion);
    glutKeyboardFunc(controlarCameraTeclado);
    glutSpecialFunc(controlarCameraEspecial);
    
    glutMainLoop();
    return 0;
}

//compilar
// gcc -o cinema.exe main.c camera.c obj.c -lfreeglut -lopengl32 -lglu32 -lm -Wall
//./cinema.exe
