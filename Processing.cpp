#include "Processing.h"

const vector<size_t> FILE_SIZES = {1000, 10000, 100000, 1000000, 10000000};

string testFileName(size_t records) {
    return "students_" + std::to_string(records) + ".txt";
}

string splitName(Split method) {
    switch (method) {
        case Split::CopyLoop:      return "strategy 1, for loop";
        case Split::MoveLoop:      return "strategy 2, for loop";
        case Split::CopyAlgo:      return "strategy 1, remove_copy_if + copy_if";
        case Split::PartitionAlgo: return "strategy 2, stable_partition";
        case Split::RemoveAlgo:    return "strategy 2, copy_if + remove_if";
    }
    return "unknown split";
}

bool isStrategy1(Split method) {
    return method == Split::CopyLoop || method == Split::CopyAlgo;
}

void printTimings(const string &title, const Timings &t, ostream &out) {
    out << "\n" << title << "  (" << t.count << " students: "
        << t.passed << " passed, " << t.failed << " failed)\n";
    out << std::fixed << std::setprecision(4);
    out << "  Read from file : " << t.read  << " s\n";
    out << "  Sorting        : " << t.sort  << " s\n";
    out << "  Splitting      : " << t.split << " s\n";
    out << "  Writing 2 files: " << t.write << " s\n";
    out << "  TOTAL          : " << t.total << " s\n";
    out << std::setprecision(2);
    out << "  In memory after the split: " << t.stored << " students, about " << t.memoryMB << " MB\n";
}

namespace {

// A number as text with a fixed count of decimals, e.g. fixedText(0.5, 4) is "0.5000".
string fixedText(double value, int decimals) {
    std::ostringstream text;
    text << std::fixed << std::setprecision(decimals) << value;
    return text.str();
}

// Prints a table on the screen and saves the same table as a Markdown file,
// so it can be pasted straight into README.md.
// Throws std::runtime_error if the Markdown file cannot be written.
void showTable(const string &title, const vector<string> &header,
               const vector<vector<string>> &rows, const string &mdFile) {
    vector<size_t> width(header.size());
    for (size_t c = 0; c < header.size(); c++) {
        width[c] = header[c].size();
        for (const vector<string> &row : rows) width[c] = std::max(width[c], row[c].size());
    }

    size_t lineLength = 0;
    cout << "\n" << title << "\n";
    for (size_t c = 0; c < header.size(); c++) {
        cout << right << setw(static_cast<int>(width[c] + 2)) << header[c];
        lineLength += width[c] + 2;
    }
    cout << "\n" << string(lineLength, '-') << "\n";
    for (const vector<string> &row : rows) {
        for (size_t c = 0; c < row.size(); c++) cout << setw(static_cast<int>(width[c] + 2)) << row[c];
        cout << "\n";
    }

    ofstream md(mdFile);
    if (!md)
        throw std::runtime_error("could not create file: " + mdFile);
    md << "**" << title << "**\n\n|";
    for (const string &h : header) md << ' ' << h << " |";
    md << "\n|";
    for (size_t c = 0; c < header.size(); c++) md << (c == 0 ? " --- |" : " ---: |");
    md << "\n";
    for (const vector<string> &row : rows) {
        md << "|";
        for (const string &cell : row) md << ' ' << cell << " |";
        md << "\n";
    }
    md.close();
    if (!md)
        throw std::runtime_error("error while writing " + mdFile);
    cout << "\nSaved this table to " << mdFile << "\n";
}

// Reads and sorts one file, printing how long that took.
template <class Container>
Container readAndSort(const string &filename, int sortChoice) {
    Timer t;
    Container all = readStudents<Container>(filename);
    double readTime = t.elapsed();
    t.reset();
    sortStudents(all, sortChoice);
    cout << "  " << left << setw(7) << containerName(all) << right << "read " << fixedText(readTime, 4)
         << " s, sort " << fixedText(t.elapsed(), 4) << " s" << std::flush;
    return all;
}

// ---- Timing one split. A run shorter than REPEAT_BELOW seconds is done twice and the
// ---- faster time is kept: the first run can be slower only because the computer is
// ---- still handing out fresh memory to the program.

const double REPEAT_BELOW = 1.0;

// Strategy 1 does not change 'all', so both runs can use it directly.
template <class Container, class SplitFunction>
double timeStrategy1(const Container &all, SplitFunction split) {
    double best = 0;
    for (int run = 1; run <= 2; run++) {
        Container passed, failed;
        Timer t;
        split(all, passed, failed);
        double time = t.elapsed();
        if (run == 1 || time < best) best = time;
        if (best >= REPEAT_BELOW) break;
    }
    return best;
}

// Strategy 2 changes the container, so every run gets its own fresh copy of 'all'.
// (Making the copy is not timed.)
template <class Container, class SplitFunction>
double timeStrategy2(const Container &all, SplitFunction split) {
    double best = 0;
    for (int run = 1; run <= 2; run++) {
        Container work = all, failed;
        Timer t;
        split(work, failed);
        double time = t.elapsed();
        if (run == 1 || time < best) best = time;
        if (best >= REPEAT_BELOW) break;
    }
    return best;
}

// ---- Option 3: strategy 1 vs strategy 2 (hand-written loops) ----

struct StrategyRow {
    double split1 = 0, split2 = 0;    // seconds
    bool skipped2 = false;            // strategy 2 loop would take far too long
    double memory1 = 0, memory2 = 0;  // MB of students held right after the split
};

template <class Container>
StrategyRow strategyRun(const string &filename, int sortChoice) {
    StrategyRow row;
    Container all = readAndSort<Container>(filename, sortChoice);

    // Strategy 1: 'all' keeps everyone AND every student is copied into 'passed' or
    // 'failed', so right after the split each student is stored twice.
    row.split1 = timeStrategy1(all, splitCopyLoop<Container>);
    row.memory1 = 2 * approxMB(all);

    // Strategy 2: the failed students move out of 'all', so each student is stored once.
    row.memory2 = approxMB(all);
    if (eraseLoopShifts(all) > MAX_SHIFTS)
        row.skipped2 = true;
    else
        row.split2 = timeStrategy2(all, splitMoveLoop<Container>);

    cout << ", split done\n";
    return row;
}

StrategyRow strategyRunIn(int container, const string &filename, int sortChoice) {
    switch (container) {
        case 1:  return strategyRun<list<Student>>(filename, sortChoice);
        case 2:  return strategyRun<deque<Student>>(filename, sortChoice);
        default: return strategyRun<vector<Student>>(filename, sortChoice);
    }
}

const vector<string> CONTAINERS = {"list", "deque", "vector"};

// ---- Option 4: all five split methods in one container ----

struct AlgoRow {
    double time[5] = {0, 0, 0, 0, 0};   // seconds, index = menu number - 1
    bool skipped[5] = {false, false, false, false, false};
};

template <class Container>
AlgoRow algorithmRun(const string &filename, int sortChoice) {
    AlgoRow row;
    Container all = readAndSort<Container>(filename, sortChoice);

    row.time[0] = timeStrategy1(all, splitCopyLoop<Container>);
    if (eraseLoopShifts(all) > MAX_SHIFTS)
        row.skipped[1] = true;
    else
        row.time[1] = timeStrategy2(all, splitMoveLoop<Container>);
    row.time[2] = timeStrategy1(all, splitCopyAlgo<Container>);
    row.time[3] = timeStrategy2(all, splitPartitionAlgo<Container>);
    row.time[4] = timeStrategy2(all, splitRemoveAlgo<Container>);

    cout << ", split done\n";
    return row;
}

AlgoRow algorithmRunIn(int container, const string &filename, int sortChoice) {
    switch (container) {
        case 1:  return algorithmRun<list<Student>>(filename, sortChoice);
        case 2:  return algorithmRun<deque<Student>>(filename, sortChoice);
        default: return algorithmRun<vector<Student>>(filename, sortChoice);
    }
}

}   // namespace

void strategyTest(size_t howMany, int sortChoice) {
    vector<vector<string>> rows;

    for (size_t i = 0; i < howMany; i++) {
        string filename = testFileName(FILE_SIZES[i]);
        cout << "Working on " << filename << " ...\n";
        for (int c = 1; c <= 3; c++) {
            try {
                StrategyRow r = strategyRunIn(c, filename, sortChoice);
                rows.push_back({std::to_string(FILE_SIZES[i]), CONTAINERS[static_cast<size_t>(c - 1)],
                                fixedText(r.split1, 6), r.skipped2 ? "skipped" : fixedText(r.split2, 6),
                                fixedText(r.memory1, 2), fixedText(r.memory2, 2)});
            } catch (const std::bad_alloc &) {
                cout << "\n  skipped: out of memory\n";
            } catch (const std::exception &e) {
                cout << "  skipped: " << e.what() << "\n";
                break;   // e.g. the file was not generated: no point trying the other containers
            }
        }
    }

    showTable("Strategy test, sort choice " + std::to_string(sortChoice) + ", split time in seconds",
              {"Records", "Container", "Strategy 1 (s)", "Strategy 2 (s)", "Memory 1 (MB)", "Memory 2 (MB)"},
              rows, "strategy_test_sort" + std::to_string(sortChoice) + ".md");
}

void algorithmTest(size_t howMany, int sortChoice, int container) {
    vector<vector<string>> rows;

    for (size_t i = 0; i < howMany; i++) {
        string filename = testFileName(FILE_SIZES[i]);
        cout << "Working on " << filename << " ...\n";
        try {
            AlgoRow r = algorithmRunIn(container, filename, sortChoice);
            vector<string> row = {std::to_string(FILE_SIZES[i])};
            for (int m = 0; m < 5; m++)
                row.push_back(r.skipped[m] ? "skipped" : fixedText(r.time[m], 6));
            rows.push_back(row);
        } catch (const std::bad_alloc &) {
            cout << "\n  skipped: out of memory\n";
        } catch (const std::exception &e) {
            cout << "  skipped: " << e.what() << "\n";
        }
    }

    showTable("Algorithm test (" + CONTAINERS[static_cast<size_t>(container - 1)] + "), sort choice " +
              std::to_string(sortChoice) + ", split time in seconds",
              {"Records", "1 S1 loop", "2 S2 loop", "3 S1 copy_if", "4 S2 stable_partition", "5 S2 remove_if"},
              rows, "algorithm_test_" + CONTAINERS[static_cast<size_t>(container - 1)] +
                    "_sort" + std::to_string(sortChoice) + ".md");
}
