#pragma once
#include "lib.h"

class Student {
    string name, surname;
    vector<int> SemPoints;
    int exam = 0;
    double finalAvg = 0, finalMed = 0;   // calculated ONCE, so sorting stays fast

    void calculate();                    // checks the scores, fills finalAvg / finalMed

public:
    Student() = default;
    Student(string N, string SN, vector<int> SP, int E);
    Student(string N, string SN, int numHW);   // random scores (1-10)

    void print() const;
    void writeRecord(ostream &out) const;      // one line: name surname hw... exam

    double rezults() const { return finalAvg; }   // final grade, AVERAGE of homework
    double median() const { return finalMed; }    // final grade, MEDIAN of homework
    const string &Name() const { return name; }
    const string &Surname() const { return surname; }
};

bool lessStud(const Student &A, const Student &B);   // name A-Z
bool MoreStud(const Student &A, const Student &B);   // name Z-A
bool lessAver(const Student &A, const Student &B);   // average grade low-high
bool MoreAver(const Student &A, const Student &B);   // average grade high-low
bool lessMed(const Student &A, const Student &B);    // median grade low-high
bool MoreMed(const Student &A, const Student &B);    // median grade high-low
