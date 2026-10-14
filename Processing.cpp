#include "Processing.h"

Timings processFile(const string &filename, int sortChoice) {
    Timings t;
    Timer whole, step;

    step.reset();
    vector<Student> all = readStudents(filename);
    t.read = step.elapsed();
    t.count = all.size();

    step.reset();
    sortStudents(all, sortChoice);
    t.sort = step.elapsed();

    vector<Student> passed, failed;
    step.reset();
    splitStudents(all, passed, failed);
    t.split = step.elapsed();
    t.passed = passed.size();
    t.failed = failed.size();

    string base = std::filesystem::path(filename).stem().string();
    step.reset();
    saveTable(passed, base + "_passed.txt");
    saveTable(failed, base + "_failed.txt");
    t.write = step.elapsed();

    t.total = whole.elapsed();
    return t;
}

void printTimings(const string &filename, const Timings &t, ostream &out) {
    out << "\nFile: " << filename << "  (" << t.count << " students: "
        << t.passed << " passed, " << t.failed << " failed)\n";
    out << std::fixed << std::setprecision(4);
    out << "  Read from file : " << t.read  << " s\n";
    out << "  Sorting        : " << t.sort  << " s\n";
    out << "  Splitting      : " << t.split << " s\n";
    out << "  Writing 2 files: " << t.write << " s\n";
    out << "  TOTAL          : " << t.total << " s\n";
}
