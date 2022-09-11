// standard headers
#include <stdlib.h>
#include <stdio.h> // temp

// user headers
#include "rate_monotonic.h"
#include "engine.h"


// static structure definitions 

// all information about a waiting process
struct Process_state {
    int work_left;
    int time_left;
    struct Periodic_process* process;
};

// node in waiting processes list
struct Active_processes {
    struct Process_state state;
    struct Active_processes* next;
};


// static functions

void DEBUG_print_processes(struct Active_processes* processes) {
    
    printf("\nProcesses:\n");
    while (processes != NULL) {
        printf("%s\n", processes->state.process->name);
        processes = processes->next;
    }
}

// remove process
void remove_process(struct Active_processes** process) {
    struct Active_processes* temp = *process;
    *process = (*process)->next;
    free(temp);
}

// add process based on priority
void add_process(struct Active_processes** process_list, struct Periodic_process* process_info) {

    // allocate memory for new process
    struct Active_processes* new_process = malloc (sizeof(struct Active_processes));

    // set new process parameters
    new_process->state.process = process_info;
    new_process->state.time_left = process_info->deadline_time;
    new_process->state.work_left = process_info->execution_time;

    // insert at beginning if list empty
    if (*process_list == NULL) {
        new_process->next = NULL;
        *process_list = new_process;
        return;
    }
    
    // insert based on priority (1/period)
        // head insertion
    if ((1.0/(*process_list)->state.process->time_period) < (1.0/process_info->time_period)) {
        new_process->next = *process_list;
        *process_list = new_process;
        return;
    }
        // internal insertion
    struct Active_processes* cursor = *process_list;
    while (cursor->next != NULL && (1.0/cursor->next->state.process->time_period) > (1.0/process_info->time_period)) {
        cursor = cursor->next;
    }
    new_process->next = cursor->next;
    cursor->next = new_process;
}

// clean-up process structures
void destroy_processes(struct Active_processes** process_list) {

    while (*process_list != NULL) {
        struct Active_processes* temp = *process_list;
        *process_list = (*process_list)->next;
        free(temp);
    }
}

// extern functions

// simulate rate monotonic scheduling algorithm
struct Simulation_events simulate_rate_monotonic(int start_time, int end_time, struct Periodic_processes processes) {

    struct Simulation_events events;
    init_simulation_events(&events);

    struct Active_processes* active_processes = NULL;
    struct Process_state* current_process = NULL;

    add_simulation_event(&events, simulation_begin_event(start_time));

    for (int t = start_time; t < end_time; t++) {

        // do work
        if (current_process != NULL) {
            current_process->work_left--;
            // if process completed
            if (current_process->work_left == 0) {

                // update structures
                add_simulation_event(&events, process_complete_event(t, current_process->process->name));
                remove_process(&active_processes);
                // update current process
                current_process = NULL;
            }
        }

        // expire processes that are past their timeouts
        {
            if (current_process != NULL) {

                // if current process will time out
                if (current_process->time_left - 1 == 0) {

                    // update structures
                    add_simulation_event(&events, process_expired_event(t, current_process->process->name));
                    remove_process(&active_processes);
                    // update current process
                    current_process = NULL;
                }
            }

            struct Active_processes* dummy = malloc(sizeof(struct Active_processes));
            dummy->next = active_processes;

            struct Active_processes* temp = dummy;

            // traverse the process list and check for timeouts
            while (temp->next != NULL) {

                temp->next->state.time_left--;

                // if process timed out
                if (temp->next->state.time_left == 0) {

                    // update structures
                    add_simulation_event(&events, process_expired_event(t, temp->next->state.process->name));
                    struct Active_processes* del = temp->next;
                    temp->next = temp->next->next;
                    free(del);
                }
                else {
                    temp = temp->next;
                }
            }

            active_processes = dummy->next;
        }

        // add new processes
        for (int i = 0; i < processes.length; i++) {
            
            if (processes.processes[i].release_time == t || t > processes.processes[i].release_time &&
                ((t - processes.processes[i].release_time) % processes.processes[i].time_period == 0)) {

                // update structures to add process
                add_simulation_event(&events, process_request_event(t, processes.processes[i].name));
                add_process(&active_processes, &processes.processes[i]);
            }
        }
        // idle -> new process in active processes (process begin)
        if (current_process == NULL && active_processes != NULL) {
            
            // update structures to start processing
            current_process = &active_processes->state;
            if (current_process->work_left != current_process->process->execution_time) {
                add_simulation_event(&events, process_restart_event(t, current_process->process->name));
            }
            else {
                add_simulation_event(&events, process_begin_event(t, current_process->process->name));
            }
        }
        else if (current_process == NULL) {
            // idle
        }
        // process pre-empted
        else if (current_process != &active_processes->state) {

            // update structures to pre-empt the current process
            add_simulation_event(&events, 
                                 process_preempt_event(t, current_process->process->name, active_processes->state.process->name));
            current_process = &active_processes->state;
        }
    }
 
    add_simulation_event(&events, simulation_end_event(end_time));

    destroy_processes(&active_processes);

    return events;
}
