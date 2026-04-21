#include "obj.h"
#include <GL/glut.h>
#include <math.h>

void desenharCadeiraVIP() {
     glPushMatrix();
    
    //base
    glColor3f(0.85, 0.72, 0.35);
    glPushMatrix();
    glTranslatef(0, 0.08, 0);
    glScalef(1.8, 0.08, 1.6);
    glutSolidCube(1.0);
    glPopMatrix();
    
    //assento
    glColor3f(0.45, 0.25, 0.18);
    glPushMatrix();
    glTranslatef(0, 0.35, 0.1);
    glScalef(1.7, 0.25, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();
   
    glColor3f(0.55, 0.18, 0.22);
    glPushMatrix();
    glTranslatef(0, 0.5, 0.1);
    glScalef(1.65, 0.1, 1.45);
    glutSolidCube(1.0);
    glPopMatrix();
    
    //encosto
    glColor3f(0.45, 0.25, 0.18);
    glPushMatrix();
    glTranslatef(0, 0.9, 0.6);
    glRotatef(18, 1, 0, 0);
    glScalef(1.7, 1.3, 0.28);  
    glutSolidCube(1.0);
    glPopMatrix();
   
    glColor3f(0.55, 0.18, 0.22);
    glPushMatrix();
    glTranslatef(0, 0.9, 0.58);
    glRotatef(18, 1, 0, 0);
    glScalef(1.65, 1.25, 0.1); 
    glutSolidCube(1.0);
    glPopMatrix();
    
    //apoio cabeca
    glColor3f(0.45, 0.25, 0.18);
    glPushMatrix();
    glTranslatef(0, 1.45, 0.75);
    glRotatef(10, 1, 0, 0);
    glScalef(1.2, 0.35, 0.2);
    glutSolidCube(1.0);
    glPopMatrix();
    
    //apoio braço esq
    glColor3f(0.35, 0.18, 0.08);
    glPushMatrix();
    glTranslatef(-0.95, 0.55, 0.05);
    glScalef(0.18, 0.35, 1.5);       
    glutSolidCube(1.0);
    glPopMatrix();
    
    glColor3f(0.55, 0.18, 0.22);
    glPushMatrix();
    glTranslatef(-0.95, 0.75, 0.05);  
    glScalef(0.18, 0.12, 1.45);     
    glutSolidCube(1.0);
    glPopMatrix();
    
    //apoio braço dir
    glColor3f(0.35, 0.18, 0.08);
    glPushMatrix();
    glTranslatef(0.95, 0.55, 0.05);  
    glScalef(0.18, 0.35, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glColor3f(0.55, 0.18, 0.22);
    glPushMatrix();
    glTranslatef(0.95, 0.75, 0.05); 
    glScalef(0.18, 0.12, 1.45);     
    glutSolidCube(1.0);
    glPopMatrix();
    
}

void desenharDegrau(float zPos, float yPos, float largura) {
    glColor3f(0.22, 0.22, 0.28);
    glPushMatrix();
    glTranslatef(0, yPos - 0.1, zPos);
    glScalef(largura, 0.3, 3.2);        
    glutSolidCube(1);
    glPopMatrix();
    
    //chao
    glColor3f(0.5, 0.08, 0.15);
    glPushMatrix();
    glTranslatef(0, yPos + 0.05, zPos);
    glScalef(largura - 0.5, 0.05, 3.0);
    glutSolidCube(1);
    glPopMatrix();
    
    //borda
    glColor3f(0.85, 0.72, 0.35);
    glPushMatrix();
    glTranslatef(0, yPos + 0.18, zPos + 1.55);  
    glScalef(largura - 0.8, 0.03, 0.08);
    glutSolidCube(1);
    glPopMatrix();
}

void desenharPlateia() {
    int fileiras = 8;
    float zInicio = -3.0;
    float espacamentoZ = 3.8;
    float alturaPorDegrau = 0.65;
    
    for(int i = 0; i < fileiras; i++) {
        float zPos = zInicio + (i * espacamentoZ);
        float yPos = i * alturaPorDegrau;
        
        float larguraEsquerda = 9.5;
        desenharDegrau(zPos, yPos, larguraEsquerda);
        
        glPushMatrix();
        glTranslatef(11.5, 0, 0);
        desenharDegrau(zPos, yPos, larguraEsquerda);
        glPopMatrix();
        
        for(float x = -7.5; x <= -2.5; x += 2.2) {
            glPushMatrix();
            glTranslatef(x, yPos + 0.25, zPos);  
            desenharCadeiraVIP();
            glPopMatrix();
        }
        
        for(float x = 3.5; x <= 8.5; x += 2.2) {
            glPushMatrix();
            glTranslatef(x, yPos + 0.25, zPos); 
            desenharCadeiraVIP();
            glPopMatrix();
        }
        
        if(i >= 5) {
            for(float x = -10; x <= -8; x += 2.0) {
                glPushMatrix();
                glTranslatef(x, yPos + 0.25, zPos);  
                desenharCadeiraVIP();
                glPopMatrix();
            }
            for(float x = 9; x <= 11; x += 2.0) {
                glPushMatrix();
                glTranslatef(x, yPos + 0.25, zPos);
                desenharCadeiraVIP();
                glPopMatrix();
            }
        }
    }
}
void desenharChao() {
    glColor3f(0.22, 0.22, 0.28);
    glPushMatrix();
    glTranslatef(0, -0.3, 0);
    glScalef(32, 0.1, 55);
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.5, 0.08, 0.15);
    glPushMatrix();
    glTranslatef(0, -0.25, 0);
    glScalef(30, 0.05, 53);
    glutSolidCube(1);
    glPopMatrix();
}

void desenharTeto() {
    glColor3f(0.08, 0.06, 0.12);
    glPushMatrix();
    glTranslatef(0, 9.0, 0); 
    glScalef(32, 0.15, 55);
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.85, 0.72, 0.35);
    for(float x = -15; x <= 15; x += 30) {
        glPushMatrix();
        glTranslatef(x, 8.6, 0); 
        glScalef(0.3, 0.4, 54);
        glutSolidCube(1);
        glPopMatrix();
    }

    glColor3f(1.0, 0.9, 0.6);
    
    for(float x = -11; x <= -3; x += 2.0) {
        for(float z = -20; z <= 25; z += 4.0) {
            glPushMatrix();
            glTranslatef(x, 8.92, z);
            glScalef(0.8, 0.03, 1.2);
            glutSolidCube(1);
            glPopMatrix();
        }
    }
    
    for(float x = 4; x <= 12; x += 2.0) {
        for(float z = -20; z <= 25; z += 4.0) {
            glPushMatrix();
            glTranslatef(x, 8.92, z);
            glScalef(0.8, 0.03, 1.2);
            glutSolidCube(1);
            glPopMatrix();
        }
    }
    
    for(float z = -18; z <= 27; z += 3.5) {
        glPushMatrix();
        glTranslatef(0, 8.92, z);
        glScalef(1.5, 0.03, 1.0);
        glutSolidCube(1);
        glPopMatrix();
    }
    
    for(float z = -18; z <= 27; z += 2.5) {
        glPushMatrix();
        glTranslatef(11, 8.92, z);
        glScalef(0.6, 0.03, 1.8);
        glutSolidCube(1);
        glPopMatrix();
    }
    
    for(float x = -11; x <= -3; x += 2.0) {
        for(float z = -20; z <= 25; z += 4.0) {
            glPushMatrix();
            glTranslatef(x, 8.89, z);
            glScalef(0.9, 0.02, 1.3);
            glutSolidCube(1);
            glPopMatrix();
        }
    }
    
    for(float x = 4; x <= 12; x += 2.0) {
        for(float z = -20; z <= 25; z += 4.0) {
            glPushMatrix();
            glTranslatef(x, 8.89, z);
            glScalef(0.9, 0.02, 1.3);
            glutSolidCube(1);
            glPopMatrix();
        }
    }
    
    //projetor
    glColor3f(0.3, 0.3, 0.35);
    glPushMatrix();
    glTranslatef(0, 8.2, -15);
    glScalef(1.5, 0.3, 1.8);
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.1, 0.1, 0.15);
    glPushMatrix();
    glTranslatef(0, 8.0, -14.5);
    glScalef(1.2, 0.2, 0.8);
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.5, 0.5, 0.6);
    glPushMatrix();
    glTranslatef(0, 7.9, -14.8);
    glScalef(0.8, 0.1, 0.5);
    glutSolidCube(1);
    glPopMatrix();
}

void desenharParedes() {
    glColor3f(0.08, 0.06, 0.12);
    
    glPushMatrix();
    glTranslatef(0, 4.5, -24); 
    glScalef(32, 9.0, 0.2);    
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-16, 4.5, 0); 
    glScalef(0.2, 9.0, 55);    
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(16, 4.5, 0);   
    glScalef(0.2, 9.0, 55);     
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 4.5, 28);   
    glScalef(32, 9.0, 0.2);     
    glutSolidCube(1);
    glPopMatrix();
}

void desenharTela() {
    glColor3f(0.85, 0.72, 0.35);
    glPushMatrix();
    glTranslatef(0, 5.0, -23.5);  
    glScalef(12, 8.0, 0.1);       
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.98, 0.98, 1.0);
    glPushMatrix();
    glTranslatef(0, 5.0, -23.3);  
    glScalef(11.5, 7.5, 0.05);    
    glutSolidCube(1);
    glPopMatrix();
    
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

void desenharLuzes() {
    glColor3f(0.85, 0.72, 0.35);
  
    for(float z = -20; z <= 25; z += 3.5) {
        glColor3f(0.85, 0.72, 0.35);
        glPushMatrix();
        glTranslatef(-15.5, 4.5, z);  
        glScalef(0.2, 0.8, 0.2);
        glutSolidCube(1);
        glPopMatrix();
        
        glColor3f(1.0, 0.7, 0.3);
        glPushMatrix();
        glTranslatef(-15.5, 4.2, z); 
        glutSolidSphere(0.15, 12, 12);
        glPopMatrix();
        
        glColor3f(0.85, 0.72, 0.35);
        glPushMatrix();
        glTranslatef(15.5, 4.5, z);  
        glScalef(0.2, 0.8, 0.2);
        glutSolidCube(1);
        glPopMatrix();
        
        glColor3f(1.0, 0.7, 0.3);
        glPushMatrix();
        glTranslatef(15.5, 4.2, z);   
        glutSolidSphere(0.15, 12, 12);
        glPopMatrix();
    }
}

void desenharCorredorLateral() {
    glColor3f(0.5, 0.08, 0.15);
    glPushMatrix();
    glTranslatef(11, -0.2, 4);
    glScalef(5.5, 0.05, 48);
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.85, 0.72, 0.35);
    for(float z = -18; z <= 25; z += 4) {
        glPushMatrix();
        glTranslatef(11.5, -0.1, z);
        glScalef(0.35, 0.05, 0.35);
        glutSolidCube(1);
        glPopMatrix();
    }
}

void desenharEscadaria() {
    float altura = 0;
    for(float z = -18; z <= 25; z += 3.2) {
        glColor3f(0.22, 0.22, 0.28);
        glPushMatrix();
        glTranslatef(11, altura - 0.15, z);
        glScalef(4.5, 0.2, 2.8);
        glutSolidCube(1);
        glPopMatrix();
        
        glColor3f(0.5, 0.08, 0.15);
        glPushMatrix();
        glTranslatef(11, altura - 0.05, z);
        glScalef(4.3, 0.05, 2.6);
        glutSolidCube(1);
        glPopMatrix();
        
        altura += 0.18;
    }
}

void desenharPorta() {
    glColor3f(0.35, 0.18, 0.08);
    glPushMatrix();
    glTranslatef(11, 1.8, 26);   
    glScalef(2.2, 4.0, 0.1);      
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.85, 0.72, 0.35);
    glPushMatrix();
    glTranslatef(10.2, 2.0, 26.05); 
    glutSolidSphere(0.1, 12, 12);
    glPopMatrix();
    
    glColor3f(0.8, 0.6, 0.1);
    glPushMatrix();
    glTranslatef(11, 3.2, 26.05);  
    glScalef(1.2, 0.3, 0.05);
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.08, 0.06, 0.12);
    glPushMatrix();
    glTranslatef(13.5, 2.5, 26);
    glScalef(0.2, 5.0, 0.5);
    glutSolidCube(1);
    glPopMatrix();
}

void desenharCaixasSom() {
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
