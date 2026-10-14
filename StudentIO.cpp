#include "StudentIO.h"

void generateStudentFile(const string &filename, size_t count, int numHW) {
    if (numHW < 1)
        throw std::invalid_argument("need at least 1 homework score per student");

    ofstream out(filename);
    if (!out)
        throw std::runtime_error("could not create file: " + filename);

    out << "Name Surname";
    for (int i = 1; i <= numHW; i++) out << " ND" << i;
    out << " Egz.\n";

    for (size_t i = 1; i <= count; i++) {
        Student s("Name" + std::to_string(i), "Surname" + std::to_string(i), numHW);
        s.writeRecord(out);
    }

    out.close();
    if (!out)
        throw std::runtime_error("error while writing " + filename + " (disk full?)");
}

vector<Student> readStudents(const string &filename) {
    ifstream file(filename);
    if (!file)
        throw std::runtime_error("could not open file: " + filename);

    string line;
    if (!std::getline(file, line))                  // header row
        throw std::runtime_error("file is empty: " + filename);

    vector<Student> Group;
    size_t lineNo = 1;

    while (std::getline(file, line)) {
        lineNo++;
        istringstream iss(line);
        string name, surname;
        if (!(iss >> name >> surname)) continue;    // blank line

        vector<int> numbers;
        int x;
        while (iss >> x) numbers.push_back(x);

        string where = "line " + std::to_string(lineNo) + ": ";
        if (!iss.eof())                             // stopped on something that is not a number
            throw std::runtime_error(where + "a score is not a number");
        if (numbers.size() < 2)
            throw std::runtime_error(where + "need at least one homework score and an exam score");

        int exam = numbers.back();
        numbers.pop_back();                         // whatever is left is homework

        try {
            Group.push_back(Student(std::move(name), std::move(surname), std::move(numbers), exam));
        } catch (const std::logic_error &e) {       // a bad score (e.g. outside 0-10); NOT out-of-memory
            throw std::runtime_error(where + e.what());
        }
    }

    return Group;
}

void printTable(const vector<Student> &Group, ostream &out) {
    // setw() only sets a MINIMUM width; the " " after each name guarantees a gap
    // even when a name is longer than the column.
    out << left << setw(14) << "Name" << " " << setw(17) << "Surname" << " "
        << right << setw(13) << "Final (Avg.)" << " |" << setw(13) << "Final (Med.)" << "\n";
    out << string(61, '-') << "\n";

    out << std::fixed << std::setprecision(2);
    for (const Student &B : Group) {
        out << left << setw(14) << B.Name() << " " << setw(17) << B.Surname() << " "
            << right << setw(13) << B.rezults() << " |" << setw(13) << B.median() << "\n";
    }
}

void saveTable(const vector<Student> &Group, const string &filename) {
    ofstream out(filename);
    if (!out)
        throw std::runtime_error("could not create file: " + filename);

    printTable(Group, out);

    out.close();
    if (!out)
        throw std::runtime_error("error while writing " + filename + " (disk full?)");
}
