// treemap_funcs.c: Provides a small library of functions that operate on
// binary search trees mapping strings keys to string values.

#include "treemap.h"

void treemap_init(treemap_t *tree){
// Initialize the given tree to have a null root and have size 0.
tree -> root = NULL;
tree -> size = 0; 
}

int treemap_add(treemap_t *tree, char key[], char val[]){
    node_t *current_node = tree->root;
    if(tree -> root == NULL){ //if root is empty
        node_t *nodepointer = malloc(sizeof(node_t));//have the node pointer to traverse tree
        strcpy(nodepointer -> key, key); //set values to the node pointer
        strcpy(nodepointer -> val, val);
        nodepointer ->right = NULL; //sets left and right childs to null
        nodepointer -> left = NULL;
        tree->root = nodepointer; //have root now equal the pointer so it can hold the values.
        tree->size += 1;
        return 1;
    }
        while(1){
            int compare_nodeval = strcmp(key, current_node ->key); //variable to compare keys so nodes are sorted.
            if(compare_nodeval == 0){ //if keys are equal each other. 
                strcpy(current_node->val, val);
                return 0;
            }
            else if(compare_nodeval < 0){// go to left branch
                if(current_node -> left == NULL){
                    node_t *nodepointer = malloc(sizeof(node_t));//same thing as above
                    strcpy(nodepointer -> key, key);
                    strcpy(nodepointer -> val, val);
                    nodepointer ->right = NULL;
                    nodepointer -> left = NULL;
                    current_node -> left = nodepointer; //only now point current_node's left child to be the pointer
                    tree->size += 1;
                    return 1;
                }
                else {
                    current_node = current_node -> left; // move current node down to the left child. 
                }

            }
            else if(compare_nodeval > 0){//go to right branch
                if(current_node -> right == NULL){
                    node_t *nodepointer = malloc(sizeof(node_t));//same thing as above
                    strcpy(nodepointer -> key, key);
                    strcpy(nodepointer -> val, val);
                    nodepointer ->right = NULL;
                    nodepointer -> left = NULL;
                    current_node -> right = nodepointer;//only now point current_node's right child to be the pointer
                    tree->size += 1;
                    return 1;
                }
                else {
                    current_node = current_node -> right;// move current node down to the right child. 
                }
                

            }
            
        }
    
}
    
// Inserts given key/value into a binary search tree. Uses an
// ITERATIVE (loopy) approach to insertion which starts a pointer at
// the root of the tree and changes its location until the correct
// insertion point is located. If the key given already exists in the
// tree, no new node is created; the existing value is changed to the
// parameter 'val' and 0 is returned.  If no node with the given key
// is found, a new node is created and with the given key/val, added
// to the tree, and 1 is returned. Makes use of strcpy() to ease
// copying characters between memory locations.


char *treemap_get(treemap_t *tree, char key[]){
    node_t *current_node = tree->root;
    while(current_node!=NULL){//if NULL (hit the end of the tree), doesn't go into the while loop, goes return NULL.
        int compare_nodeval = strcmp(key, current_node -> key); // to compare values

        if(compare_nodeval == 0){//keys are the same, return that key
            return current_node->val;
        }
        if (compare_nodeval < 0){//go down left child in order to get key
            current_node = current_node->left;
        }
        if (compare_nodeval > 0){//go down right child in order to get key
            current_node = current_node->right;
        }
    }
    return NULL;
    }

// Searches the tree for given 'key' and returns its associated
// value. Uses an ITERATIVE (loopy) search approach which starts a
// pointer at the root of the tree and changes it until the search key
// is found or determined not to be in the tree. If a matching key is
// found, returns a pointer to its value. If no matching key is found,
// returns NULL.

void treemap_clear(treemap_t *tree){
    node_remove_all(tree->root);//calls recursive method
    tree -> root = NULL;//root is NULL and size is 0 so there is no tree
    tree -> size = 0;
    return;
}
// Eliminate all nodes in the tree setting its contents empty. Uses
// recursive node_remove_all() function to free memory for all nodes.

void node_remove_all(node_t *cur){
    if(cur == NULL){
        return;
    }
    node_remove_all(cur -> left);//recursing to left
    node_remove_all(cur -> right);//recursing to right
    free(cur);//frees memory of the node
    
}


// Recursive helper function which visits all nodes in a tree and
// frees the memory associated with them. This requires a post-order
// traversal: visit left tree, visit right tree, then free the cur
// node.

void treemap_print_revorder(treemap_t *tree){
    node_print_revorder(tree -> root, 0); //calls on recursive function
}
// Prints the key/val pairs of the tree in reverse order at differing
// levels of indentation which shows all elements and their structure
// in the tree. Visually the tree can be rotated clockwise to see its
// structure. See the related node_print_revorder() for additional
// detals.

void node_print_revorder(node_t *cur, int indent){
    if(cur==NULL){
        return;
    }
    node_print_revorder(cur->right, indent+1);//recursing to right
    for(int i=0; i<indent; i++){ // prints out the spaces to show hierarchy of tree.
        printf(" ");
    }
    printf("%s -> %s\n", cur->key, cur->val);//will just print out keys with correspoding values. 
    node_print_revorder(cur ->left, indent+1);//recursing to left
    return;

}
// Recursive helper function which prints all key/val pairs in the
// tree rooted at node 'cur' in reverse order. Traverses right
// subtree, prints cur node's key/val, then traverses left tree.
// Parameter 'indent' indicates how far to indent (2 spaces per indent
// level).
//
// For example: a if the root node "El" is passed into the function
// and it has the following structure:
// 
//         ___El->strange_____     
//        |                   |   
// Dustin->corny       ___Mike->stoic
//                    |              
//               Lucas->brash     
// 
// the recursive calls will print the following output:
// 
//   Mike -> stoic                 # root->right
//     Lucas -> brash              # root->right->left
// El -> strange                   # root
//   Dustin -> corny               # root->left

void treemap_print_preorder(treemap_t *tree){
    node_write_preorder(tree -> root, stdout, 0); //calls on recursive function
}
// Print all the data in the tree in pre-order with indentation
// corresponding to the depth of the tree. Makes use of
// node_write_preorder() for this.

void treemap_save(treemap_t *tree, char *fname){
    FILE *fp = fopen(fname, "w");//open file
    node_write_preorder(tree->root, fp, 0);//writes the tree into file. 
    fclose(fp);//close file
}
// Saves the tree by opening the named file, writing the tree to it in
// pre-order with node_write_preorder(), then closing the file.

void node_write_preorder(node_t *cur, FILE *out, int depth){
    if (cur == NULL){
        return;
    }
    for(int i = 0; i<depth; i++){//print out spaces to show hierarchy of tree
        fprintf(out, " ");
    }
    fprintf(out, "%s %s\n", cur->key, cur->val); // print the keys and values
    node_write_preorder(cur->left, out, depth+1); // recurse to the left
    node_write_preorder(cur->right, out, depth+1);// recurse to the right
    return;
}
// Recursive helper function which writes/prints the tree in pre-order
// to the given open file handle. The parameter depth gives how far to
// indent node data, 2 spaces per unit depth. Depth increases by 1 on
// each recursive call. The function prints the cur node data,
// traverses the left tree, then traverses the right tree.

int treemap_load(treemap_t *tree, char *fname ){
    FILE *fp = fopen(fname, "r");
    if(fp==NULL){//check if file can be opened or not
        printf("ERROR: could not open file '%s'\n", fname);
        printf("load failed\n");
        return 0;
    }
    char key[128];
    char val[128];

    treemap_clear(tree);//clears the tree
    while(!feof(fp)){//traverses through file
        fscanf(fp, " %s %s", key, val);// takes in the key and val from file
        treemap_add(tree, key, val);// uses key and val to add to the free
    }
    
    fclose(fp);//closes the file
    return 1;
}
// Clears the given tree then loads new elements to it from the
// named. Repeated calls to treemap_add() are used to add strings read
// from the file.  If the tree is stored in pre-order in the file, its
// exact structure will be restored.  Returns 1 if the tree is loaded
// successfully and 0 if opening the named file fails in which case no
// changes are made to the tree.