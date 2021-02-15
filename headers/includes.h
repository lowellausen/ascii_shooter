#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>
#include <time.h>
#define CIMA 72
#define BAIXO 80
#define ESQUERDA 75
#define DIREITA 77
#define INIMIGOS 5
#define TEMPO 30
#define NIVEIS 3
#define ESPACO 32
#define ENTER 13
#define NEWGAME 1
#define SAVEGAME 2
#define LOADGAME 3
#define HIGHSCORES 4
#define QUIT 5
#define TAB 9
#define N 110
#define S 115
#define C 99
#define H 104
#define Q 113
#define MUNICAO 20

typedef struct TIRO
{
    int x, y;
    int sentido;
    int contMovimento;
    int vidaUtil;
} TIRO;

typedef struct JOGADOR
{
    int x, y;
    char nome[20];
    int vidas;
    int nivel;
    int pontuacao;
    int tempo;
    int chave;
    int municao;
    int pontuacaoaux;
} JOGADOR;

typedef struct INIMIGO
{
    int x, y;
    int velocidade;
    int contMovimento;
    int vida;
} INIMIGO;

typedef struct
{
    int x, y;
    int existencia;
} CHAVE;

typedef struct PONTUACAO
{
    char nome[20];
    int pontuacao;
} PONTUACAO; //estrutura para salvar os scores


//funções de interface:
void COR_TELA (JOGADOR *player); //gera a interface
void MOSTRA_PONTUACAO (JOGADOR player);//exibe pontuação na tela
void IMPRIME_CHAVE (CHAVE  *key);//imprime a chave na tela
void ABRE_PORTA();//apaga a portinha rosa
void IMPRIME_CHAR (char *str1,char *str2, char *str3, int x, int y); // imprime os chares na tela
int MENU(int salvar);//menu do jogo
void TELA_GAME_OVER (JOGADOR player); //tela final de jogo
void TELA_JOGO_GANHO (JOGADOR player);//tela de jogo ganho
void MOSTRA_HIGHSCORE(PONTUACAO *jogador, int max);//tela do ranking de pontuações
void MOSTRA_MUNICAO (JOGADOR player);//exibe munição na tela
void LEVOU_TIRO ();//avisa que o player foi baleado
void EXPLODIU_BOMBA();//avisa que a bomba explodiu
void TEMPORIZADOR(JOGADOR *player,int  *cont, INIMIGO *foe);//exibe o tempo na tela
void MOSTRA_NIVEL (JOGADOR player);//mostra o nível atual na tela

//funções de movimentação na tela:
void MOVE_TIRO(TIRO *um_tiro, int cont_tiro, int sentido);   //Faz o movimento do tiro
void MOVE_REFEM (char tecla, JOGADOR *player, CHAVE *key); //move o protagonista
void MOVE_TERROR(INIMIGO *foe, JOGADOR player, int i); //move os inimigos malvados
void MOVE_TIROS (JOGADOR *player,TIRO *tiro_player,TIRO *tiro_inimigo,INIMIGO *foe,int t,int ti,CHAVE *key,int *cont_tiro,int *cont_tiroInimigo);

//funções que  acrescentam dados do jogo:
int GERA_TIRO_INIMIGO (JOGADOR player);    //Gera randômico p/ inimigo atirar
void ATIRAR (TIRO *um_tiro, int xTiro, int yTiro, int cont_tiro, int sentido); //Realiza o comando de disparar o tiro (INIMIGO/PLAYER)
void CRIA_INIMIGOS (INIMIGO *foe, JOGADOR  player);  //gera coordenada aleatórias para inimigos
void GERA_CHAVE (CHAVE *key,INIMIGO foe);

//funcções que modificam o estado de jogo
void MATA_TERRORISTA (INIMIGO *foe, TIRO *um_tiro, int cont_tiro, CHAVE *key, JOGADOR *player); //Elimina os terroristas do campo de batalha
void AVANCA_NIVEL(JOGADOR *player,TIRO *tiro_player,TIRO *tiro_inimigo,INIMIGO *foe,CHAVE *key,int *cont_tiro,int *cont_tiroInimigo);
void PEDE_NOME (JOGADOR *player);
void INICIA_FASE(JOGADOR *player, TIRO *tiro_player, TIRO *tiro_inimigo, INIMIGO *foe, CHAVE *key, int *cont_tiro, int *cont_tiroInimigo);
void LE_TECLA (char *tecla);//lê a tecla inserida pelo usuário
void REINICIA_FASE (TIRO *tiro_inimigo,TIRO *tiro_player, JOGADOR *player,int *cont_tiroInimigo,int *cont_tiro, INIMIGO *foe, CHAVE *key);
void MATA_REFEM (JOGADOR *player,TIRO *tiro_player, TIRO *tiro_inimigo,INIMIGO *foe, int s,int *cont_tiro, int *cont_tiroInimigo,CHAVE *key );
void ACOES_TERRORISTA (INIMIGO *foe,JOGADOR *player,int *cont_tiroInimigo,TIRO *tiro_inimigo);//faz as ações dos terroristas
void INCREMENTA_PONTUACAO(JOGADOR *player, char incremento);//aumenta a pontuação do player
void NOVO_JOGO (JOGADOR *player, TIRO *tiro_player, TIRO *tiro_inimigo, INIMIGO *foe, CHAVE *key, int *cont_tiro, int *cont_tiroInimigo);

//funções de teste:
int HA_INIMIGOS (INIMIGO *foe);//testa se player está na posição da chave
void PEGOU_CHAVE (JOGADOR *player, CHAVE *key);
int VERIFICA_NOME_NA_PONTUACAO (JOGADOR player);
void OPCOES (JOGADOR *player,TIRO *tiro_player,TIRO *tiro_inimigo,INIMIGO *foe,CHAVE *key,int *cont_tiro,int *cont_tiroInimigo,char tecla);
void DARA_TIRO (JOGADOR *player, TIRO *tiro_player, int *cont_tiro, char tecla);//testa se usuário deu tiro
void EXPLODE_BOMBA (TIRO *tiro_inimigo,TIRO *tiro_player, JOGADOR *player,int *cont_tiroInimigo,int *cont_tiro, INIMIGO *foe, CHAVE *key );

//funções de arquivo:
int SALVAR_JOGO (JOGADOR player,TIRO *tiro_player,TIRO *tiro_inimigo,INIMIGO *foe,CHAVE key,int cont_tiro,int cont_tiroInimigo);
int CARREGAR_JOGO (JOGADOR *player,TIRO *tiro_player,TIRO *tiro_inimigo,INIMIGO *foe,CHAVE *key,int *cont_tiro,int *cont_tiroInimigo);
void SALVA_PONTUACAO(JOGADOR player);
void APPEND_DA_PONTUACAO(FILE *arq_score, JOGADOR player);
void CARREGA_PONTUACAO_PARA_ORDENAR(FILE *arq_score);
void ORDENA_PONTUACAO(PONTUACAO *jogador, int i);
void GRAVA_PONTUACAO_ORDENADA(PONTUACAO *jogador, int i);
void CARREGA_RANKING_PARA_EXIBIR();



