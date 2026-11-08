# Student grades (C++)

A console program that reads students from a text file (name, surname, homework scores, exam
score), calculates every student's final grade, sorts the students, splits them into **passed**
(final grade ≥ 5.0) and **failed** (final grade < 5.0), writes both groups into new files, and
measures how long every step takes.

Final grade = 40 % homework + 60 % exam. The homework part is calculated in two ways, from the
**average** and from the **median** of the homework scores. The split into passed and failed uses
the average-based grade.

1. [Releases](#releases)
2. [Usage guide](#usage-guide)
3. [Input and output files](#input-and-output-files)
4. [Results](#results)
5. [Project files](#project-files)
6. [Errors the program handles](#errors-the-program-handles)

## Releases

| Release | In one sentence |
|---|---|
| [v0.1](https://github.com/sbgururaj6-create/student-cpp-project/releases/tag/v0.1) | Reads or generates students, calculates both grades, sorts them, writes `results.txt`. |
| [v0.2](https://github.com/sbgururaj6-create/student-cpp-project/releases/tag/v0.2) | Classes and header files, exceptions, test files of 1 000 to 10 000 000 students, passed and failed files, every step timed (`std::vector`). |
| [v0.25](https://github.com/sbgururaj6-create/student-cpp-project/releases/tag/v0.25) | The same code as templates, run with `std::list`, `std::deque` and `std::vector`. |
| [v1.0](https://github.com/sbgururaj6-create/student-cpp-project/releases/tag/v1.0) | Two split strategies, the split rewritten with `<algorithm>`, Makefile and CMake, a clean repository. |

### v0.1
- One `Student` class: name, surname, homework scores, exam score.
- Students come from a file, or are generated with random scores (`Name1 Surname1`, `Name2 Surname2`, ...).
- Final grade from the average and from the median of the homework.
- Six sort orders: name A-Z or Z-A, average-based grade up or down, median-based grade up or down.
- The table is written into `results.txt`.

### v0.2
- The code is split into several `.h` / `.cpp` files: classes `Student` and `Timer`, a `Timings`
  struct, and separate files for input/output, sorting and splitting, and processing.
- Exceptions: a missing or empty file, a broken line, a score outside 0-10, wrong menu input and
  running out of memory are shown as a message instead of crashing the program.
- Menu option 1 generates the five test files: 1 000, 10 000, 100 000, 1 000 000 and 10 000 000 students.
- Students are split into passed and failed, and each group is written into its own file.
- Reading, sorting, splitting and writing are timed separately.
- Final grades are calculated once per student, and the sort comparisons take `const Student &`,
  so sorting does not copy students.

### v0.25
- Reading, sorting, splitting, writing and processing became templates, so exactly the same code
  runs with `std::list`, `std::deque` and `std::vector`.
- `std::list` cannot use `std::sort` (it cannot jump to a position), so a special version calls
  `list::sort()`.
- The menu asks which container to use, and the speed test runs every file with all three.

### v1.0
**Two split strategies, five ways to split.** Menu option 2 asks which one to use:

| Menu | Strategy | How |
|---|---|---|
| 1 | Strategy 1 | `for` loop: every student is **copied** into a new `passed` or `failed` container; the original keeps everyone. |
| 2 | Strategy 2 | `for` loop: every failed student is **moved** into `failed` and **erased** from the original one by one; then `shrink_to_fit`. |
| 3 | Strategy 1 | `std::remove_copy_if` copies the passed students, `std::copy_if` copies the failed ones. |
| 4 | Strategy 2 | `std::stable_partition` puts the passed students first, `std::copy` with move iterators moves the failed tail out, one `erase` cuts it off, `shrink_to_fit`. |
| 5 | Strategy 2 | `std::copy_if` copies the failed students out, `std::remove_if` + one `erase` removes them (the erase-remove idiom), `shrink_to_fit`. |

All five write exactly the same passed and failed files, in the chosen sort order.

**`<algorithm>` used instead of hand-written loops:**

| Algorithm | Where | Replaces |
|---|---|---|
| `std::find_if` | `Student::calculate` | the loop that checked every homework score is 0-10 |
| `std::copy` + `std::istream_iterator` | `readStudents` | `while (iss >> x) numbers.push_back(x);` |
| `std::copy` + `std::ostream_iterator` | `Student::writeRecord` | the loop that wrote the homework scores |
| `std::generate_n` | random `Student` constructor | the loop that made random scores |
| `std::copy_if`, `std::remove_copy_if` | split method 3 | the strategy 1 loop |
| `std::stable_partition`, `std::copy` | split method 4 | the strategy 2 loop |
| `std::copy_if`, `std::remove_if` | split method 5 | the strategy 2 loop |

`std::stable_partition` is used instead of `std::partition` because it keeps the sorted order inside
both groups, so the output files stay sorted.

**Two new speed tests.** Menu option 3 compares strategy 1 and 2 (the `for` loops) in list, deque
and vector. Menu option 4 compares all five split methods in one container. Both print a table and
save the same table as a Markdown file (`strategy_test_sort1.md`, `algorithm_test_vector_sort1.md`, ...).

**A safety check for the slow loop.** Before the strategy 2 loop runs, the program counts how many
times students would have to be shifted. Above 3 000 000 000 shifts it refuses (option 2) or skips
(options 3 and 4) the run, because it would take many minutes or hours.

**A clean repository.** A `Makefile` (Unix) and a `CMakeLists.txt` (any OS) were added. The
`.gitignore` keeps build results, generated data and editor settings (`.vscode/`) out of Git,
and the old `output/` files were removed from the repository. The unused `Student::print()` was removed.

## Usage guide

### 1. Build

**Linux or macOS, with the Makefile:**
```
make
```
This creates the program `output/main`. `make run` builds it if needed and starts it;
`make clean` deletes what was built.

**Any operating system, with CMake (3.16 or newer):**
```
cmake -S . -B build
cmake --build build
```
On Windows with MSYS2 (MinGW g++), tell CMake to use MinGW's make:
```
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```
CMake also puts the program in `output/` (`output\main.exe` on Windows).

**Only g++, no make or CMake:**
```
g++ -std=c++17 -O2 -Wall -Wextra main.cpp Student.cpp StudentIO.cpp Processing.cpp -o output/main
```

Always build with optimization (`-O2`; CMake builds "Release" by default). Without it the timings
mean very little.

### 2. Run
```
cd output
./main
```
On Windows: `cd output` and then `.\main.exe`. The program creates and reads its files in the
folder it runs in.

### 3. Use the menu
```
=== Students (v1.0) ===
1) Generate the 5 test files (1 000 ... 10 000 000 students)
2) Process ONE file: read, sort, split, write, with timing
3) Strategy test: strategy 1 and 2 in list, deque and vector
4) Algorithm test: for loops vs <algorithm> in one container
0) Quit
```
1. Choose **1** and type how many homework scores each student gets (the results below use 10).
   This creates `students_1000.txt` ... `students_10000000.txt`; the largest is about 500 MB.
2. Choose **2** to process one file. Type the file name, then pick the container (1 list, 2 deque,
   3 vector), the split method (1-5) and the sort order (1-6). The program prints the time of every
   step and how much memory the students take after the split, and writes two files.
3. Choose **3** for the strategy test. Pick how many test files to use (1-5, smallest first) and the
   sort order. The table is also saved as `strategy_test_sort<N>.md`.
4. Choose **4** for the algorithm test. Pick how many files, the container and the sort order. The
   table is also saved as `algorithm_test_<container>_sort<N>.md`.
5. Choose **0** to quit.

If you type something that is not one of the offered numbers, the program asks again.

## Input and output files

**Input.** The first line is a header and is skipped. Every other line is
`name surname hw1 hw2 ... hwN exam`: the last number is the exam score, all the numbers before it
are homework. Scores must be whole numbers from 0 to 10.
```
Name Surname ND1 ND2 ND3 Egz.
Name1 Surname1 8 9 7 6
Name2 Surname2 5 10 6 7
Name3 Surname3 2 3 4 4
```

**Output.** Menu option 2 writes `<input name>_<container>_passed.txt` and
`<input name>_<container>_failed.txt`. For the file above (`example.txt`, vector, sort order 4),
`example_vector_passed.txt` is:
```
Name           Surname            Final (Avg.) | Final (Med.)
-------------------------------------------------------------
Name2          Surname2                   7.00 |         6.60
Name1          Surname1                   6.80 |         6.80
```
and `example_vector_failed.txt` holds `Name3 Surname3` with 3.60 and 3.60.

## Results

All results were measured on my Windows laptop. Compiler: g++ from MSYS2 (UCRT64), `-std=c++17 -O2`.
Every test file has 10 homework scores and one exam score per student. Times are in seconds.

### v0.2: std::vector
Sort order: average grade, low to high (menu choice 3). Rows 1 000 to 1 000 000 come from one run
of the speed test; the 10 000 000 row comes from menu option 2.

| Records | Read | Sort | Split | Write | TOTAL |
|---:|---:|---:|---:|---:|---:|
| 1 000 | 0.0059 | 0.0002 | 0.0002 | 0.0040 | 0.0103 |
| 10 000 | 0.0358 | 0.0016 | 0.0010 | 0.0128 | 0.0511 |
| 100 000 | 0.3153 | 0.0212 | 0.0082 | 0.1198 | 0.4645 |
| 1 000 000 | 3.1151 | 0.2645 | 0.0792 | 1.0507 | 4.5096 |
| 10 000 000 | 31.5506 | 3.4801 | 0.8837 | 10.6864 | 46.6009 |

- For the larger files, 10 times more students takes about 10 times longer. Sorting grows a little
  faster (about 12 to 13 times), because sorting costs about n log n.
- Reading is the slowest step: 31.6 of the 46.6 seconds at 10 000 000 students, because every line
  is cut into words and every word is turned into a number.
- At 10 000 000 students, 5 864 353 passed and 4 135 647 failed.

### v0.25: list, deque and vector
Same sort order. Rows 1 000 to 1 000 000 come from one run of the speed test; the 10 000 000 rows
come from menu option 2, one container at a time.

| Records | Container | Read | Sort | Split | Write | TOTAL |
|---:|---|---:|---:|---:|---:|---:|
| 1 000 | list | 0.0202 | 0.0001 | 0.0001 | 0.0029 | 0.0234 |
| 1 000 | deque | 0.0027 | 0.0002 | 0.0001 | 0.0025 | 0.0054 |
| 1 000 | vector | 0.0027 | 0.0001 | 0.0001 | 0.0020 | 0.0050 |
| 10 000 | list | 0.0579 | 0.0012 | 0.0010 | 0.0126 | 0.0727 |
| 10 000 | deque | 0.0218 | 0.0017 | 0.0006 | 0.0123 | 0.0364 |
| 10 000 | vector | 0.0224 | 0.0019 | 0.0011 | 0.0122 | 0.0376 |
| 100 000 | list | 0.2322 | 0.0243 | 0.0175 | 0.1160 | 0.3900 |
| 100 000 | deque | 0.2205 | 0.0232 | 0.0067 | 0.1102 | 0.3606 |
| 100 000 | vector | 0.2213 | 0.0198 | 0.0080 | 0.1067 | 0.3559 |
| 1 000 000 | list | 2.2481 | 0.6051 | 0.3132 | 1.0953 | 4.2617 |
| 1 000 000 | deque | 2.1883 | 0.3263 | 0.0829 | 1.0762 | 3.6738 |
| 1 000 000 | vector | 2.1926 | 0.2752 | 0.1116 | 1.0380 | 3.6174 |
| 10 000 000 | list | 22.5720 | 17.1846 | 6.7985 | 21.8984 | 68.4536 |
| 10 000 000 | deque | 32.3259 | 4.0966 | 1.3415 | 11.2487 | 49.0130 |
| 10 000 000 | vector (v0.2) | 31.5506 | 3.4801 | 0.8837 | 10.6864 | 46.6009 |

- vector and deque perform almost the same: both keep students next to each other in memory
  (deque in short blocks) and can jump straight to any position.
- list is the slowest, above all in sorting and splitting: at 10 000 000 students it sorted in
  17.18 s, against 4.10 s for deque and 3.48 s for vector. Its students are separate nodes spread
  around memory, so the processor has to fetch almost every one separately.
- Reading and writing take about the same time in every container up to 1 000 000 students,
  because they mostly turn text into numbers and back. The 10 000 000 rows come from separate runs,
  and reading and writing times vary from run to run; the sorting and splitting gaps are much larger
  than that variation.

### v1.0: strategy 1 and strategy 2 (menu option 3)
Test files of 1 000 to 1 000 000 students. Sort order: name A-Z (menu choice 1), so the failed
students are spread through the whole list. Only the split is timed. "Memory" is the size of the
students held right after the split: every `Student` object plus its homework scores (the
containers' own extra bytes are not counted).

| Records | Container | Strategy 1 | Strategy 2 | Memory 1 (MB) | Memory 2 (MB) |
|---:|---|---:|---:|---:|---:|
| 1 000 | list | 0.000095 | 0.000022 | 0.29 | 0.14 |
| 1 000 | deque | 0.000093 | 0.001517 | 0.29 | 0.14 |
| 1 000 | vector | 0.000127 | 0.001381 | 0.29 | 0.14 |
| 10 000 | list | 0.001120 | 0.000276 | 2.90 | 1.45 |
| 10 000 | deque | 0.000895 | 0.072013 | 2.90 | 1.45 |
| 10 000 | vector | 0.001009 | 0.137312 | 2.90 | 1.45 |
| 100 000 | list | 0.013434 | 0.003569 | 28.99 | 14.50 |
| 100 000 | deque | 0.009737 | 32.578425 | 28.99 | 14.50 |
| 100 000 | vector | 0.020758 | 76.401162 | 28.99 | 14.50 |
| 1 000 000 | list | 0.273573 | 0.066290 | 289.92 | 144.96 |
| 1 000 000 | deque | 0.192636 | skipped | 289.92 | 144.96 |
| 1 000 000 | vector | 0.231425 | skipped | 289.92 | 144.96 |

- **Memory: the claim is true.** In every container strategy 1 holds twice as much as strategy 2,
  for example 289.92 MB against 144.96 MB for 1 000 000 students, because every student stays in
  the original container and also has a copy in `passed` or `failed`. One student takes 112 bytes
  plus 4 bytes per homework score, so 10 000 000 students with 10 homework scores would take about
  1 450 MB with strategy 2 and about 2 900 MB with strategy 1.
- **Strategy 1: deque was the fastest** from 10 000 students up: 0.193 s for 1 000 000 students,
  against 0.231 s for vector and 0.274 s for list. Every copy needs new memory. A deque adds
  students in fixed blocks and never moves the ones it already holds; a vector moves all its
  students into a bigger block every time it grows; a list asks for a separate node per student.
- **Strategy 2 in a `for` loop: list is by far the fastest, vector and deque are unusable.** For
  100 000 students list needed 0.0036 s, deque 32.6 s and vector 76.4 s, about 21 000 times longer
  than list. Erasing from a list only unlinks one node. Erasing from a vector shifts every student
  behind it one place to the left, and a deque shifts the shorter side, so about half as many.
  With about 4 in 10 students failing, that is billions of shifts: 10 times more students took
  450 to 560 times longer, which is the n × n growth. For 1 000 000 students the program skipped
  vector and deque, because they would need about 2 × 10<sup>11</sup> and 8 × 10<sup>10</sup>
  shifts, many hours of work.
- **The fastest container and strategy with plain loops is list with strategy 2**: 0.066 s for
  1 000 000 students, faster than every strategy 1 run, and with half the memory.
- Long runs vary on a laptop: the same vector loop with 100 000 students took 76.4 s here and
  30.5 s in the next test. The gap to list (0.0036 s) is far larger than that variation.

### v1.0: for loops and `<algorithm>` (menu option 4)
Same files and sort order. Column numbers are the split methods of menu option 2.

**vector**

| Records | 1: S1 loop | 2: S2 loop | 3: S1 copy_if | 4: S2 stable_partition | 5: S2 remove_if |
|---:|---:|---:|---:|---:|---:|
| 1 000 | 0.000216 | 0.001849 | 0.000111 | 0.000083 | 0.000063 |
| 10 000 | 0.001426 | 0.139969 | 0.001006 | 0.000764 | 0.001072 |
| 100 000 | 0.011215 | 30.525196 | 0.011954 | 0.007872 | 0.009353 |
| 1 000 000 | 0.110699 | skipped | 0.127392 | 0.082830 | 0.095530 |

**list**

| Records | 1: S1 loop | 2: S2 loop | 3: S1 copy_if | 4: S2 stable_partition | 5: S2 remove_if |
|---:|---:|---:|---:|---:|---:|
| 1 000 | 0.000130 | 0.000036 | 0.000115 | 0.000127 | 0.000085 |
| 10 000 | 0.000799 | 0.000254 | 0.000717 | 0.001057 | 0.000695 |
| 100 000 | 0.012743 | 0.003279 | 0.014019 | 0.011071 | 0.010044 |
| 1 000 000 | 0.127481 | 0.038416 | 0.153384 | 0.161368 | 0.124073 |

- **vector: the algorithms rescue strategy 2.** `std::stable_partition` split 100 000 students in
  0.0079 s instead of 30.5 s for the loop, about 3 900 times faster, and 1 000 000 students in
  0.083 s, where the loop could not run at all. Instead of shifting the rest of the vector after
  every single failed student, it moves each student only a few times, and one `erase` then
  removes all the failed students together.
- With the algorithms, strategy 2 became the fastest way to split a vector: 0.083 s
  (`stable_partition`) and 0.096 s (`remove_if`) against 0.111 s for the strategy 1 loop at
  1 000 000 students, while using half the memory.
- **Strategy 1 did not get faster with the algorithms:** 0.111 s (loop) against 0.127 s
  (`remove_copy_if` + `copy_if`) at 1 000 000 students. Both copy every student once, and the
  algorithm version reads the container twice, once for each group. Below 100 000 students the
  differences are fractions of a millisecond.
- **list: the algorithms made it slower.** The strategy 2 loop split 1 000 000 students in 0.038 s;
  `remove_if` took 0.124 s and `stable_partition` 0.161 s, 3 to 4 times longer. The general
  algorithms move whole `Student` objects from node to node (and `stable_partition` needs an extra
  buffer), while the loop moves each failed student once and simply unlinks its node.

### My choice
- **For the split alone**, the fastest was **list with strategy 2 in a `for` loop** (0.038 to
  0.066 s for 1 000 000 students). Rewriting it with algorithms made it slower, so for a list the
  loop stays.
- **For the whole program** I chose **vector with strategy 2 and `std::stable_partition`**
  (container 3, split method 4 in menu option 2):
  - its split is only a few hundredths of a second slower than list's (0.083 s at 1 000 000 students);
  - a vector reads and sorts much faster than a list: in v0.25 sorting took 0.28 s against 0.61 s
    for 1 000 000 students, and 3.48 s against 17.18 s for 10 000 000;
  - it keeps every student only once, half the memory of strategy 1;
  - `stable_partition` keeps the sorted order, so the output files stay sorted.

## Project files

| File | What is inside |
|---|---|
| `main.cpp` | the menu and the questions it asks |
| `lib.h` | shared `#include`s and `using` lines |
| `Timer.h` | class `Timer`, a stopwatch |
| `Student.h`, `Student.cpp` | class `Student`: scores, final grades, writing one record; the six sort comparisons |
| `StudentIO.h`, `StudentIO.cpp` | generating a test file; reading a file and writing a table (templates) |
| `StudentUtils.h` | sorting (templates) and the five split methods |
| `Processing.h`, `Processing.cpp` | the whole job for one file, timing, memory estimate, the shift count, speed tests 3 and 4 |
| `Makefile` | build on Unix with `make` |
| `CMakeLists.txt` | build on any operating system with CMake |
| `.gitignore` | keeps build results, data files and editor settings out of Git |

## Errors the program handles

| Situation | Exception | What you see |
|---|---|---|
| The file does not exist | `std::runtime_error` | `Error: could not open file: nope.txt` |
| The file is empty | `std::runtime_error` | `Error: file is empty: empty.txt` |
| A letter instead of a score | `std::runtime_error` | `Error: line 2: a score is not a number` |
| Only one number on a line | `std::runtime_error` | `Error: line 2: need at least one homework score and an exam score` |
| A score outside 0-10 | `std::out_of_range`, passed on as `std::runtime_error` with the line number | `Error: line 3: score 11 is outside 0-10` |
| An output file cannot be created | `std::runtime_error` | `Error: could not create file: ...` |
| The strategy 2 loop would take hours | `std::runtime_error` | `Error: the strategy 2 loop would shift students 2.1e+11 times in this vector ...` |
| Not enough memory | `std::bad_alloc` | `Error: the computer ran out of memory. Try a smaller file.` |
| Letters or a wrong number at a menu question | `std::invalid_argument` / `std::out_of_range` from `std::stoi`, caught in `askInt` | `Please type a whole number from 0 to 4.` |

After every error the menu comes back, so the program never has to be restarted.
