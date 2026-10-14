#include "Processing.h"

void printTimings(const string &title, const Timings &t, ostream &out) {
    out << "\n" << title << "  (" << t.count << " students: "
        << t.passed << " passed, " << t.failed << " failed)\n";
    out << std::fixed << std::setprecision(4);
    out << "  Read from file : " << t.read  << " s\n";
    out << "  Sorting        : " << t.sort  << " s\n";
    out << "  Splitting      : " << t.split << " s\n";
    out << "  Writing 2 files: " << t.write << " s\n";
    out << "  TOTAL          : " << t.total << " s\n";
}
