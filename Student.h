#pragma once
#include "lib.h"

class Student {
    std::string name, surname;
    std::vector<int> SemPoints;
    int exam;

public:
    Student();
    Student(std::string N, std::string SN, std::vector<int> SP, int E);
    Student(std::string N, std::string SN, int numHW);   // NEW: random-score constructor
    Student(const Student &B);
    ~Student();

    void print();
    void clear();
    double rezults();   // final grade using AVERAGE of homework
    double median();    // NEW: final grade using MEDIAN of homework
    string Name();
    string Surname();   // NEW: getter, needed for the table output
};

bool lessStud(Student A, Student B);   // name A-Z
bool MoreStud(Student A, Student B);   // name Z-A
bool lessAver(Student A, Student B);   // average grade low-high
bool MoreAver(Student A, Student B);   // average grade high-low
bool lessMed(Student A, Student B);    // NEW: median grade low-high
bool MoreMed(Student A, Student B);    // NEW: median grade high-low

// NEW: reads Students.txt (first line is a header and is skipped). Each
// remaining line is: name, surname, then any number of score columns —
// the LAST score on the line is treated as the exam, everything before
// it as homework. This is what lets it cope with a file that has 5
// homework columns or 7, without the code caring which.
vector<Student> readStudents(const string &filename);

// NEW: prints the two-column "Final (Avg.) | Final (Med.)" table,
// 2 decimal places. Does NOT sort — sort Group yourself first (see the
// comparator functions above) so main.cpp controls the order, not this
// function. Defaults to printing to the screen (cout), but you can pass
// a file stream instead, e.g.:
//   std::ofstream out("results.txt");
//   printTable(Group, out);
void printTable(vector<Student> &Group, std::ostream &out = cout);
