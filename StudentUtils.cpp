#include "StudentUtils.h"

void sortStudents(vector<Student> &Group, int choice) {
    switch (choice) {
        case 1: std::sort(Group.begin(), Group.end(), lessStud); break;
        case 2: std::sort(Group.begin(), Group.end(), MoreStud); break;
        case 3: std::sort(Group.begin(), Group.end(), lessAver); break;
        case 4: std::sort(Group.begin(), Group.end(), MoreAver); break;
        case 5: std::sort(Group.begin(), Group.end(), lessMed);  break;
        case 6: std::sort(Group.begin(), Group.end(), MoreMed);  break;
        default: throw std::invalid_argument("unknown sort choice: " + std::to_string(choice));
    }
}

void splitStudents(vector<Student> &all, vector<Student> &passed, vector<Student> &failed) {
    for (Student &s : all) {
        if (s.rezults() >= 5.0) passed.push_back(std::move(s));   // move = cheap, no copying
        else                    failed.push_back(std::move(s));
    }
    vector<Student>().swap(all);    // empty 'all' AND give its memory back
}
