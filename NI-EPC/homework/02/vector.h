#ifndef EPC_VECTOR_H
#define EPC_VECTOR_H

#include <cstdlib>
#include <algorithm>
#include <memory>

namespace epc {
    template <typename T>
    class vector {
    	size_t m_Size, m_Capacity;
    	unsigned char * m_Buffer;
    public:
        vector() noexcept
        : m_Size(0), m_Capacity(0), m_Buffer (nullptr) {}        

        vector(const vector& other) : m_Size(other.m_Size),
        	m_Capacity(other.m_Capacity), m_Buffer(nullptr) {
        	if ( !m_Capacity ) return;
        	m_Buffer = new unsigned char [m_Capacity * sizeof(T)]; // might throw, no cleanup
        	try {
        		std::uninitialized_copy_n ( other.data(), other.m_Size, reinterpret_cast<T*>(m_Buffer) );
    		}
    		catch (...) {
    			delete [] m_Buffer;
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
        	std::destroy ( data(), data() + m_Size ); // destructors won't throw
        	delete [] m_Buffer;
        }

        T* data() {
        	return reinterpret_cast<T*> ( m_Buffer );
        }
        const T* data() const {
        	return reinterpret_cast<T*> ( m_Buffer );
        }

        T& operator[](size_t ix) { 
        	return data()[ix];
        }
        const T& operator[](size_t ix) const { 
        	return data()[ix];
        }

        void push_back(const T& elem) {
        	if (m_Size == m_Capacity)
                reserve(m_Capacity ? m_Capacity * 2 : 1); // might throw, no cleanup
            new (data() + m_Size) T ( elem ); // might throw
            ++m_Size;
        }
        void pop_back() {
        	std::destroy_at ( data() + m_Size - 1 ); // call destructor
        	--m_Size;
        }

        void clear() {
        	std::destroy ( data(), data() + m_Size ); // call destructors
        	m_Size = 0;
        }

        void reserve(size_t newCapacity) {
        	if (newCapacity <= m_Capacity)
                return;
            unsigned char * newBuffer = new unsigned char [newCapacity * sizeof(T)]; // might throw, no cleanup
            try {
            	std::uninitialized_copy_n ( data(), m_Size, reinterpret_cast<T*>(newBuffer) );
            }
            catch (...) {
                delete [] newBuffer;
                throw;
            }
            std::destroy ( data(), data() + m_Size ); // won't throw
            delete [] m_Buffer; // won't throw
            m_Buffer = newBuffer;
            m_Capacity = newCapacity;
        }

        size_t capacity() const { 
        	return m_Capacity;
        }
        size_t size() const {
        	return m_Size;
        }

        void swap(vector& other) noexcept {
        	std::swap (m_Buffer, other.m_Buffer);
            std::swap (m_Size, other.m_Size);
            std::swap (m_Capacity, other.m_Capacity);
        }
    };
}

#endif
