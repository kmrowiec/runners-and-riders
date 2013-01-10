/* 
 * File:   event.h
 * Author: kamil
 *
 * Created on 1 grudzień 2012, 20:42
 */

#ifndef EVENT_H
#define	EVENT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "time.h"    
    
struct event{
    char title[81];
    char date[81];
    TIME start_time;
};

typedef struct event EVENT;


void load_event_data(EVENT* event, char datafile[]);
void display_event(EVENT* event);


#ifdef	__cplusplus
}
#endif

#endif	/* EVENT_H */

