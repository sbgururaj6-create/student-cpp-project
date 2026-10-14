# Student grades - release v0.2

Reads students (name, surname, homework scores, exam score), calculates the final
grade (40% homework + 60% exam, using the average **and** the median of homework),
sorts them, splits them into **passed** (final >= 5.0) and **failed** (final < 5.0)
and writes each group into its own file. Every step is timed. This release uses `std::vector`.

## What changed since v0.1
- Code split into several `.h` / `.cpp` files (see below).
- Classes: `Student` and `Timer`; a `Timings` struct holds the measurements.
- Exception handling: missing/empty file, broken line, score outside 0-10, a student with no
  homework, bad menu input and out-of-memory are caught and shown as a message instead of a crash.
- Generates test files of 1 000 / 10 000 / 100 000 / 1 000 000 / 10 000 000 students
  (`Name1 Surname1`, `Name2 Surname2`, ...).
- Splitting into passed / failed, written to two new files.
- Speed analysis of reading, sorting, splitting and writing.
- Comparison functions take `const Student &` (v0.1 copied two students on every comparison).
- Final grades are calculated once per student, not on every comparison.
- Removed the hand-written copy constructor / destructor / `clear()`: the compiler-made ones are
  correct, and they also allow cheap "moves" when students are split into two containers.
  The unused console-input constructor was removed too.

## Files
| File | Purpose |
|---|---|
| `lib.h` | shared `#include`s and `using` lines |
| `Student.h/.cpp` | class `Student` + sort comparison functions |
| `Timer.h` | class `Timer` (stopwatch) |
| `StudentIO.h/.cpp` | generate a file, read a file, print/save the table |
| `StudentUtils.h/.cpp` | sorting and splitting |
| `Processing.h/.cpp` | runs read -> sort -> split -> write and times each step |
| `main.cpp` | menu |

## Input file format
First line = header (skipped). Every other line: `name surname hw1 hw2 ... hwN exam`
(the last number is the exam; the number of homework scores can differ between files).
Scores must be 0-10.

## Build and run
```
g++ -std=c++17 -O2 -Wall -Wextra main.cpp Student.cpp StudentIO.cpp StudentUtils.cpp Processing.cpp -o output/main.exe
```
(or the VS Code build task). Use `-O2`: without optimization the timings are not meaningful.
Run from the folder where you want the files created.

Output files are named `<input name>_passed.txt` and `<input name>_failed.txt`.
Generated data files are not committed to GitHub (see `.gitignore`) - they are too large.

## Speed analysis (vector) - fill in from your own computer
Menu option 3 prints this table. Seconds.

| Records | Read | Sort | Split | Write | TOTAL |
|---|---|---|---|---|---|
| 1 000 | | | | | |
| 10 000 | | | | | |
| 100 000 | | | | | |
| 1 000 000 | | | | | |
| 10 000 000 | | | | | |

Computer: (CPU, RAM, compiler, flags)
