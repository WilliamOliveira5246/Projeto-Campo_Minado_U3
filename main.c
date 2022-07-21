#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "constantes/constantes.h"
#include "estruturas/estruturas.h"
#include "funcoes/funcoes.h"


int main(){
    int end=0;
    do {
        houses * board = NULL;
        srand(time(NULL));
        time_t time = 0;
        clear_screen();
        init_menu(board,0,&time,&end);
        free(board);
    }while (end==0);
    return 0;
}