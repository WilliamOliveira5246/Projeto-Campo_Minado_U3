#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

    //1 - 8
    // board[0][0] = 1;
    // board[0][1] = 2;
    // board[0][2] = 3;
    // board[0][3] = 4;
    // board[0][4] = 5;
    // board[0][5] = 22;
    // board[0][6] = 15; //bomba
    // board[0][7] = 42;

typedef struct {
    char face[3];
    int status;
}houses;


void setBombs(int row, int col, int qtdBombs, houses * pt_board){
    for (int b = 0; b<qtdBombs; b++){
        //int slot = rand() % row*col;
        int slot = b;
        if (pt_board[slot].face[1] != 15){
            pt_board[slot].face[1] = 15;
        }
        else b--;
    }         
}


houses* init_board(int row, int col, int qtdBombs){
    houses * board = (houses*) malloc(row*col*sizeof(houses));
    for(int i = 0; i < row;i++){
        for(int j = 0; j < col;j++){
            board[i*col+j].face[0] = '#';
            board[i*col+j].face[1] = '#';//
            board[i*col+j].status = 0;
        }
    }
    setBombs(row,col,qtdBombs,board);
    return board;
}


 void print_board(int row, int col, houses * board){
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            printf("| %c ",board[i*col+j].face[1]);
        }
        printf("|\n");
     }
 }


int main(){

    int row = 10;
    int col = 20;
    int qtdBombs = 40; //confirmar
    int op = 0; 
    int fim = 0;
    
    houses * board = NULL;
    //srand((unsigned int)time(NULL));
    
    board = init_board(row,col,qtdBombs);
    print_board(row, col, board);
    
    free(board);
    return 0;
}