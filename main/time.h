/* 
 * File:   time.h
 * Author: kamil
 *
 * Created on 1 grudzień 2012, 20:29
 */

#ifndef TIME_H
#define	TIME_H

#ifdef	__cplusplus
extern "C" {
#endif

struct time{
    int hours;
    int min;
};

typedef struct time TIME;

void load_time(TIME* time, char source[]);
void display_time(TIME time);
void add_time(TIME* src, int minutes);
int compare_times(TIME* t1, TIME* t2);
TIME calc_total_time(TIME start, TIME end);

#ifdef	__cplusplus
}
#endif

#endif	/* TIME_H */

