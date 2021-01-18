#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

int main(int argc, char *argv[]){
  int echo = 0;                                // controls echoing, 0: echo off,   1: echo on
  if(argc > 1 && strcmp("-echo",argv[1])==0) { // turn echoing on via -echo command line option
    echo=1;
  }
  printf("TreeMap Editor\n");
  printf("Commands:\n");
  printf("   quit:            exit the program\n");
  printf("   print:           shows contents of the tree in reverse sorted order\n");
  printf("   add <key> <val>: inserts the given key/val into the tree, duplicate keys are ignored\n");
  printf("   get <key>:       prints FOUND if the name is in the tree, NOT FOUND otherwise\n");
  printf("   clear:           eliminates all key/vals from the tree\n");
  printf("   preorder:        prints contents of the tree in pre-order which is how it will be saved\n");
  printf("   save <file>:     writes the contents of the tree in pre-order to the given file\n");
  printf("   load <file>:     clears the current tree and loads the one in the given file\n");
  
  char cmd[128];
  treemap_t tree;
  int success;
  treemap_init(&tree);
  char key[128]; 
  char val[128];
  char fname[128];

  while(1){ 
    printf("TM> ");       
    success = fscanf(stdin,"%s",cmd); // read a command
    if(success==EOF){                 // check for end of input
      printf("\n");                   // found end of input
      break;                          // break from loop
    }

    if( strcmp("quit", cmd)==0 ){     // check for exit command
      if(echo){
        printf("quit\n");
      }
      break;                          // break from loop
    }

    else if( strcmp("add", cmd)==0 ){ // to use add function (treemap_add)
      fscanf(stdin,"%s %s",key, val);            // read string to insert
      if(echo){
        printf("add %s %s\n",key, val);
      }
      success = treemap_add(&tree, key, val); // call add function
      if(!success){                      // check for success
        printf("modified existing key\n");
      }
    }

    else if( strcmp("get", cmd)==0 ){     // get command (treemap_get())
      fscanf(stdin,"%s",key);          // read an integer
      if(echo){
        printf("get %s\n",key);
      }

      char *getfun = treemap_get(&tree, key); // call get function
      if(getfun == NULL){                    // check for success
        printf("NOT FOUND\n");
      }
      else {
        printf("FOUND: %s\n",getfun);
      }
    }

    else if( strcmp("clear", cmd)==0 ){   // clear command (treemap_clear())
      if(echo){
        printf("clear\n");
      }
      treemap_clear(&tree);
    }

    else if( strcmp("print", cmd)==0 ){   // print command (treemap_print_revorder())
      if(echo){
        printf("print\n");
      }
      treemap_print_revorder(&tree);
    }
    else if (strcmp("preorder", cmd) == 0) { //print command (treemap_print_preorder())
      if (echo) {
        printf("preorder\n");
      }
      treemap_print_preorder(&tree);
    }
    else if (strcmp("save", cmd) == 0) { //save command (treemap_save)
      fscanf(stdin, "%s", fname);
      if (echo) {
        printf("save %s\n", fname);
      }
      treemap_save(&tree, fname);
    }
    else if (strcmp("load", cmd) == 0) { //load command (treemap_load)
      fscanf(stdin, "%s", fname);
      if (echo) {
         printf("load %s\n", fname);
       }
      treemap_load(&tree, fname);
    }
    else{            //unknown command                     
      if(echo){
        printf("%s\n",cmd);
      }
      printf("unknown command %s\n",cmd);
    }
  }  

  // end main while loop
  treemap_clear(&tree);                      // clean up the list
  return 0;
}