#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "constantes/constantes.h"
#include "estruturas/estruturas.h"
#include "funcoes/funcoes.h"


//        1 - 8
//         board[0][0] = 1;
//         board[0][1] = 2;
//         board[0][2] = 3;
//         board[0][3] = 4;
//         board[0][4] = 5;
//         board[0][5] = 22;
//         board[0][6] = 15;  bomba
//         board[0][7] = 42;

// const char FBOMB = '*', FNOTREV = '#', FNULL = ' ';

// const int ROW = 10, COL = 20, QTDBOMBS = 40;


// typedef struct {
//     char face[3];
//     int status; /* "0" : !revelado "1" : revelado */
// }houses;


// void setBombs(int ROW, int COL, int QTDBOMBS, houses * pt_board){
//     for (int b = 0; b < QTDBOMBS; b++){
//         int slot = rand() % (ROW*COL);
//         if (pt_board[slot].face[1] != FBOMB){
//             pt_board[slot].face[1] = FBOMB;
//         }
//         else b--;
//     }         
// }

// void setNumbers(int ROW, int COL, houses * pt_board){
//     for(int i = 0; i < ROW; i++){
//         for(int j = 0; j < COL; j++){
//             if(pt_board[i*COL+j].face[1] != FBOMB){
//                 int count = 0;
//                 for(int x = -1; x < 2; x++){
//                     for(int y = -1; y < 2; y++){
//                         if((i-x) < ROW && (i-x) >= 0 && (j-y) < COL && (j-y)  >= 0){
//                             if(pt_board[(i-x)*COL+(j-y)].face[1] == FBOMB){
//                                 count++;
//                             }
//                         }
//                     }
//                 }
//                 if(count > 0){
//                     pt_board[i*COL+j].face[1] = count + '0';
//                 }
//                 else{
//                     pt_board[i*COL+j].face[1] = FNULL;
//                 }
//             }
//         }
//     }
// }
    

// houses* init_board(int ROW, int COL, int QTDBOMBS){
//     houses * board = (houses*) malloc(ROW*COL*sizeof(houses));
//     for(int i = 0; i < ROW;i++){
//         for(int j = 0; j < COL;j++){
//             board[i*COL+j].face[0] = FNOTREV;
//             board[i*COL+j].status = 0;
//         }
//     }
//     setBombs(ROW,COL,QTDBOMBS,board);
//     setNumbers(ROW,COL,board);
//     return board;
// }


// void print_board(int ROW, int COL, houses * board){
//     printf("\n");
//     for(int i=0; i<ROW; i++){
//         for(int j=0; j<COL; j++){
//             printf("| %c ",board[i*COL+j].face[board[i*COL+j].status]);
//         }
//         printf("|\n");
//     }
// }

// int reveal(int ROW, int COL, int indexR, int indexC ,houses * pt_board, int inGame){
//     if(inGame){
//         int pos = indexR*COL+indexC;
//         pt_board[pos].status = 1;
//         if(pt_board[pos].face[1] != FBOMB){
//             if(pt_board[pos].face[1] == FNULL){
//                 for(int i = -1; i < 2;i++){
//                     for(int j = -1; j < 2;j++){
//                         int rNew = indexR - i, cNew = indexC - j;
//                         if((rNew) < ROW && (rNew) >= 0 && (cNew) < COL && (cNew)  >= 0 &&  pt_board[(rNew)*COL+(cNew)].status == 0){
//                             reveal(ROW,COL,(rNew),(cNew),pt_board,inGame);
//                         }
//                     }
//                 }
//             }
//         }
//         else{
//             inGame = 0;
//             printf("GAME OVER!\n\n");
//         }
//     }
//     return inGame;
// }


int main(){
    houses * board = gameInit(board);
    free(board);
    return 0;
}