#pragma once
#include "StudentIO.h"
#include "StudentUtils.h"
#include "Timer.h"

// Everything we measure for one file (times are in seconds).
struct Timings {
    size_t count = 0, passed = 0, failed = 0;
    double read = 0, sort = 0, split = 0, write = 0, total = 0;
};

// Runs the whole job on one file: read -> sort -> split -> write two files
// (<name>_passed.txt and <name>_failed.txt), timing every step.
Timings processFile(const string &filename, int sortChoice);

void printTimings(const string &filename, const Timings &t, ostream &out = cout);
