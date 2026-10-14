#pragma once
#include "Student.h"

// Creates a file with 'count' random students (Name1 Surname1, Name2 Surname2, ...),
// each with numHW homework scores and one exam score.
// Throws std::runtime_error if the file cannot be created.
void generateStudentFile(const string &filename, size_t count, int numHW);

// ---- v0.25: the functions below are TEMPLATES, so the very same code works with
// ---- std::vector<Student>, std::list<Student> and std::deque<Student>.
// ---- (Template code must live in the header, that is why it is here.)

// Reads a students file. The first line is a header and is skipped. Every other
// line is: name surname score score ... score  (the LAST score is the exam).
// Throws std::runtime_error if the file cannot be opened or a line is broken.
// Use it like:  auto g = readStudents<std::list<Student>>("file.txt");
template <class Container>
Container readStudents(const string &filename) {
    ifstream file(filename);
    if (!file)
        throw std::runtime_error("could not open file: " + filename);

    string line;
    if (!std::getline(file, line))                  // header row
        throw std::runtime_error("file is empty: " + filename);

    Container Group;
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

// Prints the "Final (Avg.) | Final (Med.)" table with 2 decimals.
// Does NOT sort. Defaults to the screen, but can print into a file stream.
template <class Container>
void printTable(const Container &Group, ostream &out = cout) {
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

// Same table, but written into a new file. Throws std::runtime_error on failure.
template <class Container>
void saveTable(const Container &Group, const string &filename) {
    ofstream out(filename);
    if (!out)
        throw std::runtime_error("could not create file: " + filename);

    printTable(Group, out);

    out.close();
    if (!out)
        throw std::runtime_error("error while writing " + filename + " (disk full?)");
}
