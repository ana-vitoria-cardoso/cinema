// Ana Vitória Cardoso Queiroz - 172706
// Maria Clara Muharem - 266864

#include <GL/glut.h>
#include <stdio.h>
#include "camera.h"
#include "obj.h"
#include "textura.h"

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // câmera primeiro
    gluLookAt(0, 5, distancia,
              0, 3.5, 0,
              0, 1, 0);

    glRotatef(anguloX, 1, 0, 0);
    glRotatef(anguloY, 0, 1, 0);

    // AGORA sim posiciona a luz (na tela)
    GLfloat luz_pos[] = {0.0, 7.0, -20.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, luz_pos);

    // desenhar cena
    desenharChaoComTextura();
    desenharTeto();
    desenharParedesComTextura();
    desenharCorredorLateral();
    desenharEscadaria();
    desenharPorta();
    desenharTela();
    desenharPlateia();
    desenharLuzes();
    desenharCaixasSom();

    glutSwapBuffers();
}

void init()
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    carregarTexturas();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55, 1.0, 0.8, 60.0);
    glMatrixMode(GL_MODELVIEW);

    // iluminação
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // luz (AJUSTADA)
    GLfloat luz_amb[] = {0.1, 0.1, 0.1, 1.0};   // mais forte
    GLfloat luz_dif[] = {1.2, 1.2, 1.2, 1.0};   // branco total
    GLfloat luz_esp[] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz_esp);

    // atenuação (REDUZIDA - antes estava forte demais)
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001);

    // luz ambiente global (AUMENTADA)
    GLfloat luz_global[] = {0.1, 0.1, 0.1, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luz_global);

    // material
    GLfloat mat_spec[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat brilho[] = {100.0};

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, brilho);
}

int main(int argc, char **argv)
{
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

// compilar
//  gcc -o cinema.exe main.c camera.c obj.c image.c textura.c-lfreeglut -lopengl32 -lglu32 -lm -Wall
//./cinema.exe
