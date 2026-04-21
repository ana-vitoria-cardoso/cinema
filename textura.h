#ifndef TEXTURA_H
#define TEXTURA_H

#include <GL/glut.h>
extern GLuint textureIDParede;
extern GLuint textureIDChao;

//controle
extern int texturasAtivas;
void carregarTexturas();
void desenharParedesComTextura();
void desenharChaoComTextura();
void aplicarTexturaNaParede(float x1, float y1, float z1, float x2, float y2, float z2, float repX, float repY);
void aplicarTexturaNoChao(float x1, float y1, float z1, float x2, float y2, float z2, float repX, float repZ);
void toggleTexturas();//controle

#endif