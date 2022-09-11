#pragma once

#include "engine.h"


// simulate rate monotonic scheduling algorithm
struct Simulation_events simulate_rate_monotonic(int start_time, int end_time, struct Periodic_processes processes);