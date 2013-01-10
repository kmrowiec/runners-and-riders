#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "course.h"
#include "node.h"

extern TOTAL_COURSES;
extern COURSE* courses;
extern NODE* nodes;

/*
 * Loads the information of single course from the string source,
 * (e. g. single line of data file containing courses info)
 * to the course given by pointer.
 */
void load_course(COURSE* course, char source[]){
    
  char * temp;
        
  //Loading first char of the string, i. e. id of the course
  temp = strtok(source," ");
  sscanf(temp, "%c", &course->id);
  
  //Loading number of nodes the course consists of
  temp = strtok(NULL," ");
  course->node_count = strtol(temp, NULL, 10);
  
  //Reserving space in memory for the array of pointers to node
  course->nodes = (NODE*) malloc((course->node_count)*sizeof(NODE*));
  
  //Populating the array of pointers to node
  int i = 0, n = 0;
  for(i;i<course->node_count;i++){
      temp = strtok (NULL," ");
      sscanf(temp, "%d", &n);
      course->nodes[i] = &nodes[n-1];
  }    
}


void display_course(COURSE* course){
    printf("Course %c, consists of %d nodes : ", 
            course->id, course->node_count);
    int i = 0;
    for(i;i<course->node_count;i++){
        printf("%d ", course->nodes[i]->number);
    }
    printf("\n");
}

/*
 * Loads multiple courses from file
 * and returns pointer to an array, created in result.
 */
COURSE* load_courses_from_file(char filename[]){
    FILE* file;
    file = fopen(filename, "r");
    COURSE* courses = (COURSE*) malloc(TOTAL_COURSES*(sizeof(COURSE)));
    int i = 0;
    char buff[80];
    for(i;i<TOTAL_COURSES;i++){
        fgets(buff, 80, file);
        load_course(&courses[i], buff);
    }
    return courses;
}

COURSE* find_course(char course_id){
    int i = 0;
    for(i;i<TOTAL_COURSES;i++){
        if(courses[i].id==course_id) return &courses[i];
    }
    return NULL;
}


/*
 * Function takes as an argument a pointer that points to a place in
 * an array of pointers to NODE from COURSE structure. 
 * It increments that pointer, (to point to next element in that array),
 * and checks if that element is a checkpoint. If it is, returns that pointer,
 * if not, it increments again. The function should be used only when we are 
 * sure that a checkpoint will be found, otherwise the pointer would finally
 * step over the boundaries of an array.
 * 
 */
NODE** find_next_cp(NODE** src){
    NODE **ptr = src;
    for(;;){
        ptr++;
        char *node_type = *ptr;
        if(strstr((**ptr).type, "CP")||strstr((**ptr).type, "MC")){
            return ptr;
        }
        
    }
    return NULL;
}

