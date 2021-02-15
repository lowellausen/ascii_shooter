#include "../headers/includes.h"

void INICIA_FASE(JOGADOR *player, TIRO *tiro_player, TIRO *tiro_inimigo, INIMIGO *foe, CHAVE *key, int *cont_tiro, int *cont_tiroInimigo)
{
    //inicia o jogo, que se mantém nestes laços até o final
    char tecla;
    int i, tempo, t, ti;

    COR_TELA(player);
    IMPRIME_CHAR(" 0 __","/|-| ", " ^  ", player->x, player->y);

    do
    {
        //laço principal do jogo, enquanto player tiver vidas e não terminar o jogo
        do
        {
            //laço que faz os movimentos automáticos do jogo, que devem acontecer enquanto o usuário não tecla nada
            ACOES_TERRORISTA (foe, player, cont_tiroInimigo, tiro_inimigo);
            MOVE_TIROS (player, tiro_player, tiro_inimigo, foe, t, ti, key, cont_tiro, cont_tiroInimigo);
            MOSTRA_PONTUACAO(*player);
            IMPRIME_CHAVE(key);
            PEGOU_CHAVE(player, key);
            TEMPORIZADOR (player, &tempo, foe);
            EXPLODE_BOMBA(tiro_inimigo,tiro_player, player, cont_tiroInimigo,cont_tiro, foe, key);

        }
        while (!(kbhit()) && (player->vidas>0));

        LE_TECLA(&tecla);
        OPCOES(player, tiro_player, tiro_inimigo, foe, key, cont_tiro, cont_tiroInimigo,tecla);
        DARA_TIRO (player, tiro_player, cont_tiro, tecla);
        MOSTRA_MUNICAO(*player);
        MOVE_REFEM(tecla, player, key);
        EXPLODE_BOMBA(tiro_inimigo,tiro_player, player, cont_tiroInimigo,cont_tiro, foe, key);
        AVANCA_NIVEL(player, tiro_player, tiro_inimigo, foe, key, cont_tiro, cont_tiroInimigo);
        TEMPORIZADOR (player, &tempo, foe);

    }
    while(player->vidas>0 && player->nivel<=NIVEIS);
    SALVA_PONTUACAO(*player);
    if(player->vidas==0)
        TELA_GAME_OVER(*player);
    if(player->vidas>0)
        TELA_JOGO_GANHO(*player);
    //se não vida é -1 e retorna para a main (menu)
}
void TEMPORIZADOR(JOGADOR *player,int  *cont, INIMIGO *foe)
{
    Sleep(1);

    if(HA_INIMIGOS(foe))
        *cont = *cont +1; //  incrementa o contador de milésimos de segundos passados
    if(((*cont%250==0)&&(player -> tempo>0)) || player->tempo == TEMPO) // A cada ciclo de  milésimos ou no tempo inicial
    {
        *cont=0; //para não ficar muito grande          // decrementa o tempo do jogador
        gotoxy(75,1);
        textbackground(CYAN);                         // e o imprime no canto da tela
        printf("%2d", player -> tempo);
        textbackground(DARKGRAY);
        player -> tempo = player -> tempo -1;
    }
}
void IMPRIME_CHAR (char *str1,char *str2, char *str3, int x, int y) // imprime os chares na tela
{
    cputsxy(x,y+2,str3);
    cputsxy(x,y+1,str2);
    cputsxy(x,y,str1);
}
int GERA_TIRO_INIMIGO (JOGADOR player)    //Gera randômico p/ inimigo atirar
{
    int random, gera;

    switch(player.nivel)//varia a frequência de tiros de acordo com o nível do player, para aumentar a dificuldade
    {
    //quanto menor o número de randômicos gerados, maior a frquência dos tiros
    case 1 :
        random=500;
        break;

    case 2 :
        random=300;
        break;

    case 3 :
        random=100;
        break;
    }

    gera = (int) (rand()/(double) RAND_MAX * random);

    if (gera == 1)
        return 1;
    else
        return 0;
}
void ATIRAR (TIRO *um_tiro, int xTiro, int yTiro, int cont_tiro, int sentido) //Realiza o comando de disparar o tiro (INIMIGO/PLAYER)
{
    switch(sentido) //para saber se o tiro é do player ou do inimigo, definindo o sentido
    {
    case 0 :    //Tiro do Inimigo (0)
        um_tiro[cont_tiro].x = xTiro-5;
        um_tiro[cont_tiro].y = yTiro;
        um_tiro[cont_tiro].vidaUtil = 1;

        textcolor(RED);
        cputsxy(um_tiro[cont_tiro].x, um_tiro[cont_tiro].y, "_ ");
        textcolor(WHITE);
        break;

    case 1 :    //Tiro do Player (1)
        um_tiro[cont_tiro].x = xTiro+5;
        um_tiro[cont_tiro].y = yTiro;
        um_tiro[cont_tiro].vidaUtil = 1;

        textcolor(RED);
        cputsxy(um_tiro[cont_tiro].x, um_tiro[cont_tiro].y, " _");
        textcolor(WHITE);
        break;
    }

}
void MOVE_TIRO(TIRO *um_tiro, int cont_tiro, int sentido)   //Faz o movimento do tiro
{
    // 0 = move para esquerda (tiro inimigo)
    // 1 = move para a direita (tiro player)

    um_tiro[cont_tiro].contMovimento = um_tiro[cont_tiro].contMovimento +1; //aumenta o contados de movimentos do tiro
    if ((um_tiro[cont_tiro].contMovimento)%5==0)//toda vez que esse contador subir 5 vezes, o tiro se move
        switch (sentido)                        //esse controle é feito para o tiro não ser muito rápido
        {
        case 0 :
            if (um_tiro[cont_tiro].vidaUtil == 1)
            {
                um_tiro[cont_tiro].x--;
                if (um_tiro[cont_tiro].x>1)//move o tiro
                {
                    textcolor(RED);
                    cputsxy(um_tiro[cont_tiro].x, um_tiro[cont_tiro].y, "_ ");
                    textcolor(WHITE);
                }
                else //destrói o tiro caso ele acerte a parede atrás do jogador
                {
                    um_tiro[cont_tiro].vidaUtil=0;
                    cputsxy(um_tiro[cont_tiro].x+1, um_tiro[cont_tiro].y, " ");
                }
            }
            break;

        case 1 :
            if (um_tiro[cont_tiro].vidaUtil == 1)
            {
                um_tiro[cont_tiro].x++;
                if (um_tiro[cont_tiro].x < 79)//mov o tiro
                {
                    textcolor(RED);
                    cputsxy(um_tiro[cont_tiro].x, um_tiro[cont_tiro].y, " _");
                    textcolor(WHITE);
                }
                else // destrói o tiro caso ele acerte a parede atrás dos inimigos
                {
                    um_tiro[cont_tiro].vidaUtil=0;
                    cputsxy(um_tiro[cont_tiro].x, um_tiro[cont_tiro].y, " ");
                    um_tiro[cont_tiro].x=400;
                    um_tiro[cont_tiro].y=400;
                }
            }
            break;
        }
}
void CRIA_INIMIGOS (INIMIGO *foe, JOGADOR  player)  //gera coordenada aleatórias para inimigos
{
    int i;

    for(i=0; i<INIMIGOS; i++)
    {
        foe[i].x = (i+1)*5 + 40; //gera x dos inimigos,a uma destância 5 um do outro
        foe[i].y = (int) (rand()/(double) RAND_MAX * 20)+3;
        foe[i].vida = 1;//dá vida aos inimigos
    }
}
void COR_TELA (JOGADOR *player) //gera a interface
{
    int i=3;
    char vida=3; //para impressão dos coraçõezinhos

    textbackground(DARKGRAY);//gera as cores na tela
    clrscr();
    textbackground(CYAN);
    textcolor(MAGENTA);
    switch (player->vidas)//imrpime corações de acordo com o número de vidas
    {
    case 1:
        printf("%c", vida);
        break;
    case 2:
        printf("%c %c", vida, vida);
        break;
    case 3:
        printf("%c %c %c", vida, vida, vida);
        break;
    }
    printf("\t\t\t\t\t\t\t\t\t\t");
    gotoxy(40,1);
    textcolor(WHITE);
    printf("LEVEL:%d", player->nivel);//imprime o nivel atual
    gotoxy(1,1);
    textbackground(BROWN);
    gotoxy(1,2);
    printf("\t\t\t\t\t\t\t\t\t\t");
    printf(" \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n");
    printf("\t\t\t\t\t\t\t\t\t\t");
    MOSTRA_PONTUACAO(*player);//mostra a pontuação do player
    textbackground(BROWN);
    for(i=2; i<=25; i++)
    {
        gotoxy(80, i);
        if ((i>10)&&(i<16))//imprime a portinha rosa
            textbackground(LIGHTMAGENTA);
        printf(" ");
        textbackground(BROWN);
    }
    textbackground(DARKGRAY);
    _setcursortype(1);
    textcolor(WHITE);
    gotoxy(1,1);
}
void MOVE_REFEM (char tecla, JOGADOR *player, CHAVE *key) //move o protagonista
{
    int limiteDireita;//define o limite caso haja inimigos

    if((!key->existencia)&&(!player->chave))
        limiteDireita=40; //limita a tela caso a chave não exista e o jogador não tenha a chave no bolso
    else
        limiteDireita=80;//libera a tela toda para o jogador
    switch (tecla)//detecta a direção do movimento
    {
    //efetua o movimento  do personagem, caso ele seja válido
    case CIMA: //e apaga as partes necessárias para não deixar rastro em cada caso
    case 119:
    case 87:
        if(player -> y>3)
            player -> y= player -> y-1;
        cputsxy(player -> x, player -> y+3,"    ");
        break;
    case 115:
    case BAIXO:
    case 83:
        if((player -> y+2)<24)
            player -> y = player -> y+1;
        cputsxy(player -> x, player -> y-1,"     ");
        break;
    case 97:
    case ESQUERDA:
    case 65:
        if(player -> x>2)
            player -> x= player -> x-1;
        cputsxy(player -> x+5, player -> y," ");
        break;
    case 100:
    case DIREITA:
    case 68:
        if(player -> x<(limiteDireita-5))
            player -> x= player -> x+1;
        cputsxy(player -> x-1, player -> y+1," ");
        break;
    }
    IMPRIME_CHAR(" 0 __","/|-| ", " ^  ", player -> x, player -> y);//imprime o personagem na posição atualizada
}
void MOVE_TERROR(INIMIGO *foe, JOGADOR player, int i) //move os inimigos malvados
{
    int max,r;

    if (foe[i].vida)//movimenta o inimigo caso esteja vivo
    {
        switch(player.nivel)//define a velocidade de movimento dos inimigos de acordo com o nível
        {
        //para incrementar a dificuldade
        case 1 :
            max=1000;
            break;

        case 2 :
            max=700;
            break;

        case 3 :
            max=400;
            break;
        }

        r = (int) (rand()/(double) RAND_MAX * (max +1));//gera randômico para o movimento do inimigo
        textcolor(YELLOW);
        switch (r)//define 0 para movimento para cima e 10 para baixo
        {
        //o número de aleatórioas possíveis determina a probabilidade de movimento
        case 0:    //efetua o movimento, caso válido, e apaga o rastro
            if(foe[i].y>3)
                foe[i].y= foe[i].y-1;
            cputsxy(foe[i].x+3, foe[i].y+3," ");
            break;
        case 10:
            if((foe[i].y+2)<24)
                foe[i].y = foe[i].y+1;
            cputsxy(foe[i].x, foe[i].y-1,"    ");
            break;
        default:
            break;
        }


        IMPRIME_CHAR("__ Q ", " |-|v", "   ^ ", foe[i].x, foe[i].y);//imprime o inimigo
        textcolor(WHITE);
    }

}
void MATA_TERRORISTA (INIMIGO *foe, TIRO *um_tiro, int cont_tiro, CHAVE *key, JOGADOR *player)  //Elimina os terroristas do campo de batalha
{
    int i;

    for(i=0; i<INIMIGOS; i++) //percorre os inmimigos
    {
        if ((um_tiro[cont_tiro].x == foe[i].x) && (foe[i].vida == 1))
            if ((um_tiro[cont_tiro].y == foe[i].y)||(um_tiro[cont_tiro].y==foe[i].y+1)|| (um_tiro[cont_tiro].y == foe[i].y+2))
            {
                //testa se há colisão de um tiro com algum inimigo
                INCREMENTA_PONTUACAO (player, 'i'); //se há incrementa a pontuação do jogador
                foe[i].vida = 0;//elimina o inimigo
                IMPRIME_CHAR("     ", "     ", "    ", foe[i].x, foe[i].y);
                um_tiro[cont_tiro].vidaUtil = 0; //elimina o tiro
                cputsxy(um_tiro[cont_tiro].x, um_tiro[cont_tiro].y, "  ");
                um_tiro[cont_tiro].x=400;
                um_tiro[cont_tiro].y=400;
                if(!HA_INIMIGOS(foe))
                {
                    //se este inimigo foi o último a ser morto
                    INCREMENTA_PONTUACAO (player, 't');//acrescenta o tempo à pontuação
                    GERA_CHAVE (key, foe[i]);
                }
            }
    }

}
void MATA_REFEM (JOGADOR *player,TIRO *tiro_player, TIRO *tiro_inimigo,INIMIGO *foe, int s,int *cont_tiro, int *cont_tiroInimigo,CHAVE *key )
{
    if(((player->x+4==tiro_inimigo[s].x)||(player->x==tiro_inimigo[s].x))&&((player->y==tiro_inimigo[s].y)
            ||(player->y+1==tiro_inimigo[s].y))&&(!tiro_inimigo[s].sentido))
    {
        LEVOU_TIRO();
        REINICIA_FASE(tiro_inimigo,tiro_player, player, cont_tiroInimigo,cont_tiro, foe, key);//se há colisão de tiro inimigo com o refem, reinicia o nível
    }
    if(((player->x-1==tiro_inimigo[s].x)||(player->x==tiro_inimigo[s].x))&&(player->y+2==tiro_inimigo[s].y))
    {
        cputsxy(player->x+1, player->y+2, "^");// testa se o tiro inimigo passou no caractere "^", que representa as perninhas
    }              //o imprimindo de novo para evitar que as pernas suamam momentâneamente causando pânico
}                                    //e impedindo o jogador de morrer com um tiro sob os pés
void REINICIA_FASE (TIRO *tiro_inimigo,TIRO *tiro_player, JOGADOR *player,int *cont_tiroInimigo,int *cont_tiro, INIMIGO *foe, CHAVE *key )//reinicia o nivel caso player morra
{
    int i=0;

    player->vidas = player->vidas -1;//decrementa as vidas
    player->pontuacao = player->pontuacaoaux;//carrega a pontuação axixiar, que contém a pontuação quando o player entrou na fase
    player->x = 3;//joga jogador para posição inicial/de forma que o jogador não mantenha a pontuação adquirida até então no nível
    player->y = 12;
    player->tempo = TEMPO;//reinicia o tempo
    player->municao = MUNICAO;
    key->existencia = 0;//destrói a chava, para caso o jogador tenha a sorte de ser morto pelo último inimigo após matar ele
    while(i<*cont_tiroInimigo)         //limpa os tiros
    {
        tiro_inimigo[i].vidaUtil = 0;
        tiro_inimigo[i].x=400;
        tiro_inimigo[i].y=400;
        i++;
    }
    *cont_tiroInimigo=-1;
    i=0;
    while(i<*cont_tiro)
    {
        tiro_player[i].vidaUtil = 0;
        tiro_player[i].x = 400;
        tiro_player[i].y = 400;
        i++;
    }
    *cont_tiro = -1;
    CRIA_INIMIGOS(foe, *player);//gera novos inimigos
    if(player->vidas>0)
        MOSTRA_NIVEL(*player);//exibe o nível atual
    COR_TELA(player);//volta ao jogo
    IMPRIME_CHAR(" 0 __","/|-| ", " ^  ", player->x, player->y);//imprime o player
}
void MOSTRA_PONTUACAO (JOGADOR player)//mostra a pontuação do jogador
{
    gotoxy(20,1);
    textbackground(CYAN);
    textcolor(WHITE);
    printf("SCORE: %3d", player.pontuacao);
    textbackground(DARKGRAY);
}
void LE_TECLA (char *tecla)//le tecla entrada pelo usuário, e retornando por referência
{
    *tecla = getch();
    if (*tecla<0)
        *tecla= getch();
}
int HA_INIMIGOS (INIMIGO *foe)//retorna o número de inimigos vivos
{
    int i, retorno=0;
    for(i=0; i<INIMIGOS; i++)
    {
        retorno= retorno + foe[i].vida;//soma a vida dos inimigos, sendo 1 para cada vivo e 0 para os mortos
    }                                 //resultando o número de inimigos vivos
    return(retorno);
}
void IMPRIME_CHAVE (CHAVE  *key)
{
    if(key->existencia) //caso a chave passe a existir, a imprime na posição do último inimigo morto
    {
        textcolor(YELLOW);
        cputsxy(key->x, key->y, "0¬");
        textcolor(WHITE);
    }

}
void PEGOU_CHAVE (JOGADOR *player, CHAVE *key)//testa se jogador pegou a chave
{
    if((key->existencia)&&(((player->x+4==key->x)||(player->x+1==key->x))&&((player->y==key->y)||(player->y+1==key->y)
                           ||(player->y+2==key->y))))
    {
        //testa se o jogador está na posição da chave
        cputsxy(key->x, key->y, "  ");//apaga a chave
        key->existencia = 0;//destrói a chave do cenário do jogo
        key->x = 100;
        key->y = 100;
        player->chave = 1;// e a cocola no bolso do personagem
        ABRE_PORTA();//abre a portinha rosa liberando caminho para o próximo e desafiador nível
    }
}
void ABRE_PORTA()
{
    int i;

    for(i=11; i<=15; i++)
        cputsxy(80,i, " ");
}
void AVANCA_NIVEL(JOGADOR *player,TIRO *tiro_player,TIRO *tiro_inimigo,INIMIGO *foe,CHAVE *key,int *cont_tiro,int *cont_tiroInimigo)
{
    int i;

    if((player->x+5==80)&&((player->y==11)||(player->y==12)||(player->y==13)||(player->y==14))&&(player->chave==1))//se o jogador
    {
        //está na porta
        player->nivel = player->nivel+1;//upa o nível
        player->pontuacaoaux = player->pontuacao;//salva pontuação em auxiliar, para carregá-la cajo player morra no vível
        gotoxy(1,1);
        player->x = 10;//move para posição inicial
        player->y = 12;
        player->tempo = TEMPO;
        player->chave = 0;//furta a chave do inimigo
        player->municao = MUNICAO;
        for (i=0; i<*cont_tiro; i++)//limpa os tiros
        {
            tiro_player[i].contMovimento=0;
            tiro_player[i].vidaUtil=0;
            tiro_player[i].x=400;
            tiro_player[i].y=400;
        }
        *cont_tiro = -1;
        for (i=0; i<*cont_tiroInimigo; i++)//limpa mais tiros
        {
            tiro_inimigo[i].contMovimento=0;
            tiro_inimigo[i].vidaUtil=0;
            tiro_inimigo[i].x=400;
            tiro_inimigo[i].y=400;
        }
        *cont_tiroInimigo = -1;
        CRIA_INIMIGOS(foe, *player);//cria novos inimigos ainda mais sedentos de sangue
        MOSTRA_NIVEL(*player);//exibe nível atual
        COR_TELA(player);//volta ao jogo
        IMPRIME_CHAR(" 0 __","/|-| ", " ^  ", player->x, player->y);
    }
}
int MENU(int salvar)//exibe menu de opções e retorna código da opção selecionada
{
    //entrada salvar define a exibição ou não da opção de salvar jogo, que só é diasponibilazada quando menu é chamado
    int i, seta=7, comando=0, lim=19;//posição da seta de seleção e limite inferior(muda com salvar)         //durante o jogo
    char tecla;                                                                                       //1 exibe salvar e 0 não
    //PlaySound(TEXT("ALBIS.wav"),NULL, SND_LOOP | SND_ASYNC); //musiquinha do jogo

    textbackground(BLACK);//gera interface
    system("cls");
    textcolor(WHITE);
    printf("--------------------------------------MENU--------------------------------------");
    printf("\n                                     OPTIONS:");
    textcolor(YELLOW);
    printf("\n\n\n\n                                     NEW GAME");
    if(salvar)//se salvar imprime a opção de salvar
        printf("\n\n\n\n                                     SAVE GAME");
    printf("\n\n\n\n                                     LOAD GAME");
    printf("\n\n\n\n                                     HIGH SCORES");
    printf("\n\n\n\n                                     QUIT");
    textcolor(WHITE);
    gotoxy(1,2);
    printf("-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-");
    gotoxy(1,25);
    printf("--------------------------------------------------------------------------------");
    for(i=1; i<=24; i++)
    {
        gotoxy(80, i);
        printf("-");
    }
    if(salvar) //limite inferior do menu, que é alterado com a existência da opção salvar
        lim=23;
    do
    {
        cputsxy(35,seta, "->");//seta de seleção
        LE_TECLA(&tecla);
        cputsxy(35, seta, "  ");
        switch(tecla) //move a seta d 4 em 4 posições
        {
        case CIMA:
            if(seta>7)
                seta = seta -4;
            break;
        case BAIXO:
            if(seta<lim)
                seta = seta +4;
            break;
        }
        cputsxy(35,seta, "->");
    }
    while(tecla!=ENTER&&tecla!=TAB);  //tecla tab serve para sair do menu durante o jogo
    if (tecla==ENTER)//retorna o comando de acordo com a posição final da seta no menu
        switch(seta)
        {
        case 7:
            comando = NEWGAME;
            break;
        case 11:
            comando = LOADGAME;
            if(salvar)//salvar altera a posição de tudo, devendo ser testado
                comando = SAVEGAME;
            break;
        case 15:
            comando = HIGHSCORES;
            if(salvar)
                comando = LOADGAME;
            break;
        case 19:
            comando = QUIT;
            if(salvar)
                comando=HIGHSCORES;
            break;
        case 23://sem if pois só é alcançado quando há salvar
            comando = QUIT;
            break;
        }
    return(comando);
}
void PEDE_NOME (JOGADOR *player)//pede nome ao usuário e o salva como nome do jogador
{
    system("cls");
    textcolor(YELLOW);
    cputsxy(35,12,"WHAT'S YOUR NAME?");
    textcolor(WHITE);
    gotoxy(35,13);
    gets(player->nome);
}
int SALVAR_JOGO (JOGADOR player,TIRO *tiro_player,TIRO *tiro_inimigo,INIMIGO *foe,CHAVE key,int cont_tiro,int cont_tiroInimigo)
{
    //salva dados do jogo em arquivo texto com o  nome do jogador
    char nome_arq[50]= {""}; //nome para criação do arquivo
    int i, retorno=1;
    FILE *arq;

    strcpy(nome_arq, player.nome);//transfere nome do jogador para nome do arquivo
    strcat(nome_arq, ".txt");//concatena extensão do arquivo .txt para nome do jogador
    if((arq=fopen(nome_arq, "w"))==NULL)//gera/abre o arquivo com nome.txt
    {
        system("cls");
        gotoxy(40,12);
        printf("\nFALHA AO CRIAR ARQUIVO\n");
        Sleep(2000);
        retorno=0;
    }
    else
    {
        if(!fprintf(arq," %d %d %d %d %d %d %d %d %d \n",player.x,player.y,player.vidas,player.nivel,player.pontuacao,player.tempo,player.chave,player.municao,player.pontuacaoaux))
        {
            //salva jogador
            system("cls");
            gotoxy(40,12);
            printf("\nFALHA AO SALVAR JOGADOR\n");
            Sleep(2000);
            retorno=0;
        }
        if(!fprintf(arq," %d %d %d \n",key.x,key.y,key.existencia))
        {
            //salva chave
            system("cls");
            gotoxy(40,12);
            printf("\nFALHA AO SALVAR CHAVE\n");
            Sleep(2000);
            retorno=0;
        }
        if(!fprintf(arq," %d \n", cont_tiro))
        {
            //salva contador de tiros player
            system("cls");
            gotoxy(40,12);
            printf("\nFALHA AO SALVAR CONTADOR DE TIROS\n");
            Sleep(2000);
            retorno=0;
        }
        if(!fprintf(arq," %d \n",cont_tiroInimigo))
        {
            //salava contador de tiros inimigos
            system("cls");
            gotoxy(40,12);
            printf("\nFALHA AO SALVAR CONTADOR DE TIROS DO INIMIGO\n");
            Sleep(2000);
            retorno=0;
        }
        for(i=0; i<INIMIGOS; i++)
            if(!fprintf(arq," %d %d %d %d %d \n",foe[i].x,foe[i].y,foe[i].velocidade,foe[i].contMovimento,foe[i].vida))
            {
                //salva inimigos
                system("cls");
                gotoxy(40,12);
                printf("\nFALHA AO SALVAR INIMIGO %d\n",  i);
                Sleep(2000);
                retorno=0;
            }
        i=1;
        while(i<cont_tiro)
        {
            //salva tiros player
            if(!fprintf(arq," %d %d %d %d %d \n",tiro_player[i].x,tiro_player[i].y,tiro_player[i].sentido,tiro_player[i].contMovimento,tiro_player[i].vidaUtil))
            {
                system("cls");
                gotoxy(40,12);
                printf("\nFALHA AO SALVAR TIRO %d DO JOGADOR\n", i);
                Sleep(2000);
                retorno=0;
            }
            i++;
        }
        i=1;
        while(i<cont_tiroInimigo)
        {
            //salva tiros inimigos
            if(!fprintf(arq," %d %d %d %d %d \n",tiro_inimigo[i].x,tiro_inimigo[i].y,tiro_inimigo[i].sentido,tiro_inimigo[i].contMovimento,tiro_inimigo[i].vidaUtil))
            {
                system("cls");
                gotoxy(40,12);
                printf("\nFALHA AO SALVAR TIRO %d DO INIMIGO\n", i);
                Sleep(2000);
                retorno=0;
            }
            i++;
        }
    }


    fclose(arq);//fecha o arquivo
    return(retorno);
}
int CARREGAR_JOGO (JOGADOR *player,TIRO *tiro_player,TIRO *tiro_inimigo,INIMIGO *foe,CHAVE *key,int *cont_tiro,int *cont_tiroInimigo)
{
    //carrega jogo salvo em arquivo texto com o nome do jogador
    char nome_arq[50]= {""}; //nome do arquivo a ser aberto
    int i, retorno=1;
    FILE *arq;

    strcpy(nome_arq, player->nome);//transfere nome do jogador(recem pedido, para encontrar arquivo de save) para nome do arquivo
    strcat(nome_arq, ".txt");//concatena extensão .txt para abrir o arquivo
    if((arq=fopen(nome_arq,"r"))==NULL)//abre o arquivo!
    {
        system("cls");
        gotoxy(40,12);
        printf("\nERRO AO ABRIR ARQUIVO\n");
        Sleep(2000);
        retorno=0;
    }
    else
    {
        if(!fscanf(arq," %d %d %d %d %d %d %d %d %d",&player->x,&player->y,&player->vidas,&player->nivel,&player->pontuacao,&player->tempo,&player->chave,&player->municao,&player->pontuacaoaux))
        {
            //carrega jogador
            system("cls");
            gotoxy(40,12);
            printf("\nERRO NA LEITURA DO JOGADOR\n");
            Sleep(2000);
            retorno=0;
        }
        if(!fscanf(arq," %d %d %d",&key->x,&key->y,&key->existencia))
        {
            //carrega chave
            system("cls");
            gotoxy(40,12);
            printf("\nERRO NA LEITURA DA CHAVE\n");
            Sleep(2000);
            retorno=0;
        }
        if(!fscanf(arq," %d",cont_tiro))
        {
            //carrega contador tiros player
            system("cls");
            gotoxy(40,12);
            printf("\nERRO NA LEITURA DO CONTADOR DE TIROS\n");
            Sleep(2000);
            retorno=0;
        }
        if(!fscanf(arq," %d",cont_tiroInimigo))
        {
            //catrrega contador de tiros inimigos
            system("cls");
            gotoxy(40,12);
            printf("\nERRO NA LEITURA DO CONTADOR DE TIROS DO INIMIGO\n");
            Sleep(2000);
            retorno=0;
        }
        for(i=0; i<INIMIGOS; i++)
            if(!fscanf(arq," %d %d %d %d %d",&foe[i].x,&foe[i].y,&foe[i].velocidade,&foe[i].contMovimento,&foe[i].vida))
            {
                //carrega inimigos
                system("cls");
                gotoxy(40,12);
                printf("\nERRO NA LEITURA DO INIMIGO %d\n",i);
                Sleep(2000);
                retorno=0;
            }
        i=1;
        while(i<*cont_tiro)
        {
            //carrega tiros player
            if(!fscanf(arq," %d %d %d %d %d",&tiro_player[i].x,&tiro_player[i].y,&tiro_player[i].sentido,&tiro_player[i].contMovimento,&tiro_player[i].vidaUtil))
            {
                system("cls");
                gotoxy(40,12);
                printf("\nERRO NA LEITURA DO TIRO %d DO PLAYER\n", i);
                Sleep(2000);
                retorno=0;
            }
            i++;
        }
        i=1;
        while(i<*cont_tiroInimigo)
        {
            //carrega tiros inimigos
            if(!fscanf(arq," %d %d %d %d %d",&tiro_inimigo[i].x,&tiro_inimigo[i].y,&tiro_inimigo[i].sentido,&tiro_inimigo[i].contMovimento,&tiro_inimigo[i].vidaUtil))
            {
                system("cls");
                gotoxy(40,12);
                printf("\nERRO NA LEITURA DO TIRO %d DO INIMIGO\n", i);
                Sleep(2000);
                retorno=0;
            }
            i++;
        }
    }

    fclose(arq);//fecha arquivo!!!
    return(retorno);
}
void OPCOES (JOGADOR *player,TIRO *tiro_player,TIRO *tiro_inimigo,INIMIGO *foe,CHAVE *key,int *cont_tiro,int *cont_tiroInimigo,char tecla)
{
    //verifica as entradas do usuário, caso não sejam as padrões de movimento
    switch (tecla)
    {
    case TAB://tab abre o menu durante o jogo, com 1 para a opção de salvar
        switch (MENU(1))
        {
        case NEWGAME://salva score e inicia um novo jogo
            SALVA_PONTUACAO(*player);
            MOSTRA_NIVEL(*player);
            NOVO_JOGO(player, tiro_player, tiro_inimigo, foe, key, cont_tiro, cont_tiroInimigo);
            COR_TELA(player);
            break;
        case SAVEGAME://salva score e o progresso no jogo
            SALVAR_JOGO(*player, tiro_player, tiro_inimigo, foe, *key, *cont_tiro, *cont_tiroInimigo);
            SALVA_PONTUACAO(*player);
            COR_TELA(player);
            break;
        case LOADGAME://carrega arquivo salvo com nome do jogador
            PEDE_NOME(player);
            CARREGAR_JOGO(player, tiro_player, tiro_inimigo, foe, key, cont_tiro, cont_tiroInimigo);
            COR_TELA(player);
            break;
        case HIGHSCORES://exibe ranking dos jogadores
            CARREGA_RANKING_PARA_EXIBIR();
            COR_TELA(player);
            break;
        case QUIT://sai para o menu principal e salva score
            player->vidas = -1;//seta vidas para -1 para encerrar laço do jogo
            break;
        default://retorna para o  jogo
            COR_TELA(player);
            break;
        }
        break;
    case N:
    case 78://exatas mesmas funções anteriores, mas entradas pelos atalhos, sem o menu TAB
        SALVA_PONTUACAO(*player);
        MOSTRA_NIVEL(*player);
        NOVO_JOGO(player, tiro_player, tiro_inimigo, foe, key, cont_tiro, cont_tiroInimigo);
        COR_TELA(player);
        break;
    case S:
    case 83:
        SALVAR_JOGO(*player, tiro_player, tiro_inimigo, foe, *key, *cont_tiro, *cont_tiroInimigo);
        SALVA_PONTUACAO(*player);
        COR_TELA(player);
        break;
    case C:
    case 67:
        PEDE_NOME(player);
        CARREGAR_JOGO(player, tiro_player, tiro_inimigo, foe, key, cont_tiro, cont_tiroInimigo);
        COR_TELA(player);
        break;
    case H:
        CARREGA_RANKING_PARA_EXIBIR();
        COR_TELA(player);
        break;
    case Q:
        player->vidas = -1;
    case 81:
        break;
    }
}
void SALVA_PONTUACAO(JOGADOR player)
{
    //salva a pontuação do jogador em um arquivo Pontuacoes.bin
    FILE *arq_score;

    if((VERIFICA_NOME_NA_PONTUACAO (player)) == 0)  //Testa se ha algum jogador com nome igual no arquivo de Pontuacoes.bin
        APPEND_DA_PONTUACAO(arq_score, player);     //Se não havia nenhum igual, faz o append do nome e pontuacao do player
    CARREGA_PONTUACAO_PARA_ORDENAR(arq_score);   //Carrega as pontuacoes para ordenar
}
void APPEND_DA_PONTUACAO(FILE *arq_score, JOGADOR player)
{
    PONTUACAO jogador;//variável do tipo pontuação, para receber nome e pontuação do jogador

    (strcpy(jogador.nome, player.nome));//rpassa dados do jogador para a variável
    jogador.pontuacao=player.pontuacao;

    if((arq_score = fopen("Pontuacoes.bin", "ab"))==NULL)//abre o arquivo
        printf("\nERRO AO ABRIR ARQUIVO Pontuacoes.bin!");
    else if(!fwrite(&jogador, sizeof(PONTUACAO), 1, arq_score))
    {
        //faz o append da pontuação
        Sleep(1000);
    }
    fclose(arq_score);//fecha o arquivo
}
void CARREGA_PONTUACAO_PARA_ORDENAR(FILE *arq_score)//carrega scores do arquivo Pontuacoes.bin
{
    PONTUACAO jogador[6], aux;
    int i=0, j, troca;

    if((arq_score = fopen("Pontuacoes.bin", "rb"))==NULL) //abre o arquivo
        Sleep(1000);
    else
    {
        while(!feof(arq_score) && i<6)  //Carrega as 6 pontuacoes
        {
            if(!fread(&jogador[i], sizeof(PONTUACAO), 1, arq_score))
                Sleep(1000);
            else
                i++;
        }
    }
    fclose(arq_score);
    ORDENA_PONTUACAO(jogador, i);//chama a função para ordenar as 6 pontuações
}
void ORDENA_PONTUACAO(PONTUACAO *jogador, int i)//ordena o ranking de pontuações
{
    int j, troca;
    PONTUACAO aux;

    do  //Ordena as pontuações
    {
        troca=0;

        for(j=0; j<i-1; j++)
            if(jogador[j].pontuacao < jogador[j+1].pontuacao)
            {

                troca=1;
                aux = jogador[j];
                jogador[j] = jogador[j+1];
                jogador[j+1] = aux;
            }
    }
    while(troca==1);

    GRAVA_PONTUACAO_ORDENADA(jogador, i);//passa pontuação ordenada para o arquivo
}
void GRAVA_PONTUACAO_ORDENADA(PONTUACAO *jogador, int i)//passa pontuações para o arquivo
{
    int j;
    FILE *arq_score;

    if((arq_score = fopen("Pontuacoes.bin", "wb"))==NULL)//abre o arquivo
        Sleep(1000);
    else
    {
        j=0;
        while(j<5 && j<i)//transfere as 5 primeiras pontuações para o arquivo
        {
            if(!fwrite(&jogador[j], sizeof(PONTUACAO), 1, arq_score))
            {
                Sleep(1000);
            }
            else
                j++;
        }
    }

    fclose(arq_score);
}
int VERIFICA_NOME_NA_PONTUACAO (JOGADOR player)//verifica s o jogador já não está no ranking
{
    FILE *arq_score;
    PONTUACAO jogador[5];
    int i=0, retorno=0;

    if((arq_score = fopen("Pontuacoes.bin", "r+b"))==NULL)//abre o arquivo
        Sleep(1000);
    else
        while(!feof(arq_score) && i<5)
        {
            if(fread(&jogador[i], sizeof(PONTUACAO), 1, arq_score))
                if(!strcmp(jogador[i].nome, player.nome))//percorre o arquivo comparando os nomes com o jogador atual
                {
                    retorno=1;

                    (strcpy(jogador[i].nome, player.nome));
                    jogador[i].pontuacao=player.pontuacao;

                    fseek(arq_score, -1*sizeof(PONTUACAO), 0);
                    fwrite(&jogador[i], sizeof(PONTUACAO), 1, arq_score);
                }
            i++;
        }

    fclose(arq_score);

    return (retorno);
}
void CARREGA_RANKING_PARA_EXIBIR()//carrega pontuações já ordenadas do arquivo pontuacoes.bin
{
    int i=0, j;
    PONTUACAO jogador[5];
    FILE *arq_score;

    if((arq_score = fopen("Pontuacoes.bin", "rb"))==NULL)//abre arquivo
        Sleep(1000);
    else
    {
        while((!feof(arq_score)) && i<5)//carrega até final do arquivo ou até pontuação 5
        {
            if(!fread(&jogador[i], sizeof(PONTUACAO), 1, arq_score))
            {
                //carrega pontuações para o array de pontuações
                Sleep(1000);
            }
            else
                i++;
        }
    }

    fclose(arq_score);
    MOSTRA_HIGHSCORE(jogador, i);//imprime tela com o ranking de jogadores

}
void MOSTRA_HIGHSCORE(PONTUACAO *jogador, int max)//imprime o ranking de jogadores
{
    int i=0;
    int j;  //Usada para a tabulacao dos nomes e pontos
    int t;  //Usada para a criacao da tabela

    textbackground(BLACK);
    system("cls");
    textcolor(YELLOW);
    gotoxy(20, 4);
    printf("----------------------------------------");
    gotoxy(20, 6);
    printf("----------------------------------------");
    gotoxy(20, 18);
    printf("----------------------------------------");
    cputsxy(37, 5, "RANKING");

    for(t=0; t<15; t++)
    {
        cputsxy(20, 4+t, "=");
        cputsxy(60, 4+t, "=");
    }

    textcolor(WHITE);
    while(i<max && i<5)//laço para imprimir as pontuações
    {
        j=i;    //Usado para fins de tabulação do ranking
        gotoxy(22, 8+i+j);
        printf("%s", jogador[i].nome);
        gotoxy(49, 8+i+j);
        printf("%d pontos", jogador[i].pontuacao);
        i++;
    }
    getch();
}
void TELA_JOGO_GANHO (JOGADOR player)//tela para quem vencer todos os incríveis níveis
{
    textbackground(DARKGRAY);
    system("cls");
    textcolor(LIGHTGREEN);
    gotoxy(2, 3);
    printf(" ___    ___  _________   __     __      __      __   ___   ___    __");
    printf("\n  \\  \\  /  / |         | |  |   |  |    |  |    |  | |   | |   \\  |  |");
    printf("\n   \\  \\/  /  |    _    | |  |   |  |    |  | /\\ |  | |   | |    \\ |  |");
    printf("\n    \\    /   |   |_|   | |  |___|  |    |  |/  \\|  | |   | |  |\\ \\|  |");
    printf("\n     |  |    |         | |         |    |          | |   | |  | \\    |");
    printf("\n     |__|    |_________| |_________|    |____/\\____| |___| |__|  \\___|");

    textcolor(YELLOW);
    gotoxy(37, 12);
    printf(" 0 __");
    gotoxy(37, 13);
    printf("/|-| ");
    gotoxy(37, 14);
    printf(" ^");
    textcolor(RED);
    cputsxy(44, 12, "_");

    textcolor(LIGHTGREEN);
    cputsxy(33, 17, "NAME:");
    cputsxy(33, 18, "SCORE:");
    textcolor(WHITE);
    gotoxy(39, 17);
    printf("%s", player.nome);
    gotoxy(40, 18);
    printf("%d", player.pontuacao);
    Sleep (5000);
}
void TELA_GAME_OVER (JOGADOR player)//tela para quem não suportar o terrorismo deste jogo
{
    textbackground(DARKGRAY);
    system("cls");
    textcolor(RED);
    gotoxy(2, 3);
    printf("_______   _______   ___  ___   ______    _______  __      __  ______   ______");
    printf("\n|  _____| |  ___  | |   \\/   | |  ____|  |       | \\ \\    / / |  ____| |  _   |");
    printf("\n| |  ___  | |___| | | |\\  /| | | |__     |   _   |  \\ \\  / /  | |__    | |_|  |");
    printf("\n| | |_  | |  ___  | | | \\/ | | |  __|    |  |_|  |   \\ \\/ /   |  __|   |______|");
    printf("\n| |___| | | |   | | | |    | | | |____   |       |    \\  /    | |____  | |\\ \\ ");
    printf("\n|_______| |_|   |_| |_|    |_| |______|  |_______|     \\/     |______| |_| \\_\\ ");


    textcolor(YELLOW);
    gotoxy(37, 12);
    printf(" 0 __");
    gotoxy(37, 13);
    printf("/|-| ");
    gotoxy(37, 14);
    printf(" ^");
    textcolor(RED);
    cputsxy(44, 12, "_");

    textcolor(RED);
    cputsxy(33, 17, "NAME:");
    cputsxy(33, 18, "SCORE:");
    textcolor(WHITE);
    gotoxy(39, 17);
    printf("%s", player.nome);
    gotoxy(40, 18);
    printf("%d", player.pontuacao);
    Sleep(5000);
}
void DARA_TIRO (JOGADOR *player, TIRO *tiro_player, int *cont_tiro, char tecla)//testa se o jogador dará um tiro
{
    if (tecla == ESPACO &&(player->municao > 0))         //se a tecla apertada é espaço dá um tiro
    {
        *cont_tiro= *cont_tiro +1;    //Usado para definir qual posição do tiro no array
        ATIRAR(tiro_player, player->x, player->y, *cont_tiro, 1);
        player->municao--; //reduz em um a munição do player
    }
}
void INCREMENTA_PONTUACAO (JOGADOR *player, char incremento)//incrementa a pontuacão do joagador baseado em um incremento
{
    //que pode ser 't' significando que o incremento é o tempo, ao término da fase
    if (incremento =='i') //ou 'i' significando que o incremento é um inimigo
        player->pontuacao += (((float)(1+player->nivel)/2)*10);
    else
        player->pontuacao += (((float)(1+player->nivel)/2)*player->tempo);
}
void NOVO_JOGO (JOGADOR *player, TIRO *tiro_player, TIRO *tiro_inimigo, INIMIGO *foe, CHAVE *key, int *cont_tiro, int *cont_tiroInimigo)
{
    //zera todos os dados do jogos para começar do zero
    int i =0;

    player->x = 3;
    player->y = 12;
    player->vidas = 3;
    player->nivel = 1;
    player->chave = 0;
    player->pontuacao = 0;
    player->pontuacaoaux = 0;
    player->tempo = TEMPO;
    player ->municao = MUNICAO;
    PEDE_NOME(player);
    key->x = 0;
    key->y =0;
    key->existencia = 0;
    for (i=0; i<*cont_tiro; i++)
    {
        tiro_player[i].contMovimento=0;
        tiro_player[i].vidaUtil=0;
        tiro_player[i].x=400;
        tiro_player[i].y=400;
    }
    *cont_tiro = -1;
    for (i=0; i<*cont_tiroInimigo; i++)
    {
        tiro_inimigo[i].contMovimento=0;
        tiro_inimigo[i].vidaUtil=0;
        tiro_inimigo[i].x=400;
        tiro_inimigo[i].y=400;
    }
    *cont_tiroInimigo = -1;
    CRIA_INIMIGOS(foe, *player);
    IMPRIME_CHAR(" 0 __","/|-| ", " ^  ", player->x, player->y);
    COR_TELA(player);//volta para o jogo
}
void MOSTRA_NIVEL (JOGADOR player)//exibe no centro da tela o nível o qual o jogador está iniciando
{
    if(player.nivel <=NIVEIS)
    {
        textbackground(BLACK);
        textcolor(YELLOW);
        system("cls");
        gotoxy(35,12);
        printf("LEVEL %d", player.nivel);
        Sleep(2000);
    }

}
void MOVE_TIROS (JOGADOR *player,TIRO *tiro_player,TIRO *tiro_inimigo,INIMIGO *foe,int t,int ti,CHAVE *key,int *cont_tiro,int *cont_tiroInimigo)
{
    //move os tiros e faz testes
    for (t=0; t<=*cont_tiro; t=t+1)    //move tiros do player
    {
        MATA_TERRORISTA(foe, tiro_player, t, key, player);//testa se mata algum terrorista
        MOVE_TIRO(tiro_player, t, 1);//move os tiros
    }
    for (ti=0; ti<=*cont_tiroInimigo; ti=ti+1)//move os tiros inimigos
    {
        MOVE_TIRO(tiro_inimigo, ti, 0);//move o tiro
        MATA_REFEM (player,tiro_player, tiro_inimigo,foe, ti,cont_tiro, cont_tiroInimigo, key);//testa se matou o refem
    }
}
void ACOES_TERRORISTA (INIMIGO *foe,JOGADOR *player,int *cont_tiroInimigo,TIRO *tiro_inimigo)
{
    //faz as ações de mover os terrorista, e, eventualmente, atirar
    int i;

    for(i=0; i<INIMIGOS; i++)//percorre os inimigos vivos
    {
        if (foe[i].vida)
        {
            MOVE_TERROR( foe,*player, i);//os move
            if(*cont_tiroInimigo<1000 &&(GERA_TIRO_INIMIGO(*player)==1))// e se o randômico for favorável a tiros...
            {
                *cont_tiroInimigo=*cont_tiroInimigo+1;//incrementa o contador
                tiro_inimigo[*cont_tiroInimigo].vidaUtil=1;//dá vida ao tiro
                ATIRAR(tiro_inimigo, foe[i].x, foe[i].y, *cont_tiroInimigo, 0);//...gera um tiro!
            }
        }
    }
}
void MOSTRA_MUNICAO (JOGADOR player)//mostra a munição do jogador
{
    gotoxy(55,1);
    textbackground(CYAN);
    textcolor(WHITE);
    printf("AMMO %3d", player.municao);
    textbackground(DARKGRAY);
}
void GERA_CHAVE (CHAVE *key,INIMIGO foe)
{
    // gera a chave na posição do inimigo
    key->existencia = 1;
    key->x = foe.x;
    key->y = foe.y;
    IMPRIME_CHAVE(key);
}
void EXPLODE_BOMBA (TIRO *tiro_inimigo,TIRO *tiro_player, JOGADOR *player,int *cont_tiroInimigo,int *cont_tiro, INIMIGO *foe, CHAVE *key )
{
    //se o jogador ficou sem tempo explode a bomba
    if(player->tempo==0)
    {
        EXPLODIU_BOMBA();
        REINICIA_FASE(tiro_inimigo,tiro_player, player, cont_tiroInimigo,cont_tiro, foe, key);//reinicia a fase atual
    }
}
void LEVOU_TIRO ()
{
    //acvisa ao usuário que ele foi baleado e será levado ao início do nível
    textbackground(BLACK);
    textcolor(RED);
    system("cls");
    cputsxy(35,12,"YOU'VE BEEN SHOT");//aviso de que explodiu a bomba
    Sleep(3000);
}
void EXPLODIU_BOMBA ()
{
    textbackground(BLACK);
    textcolor(RED);
    system("cls");
    cputsxy(35,12,"YOU RAN OUT OF TIME");//aviso de que explodiu a bomba
    cputsxy(35,13,"THE BOMB HAS EXPLODED");
    Sleep(3000);
}

