// standard headers
#include <stdio.h>
#include <stdbool.h>

// user headers
#include "user_interface.h"

// ---------------


int main() {

    // greet the user
    greet_user();
    
    // do work of the user until she wants to exit
    bool app_running = true;
    while (app_running) {

        // get user's wish
        switch(prompt_program_action()) {

            case PROGRAM_SIMULATE:

                // set simulation parameters
                set_simulation_parameters(prompt_simulation_parameters());

                // delegate to appropriate algorithm
                switch (prompt_simulation_type()) {

                    case SIMULATION_RATE_MONOTONIC: {

                        // get infos of processes to be scheduled
                        struct Periodic_processes processes = prompt_periodic_processes_info();

                        // perform simulation
                        struct Simulation_events events = simulate(SIMULATION_RATE_MONOTONIC, (void*)&processes);

                        // display simulation result
                        display_simulation(events);
                        // clean up
                        destroy_simulation_events(&events);
                        break;
                    }
                }                
                break;

            case PROGRAM_SETTINGS:
                printf("No Settings Available At This Moment.\n\n");
                break;

            case PROGRAM_EXIT:
                app_running = false;
                break;
        }
    }

    // say goodbye
    say_goodbye();

    return 0;
}