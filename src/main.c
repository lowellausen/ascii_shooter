#include "../headers/includes.h"


int main()
{
    //implementa, basicamente, o menu inicial do jogo
    int i, cont_tiro=-1, cont_tiroInimigo=-1, comando = 1;//comando para sair do menu e encerrar jogo
    JOGADOR player = {3, 12, "", 3, 1, 0, 60, 0, MUNICAO, 0};      //Inicialização do PLAYER
    TIRO tiro_player[MUNICAO];  //Vetor com os tiros do Player
    TIRO tiro_inimigo[1000]; //Vetor com os tiros do Inimigo
    INIMIGO foe[INIMIGOS];  //Vetor de estruturas dos inimigos
    CHAVE key = {0,0,0}; //Chave para abrir a porta

    srand(time(0));

    do
    {
        switch(MENU(0))//abre o menu do jogo, com 0 para não exibir SAVEGAME
        {
        case NEWGAME://inicia o jogo
            NOVO_JOGO(&player, tiro_player, tiro_inimigo, foe, &key, &cont_tiro, &cont_tiroInimigo);
            MOSTRA_NIVEL(player);
            INICIA_FASE(&player, tiro_player, tiro_inimigo, foe, &key, &cont_tiro, &cont_tiroInimigo);
            break;
        case LOADGAME://carrega um jogo salvo em nome.txt
            PEDE_NOME(&player);
            CARREGAR_JOGO(&player, tiro_player, tiro_inimigo, foe, &key, &cont_tiro, &cont_tiroInimigo);
            INICIA_FASE(&player, tiro_player, tiro_inimigo, foe, &key, &cont_tiro, &cont_tiroInimigo);
            break;
        case HIGHSCORES://exibe ranking
            CARREGA_RANKING_PARA_EXIBIR();
            COR_TELA(&player);
            break;
        case QUIT://sai do jogo
            comando = 0;
            break;
        }
    }
    while(comando);


    return(0);
}


