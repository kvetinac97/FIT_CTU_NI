#include <stdexcept>
#include <iostream>
#include "CStack.h"

using namespace std;

int main () {
    CStackInt stack;
    while (true) {
        cout << "Enter the operation (and a number):" << endl;
        char op = 'x'; int num = -1;
        cin >> op;
        switch (op) {
            case '+':
                cin >> num;
                if (num == -1) {
                    cout << "Error: invalid number" << endl;
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
                    CStackInt other = stack;
                    other.push(3);
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
                    CStackInt other;
                    other.push(3);
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
