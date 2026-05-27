// Ana Vitória Cardoso Queiroz - 172706
// Maria Clara Muharem - 266864
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
void camera_set_posicao(float x, float y, float z);
void camera_set_alvo(float x, float y, float z);
void camera_get_posicao(float *x, float *y, float *z);
float camera_get_camX(void);
float camera_get_camY(void);
float camera_get_camZ(void);
float camera_get_lookX(void);
float camera_get_lookY(void);
float camera_get_lookZ(void);

#endif