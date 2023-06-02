#pragma once

#include <iostream>

const int DEFAULT_STACK_SIZE = 20;

class CStackInt {
    int * m_Values;
    size_t m_MaxSize;
    size_t m_Top;

    void detach();
    void attach(const CStackInt &);
public:
    explicit inline CStackInt(int size = DEFAULT_STACK_SIZE)
    : m_Values(new int [size]), m_MaxSize(size), m_Top(0) {}
    ~CStackInt();
    CStackInt(const CStackInt &);
    CStackInt & operator = (const CStackInt &);

    bool empty() const;
    bool full() const;
    void push(int);
    int pop();

    friend std::ostream & operator << (std::ostream &, const CStackInt &);
};
