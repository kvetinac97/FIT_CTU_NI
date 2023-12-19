#ifndef EPC_VECTOR_H
#define EPC_VECTOR_H

#include <cstdlib>
#include <algorithm>

namespace epc
{
    template <typename T>
    class vector
    {
        size_t m_Capacity, m_Size;
        T * m_Data;
    public:
        vector() noexcept : m_Capacity (0), m_Size (0), m_Data (nullptr) {}        

        vector(const vector & other) : m_Size(other.m_Size),
            m_Capacity(other.m_Capacity), m_Data(nullptr) {
            m_Data = m_Capacity ? new T[m_Capacity] : nullptr;
            try {
                std::copy(other.m_Data, other.m_Data + other.m_Size, m_Data);
            }
            catch (...) {
                delete [] m_Data;
                throw;
            }
        }
        vector& operator=(const vector& other) {
            if (this != &other) {
                vector tmp = other; // might throw, no cleanup
                swap(tmp);
            }
            return *this;
        }

        ~vector() {
            delete [] m_Data; // won't throw
        }

        T* data() {
            return m_Data;
        }
        const T* data() const {
            return m_Data;
        }

        T& operator[](size_t i) {
            return m_Data[i];
        }
        const T& operator[](size_t i) const { 
            return m_Data[i];
        }

        void push_back(const T& elem) {
            if (m_Size == m_Capacity)
                reserve(m_Capacity ? m_Capacity * 2 : 1); // might throw, no cleanup
            m_Data[m_Size] = elem; // might throw
            ++m_Size;
        }
        void pop_back() { 
            --m_Size;
        }

        void reserve(size_t newCapacity) {
            if (newCapacity <= m_Capacity)
                return;
            T * newData = new T[newCapacity]; // might throw, no cleanup
            try {
                std::copy (m_Data, m_Data + m_Size, newData);
            }
            catch (...) {
                delete [] newData;
                throw;
            }
            delete [] m_Data; // won't throw
            m_Data = newData;
            m_Capacity = newCapacity;
        }

        size_t capacity() const {
            return m_Capacity;
        }
        size_t size() const {
            return m_Size;
        }

        void swap(vector& other) noexcept {
            std::swap (m_Data, other.m_Data);
            std::swap (m_Size, other.m_Size);
            std::swap (m_Capacity, other.m_Capacity);
        }
    };
}

#endif
