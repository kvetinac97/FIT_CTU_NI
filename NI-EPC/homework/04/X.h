#ifndef X_H
#define X_H

#include <array>
#include <cstdlib>
#include <utility>
#include <string>

class Exception
{
   std::string what_;

public:
   Exception(const std::string& what) : what_(what) { }

   const std::string& what() const
   {
      return what_;
   }
};

class X
{
public:
   // indexes of counters:
   enum counter_index
   {
      CONSTRUCTED,
      DEFAULT_CONSTRUCTED,
      INT_CONSTRUCTED,
      COPY_CONSTRUCTED,
      COPY_ASSIGNED,
      MOVE_CONSTRUCTED,
      MOVE_ASSIGNED,
      DESTRUCTED,
      ALIVE,
      CRITICAL,
      NUMBER_OF_COUNTERS_  // for private use only 
   };

   // indexes of exception counters:
   enum exception_index
   {
      DEFAULT_CONSTRUCTOR,
      INT_CONSTRUCTOR,
      COPY_CONSTRUCTOR,
      COPY_ASSIGNMENT_OP,
      NUMBER_OF_EXCEPTION_COUNTERS_  // for private use only
   };

   X() : i_(-1), this_(this)
   {
      throw_if(DEFAULT_CONSTRUCTOR, "X::X()");

      counters_[CONSTRUCTED]++;
      counters_[DEFAULT_CONSTRUCTED]++;
      counters_[ALIVE]++;
   }

   X(int i) : i_(i), this_(this)
   {
      throw_if(INT_CONSTRUCTOR, "X::X(int)");

      counters_[CONSTRUCTED]++;
      counters_[INT_CONSTRUCTED]++;
      counters_[ALIVE]++;
   }

   X(const X& other) : i_(other.i_), this_(this)
   {
      throw_if(COPY_CONSTRUCTOR, "X::X(const X&)");

      counters_[CONSTRUCTED]++;
      counters_[COPY_CONSTRUCTED]++;
      counters_[ALIVE]++;
   }

   X(X&& other) : i_(std::exchange(other.i_, -1)), this_(this)
   {
      counters_[CONSTRUCTED]++;
      counters_[MOVE_CONSTRUCTED]++;
      counters_[ALIVE]++;
   }

   X& operator=(const X& other)
   {
      throw_if(COPY_ASSIGNMENT_OP, "X::operator=(const X&)");

      counters_[COPY_ASSIGNED]++;

      i_ = other.i_;
      return *this;
   }

   X& operator=(X&& other)
   {
      counters_[MOVE_ASSIGNED]++;

      i_ = std::exchange(other.i_, -1);
      return *this;
   }


   ~X()
   {
      counters_[DESTRUCTED]++;
      counters_[ALIVE]--;

      if (this != this_)
      {
         counters_[CRITICAL]++;
         std::cerr << "critical error: this_ has changed" << std::endl;
      }
   }

   operator int() const
   {
      return i_;
   }

   static void reset_counters()
   {
      counters_.fill(0);
   }

   static unsigned long counter(counter_index i)
   {
      return counters_[i];
   }

   static void reset_exceptions()
   {
      exceptions_.fill(std::make_pair(false, 0UL));
   }

   static void set_exception_counter(exception_index i, unsigned long n)
   {
      if (n > 0) {
         exceptions_[i].first = true;
         exceptions_[i].second = n;
      }
   }

   static void throw_if(exception_index i, const std::string& what)
   {
      auto & [e, n] = exceptions_[i];

      if (e)
      {
         n--;
         if (n == 0)
         {
            e = false;
            throw Exception(what);
         }
      }
   }

private:

   // stats counters:
   static inline std::array<unsigned long, NUMBER_OF_COUNTERS_> counters_ = {};

   // exception counters:
   static inline std::array<std::pair<bool, unsigned long>, NUMBER_OF_EXCEPTION_COUNTERS_> exceptions_ = {};

   int i_;  // stored value
   X* this_;  // saved storage pointer
};

#endif
