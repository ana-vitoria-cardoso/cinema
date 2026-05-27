// Ana Vitória Cardoso Queiroz - 172706
// Maria Clara Muharem - 266864

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "camera.h"
#include "obj.h"
#include "textura.h"
#include "animacao.h"
#include "tempo.h"

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (animacao_esta_ativa()) 
    {
       gluLookAt(camera_get_camX(), camera_get_camY(), camera_get_camZ(),
                  camera_get_lookX(), camera_get_lookY(), camera_get_lookZ(),
                  0, 1, 0);
    } 
    else 
    {
        float radX = anguloX * 3.14159f / 180.0f;
        float radY = anguloY * 3.14159f / 180.0f;
        float cx = 3.5f + distancia * cos(radX) * sin(radY);
        float cy = 1.35f + distancia * sin(radX);
        float cz = -3.0f + distancia * cos(radX) * cos(radY);
        gluLookAt(cx, cy, cz, 0.0, 1.5, -23.0, 0, 1, 0);
    }
    
    float intensidade = animacao_get_intensidade_luz();
    GLfloat luz0_amb[] = {0.25f * intensidade, 0.22f * intensidade, 0.22f * intensidade, 1.0f};
    GLfloat luz0_dif[] = {0.85f * intensidade, 0.80f * intensidade, 0.85f * intensidade, 1.0f};  
    GLfloat luz0_esp[] = {0.55f * intensidade, 0.55f * intensidade, 0.55f * intensidade, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, luz0_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz0_dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz0_esp);

    GLfloat luz_global[] = {0.18f * intensidade, 0.16f * intensidade, 0.20f * intensidade, 1.0f};  
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luz_global);
    GLfloat pos0[] = {0.0, 8.2, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);

    static float zLuz = -20.0;
    zLuz += 0.3;
    if (zLuz > 28) zLuz = -22;
    GLfloat pos1[] = {14.5, 3.8, zLuz, 1.0};
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);

    GLfloat pos2[] = {0.0, 5.2, -8.0, 1.0};
    GLfloat dir2[] = {0.0, -0.35, -0.94};
    glLightfv(GL_LIGHT2, GL_POSITION, pos2);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir2);

    //cena
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

    //texto final
    if(animacao_get_texto_visivel())
    {
        glDisable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, 1024, 0, 768);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        glColor3f(1.0, 0.9, 0.5);
        glRasterPos2i(320, 400);
        char *texto = "Studio Maria Ana Apresenta";
        char *c;
        for(c = texto; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
        
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_LIGHTING);
    }

    glutSwapBuffers();
}

void init()
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05, 0.05, 0.1, 1.0);
    carregarTexturas(); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55, 1.0, 0.8, 60.0);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_COLOR_MATERIAL);
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat luz0_amb[] = {0.25, 0.22, 0.22, 1.0};
    GLfloat luz0_dif[] = {0.85, 0.80, 0.85, 1.0};  
    GLfloat luz0_esp[] = {0.55, 0.55, 0.55, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, luz0_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz0_dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz0_esp);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.03);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.005);

    GLfloat luz1_amb[] = {0.05, 0.03, 0.01, 1.0};
    GLfloat luz1_dif[] = {1.2, 0.95, 0.55, 1.0};  
    GLfloat luz1_esp[] = {0.8, 0.7, 0.5, 1.0};

    glLightfv(GL_LIGHT1, GL_AMBIENT, luz1_amb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luz1_dif);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luz1_esp);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.08);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.02);

    GLfloat luz2_amb[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat luz2_dif[] = {1.3, 1.2, 1.0, 1.0};  
    GLfloat luz2_esp[] = {1.0, 0.95, 0.85, 1.0};

    glLightfv(GL_LIGHT2, GL_AMBIENT, luz2_amb);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, luz2_dif);
    glLightfv(GL_LIGHT2, GL_SPECULAR, luz2_esp);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.05);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.01);
    
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 35.0);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 12.0);

    GLfloat luz_global[] = {0.18, 0.16, 0.20, 1.0};  
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luz_global);

    GLfloat mat_spec[] = {0.85, 0.80, 0.75, 1.0};
    GLfloat brilho[] = {60.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, brilho);
    
    animacao_iniciar();
}

void idle(void)
{
    animacao_atualizar();
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Cinema 3D - Animacao");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMouseFunc(controlarCameraMouse);
    glutMotionFunc(controlarCameraMotion);
    glutKeyboardFunc(controlarCameraTeclado);
    glutSpecialFunc(controlarCameraEspecial);
    
    glutMainLoop();
    return 0;
}

//compilar
//gcc -o cinema.exe main.c camera.c obj.c textura.c image.c animacao.c tempo.c -lfreeglut -lglu32 -lopengl32