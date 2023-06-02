#pragma once

#include <iostream>

const int MAX_STACK_SIZE = 20;

class CStackInt {
    int m_Values [MAX_STACK_SIZE];
    size_t m_Top;
public:
    CStackInt() : m_Values(), m_Top(0) {}
    bool empty() const;
    bool full() const;
    void push(int);
    int pop();
};
