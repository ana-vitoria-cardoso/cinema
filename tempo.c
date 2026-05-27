// Ana Vitória Cardoso Queiroz - 172706
// Maria Clara Muharem - 266864

#include "tempo.h"
#include <GL/glut.h>

static int tempo_inicial = 0;
static int tempo_anterior = 0;

void tempo_iniciar(void)
{
    tempo_inicial = glutGet(GLUT_ELAPSED_TIME);
    tempo_anterior = tempo_inicial;
}

float tempo_get_segundos(void)
{
    int agora = glutGet(GLUT_ELAPSED_TIME);
    return (agora - tempo_inicial) / 1000.0f;
}

float tempo_get_delta(void)
{
    int agora = glutGet(GLUT_ELAPSED_TIME);
    float delta = (agora - tempo_anterior) / 1000.0f;
    tempo_anterior = agora;
    if(delta > 0.033) delta = 0.033; // limita para não pular muito
    return delta;
}
