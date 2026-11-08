#pragma once
#include "StudentIO.h"
#include "StudentUtils.h"
#include "Timer.h"

// The five test file sizes required by the task, and their names (students_1000.txt ...).
extern const vector<size_t> FILE_SIZES;
string testFileName(size_t records);

// v1.0: the five ways to split the students. The numbers are the menu numbers.
enum class Split {
    CopyLoop = 1,     // strategy 1, hand-written loop
    MoveLoop,         // strategy 2, hand-written loop (erase one by one)
    CopyAlgo,         // strategy 1, std::remove_copy_if + std::copy_if
    PartitionAlgo,    // strategy 2, std::stable_partition + std::copy + erase
    RemoveAlgo        // strategy 2, std::copy_if + std::remove_if + erase
};
string splitName(Split method);
bool isStrategy1(Split method);

// Everything we measure for one file (times are in seconds).
struct Timings {
    size_t count = 0, passed = 0, failed = 0;
    size_t stored = 0;        // students held in memory right after the split
    double memoryMB = 0;      // about how much memory those students take
    double read = 0, sort = 0, split = 0, write = 0, total = 0;
};

// Names used in messages and output file names.
inline string containerName(const vector<Student> &) { return "vector"; }
inline string containerName(const list<Student> &)   { return "list"; }
inline string containerName(const deque<Student> &)  { return "deque"; }

// About how many MB the students in a container take: each Student object plus its
// homework scores. (The container's own extra bytes are left out, so this is a minimum.)
template <class Container>
double approxMB(const Container &c) {
    double bytes = accumulate(c.begin(), c.end(), 0.0, [](double sum, const Student &s) {
        return sum + static_cast<double>(sizeof(Student) + s.homeworkCount() * sizeof(int));
    });
    return bytes / (1024.0 * 1024.0);
}

// ---- Why strategy 2 with a loop is "painful": every erase may SHIFT other students.
// ---- This counts the shifts BEFORE the loop runs, so the program can refuse a run that
// ---- would take hours.
//   vector: erasing a student shifts every student behind it one place to the left.
//   deque:  erasing shifts the shorter side - the students in front of it OR behind it.
//   list:   erasing just unlinks one node; nothing is shifted.
template <class Container>
double countShifts(const Container &all, bool shorterSide) {
    double shifts = 0;
    size_t size = all.size();   // how many students are left in the container
    size_t pos = 0;             // where the current student sits at that moment
    for (const Student &s : all) {
        if (isFailed(s)) {
            size_t behind = size - pos - 1;
            shifts += static_cast<double>((shorterSide && pos < size / 2) ? pos : behind);
            size--;             // this student is erased, the container is one shorter
        } else {
            pos++;              // this student stays
        }
    }
    return shifts;
}
inline double eraseLoopShifts(const vector<Student> &c) { return countShifts(c, false); }
inline double eraseLoopShifts(const deque<Student> &c)  { return countShifts(c, true); }
inline double eraseLoopShifts(const list<Student> &)    { return 0; }

// More shifts than this would keep the computer busy for many minutes (or hours),
// so the strategy 2 loop is refused (option 2) or skipped (options 3 and 4).
const double MAX_SHIFTS = 3e9;

// Runs the chosen split method.
// Strategy 1: 'all' stays complete, 'passed' and 'failed' get copies.
// Strategy 2: the failed students move into 'failed', 'all' keeps the passed ones
//             ('passed' is not used).
template <class Container>
void runSplit(Split method, Container &all, Container &passed, Container &failed) {
    switch (method) {
        case Split::CopyLoop:      splitCopyLoop(all, passed, failed); break;
        case Split::MoveLoop:      splitMoveLoop(all, failed);         break;
        case Split::CopyAlgo:      splitCopyAlgo(all, passed, failed); break;
        case Split::PartitionAlgo: splitPartitionAlgo(all, failed);    break;
        case Split::RemoveAlgo:    splitRemoveAlgo(all, failed);       break;
    }
}

// Throws std::runtime_error if the strategy 2 loop would need too many shifts.
template <class Container>
void checkEraseLoop(const Container &all) {
    double shifts = eraseLoopShifts(all);
    if (shifts > MAX_SHIFTS) {
        std::ostringstream msg;
        msg << "the strategy 2 loop would shift students " << std::scientific << std::setprecision(1)
            << shifts << " times in this " << containerName(all)
            << " (it could take hours). Use split 4 or 5, or a smaller file.";
        throw std::runtime_error(msg.str());
    }
}

// Runs the whole job on one file with the chosen container type and split method:
// read -> sort -> split -> write two files, timing every step.
// Output files: <name>_<container>_passed.txt and <name>_<container>_failed.txt
// Use it like:  processFile<std::list<Student>>("students_1000.txt", 3, Split::CopyLoop);
template <class Container>
Timings processFile(const string &filename, int sortChoice, Split method) {
    Timings t;
    Timer step;

    step.reset();
    Container all = readStudents<Container>(filename);
    t.read = step.elapsed();
    t.count = all.size();

    step.reset();
    sortStudents(all, sortChoice);
    t.sort = step.elapsed();

    if (method == Split::MoveLoop) checkEraseLoop(all);

    Container passed, failed;
    step.reset();
    runSplit(method, all, passed, failed);
    t.split = step.elapsed();

    // After strategy 2 the passed students are the ones left in 'all'.
    const Container &passedGroup = isStrategy1(method) ? passed : all;
    t.passed = passedGroup.size();
    t.failed = failed.size();
    t.stored = all.size() + passed.size() + failed.size();
    t.memoryMB = approxMB(all) + approxMB(passed) + approxMB(failed);

    string base = std::filesystem::path(filename).stem().string() + "_" + containerName(all);
    step.reset();
    saveTable(passedGroup, base + "_passed.txt");
    saveTable(failed, base + "_failed.txt");
    t.write = step.elapsed();

    t.total = t.read + t.sort + t.split + t.write;
    return t;
}

void printTimings(const string &title, const Timings &t, ostream &out = cout);

// v1.0 speed tests (menu options 3 and 4). Both print a table and also save it as a
// Markdown file (strategy_test_sort1.md, algorithm_test_vector_sort1.md, ...), ready to
// paste into README.md.
// strategyTest:  strategy 1 and 2 (hand-written loops) in list, deque and vector.
// algorithmTest: all five split methods in one container (1 list, 2 deque, 3 vector).
void strategyTest(size_t howMany, int sortChoice);
void algorithmTest(size_t howMany, int sortChoice, int container);
