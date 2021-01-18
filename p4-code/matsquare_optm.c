// optimized versions of matrix diagonal summing
#include "matvec.h"

int matsquare_VER1(matrix_t mat, matrix_t matsq) {
  for(int i=0; i<mat.rows; i++){
    for(int j=0; j<mat.cols; j++){
      MSET(matsq,i,j,0);                         // initialize to 0s
    }
  }
  for(int i=0; i<mat.rows; i++){
    for(int k=0; k<mat.rows; k++){ //changes it to re-order the memory access pattern to favor cache
      for(int j=0; j<mat.cols; j++){
        int lead = MGET(mat, i, k); //being row major, we have it where it goes through rows first, makes it go sequentially in memory
          int mkj = MGET(mat, k, j);
          int mulval = lead * mkj;
          int cur = MGET(matsq, i ,j);
          int new = cur + mulval;
          MSET(matsq,i,j,new);
        
  
      
        
         }
    }
  }
  return 0;
}

int matsquare_VER2(matrix_t mat, matrix_t matsq) {
  for(int i=0; i<mat.rows; i++){
    for(int j=0; j<mat.cols; j++){
      MSET(matsq,i,j,0); // initialize to 0s
    }
  }
  for(int i=0; i<mat.rows; i++){
    for(int k=0; k<mat.rows; k++){
      for(int j=0; j<mat.cols; j++){
        int lead = MGET(mat, i, k);
          int mkj = MGET(mat, k, j);
          int cur = MGET(matsq, i ,j);
          int new = cur + lead*mkj; //getting rid of mulval from above to try to simplify arithmetic a bit
          MSET(matsq,i,j,new);
        
  
      
        
         }
    }
  }
  return 0;
}

int matsquare_VER3(matrix_t mat, matrix_t matsq) {
  for(int i=0; i<mat.rows; i++){
    for(int j=0; j<mat.cols; j++){
      MSET(matsq,i,j,0);                         // initialize to 0s
    }
  }
  for(int i=0; i<mat.rows; i++){
    for(int k=0; k<mat.rows; k++){
      int lead = MGET(mat, i, k); //finding the lead first before we get to the columns
      for(int j=0; j<mat.cols; j++){
          int cur = MGET(matsq, i ,j) + lead*MGET(mat, k, j); //just simplifying all the arithmetic down to one line, don't have to go back to other variables
          MSET(matsq,i,j,cur);
          }
    }
  }
  return 0;
}

int matsquare_VER4(matrix_t mat, matrix_t matsq) {
  int j;
  for(int i=0; i<mat.rows; i++){
    for(int j=0; j<mat.cols; j++){
      MSET(matsq,i,j,0);   // initialize to 0s
    }
  }
  for(int i=0; i<mat.rows; i++){
    for(int k=0; k<mat.rows; k++){
      int lead = MGET(mat, i, k);
      for(j=0; j<(mat.cols-4); j+=4){ //unrolling the loop by "going by groups of 4", helps with pipelining
          int cur0 = MGET(matsq, i ,j) + lead*MGET(mat, k, j);
          MSET(matsq,i,j,cur0);

          int cur1 = MGET(matsq, i ,j+1) + lead*MGET(mat, k, j+1);
          MSET(matsq,i,j+1,cur1);

          int cur2 = MGET(matsq, i ,j+2) + lead*MGET(mat, k, j+2);
          MSET(matsq,i,j+2,cur2);

          int cur3 = MGET(matsq, i ,j+3) + lead*MGET(mat, k, j+3);
          MSET(matsq,i,j+3,cur3);
          }
          for(; j<mat.cols; j++){ //finishes up remainder of elements in matrix if not by a factor of 4
           int cur = MGET(matsq, i ,j) + lead*MGET(mat, k, j);
          MSET(matsq,i,j,cur);
         }
    }
  }
  return 0;
}

int matsquare_OPTM(matrix_t mat, matrix_t matsq) {
  if(mat.rows != mat.cols ||                       // must be a square matrix to square it
     mat.rows != matsq.rows || mat.cols != matsq.cols)
  {
    printf("matsquare_OPTM: dimension mismatch\n");
    return 1;
  }


  // Call to some version of optimized code
  return matsquare_VER4(mat, matsq);
}
