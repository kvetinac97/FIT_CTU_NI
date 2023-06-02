#include "CStack.h"

void CStackInt::detach() {
    delete [] m_Values;
    m_Values = nullptr;
}

void CStackInt::attach(const CStackInt & other) {
    m_MaxSize = other.m_MaxSize;
    m_Top = other.m_Top;

    m_Values = new int [m_MaxSize];
    for (size_t i = 0; i < m_MaxSize; ++i)
        m_Values[i] = other.m_Values[i];
}

CStackInt::~CStackInt() {
    detach();
}

CStackInt::CStackInt(const CStackInt & other)
: m_MaxSize(0), m_Top(0), m_Values(nullptr) {
    attach(other);
}

CStackInt & CStackInt::operator= (const CStackInt & other) {
    if (this == &other)
        return *this;

    detach();
    attach(other);
    return *this;
}

bool CStackInt::empty() const {
    return !m_Top;
}

bool CStackInt::full() const {
    return m_Top == m_MaxSize;
}

void CStackInt::push(int x) {
    if (full())
        throw std::logic_error("Cannot push into a full stack.");
    m_Values[m_Top++] = x;
}

int CStackInt::pop() {
    if (empty())
        throw std::logic_error("Cannot pop from an empty stack.");
    return m_Values[--m_Top];
}

std::ostream & operator << (std::ostream & os, const CStackInt & stack) {
    os << "CStackInt(top=" << stack.m_Top << ",size=" << stack.m_MaxSize << ",values=[";
    for (size_t i = 0; i < stack.m_Top; ++i) {
        if (i) os << ",";
        os << stack.m_Values[i];
    }
    os << "])";
    return os;
}
