# Student grades - release v0.25 (std::list and std::deque)

Same program as v0.2, but the student container is a template parameter, so exactly the same
code runs with `std::list`, `std::deque` and (as a baseline) `std::vector`.

## What changed since v0.2
- `readStudents`, `printTable`, `saveTable`, `splitStudents`, `sortStudents`, `processFile` are
  templates (they live in the headers `StudentIO.h`, `StudentUtils.h`, `Processing.h`).
- `std::list` cannot use `std::sort` (it is not random access), so it uses its own `list::sort()`.
- Menu asks which container to use; option 3 runs every test file with list, deque and vector.
- Output files include the container name, e.g. `students_1000_list_passed.txt`.

## Build and run
```
g++ -std=c++17 -O2 -Wall -Wextra main.cpp Student.cpp StudentIO.cpp Processing.cpp -o output/main.exe
```

## Speed analysis - fill in from your own computer
Menu option 3 prints this table. Seconds.

| Records | Container | Read | Sort | Split | Write | TOTAL |
|---|---|---|---|---|---|---|
| 1 000 | list | | | | | |
| 1 000 | deque | | | | | |
| 1 000 | vector | | | | | |
| ... | | | | | | |

Computer: (CPU, RAM, compiler, flags)
