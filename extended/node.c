#include <stdio.h>
#include <stdlib.h>
#include "node.h"

extern TOTAL_NODES;

/*
 * Loads the node from given string of chars.
 * Expects format given in assignment spec,
 * i. e. 3 CP
 */
void load_node(NODE* node, char source[]){
    sscanf(source, " %d %s", &node->number, &node->type);
}

void display_node(NODE node){
    printf("%d , %s \n", node.number, node.type);
}

/*
 * Loads multiple nodes from file
 * and returns pointer to an array, created as a result.
 */
NODE* load_nodes_from_file(char filename[]){  
    //allocating enough memory to an array 
    //that will hold all the nodes we will load
    NODE *nodes = (NODE*) malloc(TOTAL_NODES*sizeof(NODE));
    
    //opening file and loading every single node
    FILE* file;
    file = fopen(filename, "r");
    int i = 0;
    for(i; i<TOTAL_NODES;i++){
        char buff[10];
        fgets(buff, 10, file);
         load_node(&nodes[i], buff);
    }
    return nodes;
    
}
