#include <iostream>
#include "CString.h"

using namespace std;

int main() {
    CString str = "ahoj";
    cout << str << " " << str.size() << endl;

    CString str2 = str;
    str2[1] = 'c';
    cout << str2 << endl;

    CString str3;
    str3 = str + str2;
    cout << str3 << endl;

    CString str4;
    str4 = str3;

    if (str < str4) cout << "Smaller" << endl;
    cout << *str.m_RefCnt << endl;
    cout << *str2.m_RefCnt << endl;
    cout << *str3.m_RefCnt << endl;
    return 0;
}