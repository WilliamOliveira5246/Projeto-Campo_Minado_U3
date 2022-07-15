#ifndef FUNCOES_H_
#define FUNCOES_H_

// Definição de tipos e estrturas devem estar num arquivo .h

void clear_screen();

void setBombs(int ROW, int COL, int QTDBOMBS, houses * pt_board);

void setNumbers(int ROW, int COL, houses * pt_board);

houses * init_board(int ROW, int COL, int QTDBOMBS, houses * board);

void print_board(int ROW, int COL, houses * board);

int reveal(int ROW, int COL, int indexR, int indexC ,houses * pt_board, int inGame);

houses * init_game(houses * pt_boar, FILE * save);

void init_menu(houses * board,FILE * save,int avaliableL,int avaliableS);


#endif
