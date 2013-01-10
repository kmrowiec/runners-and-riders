/* 
 * File:   node.h
 * Author: kamil
 *
 * Created on 1 grudzień 2012, 22:25
 */

#ifndef NODE_H
#define	NODE_H

#ifdef	__cplusplus
extern "C" {
#endif

struct node{
    int number;
    char type[3];
};

typedef struct node NODE;

void load_node(NODE* node, char source[]);
void display_node(NODE node);
NODE* load_nodes_from_file(char filename[]);


#ifdef	__cplusplus
}
#endif

#endif	/* NODE_H */

