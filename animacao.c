// Ana Vitória Cardoso Queiroz - 172706
// Maria Clara Muharem - 266864

#include "animacao.h"
#include "camera.h"
#include "tempo.h"
#include "obj.h"
#include <math.h>
#include <GL/glut.h>

static EstadoAnimacao estado = ANIM_ABRINDO_PORTA;
static float tempo_estado = 0.0;
static int ativa = 1;

static float inicioX = -11.0, inicioZ = 28.0;
static float inicioY = 2.0;
static float cadeiraX = 3.5, cadeiraZ = -3.0;
static float cadeiraY = 0.5;
static float telaZ = -23.0;

// var de caminho
static float etapaX = 0.0;
static float etapaZ = 0.0;

static float lerp(float a, float b, float t)
{
    t = t * t * (3.0 - 2.0 * t);
    return a + (b - a) * t;
}

void animacao_iniciar(void)
{
    estado = ANIM_ABRINDO_PORTA;
    tempo_estado = 0.0;
    ativa = 1;
    tempo_iniciar();

    camera_set_posicao(inicioX, inicioY, inicioZ);
    camera_set_alvo(inicioX, inicioY, inicioZ - 5);
}

void animacao_atualizar(void)
{
    if (!ativa)
        return;

    float delta = tempo_get_delta();
    tempo_estado += delta;

    switch (estado)
    {
    case ANIM_ABRINDO_PORTA:
    {
        float t = tempo_estado / 2.0;
        if (t >= 1.0) t = 1.0;
        camera_set_posicao(inicioX, inicioY, inicioZ);
        camera_set_alvo(inicioX, inicioY, inicioZ - 5);
        if (t >= 1.0)
        {
            estado = ANIM_INICIO;
            tempo_estado = 0.0;
        }
        break;
    }

    case ANIM_INICIO:
        if (tempo_estado >= 0.5)
        {
            estado = ANIM_ANDANDO_Z;
            tempo_estado = 0.0;
            etapaZ = inicioZ;
            etapaX = inicioX;
        }
        break;

    case ANIM_ANDANDO_Z:
    {
        float t = tempo_estado / 8.0;
        if (t >= 1.0) t = 1.0;

        float z = lerp(inicioZ, -4.5, t);
        float x = inicioX;
        float y = inicioY + sin(t * 3.14159) * 0.03;

        camera_set_posicao(x, y, z);

        float alvoX = inicioX;
        float alvoY = inicioY;
        float alvoZ = z - 5.0;
        if (t > 0.7)
        {
            float tGiro = (t - 0.7) / 0.3;
            alvoX = lerp(inicioX, cadeiraX, tGiro);
            alvoY = lerp(inicioY, cadeiraY + 0.3, tGiro);
            alvoZ = lerp(z - 5.0, cadeiraZ, tGiro);
        }
        camera_set_alvo(alvoX, alvoY, alvoZ);
        if (t >= 1.0)
        {
            estado = ANIM_ANDANDO_X;
            tempo_estado = 0.0;
        }
        break;
    }

    case ANIM_ANDANDO_X:
    {
        static int primeiraVezX = 1;
        if (primeiraVezX)
        {
            etapaX = camera_get_camX();
            primeiraVezX = 0;
        }

        float t = tempo_estado / 6.0;
        if (t >= 1.0) t = 1.0;

        float x = lerp(etapaX, cadeiraX, t);
        float z = -8.0;
        float y = inicioY + sin(t * 3.14159) * 0.03;

        camera_set_posicao(x, y, z);
        camera_set_alvo(cadeiraX, cadeiraY + 0.3, cadeiraZ);

        if (t >= 1.0)
        {
            primeiraVezX = 1;
            estado = ANIM_OLHANDO_LATERAL;
            tempo_estado = 0.0;
        }
        break;
    }

    case ANIM_OLHANDO_LATERAL:
    {
        static int primeiraVezLateral = 1;
        static float oX = 0.0, oZ = 0.0;

        if (primeiraVezLateral)
        {
            oX = camera_get_camX();
            oZ = camera_get_camZ();
            primeiraVezLateral = 0;
        }

        float t = tempo_estado / 2.0;
        if (t >= 1.0) t = 1.0;
        float xFinal = cadeiraX - 2.5;
        float zFinal = cadeiraZ - 0.14;
        float x = lerp(oX, xFinal, t);
        float z = lerp(oZ, zFinal, t);
        camera_set_posicao(x, inicioY, z);
        camera_set_alvo(cadeiraX, cadeiraY + 0.6, cadeiraZ);
        if (t >= 1.0)
        {
            primeiraVezLateral = 1;
            estado = ANIM_SENTANDO;
            tempo_estado = 0.0;
        }
        break;
    }

    case ANIM_SENTANDO:
    {
        float t = tempo_estado / 2.0;
        if (t >= 1.0) t = 1.0;
        float xLateral = cadeiraX - 2.5;
        float zLateral = cadeiraZ - 0.14;
        camera_set_posicao(xLateral, inicioY, zLateral);
        camera_set_alvo(cadeiraX, cadeiraY + 0.6, cadeiraZ);
        if (t >= 1.0)
        {
            estado = ANIM_IR_PARA_FRENTE;
            tempo_estado = 0.0;
        }
        break;
    }

    case ANIM_IR_PARA_FRENTE:
    {
        static int primeiraVezFrente = 1;
        static float oX = 0.0, oZ = 0.0;
        if (primeiraVezFrente)
        {
            oX = camera_get_camX();
            oZ = camera_get_camZ();
            primeiraVezFrente = 0;
        }
        float t = tempo_estado / 2.5;
        if (t >= 1.0) t = 1.0;
        float xFinal = cadeiraX;
        float zFinal = cadeiraZ + 2.0;
        float x = lerp(oX, xFinal, t);
        float z = lerp(oZ, zFinal, t);
        float y = inicioY + sin(t * 3.14159) * 0.02;
        camera_set_posicao(x, y, z);
        float alvoX = lerp(cadeiraX, 0.0, t);
        float alvoY = lerp(cadeiraY + 0.6, 1.5, t);
        float alvoZ = lerp(cadeiraZ, telaZ, t);
        camera_set_alvo(alvoX, alvoY, alvoZ);
        if (t >= 1.0)
        {
            primeiraVezFrente = 1;
            estado = ANIM_SENTANDO_CAMERA;
            tempo_estado = 0.0;
        }
        break;
    }

    case ANIM_SENTANDO_CAMERA:
    {
        static int primeiraVezS = 1;
        static float oX = 0.0, oZ = 0.0, oY = 0.0;

        if (primeiraVezS)
        {
            oX = camera_get_camX();
            oZ = camera_get_camZ();
            oY = camera_get_camY();
            primeiraVezS = 0;
        }
        float t = tempo_estado / 2.0;
        if (t >= 1.0) t = 1.0;
        float x = lerp(oX, cadeiraX, t);
        float z = lerp(oZ, cadeiraZ + 0.2, t);
        float y = lerp(oY, cadeiraY + 0.85, t);
        camera_set_posicao(x, y, z);
        camera_set_alvo(0.0, 1.5, telaZ);
        if (t >= 1.0)
        {
            primeiraVezS = 1;
            estado = ANIM_OLHANDO_TELA;
            tempo_estado = 0.0;
        }
        break;
    }

    case ANIM_OLHANDO_TELA:
    {
        float t = tempo_estado / 0.5;
        if (t >= 1.0) t = 1.0;
        camera_set_posicao(cadeiraX, cadeiraY + 0.85, cadeiraZ + 0.2);
        camera_set_alvo(0, 1.5, telaZ);
        if (t >= 1.0)
        {
            estado = ANIM_ESCURECENDO;
            tempo_estado = 0.0;
        }
        break;
    }

    case ANIM_ESCURECENDO:
    {
        float t = tempo_estado / 3.0;
        if (t >= 1.0) t = 1.0;
        if (t >= 1.0)
        {
            estado = ANIM_PROJETANDO;
            tempo_estado = 0.0;
        }
        break;
    }

    // "Studio Maria Ana Apresenta" — dura 4s (3s visivel + 1s fade out)
    case ANIM_PROJETANDO:
    {
        if (tempo_estado >= 4.0)
        {
            estado = ANIM_CREDITOS;
            tempo_estado = 0.0;
        }
        break;
    }

    // "Projeto final de Computacao Grafica..." — dura 5s
    case ANIM_CREDITOS:
    {
        if (tempo_estado >= 5.0)
        {
            estado = ANIM_TITULO_FORMAS;
            tempo_estado = 0.0;
        }
        break;
    }

    // "Formas Geometricas" — dura 3s
    case ANIM_TITULO_FORMAS:
    {
        if (tempo_estado >= 3.0)
        {
            estado = ANIM_FORMAS;
            tempo_estado = 0.0;
        }
        break;
    }

    // animacao de formas geometricas — roda ate ANIM_FIM
    case ANIM_FORMAS:
    {
        if (tempo_estado >= 12.0)
        {
            estado = ANIM_FIM_TEXTO;
            tempo_estado = 0.0;
        }
        break;
    }

    // "Fim! Obrigada!" — dura 5s
    case ANIM_FIM_TEXTO:
    {
        if (tempo_estado >= 5.0)
        {
            estado = ANIM_APAGANDO;
            tempo_estado = 0.0;
        }
        break;
    }

    // camera vai para frente da tela, tela apaga — dura 3s
    case ANIM_APAGANDO:
    {
        static int primeiraVezApagando = 1;
        static float oX = 0.0, oY = 0.0, oZ = 0.0;

        if (primeiraVezApagando)
        {
            oX = camera_get_camX();
            oY = camera_get_camY();
            oZ = camera_get_camZ();
            primeiraVezApagando = 0;
        }

        float t = tempo_estado / 3.0;
        if (t >= 1.0) t = 1.0;

        float x = lerp(oX, 0.0, t);
        float y = lerp(oY, 5.0, t);
        float z = lerp(oZ, 0.0, t);
        camera_set_posicao(x, y, z);
        camera_set_alvo(0.0, 5.0, -23.0);

        if (t >= 1.0)
        {
            primeiraVezApagando = 1;
            estado = ANIM_FIM;
            tempo_estado = 0.0;
        }
        break;
    }

    case ANIM_FIM:
        break;
    }
}

int animacao_esta_ativa(void)
{
    return ativa && (estado != ANIM_FIM);
}

EstadoAnimacao animacao_get_estado(void)
{
    return estado;
}

float animacao_get_intensidade_luz(void)
{
    if (estado == ANIM_APAGANDO)
    {
        float t = tempo_estado / 3.0;
        if (t > 1.0) t = 1.0;
        return t * 0.05; // sala continua quase escura
    }
    if (estado >= ANIM_ESCURECENDO && estado != ANIM_FIM)
    {
        float t = (estado == ANIM_ESCURECENDO) ? tempo_estado / 3.0 : 1.0;
        if (t > 1.0) t = 1.0;
        return 1.0 - (t * 0.95);
    }
    return 1.0;
}

float animacao_get_escurecimento(void)
{
    if (estado == ANIM_APAGANDO)
        return 0.95; // tela permanece escura enquanto camera se move
    if (estado >= ANIM_ESCURECENDO && estado != ANIM_FIM)
    {
        float t = (estado == ANIM_ESCURECENDO) ? tempo_estado / 3.0 : 1.0;
        if (t > 1.0) t = 1.0;
        return t * 0.95;
    }
    return 0.0;
}

// "Studio Maria Ana Apresenta" — fade in nos primeiros 1s, visivel ate 3s, fade out ate 4s
int animacao_get_texto_visivel(void)
{
    if (estado == ANIM_PROJETANDO && tempo_estado < 4.0)
        return 1;
    return 0;
}

// fade in/out generico para textos da tela (0.0 a 1.0)
float animacao_get_alpha_tela(void)
{
    float FADE = 0.8f; // segundos de fade

    if (estado == ANIM_PROJETANDO)
    {
        if (tempo_estado < FADE) return tempo_estado / FADE;
        if (tempo_estado < 3.0f) return 1.0f;
        return 1.0f - (tempo_estado - 3.0f) / FADE;
    }
    if (estado == ANIM_CREDITOS)
    {
        if (tempo_estado < FADE) return tempo_estado / FADE;
        if (tempo_estado < 4.0f) return 1.0f;
        return 1.0f - (tempo_estado - 4.0f) / FADE;
    }
    if (estado == ANIM_TITULO_FORMAS)
    {
        if (tempo_estado < FADE) return tempo_estado / FADE;
        if (tempo_estado < 2.2f) return 1.0f;
        return 1.0f - (tempo_estado - 2.2f) / FADE;
    }
    if (estado == ANIM_FIM_TEXTO)
    {
        if (tempo_estado < FADE) return tempo_estado / FADE;
        if (tempo_estado < 4.0f) return 1.0f;
        return 1.0f - (tempo_estado - 4.0f) / FADE;
    }
    return 1.0f;
}

int animacao_get_creditos_visivel(void)
{
    return (estado == ANIM_CREDITOS);
}

int animacao_get_titulo_formas_visivel(void)
{
    return (estado == ANIM_TITULO_FORMAS);
}

int animacao_get_formas_visivel(void)
{
    return (estado == ANIM_FORMAS);
}

float animacao_get_tempo_formas(void)
{
    if (estado == ANIM_FORMAS)
        return tempo_estado;
    return 0.0f;
}

float animacao_get_inclinacao_cadeira(void)
{
    if (estado == ANIM_SENTANDO)
    {
        float t = tempo_estado / 2.0;
        if (t > 1.0) t = 1.0;
        return t * 18.0;
    }
    if (estado > ANIM_SENTANDO && estado <= ANIM_FIM)
        return 18.0;
    return 0.0;
}

float animacao_get_abertura_porta(void)
{
    if (estado == ANIM_ABRINDO_PORTA)
    {
        float t = tempo_estado / 1.5;
        if (t > 1.0) t = 1.0;
        return t * 90.0;
    }
    if (estado > ANIM_ABRINDO_PORTA)
        return 90.0;
    return 0.0;
}

int animacao_get_fim_visivel(void)
{
    return (estado == ANIM_FIM_TEXTO);
}

float animacao_get_alpha_apagando(void)
{
    if (estado == ANIM_APAGANDO)
    {
        float t = tempo_estado / 3.0;
        if (t > 1.0) t = 1.0;
        return t; // 0.0 = visivel, 1.0 = tela totalmente preta
    }
    if (estado == ANIM_FIM)
        return 1.0;
    return 0.0;
}
