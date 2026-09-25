#pragma once
#include "lib.h"

class Student {
    std::string name, surname;
    std::vector<int> SemPoints;
    int exam;

public:
    Student();                                              // default: leaves everything empty
    Student(std::string N, std::string SN, std::vector<int> SP, int E); // full constructor
    Student(const Student &B);                              // copy constructor
    Student& operator=(const Student &A);                   // copy-assignment operator
    ~Student();                                              // destructor

    void print();
    void clear();
    double rezults();
    string Name();

    // "friend" lets these free functions reach the private members above.
    friend std::ostream& operator<<(std::ostream& out, Student& A);  // for cout << A
    friend std::istream& operator>>(std::istream& in, Student& A);   // for cin  >> A
};

bool lessStud(Student A, Student B);
bool MoreStud(Student A, Student B);
bool lessAver(Student A, Student B);
bool MoreAver(Student A, Student B);
    