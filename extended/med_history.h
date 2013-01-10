/* 
 * File:   med_history.h
 * Author: kamil
 *
 * Created on 12 grudzień 2012, 23:15
 */

#ifndef MED_HISTORY_H
#define	MED_HISTORY_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "time.h"
    
    struct entrant;
    typedef struct entrant ENTRANT;
    
    struct med_hist_node{
        int node_number;
        char type; // 'A' - arrived, 'D' - departed
        TIME time;
        struct med_hist_node *next;
    };
    
    typedef struct med_hist_node MH_NODE;
    
    void load_med_cp_data(ENTRANT* ent, char type, int node_nr, TIME time);
    void display_med_hist(ENTRANT *ent);


#ifdef	__cplusplus
}
#endif

#endif	/* MED_HISTORY_H */

