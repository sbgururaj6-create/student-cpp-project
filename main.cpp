#include "lib.h"
#include "Student.h"

int main() {
    srand((unsigned)time(0));   // seed rand() once, so numbers differ each run

    vector<Student> Group;

    cout << "1) Read students from a file\n";
    cout << "2) Randomly generate students\n";
    cout << "Choice: ";
    char choice; cin >> choice;

    if (choice == '1') {
        cout << "Filename (e.g. Students.txt): ";
        string filename; cin >> filename;
        Group = readStudents(filename);
        cout << "Loaded " << Group.size() << " students.\n";
    } else {
        cout << "How many students? ";
        int count; cin >> count;
        cout << "How many homework scores each? ";
        int numHW; cin >> numHW;

        for (int i = 1; i <= count; i++) {
            Group.push_back(Student("Name" + std::to_string(i), "Surname" + std::to_string(i), numHW));
        }
    }

    cout << "How sort student list?\n";
    cout << "1) Name A-Z\n";
    cout << "2) Name Z-A\n";
    cout << "3) Average grade, low to high\n";
    cout << "4) Average grade, high to low\n";
    cout << "5) Median grade, low to high\n";
    cout << "6) Median grade, high to low\n";
    cout << "Choice: ";
    char sel; cin >> sel;

    switch (sel) {
        case '1': std::sort(Group.begin(), Group.end(), lessStud); break;
        case '2': std::sort(Group.begin(), Group.end(), MoreStud); break;
        case '3': std::sort(Group.begin(), Group.end(), lessAver); break;
        case '4': std::sort(Group.begin(), Group.end(), MoreAver); break;
        case '5': std::sort(Group.begin(), Group.end(), lessMed);  break;
        case '6': std::sort(Group.begin(), Group.end(), MoreMed);  break;
        default: break;   // anything else: leave the order as loaded
    }

    // Write the table into results.txt ourselves, instead of relying on
    // the terminal to redirect it — this avoids any confusion about
    // whether the program is "stuck" waiting for input.
    std::ofstream outFile("results.txt");
    if (!outFile) {
        cout << "Could not create results.txt\n";
        return 1;
    }
    printTable(Group, outFile);
    outFile.close();

    cout << "Done! Results saved to results.txt, in the same folder as this program.\n";

    return 0;
}
