#include "lib.h"
#include "Processing.h"

// Container 1 and 2 are the ones task 2 asked for; vector is the third one to compare.
const vector<string> CONTAINER_NAMES = {"list", "deque", "vector"};

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
         << "3) std::vector\n";
    return askInt("Choice: ", 1, 3);
}

Split askSplit() {
    cout << "\nHow to split the students into passed and failed?\n"
         << "1) Strategy 1, for loop: copy everyone into two new containers\n"
         << "2) Strategy 2, for loop: move the failed out, erase them one by one\n"
         << "3) Strategy 1, <algorithm>: remove_copy_if + copy_if\n"
         << "4) Strategy 2, <algorithm>: stable_partition + copy + erase\n"
         << "5) Strategy 2, <algorithm>: copy_if + remove_if + erase\n";
    return static_cast<Split>(askInt("Choice: ", 1, 5));
}

// Picks the right container type for the number the person chose.
Timings processWith(int container, const string &filename, int sortChoice, Split method) {
    switch (container) {
        case 1:  return processFile<list<Student>>(filename, sortChoice, method);
        case 2:  return processFile<deque<Student>>(filename, sortChoice, method);
        default: return processFile<vector<Student>>(filename, sortChoice, method);
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
    Split method = askSplit();
    int sortChoice = askSortChoice();

    Timings t = processWith(container, filename, sortChoice, method);
    const string cname = CONTAINER_NAMES[static_cast<size_t>(container - 1)];
    printTimings(filename + " [" + cname + ", " + splitName(method) + "]", t);

    string base = std::filesystem::path(filename).stem().string() + "_" + cname;
    cout << "\nSaved " << base << "_passed.txt and " << base << "_failed.txt\n";
}

void runStrategyTest() {
    int howMany = askInt("Test how many files, smallest first (1-5)? ", 1, 5);
    int sortChoice = askSortChoice();
    strategyTest(static_cast<size_t>(howMany), sortChoice);
}

void runAlgorithmTest() {
    int howMany = askInt("Test how many files, smallest first (1-5)? ", 1, 5);
    int container = askContainer();
    int sortChoice = askSortChoice();
    algorithmTest(static_cast<size_t>(howMany), sortChoice, container);
}

int main() {
    srand((unsigned)time(0));   // seed rand() once, so numbers differ each run

    while (true) {
        cout << "\n=== Students (v1.0) ===\n"
             << "1) Generate the 5 test files (1 000 ... 10 000 000 students)\n"
             << "2) Process ONE file: read, sort, split, write, with timing\n"
             << "3) Strategy test: strategy 1 and 2 in list, deque and vector\n"
             << "4) Algorithm test: for loops vs <algorithm> in one container\n"
             << "0) Quit\n";

        try {
            int choice = askInt("Choice: ", 0, 4);
            if (choice == 0) break;
            if (choice == 1) generateFiles();
            else if (choice == 2) processOne();
            else if (choice == 3) runStrategyTest();
            else runAlgorithmTest();
        } catch (const std::bad_alloc &) {
            cout << "Error: the computer ran out of memory. Try a smaller file.\n";
        } catch (const std::exception &e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}
