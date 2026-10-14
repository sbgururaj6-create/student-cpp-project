#include "lib.h"
#include "Processing.h"

// The five test file sizes required by the task.
const vector<size_t> FILE_SIZES = {1000, 10000, 100000, 1000000, 10000000};

// Container 1 and 2 are the ones the task asks for; vector is kept as a baseline
// so v0.2 and v0.25 results can be compared in the same table.
const vector<string> CONTAINER_NAMES = {"list", "deque", "vector"};

string testFileName(size_t records) {
    return "students_" + std::to_string(records) + ".txt";
}

// Asks until the person types a whole number between lo and hi.
// (std::stoi throws an exception on text like "abc" - we catch it and ask again.)
int askInt(const string &prompt, int lo, int hi) {
    while (true) {
        cout << prompt;
        string line;
        if (!std::getline(cin, line))
            throw std::runtime_error("input closed");
        try {
            size_t used = 0;
            int value = std::stoi(line, &used);
            if (used == line.size() && value >= lo && value <= hi) return value;
        } catch (const std::exception &) {
            // not a number, or too big for an int: fall through and ask again
        }
        cout << "Please type a whole number from " << lo << " to " << hi << ".\n";
    }
}

int askSortChoice() {
    cout << "\nHow to sort the student list?\n"
         << "1) Name A-Z\n"
         << "2) Name Z-A\n"
         << "3) Average grade, low to high\n"
         << "4) Average grade, high to low\n"
         << "5) Median grade, low to high\n"
         << "6) Median grade, high to low\n";
    return askInt("Choice: ", 1, 6);
}

int askContainer() {
    cout << "\nWhich container?\n"
         << "1) std::list\n"
         << "2) std::deque\n"
         << "3) std::vector (for comparison)\n";
    return askInt("Choice: ", 1, 3);
}

// Picks the right container type for the number the person chose.
Timings processWith(int container, const string &filename, int sortChoice) {
    switch (container) {
        case 1:  return processFile<list<Student>>(filename, sortChoice);
        case 2:  return processFile<deque<Student>>(filename, sortChoice);
        default: return processFile<vector<Student>>(filename, sortChoice);
    }
}

void generateFiles() {
    int numHW = askInt("How many homework scores per student (1-50)? ", 1, 50);
    for (size_t n : FILE_SIZES) {
        Timer t;
        generateStudentFile(testFileName(n), n, numHW);
        cout << "Created " << testFileName(n) << " (" << n << " students) in "
             << std::fixed << std::setprecision(2) << t.elapsed() << " s\n";
    }
}

void processOne() {
    cout << "Filename (e.g. students_1000.txt): ";
    string filename;
    std::getline(cin, filename);
    int container = askContainer();
    
    int sortChoice = askSortChoice();

    Timings t = processWith(container, filename, sortChoice);
    const string cname = CONTAINER_NAMES[static_cast<size_t>(container - 1)];
    printTimings(filename + " [" + cname + "]", t);

    string base = std::filesystem::path(filename).stem().string() + "_" + cname;
    cout << "\nSaved " << base << "_passed.txt and " << base << "_failed.txt\n";
}

void speedTest() {
    int howMany = askInt("Test how many files, smallest first (1-5)? ", 1, 5);
    int sortChoice = askSortChoice();

    struct Row { size_t records; string container; Timings t; };
    vector<Row> rows;

    for (size_t i = 0; i < static_cast<size_t>(howMany); i++) {
        string filename = testFileName(FILE_SIZES[i]);
        for (int c = 1; c <= 3; c++) {
            const string cname = CONTAINER_NAMES[static_cast<size_t>(c - 1)];
            cout << "Working on " << filename << " with " << cname << " ...\n";
            try {
                rows.push_back({FILE_SIZES[i], cname, processWith(c, filename, sortChoice)});
            } catch (const std::bad_alloc &) {
                cout << "  skipped: out of memory\n";
            } catch (const std::exception &e) {
                cout << "  skipped: " << e.what() << "\n";   // e.g. file not generated yet
            }
        }
    }

    cout << "\nSpeed test, seconds\n";
    cout << right << setw(10) << "Records" << setw(9) << "Type" << setw(10) << "Read" << setw(10) << "Sort"
         << setw(10) << "Split" << setw(10) << "Write" << setw(10) << "TOTAL" << "\n";
    cout << string(69, '-') << "\n" << std::fixed << std::setprecision(4);
    for (const Row &r : rows) {
        cout << setw(10) << r.records << setw(9) << r.container << setw(10) << r.t.read << setw(10) << r.t.sort
             << setw(10) << r.t.split << setw(10) << r.t.write << setw(10) << r.t.total << "\n";
    }
}

int main() {
    srand((unsigned)time(0));   // seed rand() once, so numbers differ each run

    while (true) {
        cout << "\n=== Students (v0.25, std::list and std::deque) ===\n"
             << "1) Generate the 5 test files (1 000 ... 10 000 000 students)\n"
             << "2) Process ONE file: read, sort, split, write, with timing\n"
             << "3) Speed test on the test files (list, deque, vector)\n"
             << "0) Quit\n";

        try {
            int choice = askInt("Choice: ", 0, 3);
            if (choice == 0) break;
            if (choice == 1) generateFiles();
            else if (choice == 2) processOne();
            else speedTest();
        } catch (const std::bad_alloc &) {
            cout << "Error: the computer ran out of memory. Try a smaller file.\n";
        } catch (const std::exception &e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}
