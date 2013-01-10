#include "time.h"
#include <stdio.h>
#include <stdlib.h>

void display_time(TIME time){
    printf("%d:%02d\n", time.hours, time.min);
}

/*
 * Loads time from string source,
 * formatted as "hh:mm".
 */
void load_time(TIME* time, char source[]){
    sscanf(source, "%d:%d", &time->hours, &time->min);
}

/*
 * Adds the amount of time given in minutes to src.
 */
void add_time(TIME* src, int minutes){
    src->min += minutes;
    src->hours += src->min/60;
    src->min = src->min%60;
}

/*
 * Compares t1 and t2 and returns :
 * -1 when t1 is smaller (earlier) than t2
 *  0 when t1 is equal to t2
 *  1 when t1 is greater (later) than t2
 */
int compare_times(TIME* t1, TIME* t2){
    if(t1->hours == t2->hours){
        if(t1->min < t2->min) return -1;
        if(t1->min > t2->min) return 1;
        return 0;
    }        
    if(t1->hours > t2->hours) return 1;
    return -1;
}

/*
 * Calculates time that passed between start and end,
 * by converting both times to integers (as only minutes),
 * subtracting them, and converting again to TIME, which is returned.
 */
TIME calc_total_time(TIME start, TIME end, int substract){
    int t1 = start.hours*60 + start.min;
    int t2 = end.hours*60 + end.min;
    int minutes = t2 - t1 - substract;
    TIME total;
    total.hours = minutes/60;
    total.min = minutes%60;
    return total;
}

/*
 * Returns time difference in minutes
 * between start time and end time.
 */
int time_diff(TIME start, TIME end){
    int t1 = start.hours*60 + start.min;
    int t2 = end.hours*60 + end.min;
    return t2 - t1;
}