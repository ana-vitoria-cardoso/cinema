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
    ANIM_PROJETANDO,
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

#endif