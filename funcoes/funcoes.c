#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../constantes/constantes.h"
#include "../estruturas/estruturas.h"
#include "funcoes.h"


void clear_screen(){
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

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

int reveal(int ROW, int COL, int indexR, int indexC ,houses * pt_board, int inGame){
    if(inGame >= 0 && inGame < ((ROW*COL) - QTDBOMBS)){
        int pos = indexR*COL+indexC;
        pt_board[pos].status = 1;
        if(pt_board[pos].face[1] != FBOMB){
            inGame++;
            if(pt_board[pos].face[1] == FNULL){
                for(int i = -1; i < 2;i++){
                    for(int j = -1; j < 2;j++){
                        int rNew = indexR - i, cNew = indexC - j;
                        if((rNew) < ROW && (rNew) >= 0 && (cNew) < COL && (cNew)  >= 0 &&  pt_board[(rNew)*COL+(cNew)].status == 0){
                            reveal(ROW,COL,(rNew),(cNew),pt_board,inGame);
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

houses * init_game(houses * pt_board){
    srand((unsigned int)time(NULL));  
    printf("\nInsira uma coodenada abaixo de de x = [1,%d] e y = [1,%d]\nDigite Coordenadas \"-1 -1\" pra voltar ao menu",ROW,COL);
    print_board(ROW, COL, pt_board);
    int inGame = 0, r, c;
    while(inGame >= 0 && inGame != SEGUROS){
        scanf("%d %d",&r,&c);
        r--;
        c--;
        if(r < ROW && r >= 0 && c < COL && c >= 0){
            inGame = reveal(ROW,COL,r,c,pt_board,inGame);
            if(inGame == -1){
                printf("\nGAME OVER!");
            }
            else if(inGame == SEGUROS){
                printf("\nParabéns, Você Ganhou!");
            }
            print_board(ROW, COL, pt_board);
        }
        else if((r == -2) && (c == -2)){
            printf("\nDeseja Retornar ao Menu?\n0 - Sim\n1 - Não\n\n");
            int i;
            scanf("%d",&i);
            if(!i){
                init_menu(pt_board);
            }
            else{
                printf("\nInsira uma coodenada abaixo de de x = [1,%d] e y = [1,%d]\nDigite Coordenadas \"-1 -1\" pra voltar ao menu\n\n",ROW,COL);

            }
        }
        else{
            printf("\nInsira uma Coordenada Válida\n");
        }
    }
    return pt_board;
}

 void init_menu(houses * board){   
     menu i = -1;
     int bufferI;
     clear_screen();
     printf("Campo Minado\n\n");
     printf("Iniciar Novo Jogo - 0\n");
     printf("Salvar Jogo - 1\n");
     printf("Carregar Jogo - 2\n");
     printf("Sair - 3\n\n");
     scanf("%d",&bufferI);
     i = bufferI;
     switch(i){
         case START : 
             board = init_board(ROW,COL,QTDBOMBS,board);
             board = init_game(board);
             break;
         case SAVE :
             break;
         case LOAD :
             break;
         case EXIT :
             break;
         default :
             printf("Insira um Número Valido");

     }
    
 }
