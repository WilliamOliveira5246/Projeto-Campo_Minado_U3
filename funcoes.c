void setBombs(int row, int col, int qtdBombs, houses * pt_board){
    for (int b = 0; b<qtdBombs; b++){
        int slot = rand() % (row*col);
        if (pt_board[slot].face[1] != FBOMB){
            pt_board[slot].face[1] = FBOMB;
        }
        else b--;
    }         
}

void setNumbers(int row, int col, houses * pt_board){
     for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            if(pt_board[i*col+j].face[1] != FBOMB){
                int k = 0;
                for(int z = -1; z < 2;z++){
                    for(int x = -1; x < 2;x++){
                        if((i-z) <= row && (i-z) >= 0 && (j-x) <= col && (j-x)  >= 0){
                            if(pt_board[(i-z)*col+(j-x)].face[1] == FBOMB){
                                k++;
                            }
                        }
                    }
                }
                if(k > 0){
                    pt_board[i*col+j].face[1] = k + '0';
                }
                else{
                    pt_board[i*col+j].face[1] = FNULL;
                }
            }
        }
     }
}

houses* init_board(int row, int col, int qtdBombs){
    houses * board = (houses*) malloc(row*col*sizeof(houses));
    for(int i = 0; i < row;i++){
        for(int j = 0; j < col;j++){
            board[i*col+j].face[0] = FNOTREV;
            board[i*col+j].status = 0;
        }
    }
    setBombs(row,col,qtdBombs,board);
    setNumbers(row,col,board);
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

//   void reveal(int row, int col, int colT,houses * pt_board){
//       pt_board[row*colT+col].status = 1;
//       if(pt_board[row*colT+col].face != FBOMB){
//           if(pt_board[row*colT+col].face == FBOMB)
//       }
//   }