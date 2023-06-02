#pragma once

#include <iostream>

const int DEFAULT_STACK_SIZE = 20;

template<typename T>
class CStack {
    T * m_Values;
    size_t m_MaxSize;
    size_t m_Top;

    void detach() {
        delete [] m_Values;
        m_Values = nullptr;
    }
    void attach(const CStack<T> & other) {
        m_MaxSize = other.m_MaxSize;
        m_Top = other.m_Top;

        m_Values = new T [m_MaxSize];
        for (size_t i = 0; i < m_MaxSize; ++i)
            m_Values[i] = other.m_Values[i];
    }
public:
    explicit inline CStack(int size = DEFAULT_STACK_SIZE)
    : m_Values(new T [size]), m_MaxSize(size), m_Top(0) {}
    ~CStack() {
        detach();
    }
    CStack(const CStack<T> & other)
    : m_MaxSize(0), m_Top(0), m_Values(nullptr) {
        attach(other);
    }
    CStack<T> & operator = (const CStack<T> & other) {
        if (this == &other)
            return *this;

        detach();
        attach(other);
        return *this;
    }

    bool empty() const {
        return !m_Top;
    }
    bool full() const {
        return m_Top == m_MaxSize;
    }
    void push(T x) {
        if (full())
            throw std::logic_error("Cannot push into a full stack.");
        m_Values[m_Top++] = x;
    }
    T pop() {
        if (empty())
            throw std::logic_error("Cannot pop from an empty stack.");
        return m_Values[--m_Top];
    }

    friend std::ostream & operator << (std::ostream & os, const CStack<T> & stack) {
        os << "CStackInt(top=" << stack.m_Top << ",size=" << stack.m_MaxSize << ",values=[";
        for (size_t i = 0; i < stack.m_Top; ++i) {
            if (i) os << ",";
            os << stack.m_Values[i];
        }
        os << "])";
        return os;
    }
};
