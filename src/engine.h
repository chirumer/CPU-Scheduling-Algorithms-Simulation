#pragma once

#include <stdbool.h>

#define PROCESS_NAME_SIZE 20


// simulation types (values beginning from 1)
enum Simulation_type {
    SIMULATION_TYPES_START, SIMULATION_RATE_MONOTONIC, SIMULATION_TYPES_END
};

// types of simulation events
enum Simulation_event_type {
    PROCESS_REQUEST, PROCESS_BEGIN, PROCESS_PREEMPT, PROCESS_COMPLETE, PROCESS_EXPIRED, 
    PROCESS_RESTART, SIMULATION_BEGIN, SIMULATION_END
};

// for preempt event data
struct Process_switch {
    char from[PROCESS_NAME_SIZE];
    char to[PROCESS_NAME_SIZE];
};

// data of the simulation event
union Simulation_event_data {
    struct Process_switch switch_info;
    char process_name[PROCESS_NAME_SIZE];
};

// simulation event
struct Simulation_event {
    union Simulation_event_data data;
    enum Simulation_event_type type;
    int timestamp;
    struct Simulation_event* next;
};

// list of simulation events
struct Simulation_events {
        // linked list implementation
    struct Simulation_event* event_head;
    int length;
};

// periodic process info
struct Periodic_process {
    char name[PROCESS_NAME_SIZE];
    int release_time;
    int execution_time;
    int time_period;
    int deadline_time;
};

// periodic processes list
struct Periodic_processes {
        // array implementation
    struct Periodic_process* processes;
    int length;
};

// Simulation parameters
struct Simulation_parameters {
    int start_time;
    int end_time;
};

// simulation parameters
extern struct Simulation_parameters simulation_parameters;


// initialize simulation events list
void init_simulation_events(struct Simulation_events* list);

// add simulation event to simulation events list
void add_simulation_event(struct Simulation_events* list, struct Simulation_event* event);

// destroy simulation events list
void destroy_simulation_events(struct Simulation_events* list);

// set simulation parameters
void set_simulation_parameters(struct Simulation_parameters parameters);

// create simulation begin event
struct Simulation_event* simulation_begin_event(int begin_timestamp);

// create simulation end event
struct Simulation_event* simulation_end_event(int end_timestamp);

// create process request event
struct Simulation_event* process_request_event(int timestamp, char process_name[]);

// create process begin event
struct Simulation_event* process_begin_event(int timestamp, char process_name[]);

// create process restart event
struct Simulation_event* process_restart_event(int begin_timestamp, char process_name[]);

// create process preempt event
struct Simulation_event* process_preempt_event(int timestamp, char process_from[], char process_to[]);

// create process complete event
struct Simulation_event* process_complete_event(int timestamp, char process_name[]);

// create process expired event
struct Simulation_event* process_expired_event(int timestamp, char process_name[]);

// simulate
struct Simulation_events simulate(enum Simulation_type type, void* processes);

// whether any of the events are PROCESS_TIMEOUT
bool has_timeout(struct Simulation_events events);