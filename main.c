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

// matrizes da câmera capturadas logo após gluLookAt, antes de desenhar os objetos
static GLdouble g_modelview[16];
static GLdouble g_projection[16];
static GLint g_viewport[4];

// utilitários de texto 2D 

static void renderizarTexto(const char *texto, void *fonte, float x, float y)
{
    glRasterPos2f(x, y);
    for (const char *c = texto; *c != '\0'; c++)
        glutBitmapCharacter(fonte, *c);
}

// largura aproximada de uma string em pixels com a fonte dada
static float larguraTexto(const char *texto, void *fonte)
{
    float w = 0;
    for (const char *c = texto; *c != '\0'; c++)
        w += glutBitmapWidth(fonte, *c);
    return w;
}

// entra em modo ortogonal 1024x768, desliga iluminação/profundidade
static void iniciarOrtho(void)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    // usa o tamanho real da janela para coincidir com as coordenadas do gluProject
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

static void encerrarOrtho(void)
{
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

// projetar ponto 3D da tela do cinema para coordenadas 2D da janela 
static void projetarPontoTela(float x3, float y3, float z3,
                              double *ox, double *oy)
{
    GLdouble wz;
    gluProject(x3, y3, z3, g_modelview, g_projection, g_viewport, ox, oy, &wz);
}

static void calcCantosTelaProjetados(double px[4], double py[4])
{
    // face frontal da tela branca: z = -23.275
    // x: +-5.75 (borda real da superficie branca)
    // y_base: 1.25   y_topo: 8.35 (recuado para ficar abaixo da barra superior)
    const float z = -23.275f;
    const float x1 = -5.75f, x2 = 5.75f;
    const float yB = 1.25f, yT = 8.35f;
    projetarPontoTela(x1, yB, z, &px[0], &py[0]); // BL
    projetarPontoTela(x2, yB, z, &px[1], &py[1]); // BR
    projetarPontoTela(x2, yT, z, &px[2], &py[2]); // TR
    projetarPontoTela(x1, yT, z, &px[3], &py[3]); // TL
}

// Mantida para compatibilidade com desenharTextoTela 
static void calcBBoxTela(float *out_tx1, float *out_ty1,
                         float *out_tx2, float *out_ty2,
                         float *out_cx, float *out_cy)
{
    double px[4], py[4];
    calcCantosTelaProjetados(px, py);

    float tx1 = (float)px[0], tx2 = (float)px[0];
    float ty1 = (float)py[0], ty2 = (float)py[0];
    for (int i = 1; i < 4; i++)
    {
        if ((float)px[i] < tx1)
            tx1 = (float)px[i];
        if ((float)px[i] > tx2)
            tx2 = (float)px[i];
        if ((float)py[i] < ty1)
            ty1 = (float)py[i];
        if ((float)py[i] > ty2)
            ty2 = (float)py[i];
    }
    *out_tx1 = tx1;
    *out_ty1 = ty1;
    *out_tx2 = tx2;
    *out_ty2 = ty2;
    *out_cx = (tx1 + tx2) * 0.5f;
    *out_cy = (ty1 + ty2) * 0.5f;
}

// desenhar textos com fade e centralização 

static void desenharTextoTela(const char *linha1, const char *linha2, float alpha,
                              float r, float g, float b)
{
    float tx1, ty1, tx2, ty2, cx, cy;
    calcBBoxTela(&tx1, &ty1, &tx2, &ty2, &cx, &cy);

    glEnable(GL_SCISSOR_TEST);
    glScissor((int)tx1, (int)ty1, (int)(tx2 - tx1), (int)(ty2 - ty1));
    iniciarOrtho();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    void *fonte = GLUT_BITMAP_HELVETICA_18;
    float w1 = larguraTexto(linha1, fonte);
    glColor4f(r, g, b, alpha);
    renderizarTexto(linha1, fonte, cx - w1 * 0.5f, cy + 18.0f);
    if (linha2 && linha2[0] != '\0')
    {
        float w2 = larguraTexto(linha2, fonte);
        glColor4f(r, g, b, alpha * 0.9f);
        renderizarTexto(linha2, fonte, cx - w2 * 0.5f, cy - 14.0f);
    }

    glDisable(GL_BLEND);
    encerrarOrtho();
    glDisable(GL_SCISSOR_TEST);
}

// animação de fundo do mar na tela
static void circulo(float cx, float cy, float r, int n)
{
    float PI = 3.14159f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= n; i++)
    {
        float a = i * 2.0f * PI / n;
        glVertex2f(cx + cosf(a) * r, cy + sinf(a) * r);
    }
    glEnd();
}

// alga 
static void desenharAlga(float bx, float by, float altura, float fase, float alpha)
{
    float PI = 3.14159f;
    int segs = 14;
    float largura = altura * 0.07f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(largura < 3 ? 3 : largura);
    glColor4f(0.10f, 0.65f, 0.25f, alpha * 0.85f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= segs; i++)
    {
        float frac = (float)i / segs;
        float ondX = sinf(frac * PI * 2.0f + fase) * (altura * 0.08f) * frac;
        glVertex2f(bx + ondX, by + frac * altura);
    }
    glEnd();
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
}

// peixe 
static void desenharPeixe(float cx, float cy, float escala, float r, float g, float b, float alpha, int virandoEsq)
{
    float PI = 3.14159f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float dir = virandoEsq ? -1.0f : 1.0f;

    // corpo 
    glColor4f(r, g, b, alpha);
    int n = 32;
    float rx = escala * 1.0f;
    float ry = escala * 0.45f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= n; i++)
    {
        float a = i * 2.0f * PI / n;
        glVertex2f(cx + cosf(a) * rx * dir, cy + sinf(a) * ry);
    }
    glEnd();

    // cauda (triângulo)
    glColor4f(r * 0.7f, g * 0.7f, b * 0.7f, alpha);
    glBegin(GL_TRIANGLES);
    glVertex2f(cx - dir * rx * 0.8f, cy);
    glVertex2f(cx - dir * rx * 1.55f, cy + ry * 0.85f);
    glVertex2f(cx - dir * rx * 1.55f, cy - ry * 0.85f);
    glEnd();

    // barbatana dorsal
    glColor4f(r * 0.75f, g * 0.75f, b * 0.75f, alpha * 0.9f);
    glBegin(GL_TRIANGLES);
    glVertex2f(cx - dir * rx * 0.1f, cy + ry);
    glVertex2f(cx + dir * rx * 0.5f, cy + ry * 1.5f);
    glVertex2f(cx + dir * rx * 0.7f, cy + ry);
    glEnd();

    // olho
    glColor4f(0.0f, 0.0f, 0.0f, alpha);
    circulo(cx + dir * rx * 0.55f, cy + ry * 0.2f, escala * 0.1f, 12);
    glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.7f);
    circulo(cx + dir * rx * 0.57f, cy + ry * 0.22f, escala * 0.04f, 8);

    glDisable(GL_BLEND);
}

// estrela do mar 
static void desenharEstrelaMar(float cx, float cy, float raio, float rot, float alpha)
{
    float PI = 3.14159f;
    int pontas = 5;
    float raioInt = raio * 0.38f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // corpo principal
    glColor4f(0.90f, 0.38f, 0.10f, alpha);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= pontas * 2; i++)
    {
        float a = i * PI / pontas + rot;
        float r2 = (i % 2 == 0) ? raio : raioInt;
        glVertex2f(cx + cosf(a) * r2, cy + sinf(a) * r2);
    }
    glEnd();
    // manchas de textura
    glColor4f(0.75f, 0.25f, 0.05f, alpha * 0.7f);
    for (int p = 0; p < pontas; p++)
    {
        float a = p * 2.0f * PI / pontas + rot;
        float bx = cx + cosf(a) * raio * 0.55f;
        float by = cy + sinf(a) * raio * 0.55f;
        circulo(bx, by, raio * 0.1f, 8);
    }
    glDisable(GL_BLEND);
}

// bolha 
static void desenharBolha(float cx, float cy, float r, float alpha)
{
    float PI = 3.14159f;
    int n = 20;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // anel
    glColor4f(0.75f, 0.92f, 1.0f, alpha * 0.55f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < n; i++)
    {
        float a = i * 2.0f * PI / n;
        glVertex2f(cx + cosf(a) * r, cy + sinf(a) * r);
    }
    glEnd();
    glLineWidth(1.0f);
    // brilho interno
    glColor4f(0.9f, 0.97f, 1.0f, alpha * 0.25f);
    circulo(cx - r * 0.25f, cy + r * 0.25f, r * 0.35f, 10);
    glDisable(GL_BLEND);
}

// medusa 
static void desenharMedusa(float cx, float cy, float raio, float t, float fase, float alpha)
{
    float PI = 3.14159f;
    int n = 30;
    // pulso: dôme expande e contrai
    float pulso = 0.85f + 0.15f * sinf(t * 2.5f + fase);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // dome (semiesfera superior)
    float rx = raio * pulso;
    float ry = raio * 0.65f * pulso;
    glColor4f(0.85f, 0.35f, 0.90f, alpha * 0.55f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= n; i++)
    {
        float a = i * PI / n; 
        glVertex2f(cx + cosf(a) * rx, cy + sinf(a) * ry);
    }
    glEnd();

    // brilho interno
    glColor4f(1.0f, 0.7f, 1.0f, alpha * 0.25f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy + ry * 0.2f);
    for (int i = 0; i <= 16; i++)
    {
        float a = i * PI / 16;
        glVertex2f(cx + cosf(a) * rx * 0.5f, cy + sinf(a) * ry * 0.5f);
    }
    glEnd();

    // tentáculos 
    glColor4f(0.75f, 0.25f, 0.85f, alpha * 0.55f);
    glLineWidth(1.5f);
    int nTent = 7;
    for (int k = 0; k < nTent; k++)
    {
        float tx0 = cx + cosf(PI + k * PI / (nTent - 1)) * rx * 0.7f;
        float ty0 = cy; 
        float comprimento = raio * (1.0f + 0.4f * sinf(t * 1.5f + k));
        glBegin(GL_LINE_STRIP);
        int segs = 10;
        for (int s = 0; s <= segs; s++)
        {
            float frac = (float)s / segs;
            float ondX = sinf(frac * PI * 2.5f + t * 2.0f + k * 0.8f) * raio * 0.18f * frac;
            glVertex2f(tx0 + ondX, ty0 - frac * comprimento);
        }
        glEnd();
    }
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
}

// caranguejo 
static void desenharCaranguejo(float cx, float cy, float escala, float t, float alpha)
{
    float PI = 3.14159f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float r = 0.88f, g = 0.22f, b = 0.10f;

    // corpo oval
    glColor4f(r, g, b, alpha);
    int n = 24;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= n; i++)
    {
        float a = i * 2.0f * PI / n;
        glVertex2f(cx + cosf(a) * escala * 1.1f, cy + sinf(a) * escala * 0.7f);
    }
    glEnd();

    // carapaça 
    glColor4f(r * 0.7f, g * 0.7f, b * 0.7f, alpha * 0.7f);
    glLineWidth(1.2f);
    for (int k = -1; k <= 1; k++)
    {
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 10; i++)
        {
            float frac = i / 10.0f;
            float ax = cx + (frac - 0.5f) * escala * 1.8f;
            float ay = cy + sinf(frac * PI) * escala * 0.4f + k * escala * 0.22f;
            glVertex2f(ax, ay);
        }
        glEnd();
    }
    glLineWidth(1.0f);

    // olhos 
    for (int e = -1; e <= 1; e += 2)
    {
        float ox = cx + e * escala * 0.4f;
        float oy = cy + escala * 0.65f;
        glColor4f(r * 0.8f, g * 0.8f, b * 0.8f, alpha);
        glBegin(GL_LINES);
        glVertex2f(ox, oy);
        glVertex2f(ox + e * escala * 0.15f, oy + escala * 0.3f);
        glEnd();
        glColor4f(0.05f, 0.05f, 0.05f, alpha);
        circulo(ox + e * escala * 0.15f, oy + escala * 0.3f, escala * 0.1f, 8);
    }

    // 3 pernas de cada lado
    for (int side = -1; side <= 1; side += 2)
    {
        for (int k = 0; k < 3; k++)
        {
            float baseX = cx + side * escala * 0.9f;
            float baseY = cy - escala * 0.2f + k * escala * 0.3f - escala * 0.3f;
            float angBase = side * (0.4f + k * 0.15f);
            float swing = sinf(t * 3.0f + k * 1.0f) * 0.12f * side;
            float endX = baseX + side * escala * 0.9f * cosf(angBase + swing);
            float endY = baseY - escala * 0.4f + sinf(angBase + swing) * escala * 0.3f;
            glColor4f(r * 0.85f, g * 0.85f, b * 0.85f, alpha * 0.9f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_STRIP);
            glVertex2f(baseX, baseY);
            // joelho
            float midX = (baseX + endX) * 0.5f + side * escala * 0.3f;
            float midY = (baseY + endY) * 0.5f + escala * 0.15f;
            glVertex2f(midX, midY);
            glVertex2f(endX, endY);
            glEnd();
        }
    }

    // pinças (maiores, na frente)
    for (int side = -1; side <= 1; side += 2)
    {
        float bx = cx + side * escala * 1.0f;
        float by = cy + escala * 0.1f;
        float swing = sinf(t * 2.2f + side) * 0.15f;
        float ex = bx + side * escala * 0.85f;
        float ey = by + escala * 0.45f + swing * escala;
        glColor4f(r, g * 0.5f, b * 0.5f, alpha);
        glLineWidth(2.5f);
        glBegin(GL_LINES);
        glVertex2f(bx, by);
        glVertex2f(ex, ey);
        glEnd();
        // garra
        circulo(ex, ey, escala * 0.22f, 10);
    }
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
}

// tartaruga 
static void desenharTartaruga(float cx, float cy, float escala, float t, float fase, int virandoEsq, float alpha)
{
    float PI = 3.14159f;
    float dir = virandoEsq ? -1.0f : 1.0f;
    // nado suave: oscila levemente para cima e baixo
    float swim = sinf(t * 1.8f + fase) * escala * 0.05f;
    cy += swim;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // casco 
    float rx = escala * 1.0f;
    float ry = escala * 0.7f;
    glColor4f(0.15f, 0.50f, 0.20f, alpha);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    int n = 28;
    for (int i = 0; i <= n; i++)
    {
        float a = i * 2.0f * PI / n;
        glVertex2f(cx + cosf(a) * rx * dir, cy + sinf(a) * ry);
    }
    glEnd();

    // padrão hexagonal do casco
    glColor4f(0.10f, 0.38f, 0.15f, alpha * 0.8f);
    glLineWidth(1.2f);
    for (int row = -1; row <= 1; row++)
    {
        for (int col = -1; col <= 1; col++)
        {
            float hx = cx + col * rx * 0.55f * dir;
            float hy = cy + row * ry * 0.5f + (col % 2) * ry * 0.25f;
            float hr = escala * 0.22f;
            glBegin(GL_LINE_LOOP);
            for (int k = 0; k < 6; k++)
            {
                float a = k * PI / 3.0f;
                glVertex2f(hx + cosf(a) * hr, hy + sinf(a) * hr * 0.75f);
            }
            glEnd();
        }
    }
    glLineWidth(1.0f);

    // cabeça
    float hx = cx + dir * rx * 1.0f;
    float hy = cy + ry * 0.05f;
    glColor4f(0.20f, 0.55f, 0.25f, alpha);
    circulo(hx, hy, escala * 0.28f, 14);
    // olho
    glColor4f(0.05f, 0.05f, 0.05f, alpha);
    circulo(hx + dir * escala * 0.14f, hy + escala * 0.08f, escala * 0.07f, 8);

    // 4 nadadeiras
    float flapFrente = sinf(t * 2.0f + fase) * 0.25f;
    float flapTras = sinf(t * 2.0f + fase + PI) * 0.2f;
    // frente-cima, frente-baixo, trás-cima, trás-baixo
    float nadadeiras[4][4] = {
        {cx + dir * rx * 0.4f, cy + ry * 0.55f, dir * 0.6f, 0.9f + flapFrente},
        {cx + dir * rx * 0.4f, cy - ry * 0.55f, dir * 0.6f, -0.9f - flapFrente},
        {cx - dir * rx * 0.5f, cy + ry * 0.45f, -dir * 0.5f, 0.8f + flapTras},
        {cx - dir * rx * 0.5f, cy - ry * 0.45f, -dir * 0.5f, -0.8f - flapTras},
    };
    glColor4f(0.18f, 0.52f, 0.22f, alpha * 0.9f);
    for (int k = 0; k < 4; k++)
    {
        float bx = nadadeiras[k][0], by = nadadeiras[k][1];
        float ex = bx + nadadeiras[k][2] * escala * 0.7f;
        float ey = by + nadadeiras[k][3] * escala * 0.35f;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(bx, by);
        glVertex2f(ex - escala * 0.15f, ey);
        glVertex2f(ex, ey);
        glVertex2f(ex + escala * 0.1f, ey - nadadeiras[k][3] * escala * 0.15f);
        glEnd();
    }

    glDisable(GL_BLEND);
}

// cena completa do fundo do mar 
static void desenharFundoMar(float t)
{
    // Projeta os 4 cantos EXATOS da superficie branca da tela do cinema.
    double px[4], py[4];
    calcCantosTelaProjetados(px, py);

    // Bounding box apenas para o glScissor grosseiro (otimizacao de fill rate)
    float tx1 = (float)px[0], tx2 = (float)px[0];
    float ty1 = (float)py[0], ty2 = (float)py[0];
    for (int i = 1; i < 4; i++)
    {
        if ((float)px[i] < tx1)
            tx1 = (float)px[i];
        if ((float)px[i] > tx2)
            tx2 = (float)px[i];
        if ((float)py[i] < ty1)
            ty1 = (float)py[i];
        if ((float)py[i] > ty2)
            ty2 = (float)py[i];
    }
    float W = tx2 - tx1;
    float H = ty2 - ty1;
    float cx = (tx1 + tx2) * 0.5f;
    float cy = (ty1 + ty2) * 0.5f;

    // Scissor grosseiro (evita processar pixels muito alem da tela)
    glEnable(GL_SCISSOR_TEST);
    glScissor((int)(tx1 - 2), (int)(ty1 - 2), (int)(W + 4), (int)(H + 4));

    // Escrever a mascara exata no stencil 
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    iniciarOrtho();
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(px[0], py[0]); // BL
    glVertex2d(px[1], py[1]); // BR
    glVertex2d(px[2], py[2]); // TR
    glVertex2d(px[3], py[3]); // TL
    glEnd();
    encerrarOrtho();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    iniciarOrtho();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float PI = 3.14159f;

    // fundo sempre presente: oceano + luz + areia + algas + pedras 
    // gradiente oceano
    glBegin(GL_QUADS);
    glColor4f(0.01f, 0.10f, 0.35f, 1.0f);
    glVertex2f(tx1, ty1);
    glColor4f(0.01f, 0.10f, 0.35f, 1.0f);
    glVertex2f(tx2, ty1);
    glColor4f(0.05f, 0.40f, 0.75f, 1.0f);
    glVertex2f(tx2, ty2);
    glColor4f(0.05f, 0.40f, 0.75f, 1.0f);
    glVertex2f(tx1, ty2);
    glEnd();

    // raios de luz solares
    {
        float brilhoT = 0.5f + 0.5f * sinf(t * 0.4f);
        for (int i = 0; i < 5; i++)
        {
            float lx = tx1 + W * (0.1f + i * 0.2f + sinf(t * 0.15f + i) * 0.04f);
            float larT = W * (0.03f + i * 0.005f);
            float larB = larT * 4.5f;
            glColor4f(0.75f, 0.92f, 1.0f, 0.06f + brilhoT * 0.04f);
            glBegin(GL_TRIANGLES);
            glVertex2f(lx - larT, ty2);
            glVertex2f(lx + larT, ty2);
            glVertex2f(lx + larB, ty1 + H * 0.45f);
            glEnd();
            glBegin(GL_TRIANGLES);
            glVertex2f(lx - larT, ty2);
            glVertex2f(lx - larB, ty1 + H * 0.45f);
            glVertex2f(lx + larB, ty1 + H * 0.45f);
            glEnd();
        }
    }

    // areia
    {
        float sandH = H * 0.14f;
        glBegin(GL_QUADS);
        glColor4f(0.78f, 0.68f, 0.42f, 0.92f);
        glVertex2f(tx1, ty1);
        glColor4f(0.78f, 0.68f, 0.42f, 0.92f);
        glVertex2f(tx2, ty1);
        glColor4f(0.68f, 0.58f, 0.32f, 0.80f);
        glVertex2f(tx2, ty1 + sandH);
        glColor4f(0.68f, 0.58f, 0.32f, 0.80f);
        glVertex2f(tx1, ty1 + sandH);
        glEnd();
        glColor4f(0.60f, 0.50f, 0.28f, 0.45f);
        for (int i = 0; i < 6; i++)
        {
            float sY = ty1 + sandH * (0.35f + i * 0.11f);
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j <= 30; j++)
            {
                float sx = tx1 + W * j / 30.0f;
                float sy = sY + sinf(j * 0.6f + i * 1.1f + t * 0.2f) * (sandH * 0.06f);
                glVertex2f(sx, sy);
            }
            glEnd();
        }
    }

    // algas
    {
        float algasDados[10][3] = {
            {0.04f, 0.32f, 0.14f}, {0.10f, 0.28f, 0.11f}, {0.17f, 0.34f, 0.18f}, {0.25f, 0.30f, 0.12f}, {0.35f, 0.36f, 0.15f}, {0.58f, 0.33f, 0.13f}, {0.67f, 0.30f, 0.16f}, {0.75f, 0.35f, 0.12f}, {0.83f, 0.32f, 0.17f}, {0.92f, 0.28f, 0.11f}};
        for (int i = 0; i < 10; i++)
        {
            float ax = tx1 + W * algasDados[i][0];
            float ay = ty1 + H * 0.01f;
            float alt = H * algasDados[i][2];
            float fase = algasDados[i][1] * 10.0f + t * 1.2f;
            desenharAlga(ax, ay, alt, fase, 1.0f);
        }
    }

    // pedras
    {
        glColor4f(0.40f, 0.37f, 0.32f, 0.88f);
        float ped[5][3] = {
            {0.06f, 0.09f, 0.04f}, {0.30f, 0.08f, 0.03f}, {0.52f, 0.10f, 0.05f}, {0.71f, 0.07f, 0.03f}, {0.88f, 0.09f, 0.04f}};
        for (int i = 0; i < 5; i++)
        {
            float pedx = tx1 + W * ped[i][0];
            float pedy = ty1 + H * ped[i][1];
            float pedr = W * ped[i][2];
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(pedx, pedy);
            for (int j = 0; j <= 20; j++)
            {
                float a = j * 2.0f * PI / 20;
                glVertex2f(pedx + cosf(a) * pedr * 1.6f, pedy + sinf(a) * pedr * 0.7f);
            }
            glEnd();
        }
    }

    // estrela do mar — sempre presente no fundo
    {
        float ex = tx1 + W * 0.47f;
        float ey = ty1 + H * 0.10f;
        float er = W * 0.045f;
        float rot = 0.3f + t * 0.04f;
        desenharEstrelaMar(ex, ey, er, rot, 0.95f);
    }

    // bolhas — sempre presentes
    {
        for (int i = 0; i < 14; i++)
        {
            float bxFrac = 0.05f + (i * 0.31f + sinf(i * 2.3f) * 0.15f);
            bxFrac -= (int)bxFrac;
            float bx = tx1 + W * bxFrac;
            float periodo = 6.0f + i * 0.7f;
            float fase = i * 0.9f;
            float frac = fmodf(t / periodo + fase, 1.0f);
            float by = ty1 + H * frac;
            float raioB = W * (0.005f + (i % 4) * 0.003f);
            float alphaB = 0.5f + 0.5f * sinf(frac * PI);
            bx += sinf(t * 0.8f + i) * W * 0.012f;
            desenharBolha(bx, by, raioB, alphaB * 0.7f);
        }
    }

// CENAS — cronograma total 65s
// [0 – 5s] Cena 1: caranguejo 1 entra pela esquerda e para
// [4 – 9s] Cena 2: caranguejo 2 entra pela direita logo em seguida
// [9 – 22s] Cena 3: grupo de peixinhos passa uma única vez (esq→dir)
// [20 – 35s] Cena 4: cardume entra, agrupa e sai pela lateral direita
// [33 – 65s] Cena 5: tartarugas pelas laterais + medusas de cima para baixo

// utilidade: smoothstep local
#define SMIN(a, b, x) ((x) < (a) ? 0.0f : (x) > (b) ? 1.0f \
                                                    : (((x) - (a)) / ((b) - (a)) * (((x) - (a)) / ((b) - (a))) * (3.0f - 2.0f * (((x) - (a)) / ((b) - (a))))))

    // posição de repouso dos caranguejos 
    float escCara = W * 0.032f;
    float caraY = ty1 + H * 0.09f;
    float cara1DestinoX = tx1 + W * 0.28f;
    float cara2DestinoX = tx1 + W * 0.72f;

    // CENA 1 [0–5s]: caranguejo 1 entra pela esquerda
    // CENA 2 [4–9s]: caranguejo 2 entra pela direita logo a seguir
    // Ambos permanecem em tela até o fim

    // Caranguejo 1 — entra [0–5s]
    {
        float cara1X;
        if (t < 5.0f)
        {
            float s = SMIN(0.0f, 5.0f, t);
            cara1X = (tx1 - escCara * 2.0f) + (cara1DestinoX - (tx1 - escCara * 2.0f)) * s;
        }
        else
            cara1X = cara1DestinoX;
        float alphaC1 = SMIN(0.0f, 1.5f, t);
        desenharCaranguejo(cara1X, caraY, escCara, t, alphaC1);
    }

    // Caranguejo 2 — começa a entrar em t=4 (logo que o 1 está quase lá)
    if (t >= 4.0f)
    {
        float cara2X;
        if (t < 4.0f)
            cara2X = tx2 + escCara * 2.0f;
        else if (t < 9.0f)
        {
            float s = SMIN(4.0f, 9.0f, t);
            cara2X = (tx2 + escCara * 2.0f) - ((tx2 + escCara * 2.0f) - cara2DestinoX) * s;
        }
        else
            cara2X = cara2DestinoX;
        float alphaC2 = SMIN(4.0f, 6.0f, t);
        desenharCaranguejo(cara2X, caraY, escCara, t + 1.3f, alphaC2);
    }

    // CENA 3 [9–22s]: grupo de peixinhos passa UMA VEZ, esquerda→direita
    if (t >= 9.0f && t < 24.0f)
    {
        typedef struct
        {
            float yFrac;
            float esc;
            float r, g, b;
        } PeixeGrupo;
        PeixeGrupo ga[5] = {
            {0.60f, 0.048f, 1.0f, 0.55f, 0.10f},
            {0.68f, 0.042f, 0.20f, 0.70f, 0.90f},
            {0.52f, 0.055f, 0.95f, 0.80f, 0.10f},
            {0.74f, 0.038f, 0.15f, 0.75f, 0.40f},
            {0.62f, 0.045f, 0.85f, 0.40f, 0.80f},
        };
        float velGA = W * 0.072f; 
        float tLocal = t - 9.0f;  
        for (int i = 0; i < 5; i++)
        {
            float escala = W * ga[i].esc;
            float offset = i * W * 0.20f; // espaçamento entre peixinhos
            float posX = tx1 - escala * 2.0f + velGA * tLocal - offset;
            // offset negativo: os peixinhos entram em sequência, não todos juntos
            posX = tx1 - escala * 2.0f + velGA * tLocal + i * W * 0.14f;
            float posY = ty1 + H * ga[i].yFrac + sinf(t * 2.0f + i * 1.3f) * H * 0.025f;
            posX += sinf(t * 4.5f + i) * escala * 0.05f;
            // só desenha enquanto não saiu pela direita
            if (posX < tx2 + escala * 2.0f && posX > tx1 - escala * 3.0f)
            {
                float entradaFrac = (posX - (tx1 - escala * 2.0f)) / (escala * 3.0f);
                float alphaGA = entradaFrac < 1.0f ? entradaFrac : 1.0f;
                if (alphaGA < 0.0f)
                    alphaGA = 0.0f;
                desenharPeixe(posX, posY, escala, ga[i].r, ga[i].g, ga[i].b, alphaGA * 0.92f, 0);
            }
        }
    }

    // CENA 4 [20–35s]: cardume entra pela esquerda, agrupa e sai pela direita
    if (t >= 20.0f && t < 36.0f)
    {
        float cardumeDx[10] = {0.00f, 0.06f, -0.06f, 0.12f, -0.12f, 0.03f, -0.03f, 0.09f, -0.09f, 0.00f};
        float cardumeDy[10] = {0.00f, 0.04f, -0.04f, 0.01f, -0.01f, 0.08f, -0.08f, 0.05f, -0.05f, -0.09f};
        float cardumeCores[10][3] = {
            {1.0f, 0.55f, 0.10f}, {0.20f, 0.70f, 0.90f}, {0.95f, 0.80f, 0.10f}, {0.15f, 0.75f, 0.40f}, {0.85f, 0.40f, 0.80f}, {1.0f, 0.65f, 0.30f}, {0.50f, 0.85f, 1.0f}, {0.90f, 0.20f, 0.60f}, {0.30f, 0.85f, 0.55f}, {0.95f, 0.60f, 0.20f}};
        float cardumeEsc[10] = {
            0.048f, 0.042f, 0.055f, 0.038f, 0.045f, 0.040f, 0.050f, 0.038f, 0.043f, 0.047f};

        float alphaCardume = SMIN(20.0f, 22.5f, t);
        float tFase = t - 20.0f; 

        float centroX, centroY;
        if (tFase < 7.0f)
        {
            // agrupamento: peixes se agrupam para o centro e depois esquerda
            float s = SMIN(0.0f, 7.0f, tFase);
            centroX = tx1 + W * (0.25f + s * 0.12f);
            centroY = ty1 + H * (0.60f + sinf(t * 0.5f) * 0.03f);
        }
        else
        {
            // saída pela direita
            float tSaida = tFase - 7.0f; 
            float s = tSaida / 7.0f;
            if (s > 1.0f)
                s = 1.0f;
            centroX = tx1 + W * (0.37f + s * 1.05f);
            centroY = ty1 + H * (0.60f + sinf(t * 0.5f) * 0.02f);
        }

        float ondaCardume = sinf(t * 1.2f) * H * 0.015f;

        for (int i = 0; i < 10; i++)
        {
            float escala = W * cardumeEsc[i];
            float alvoX = centroX + cardumeDx[i] * W;
            float alvoY = centroY + cardumeDy[i] * H + ondaCardume;

            float posX, posY;
            if (tFase < 7.0f)
            {
                float s = SMIN(0.0f, 7.0f, tFase);
                float inicX = tx1 - escala * 2.0f - i * W * 0.04f;
                float inicY = ty1 + H * (0.45f + (i % 4) * 0.08f);
                posX = inicX + (alvoX - inicX) * s;
                posY = inicY + (alvoY - inicY) * s;
            }
            else
            {
                posX = alvoX;
                posY = alvoY;
            }

            posX += sinf(t * 3.5f + i * 0.7f) * escala * 0.06f;
            posY += sinf(t * 2.8f + i * 1.1f) * escala * 0.08f;

            if (posX < tx2 + escala * 2.0f)
            {
                desenharPeixe(posX, posY, escala,
                              cardumeCores[i][0], cardumeCores[i][1], cardumeCores[i][2],
                              alphaCardume * 0.93f, 0);
            }
        }
    }

    // CENA 5 [33–65s]: tartarugas entram pelas laterais + medusas de cima p/ baixo
    // Tartaruga 1 (esq→dir) e 2 (dir→esq) entram em t=33
    // Tartaruga 3 (esq→dir) entra um pouco depois, em t=38
    // Medusas surgem de cima para baixo a partir de t=33, com deslocamentos individuais
    if (t >= 33.0f)
    {
        float alphaCena5 = SMIN(33.0f, 36.0f, t);

        // tartaruga 1: entra da esquerda 
        {
            float escTart = W * 0.060f;
            float vel1 = W * 0.015f;
            float posX1 = tx1 - escTart * 1.5f + vel1 * (t - 33.0f + 2.0f);
            float posY1 = ty1 + H * 0.42f + sinf(t * 0.7f) * H * 0.03f;
            desenharTartaruga(posX1, posY1, escTart, t, 0.0f, 0, alphaCena5 * 0.93f);
        }

        // tartaruga 2: entra da direita 
        {
            float escTart = W * 0.050f;
            float vel2 = W * 0.012f;
            float posX2 = tx2 + escTart * 1.5f - vel2 * (t - 33.0f + 3.0f);
            float posY2 = ty1 + H * 0.30f + sinf(t * 0.9f + 1.5f) * H * 0.025f;
            desenharTartaruga(posX2, posY2, escTart, t, 1.5f, 1, alphaCena5 * 0.90f);
        }

        // tartaruga 3: entra da esquerda um pouco depois 
        if (t >= 38.0f)
        {
            float escTart = W * 0.045f;
            float vel3 = W * 0.010f;
            float alphaT3 = SMIN(38.0f, 41.0f, t);
            float posX3 = tx1 - escTart * 1.5f + vel3 * (t - 38.0f + 1.0f);
            float posY3 = ty1 + H * 0.55f + sinf(t * 0.6f + 3.0f) * H * 0.02f;
            desenharTartaruga(posX3, posY3, escTart, t, 3.0f, 0, alphaT3 * 0.88f);
        }

        // medusas: surgem de cima para baixo (entram pelo topo, descem oscilando) 
        {
            // cada medusa: {xFrac, velocidade_descida, fase_entrada, fase_oscilacao}
            float dadosMedusa[4][4] = {
                {0.15f, H * 0.012f, 33.0f, 0.0f},
                {0.40f, H * 0.010f, 35.0f, 1.8f},
                {0.65f, H * 0.013f, 34.5f, 3.5f},
                {0.85f, H * 0.009f, 36.5f, 5.2f},
            };
            for (int i = 0; i < 4; i++)
            {
                float tEntrada = dadosMedusa[i][2];
                if (t < tEntrada)
                    continue;
                float tLocal = t - tEntrada;
                float mx = tx1 + W * dadosMedusa[i][0] + sinf(t * 0.5f + dadosMedusa[i][3]) * W * 0.04f;
                // começa no topo (ty2) e desce devagar
                float my = ty2 - dadosMedusa[i][1] * tLocal + sinf(t * 0.6f + dadosMedusa[i][3]) * H * 0.025f;
                // mantém dentro da tela verticalmente
                if (my < ty1 + H * 0.15f)
                    my = ty1 + H * 0.15f;
                float mr = W * 0.038f;
                float alphaM = SMIN(tEntrada, tEntrada + 2.5f, t);
                desenharMedusa(mx, my, mr, t, dadosMedusa[i][3], alphaCena5 * alphaM * 0.85f);
            }
        }
    }

#undef SMIN

    glDisable(GL_BLEND);
    encerrarOrtho();
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_SCISSOR_TEST);
}

// reshape 

void reshape(int w, int h)
{
    if (h == 0)
        h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55, (double)w / (double)h, 0.8, 60.0);
    glMatrixMode(GL_MODELVIEW);
}

// display

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
    if (zLuz > 28)
        zLuz = -22;
    GLfloat pos1[] = {14.5, 3.8, zLuz, 1.0};
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);

    GLfloat pos2[] = {0.0, 5.2, -8.0, 1.0};
    GLfloat dir2[] = {0.0, -0.35, -0.94};
    glLightfv(GL_LIGHT2, GL_POSITION, pos2);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir2);

    // captura as matrizes da câmera para uso no gluProject das sobreposições 2D
    glGetDoublev(GL_MODELVIEW_MATRIX, g_modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, g_projection);
    glGetIntegerv(GL_VIEWPORT, g_viewport);

    // cena 3D
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

    // sobreposições 2D na tela

    EstadoAnimacao est = animacao_get_estado();
    float alpha = animacao_get_alpha_tela();

    if (animacao_get_texto_visivel())
    {
        desenharTextoTela("Studio Maria&Ana Apresenta...", "",
                          alpha, 1.0f, 0.9f, 0.5f);
    }

    if (animacao_get_creditos_visivel())
    {
        desenharTextoTela("Projeto Final de Computacao Grafica",
                          "Prof. Marco Antonio Garcia de Carvalho",
                          alpha, 0.85f, 0.92f, 1.0f);
    }

    // "Fundo do Mar"
    if (animacao_get_titulo_formas_visivel())
    {
        desenharTextoTela("Fundo do Mar", "",
                          alpha, 0.55f, 0.88f, 1.0f);
    }

    // animação de fundo do mar
    if (animacao_get_formas_visivel())
    {
        desenharFundoMar(animacao_get_tempo_formas());
    }

    // "Fim! Obrigada!"
    if (animacao_get_fim_visivel())
    {
        desenharTextoTela("Fim!", "Obrigada!",
                          animacao_get_alpha_tela(), 1.0f, 0.9f, 0.5f);
    }

    // fade para preto no final — cobre a tela inteira
    float alphaApagando = animacao_get_alpha_apagando();
    if (alphaApagando > 0.0f)
    {
        int winW = glutGet(GLUT_WINDOW_WIDTH);
        int winH = glutGet(GLUT_WINDOW_HEIGHT);
        iniciarOrtho();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, alphaApagando);
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(winW, 0);
        glVertex2f(winW, winH);
        glVertex2f(0, winH);
        glEnd();
        glDisable(GL_BLEND);
        encerrarOrtho();
    }

    glutSwapBuffers();
}

// ─── init / idle / main

void init()
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05, 0.05, 0.1, 1.0);
    carregarTexturas();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55, (double)glutGet(GLUT_WINDOW_WIDTH) / (double)glutGet(GLUT_WINDOW_HEIGHT), 0.8, 60.0);
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Cinema 3D - Animacao");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(controlarCameraMouse);
    glutMotionFunc(controlarCameraMotion);
    glutKeyboardFunc(controlarCameraTeclado);
    glutSpecialFunc(controlarCameraEspecial);

    glutMainLoop();
    return 0;
}

// compilar:
// gcc *.c -o cinema -lfreeglut -lopengl32 -lglu32
