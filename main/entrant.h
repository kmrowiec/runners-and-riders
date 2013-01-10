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

    struct entrant{
        int number;
        COURSE* course;
        char name[51];
        char status[3];
        TIME start_time;
        TIME finish_time;
        TIME last_check_time;
        //last visited node
        NODE **lvn; 
        TIME total_time;
    };

    typedef struct entrant ENTRANT;
    
    void load_entrant(ENTRANT* entrant, char source[]);
    void display_entrant(ENTRANT* entrant);
    ENTRANT* load_entrants_from_file(char filename[]);
    int update_entrant_position(char src[]);
    void disp_ent_status(ENTRANT *ent);
    void load_cp_file(char filename[]);
    int count_entrants(char status[]);

#ifdef	__cplusplus
}
#endif

#endif	/* ENTRANT_H */

