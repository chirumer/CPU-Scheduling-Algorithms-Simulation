// standard headers
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// user headers
#include "user_interface.h"
#include "engine.h"

// ---------------


// prints fancy title
void print_fancy_title(char* title) {

    // length of the title
    size_t length = strlen(title);

    // print the title along with a design
    for (int i = 0; i < length; i++) {
        printf("#");
    }
    printf("\n%s\n", title);
    for (int i = 0; i < length; i++) {
        printf("#");
    }
    // line break between title and next output
    printf("\n\n");
}

// greets the user
void greet_user() {
    print_fancy_title("Welcome To CPU Scheduling Algorithm Simulator!");
}

// say goodbye
void say_goodbye() {
    printf("Program Exited.\n");
}

// display unsuccessful scheduling message
void display_unsuccessful_scheduling() {
    printf("Scheduling is not possible with given parameters.\n\n");
}

// get yes or no response from user
enum Choice prompt_yes_or_no(char* prompt) {
    
    // prompt user until she gives a valid Choice
    while(true) {
        printf("%s\n", prompt);
        printf(
            "(%d) Yes\n"
            "(%d) No\n"
            "Your option --> ",
            CHOICE_YES, CHOICE_NO
        );
        int selected_option;
        scanf ("%d", &selected_option);

        printf("\n");

        if (selected_option > CHOICE_START 
            && selected_option < CHOICE_END) {

                return selected_option;
        }
        printf("Invalid Option. Try Again\n\n");
    }  
}

// prompt the user for program action
enum Program_action prompt_program_action() {

    // prompt user until she gives a valid Program_action
    while(true) {
        printf(
            "What would you like to do?\n"
            "(%d) Simulate A CPU Scheduling Algorithm\n"
            "(%d) Modify Program Settings\n"
            "(%d) Exit Program\n"
            "Your option --> ",
            PROGRAM_SIMULATE, PROGRAM_SETTINGS, PROGRAM_EXIT
        );
        int selected_option;
        scanf ("%d", &selected_option);

        printf("\n");

        if (selected_option > PROGRAM_ACTIONS_START 
            && selected_option < PROGRAM_ACTIONS_END) {

                return selected_option;
        }
        printf("Invalid Option. Try Again\n\n");
    }
}

// prompt the user for simulation type
enum Simulation_type prompt_simulation_type() {

    // prompt user until she gives a valid Simulation_type
    while(true) {
        printf(
            "What Algorithm would you like to simulate?\n"
            "(%d) Rate Monotonic Algorithm\n"
            "Your option --> ",
            SIMULATION_RATE_MONOTONIC
        );
        int selected_option;
        scanf ("%d", &selected_option);

        printf("\n");

        if (selected_option > SIMULATION_TYPES_START 
            && selected_option < SIMULATION_TYPES_END) {

                return selected_option;
        }
        printf("Invalid Option. Try Again\n\n");
    }
}

struct Periodic_process prompt_periodic_process() {

    struct Periodic_process process;

    printf("Enter process name: ");
    scanf("%s", process.name);

    printf("Enter process release time: ");
    scanf("%d", &process.release_time);

    printf("Enter process execution time: ");
    scanf("%d", &process.execution_time);

    printf("Enter process period: ");
    scanf("%d", &process.time_period);

    printf("Enter process deadline time: ");
    scanf("%d", &process.deadline_time);

    return process;
}

// prompt period processes info
struct Periodic_processes prompt_periodic_processes_info() {

    int n_processes;
    printf("Enter number of periodic processes to schedule: ");
    scanf("%d", &n_processes);

    struct Periodic_process* processes = malloc(n_processes * sizeof(struct Periodic_process));

    printf("\n");
    for (int i = 0; i < n_processes; i++) {
        processes[i] = prompt_periodic_process();
        printf("\n");
    }

    return (struct Periodic_processes){ processes, n_processes };
}

// prompt simulation parameters
struct Simulation_parameters prompt_simulation_parameters() {
    
    int start_time;
    printf("Enter start time: ");
    scanf("%d", &start_time);

    int end_time;
    printf("Enter end time: ");
    scanf("%d", &end_time);

    printf("\n");

    return (struct Simulation_parameters){ start_time, end_time };
}

// display the simulation
void display_simulation(struct Simulation_events events) {
    printf("Simulation Events:\n");
    printf("Timestamp (in milli)    | Event\n");
    printf("--------- --- ------    | -----\n");

    for (int i = 0; i < events.length; i++) {

        struct Simulation_event event = *events.event_head;

        printf("    %-20d| ", event.timestamp);

        switch (event.type) {

            case PROCESS_REQUEST:
                printf("Process (%s) Queued\n", event.data.process_name);
                break;

            case PROCESS_BEGIN:
                printf("Process (%s) Begun Executing\n", event.data.process_name);
                break;

            case PROCESS_RESTART:
                printf("Process (%s) Has Restarted\n", event.data.process_name);
                break;

            case PROCESS_PREEMPT:
                printf("Process (%s) Has Pre-empted Process(%s)\n", event.data.switch_info.to, 
                                                                    event.data.switch_info.from);
                break;

            case PROCESS_COMPLETE:
                printf("Process (%s) Has Completed\n", event.data.process_name);
                break;

            case PROCESS_EXPIRED:
                printf("Process (%s) Has Expired\n", event.data.process_name);
                break;

            case SIMULATION_BEGIN:
                printf("Simulation Begins\n");
                break;

            case SIMULATION_END:
                printf("Simulation Ends\n");
                break;
        }

        events.event_head = events.event_head->next;
    }

    printf("\n");
}

// display simulation metrics
void display_simulation_metrics(struct Simulation_metrics_list metrics) {

    printf("Algorithm Metrics:\n");
    printf("Proccess Name     | Waiting Time     | Turnaround Time  |\n");

    for (int i = 0; i < metrics.length; i++) {

        printf("  %-16s|  %-16lf|  %-16lf\n", metrics.metrics[i].process_name, 
               metrics.metrics[i].waiting_time, metrics.metrics[i].turnaround_time);
    }
    printf("\n");
}