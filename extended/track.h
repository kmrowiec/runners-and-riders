/* 
 * File:   track.h
 * Author: kamil
 *
 * Created on 2 grudzień 2012, 00:26
 */

#ifndef TRACK_H
#define	TRACK_H

#ifdef	__cplusplus
extern "C" {
#endif

    struct track{
        int number;
        int start;
        int end;
        int maxtime;
    };
    
    typedef struct track TRACK;
    
    void load_track(TRACK* track, char source[]);
    void display_track(TRACK* track);
    TRACK* load_tracks_from_file(char filename[]);
    TRACK* find_track(int node1, int node2);

#ifdef	__cplusplus
}
#endif

#endif	/* TRACK_H */

