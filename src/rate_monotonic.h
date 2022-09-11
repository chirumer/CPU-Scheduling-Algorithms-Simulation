#pragma once

#include "engine.h"


// simulate rate monotonic scheduling algorithm
struct Simulation_events simulate_rate_monotonic(int start_time, int end_time, struct Periodic_processes processes);

// get metrics
struct Simulation_metrics_list metrics_rate_monotonic(struct Periodic_processes processes, struct Simulation_events events);