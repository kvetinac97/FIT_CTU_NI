#define BOOST_TEST_MODULE assignment3_test

#include <boost/test/included/unit_test.hpp>

#include "vector.h"

#include "X.h"

template <typename T>
bool check_included_ptr(T& obj, void* ptr)
{
   auto begin = reinterpret_cast<unsigned char*>(&obj);
   auto end = begin + sizeof(T);

   auto checked = reinterpret_cast<unsigned char*>(ptr);

   return ((checked >= begin) && (checked < end));
}

// check state of empty vector:
BOOST_AUTO_TEST_CASE(empty_vector)
{
   epc::vector<int, 2> v;

   BOOST_TEST( v.data() != nullptr );

   auto included = check_included_ptr(v, v.data());
   BOOST_TEST( included == true );

   BOOST_TEST( v.size() == 0 );
   BOOST_TEST( v.capacity() == 2 );
}

// empty vector must not create objects:
BOOST_AUTO_TEST_CASE(empty_vector_object_creation)
{
   X::reset_counters();

   epc::vector<X, 2> v;

   BOOST_TEST( X::counter(X::CONSTRUCTED) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// long storage test:
BOOST_AUTO_TEST_CASE(long_capacity_allocation)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v;

      v.reserve(3);

      auto included = check_included_ptr(v, v.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v.size() == 0 );
      BOOST_TEST( v.capacity() == 3 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// short vector insertion:
BOOST_AUTO_TEST_CASE(short_insertion)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v;

      v.push_back(1);
      v.push_back(2);

      auto included = check_included_ptr(v, v.data());
      BOOST_TEST( included == true );

      BOOST_TEST( v.size() == 2 );
      BOOST_TEST( v.capacity() == 2 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// long vector insertion with preallocation:
BOOST_AUTO_TEST_CASE(long_insertion_preallocation)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v;

      v.reserve(3);

      v.push_back(1);
      v.push_back(2);
      v.push_back(3);

      auto included = check_included_ptr(v, v.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v.size() == 3 );
      BOOST_TEST( v.capacity() == 3 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// long vector insertion without preallocation:
BOOST_AUTO_TEST_CASE(long_insertion)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v;

      v.push_back(1);
      v.push_back(2);
      v.push_back(3);

      auto included = check_included_ptr(v, v.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v.size() == 3 );
      BOOST_TEST( v.capacity() == 4 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// check values of inserted elements:
BOOST_AUTO_TEST_CASE(element_values)
{
   epc::vector<int, 2> v;

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
      epc::vector<X, 2> v;

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

// short pop_back test:
BOOST_AUTO_TEST_CASE(short_pop_back)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v;

      v.push_back(1);
      v.push_back(2);

      auto data = v.data();
      auto destructed = X::counter(X::DESTRUCTED);
      
      v.pop_back();
      
      auto included = check_included_ptr(v, v.data());
      BOOST_TEST( included == true );

      BOOST_TEST( v.data() == data );
      BOOST_TEST( v.capacity() == 2 );
      BOOST_TEST( v.size() == 1 );
      BOOST_TEST( v[0] == 1 );
      BOOST_TEST( X::counter(X::DESTRUCTED) == destructed + 1 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// long pop_back test:
BOOST_AUTO_TEST_CASE(long_pop_back)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v;

      v.push_back(1);
      v.push_back(2);
      v.push_back(3);

      auto data = v.data();
      auto destructed = X::counter(X::DESTRUCTED);
      
      v.pop_back();
      
      auto included = check_included_ptr(v, v.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v.data() == data );
      BOOST_TEST( v.capacity() == 4 );
      BOOST_TEST( v.size() == 2 );
      BOOST_TEST( v[0] == 1 );
      BOOST_TEST( v[1] == 2 );
      BOOST_TEST( X::counter(X::DESTRUCTED) == destructed + 1 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// short clear test:
BOOST_AUTO_TEST_CASE(short_clear)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v;

      v.push_back(1);
      v.push_back(2);

      auto data = v.data();
      auto destructed = X::counter(X::DESTRUCTED);

      v.clear();

      auto included = check_included_ptr(v, v.data());
      BOOST_TEST( included == true );

      BOOST_TEST( v.data() == data );
      BOOST_TEST( v.capacity() == 2 );
      BOOST_TEST( v.size() == 0 );
      BOOST_TEST( X::counter(X::DESTRUCTED) == destructed + 2 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// long clear test:
BOOST_AUTO_TEST_CASE(long_clear)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v;

      v.push_back(1);
      v.push_back(2);
      v.push_back(3);

      auto data = v.data();
      auto destructed = X::counter(X::DESTRUCTED);

      v.clear();

      auto included = check_included_ptr(v, v.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v.data() == data );
      BOOST_TEST( v.capacity() == 4 );
      BOOST_TEST( v.size() == 0 );
      BOOST_TEST( X::counter(X::DESTRUCTED) == destructed + 3 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// swap test short-short:
BOOST_AUTO_TEST_CASE(swap_short_short)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v1;

      v1.push_back(1);

      epc::vector<X, 2> v2;

      v2.push_back(11);
      v2.push_back(12);

      v1.swap(v2);

      auto included = check_included_ptr(v1, v1.data());
      BOOST_TEST_WARN( included == true );

      BOOST_TEST_WARN( v1.capacity() == 2 );
      BOOST_TEST( v1.size() == 2 );
      BOOST_TEST( v1[0] == 11 );
      BOOST_TEST( v1[1] == 12 );

      included = check_included_ptr(v2, v2.data());
      BOOST_TEST_WARN( included == true );

      BOOST_TEST_WARN( v2.capacity() == 2 );
      BOOST_TEST( v2.size() == 1 );
      BOOST_TEST( v2[0] == 1 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// swap test short-long:
BOOST_AUTO_TEST_CASE(swap_short_long)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v1;

      v1.push_back(1);

      epc::vector<X, 2> v2;

      v2.push_back(11);
      v2.push_back(12);
      v2.push_back(13);

      v1.swap(v2);

      auto included = check_included_ptr(v1, v1.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v1.size() == 3 );
      BOOST_TEST( v1[0] == 11 );
      BOOST_TEST( v1[1] == 12 );
      BOOST_TEST( v1[2] == 13 );

      included = check_included_ptr(v2, v2.data());
      BOOST_TEST_WARN( included == true );

      BOOST_TEST_WARN( v2.capacity() == 2 );
      BOOST_TEST( v2.size() == 1 );
      BOOST_TEST( v2[0] == 1 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// swap test long-short:
BOOST_AUTO_TEST_CASE(swap_long_short)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v1;

      v1.push_back(1);

      epc::vector<X, 2> v2;

      v2.push_back(11);
      v2.push_back(12);
      v2.push_back(13);

      v2.swap(v1);

      auto included = check_included_ptr(v1, v1.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v1.size() == 3 );
      BOOST_TEST( v1[0] == 11 );
      BOOST_TEST( v1[1] == 12 );
      BOOST_TEST( v1[2] == 13 );

      included = check_included_ptr(v2, v2.data());
      BOOST_TEST_WARN( included == true );

      BOOST_TEST_WARN( v2.capacity() == 2 );
      BOOST_TEST( v2.size() == 1 );
      BOOST_TEST( v2[0] == 1 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// swap test long-long:
BOOST_AUTO_TEST_CASE(swap_long_long)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v1;

      v1.push_back(1);
      v1.push_back(2);
      v1.push_back(3);

      epc::vector<X, 2> v2;

      v2.push_back(11);
      v2.push_back(12);
      v2.push_back(13);

      v1.swap(v2);

      auto included = check_included_ptr(v1, v1.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v1.size() == 3 );
      BOOST_TEST( v1[0] == 11 );
      BOOST_TEST( v1[1] == 12 );
      BOOST_TEST( v1[2] == 13 );

      included = check_included_ptr(v2, v2.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v2.size() == 3 );
      BOOST_TEST( v2[0] == 1 );
      BOOST_TEST( v2[1] == 2 );
      BOOST_TEST( v2[2] == 3 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// short copy-constructor test:
BOOST_AUTO_TEST_CASE(copy_constructor_short)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v1;

      v1.push_back(1);
      v1.push_back(2);

      auto data = v1.data();
      auto capacity = v1.capacity();

      auto copy_constructed = X::counter(X::COPY_CONSTRUCTED);

      epc::vector<X, 2> v2(v1);

      BOOST_TEST( v1.data() == data );
      BOOST_TEST( v1.capacity() == capacity );
      BOOST_TEST( v1.size() == 2 );
      BOOST_TEST( v1[0] == 1 );
      BOOST_TEST( v1[1] == 2 );

      auto included = check_included_ptr(v2, v2.data());
      BOOST_TEST_WARN( included == true );

      BOOST_TEST( v2.data() != data );
      BOOST_TEST( v2.size() == 2 );

      BOOST_TEST( v2[0] == 1 );
      BOOST_TEST( v2[1] == 2 );

      BOOST_TEST_WARN( X::counter(X::COPY_CONSTRUCTED) == copy_constructed + 2 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// long copy-constructor test:
BOOST_AUTO_TEST_CASE(copy_constructor_long)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v1;

      v1.push_back(1);
      v1.push_back(2);
      v1.push_back(3);

      auto data = v1.data();
      auto capacity = v1.capacity();

      auto copy_constructed = X::counter(X::COPY_CONSTRUCTED);

      epc::vector<X, 2> v2(v1);

      BOOST_TEST( v1.data() == data );
      BOOST_TEST( v1.capacity() == capacity );
      BOOST_TEST( v1.size() == 3 );
      BOOST_TEST( v1[0] == 1 );
      BOOST_TEST( v1[1] == 2 );
      BOOST_TEST( v1[2] == 3 );

      auto included = check_included_ptr(v2, v2.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v2.data() != data );
      BOOST_TEST( v2.size() == 3 );

      BOOST_TEST( v2[0] == 1 );
      BOOST_TEST( v2[1] == 2 );
      BOOST_TEST( v2[2] == 3 );

      BOOST_TEST_WARN( X::counter(X::COPY_CONSTRUCTED) == copy_constructed + 3 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// copy-assignment test short-short:
BOOST_AUTO_TEST_CASE(copy_assignment_short_short)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v1;
      epc::vector<X, 2> v2;

      v1.push_back(1);

      v2.push_back(11);
      v2.push_back(12);

      v1 = v2;

      auto included = check_included_ptr(v1, v1.data());
      BOOST_TEST_WARN( included == true );

      BOOST_TEST( v1.size() == 2 );

      BOOST_TEST( v1[0] == 11 );
      BOOST_TEST( v1[1] == 12 );

      included = check_included_ptr(v2, v2.data());
      BOOST_TEST( included == true );

      BOOST_TEST( v2.size() == 2 );

      BOOST_TEST( v2[0] == 11 );
      BOOST_TEST( v2[1] == 12 );

   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// copy-assignment test short-long:
BOOST_AUTO_TEST_CASE(copy_assignment_short_long)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v1;
      epc::vector<X, 2> v2;

      v1.push_back(1);
      v1.push_back(2);

      v2.push_back(11);
      v2.push_back(12);
      v2.push_back(13);

      v1 = v2;

      auto included = check_included_ptr(v1, v1.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v1.size() == 3 );

      BOOST_TEST( v1[0] == 11 );
      BOOST_TEST( v1[1] == 12 );
      BOOST_TEST( v1[2] == 13 );

      included = check_included_ptr(v2, v2.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v2.size() == 3 );

      BOOST_TEST( v2[0] == 11 );
      BOOST_TEST( v2[1] == 12 );
      BOOST_TEST( v2[2] == 13 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// copy-assignment test long-short:
BOOST_AUTO_TEST_CASE(copy_assignment_long_short)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v1;
      epc::vector<X, 2> v2;

      v1.push_back(1);
      v1.push_back(2);

      v2.push_back(11);
      v2.push_back(12);
      v2.push_back(13);

      v2 = v1;

      BOOST_TEST( v2.size() == 2 );

      BOOST_TEST( v2[0] == 1 );
      BOOST_TEST( v2[1] == 2 );

      auto included = check_included_ptr(v1, v1.data());
      BOOST_TEST( included == true );

      BOOST_TEST( v1.size() == 2 );

      BOOST_TEST( v1[0] == 1 );
      BOOST_TEST( v1[1] == 2 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}

// copy-assignment test long-long:
BOOST_AUTO_TEST_CASE(copy_assignment_long_long)
{
   X::reset_counters();

   {
      epc::vector<X, 2> v1;
      epc::vector<X, 2> v2;

      v1.push_back(1);
      v1.push_back(2);
      v1.push_back(3);

      v2.push_back(11);
      v2.push_back(12);
      v2.push_back(13);

      v1 = v2;

      auto included = check_included_ptr(v1, v1.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v1.size() == 3 );

      BOOST_TEST( v1[0] == 11 );
      BOOST_TEST( v1[1] == 12 );
      BOOST_TEST( v1[2] == 13 );

      included = check_included_ptr(v2, v2.data());
      BOOST_TEST( included == false );

      BOOST_TEST( v2.size() == 3 );

      BOOST_TEST( v2[0] == 11 );
      BOOST_TEST( v2[1] == 12 );
      BOOST_TEST( v2[2] == 13 );
   }

   BOOST_TEST( X::counter(X::CONSTRUCTED) == X::counter(X::DESTRUCTED) );
   BOOST_TEST( X::counter(X::ALIVE) == 0 );
   BOOST_TEST( X::counter(X::CRITICAL) == 0 );
}
