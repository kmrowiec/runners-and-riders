/* 
 * File:   main.c
 * Author: Kamil Mrowiec <kam20@aber.ac.uk>
 *
 * Created on 1 december 2012, 20:10
 */
#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "node.h"
#include "track.h"
#include "course.h"
#include "entrant.h"

int TOTAL_NODES = 0;
int TOTAL_TRACKS = 0;
int TOTAL_COURSES = 0;
int TOTAL_ENTRANTS = 0;

EVENT* event;
NODE* nodes;
TRACK *tracks;
COURSE* courses;
ENTRANT* entrants;
TIME current_time;

void print_menu();
int count_lines(char filename[]);
void display_results();
void prompt_for_files();

/*
 * Main function of the program. 
 * Asks user to provide data files, loads them and then
 * keeps displaying a menu, taking user's input and responding,
 * until user input is 0, what quits the program.
 */
int main(int argc, char** argv) {

    TOTAL_NODES = count_lines("./data_files/nodes.txt");
    TOTAL_TRACKS = count_lines("./data_files/tracks.txt");
    TOTAL_COURSES = count_lines("./data_files/courses.txt");
    TOTAL_ENTRANTS = count_lines("./data_files/entrants.txt");
    load_event_data(&event, "./data_files/name.txt");
    nodes = load_nodes_from_file("./data_files/nodes.txt");
    tracks = load_tracks_from_file("./data_files/tracks.txt");
    courses = load_courses_from_file("./data_files/courses.txt");
    entrants = load_entrants_from_file("./data_files/entrants.txt");
    load_cp_file("./data_files/cp_times_1.txt");
  //  load_cp_file("./data_files/cp_times_2.txt");
    
//    prompt_for_files();
    
    //Menu loop. Displays menu, takes user's input and responds to it
    //as long as n (user's input) is not 0.
    int n = 10;
    while (n != 0) {
        print_menu();           //displays menu
        scanf("%d", &n);        //takes input from user
        printf("\n");
        if(!(n>=0 && n<8)){     //checks if input is correct
            printf("Incorrect input. Try again.\n");
            continue;
        }
        switch (n) {            
            case 1:             //displaying entrant's status
                printf("Give entrant's number: ");
                int cn = -1;
                scanf("%d", &cn);
                if(!(cn>0 && cn<=TOTAL_ENTRANTS)){
                        printf("Incorrect input. Try again.\n");
                        continue;
                }
                disp_ent_status(&entrants[cn-1]);
                break;
            case 2:             //show how many not yet started
                printf("%d competitor(s) have not yet started.\n",
                        count_entrants("NS"));
                break;
            case 3:             //show how many are on courses
                printf("%d competitor(s) are out on courses.\n",
                        count_entrants("TR"));
                break;
            case 4:             //show how many finished
                printf("%d competitor(s) have finished.\n",
                        count_entrants("CC"));
                break;
            case 5:             //manually supply CP data
                printf("Supply checkpoint data for an entrant,");
                printf(" formatted as: CP_TYPE CP_NUMBER ENT_NUMBER TIME\n");
                printf("E. g. T 1 3 9:35\n");
                char line[20];
                scanf(" %[^\n]%*c", &line);
                update_entrant_position(line);
                break;
            case 6:             //load checkpoints from file
                printf("Give file name or path to file: ");
                char src[100];
                scanf("%s", &src);
                load_cp_file(src);
                break;
            case 7:             //display results
                display_results();
                break;
        }
    }

    return (EXIT_SUCCESS);
}

void print_menu() {
    printf("\n1. Show status of a competitor. \n");
    printf("2. How many entrants have not yet started?\n");
    printf("3. How many entrants are out on courses?\n");
    printf("4. How many entrants finished?\n");
    printf("5. Supply checkpoint data for an entrant.\n");
    printf("6. Load checkpoints data from file.\n");
    printf("7. View results list.\n");
    printf("0. Quit.\n");
    printf("Select option: ");
}

/*
 * Prompts user to input paths to data files, then loads it.
 */
void prompt_for_files(){
    char fname[100];
    
    printf("Please, give a path to the file containing "
            "event name, date and start time:\n ");
    scanf("%s", &fname);
    load_event_data(&event, fname);
    
    printf("\nPlease, give a path to the file containing "
            "nodes:\n ");
    scanf("%s", &fname);
    TOTAL_NODES = count_lines(fname);
    nodes = load_nodes_from_file(fname);
    
    printf("\nPlease, give a path to the file containing "
            "tracks:\n ");
    scanf("%s", &fname);
    TOTAL_TRACKS = count_lines(fname);
    tracks = load_tracks_from_file(fname);
    
    printf("\nPlease, give a path to the file containing "
            "courses:\n ");
    scanf("%s", &fname);
    TOTAL_COURSES = count_lines(fname);
    courses = load_courses_from_file(fname);
    
    printf("\nPlease, give a path to the file containing "
            "entrants:\n ");
    scanf("%s", &fname);
    TOTAL_ENTRANTS = count_lines(fname);
    entrants = load_entrants_from_file(fname);    
}

/*
 * Counts total amount of lines in a file.
 */
int count_lines(char filename[]) {
    FILE* file;
    file = fopen(filename, "r");
    int counter = 0;
    char mock[100];
    while (fgets(mock, 100, file) != NULL) counter++;
    return counter;
}

/*
 * Displays result lists.
 * Copies the array of entrants, then performs bubble search
 * on the copied array, to order entrants by total time.
 * All entrants that have not completed the race yet
 * are listed at the bottom of the list.
 */
void display_results(){
    printf("\nFor which course? \n");
    char c;
    scanf(" %c", &c);
    if(TOTAL_ENTRANTS!=count_entrants("CC")){
        printf("WARNING: Some entrants are still on track.\n");
    }
    
    int i = 0, count = 0;
    for(i; i<TOTAL_ENTRANTS;i++){
        if(entrants[i].course->id==c) count++;
    }
    
    ENTRANT ents[count];
    i = 0;
    int j = 0;
    for(i;i<TOTAL_ENTRANTS;i++){
        if(entrants[i].course->id==c){
            ents[j] = entrants[i];
            j++;
        }
    }
    
    //Bubble sort.
    int swapped = 0;
    do{
        i =0;
        swapped = 0;
        for(i; i<count-1; i++){
            if(compare_times(&ents[i].total_time, 
                    &ents[i+1].total_time)<=0){
                        continue;
            }
            else{ //if items are in wrong order, they are swapped 
                ENTRANT temp = ents[i];
                ents[i] = ents[i+1];
                ents[i+1] = temp;
                swapped = 1;
            }
        }
        
    }while(swapped!=0); // ends when not a single swap occurred
    
    printf("Place  (Number) Name               "
            "                           Time\n");
    printf("==================================="
            "===============================\n");

    i = 0;
    for(i;i<count;i++){
        printf("%d", i+1);
        if(i+1<10) printf("      ");
        else if(i+1<100) printf("     ");
        else printf("    ");
        printf("(%02d) %s", ents[i].number, ents[i].name);
        int n = 0;
        //printing space between entrants name and time
        //so that everything is formated neatly
        for(n; n<50-strlen(ents[i].name); n++) printf(" ");
        if(ents[i].total_time.hours==99){
            //no time displayed if entrant still on track
            printf("-:--\n"); 
        }else display_time(ents[i].total_time);
    }
}
