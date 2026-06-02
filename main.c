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
static GLint    g_viewport[4];

// ─── utilitários de texto 2D ────────────────────────────────────────────────

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

// ─── projetar ponto 3D da tela do cinema para coordenadas 2D da janela ────────

// A tela 3D fica em: centro (0, 5.0, -23.3), largura 11.5, altura 7.5
// Cantos: x in [-5.75, 5.75], y in [1.25, 8.75], z = -23.3
// Esta função converte um ponto 3D da tela para pixels da janela (Y invertido para ortho)
static void projetarPontoTela(float x3, float y3, float z3,
                               int winW, int winH,
                               double *ox, double *oy)
{
    GLdouble wx, wy, wz;
    // usa as matrizes capturadas logo após o gluLookAt, antes de qualquer push/pop
    gluProject(x3, y3, z3, g_modelview, g_projection, g_viewport, &wx, &wy, &wz);
    *ox = wx;
    *oy = wy;
}

// ─── desenhar textos com fade e centralização ────────────────────────────────

static void desenharTextoTela(const char *linha1, const char *linha2, float alpha,
                               float r, float g, float b)
{
    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);

    // projeta os 4 cantos da superfície branca para achar centro e limites
    // Projeta os 4 cantos da superfície branca individualmente
    // para obter o trapézio correto em perspectiva
    double pTL_x, pTL_y; // top-left
    double pTR_x, pTR_y; // top-right
    double pBL_x, pBL_y; // bottom-left
    double pBR_x, pBR_y; // bottom-right
    double pCx,   pCy;   // centro
    projetarPontoTela(-5.75, 8.75, -23.275, winW, winH, &pTL_x, &pTL_y);
    projetarPontoTela( 5.75, 8.75, -23.275, winW, winH, &pTR_x, &pTR_y);
    projetarPontoTela(-5.75, 1.25, -23.275, winW, winH, &pBL_x, &pBL_y);
    projetarPontoTela( 5.75, 1.25, -23.275, winW, winH, &pBR_x, &pBR_y);
    projetarPontoTela( 0.0,  5.0,  -23.275, winW, winH, &pCx,   &pCy);

    // bounding box para scissor e escala
    float tx1 = (float)( pTL_x < pBL_x ? pTL_x : pBL_x );
    float tx2 = (float)( pTR_x > pBR_x ? pTR_x : pBR_x );
    // altura pelo aspect ratio da tela 3D, reduzida 8% para compensar perspectiva
    float halfW = (tx2 - tx1) * 0.5f;
    float halfH = halfW / (11.5f / 7.5f) * 0.94f;
    float tcx = (float)pCx;
    float tcy = (float)pCy;
    // centro Y real = média dos 4 cantos projetados, com leve ajuste para baixo
    float tcy_real = ((float)pTL_y + (float)pTR_y + (float)pBL_y + (float)pBR_y) * 0.25f;
    tcy = tcy_real - halfH * 0.08f;
    float ty1 = tcy - halfH;
    float ty2 = tcy + halfH;
    float cx = tcx;
    float cy = tcy;

    glEnable(GL_SCISSOR_TEST);
    glScissor((int)tx1,(int)ty1,(int)(tx2-tx1),(int)(ty2-ty1));

    iniciarOrtho();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    void *fonte = GLUT_BITMAP_TIMES_ROMAN_24;

    float w1 = larguraTexto(linha1, fonte);
    glColor4f(r, g, b, alpha);
    renderizarTexto(linha1, fonte, cx - w1*0.5f, cy + 18.0f);

    if (linha2 && linha2[0] != '\0')
    {
        float w2 = larguraTexto(linha2, fonte);
        glColor4f(r, g, b, alpha * 0.9f);
        renderizarTexto(linha2, fonte, cx - w2*0.5f, cy - 14.0f);
    }

    glDisable(GL_BLEND);
    encerrarOrtho();
    glDisable(GL_SCISSOR_TEST);
}

// ─── animação de formas geométricas na tela ──────────────────────────────────
// triângulo → quadrado → círculo → estrela → ... com interpolação suave

static void desenharTriangulo(float t, float r, float g, float b, float alpha)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, alpha);
    float s = 120.0f;
    float cx = 512.0f, cy = 384.0f;
    glBegin(GL_TRIANGLES);
        glVertex2f(cx,          cy + s);
        glVertex2f(cx - s*0.866f, cy - s*0.5f);
        glVertex2f(cx + s*0.866f, cy - s*0.5f);
    glEnd();
    glDisable(GL_BLEND);
}

static void desenharQuadrado(float t, float r, float g, float b, float alpha)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, alpha);
    float s = 100.0f;
    float cx = 512.0f, cy = 384.0f;
    float ang = t * 45.0f * 3.14159f / 180.0f; // rotação lenta
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(t * 20.0f, 0, 0, 1);
    glBegin(GL_QUADS);
        glVertex2f(-s, -s);
        glVertex2f( s, -s);
        glVertex2f( s,  s);
        glVertex2f(-s,  s);
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
}

static void desenharCirculo(float t, float r, float g, float b, float alpha)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, alpha);
    float raio = 110.0f;
    float cx = 512.0f, cy = 384.0f;
    int n = 64;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= n; i++)
        {
            float a = i * 2.0f * 3.14159f / n;
            glVertex2f(cx + cos(a) * raio, cy + sin(a) * raio);
        }
    glEnd();
    glDisable(GL_BLEND);
}

static void desenharEstrela(float t, float r, float g, float b, float alpha)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, alpha);
    float raioExt = 120.0f, raioInt = 50.0f;
    float cx = 512.0f, cy = 384.0f;
    int pontas = 5;
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(t * 15.0f, 0, 0, 1);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0);
        for (int i = 0; i <= pontas * 2; i++)
        {
            float a = i * 3.14159f / pontas - 3.14159f * 0.5f;
            float raio = (i % 2 == 0) ? raioExt : raioInt;
            glVertex2f(cos(a) * raio, sin(a) * raio);
        }
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
}

// interpola vértices de duas formas poligonais (mesma quantidade de vértices)
// formas: 0=triângulo 1=quadrado 2=círculo(8pts) 3=estrela(10pts)
// usaremos círculo como base de interpolação (N pontos)

#define N_VERTS 20

static void calcVertices(int forma, float escala, float angExtra, float *vx, float *vy)
{
    float cx = 0, cy = 0;
    float PI = 3.14159f;

    if (forma == 0) // triângulo
    {
        for (int i = 0; i < N_VERTS; i++)
        {
            int v = i % 3;
            float a = v * 2.0f * PI / 3.0f - PI * 0.5f;
            vx[i] = cos(a) * escala;
            vy[i] = sin(a) * escala;
        }
    }
    else if (forma == 1) // quadrado
    {
        for (int i = 0; i < N_VERTS; i++)
        {
            int v = i % 4;
            float a = v * 2.0f * PI / 4.0f + PI * 0.25f;
            vx[i] = cos(a) * escala;
            vy[i] = sin(a) * escala;
        }
    }
    else if (forma == 2) // círculo
    {
        for (int i = 0; i < N_VERTS; i++)
        {
            float a = i * 2.0f * PI / N_VERTS;
            vx[i] = cos(a) * escala;
            vy[i] = sin(a) * escala;
        }
    }
    else // estrela (5 pontas) — 10 pontos reais mapeados em N_VERTS
    {
        float raioExt = escala, raioInt = escala * 0.42f;
        for (int i = 0; i < N_VERTS; i++)
        {
            // mapeia i para 10 vértices da estrela e interpola
            float fi = i * 10.0f / N_VERTS;
            int   vi = (int)fi;
            float ft = fi - vi; // fração entre vértices
            float aA = vi       * PI / 5.0f - PI * 0.5f;
            float aB = (vi + 1) * PI / 5.0f - PI * 0.5f;
            float rA = (vi % 2 == 0) ? raioExt : raioInt;
            float rB = ((vi+1) % 2 == 0) ? raioExt : raioInt;
            float xa = cos(aA) * rA, ya = sin(aA) * rA;
            float xb = cos(aB) * rB, yb = sin(aB) * rB;
            vx[i] = xa + (xb - xa) * ft;
            vy[i] = ya + (yb - ya) * ft;
        }
    }
}

// sequência de formas e cores ao longo de 12 segundos
static void desenharFormasAnimadas(float t)
{
    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);

    // Projeta os 4 cantos EXATOS da superfície branca (11.5 x 7.5 centrada em 0,5,-23.3)
    // Projeta os 4 cantos da superfície branca individualmente
    // para obter o trapézio correto em perspectiva
    double pTL_x, pTL_y; // top-left
    double pTR_x, pTR_y; // top-right
    double pBL_x, pBL_y; // bottom-left
    double pBR_x, pBR_y; // bottom-right
    double pCx,   pCy;   // centro
    projetarPontoTela(-5.75, 8.75, -23.275, winW, winH, &pTL_x, &pTL_y);
    projetarPontoTela( 5.75, 8.75, -23.275, winW, winH, &pTR_x, &pTR_y);
    projetarPontoTela(-5.75, 1.25, -23.275, winW, winH, &pBL_x, &pBL_y);
    projetarPontoTela( 5.75, 1.25, -23.275, winW, winH, &pBR_x, &pBR_y);
    projetarPontoTela( 0.0,  5.0,  -23.275, winW, winH, &pCx,   &pCy);

    // bounding box para scissor e escala
    float tx1 = (float)( pTL_x < pBL_x ? pTL_x : pBL_x );
    float tx2 = (float)( pTR_x > pBR_x ? pTR_x : pBR_x );
    // altura pelo aspect ratio da tela 3D, reduzida 8% para compensar perspectiva
    float halfW = (tx2 - tx1) * 0.5f;
    float halfH = halfW / (11.5f / 7.5f) * 0.94f;
    float tcx = (float)pCx;
    float tcy = (float)pCy;
    // centro Y real = média dos 4 cantos projetados, com leve ajuste para baixo
    float tcy_real = ((float)pTL_y + (float)pTR_y + (float)pBL_y + (float)pBR_y) * 0.25f;
    tcy = tcy_real - halfH * 0.08f;
    float ty1 = tcy - halfH;
    float ty2 = tcy + halfH;

    // usa glScissor para garantir que nada extrapola a área da tela
    glEnable(GL_SCISSOR_TEST);
    glScissor((int)tx1, (int)ty1, (int)(tx2-tx1), (int)(ty2-ty1));

    iniciarOrtho();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // fundo escuro no formato trapézio da perspectiva da tela
    glColor4f(0.0f, 0.0f, 0.05f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f((float)pBL_x, (float)pBL_y); // bottom-left
        glVertex2f((float)pBR_x, (float)pBR_y); // bottom-right
        glVertex2f((float)pTR_x, (float)pTR_y); // top-right
        glVertex2f((float)pTL_x, (float)pTL_y); // top-left
    glEnd();

    // paleta de cores por fase
    float cores[5][3] = {
        {0.95f, 0.35f, 0.25f}, // vermelho-laranja
        {0.25f, 0.75f, 0.95f}, // azul-ciano
        {0.45f, 0.95f, 0.45f}, // verde
        {0.95f, 0.85f, 0.25f}, // dourado
        {0.75f, 0.35f, 0.95f}, // roxo
    };

    // formas: 0=tri, 1=quad, 2=circ, 3=estrela, 0=tri...
    int seq[5] = {0, 1, 2, 3, 0};

    float duracao = 2.4f;  // duração de cada fase
    float transicao = 0.8f; // sobreposição de fade

    float fase_f = t / duracao;
    int fase = (int)fase_f;
    if (fase > 4) fase = 4;
    float local = fase_f - fase; // 0..1 dentro da fase

    int formaA = seq[fase % 5];
    int formaB = seq[(fase + 1) % 5];
    int corA = fase % 5;
    int corB = (fase + 1) % 5;

    float vxA[N_VERTS], vyA[N_VERTS];
    float vxB[N_VERTS], vyB[N_VERTS];

    // escala proporcional à largura da tela do cinema visível
    float escala = (tx2 - tx1) * 0.12f; // tamanho proporcional à tela
    float rotA = t * 18.0f; // graus
    float rotB = t * 18.0f + 5.0f;

    calcVertices(formaA, escala, 0, vxA, vyA);
    calcVertices(formaB, escala, 0, vxB, vyB);

    // blend suave: começa a misturar nos últimos `transicao` segundos da fase
    float blend = 0.0f;
    if (local > 1.0f - transicao / duracao)
        blend = (local - (1.0f - transicao / duracao)) / (transicao / duracao);

    // smooth step
    blend = blend * blend * (3.0f - 2.0f * blend);

    // usa o centro calculado da tela do cinema
    float cx = tcx, cy = tcy; // centro exato da tela projetada
    float PI = 3.14159f;

    // sombra suave
    glColor4f(0.0f, 0.0f, 0.0f, 0.35f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx + 5, cy - 5);
    for (int i = 0; i <= N_VERTS; i++)
    {
        float tvx = vxA[i % N_VERTS] + (vxB[i % N_VERTS] - vxA[i % N_VERTS]) * blend;
        float tvy = vyA[i % N_VERTS] + (vyB[i % N_VERTS] - vyA[i % N_VERTS]) * blend;
        float ra = rotA * PI / 180.0f;
        float rx = tvx * cos(ra) - tvy * sin(ra);
        float ry = tvx * sin(ra) + tvy * cos(ra);
        glVertex2f(cx + 5 + rx, cy - 5 + ry);
    }
    glEnd();

    // forma interpolada
    float r = cores[corA][0] + (cores[corB][0] - cores[corA][0]) * blend;
    float g = cores[corA][1] + (cores[corB][1] - cores[corA][1]) * blend;
    float b = cores[corA][2] + (cores[corB][2] - cores[corA][2]) * blend;

    glColor4f(r, g, b, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= N_VERTS; i++)
    {
        float tvx = vxA[i % N_VERTS] + (vxB[i % N_VERTS] - vxA[i % N_VERTS]) * blend;
        float tvy = vyA[i % N_VERTS] + (vyB[i % N_VERTS] - vyA[i % N_VERTS]) * blend;
        float ra = rotA * PI / 180.0f;
        float rx = tvx * cos(ra) - tvy * sin(ra);
        float ry = tvx * sin(ra) + tvy * cos(ra);
        glVertex2f(cx + rx, cy + ry);
    }
    glEnd();

    // contorno brilhante
    glLineWidth(2.5f);
    glColor4f(1.0f, 1.0f, 1.0f, 0.55f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < N_VERTS; i++)
    {
        float tvx = vxA[i] + (vxB[i] - vxA[i]) * blend;
        float tvy = vyA[i] + (vyB[i] - vyA[i]) * blend;
        float ra = rotA * PI / 180.0f;
        float rx = tvx * cos(ra) - tvy * sin(ra);
        float ry = tvx * sin(ra) + tvy * cos(ra);
        glVertex2f(cx + rx, cy + ry);
    }
    glEnd();

    // nome centralizado dentro da tela do cinema (abaixo do centro)
    const char *nomes[] = {"Triangulo", "Quadrado", "Circulo", "Estrela", "Triangulo"};
    const char *nomeA = nomes[formaA];
    const char *nomeB = nomes[formaB];
    void *fonte = GLUT_BITMAP_HELVETICA_18;
    float wA = larguraTexto(nomeA, fonte);
    float wB = larguraTexto(nomeB, fonte);
    float nomeY = ty1 + (ty2 - ty1) * 0.10f; // relativo à altura da tela projetada

    // fade out nome A, fade in nome B
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - blend);
    renderizarTexto(nomeA, fonte, tcx - wA * 0.5f, nomeY);
    glColor4f(1.0f, 1.0f, 1.0f, blend);
    renderizarTexto(nomeB, fonte, tcx - wB * 0.5f, nomeY);

    glDisable(GL_BLEND);
    encerrarOrtho();
    glDisable(GL_SCISSOR_TEST);
}

// ─── reshape ─────────────────────────────────────────────────────────────────

void reshape(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55, (double)w / (double)h, 0.8, 60.0);
    glMatrixMode(GL_MODELVIEW);
}

// ─── display ─────────────────────────────────────────────────────────────────

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
    glLightfv(GL_LIGHT0, GL_AMBIENT,  luz0_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  luz0_dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz0_esp);

    GLfloat luz_global[] = {0.18f * intensidade, 0.16f * intensidade, 0.20f * intensidade, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luz_global);
    GLfloat pos0[] = {0.0, 8.2, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);

    static float zLuz = -20.0;
    zLuz += 0.3;
    if (zLuz > 28) zLuz = -22;
    GLfloat pos1[] = {14.5, 3.8, zLuz, 1.0};
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);

    GLfloat pos2[] = {0.0, 5.2, -8.0, 1.0};
    GLfloat dir2[] = {0.0, -0.35, -0.94};
    glLightfv(GL_LIGHT2, GL_POSITION, pos2);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir2);

    // captura as matrizes da câmera para uso no gluProject das sobreposições 2D
    glGetDoublev(GL_MODELVIEW_MATRIX,  g_modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, g_projection);
    glGetIntegerv(GL_VIEWPORT,         g_viewport);

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

    // ── sobreposições 2D na tela ────────────────────────────────────────────

    EstadoAnimacao est = animacao_get_estado();
    float alpha = animacao_get_alpha_tela();

    // "Studio Maria Ana Apresenta"
    if (animacao_get_texto_visivel())
    {
        desenharTextoTela("Studio Maria Ana Apresenta", "",
                          alpha, 1.0f, 0.9f, 0.5f);
    }

    // "Projeto final de Computacao Grafica..."
    if (animacao_get_creditos_visivel())
    {
        desenharTextoTela("Projeto Final de Computacao Grafica",
                          "Prof. Marco Antonio Garcia de Carvalho",
                          alpha, 0.85f, 0.92f, 1.0f);
    }

    // "Formas Geometricas"
    if (animacao_get_titulo_formas_visivel())
    {
        desenharTextoTela("Formas Geometricas", "",
                          alpha, 0.95f, 0.95f, 0.95f);
    }

    // animação de formas
    if (animacao_get_formas_visivel())
    {
        desenharFormasAnimadas(animacao_get_tempo_formas());
    }

    glutSwapBuffers();
}

// ─── init / idle / main ──────────────────────────────────────────────────────

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
    glLightfv(GL_LIGHT0, GL_AMBIENT,  luz0_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  luz0_dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz0_esp);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.03);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.005);

    GLfloat luz1_amb[] = {0.05, 0.03, 0.01, 1.0};
    GLfloat luz1_dif[] = {1.2, 0.95, 0.55, 1.0};
    GLfloat luz1_esp[] = {0.8, 0.7, 0.5, 1.0};
    glLightfv(GL_LIGHT1, GL_AMBIENT,  luz1_amb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  luz1_dif);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luz1_esp);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.08);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.02);

    GLfloat luz2_amb[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat luz2_dif[] = {1.3, 1.2, 1.0, 1.0};
    GLfloat luz2_esp[] = {1.0, 0.95, 0.85, 1.0};
    glLightfv(GL_LIGHT2, GL_AMBIENT,  luz2_amb);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  luz2_dif);
    glLightfv(GL_LIGHT2, GL_SPECULAR, luz2_esp);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.05);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.01);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 35.0);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 12.0);

    GLfloat luz_global[] = {0.18, 0.16, 0.20, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luz_global);

    GLfloat mat_spec[] = {0.85, 0.80, 0.75, 1.0};
    GLfloat brilho[]   = {60.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_spec);
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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
// gcc -o cinema.exe main.c camera.c obj.c textura.c image.c animacao.c tempo.c -lfreeglut -lglu32 -lopengl32
