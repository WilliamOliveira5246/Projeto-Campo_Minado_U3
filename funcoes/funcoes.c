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

int calc_prob(int r, int c, houses * board){
    int total = 0, prob;
    for(int i = -1; i < 2;i++){
        for(int j = -1; j < 2;j++){
            int rNew = r - i, cNew = c - j;
            if((rNew) < ROW && (rNew) >= 0 && (cNew) < COL && (cNew)  >= 0 &&  board[(rNew)*COL+(cNew)].status == 0){
                total++;
            }
        }
    }
    prob = 100 - (((board[r*COL+c].face[1] - '0')*10)/total);
    return prob;
}

void help(int * r,int * c, houses * board){
    int index = 0, rPos, cPos, buffer;
    houses * pos;
    for(int x = 0; x < ROW && index == 0;x++){
        for(int y = 0; y < COL && index == 0;y++){
            buffer = x*COL+y;
            if(board[index].status == 1){
                pos = &board[index];
                rPos = x, cPos = y;
                index = 1;                
            }
        }
    }
    int probM = calc_prob(rPos,cPos,board),probA;
    for(int x = 0; x < ROW;x++){
        for(int y = 0; y < COL;y++){
            buffer = x*COL+y;
            if(board[buffer].status == 1 && board[buffer].face[1] != FNULL){
                probA = calc_prob(x,y,board);
                if(probA > probM){                
                    pos = &board[buffer];
                    *r = x;
                    *c = y;
                }
            }        
        }
    }
}


houses * init_game(houses * pt_board, FILE * save){
    srand((unsigned int)time(NULL));  
    clear_screen();
    printf("Insira uma coodenada abaixo de de x = [1,%d] e y = [1,%d]\nDigite as Coordenadas \"-1 -1\" para voltar ao menu\nDigite as Coordenadas \"-2 -2\" para receber ajuda\n",ROW,COL);
    print_board(ROW, COL, pt_board);
    int inGame = 0, r, c, rHelp = -1, cHelp = -1;
    while(inGame >= 0 && inGame != SEGUROS){
        scanf("%d %d",&r,&c);
        r--;
        c--;
        if(r < ROW && r >= 0 && c < COL && c >= 0){
            inGame = reveal(ROW,COL,r,c,pt_board,inGame);
            clear_screen();
            if(inGame > 0){
                printf("Insira uma coodenada abaixo de de x = [1,%d] e y = [1,%d]\nDigite as Coordenadas \"-1 -1\" para voltar ao menu\nDigite as Coordenadas \"-2 -2\" para receber ajuda\n",ROW,COL);
            }
            else if(inGame == -1){
                printf("GAME OVER!\n");
            }
            else if(inGame == SEGUROS){
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
                init_menu(pt_board, save,1,1);
            }
        }
        else if((r == -3) && (c == -3)){
            help(&rHelp,&cHelp,pt_board);
            clear_screen();
            printf("Insira uma coodenada abaixo de de x = [1,%d] e y = [1,%d]\nDigite as Coordenadas \"-1 -1\" para voltar ao menu\nDigite as Coordenadas \"-2 -2\" para receber ajuda\n",ROW,COL);
            print_board(ROW, COL, pt_board);
            printf("Tente uma das Casas bloqueadas ao redor da casa nas Coordenadas x = \"%d\" e y = \"%d\"\n",rHelp,cHelp);
            cHelp = -1; 
            rHelp = -1;
            
        }
        else{
            printf("\nInsira uma Coordenada Válida\n");
        }
    }
    return pt_board;
}

 void init_menu(houses * board, FILE * save,int avaliableL,int avaliableS){   
     menu i = -1;
     int bufferI;
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
             board = init_game(board, save);
             break;
         case SAVE :
            if(avaliableS){

            }
            else{
                clear_screen();
                printf("Você Não pode Salvar o Jogo ainda!\n\n");
                init_menu(board,save,0,0);
            }
             break;
         case LOAD :
            if(avaliableL){

            }
            else{
                clear_screen();
                printf("Você Não tem nenhum jogo iniado ainda!\n\n");
                init_menu(board,save,0,0);
            }
             break;
         case EXIT :
             printf("\nVou sentir Saudade :(\n");
             break;
         default :
             printf("Insira um Número Valido");

     }
    
 }


