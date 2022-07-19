#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../constantes/constantes.h"
#include "../estruturas/estruturas.h"
#include "funcoes.h"


void clear_screen(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void start_time(time_t * t){time(t);}

double get_time(time_t * t){return time(NULL) - *t;}

void setBombs(int ROW, int COL, int QTDBOMBS, houses * pt_board){
    for (int b = 0; b < QTDBOMBS; b++){
        int slot = rand() % (ROW*COL);
        if (pt_board[slot].face[1] != FBOMB){
            pt_board[slot].face[1] = FBOMB;
        }
        else b--;
    }         
}

void setNumbers(int ROW, int COL, houses * pt_board){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(pt_board[i*COL+j].face[1] != FBOMB){
                int count = 0;
                for(int x = -1; x < 2; x++){
                    for(int y = -1; y < 2; y++){
                        if((i-x) < ROW && (i-x) >= 0 && (j-y) < COL && (j-y)  >= 0){
                            if(pt_board[(i-x)*COL+(j-y)].face[1] == FBOMB){
                                count++;
                            }
                        }
                    }
                }
                if(count > 0){
                    pt_board[i*COL+j].face[1] = count + '0';
                }
                else{
                    pt_board[i*COL+j].face[1] = FNULL;
                }
            }
        }
    }
}

houses * init_board(int ROW, int COL, int QTDBOMBS, houses * board){
    board = (houses*) malloc(ROW*COL*sizeof(houses));
    for(int i = 0; i < ROW;i++){
        for(int j = 0; j < COL;j++){
            board[i*COL+j].face[0] = FNOTREV;
            board[i*COL+j].status = 0;
        }
    }
    setBombs(ROW,COL,QTDBOMBS,board);
    setNumbers(ROW,COL,board);
    return board;
}

void print_board(int ROW, int COL, houses * board){
    printf("\n");
    for(int i=-1; i<ROW; i++){
        if (i==-1) {
            for(int k=0; k<COL; k++){
                if (k==0){
                    printf("  %4d",k+1);
                }
                else {
                    printf("%4d",k+1);
                }
            }
            printf("\n");
        }
        else{
            for(int j=0; j<COL; j++){
                if(j==0){
                    printf("%2d | %c ",i+1,board[i*COL+j].face[board[i*COL+j].status]);
                }
                else {
                    printf("| %c ",board[i*COL+j].face[board[i*COL+j].status]);
                }
            }
            printf("|\n");
        }
    }
    printf("\n");
}

int reveal(int ROW, int COL, int indexR, int indexC ,houses * pt_board, int inGame, int * avaliableT, time_t * time){
    if(inGame >= 0 && inGame < ((ROW*COL) - QTDBOMBS)){
        if (inGame==0){
            *avaliableT = 1;
            start_time(time);
        }
        int pos = indexR*COL+indexC;
        pt_board[pos].status = 1;
        if(pt_board[pos].face[1] != FBOMB){
            inGame++;
            if(pt_board[pos].face[1] == FNULL){
                for(int i = -1; i < 2;i++){
                    for(int j = -1; j < 2;j++){
                        int rNew = indexR - i, cNew = indexC - j;
                        if((rNew) < ROW && (rNew) >= 0 && (cNew) < COL && (cNew)  >= 0 &&  pt_board[(rNew)*COL+(cNew)].status == 0){
                            reveal(ROW,COL,(rNew),(cNew),pt_board,inGame,avaliableT,time);
                        }
                    }
                }
            }
        }
        else{
            inGame = -1;
        }
    }
    return inGame;
}

float calc_prob(int r, int c, houses * board){
    float total = 0, prob;
    for(int i = -1; i < 2;i++){
        for(int j = -1; j < 2;j++){
            int rNew = r - i, cNew = c - j;
            if((rNew) < ROW && (rNew) >= 0 && (cNew) < COL && (cNew)  >= 0){
                if(board[(rNew)*COL+(cNew)].status == 0){
                    total++;
                }                
            }
        }
    }
    prob = 1 - ((board[r*COL+c].face[1] - '0')/total);
    return prob;
}

void help(int * r,int * c, houses * board){
    int index = 0, buffer;
    houses * pos;
    for(int x = 0; x < ROW && index == 0;x++){
        for(int y = 0; y < COL && index == 0;y++){
            buffer = x*COL+y;
            if(board[buffer].status == 1 && board[buffer].face[1] != FNULL){
                pos = &(board[buffer]);
                *r = x, *c = y;
                index = 1;                
            }
        }
    }
    float probM = calc_prob(*r,*c,board),probA;
    for(int x = 0; x < ROW;x++){
        for(int y = 0; y < COL;y++){
            buffer = x*COL+y;
            if(board[buffer].status == 1 && board[buffer].face[1] != FNULL){
                probA = calc_prob(x,y,board);
                if(probA > probM){                
                    probM = probA;
                    pos = &(board[buffer]);
                    *r = x, *c = y;
                }
            }        
        }
    }
}

void init_bot(houses * pt_board, FILE * save, int * avaliableT,time_t * time){
    int inGame = 0, r, c, rHelp = -1, cHelp = -1, round=0;
    while(inGame >= 0 && inGame != SEGUROS && round < 10){
        if (inGame==0){
            r = rand() % ROW;
            c = rand() % COL;
            *avaliableT = 1;
            clear_screen();
            print_board(ROW, COL, pt_board);
            printf("%d - Escolhendo coordenada (%d,%d)\n",(round+1),(r+1),(c+1));
            inGame = reveal(ROW,COL,r,c,pt_board,inGame,avaliableT,time);
            printf("inGame = %d",inGame);
            round++;
        } 
        else {
            if(inGame > 0){
                print_board(ROW, COL, pt_board);
                help(&rHelp,&cHelp,pt_board);


                printf("%d - Escolhendo coordenada (%d,%d)\n",(round+1),(r+1),(c+1));
                inGame = reveal(ROW,COL,r,c,pt_board,inGame,avaliableT,time);
                printf("inGame = %d",inGame);
                round++;
            }
            else if(inGame == 10 || inGame == -1){
                printf("Tempo decorrido: %.0f segundos.\n",get_time(time));
                printf("GAME OVER!\n");
            }
            else if(inGame == SEGUROS){
                printf("Tempo decorrido: %.0f segundos.\n",get_time(time));
                printf("A IA venceu!\n");
            }            
            printf("\n---------------------------------------------------\n");
            print_board(ROW, COL, pt_board);
        }
    }
}

houses * init_game(houses * pt_board, FILE * save, int * avaliableT, time_t * time){
    clear_screen();
    printf("\nInsira uma coodenada abaixo de de x = [1,%d] e y = [1,%d]\nDigite Coordenadas \"-1 -1\" para voltar ao menu\n",ROW,COL);
    print_board(ROW, COL, pt_board);
    int inGame = 0, r, c, rHelp = -1, cHelp = -1, avaliableH = 0, index = 1;
    while(inGame >= 0 && inGame != SEGUROS && index){
        scanf("%d %d",&r,&c);
        r--;
        c--;
        if(r < ROW && r >= 0 && c < COL && c >= 0){
            inGame = reveal(ROW,COL,r,c,pt_board,inGame,avaliableT,time);
            avaliableH = 1;
            clear_screen();
            if(inGame > 0){
                printf("\nInsira uma coodenada abaixo de de x = [1,%d] e y = [1,%d]\nDigite Coordenadas \"-1 -1\" para voltar ao menu\n\"-2 -2 para ver o tempo de jogo\"\n\"-3 -3\" para receber ajuda\n",ROW,COL);
            }
            else if(inGame == -1){
                printf("Tempo decorrido: %.0f segundos.\n",get_time(time));
                printf("GAME OVER!\n");
            }
            else if(inGame == SEGUROS){
                printf("Tempo decorrido: %.0f segundos.\n",get_time(time));
                printf("Parabéns, Você Ganhou!\n");
            }            
            print_board(ROW, COL, pt_board);
            
        }
        else if((r == -2) && (c == -2)){
            printf("\nDeseja Retornar ao Menu?\n0 - Sim\n1 - Não\n\n");
            int i;
            scanf("%d",&i);
            if(!i){
                clear_screen();
                index = 0;
                print_menu();
            }
            else{
                printf("\nInsira uma coodenada abaixo de de x = [1,%d] e y = [1,%d]\nDigite Coordenadas \"-1 -1\" para voltar ao menu\n\"-2 -2 para ver o tempo de jogo\"\n\"-3 -3\" para receber ajuda\n",ROW,COL);
            }
        }
        else if((r == -3) && (c == -3) && avaliableT){
            if (*avaliableT){printf("\nTempo decorrido: %.0f segundos.\n",get_time(time));}
        }
        else if((r == -4) && (c == -4) && avaliableH){
            help(&rHelp,&cHelp,pt_board);
            printf("\nTente uma casa ao redor das Coordenadas x = \"%d\" e y = \"%d\"\n\n",(rHelp+1),(cHelp+1));
            cHelp = -1;
            rHelp = -1;
        }
        else{
            printf("\nInsira uma Coordenada Válida\n");
        }
    }
    return pt_board;
}

void print_menu(){
    printf("Campo Minado\n\n");
	printf("Iniciar Novo Jogo - 0\n");
    printf("Iniciar BOT - 1\n");
	printf("Tempo Jogo - 2\n");
	printf("Sair - 3\n\n");
}

 void init_menu(houses * board,FILE * save, int avaliableT,time_t * time){   
	menu i = -1;
	int bufferI;
	print_menu();	
    while(i != 2){
        scanf("%d",&bufferI);
	    i = bufferI;
        switch(i){
            case START : 
                board = init_board(ROW,COL,QTDBOMBS,board);
                board = init_game(board, save, &avaliableT,time);
                break;
            case BOT :
                board = init_board(ROW,COL,QTDBOMBS,board);
                init_bot(board, save, &avaliableT,time);
                break;
            case TIME:
                if(avaliableT){
                    printf("Tempo decorrido: %.0f segundos.\n\n",get_time(time));
                }
                else {
                    clear_screen();
                    printf("Voce Nao tem nenhum jogo iniciado ainda!\n\n");
                    print_menu();           
                }
                break;
            case EXIT :
                printf("\nVou sentir Saudade :(\n");   
                break;
            default :
                printf("\nInsira um Número Valido\n");
        }
    }
    
}
