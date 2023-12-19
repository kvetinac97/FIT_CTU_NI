#ifndef EPC_VECTOR_H
#define EPC_VECTOR_H

#include <cstdlib>
#include <algorithm>
#include <memory>
#include <type_traits>

namespace epc {
    template <typename T, size_t N>
    class vector {
        size_t m_Size, m_Capacity;
        unsigned char * m_Data;
        alignas(T) unsigned char m_Buffer[N * sizeof(T)];  
        
        bool isShort () const {
            return m_Capacity == N;
        }      
    public:
        vector() noexcept
        : m_Size(0), m_Capacity(N), m_Data (m_Buffer) {}        

        vector(const vector& other) : m_Size(other.m_Size),
            m_Capacity(other.m_Capacity), m_Data (m_Buffer) {
            if ( !m_Capacity ) return;
            // copy buffer
            if ( other.isShort() ) {
                std::uninitialized_copy_n ( other.data(), other.m_Size,
                    reinterpret_cast<T*>(m_Buffer) ); // might throw, no cleanup
                return;
            }
            // other is long
            m_Data = new unsigned char [m_Capacity * sizeof(T)]; // might throw, no cleanup
            try {
                std::uninitialized_copy_n ( other.data(), other.m_Size, reinterpret_cast<T*>(m_Data) );
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
        
        vector(vector && other) : m_Size (other.m_Size),
        	m_Capacity(other.m_Capacity), m_Data(m_Buffer) {
        	if (!m_Capacity) return;
        	// other is short = move buffer
        	if ( other.isShort() ) {
                std::uninitialized_move_n ( other.data(), other.m_Size,
                    reinterpret_cast<T*>(m_Buffer) ); // might throw, no cleanup
                std::destroy ( other.data(), other.data() + other.m_Size );
            }
            // other is long = use it's data
            else
            	m_Data = other.m_Data;

            // make other vector short with size 0
            other.m_Data = other.m_Buffer;
            other.m_Capacity = N;
            other.m_Size = 0;
    	}
    	vector& operator=(vector&& other) { 
            if (this != &other) {
                vector tmp = std::move(other); // might throw, no cleanup
                swap(tmp);
            }
            return *this;
        }

        ~vector() { 
            std::destroy ( data(), data() + m_Size ); // destructors won't throw
            if ( !isShort() )
                delete [] m_Data;
        }

        T* data() {
            return reinterpret_cast<T*> ( m_Data );
        }
        const T* data() const {
            return reinterpret_cast<T*> ( m_Data );
        }

        T& operator[](size_t ix) { 
            return data()[ix];
        }
        const T& operator[](size_t ix) const { 
            return data()[ix];
        }
        
        template <typename... Ts> void emplace_back(Ts&&... vs) {
        	if (m_Size == m_Capacity)
                reserve(m_Capacity ? m_Capacity * 2 : 1); // might throw, no cleanup
            new (data() + m_Size) T ( std::forward<Ts>(vs)... ); // might throw
            ++m_Size;
        }

        void push_back(const T& elem) {
            if (m_Size == m_Capacity)
                reserve(m_Capacity ? m_Capacity * 2 : 1); // might throw, no cleanup
            new (data() + m_Size) T ( elem ); // might throw
            ++m_Size;
        }
        void push_back(T && elem) {
        	emplace_back(std::forward<T>(elem));
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
            	std::uninitialized_move_n ( data(), m_Size, reinterpret_cast<T*>(newBuffer) ); // moves or copies (can throw)
	        }
	        catch (...) {
	            delete [] newBuffer;
	            throw;
	        }
            std::destroy ( data(), data() + m_Size ); // won't throw
	        if ( !isShort () )
	            delete [] m_Data; // won't throw
            
            m_Data = newBuffer;
            m_Capacity = newCapacity;
        }

        size_t capacity() const { 
            return m_Capacity;
        }
        size_t size() const {
            return m_Size;
        }

        void swap (vector & other) {
        	// Both are long, no need to copy or move (just swap)
            if ( !isShort() && !other.isShort() ) {
                std::swap (m_Data, other.m_Data);
            }
            else if ( isShort() && !other.isShort() ) {
                std::uninitialized_move_n ( data(), m_Size, reinterpret_cast<T*>(other.m_Buffer) ); // might throw, no cleanup
                std::destroy ( data(), data() + m_Size );
                m_Data = other.m_Data; // no cleanup, just switch
                other.m_Data = other.m_Buffer;
            }
            else if ( !isShort() && other.isShort() ) {
                std::uninitialized_move_n ( other.data(), other.m_Size, reinterpret_cast<T*>(m_Buffer) ); // might throw, no cleanup
                std::destroy ( other.data(), other.data() + other.m_Size );
                other.m_Data = m_Data; // no cleanup, just switch
                m_Data = m_Buffer;
            }
            // Both are short, swap buffer contents
            else {
            	unsigned char tmpBuffer [N * sizeof(T)];
                std::uninitialized_move_n ( data(), m_Size, reinterpret_cast<T*>(tmpBuffer) ); // might throw, no cleanup
                std::destroy ( data(), data() + m_Size );
                try {
                    std::uninitialized_move_n ( other.data(), other.m_Size, data() );
                }
                catch (...) {
                    std::destroy ( reinterpret_cast<T*>(tmpBuffer), reinterpret_cast<T*>(tmpBuffer) + m_Size );
                    throw;
                }
                std::destroy ( other.data(), other.data() + other.m_Size );
                
                // if this fails, we lose some data (and there's no way out)
                try {
                	std::uninitialized_move_n ( reinterpret_cast<T*>(tmpBuffer), m_Size, other.data() );
            	}
            	catch (...) {
            		std::destroy ( reinterpret_cast<T*>(tmpBuffer), reinterpret_cast<T*>(tmpBuffer) + m_Size );
            		other.m_Size = 0; // we lost the data
            		throw;
            	}
                std::destroy ( reinterpret_cast<T*>(tmpBuffer), reinterpret_cast<T*>(tmpBuffer) + m_Size );
            }
            std::swap (m_Size, other.m_Size);
            std::swap (m_Capacity, other.m_Capacity);
        }
    };
}

#endif
