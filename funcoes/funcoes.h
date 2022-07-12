#ifndef FUNCOES_H_
#define FUNCOES_H_

// Definição de tipos e estrturas devem estar num arquivo .h

void setBombs(int row, int col, int qtdBombs, houses * pt_board);

void setNumbers(int row, int col, houses * pt_board);

houses* init_board(int row, int col, int qtdBombs);

void print_board(int row, int col, houses * board);

int reveal(int ROW, int COL, int indexR, int indexC ,houses * pt_board, int inGame);

houses * init_game(houses * pt_board);

#endif
