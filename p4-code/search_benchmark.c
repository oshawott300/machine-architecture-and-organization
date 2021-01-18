// Complete this main to benchmark the search algorithms outlined in
// the project specification
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "search.h"


int main(int argc, char *argv[]){
  clock_t begin, end;
  int run_linear_array = 0;
  int run_linear_list = 0;
  int run_binary_array = 0;
  int run_binary_tree = 0;
  if(argc == 4){
    run_linear_array = 1;
    run_linear_list = 1;
    run_binary_array = 1;
    run_binary_tree = 1;
  }
  else if (argc>4){
  int len = strlen(argv[4]);
  char algs_string[len];
  strcpy(algs_string, argv[4]);
  for(int i=0; i < len; i++){
    if(algs_string[i] == 'a'){
      run_linear_array = 1;
    }
    else if(algs_string[i] == 'l'){
      run_linear_list = 1;
    }
    else if(algs_string[i] == 'b'){
      run_binary_array = 1;
    }
    else if(algs_string[i] == 't'){
      run_binary_tree = 1;
    }
    }
  }
 
  
  int min = atoi(argv[1]);
  int max = atoi(argv[2]);
  int repeats = atoi(argv[3]);
  
  printf("%6s ","LENGTH");
  printf("%6s ","SEARCHES");
  if(run_linear_array){
    printf("%6s ","array");
  }
  if(run_linear_list){
    printf("%7s ","list");
  }
  if(run_binary_array){
    printf("%10s ","binary");
  }
  if(run_binary_tree){
    printf("%6s ","tree");
  }
  printf("\n");

  
  for(int i = min; i<=max;i++){
    int obj_length = 2;
    obj_length = obj_length << (i-1);
    printf("%6d ", obj_length);
    int REPEATS = (2*repeats) * obj_length;
    printf("%6d ", REPEATS);
    
    if(run_linear_array){
    int result=0;
    int *array = make_evens_array(obj_length);
    begin = clock();
    for(int j=0; j<repeats; j++){
      for(int k=0; k<(obj_length*2); k++ ){
      result += linear_array_search(array, obj_length, k);     
    }
    }
    end = clock();
    double cpu_time_linear_array = ((double) (end - begin)) / CLOCKS_PER_SEC;
    //print cpu_time... free(array).
    printf("%10.4e ",cpu_time_linear_array);
    free(array);
    if(result == (repeats*max)/2){
      printf("ERROR, does not work");
    }
    
  }
  if(run_linear_list){
    int result = 0;
    list_t *list = make_evens_list(obj_length);
    begin = clock();
    for(int j=0; j<repeats; j++){
      for(int k=0; k<(obj_length*2); k++ ){
      result += linkedlist_search(list, obj_length, k);     
    }
    }
    end = clock();
    double cpu_time_linked_list = ((double) (end - begin)) / CLOCKS_PER_SEC;
    printf("%10.4e ",cpu_time_linked_list);
    list_free(list);
    if(result == (repeats*max)/2){
      printf("ERROR, does not work");
    }
  }
  if(run_binary_array){
    int result = 0;
    int *array = make_evens_array(obj_length);
    begin = clock();
    for(int j=0; j<repeats; j++){
      for(int k=0; k<(obj_length*2); k++ ){
      result += binary_array_search(array, obj_length, k);      
    }
    }
    end = clock();
    double cpu_time_binary_array = ((double) (end - begin)) / CLOCKS_PER_SEC;
    printf("%10.4e ",cpu_time_binary_array);
    free(array);
    if(result == (repeats*max)/2){
      printf("ERROR, does not work");
    }
  }
  if(run_binary_tree){
    int result = 0; 
    bst_t *tree = make_evens_tree(obj_length);
    begin = clock();
    for(int j=0; j<repeats; j++){
      for(int k=0; k<(obj_length*2); k++ ){
      result += binary_tree_search(tree, obj_length, k); 
    }
    }
    end = clock();
    double cpu_time_binary_tree = ((double) (end - begin)) / CLOCKS_PER_SEC;
    printf("%10.4e ",cpu_time_binary_tree);
    bst_free(tree);
    if(result == (repeats*max)/2){
      printf("ERROR, does not work");
    }
  }
    printf("\n");
  }
  return 0;
  
  }

  
