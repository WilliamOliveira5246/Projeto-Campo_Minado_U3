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

void start_time(time_t * t){time(t);}

double get_time(time_t * t){return time(NULL) - *t;}

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

int reveal(int ROW, int COL, int indexR, int indexC ,houses * pt_board, int inGame, int * avaliableT, time_t * time){
	if(inGame >= 0 && inGame < ((ROW*COL) - QTDBOMBS)){
		if (inGame==0){
			*avaliableT = 1;
			start_time(time);
		}
		int pos = indexR*COL+indexC;
		pt_board[pos].status = 1;
		if(pt_board[pos].face[1] != FBOMB){
			inGame++;
			if(pt_board[pos].face[1] == FNULL){
				for(int i = -1; i < 2;i++){
					for(int j = -1; j < 2;j++){
						int rNew = indexR - i, cNew = indexC - j;
						if((rNew) < ROW && (rNew) >= 0 && (cNew) < COL && (cNew)  >= 0 &&  pt_board[(rNew)*COL+(cNew)].status == 0){
							reveal(ROW,COL,(rNew),(cNew),pt_board,inGame,avaliableT,time);
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

float calc_prob(int r, int c, houses * board, float * total, int * mat){
	*total = 0;
	int t = 0;
	float prob;
	for(int i = -1; i < 2;i++){
		for(int j = -1; j < 2;j++){
			int rNew = r - i, cNew = c - j;
			if((rNew) < ROW && (rNew) >= 0 && (cNew) < COL && (cNew)  >= 0){
				if(board[(rNew)*COL+(cNew)].status == 0){
					(mat[t*2]) = rNew;
					(mat[t*2+1]) = cNew;
					t++;
					(*total)++;
				}                
			}
		}
	}
	prob = 1 - ((board[r*COL+c].face[1] - '0')/(*total));
	return prob;
}

int help(int * r,int * c, houses * board,float * total, int * mat){
	int index = 0, buffer, cont = 0,* m;
	float  count = 0;
	m = (int*) malloc(16*sizeof(int));
	houses * pos;
	for(int x = 0; x < ROW && index == 0;x++){
		for(int y = 0; y < COL && index == 0;y++){
			buffer = x*COL+y;
			if(board[buffer].status == 1 && board[buffer].face[1] != FNULL){
				pos = &(board[buffer]);
				*r = x, *c = y;
				index = 1;                
			}
		}
	}
	float probM = calc_prob(*r,*c,board,total,mat),probA;
	for(int x = 0; x < ROW;x++){
		for(int y = 0; y < COL;y++){
			buffer = x*COL+y;
			if(board[buffer].status == 1 && board[buffer].face[1] != FNULL){
				probA = calc_prob(x,y,board,&count,m);
				if(probA > probM){ 
					for(int i = 0; i < count;i++){                        
						mat[i*2] = m[i*2];
						mat[i*2+1] = m[i*2+1];
					}   
					*total = count;            
					probM = probA;
					pos = &(board[buffer]);
					*r = x, *c = y;
				}
			}        
		}
	}
	free(m);
	if(probM <= 0.5){
		cont = 1;
	}
	return cont;
}

void init_bot(houses * pt_board, int * avaliableT,time_t * time, int * end){
	int inGame = 0, r, c, rHelp = -2, cHelp = -2, round=0, cont,t ,rNew,cNew, buffer = 0;
	float total;
	while(inGame >= 0 && inGame != SEGUROS){
		if (inGame==0){
			r = rand() % ROW;
			c = rand() % COL;
			*avaliableT = 1;
			clear_screen();
			print_board(ROW, COL, pt_board);
			printf("%d - Escolhendo coordenada (%d,%d)\n",(round+1),(r+1),(c+1));
			inGame = reveal(ROW,COL,r,c,pt_board,inGame,avaliableT,time);
			round++;
		} 
		else {
			if(inGame > 0){
				int * mat =(int*) malloc(16*sizeof(int));
				cont = help(&rHelp,&cHelp,pt_board,&total,mat);
				if(cont){
					free(mat);
					mat =(int*) malloc(200*sizeof(int));
					int index;
					t = 0;
					for(int x = 0; x < ROW;x++){
						for(int y = 0; y < COL;y++){
							index = 1;
							for(int i = -1; i < 2;i++){
								for(int j = -1; j < 2;j++){
									rNew = x - i, cNew = y - j;
									if((rNew) < ROW && (rNew) >= 0 && (cNew) < COL && (cNew)  >= 0){
										if(pt_board[rNew*COL+cNew].status == 1){
											index = 0;
										}
									}
								}
							}
							if(index){                                                              
								mat[t*2] = x;
								mat[t*2+1] = y;
								t++;
							}
						}
					}                     
				}
				else{
					t = (int) total;
				}
				int p = rand() % t;
				r = mat[p*2];
				c = mat[p*2+1]; 
				free(mat);
				printf("%d - Escolhendo coordenada (%d,%d)\n",(round+1),(r+1),(c+1));
				inGame = reveal(ROW,COL,r,c,pt_board,inGame,avaliableT,time);
				round++;
			}

		}
		if(inGame == -1){
				printf("GAME OVER!\n");
				printf("A IA jogou por %d Rodadas",round);
				buffer = 1;
		}
		else if(inGame == SEGUROS){
				printf("A IA venceu!\n");
				printf("A IA jogou por %d Rodadas",round);
				buffer = 1;
		}            
		printf("\n---------------------------------------------------\n");
		print_board(ROW, COL, pt_board);
		if(buffer==1){
			do{
				printf("\nDeseja retornar ao menu? 0 - sim / 1 - nao\n");
				scanf("%d",end);                             
			} while(*end != 1 && *end != 0);
		}
	}
}

void init_game(houses * pt_board, int * avaliableT, time_t * time, int * end){
	clear_screen();
	*avaliableT = 0;
	print_board(ROW, COL, pt_board);
	float lixo;
	int inGame = 0, r, c, rHelp = -1, cHelp = -1, avaliableH = 0, fim_while = 0,cont,* lixomaior = (int*) malloc(16*sizeof(int)),isEndPlay = 0;
	while(inGame >= 0 && inGame != SEGUROS){
		printf("\n>>> Insira uma coodenada igual ou entre de x = [1,%d] e y = [10,%d]\nDigite:\n\"-1 -1\" para encerrar o jogo\n\"-2 -2\" para ver o tempo de jogo\n\"-3 -3\" para receber ajuda\n",ROW,COL);
		scanf("%d %d",&r,&c);
		r--;
		c--;
		if(r < ROW && r >= 0 && c < COL && c >= 0){
			inGame = reveal(ROW,COL,r,c,pt_board,inGame,avaliableT,time);
			avaliableH = 1;
			if(inGame > 0){
				clear_screen();
				//printf("\n>>> Insira uma coodenada abaixo de de x = [1,%d] e y = [10,%d]\nDigite:\n\"-1 -1\" para encerrar o jogo\n\"-2 -2\" para ver o tempo de jogo\n\"-3 -3\" para receber ajuda\n",ROW,COL);
				print_board(ROW, COL, pt_board);
			}
			else if(inGame == -1){
				clear_screen();
				print_board(ROW, COL, pt_board);
				printf(">>> GAME OVER!\n");
				printf(">>> Tempo decorrido: %.0f segundos.\n",get_time(time));
				isEndPlay = 1;
			}
			else if(inGame == SEGUROS){
				clear_screen();
				print_board(ROW, COL, pt_board);
				printf(">>> Parabéns, Você Ganhou!\n");
				printf(">>> Tempo decorrido: %.0f segundos.\n",get_time(time));
				isEndPlay = 1;
			}
			if(isEndPlay==1){
				do{
					printf("\n>>> Deseja retornar ao menu? 0 - sim / 1 - nao\n");
					scanf("%d",end);                             
				} while(*end != 1 && *end != 0);
			}
		}
		else if((r == -2) && (c == -2)){
			do{
				printf("\n>>> Deseja retornar ao menu? 0 - sim / 1 - nao\n");
				scanf("%d",end);                             
			} while(*end != 1 && *end != 0);
			inGame = -1;
		}
		else if((r == -3) && (c == -3)){
			if (*avaliableT==1){
				clear_screen();
				print_board(ROW, COL, pt_board);
				printf("\n>>> Tempo decorrido: %.0f segundos.\n",get_time(time));
			}
			else {
				clear_screen();
				print_board(ROW, COL, pt_board);
				printf("\n>>> Jogo ainda nao iniciado\n");
			}
		}
		else if((r == -4) && (c == -4)){
			if (avaliableH==1){
				clear_screen();
				print_board(ROW, COL, pt_board);
				cont = help(&rHelp,&cHelp,pt_board,&lixo,lixomaior);
				if(!cont){
					printf("\n>>> Tente uma casa ao redor das Coordenadas x = \"%d\" e y = \"%d\"\n",(rHelp+1),(cHelp+1));
				}
				else{
					printf("\n>>> Tente uma casa aleatória longe das abertas\n");
				}
				cHelp = -1;
				rHelp = -1;
			}
			else {
				clear_screen();
				print_board(ROW, COL, pt_board);
				printf("\n>>> Nao tenha medo de dar o primeiro passo! :)\n");
			}
		}
		else{
			printf("\n>>> Insira uma Coordenada Valida\n");
		}
	}
	free(lixomaior);
}

void print_menu(){
	printf("Campo Minado\n\n");
	printf("Iniciar Novo Jogo - 0\n");
	printf("Iniciar BOT - 1\n");
	printf("Sair - 2\n\n");
}

void init_menu(houses * board, int avaliableT,time_t * time, int * end){   
	menu i = -1;
	int bufferI;
	do {
		if (*end==0){
			clear_screen();
			print_menu();	
			scanf("%d",&bufferI);
			i = bufferI;
		}
		else {i=2;}
		switch(i){
			case START : 
				board = init_board(ROW,COL,QTDBOMBS,board);
				init_game(board, &avaliableT,time,end);
				break;
			case BOT :
				board = init_board(ROW,COL,QTDBOMBS,board);
				init_bot(board, &avaliableT,time,end);
				break;
			case EXIT :
				*end = 1;
				clear_screen();
				printf("\nVou sentir saudade :(\n");
				break;
			default :
				printf("\nInsira uma Opcao valida!\n");
				break;
		}
	} while(i!=2);
}
