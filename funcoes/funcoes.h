#ifndef FUNCOES_H_
#define FUNCOES_H_

// Definição de tipos e estrturas devem estar num arquivo .h

void clear_screen();

void start_time(time_t * time);

double get_time(time_t * time);

void setBombs(int ROW, int COL, int QTDBOMBS, houses * pt_board);

void setNumbers(int ROW, int COL, houses * pt_board);

houses * init_board(int ROW, int COL, int QTDBOMBS, houses * board);

void print_board(int ROW, int COL, houses * board);

int reveal(int ROW, int COL, int indexR, int indexC ,houses * pt_board, int inGame, int * avaliableT, time_t * time);

float calc_prob(int r, int c, houses * board, float * total, int * ponteiro);

int help(int * r,int * c, houses * board, float * total, int * ponteiro);

void init_bot(houses * pt_board, int * avaliableT,time_t * time, int * end);

void init_game(houses * pt_board, int * avaliableT, time_t * time, int * end);

void print_menu();

void init_menu(houses * board, int avaliableT,time_t * time, int * end);


#endif
