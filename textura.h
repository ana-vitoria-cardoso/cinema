// Ana Vitória Cardoso Queiroz - 172706
// Maria Clara Muharem - 266864

#ifndef TEXTURA_H
#define TEXTURA_H

#include <GL/glut.h>

extern GLuint textureIDParede;
extern GLuint textureIDChao;
extern int texturasAtivas;

void carregarTexturas();
void toggleTexturas();
void aplicarTexturaNaParede(float x1, float y1, float z1, float x2, float y2, float z2, float repX, float repY, int paredeTipo);
void aplicarTexturaNoChao(float x1, float y1, float z1, float x2, float y2, float z2, float repX, float repZ);
void desenharParedesComTextura();
void desenharChaoComTextura();

#endif