#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "event.h"

/*
 * Loads event data from file
 * and saves it under *event
 */
void load_event_data(EVENT* event, char filename[]){
    FILE* file;
    file = fopen(filename, "r");
    fgets(event->title, 80, file);
    fgets(event->date, 80, file);
    char buff[6];
    fgets(buff, 6, file);
    load_time(&event->start_time, buff);
}

void display_event(EVENT* event){
    printf("Event: %s", event->title);
    printf("Date: %s", event->date);
    display_time(event->start_time);
    
}