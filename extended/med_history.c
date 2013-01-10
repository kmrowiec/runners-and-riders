#include <stdio.h>
#include <stdlib.h>
#include "med_history.h"
#include "entrant.h"

/*
 * When entrant arrives at medical checkpoint, 
 * the arrival is saved in linked list of MH_NODES, in entrant structure.
 * This function saves single arrival to that list.
 */
void load_med_cp_data(ENTRANT* ent, char type, int node_nr, TIME time){
    if(ent->mh_root==NULL){
        //Creating first node.
        ent->mh_root = (MH_NODE**) malloc(sizeof(MH_NODE*));
        *(ent->mh_root) = (MH_NODE*) malloc(sizeof(MH_NODE));
        (*(ent->mh_root))->node_number = node_nr;
        (*(ent->mh_root))->type = type;
        (*(ent->mh_root))->time = time;
        (*(ent->mh_root))->next = NULL;
    }else{
        //Adding node to existing list.
        MH_NODE *ptr = *(ent->mh_root);
        for(;;){
            if(ptr->next!=NULL){
                //searching for end of the list
                ptr = ptr->next;
            }else{
                // if ptr->next is null that means
                // we fount the end of the list
                ptr->next = (MH_NODE*) malloc(sizeof(MH_NODE));
                ptr->next->node_number = node_nr;
                ptr->next->type = type;
                ptr->next->time = time;
                break;
            }
        }
    }
}

/*
 * Displays history of arrival at medical checkpoints 
 * for entrant given as an argument.
 */
void display_med_hist(ENTRANT *ent){

    if(ent->mh_root==NULL){
        printf("Entrant %d did not arrive at any medical checkpoint yet.",
                ent->number);
    }else{
        MH_NODE *ptr = &(**(ent->mh_root));
        for(;;){
            if(ptr->type=='A') printf("Arrived at ");
            else printf("Departed from ");
            printf("checkpoint %d at ", ptr->node_number);
            display_time(ptr->time);
            
            if(ptr->next!=NULL){
                ptr = ptr->next;
            }else break;
        }
        printf("Time spent on medical checks: %d minutes.\n", 
                ent->time_on_mc);
    }
}
