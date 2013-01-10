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
    char buff[60];
    for(i;i<TOTAL_ENTRANTS;i++){
        fgets(buff, 60, file);
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
    
    //In this case, we only deal with time checkpoints,
    //so if the line does not start with T, that means something is wrong.
    if(type!='T') return 1;
    
    //Getting a pointer to entrant that passed the checkpoint.
    ENTRANT* ent = &entrants[ent_nr-1];
    
    ent->last_check_time = current_time;
    
    //If the entrant does not start yet, sets his start time
    //to the time that has just been loaded.
    if(strstr(ent->status, "NS")!=NULL){
        ent->start_time = current_time;
        ent->lvn = &(ent->course->nodes[0]);
        strcpy(ent->status, "TR"); 
    }else{
        ent->lvn = find_next_cp(ent->lvn);
        if(ent->course->nodes[0]->number==(**(ent->lvn)).number) {
        //If that is the last node of the course the entrant is taking,
        //it should update his finish time and set status to CC.
        ent->finish_time = current_time;
        strcpy(ent->status, "CC");
        ent->total_time = calc_total_time(ent->start_time, ent->finish_time);
        }
    }    
}

/*
 * Loads multiple checkpoints from file.
 */
void load_cp_file(char filename[]){
    FILE* file = fopen(filename, "r");
    char buff[20];
    while(fgets(buff, 20, file)!=NULL) update_entrant_position(buff);
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
    }else if(strstr(ent->status, "CC")){
        printf("%s (%d) completed the race correctly.\n", 
                ent->name, ent->number);
    }else if(strstr(ent->status, "TR")){
        TIME ent_time = ent->last_check_time;
        
        NODE **lvn = ent->lvn;
        NODE **next = lvn;
        next++;   
        for(;;){  
            int s_node = (**lvn).number;
            int e_node = (**next).number;
            TRACK *trk = find_track(s_node, e_node);
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