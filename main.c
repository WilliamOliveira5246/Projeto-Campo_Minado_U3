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
    clear_screen();
    init_menu(board, save,0,0);
    return 0;
}