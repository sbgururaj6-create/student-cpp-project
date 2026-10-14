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
