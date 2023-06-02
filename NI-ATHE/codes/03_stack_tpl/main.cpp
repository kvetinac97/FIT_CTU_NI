#include <stdexcept>
#include <iostream>
#include "CStack.h"

using namespace std;

typedef string T;
#define RANDOM_VALUE "test"

int main () {
    CStack<T> stack;
    while (true) {
        cout << "Enter the operation (and a string):" << endl;
        char op = 'x'; T num;
        cin >> op;
        switch (op) {
            case '+':
                if (!(cin >> num)) {
                    cout << "Error: invalid string" << endl;
                    break;
                }

                try {
                    stack.push(num);
                    cout << "Added " << num << " to stack" << endl;
                }
                catch (const logic_error & error) {
                    cout << error.what() << endl;
                }
                break;
            case '-':
                try {
                    cout << "Number: " << stack.pop() << endl;
                }
                catch (const logic_error & error) {
                    cout << error.what() << endl;
                }
                break;
            case '?':
                cout << "Stack empty: " << stack.empty() << endl;
                cout << "Stack full: " << stack.full() << endl;
                break;
            case 'c':
                cout << "Copying stack..." << endl;
                {
                    CStack<T> other = stack;
                    other.push(RANDOM_VALUE);
                    other.pop();
                    cout << "Other here: " << other.empty() << endl;
                }
                break;
            case 'p':
                cout << stack << endl;
                break;
            case 'm':
                cout << "Moving stack..." << endl;
                {
                    CStack<T> other;
                    other.push(RANDOM_VALUE);
                    other.pop();
                    cout << "Other here: " << other.empty() << endl;
                    other = stack;
                    cout << "Other moved: " << other.empty() << endl;
                }
                break;
            case 'q':
                return 0;
            default:
                if (cin.eof()) return 0;
                cout << "Invalid operation " << op << ", try again." << endl;
                cin.clear();
                break;
        }
    }
}
