#pragma once
#include "Student.h"

// Sorts the group. choice: 1 name A-Z, 2 name Z-A, 3 average low-high,
// 4 average high-low, 5 median low-high, 6 median high-low.
// Throws std::invalid_argument for any other choice.
void sortStudents(vector<Student> &Group, int choice);

// Moves every student with final (average) grade >= 5.0 into 'passed' and
// every student with a grade < 5.0 into 'failed'. 'all' is emptied.
void splitStudents(vector<Student> &all, vector<Student> &passed, vector<Student> &failed);
