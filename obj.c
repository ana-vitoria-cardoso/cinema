// Ana Vitória Cardoso Queiroz - 172706
// Maria Clara Muharem - 266864

#include "obj.h"
#include <GL/glut.h>
#include <math.h>
#include "animacao.h"

void desenharCadeiraVIP(int ehAlvo)
{// estatica
    glPushMatrix();

    glColor3f(0.85, 0.72, 0.35);
    glPushMatrix();
    glTranslatef(0, 0.08, 0);
    glScalef(1.8, 0.08, 1.6);
    glNormal3f(0, 1, 0);
    glutSolidCube(1.0);
    glPopMatrix();

    // assento
    glColor3f(0.45, 0.25, 0.18);
    glPushMatrix();
    glTranslatef(0, 0.35, 0.1);
    glScalef(1.7, 0.25, 1.5);
    glNormal3f(0, 1, 0);
    glutSolidCube(1.0);
    glPopMatrix();

    // assento acabamento
    glColor3f(0.55, 0.18, 0.22);
    glPushMatrix();
    glTranslatef(0, 0.5, 0.1);
    glScalef(1.65, 0.1, 1.45);
    glNormal3f(0, 1, 0);
    glutSolidCube(1.0);
    glPopMatrix();

    //apoio braço esquerdo
    glColor3f(0.35, 0.18, 0.08);
    glPushMatrix();
    glTranslatef(-0.95, 0.55, 0.05);
    glScalef(0.18, 0.35, 1.5);
    glNormal3f(-1, 0, 0);
    glutSolidCube(1.0);
    glPopMatrix();

    // apoio braço esquerdo acabamento
    glColor3f(0.55, 0.18, 0.22);
    glPushMatrix();
    glTranslatef(-0.95, 0.75, 0.05);
    glScalef(0.18, 0.12, 1.45);
    glNormal3f(-1, 0, 0);
    glutSolidCube(1.0);
    glPopMatrix();

    // apoio braço direito (base)
    glColor3f(0.35, 0.18, 0.08);
    glPushMatrix();
    glTranslatef(0.95, 0.55, 0.05);
    glScalef(0.18, 0.35, 1.5);
    glNormal3f(1, 0, 0);
    glutSolidCube(1.0);
    glPopMatrix();

    // apoio braço direito (acabamento)
    glColor3f(0.55, 0.18, 0.22);
    glPushMatrix();
    glTranslatef(0.95, 0.75, 0.05);
    glScalef(0.18, 0.12, 1.45);
    glNormal3f(1, 0, 0);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();// movel
    glTranslatef(0.0, 0.5, 0.45);
        float inclinacaoDinamica = 0.0;
        if (ehAlvo) 
        {
            inclinacaoDinamica = animacao_get_inclinacao_cadeira();
        }
        glRotatef(18.0 + inclinacaoDinamica, 1, 0, 0);
        glTranslatef(0.0, -0.5, -0.45);

        // encosto
        glColor3f(0.45, 0.25, 0.18);
        glPushMatrix();
        glTranslatef(0, 0.9, 0.6);
        glScalef(1.7, 1.3, 0.28);
        glNormal3f(0, 0.95, 0.31);
        glutSolidCube(1.0);
        glPopMatrix();

        // encosto acabamento
        glColor3f(0.55, 0.18, 0.22);
        glPushMatrix();
        glTranslatef(0, 0.9, 0.58);
        glScalef(1.65, 1.25, 0.1);
        glNormal3f(0, 0.95, 0.31);
        glutSolidCube(1.0);
        glPopMatrix();

        // apoio cabeca
        glColor3f(0.45, 0.25, 0.18);
        glPushMatrix();
        glTranslatef(0, 1.45, 0.75);
        glRotatef(10, 1, 0, 0);
        glScalef(1.2, 0.35, 0.2);
        glNormal3f(0, 0.98, 0.17);
        glutSolidCube(1.0);
        glPopMatrix();

    glPopMatrix(); // movel
    glPopMatrix(); // cadeira
}

void desenharDegrau(float zPos, float yPos, float largura)
{
    // base do degrau
    glColor3f(0.22, 0.22, 0.28);
    glPushMatrix();
    glTranslatef(0, yPos - 0.1, zPos);
    glScalef(largura, 0.3, 3.2);
    glNormal3f(0, 1, 0);
    glutSolidCube(1);
    glPopMatrix();

    // piso do degrau
    glColor3f(0.5, 0.08, 0.15);
    glPushMatrix();
    glTranslatef(0, yPos + 0.05, zPos);
    glScalef(largura - 0.5, 0.05, 3.0);
    glNormal3f(0, 1, 0);
    glutSolidCube(1);
    glPopMatrix();

    // borda dourada
    glColor3f(0.85, 0.72, 0.35);
    glPushMatrix();
    glTranslatef(0, yPos + 0.18, zPos + 1.55);
    glScalef(largura - 0.8, 0.03, 0.08);
    glNormal3f(0, 1, 0);
    glutSolidCube(1);
    glPopMatrix();
}

void desenharPlateia()
{
    int fileiras = 8;
    float zInicio = -3.0;
    float espacamentoZ = 3.8;
    float alturaPorDegrau = 0.65;

    for (int i = 0; i < fileiras; i++)
    {
        float zPos = zInicio + (i * espacamentoZ);
        float yPos = i * alturaPorDegrau;

        float larguraEsquerda = 9.5;
        desenharDegrau(zPos, yPos, larguraEsquerda);

        glPushMatrix();
        glTranslatef(11.5, 0, 0);
        desenharDegrau(zPos, yPos, larguraEsquerda);
        glPopMatrix();
        for (float x = -7.5; x <= -2.5; x += 2.2)//esq
        {
            glPushMatrix();
            glTranslatef(x, yPos + 0.25, zPos);
            desenharCadeiraVIP(0);
            glPopMatrix();
        }
        for (float x = 3.5; x <= 8.5; x += 2.2)//dir
        {
            glPushMatrix();
            glTranslatef(x, yPos + 0.25, zPos);
            if (i == 0 && fabs(x - 3.5) < 0.1) 
            {
                desenharCadeiraVIP(1); 
            }
            else 
            {
                desenharCadeiraVIP(0); 
            }
            
            glPopMatrix();
        }
        if (i >= 5)
        {
            for (float x = -10; x <= -8; x += 2.0)
            {
                glPushMatrix();
                glTranslatef(x, yPos + 0.25, zPos);
                desenharCadeiraVIP(0);
                glPopMatrix();
            }
            for (float x = 9; x <= 11; x += 2.0)
            {
                glPushMatrix();
                glTranslatef(x, yPos + 0.25, zPos);
                desenharCadeiraVIP(0);
                glPopMatrix();
            }
        }
    }
}

void desenharChao()
{
    glColor3f(0.22, 0.22, 0.28);
    glPushMatrix();
    glTranslatef(0, -0.3, 0);
    glScalef(32, 0.1, 55);
    glNormal3f(0, 1, 0);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(0.5, 0.08, 0.15);
    glPushMatrix();
    glTranslatef(0, -0.25, 0);
    glScalef(30, 0.05, 53);
    glNormal3f(0, 1, 0);
    glutSolidCube(1);
    glPopMatrix();
}

void desenharTeto()
{
    glColor3f(0.08, 0.06, 0.12);
    glPushMatrix();
    glTranslatef(0, 9.0, 0);
    glScalef(32, 0.15, 55);
    glNormal3f(0, -1, 0);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(0.85, 0.72, 0.35);
    for (float x = -15; x <= 15; x += 30)
    {
        glPushMatrix();
        glTranslatef(x, 8.6, 0);
        glScalef(0.3, 0.4, 54);
        glNormal3f(0, 1, 0);
        glutSolidCube(1);
        glPopMatrix();
    }

    glColor3f(1.0, 0.9, 0.6);

    for (float x = -11; x <= -3; x += 2.0)
    {
        for (float z = -20; z <= 25; z += 4.0)
        {
            glPushMatrix();
            glTranslatef(x, 8.92, z);
            glScalef(0.8, 0.03, 1.2);
            glNormal3f(0, 1, 0);
            glutSolidCube(1);
            glPopMatrix();
        }
    }

    for (float x = 4; x <= 12; x += 2.0)
    {
        for (float z = -20; z <= 25; z += 4.0)
        {
            glPushMatrix();
            glTranslatef(x, 8.92, z);
            glScalef(0.8, 0.03, 1.2);
            glNormal3f(0, 1, 0);
            glutSolidCube(1);
            glPopMatrix();
        }
    }

    for (float z = -18; z <= 27; z += 3.5)
    {
        glPushMatrix();
        glTranslatef(0, 8.92, z);
        glScalef(1.5, 0.03, 1.0);
        glNormal3f(0, 1, 0);
        glutSolidCube(1);
        glPopMatrix();
    }

    for (float z = -18; z <= 27; z += 2.5)
    {
        glPushMatrix();
        glTranslatef(11, 8.92, z);
        glScalef(0.6, 0.03, 1.8);
        glNormal3f(0, 1, 0);
        glutSolidCube(1);
        glPopMatrix();
    }

    for (float x = -11; x <= -3; x += 2.0)
    {
        for (float z = -20; z <= 25; z += 4.0)
        {
            glPushMatrix();
            glTranslatef(x, 8.89, z);
            glScalef(0.9, 0.02, 1.3);
            glNormal3f(0, 1, 0);
            glutSolidCube(1);
            glPopMatrix();
        }
    }

    for (float x = 4; x <= 12; x += 2.0)
    {
        for (float z = -20; z <= 25; z += 4.0)
        {
            glPushMatrix();
            glTranslatef(x, 8.89, z);
            glScalef(0.9, 0.02, 1.3);
            glNormal3f(0, 1, 0);
            glutSolidCube(1);
            glPopMatrix();
        }
    }

    // projetor
    glColor3f(0.3, 0.3, 0.35);
    glPushMatrix();
    glTranslatef(0, 8.2, -15);
    glScalef(1.5, 0.3, 1.8);
    glNormal3f(0, 1, 0);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(0.1, 0.1, 0.15);
    glPushMatrix();
    glTranslatef(0, 8.0, -14.5);
    glScalef(1.2, 0.2, 0.8);
    glNormal3f(0, 1, 0);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(0.5, 0.5, 0.6);
    glPushMatrix();
    glTranslatef(0, 7.9, -14.8);
    glScalef(0.8, 0.1, 0.5);
    glNormal3f(0, 1, 0);
    glutSolidCube(1);
    glPopMatrix();
}

void desenharTela()
{
    // moldura externa
    glColor3f(0.85, 0.72, 0.35);
    glPushMatrix();
    glTranslatef(0, 5.0, -23.5);
    glScalef(12, 8.0, 0.1);
    glutSolidCube(1);
    glPopMatrix();


   float escurecimento = animacao_get_escurecimento();
    float corTela = 0.95 * (1.0 - escurecimento);
    GLfloat emissive[] = {0.2f * (1.0f - escurecimento), 0.2f * (1.0f - escurecimento), 0.2f * (1.0f - escurecimento), 1.0f}; 
    glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
    glColor3f(corTela, corTela, corTela); // cor mudando
    glPushMatrix();
    glTranslatef(0, 5.0, -23.3);
    glScalef(11.5, 7.5, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();
    GLfloat no_emissive[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emissive);

    glColor3f(0.65, 0.12, 0.15);
    glPushMatrix();
    glTranslatef(-6.5, 5.0, -23);
    glScalef(1.2, 8.0, 0.4);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(6.5, 5.0, -23);
    glScalef(1.2, 8.0, 0.4);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 8.8, -23);
    glScalef(13, 0.8, 0.5);
    glutSolidCube(1);
    glPopMatrix();
}

void desenharLuzes()
{

    for (float z = -20; z <= 25; z += 3.5)
    {

        glColor3f(0.6, 0.5, 0.3);

        // esquerda
        glPushMatrix();
        glTranslatef(-15.5, 4.5, z);
        glScalef(0.2, 0.8, 0.2);
        glutSolidCube(1);
        glPopMatrix();

        // direita
        glPushMatrix();
        glTranslatef(15.5, 4.5, z);
        glScalef(0.2, 0.8, 0.2);
        glutSolidCube(1);
        glPopMatrix();

        // lampadas
        GLfloat emissive[] = {1.5, 1.1, 0.5, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, emissive);

        glColor3f(1.0, 0.75, 0.3);

        // esquerda
        glPushMatrix();
        glTranslatef(-15.5, 4.2, z);
        glutSolidSphere(0.18, 16, 16);
        glPopMatrix();

        // direita
        glPushMatrix();
        glTranslatef(15.5, 4.2, z);
        glutSolidSphere(0.18, 16, 16);
        glPopMatrix();
        GLfloat no_emissive[] = {0.0, 0.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emissive);
    }
}

void desenharCorredorLateral()
{
    glColor3f(0.5, 0.08, 0.15);
    glPushMatrix();
    glTranslatef(11, -0.2, 4);
    glScalef(5.5, 0.05, 48);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(0.85, 0.72, 0.35);
    for (float z = -18; z <= 25; z += 4)
    {
        glPushMatrix();
        glTranslatef(11.5, -0.1, z);
        glScalef(0.35, 0.05, 0.35);
        glutSolidCube(1);
        glPopMatrix();
    }
}

void desenharEscadaria()
{
    float altura = 0;
    for (float z = -18; z <= 25; z += 3.2)
    {
        glColor3f(0.22, 0.22, 0.28);
        glPushMatrix();
        glTranslatef(11, altura - 0.15, z);
        glScalef(4.5, 0.2, 2.8);
        glNormal3f(0, 1, 0);
        glutSolidCube(1);
        glPopMatrix();

        glColor3f(0.5, 0.08, 0.15);
        glPushMatrix();
        glTranslatef(11, altura - 0.05, z);
        glScalef(4.3, 0.05, 2.6);
        glNormal3f(0, 1, 0);
        glutSolidCube(1);
        glPopMatrix();

        altura += 0.18;
    }
}
void desenharPorta()
{
    float anguloPorta = animacao_get_abertura_porta();
    glPushMatrix();// movel
        glTranslatef(-12.1, 1.8, 26.0);
        glRotatef(-anguloPorta, 0, 1, 0);
        glTranslatef(12.1, -1.8, -26.0);
        glColor3f(0.35, 0.18, 0.08);
        glPushMatrix();
        glTranslatef(-11.0, 1.8, 26.0);
        glScalef(2.2, 4.0, 0.1);
        glutSolidCube(1);
        glPopMatrix();
        //maçaneta
        glColor3f(0.85, 0.72, 0.35);
        glPushMatrix();
        glTranslatef(-11.8, 2.0, 26.05);
        glutSolidSphere(0.1, 12, 12);
        glPopMatrix();
        glColor3f(0.8, 0.6, 0.1);
        glPushMatrix();
        glTranslatef(-11.0, 3.2, 26.05);
        glScalef(1.2, 0.3, 0.05);
        glutSolidCube(1);
        glPopMatrix();

    glPopMatrix();

    glColor3f(0.08, 0.06, 0.12);// estatica moldura
    glPushMatrix();
    glTranslatef(-13.5, 2.5, 26.0);
    glScalef(0.2, 5.0, 0.5);
    glutSolidCube(1);
    glPopMatrix();
}

void desenharCaixasSom()
{
    glColor3f(0.22, 0.22, 0.28);

    glPushMatrix();
    glTranslatef(-10, 4.5, -21);
    glScalef(1.2, 2.5, 1.0);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(-10, 4.0, -20.7);
    glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    glColor3f(0.22, 0.22, 0.28);
    glPushMatrix();
    glTranslatef(10, 4.5, -21);
    glScalef(1.2, 2.5, 1.0);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(10, 4.0, -20.7);
    glutSolidSphere(0.4, 20, 20);
    glPopMatrix();
}

void desenharParedes()
{
    glColor3f(0.08, 0.06, 0.12);

    // parede tela
    glPushMatrix();
    glTranslatef(0, 4.5, -24);
    glScalef(32, 9.0, 0.2);
    glNormal3f(0, 0, 1);
    glutSolidCube(1);
    glPopMatrix();

    // parede esquerda
    glPushMatrix();
    glTranslatef(-16, 4.5, 0);
    glScalef(0.2, 9.0, 55);
    glNormal3f(1, 0, 0);
    glutSolidCube(1);
    glPopMatrix();

    // parede direita
    glPushMatrix();
    glTranslatef(16, 4.5, 0);
    glScalef(0.2, 9.0, 55);
    glNormal3f(-1, 0, 0);
    glutSolidCube(1);
    glPopMatrix();

    // parede traseira
    glPushMatrix();
    glTranslatef(0, 4.5, 28);
    glScalef(32, 9.0, 0.2);
    glNormal3f(0, 0, -1);
    glutSolidCube(1);
    glPopMatrix();
}