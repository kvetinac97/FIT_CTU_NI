#pragma once

#include <iostream>

class CString {
    char * m_Str = nullptr;
    size_t m_Len = 0;
    size_t * m_RefCnt = nullptr;

    void detach();
    void attach(const CString & str);
public:
    CString (const char * str = "");
    CString (const CString & other);
    ~CString();
    CString & operator = (const CString & other);

    size_t size() const;

    CString operator + (const CString & other);
    CString & operator += (const CString & other);

    char operator [] (size_t index) const;
    char & operator [] (size_t index);

    bool operator < (const CString & other) const;
    bool operator == (const CString & other) const;
    bool operator != (const CString & other) const;
    bool operator <= (const CString & other) const;
    bool operator > (const CString & other) const;
    bool operator >= (const CString & other) const;

    friend std::ostream & operator << (std::ostream & os, const CString & str);
    friend int main();
};
