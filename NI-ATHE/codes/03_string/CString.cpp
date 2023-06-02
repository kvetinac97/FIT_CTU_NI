#include "CString.h"

void CString::detach() {
    if (--(*m_RefCnt) == 0) {
        delete [] m_Str;
        delete m_RefCnt;
    }
}

void CString::attach(const CString & str) {
    m_Str = str.m_Str;
    m_Len = str.m_Len;
    m_RefCnt = str.m_RefCnt;

    (*m_RefCnt)++;
}

CString::CString (const char * str) {
    m_Len = strlen(str);
    m_Str = new char [m_Len + 1];
    strcpy(m_Str, str);
    m_RefCnt = new size_t(1);
}

CString::CString (const CString & other) {
    attach(other);
}

CString::~CString() {
    detach();
}

CString & CString::operator = (const CString & other) {
    if (this != &other) {
        detach();
        attach(other);
    }
    return *this;
}

size_t CString::size() const {
    return m_Len;
}

CString CString::operator + (const CString & other) {
    CString result ("");
    result.m_Len = m_Len + other.m_Len;
    result.m_Str = new char[result.m_Len + 1];
    result.m_RefCnt = new size_t (1);
    strcpy(result.m_Str, m_Str);
    strcpy(result.m_Str + m_Len, other.m_Str);
    return result;
}

CString & CString::operator += (const CString & other) {
    *this = (*this + other);
    return *this;
}

char CString::operator [] (size_t index) const {
    if (index > m_Len)
        throw std::out_of_range("Index out of bounds");
    return m_Str[index];
}

char & CString::operator [] (size_t index) {
    if (index > m_Len)
        throw std::out_of_range("Index out of bounds");

    if (*m_RefCnt > 1) {
        std::cout << "Detaching" << std::endl;
        CString newString (m_Str);
        *this = newString;
    }
    return m_Str[index];
}

bool CString::operator < (const CString & other) const {
    return strcmp(m_Str, other.m_Str) < 0;
}
bool CString::operator == (const CString & other) const {
    return !(*this < other) && !(other < *this);
}
bool CString::operator != (const CString & other) const {
    return !(*this == other);
}
bool CString::operator <= (const CString & other) const {
    return (*this == other) || (*this < other);
}
bool CString::operator > (const CString & other) const {
    return !(*this <= other);
}
bool CString::operator >= (const CString & other) const {
    return !(*this < other);
}

std::ostream & operator << (std::ostream & os, const CString & str) {
    return os << str.m_Str;
}
