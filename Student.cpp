#include "Student.h"

// Default constructor. All the input logic now lives in operator>>,
// so this is deliberately left empty.
Student::Student() {
}

Student::Student(std::string N, std::string SN, std::vector<int> SP, int E) {
    name = N;
    surname = SN;
    SemPoints = SP;
    exam = E;
}

Student::Student(const Student &B) {
    name = B.name;
    surname = B.surname;
    SemPoints = B.SemPoints;
    exam = B.exam;
}

// Copy-assignment: runs when you write `A = B;` on two Students that already exist
// (the copy constructor above only runs when a NEW Student is being created).
Student& Student::operator=(const Student &A) {
    if (this == &A) return *this;   // guard against self-assignment, e.g. `A = A;`
    name = A.name;
    surname = A.surname;
    SemPoints = A.SemPoints;
    exam = A.exam;
    return *this;
}

Student::~Student() {
    name.clear();
    surname.clear();
    SemPoints.clear();
    exam = 0;
}

void Student::print() {
    cout << "|" << setw(10) << name << "|" << surname << "|"
         << exam << "|   |" << rezults() << "\n";
}

void Student::clear() {
    name.clear();
    surname.clear();
    SemPoints.clear();
    exam = 0;
}

double Student::rezults() {
    if (SemPoints.empty()) return exam * 0.6;   // avoid dividing by zero
    return 0.4 * accumulate(SemPoints.begin(), SemPoints.end(), 0.0) / SemPoints.size()
           + exam * 0.6;
}

// Lets you write: cout << someStudent;
std::ostream& operator<<(std::ostream& out, Student& A) {
    out << "|" << setw(10) << A.name << "|" << A.surname << "|"
        << A.exam << "|   |" << A.rezults() << "\n";
    return out;
}

// Lets you write: cin >> someStudent;
std::istream& operator>>(std::istream& in, Student& A) {
    in >> A.name >> A.surname;

    int n;
    cout << "Input semester points (enter a non-number, e.g. x, to stop): ";
    while (cin >> n) {
        A.SemPoints.push_back(n);
    }
    // cin >> n fails once you type something that isn't a number — that sets the
    // stream's fail flag and leaves the bad input sitting in the buffer. We MUST
    // clear the flag and throw that leftover text away before reading anything
    // else, or every read after this point will silently fail too.
    cin.clear();
    cin.ignore(100, '\n');

    cout << "Input final exam point: ";
    in >> A.exam;
    cin.clear();
    cin.ignore(100, '\n');
    return in;
}



string Student::Name(){

return name;
}


bool lessStud(Student A, Student B) {
    return A.Name() < B.Name();
}

bool MoreStud(Student A, Student B) {
    return A.Name() > B.Name();
}

bool lessAver(Student A, Student B) {
    return A.rezults() < B.rezults();
}

bool MoreAver(Student A, Student B) {
    return A.rezults() > B.rezults();
}