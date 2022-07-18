#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "constantes/constantes.h"
#include "estruturas/estruturas.h"
#include "funcoes/funcoes.h"


int main(){
    FILE * save;
    houses * board = NULL;
    srand(time(NULL));
    clock_t time = 0;
    clear_screen();
    init_menu(board,save,0,0,0,&time);
    return 0;
}