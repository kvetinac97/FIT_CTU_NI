#define BOOST_TEST_MODULE assignment2_test

#include <exception>

#include <boost/test/included/unit_test.hpp>

#include "vector.h"

#include "X.h"

// check state of empty vector:
BOOST_AUTO_TEST_CASE(empty_vector)
{
   epc::vector<int> v;

   BOOST_TEST( v.data() == nullptr );
   BOOST_TEST( v.size() == 0 );
   BOOST_TEST( v.capacity() == 0 );
}

// empty vector must not create objects:
BOOST_AUTO_TEST_CASE(empty_vector_object_creation)
{
   X::reset_counters();

   epc::vector<X> v;

   BOOST_TEST( X::counter(X::CONSTRUCTED) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// capacity allocation test:
BOOST_AUTO_TEST_CASE(capacity_allocation)
{
   X::reset_counters();

   {
      epc::vector<X> v;

      v.reserve(0);

      BOOST_TEST( v.data() == nullptr );
      BOOST_TEST( v.size() == 0 );
      BOOST_TEST( v.capacity() == 0 );

      v.reserve(1);
      
      BOOST_TEST( v.data() != nullptr );
      BOOST_TEST( v.size() == 0 );
      BOOST_TEST( v.capacity() == 1 );

      BOOST_TEST( X::counter(X::DEFAULT_CONSTRUCTED) == 0 );
      BOOST_TEST( X::counter(X::CONSTRUCTED) == 0 );

      auto data = v.data();

      v.reserve(7);

      BOOST_TEST( v.data() != nullptr );
      BOOST_TEST( v.data() != data );
      BOOST_TEST( v.size() == 0 );
      BOOST_TEST( v.capacity() == 7 );

      BOOST_TEST( X::counter(X::DEFAULT_CONSTRUCTED) == 0 );
      BOOST_TEST( X::counter(X::CONSTRUCTED) == 0 );

      data = v.data();

      v.reserve(2); // should have no effect

      BOOST_TEST( v.data() != nullptr );
      BOOST_TEST( v.data() == data );
      BOOST_TEST( v.size() == 0 );
      BOOST_TEST( v.capacity() == 7 );

      BOOST_TEST( X::counter(X::DEFAULT_CONSTRUCTED) == 0 );
      BOOST_TEST( X::counter(X::CONSTRUCTED) == 0 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// element insertion with preallocation:
BOOST_AUTO_TEST_CASE(insertion_with_preallocation)
{
   X::reset_counters();

   {
      epc::vector<X> v;
       
      v.reserve(3);

      auto data = v.data();

      v.push_back(1);
      v.push_back(2);
      v.push_back(3);

      BOOST_TEST( v.data() == data );
      BOOST_TEST( v.size() == 3 );
      BOOST_TEST( v.capacity() == 3 );

      BOOST_TEST_WARN( X::counter(X::INT_CONSTRUCTED) == 3 );
      BOOST_TEST_WARN( X::counter(X::COPY_CONSTRUCTED) == 3 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// reallocation test:
BOOST_AUTO_TEST_CASE(reallocation)
{
   X::reset_counters();

   {
      epc::vector<X> v;

      v.reserve(3);

      v.push_back(1);
      v.push_back(2);
      v.push_back(3);

      auto data = v.data();

      v.reserve(7);

      BOOST_TEST( v.data() != data );
      BOOST_TEST( v.size() == 3 );
      BOOST_TEST( v.capacity() == 7 );

      BOOST_TEST_WARN( X::counter(X::COPY_CONSTRUCTED) == 6 );

      data = v.data();

      v.push_back(4);
      v.push_back(5);
      v.push_back(6);
      v.push_back(7);

      BOOST_TEST( v.data() == data );
      BOOST_TEST( v.size() == 7 );
      BOOST_TEST( v.capacity() == 7 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// check growing capacity when adding elemnents:
BOOST_AUTO_TEST_CASE(growing_capcacity)
{
   epc::vector<int> v;

   v.push_back(1);

   BOOST_TEST( v.size() == 1 );
   BOOST_TEST( v.capacity() == 1 );

   v.push_back(2);

   BOOST_TEST( v.size() == 2 );
   BOOST_TEST( v.capacity() == 2 );

   v.push_back(3);

   BOOST_TEST( v.size() == 3 );
   BOOST_TEST( v.capacity() == 4 );

   v.push_back(4);
   
   BOOST_TEST( v.size() == 4 );
   BOOST_TEST( v.capacity() == 4 );
   
   v.push_back(5);

   BOOST_TEST( v.size() == 5 );
   BOOST_TEST( v.capacity() == 8 );
}

// check values of inserted elements:
BOOST_AUTO_TEST_CASE(element_values)
{
   epc::vector<int> v;

   v.push_back(1);

   BOOST_TEST( v[0] == 1 );
   BOOST_TEST( *v.data() == 1 );

   v.reserve(3);

   BOOST_TEST( v[0] == 1 );
   BOOST_TEST( *v.data() == 1 );

   v.push_back(2);

   BOOST_TEST( v[0] == 1 );
   BOOST_TEST( v[1] == 2 );
   BOOST_TEST( *v.data() == 1 );
   BOOST_TEST( *(v.data() + 1) == 2 );

   v.push_back(3);

   BOOST_TEST( v[0] == 1 );
   BOOST_TEST( v[1] == 2 );
   BOOST_TEST( v[2] == 3 );
   BOOST_TEST( *v.data() == 1 );
   BOOST_TEST( *(v.data() + 1) == 2 );
   BOOST_TEST( *(v.data() + 2) == 3 );

   v.push_back(4);

   BOOST_TEST( v[0] == 1 );
   BOOST_TEST( v[1] == 2 );
   BOOST_TEST( v[2] == 3 );
   BOOST_TEST( v[3] == 4 );
   BOOST_TEST( *v.data() == 1 );
   BOOST_TEST( *(v.data() + 1) == 2 );
   BOOST_TEST( *(v.data() + 2) == 3 );
   BOOST_TEST( *(v.data() + 3) == 4 );
}

// test special member operations used for insertion/reallocation:
BOOST_AUTO_TEST_CASE(special_members_insertion)
{
   X::reset_counters();

   {
      epc::vector<X> v;

      v.push_back(1);
      v.reserve(3);
      v.push_back(2);
      v.push_back(3);
      v.push_back(4);

      BOOST_TEST_WARN( X::counter(X::DEFAULT_CONSTRUCTED) == 0 );
      BOOST_TEST_WARN( X::counter(X::INT_CONSTRUCTED) == 4 );
      BOOST_TEST_WARN( X::counter(X::COPY_CONSTRUCTED) == 8 );
      BOOST_TEST_WARN( X::counter(X::COPY_ASSIGNED) == 0 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// pop_back test:
BOOST_AUTO_TEST_CASE(pop_back)
{
   X::reset_counters();

   {
      epc::vector<X> v;

      v.push_back(1);
      v.push_back(2);
      v.push_back(3);

      auto data = v.data();
      auto destructed = X::counter(X::DESTRUCTED);
      
      v.pop_back();

      BOOST_TEST( v.data() == data );
      BOOST_TEST( v.capacity() == 4 );
      BOOST_TEST( v.size() == 2 );
      BOOST_TEST( v[0] == 1 );
      BOOST_TEST( v[1] == 2 );
      BOOST_TEST( X::counter(X::DESTRUCTED) == destructed + 1 );

      v.push_back(3);

      destructed = X::counter(X::DESTRUCTED);

      v.pop_back();
      v.pop_back();
      v.pop_back();

      BOOST_TEST( v.data() == data );
      BOOST_TEST( v.capacity() == 4 );
      BOOST_TEST( v.size() == 0 );
      BOOST_TEST( X::counter(X::DESTRUCTED) == destructed + 3);
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// clear test:
BOOST_AUTO_TEST_CASE(clear)
{
   X::reset_counters();

   {
      epc::vector<X> v;

      v.push_back(1);
      v.push_back(2);
      v.push_back(3);

      auto data = v.data();
      auto destructed = X::counter(X::DESTRUCTED);

      v.clear();

      BOOST_TEST( v.data() == data );
      BOOST_TEST( v.capacity() == 4 );
      BOOST_TEST( v.size() == 0 );
      BOOST_TEST( X::counter(X::DESTRUCTED) == destructed + 3 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// swap test:
BOOST_AUTO_TEST_CASE(swap)
{
   // two empty vectors: 

   X::reset_counters();

   {
      epc::vector<X> v1;
      epc::vector<X> v2;

      v1.swap(v2);

      BOOST_TEST( v1.data() == nullptr );
      BOOST_TEST( v1.capacity() == 0 );
      BOOST_TEST( v1.size() == 0 );

      BOOST_TEST( v2.data() == nullptr );
      BOOST_TEST( v2.capacity() == 0 );
      BOOST_TEST( v2.size() == 0 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == 0 );
   BOOST_TEST( X::counter(X::DESTRUCTED) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   // one empty, one non-empty:

   X::reset_counters();

   {
      epc::vector<X> v1;
      epc::vector<X> v2;

      v1.push_back(1);
      v1.push_back(2);
      v1.push_back(3);

      auto data = v1.data();
      auto capacity = v1.capacity();

      v1.swap(v2);

      BOOST_TEST( v1.data() == nullptr );
      BOOST_TEST_WARN( v1.capacity() == 0 );
      BOOST_TEST( v1.size() == 0 );

      BOOST_TEST_WARN( v2.data() == data );
      BOOST_TEST_WARN( v2.capacity() == capacity );
      BOOST_TEST( v2.size() == 3 );

      BOOST_TEST( v2[0] == 1 );
      BOOST_TEST( v2[1] == 2 );
      BOOST_TEST( v2[2] == 3 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   // two non-empty vectors:

   X::reset_counters();

   {
      epc::vector<X> v1;
      epc::vector<X> v2;

      v1.push_back(1);
      v1.push_back(2);
      v1.push_back(3);

      v2.push_back(11);
      v2.push_back(12);

      auto data1 = v1.data();
      auto capacity1 = v1.capacity();

      auto data2 = v2.data();
      auto capacity2 = v2.capacity();

      v1.swap(v2);

      BOOST_TEST_WARN( v1.data() == data2 );
      BOOST_TEST_WARN( v1.capacity() == capacity2 );
      BOOST_TEST( v1.size() == 2 );

      BOOST_TEST_WARN( v2.data() == data1 );
      BOOST_TEST_WARN( v2.capacity() == capacity1 );
      BOOST_TEST( v2.size() == 3 );

      BOOST_TEST( v2[0] == 1 );
      BOOST_TEST( v2[1] == 2 );
      BOOST_TEST( v2[2] == 3 );

      BOOST_TEST( v1[0] == 11 );
      BOOST_TEST( v1[1] == 12 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// copy-constructor test:
BOOST_AUTO_TEST_CASE(copy_constructor)
{
   // empty source vector:

   X::reset_counters();

   {
      epc::vector<X> v1;
      epc::vector<X> v2(v1);

      BOOST_TEST( v1.data() == nullptr );
      BOOST_TEST( v1.capacity() == 0 );
      BOOST_TEST( v1.size() == 0 );

      BOOST_TEST_WARN( v2.data() == nullptr );
      BOOST_TEST_WARN( v2.capacity() == 0 );
      BOOST_TEST( v2.size() == 0 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == 0 );
   BOOST_TEST( X::counter(X::DESTRUCTED) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   // non-empty source vector:

   X::reset_counters();

   {
      epc::vector<X> v1;

      v1.push_back(1);
      v1.push_back(2);
      v1.push_back(3);

      auto data = v1.data();
      auto capacity = v1.capacity();

      auto copy_constructed = X::counter(X::COPY_CONSTRUCTED);

      epc::vector<X> v2(v1);

      BOOST_TEST( v1.data() == data );
      BOOST_TEST( v1.capacity() == capacity );
      BOOST_TEST( v1.size() == 3 );

      BOOST_TEST( v2.data() != data );
      BOOST_TEST( v2.capacity() >= 3 );
      BOOST_TEST( v2.size() == 3 );

      BOOST_TEST( v1[0] == 1 );
      BOOST_TEST( v1[1] == 2 );
      BOOST_TEST( v1[2] == 3 );

      BOOST_TEST( v2[0] == 1 );
      BOOST_TEST( v2[1] == 2 );
      BOOST_TEST( v2[2] == 3 );

      BOOST_TEST_WARN( X::counter(X::COPY_CONSTRUCTED) == copy_constructed + 3 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// copy-assignment test:
BOOST_AUTO_TEST_CASE(copy_assignment)
{
   // empty source, empty destination:

   X::reset_counters();

   {
      epc::vector<X> v1;
      epc::vector<X> v2;

      v1 = v2;

      BOOST_TEST_WARN( v1.data() == nullptr );
      BOOST_TEST_WARN( v1.capacity() == 0 );
      BOOST_TEST( v1.size() == 0 );

      BOOST_TEST( v2.data() == nullptr );
      BOOST_TEST( v2.capacity() == 0 );
      BOOST_TEST( v2.size() == 0 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == 0 );
   BOOST_TEST( X::counter(X::DESTRUCTED) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   // empty source, non-empty destination:

   X::reset_counters();

   {
      epc::vector<X> v1;
      epc::vector<X> v2;

      v1.push_back(1);
      v1.push_back(2);
      v1.push_back(3);

      v1 = v2;

      BOOST_TEST( v2.data() == nullptr );
      BOOST_TEST( v2.capacity() == 0 );
      BOOST_TEST( v2.size() == 0 );

      BOOST_TEST( v1.size() == 0 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   // non-empty source, empty destination:

   X::reset_counters();

   {
      epc::vector<X> v1;
      epc::vector<X> v2;

      v2.push_back(1);
      v2.push_back(2);
      v2.push_back(3);

      v1 = v2;

      BOOST_TEST( v2.data() != nullptr );
      BOOST_TEST( v2.capacity() >= 3 );
      BOOST_TEST( v2.size() == 3 );

      BOOST_TEST( v2[0] == 1 );
      BOOST_TEST( v2[1] == 2 );
      BOOST_TEST( v2[2] == 3 );

      BOOST_TEST( v1.data() != nullptr );
      BOOST_TEST( v1.data() != v2.data() );
      BOOST_TEST( v1.capacity() >= 3 );
      BOOST_TEST( v1.size() == 3 );

      BOOST_TEST( v1[0] == 1 );
      BOOST_TEST( v1[1] == 2 );
      BOOST_TEST( v1[2] == 3 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   // non-empty source, non-empty destination:

   X::reset_counters();

   {
      epc::vector<X> v1;
      epc::vector<X> v2;

      v1.push_back(11);
      v1.push_back(12);
      v1.push_back(13);

      v2.push_back(1);
      v2.push_back(2);

      auto data = v1.data();

      v1 = v2;

      BOOST_TEST( v2.data() != nullptr );
      BOOST_TEST( v2.capacity() >= 2 );
      BOOST_TEST( v2.size() == 2 );

      BOOST_TEST( v2[0] == 1 );
      BOOST_TEST( v2[1] == 2 );

      BOOST_TEST( v1.data() != nullptr );
      BOOST_TEST( v1.data() != v2.data() );
      BOOST_TEST( v1.capacity() >= 2 );
      BOOST_TEST( v1.size() == 2 );

      BOOST_TEST( v1[0] == 1 );
      BOOST_TEST( v1[1] == 2 );

      BOOST_TEST_MESSAGE( "Storage of longer destination vector has changed: " << (v1.data() != data) );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// copy self-assignment test:
BOOST_AUTO_TEST_CASE(copy_self_assignment)
{
   // empty vector:

   X::reset_counters();

   {
      epc::vector<X> v;

      v = v;

      BOOST_TEST_WARN( v.data() == nullptr );
      BOOST_TEST_WARN( v.capacity() == 0 );
      BOOST_TEST( v.size() == 0 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   // non-empty vector:

   X::reset_counters();

   {
      epc::vector<X> v;

      v.push_back(1);
      v.push_back(2);
      v.push_back(3);

      auto data = v.data();

      v = v;

      BOOST_TEST_WARN( v.data() != nullptr );
      BOOST_TEST_WARN( v.capacity() >= 3 );
      BOOST_TEST( v.size() == 3 );

      BOOST_TEST( v[0] == 1 );
      BOOST_TEST( v[1] == 2 );
      BOOST_TEST( v[2] == 3 );

      BOOST_TEST_MESSAGE( "Storage of self-assigned vector has changed: " << (v.data() != data) );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// swap no-except test: 
BOOST_AUTO_TEST_CASE(swap_exception)
{
   X::reset_counters();
   X::reset_exceptions();

   {
      epc::vector<X> v1;
      epc::vector<X> v2;

      v1.push_back(1);
      v1.push_back(2);
      v1.push_back(3);

      v2.push_back(11);
      v2.push_back(12);

      X::set_exception_counter(X::DEFAULT_CONSTRUCTOR, 1);
      X::set_exception_counter(X::INT_CONSTRUCTOR, 1);
      X::set_exception_counter(X::COPY_CONSTRUCTOR, 1);
      X::set_exception_counter(X::COPY_ASSIGNMENT_OP, 1);

      try
      {
         v1.swap(v2);
      }
      catch (const Exception& e)
      {
         BOOST_TEST( false );
         BOOST_TEST_MESSAGE( "swap() throws Exception: " << e.what() );
      }
      catch (const std::exception& e)
      {
         BOOST_TEST( false );
         BOOST_TEST_MESSAGE( "swap() throws std::exception: " << e.what() );
      }
      catch (...)
      {
         BOOST_TEST( false );
         BOOST_TEST_MESSAGE( "swap() throws unspecified exception" );
      }
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   X::reset_exceptions();
}

// reserve exception test: 
BOOST_AUTO_TEST_CASE(reserve_exception)
{
   X::reset_counters();
   X::reset_exceptions();

   {
      epc::vector<X> v;

      v.push_back(1);
      v.push_back(2);

      auto data = v.data();

      X::set_exception_counter(X::COPY_CONSTRUCTOR, 2);

      try 
      {
         v.reserve(5);
      }
      catch (...)
      {
         BOOST_TEST_WARN( v.data() == data );
         BOOST_TEST_WARN( v.capacity() == 2 );
         BOOST_TEST( v.size() == 2 );
      }
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   X::reset_exceptions();
}

// empty push_back exception test: 
BOOST_AUTO_TEST_CASE(empty_push_back_exception)
{
   X::reset_counters();
   X::reset_exceptions();

   {
      epc::vector<X> v;

      X::set_exception_counter(X::COPY_CONSTRUCTOR, 1);

      try 
      {
         v.push_back(1);
      }
      catch (...)
      {
         BOOST_TEST( v.size() == 0 );
      }

      BOOST_TEST_WARN( v.size() == 0 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   X::reset_exceptions();
}

// reallocation exception test:
BOOST_AUTO_TEST_CASE(reallocation_exception)
{
   X::reset_counters();
   X::reset_exceptions();

   {
      epc::vector<X> v;
      
      v.reserve(3);

      v.push_back(1);
      v.push_back(2);
      v.push_back(3);

      auto data = v.data();

      X::set_exception_counter(X::COPY_CONSTRUCTOR, 2);

      try 
      {
         v.push_back(4);
      }
      catch (...)
      {
         BOOST_TEST_WARN( v.size() == 3 );
      }

      BOOST_TEST_WARN( v.size() == 3 );
      BOOST_TEST_WARN( v.data() == data );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   X::reset_exceptions();
}

// copy constructor exception test:
BOOST_AUTO_TEST_CASE(copy_constructor_exception)
{
   X::reset_counters();
   X::reset_exceptions();

   {
      epc::vector<X> v1;
      
      v1.push_back(1);
      v1.push_back(2);
      v1.push_back(3);

      X::set_exception_counter(X::COPY_CONSTRUCTOR, 2);

      try 
      {
         epc::vector<X> v2(v1);
      }
      catch (...)
      {
      }
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   X::reset_exceptions();
}

// copy assignment exception test:
BOOST_AUTO_TEST_CASE(copy_assignment_exception)
{
   X::reset_counters();
   X::reset_exceptions();

   {
      epc::vector<X> v1;
      epc::vector<X> v2;
      
      v1.push_back(1);
      v1.push_back(2);
      v1.push_back(3);

      v2.push_back(11);
      v2.push_back(12);

      X::set_exception_counter(X::COPY_ASSIGNMENT_OP, 2);
      X::set_exception_counter(X::COPY_CONSTRUCTOR, 2);

      try 
      {
         v2 = v1;
      }
      catch (...)
      {
      }
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );

   X::reset_exceptions();
}
