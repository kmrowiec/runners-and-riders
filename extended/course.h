/* 
 * File:   course.h
 * Author: kamil
 *
 * Created on 2 grudzień 2012, 00:51
 */
#ifndef COURSE_H
#define	COURSE_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "node.h"
        
    struct course{
        char id;
        int node_count;
        NODE **nodes;
    };

    typedef struct course COURSE;
    
    void load_course(COURSE* course, char source[]);
    void display_course(COURSE* course);
    COURSE* load_courses_from_file(char filename[]);
    COURSE* find_course(char course_id);
    NODE** find_next_cp(NODE** src);

#ifdef	__cplusplus
}
#endif

#endif	/* COURSE_H */

