#pragma once
#include "StudentIO.h"
#include "StudentUtils.h"
#include "Timer.h"

// Everything we measure for one file (times are in seconds).
struct Timings {
    size_t count = 0, passed = 0, failed = 0;
    double read = 0, sort = 0, split = 0, write = 0, total = 0;
};

// Names used in messages and output file names.
inline string containerName(const vector<Student> &) { return "vector"; }
inline string containerName(const list<Student> &)   { return "list"; }
inline string containerName(const deque<Student> &)  { return "deque"; }

// Runs the whole job on one file with the chosen container type:
// read -> sort -> split -> write two files, timing every step.
// Output files: <name>_<container>_passed.txt and <name>_<container>_failed.txt
// Use it like:  processFile<std::list<Student>>("students_1000.txt", 3);
template <class Container>
Timings processFile(const string &filename, int sortChoice) {
    Timings t;
    Timer whole, step;

    step.reset();
    Container all = readStudents<Container>(filename);
    t.read = step.elapsed();
    t.count = all.size();

    step.reset();
    sortStudents(all, sortChoice);
    t.sort = step.elapsed();

    Container passed, failed;
    step.reset();
    splitStudents(all, passed, failed);
    t.split = step.elapsed();
    t.passed = passed.size();
    t.failed = failed.size();

    string base = std::filesystem::path(filename).stem().string() + "_" + containerName(all);
    step.reset();
    saveTable(passed, base + "_passed.txt");
    saveTable(failed, base + "_failed.txt");
    t.write = step.elapsed();

    t.total = whole.elapsed();
    return t;
}

void printTimings(const string &title, const Timings &t, ostream &out = cout);
