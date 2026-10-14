#pragma once
#include "Student.h"

// Creates a file with 'count' random students (Name1 Surname1, Name2 Surname2, ...),
// each with numHW homework scores and one exam score.
// Throws std::runtime_error if the file cannot be created.
void generateStudentFile(const string &filename, size_t count, int numHW);

// Reads a students file. The first line is a header and is skipped. Every other
// line is: name surname score score ... score  (the LAST score is the exam).
// Throws std::runtime_error if the file cannot be opened or a line is broken.
vector<Student> readStudents(const string &filename);

// Prints the "Final (Avg.) | Final (Med.)" table with 2 decimals.
// Does NOT sort. Defaults to the screen, but can print into a file stream.
void printTable(const vector<Student> &Group, ostream &out = cout);

// Same table, but written into a new file. Throws std::runtime_error on failure.
void saveTable(const vector<Student> &Group, const string &filename);
