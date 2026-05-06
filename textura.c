// Ana Vitória Cardoso Queiroz - 172706
// Maria Clara Muharem - 266864

#include "textura.h"
#include "image.h"
#include "obj.h" 
#include <stdio.h>
#include <GL/glut.h>

GLuint textureIDParede = 0;
GLuint textureIDChao = 0;
int texturasAtivas = 1;  
IMAGE *textureImage = NULL;

void carregarTexturas() {
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //parede
    textureImage = ImageLoad("wall.rgb");
  {
        glGenTextures(1, &textureIDParede);
        glBindTexture(GL_TEXTURE_2D, textureIDParede);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        if (textureImage->sizeZ == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage->sizeX, 
                         textureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, 
                         textureImage->data);
        } else if (textureImage->sizeZ == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage->sizeX, 
                         textureImage->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                         textureImage->data);
        }
        
        free(textureImage->data);
        free(textureImage);
        textureImage = NULL;
    }
    //chao
    textureImage = ImageLoad("chao.rgb");
  {        
        glGenTextures(1, &textureIDChao);
        glBindTexture(GL_TEXTURE_2D, textureIDChao);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        if (textureImage->sizeZ == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage->sizeX, 
                         textureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, 
                         textureImage->data);
        } else if (textureImage->sizeZ == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage->sizeX, 
                         textureImage->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                         textureImage->data);
        }
        
        free(textureImage->data);
        free(textureImage);
        textureImage = NULL;
    }
}

void toggleTexturas() {
    texturasAtivas = !texturasAtivas;
    glutPostRedisplay();  
}

//função com normal específica para cada parede
void aplicarTexturaNaParede(float x1, float y1, float z1, float x2, float y2, float z2, float repX, float repY, int paredeTipo) {
    glBegin(GL_QUADS);
    
    // Define a normal baseada no tipo da parede
    switch(paredeTipo) {
        case 0: // parede frontal (tela) - aponta para +Z
            glNormal3f(0.0, 0.0, 1.0); break;
        case 1: // parede esquerda - aponta para +X
            glNormal3f(1.0, 0.0, 0.0); break;
        case 2: // parede direita - aponta para -X
            glNormal3f(-1.0, 0.0, 0.0); break;
        case 3: // parede traseira - aponta para -Z
            glNormal3f(0.0, 0.0, -1.0); break;
    }
    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y1, z1);
    glTexCoord2f(repX, 0.0f); glVertex3f(x2, y1, z2);
    glTexCoord2f(repX, repY); glVertex3f(x2, y2, z2);
    glTexCoord2f(0.0f, repY); glVertex3f(x1, y2, z1);
    glEnd();
}

void aplicarTexturaNoChao(float x1, float y1, float z1, float x2, float y2, float z2, float repX, float repZ) {
    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y1, z1);
    glTexCoord2f(repX, 0.0f); glVertex3f(x2, y1, z1);
    glTexCoord2f(repX, repZ); glVertex3f(x2, y2, z2);
    glTexCoord2f(0.0f, repZ); glVertex3f(x1, y2, z2);
    glEnd();
}

void desenharParedesComTextura() {
    if (texturasAtivas && textureIDParede != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureIDParede);
        glColor3f(1.0f, 1.0f, 1.0f);
        
        // parede tela (frontal) - tipo 0
        aplicarTexturaNaParede(-16, 0, -24, 16, 9, -24, 4.0f, 3.0f, 0);
        // parede esquerda - tipo 1
        aplicarTexturaNaParede(-16, 0, -24, -16, 9, 28, 8.0f, 3.0f, 1);
        // parede direita - tipo 2
        aplicarTexturaNaParede(16, 0, -24, 16, 9, 28, 8.0f, 3.0f, 2);
        // parede tras - tipo 3
        aplicarTexturaNaParede(-16, 0, 28, 16, 9, 28, 4.0f, 3.0f, 3);
        
        glDisable(GL_TEXTURE_2D);
    } else {
        desenharParedes();
    }
}

void desenharChaoComTextura() {
    if (texturasAtivas && textureIDChao != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureIDChao);
        glColor3f(1.0f, 1.0f, 1.0f);
       
        aplicarTexturaNoChao(-16, -0.3, -24, 16, -0.3, 28, 8.0f, 13.0f);
        
        glDisable(GL_TEXTURE_2D);
    } else {
        desenharChao();
    }
}