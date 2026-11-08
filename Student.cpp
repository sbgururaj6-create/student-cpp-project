#include "Student.h"

namespace {
    const int MIN_SCORE = 0;
    const int MAX_SCORE = 10;

    bool outsideRange(int score) { return score < MIN_SCORE || score > MAX_SCORE; }

    void checkScore(int score) {
        if (outsideRange(score))
            throw std::out_of_range("score " + std::to_string(score) + " is outside " +
                                    std::to_string(MIN_SCORE) + "-" + std::to_string(MAX_SCORE));
    }

    int randomScore() { return rand() % 10 + 1; }
}

Student::Student(string N, string SN, vector<int> SP, int E)
    : name(std::move(N)), surname(std::move(SN)), SemPoints(std::move(SP)), exam(E) {
    calculate();
}

// Random-score constructor: invents numHW homework scores and an exam score.
// v1.0: std::generate_n calls randomScore() numHW times and adds each result to SemPoints.
Student::Student(string N, string SN, int numHW)
    : name(std::move(N)), surname(std::move(SN)) {
    std::generate_n(std::back_inserter(SemPoints), numHW, randomScore);
    exam = randomScore();
    calculate();
}

void Student::calculate() {
    if (SemPoints.empty())
        throw std::invalid_argument("student " + name + " " + surname + " has no homework scores");

    // v1.0: std::find_if returns the FIRST score outside 0-10, or end() if every score is fine.
    auto bad = std::find_if(SemPoints.begin(), SemPoints.end(), outsideRange);
    if (bad != SemPoints.end()) checkScore(*bad);    // throws, naming the bad score
    checkScore(exam);

    // 40% homework + 60% exam. Homework part = AVERAGE ...
    finalAvg = 0.4 * accumulate(SemPoints.begin(), SemPoints.end(), 0.0) / static_cast<double>(SemPoints.size())
               + 0.6 * exam;

    // ... or the MEDIAN (middle value) of the homework scores.
    vector<int> sorted = SemPoints;
    std::sort(sorted.begin(), sorted.end());
    size_t n = sorted.size();
    double med;
    if (n % 2 == 0)
        med = (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;   // even count: average the two middle values
    else
        med = sorted[n / 2];                               // odd count: the single middle value
    finalMed = 0.4 * med + 0.6 * exam;
}

// v1.0: std::copy sends every homework score to the file, each one followed by a space.
void Student::writeRecord(ostream &out) const {
    out << name << ' ' << surname << ' ';
    std::copy(SemPoints.begin(), SemPoints.end(), std::ostream_iterator<int>(out, " "));
    out << exam << '\n';
}

bool lessStud(const Student &A, const Student &B) { return A.Name() < B.Name(); }
bool MoreStud(const Student &A, const Student &B) { return A.Name() > B.Name(); }
bool lessAver(const Student &A, const Student &B) { return A.rezults() < B.rezults(); }
bool MoreAver(const Student &A, const Student &B) { return A.rezults() > B.rezults(); }
bool lessMed(const Student &A, const Student &B)  { return A.median() < B.median(); }
bool MoreMed(const Student &A, const Student &B)  { return A.median() > B.median(); }
