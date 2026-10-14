#pragma once
#include "Student.h"

// ---- Sorting: std::sort needs "random access" (vector, deque), but std::list
// ---- is a chain of nodes and has its own member function list::sort().
// ---- So there is one general version and one special version for list.

template <class Container, class Compare>
void sortWith(Container &Group, Compare cmp) {          // vector, deque
    std::sort(Group.begin(), Group.end(), cmp);
}

template <class Compare>
void sortWith(list<Student> &Group, Compare cmp) {      // list
    Group.sort(cmp);
}

// Sorts the group. choice: 1 name A-Z, 2 name Z-A, 3 average low-high,
// 4 average high-low, 5 median low-high, 6 median high-low.
// Throws std::invalid_argument for any other choice.
template <class Container>
void sortStudents(Container &Group, int choice) {
    switch (choice) {
        case 1: sortWith(Group, lessStud); break;
        case 2: sortWith(Group, MoreStud); break;
        case 3: sortWith(Group, lessAver); break;
        case 4: sortWith(Group, MoreAver); break;
        case 5: sortWith(Group, lessMed);  break;
        case 6: sortWith(Group, MoreMed);  break;
        default: throw std::invalid_argument("unknown sort choice: " + std::to_string(choice));
    }
}

// Moves every student with final (average) grade >= 5.0 into 'passed' and
// every student with a grade < 5.0 into 'failed'. 'all' is emptied.
template <class Container>
void splitStudents(Container &all, Container &passed, Container &failed) {
    for (Student &s : all) {
        if (s.rezults() >= 5.0) passed.push_back(std::move(s));   // move = cheap, no copying
        else                    failed.push_back(std::move(s));
    }
    Container().swap(all);    // empty 'all' AND give its memory back
}
