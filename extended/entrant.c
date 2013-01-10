#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "entrant.h"
#include "time.h"
#include "course.h"
#include "track.h"
#include "node.h"

extern int TOTAL_ENTRANTS;
extern ENTRANT* entrants;
extern COURSE* courses;
extern TIME current_time;

/*
 * Loads entrant data from string source
 * and saves it under *entrant
 */
void load_entrant(ENTRANT* entrant, char source[]){
    char course_id;
    sscanf(source, "%d %c %[^\n]s",  
            &entrant->number, &course_id, &entrant->name);
    //Finds proper course in the array of courses,
    //and assigns pointer to it to entrant.
    entrant->course = find_course(course_id);
    strcpy(entrant->status, "NS");
    entrant->total_time.hours = 99;
    entrant->total_time.min = 59;
    entrant->mh_root = NULL;
    entrant->time_on_mc = 0;
    
}

/*
 * Displays entrant's number, name and id 
 * of the course attempted by entrant.
 */
void display_entrant(ENTRANT* entrant){
    printf("%d: %s, on course %c \n", 
            entrant->number, entrant->name, entrant->course->id);

}

/*
 * Loads multiple entrants from a file
 * and returns the resulting array.
 */
ENTRANT* load_entrants_from_file(char filename[]){
    FILE* file = fopen(filename, "r");
    ENTRANT* ent = (ENTRANT*) malloc(TOTAL_ENTRANTS*(sizeof(ENTRANT)));
    int i = 0;
    char buff[100];
    for(i;i<TOTAL_ENTRANTS;i++){
        fgets(buff, 100, file);
        load_entrant(&ent[i],buff);
    }
    return ent;
}

/*
 * Processes data concerning entrant's arrival at checkpoint,
 * given in string, formatted as (e.g.) T 1 1 11.36,
 * and updates entrant's time, status and position.
 */
int update_entrant_position(char src[]){
    char type, time_s[6];
    int node_nr = 0, ent_nr = 0;
    sscanf(src, "%c %d %d %s", &type, &node_nr, &ent_nr, &time_s);
    load_time(&current_time, time_s);
    
    
    //Getting a pointer to entrant that passed the checkpoint.
    ENTRANT* ent = find_entrant(ent_nr);
    
        
    //if entrant came to incorrect checkpoint
    if(type=='I'){
        strcpy(ent->status, "FEI");
        ent->finish_time = current_time;
    }else
    //if excluded at medical checkpoint
    if(type=='E'){
        strcpy(ent->status, "FEM");
        ent->finish_time = current_time;
    }else
    //if entrant arrives at medical checkpoint
    if(type=='A'){
        load_med_cp_data(ent, type, node_nr, current_time);
        ent->lvn = find_next_cp(ent->lvn);
        strcpy(ent->status, "MC");
    }else if(type=='D'){
        //if entrants departs from medical check
        load_med_cp_data(ent, type, node_nr, current_time);
        //calculating time spent on med check
        int spent = time_diff(ent->last_check_time, current_time);
        ent->time_on_mc+=spent;
        strcpy(ent->status, "TR");
    }else
    
    //If the entrant does not start yet, sets his start time
    //to the time that has just been loaded.
    if(strstr(ent->status, "NS")){
        ent->start_time = current_time;
        ent->lvn = &(ent->course->nodes[0]);
        strcpy(ent->status, "TR"); 
    }else{
        //That means it is just a time checkpoint.
        ent->lvn = find_next_cp(ent->lvn);
        
        if(ent->course->nodes[0]->number==(**(ent->lvn)).number) {
        //If that is the last node of the course the entrant is taking,
        //it should update his finish time and set status to FCC.
        ent->finish_time = current_time;
        strcpy(ent->status, "FCC");
        ent->total_time = calc_total_time(ent->start_time, 
                ent->finish_time, ent->time_on_mc);
        }
    }    
    ent->last_check_time = current_time;
}

/*
 * Loads multiple checkpoints from file.
 */
void load_cp_file(char filename[]){
    FILE* file = fopen(filename, "r");
    char buff[50];
    int i = 0;
    while(fgets(buff, 50, file)!=NULL){
        update_entrant_position(buff);
    }
    printf("Data file loaded successfully.\n");
}

/*
 * Displays status information of an entrant.
 * This can either be not started, correctly completed, or on track.
 * If on track, it estimates on which track the entrant should be
 * (as presence on junctions is not recorded), based on 
 * the time of last known checkpoint appearance 
 * and the amount of time for a track to be safely completed. 
 */
void disp_ent_status(ENTRANT *ent){
    if(strstr(ent->status, "NS")){
        printf("%s (%d) has not yet started.\n", ent->name, ent->number);
    }else if(strstr(ent->status, "FCC")){
        printf("%s (%d) completed the race correctly.\n", 
                ent->name, ent->number);
    }else if(strstr(ent->status, "MC")){
        printf("%s (%d) is at medical checkpoint number %d.\n", 
                ent->name, ent->number, (**ent->lvn).number);
    }else if(strstr(ent->status, "FEI")){
        printf("%s (%d) has been excluded on checkpoint %d "
                "due to taking incorrect route.\n", 
                ent->name, ent->number, (**ent->lvn).number);
    }else if(strstr(ent->status, "FEM")){
        printf("%s (%d) has been excluded on medical check number %d.\n", 
                ent->name, ent->number, (**ent->lvn).number);
    }else if(strstr(ent->status, "TR")){
        TIME ent_time = ent->last_check_time;
        NODE **lvn = ent->lvn;
        NODE **next = lvn;
        next++;   
        for(;;){  
            int s_node = (**lvn).number; //start node of the track
            int e_node = (**next).number;//end node of the track
            TRACK *trk = find_track(s_node, e_node);
            //adding max time for the track to last check time of an entrant
            //and checking if current time is later than that
            //if is, that means entrant ist most probably at the next track
            add_time(&ent_time, trk->maxtime); 
            if(compare_times(&current_time, &ent_time)==-1){
                printf("%s (%d) is on track %d.\n", ent->name, 
                        ent->number, trk->number);
                return;
            }else{
                lvn = next;
                next++;
            }
        }
    }
}

/*
 * Returns the number of entrants, whose status matches the given string,
 * ("NS" - not started, "TR" - on track, "CC" - completed correctly) 
 */
int count_entrants(char status[]){
    int result = 0;
    int i = 0;
    for(i;i<TOTAL_ENTRANTS;i++)
        if( strstr(entrants[i].status, status) ) result++;
    return result;
}

/*
 * As entrants' id's not always follow one another in 
 * extended mission data files, we cannot rely on 
 * entrant number n to be at place n-1 in an array.
 * This function returns entrant with given id, by checking
 * place number ent_id, and then iterate towards the beginning to find
 * the proper entrant.
 */
ENTRANT* find_entrant(int ent_id){
    int i = TOTAL_ENTRANTS;
    if(ent_id<TOTAL_ENTRANTS) i = ent_id;
    for(i; i>=0; i--){
        if(entrants[i].number==ent_id) return &entrants[i];
    }
    printf("CANT FIND ENTRANT!\n");
    return NULL;
}

/*
 * Prints a list of entrants whose status matches the given string.
 */
void list_entrants(char status[]){
    int i = 0, test = 0;
    for(i;i<TOTAL_ENTRANTS;i++)
        if( strstr(entrants[i].status, status) ){
            test = 1;
            printf("(%d) %s\n", entrants[i].number, entrants[i].name);
        }
    if(!test) printf("No entrants to display.");
}