/* 
 * File:   entrant.h
 * Author: kamil
 *
 * Created on 3 grudzień 2012, 00:35
 */

#ifndef ENTRANT_H
#define	ENTRANT_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "time.h"
#include "course.h"
#include "med_history.h"

    struct entrant{
        int number;
        COURSE* course;
        char name[51];
        char status[4];
        TIME start_time;
        TIME finish_time;
        TIME last_check_time; //time of arrival at last checkpoint
        //last visited node
        NODE **lvn; 
        TIME total_time;
        MH_NODE **mh_root; //pointer to first node of med checks history
        int time_on_mc; //total time in minutes, spent on medical checks
    };

    typedef struct entrant ENTRANT;
    
    void load_entrant(ENTRANT* entrant, char source[]);
    void display_entrant(ENTRANT* entrant);
    ENTRANT* load_entrants_from_file(char filename[]);
    int update_entrant_position(char src[]);
    void disp_ent_status(ENTRANT *ent);
    void load_cp_file(char filename[]);
    int count_entrants(char status[]);
    ENTRANT* find_entrant(int ent_id);
    void list_entrants(char status[]);
    
#ifdef	__cplusplus
}
#endif

#endif	/* ENTRANT_H */

