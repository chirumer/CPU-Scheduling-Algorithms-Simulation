// standard headers
#include <stdlib.h>
#include <string.h>

// user headers
#include "engine.h"
#include "rate_monotonic.h"


// simulation parameters (to be set)
struct Simulation_parameters simulation_parameters = { -1, -1 };


// set simulation parameters
void set_simulation_parameters(struct Simulation_parameters parameters) {
    simulation_parameters = parameters;
}

// initialize simulation events list
void init_simulation_events(struct Simulation_events* list) {
    list->event_head = NULL;
    list->length = 0;
}

// add simulation event to simulation events list
void add_simulation_event(struct Simulation_events* list, struct Simulation_event* event) {
    
    // first addition
    if (list->event_head == NULL) {
        list->event_head = event;
        list->length = 1;
        return;
    }

    // traverse and add to end
    struct Simulation_event* cursor = list->event_head;
    while (cursor->next != NULL) {
        cursor = cursor->next;
    }
    cursor->next = event;
    list->length++;
}

// destroy simulation events list
void destroy_simulation_events(struct Simulation_events* list) {

    // free every event node
    while (list->event_head != NULL) {
        struct Simulation_event* temp = list->event_head;
        list->event_head = list->event_head->next;
        free(temp);
    }
    list->event_head = NULL;
    list->length = 0;
}

// create simulation begin event
struct Simulation_event* simulation_begin_event(int begin_timestamp) {

    // allocate memory
    struct Simulation_event* event = malloc(sizeof(struct Simulation_event));

    // set data
    event->type = SIMULATION_BEGIN;
    event->timestamp = begin_timestamp;

    return event;
}

// create simulation end event
struct Simulation_event* simulation_end_event(int end_timestamp) {

    // allocate memory
    struct Simulation_event* event = malloc(sizeof(struct Simulation_event));

    // set data
    event->type = SIMULATION_END;
    event->timestamp = end_timestamp;

    return event;
}

// create process request event
struct Simulation_event* process_request_event(int timestamp, char process_name[]) {

    // allocate memory
    struct Simulation_event* event = malloc(sizeof(struct Simulation_event));

    // set data
    event->type = PROCESS_REQUEST;
    event->timestamp = timestamp;
    strcpy(event->data.process_name, process_name);

    return event;
}

// create process begin event
struct Simulation_event* process_begin_event(int timestamp, char process_name[]) {

    // allocate memory
    struct Simulation_event* event = malloc(sizeof(struct Simulation_event));

    // set data
    event->type = PROCESS_BEGIN;
    event->timestamp = timestamp;
    strcpy(event->data.process_name, process_name);

    return event;
}

// create process restart event
struct Simulation_event* process_restart_event(int timestamp, char process_name[]) {

    // allocate memory
    struct Simulation_event* event = malloc(sizeof(struct Simulation_event));

    // set data
    event->type = PROCESS_RESTART;
    event->timestamp = timestamp;
    strcpy(event->data.process_name, process_name);

    return event;
}

// create process preempt event
struct Simulation_event* process_preempt_event(int timestamp, char process_from[], char process_to[]) {

    // allocate memory
    struct Simulation_event* event = malloc(sizeof(struct Simulation_event));

    // set data
    event->type = PROCESS_PREEMPT;
    event->timestamp = timestamp;
    strcpy(event->data.switch_info.from, process_from);
    strcpy(event->data.switch_info.to, process_to);

    return event;
}

// create process complete event
struct Simulation_event* process_complete_event(int timestamp, char process_name[]) {

    // allocate memory
    struct Simulation_event* event = malloc(sizeof(struct Simulation_event));

    // set data
    event->type = PROCESS_COMPLETE;
    event->timestamp = timestamp;
    strcpy(event->data.process_name, process_name);

    return event;
}

// create process expired event
struct Simulation_event* process_expired_event(int timestamp, char process_name[]) {

    // allocate memory
    struct Simulation_event* event = malloc(sizeof(struct Simulation_event));

    // set data
    event->type = PROCESS_EXPIRED;
    event->timestamp = timestamp;
    strcpy(event->data.process_name, process_name);

    return event;
}

// whether any of the events are PROCESS_TIMEOUT
bool has_timeout(struct Simulation_events events) {
    struct Simulation_event* temp = events.event_head;
    while (temp != NULL) {
        if (temp->type == PROCESS_EXPIRED) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

// simulate
struct Simulation_events simulate(enum Simulation_type type, void* processes) {
    switch (type) {
        case SIMULATION_RATE_MONOTONIC:
            return simulate_rate_monotonic(simulation_parameters.start_time, simulation_parameters.end_time, 
                                           *(struct Periodic_processes*)processes);
    }
}