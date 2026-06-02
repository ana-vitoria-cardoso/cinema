// Ana Vitória Cardoso Queiroz - 172706
// Maria Clara Muharem - 266864

#ifndef ANIMACAO_H
#define ANIMACAO_H

typedef enum {
    ANIM_ABRINDO_PORTA,   
    ANIM_INICIO,
    ANIM_ANDANDO_Z,
    ANIM_ANDANDO_X,
    ANIM_OLHANDO_LATERAL, 
    ANIM_SENTANDO,        
    ANIM_IR_PARA_FRENTE,  
    ANIM_SENTANDO_CAMERA, 
    ANIM_OLHANDO_TELA,
    ANIM_ESCURECENDO,
    ANIM_PROJETANDO,        // "Studio Maria Ana Apresenta"
    ANIM_CREDITOS,          // "Projeto final de computacao grafica..."
    ANIM_TITULO_FORMAS,     // "Formas Geometricas"
    ANIM_FORMAS,            // animacao de formas na tela
    ANIM_FIM_TEXTO,         // "Fim! Obrigada!"
    ANIM_APAGANDO,          // camera recua, tela apaga
    ANIM_FIM
} EstadoAnimacao;

void animacao_iniciar(void);
void animacao_atualizar(void);
int animacao_esta_ativa(void);
EstadoAnimacao animacao_get_estado(void);

float animacao_get_intensidade_luz(void);
float animacao_get_escurecimento(void);
int animacao_get_texto_visivel(void);
float animacao_get_inclinacao_cadeira(void);
float animacao_get_abertura_porta(void);

// novos getters para os estados da tela
int animacao_get_creditos_visivel(void);
int animacao_get_titulo_formas_visivel(void);
int animacao_get_formas_visivel(void);
float animacao_get_tempo_formas(void);
float animacao_get_alpha_tela(void);  // fade in/out dos textos
int animacao_get_fim_visivel(void);
float animacao_get_alpha_apagando(void);

#endif
