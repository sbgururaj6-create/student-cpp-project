#include "Student.h"

Student::Student() {
    cout << "Input name: ";    cin >> name;
    cout << "Input surname: "; cin >> surname;

    int n;
    cout << "Input semester points: ";
    while (true) {
        cin >> n;
        SemPoints.push_back(n);
        cout << "want to input another point? y/n ";
        char answ; cin >> answ;
        if (answ == 'n' || answ == 'N') break;
    }

    cout << "Input final exam point: ";
    cin >> exam;
}

Student::Student(std::string N, std::string SN, std::vector<int> SP, int E) {
    name = N;
    surname = SN;
    SemPoints = SP;
    exam = E;
}

// NEW: random-score constructor. Give it a name, a surname, and how many
// homework scores to invent, and it fills SemPoints and exam itself.
// Uses a range of 1-10 to match the hand-typed example data you've seen
// so far (change the "+ 1" / "% 10" below if your scale is different).
Student::Student(std::string N, std::string SN, int numHW) {
    name = N;
    surname = SN;
    for (int i = 0; i < numHW; i++) {
        SemPoints.push_back(rand() % 10 + 1);
    }
    exam = rand() % 10 + 1;
}

Student::Student(const Student &B) {
    name = B.name;
    surname = B.surname;
    SemPoints = B.SemPoints;
    exam = B.exam;
}

void Student::print() {
    cout << name << "|" << surname << "|";
    for (int a : SemPoints) cout << a << "|";
    cout << exam << "|   |" << rezults() << "\n";
}

Student::~Student() {
    name.clear();
    surname.clear();
    SemPoints.clear();
    exam = 0;
}

void Student::clear() {
    name.clear();
    surname.clear();
    SemPoints.clear();
    exam = 0;
}

double Student::rezults() {
    return 0.4 * accumulate(SemPoints.begin(), SemPoints.end(), 0.0) / SemPoints.size()
           + exam * 0.6;
}

// NEW: same 40/60 weighting as rezults(), but using the MIDDLE homework
// score instead of the average one.
double Student::median() {
    vector<int> sorted = SemPoints;
    std::sort(sorted.begin(), sorted.end());

    int n = (int)sorted.size();
    double med = 0;
    if (n > 0) {
        if (n % 2 == 0)
            med = (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;   // even count: average the two middle values
        else
            med = sorted[n / 2];                               // odd count: take the single middle value
    }

    return 0.4 * med + exam * 0.6;
}

string Student::Name() {
    return name;
}

string Student::Surname() {
    return surname;
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
bool lessMed(Student A, Student B) {
    return A.median() < B.median();
}
bool MoreMed(Student A, Student B) {
    return A.median() > B.median();
}

// NEW: reads Students.txt. First line is a header (column titles) and is
// skipped. For every line after that: first token = name, second = surname,
// then every remaining number on the line is a score — the LAST one is
// treated as the exam, everything before it as homework. This works
// whether a file has 5 homework columns or 7, since we don't hardcode
// how many numbers to expect.
vector<Student> readStudents(const string &filename) {
    vector<Student> Group;
    ifstream file(filename);

    if (!file) {
        cout << "Could not open file: " << filename << "\n";
        return Group;
    }

    string line;
    std::getline(file, line);   // throw away the header row

    while (std::getline(file, line)) {
        istringstream iss(line);
        string name, surname;
        if (!(iss >> name >> surname)) continue;   // skip blank/bad lines

        vector<int> numbers;
        int x;
        while (iss >> x) numbers.push_back(x);
        if (numbers.empty()) continue;

        int exam = numbers.back();
        numbers.pop_back();                         // whatever's left is homework
        Group.push_back(Student(name, surname, numbers, exam));
    }

    return Group;
}

// NEW: the required side-by-side table, 2 decimal places. Does NOT sort
// — main.cpp sorts Group before calling this, based on the user's choice.
// "out" defaults to the screen (cout) but can be a file stream instead —
// see the comment on the declaration in Student.h.
void printTable(vector<Student> &Group, std::ostream &out) {
    // Note: setw() only sets a MINIMUM width — if a name is longer than
    // that, it prints in full with no padding, so without an explicit
    // space between columns, two long fields can run into each other.
    // Adding the " " after each name/surname guarantees a gap no matter
    // how long the text is.
    out << left << setw(10) << "Name" << " " << setw(12) << "Surname" << " "
        << right << setw(13) << "Final (Avg.)" << " |" << setw(13) << "Final (Med.)" << "\n";
    out << string(54, '-') << "\n";

    out << std::fixed << std::setprecision(2);
    for (Student &B : Group) {
        out << left << setw(10) << B.Name() << " " << setw(12) << B.Surname() << " "
            << right << setw(13) << B.rezults() << " |" << setw(13) << B.median() << "\n";
    }
}
