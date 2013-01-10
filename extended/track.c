#include <stdio.h>
#include <stdlib.h>
#include "track.h"

extern int TOTAL_TRACKS;
extern TRACK* tracks;

/*
 * Loads single track from string source
 * and saves it under *track.
 */
void load_track(TRACK* track, char source[]){
    sscanf(source, " %d %d %d %d", &track->number, &track->start, 
            &track->end, &track->maxtime);
}

void display_track(TRACK* track){
    printf("Track %d : starts at %d, ends at %d, max time: %d \n",
            track->number, track->start, track->end, track->maxtime);
}

/*
 * Loads multiple tracks from file
 * and returns pointer to an array, created in result.
 */
TRACK* load_tracks_from_file(char filename[]){
    TRACK* tracks = (TRACK*) malloc(TOTAL_TRACKS*sizeof(TRACK));
    FILE* file;
    file = fopen(filename, "r");
    int i = 0;
    for(i; i<TOTAL_TRACKS;i++){
        char buff[15];
        fgets(buff, 15, file);
         load_track(&tracks[i], buff);
    }
    return tracks;
}

/*
 * Returns pointer to track that lies between node1 and node2
 */
TRACK* find_track(int node1, int node2){
    int i = 0;
    for(i;i<TOTAL_TRACKS;i++){
        if((tracks[i].start==node1 && tracks[i].end==node2)
                ||(tracks[i].start==node2 && tracks[i].end==node1))
            return &tracks[i];
    }
    return NULL;
}


