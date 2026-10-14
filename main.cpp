#include "lib.h"
#include "Processing.h"

// The five test file sizes required by the task.
const vector<size_t> FILE_SIZES = {1000, 10000, 100000, 1000000, 10000000};

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
    int sortChoice = askSortChoice();

    Timings t = processFile(filename, sortChoice);
    printTimings(filename, t);
    cout << "\nSaved " << std::filesystem::path(filename).stem().string() << "_passed.txt and "
         << std::filesystem::path(filename).stem().string() << "_failed.txt\n";
}

void speedTest() {
    int howMany = askInt("Test how many files, smallest first (1-5)? ", 1, 5);
    int sortChoice = askSortChoice();

    vector<std::pair<size_t, Timings>> results;
    for (size_t i = 0; i < static_cast<size_t>(howMany); i++) {
        string filename = testFileName(FILE_SIZES[i]);
        cout << "Working on " << filename << " ...\n";
        try {
            results.push_back({FILE_SIZES[i], processFile(filename, sortChoice)});
        } catch (const std::exception &e) {
            cout << "  skipped: " << e.what() << "\n";   // e.g. file not generated yet
        }
    }

    cout << "\nSpeed test (vector), seconds\n";
    cout << right << setw(10) << "Records" << setw(10) << "Read" << setw(10) << "Sort"
         << setw(10) << "Split" << setw(10) << "Write" << setw(10) << "TOTAL" << "\n";
    cout << string(60, '-') << "\n" << std::fixed << std::setprecision(4);
    for (const auto &r : results) {
        const Timings &t = r.second;
        cout << setw(10) << r.first << setw(10) << t.read << setw(10) << t.sort
             << setw(10) << t.split << setw(10) << t.write << setw(10) << t.total << "\n";
    }
}

int main() {
    srand((unsigned)time(0));   // seed rand() once, so numbers differ each run

    while (true) {
        cout << "\n=== Students (v0.2, std::vector) ===\n"
             << "1) Generate the 5 test files (1 000 ... 10 000 000 students)\n"
             << "2) Process ONE file: read, sort, split, write, with timing\n"
             << "3) Speed test on the test files\n"
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
