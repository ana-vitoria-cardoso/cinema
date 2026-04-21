#ifndef CAMERA_H
#define CAMERA_H

//camera
extern float anguloY;
extern float anguloX;
extern float distancia;

//funcoes
void initCamera();
void controlarCameraTeclado(unsigned char key, int x, int y);
void controlarCameraEspecial(int key, int x, int y);
void controlarCameraMouse(int button, int state, int x, int y);
void controlarCameraMotion(int x, int y);

#endif