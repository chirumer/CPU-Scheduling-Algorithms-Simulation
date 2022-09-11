#pragma once

#include "engine.h"


// program actions (values beginning from 1)
enum Program_action {
    PROGRAM_ACTIONS_START, PROGRAM_SIMULATE, PROGRAM_SETTINGS, PROGRAM_EXIT, PROGRAM_ACTIONS_END
};

// user choices
enum Choice {
    CHOICE_START, CHOICE_YES, CHOICE_NO, CHOICE_END
};

// greets the user 
void greet_user();

// say goodbye
void say_goodbye();

// display unsuccessful scheduling message
void display_unsuccessful_scheduling();

// get yes or no response from user
enum Choice prompt_yes_or_no(char* prompt);

// prompt the user for program action
enum Program_action prompt_program_action();

// prompt the user for simulation type
enum Simulation_type prompt_simulation_type();

// prompt period processes info
struct Periodic_processes prompt_periodic_processes_info();

// display the simulation
void display_simulation(struct Simulation_events events);

// prompt simulation parameters
struct Simulation_parameters prompt_simulation_parameters();