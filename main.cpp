#include "lib.h"
#include "Student.h"

int main() {
    // Example of the full constructor, no keyboard input needed:
    // Student A("Name", "Surname", {6,7,8,9,6,7,8,9}, 10);

    vector<Student> Group;

    while (true) {
        Student A;      // empty student
        cin >> A;       // fill it in via operator>>

        Student B(A);   // copy constructor: B is a separate copy of A

        cout << B;              // print it via operator<<
        Group.push_back(B);     // store another copy of B in the group
        B.clear();               // empty B now that its data is saved (it's about to
                                  // go out of scope anyway, but this shows clear() works)

        cout << "want to input another Student? y/n ";
        char answ; cin >> answ;
        if (answ == 'n' || answ == 'N') break;
    }

        cout<<"How to sort student list? 1,2,3,4" ; char sel;
    cin >> sel;
    switch(sel)
    {
        case '1':
        std::sort (Group.begin(),Group.end(),lessStud);
        break;
          case '2':
        std::sort (Group.begin(),Group.end(),MoreStud);
        break;
          case '3':
        std::sort (Group.begin(),Group.end(),lessAver);
        break;
          case '4':
        std::sort (Group.begin(),Group.end(),MoreAver);
        break;
        default:
        break;
    }




    for (Student B : Group) cout << B;

    return 0;
}
