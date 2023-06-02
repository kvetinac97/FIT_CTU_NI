#include "CStack.h"

bool CStackInt::empty() const {
    return !m_Top;
}

bool CStackInt::full() const {
    return m_Top == MAX_STACK_SIZE;
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
