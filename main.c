#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** init_board(int row, int col){
    char ** board = (char **) malloc(row*sizeof(char*));
    for(int i=0; i<row; i++){
        board[i] = (char *) malloc(col*sizeof(char));
        memset(board[i], ' ', col*sizeof(char));
    }
    //Necessita inserir código de bombas aleatorias
    return board;
}

void print_board(int row, int col, char ** board){
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            printf("| %c ",board[i][j]);
        }
        printf("|\n");
    }
}

int main(){

    int row = 10;
    int col = 20;
    int op = 0;
    int fim = 0;
    
    char ** board = NULL;
    board = init_board(row,col);
    
    board[0][0] = 1;
    board[0][1] = 2;
    board[0][2] = 3;
    board[0][3] = 4;
    board[0][4] = 5;
    board[0][5] = 22;

    while (fim == 0){
        print_board(row, col, board);
        printf("\nMenu: \n1 - Revelar\n2 - Tempo\n3 - Ajuda\n4 - Fim\n\n");
        scanf(" %d",&op);
        switch ( op ){
            case 1 :
            printf("revelar");
            break;
            
            case 2 :
            printf("tempo");
            break;

            case 3 :
            printf("ajuda");
            break;

            case 4 :
            printf("fim");
            fim = 1;
            break;

            default :
            printf("erro");
            break;
        }
        printf("\n");
        op = 0;
    }
    
    return 0;
}