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

// =====================================================================================
// v1.0: splitting the students into "passed" (final grade >= 5.0) and "failed" (< 5.0)
// =====================================================================================

const double PASS_GRADE = 5.0;

// The pass/fail test, written as two tiny "function objects": you call them like a
// function, isPassed(s), and the algorithms below take them as the question to ask
// about every student. (The compiler can build them right into the algorithm.)
struct PassedTest { bool operator()(const Student &s) const { return s.rezults() >= PASS_GRADE; } };
struct FailedTest { bool operator()(const Student &s) const { return s.rezults() < PASS_GRADE; } };
inline constexpr PassedTest isPassed{};
inline constexpr FailedTest isFailed{};

// After strategy 2 the base container holds fewer students. shrink_to_fit asks a vector
// or deque to give the memory it no longer needs back. A list frees each node at erase
// time, so it has nothing to give back.
inline void shrinkToFit(vector<Student> &c) { c.shrink_to_fit(); }
inline void shrinkToFit(deque<Student> &c)  { c.shrink_to_fit(); }
inline void shrinkToFit(list<Student> &)    { }

// When we know in advance how many students will be added (first..last), a vector can
// reserve the space ONCE instead of growing step by step (every growth step moves all
// its students into a bigger block). A deque or a list never moves its students when it
// grows, so for them this does nothing.
template <class Iterator>
void reserveFor(vector<Student> &c, Iterator first, Iterator last) {
    c.reserve(c.size() + static_cast<size_t>(std::distance(first, last)));
}
template <class Container, class Iterator>
void reserveFor(Container &, Iterator, Iterator) { }

// ---- STRATEGY 1: two NEW containers. 'all' keeps every student, and every student is
// ---- COPIED into 'passed' or 'failed', so each student is stored twice.

// Strategy 1 with a hand-written loop.
template <class Container>
void splitCopyLoop(const Container &all, Container &passed, Container &failed) {
    for (const Student &s : all) {
        if (isPassed(s)) passed.push_back(s);    // push_back(s) makes a copy
        else             failed.push_back(s);
    }
}

// Strategy 1 with <algorithm>.
// std::remove_copy_if copies every student who is NOT failed (it leaves the failed ones out).
// std::copy_if copies every student who IS failed.
template <class Container>
void splitCopyAlgo(const Container &all, Container &passed, Container &failed) {
    std::remove_copy_if(all.begin(), all.end(), std::back_inserter(passed), isFailed);
    std::copy_if(all.begin(), all.end(), std::back_inserter(failed), isFailed);
}

// ---- STRATEGY 2: ONE new container. Failed students are MOVED into 'failed' and removed
// ---- from 'all', so in the end 'all' holds only the passed students. Every student is
// ---- stored once.

// Strategy 2 with a hand-written loop: erase the failed students one by one.
// For a list, erase just unlinks one node. A vector must shift EVERY student behind the
// erased one a place to the left, and a deque shifts the shorter side, so on big files
// this loop becomes very slow (see eraseLoopShifts in Processing.h).
template <class Container>
void splitMoveLoop(Container &all, Container &failed) {
    auto it = all.begin();
    while (it != all.end()) {
        if (isFailed(*it)) {
            failed.push_back(std::move(*it));   // move = take the data over, no copying
            it = all.erase(it);                 // erase returns the student after it
        } else {
            ++it;
        }
    }
    shrinkToFit(all);
}

// Strategy 2 with <algorithm>: std::stable_partition.
// 1. stable_partition puts the passed students first and the failed students after them,
//    keeping the sorted order inside both groups. It returns where the failed ones start.
// 2. std::copy with move iterators MOVES that tail of failed students into 'failed'
//    (now we know how many there are, so a vector reserves the space first).
// 3. One erase() cuts off the whole tail at once, so nothing has to be shifted.
template <class Container>
void splitPartitionAlgo(Container &all, Container &failed) {
    auto firstFailed = std::stable_partition(all.begin(), all.end(), isPassed);
    reserveFor(failed, firstFailed, all.end());
    std::copy(std::make_move_iterator(firstFailed), std::make_move_iterator(all.end()),
              std::back_inserter(failed));
    all.erase(firstFailed, all.end());
    shrinkToFit(all);
}

// Strategy 2 with <algorithm>: the "erase-remove idiom".
// 1. std::copy_if copies the failed students into 'failed'.
// 2. std::remove_if slides the passed students to the front (keeping their order) and
//    returns where the leftovers begin; one erase() cuts the leftovers off.
template <class Container>
void splitRemoveAlgo(Container &all, Container &failed) {
    std::copy_if(all.begin(), all.end(), std::back_inserter(failed), isFailed);
    all.erase(std::remove_if(all.begin(), all.end(), isFailed), all.end());
    shrinkToFit(all);
}
