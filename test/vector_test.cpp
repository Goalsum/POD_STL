// vector测试用例文件

#include "vector"
#include "string"
#include "algorithm"
#include "cppunit_mini.h"

using namespace podstl;

// TestCase class
class VectorTest : public TestCase
{
    CPPUNIT_TEST_SUITE(VectorTest);
    CPPUNIT_TEST(vec_test_1);
    CPPUNIT_TEST(vec_test_2);
    CPPUNIT_TEST(vec_test_3);
    CPPUNIT_TEST(vec_test_4);
    CPPUNIT_TEST(vec_test_5);
    CPPUNIT_TEST(vec_test_6);
    CPPUNIT_TEST(vec_test_7);
    CPPUNIT_TEST(capacity);
    CPPUNIT_TEST(at);
    CPPUNIT_TEST(pointer);
    CPPUNIT_TEST(auto_ref);
    CPPUNIT_TEST(assign_check);
    CPPUNIT_TEST(iterators);
    CPPUNIT_TEST(element_default_contruct);
    CPPUNIT_TEST(empty_iterator_range);
    CPPUNIT_TEST(string_nested);
    CPPUNIT_TEST_SUITE_END();

protected:
    void vec_test_1();
    void vec_test_2();
    void vec_test_3();
    void vec_test_4();
    void vec_test_5();
    void vec_test_6();
    void vec_test_7();
    void capacity();
    void at();
    void pointer();
    void auto_ref();
    void assign_check();
    void iterators();
    void element_default_contruct();
    void empty_iterator_range();
    void string_nested();
};

CPPUNIT_TEST_SUITE_REGISTRATION(VectorTest);

// tests implementation
void VectorTest::vec_test_1()
{
    vector<int> v1; // Empty vector of integers.

    CPPUNIT_ASSERT( v1.empty() == true );
    CPPUNIT_ASSERT( v1.size() == 0 );

    v1.push_back(42); // Add an integer to the vector.

    CPPUNIT_ASSERT( v1.size() == 1 );

    CPPUNIT_ASSERT( v1[0] == 42 );

    {
        vector<vector<int> > vect(10);
        vector<vector<int> >::iterator it(vect.begin()), end(vect.end());
        for (; it != end; ++it)
        {
            CPPUNIT_ASSERT( (*it).empty() );
            CPPUNIT_ASSERT( (*it).size() == 0 );
            CPPUNIT_ASSERT( (*it).capacity() == 0 );
            CPPUNIT_ASSERT( (*it).begin() == (*it).end() );
        }
    }
}

void VectorTest::vec_test_2()
{
    vector<double> v1; // Empty vector of doubles.
    v1.push_back(32.1);
    v1.push_back(40.5);
    vector<double> v2; // Another empty vector of doubles.
    v2.push_back(3.56);

    CPPUNIT_ASSERT( v1.size() == 2 );
    CPPUNIT_ASSERT( v1[0] == 32.1 );
    CPPUNIT_ASSERT( v1[1] == 40.5 );

    CPPUNIT_ASSERT( v2.size() == 1 );
    CPPUNIT_ASSERT( v2[0] == 3.56 );
    size_t v1Cap = v1.capacity();
    size_t v2Cap = v2.capacity();

    v1.swap(v2); // Swap the vector's contents.

    CPPUNIT_ASSERT( v1.size() == 1 );
    CPPUNIT_ASSERT( v1.capacity() == v2Cap );
    CPPUNIT_ASSERT( v1[0] == 3.56 );

    CPPUNIT_ASSERT( v2.size() == 2 );
    CPPUNIT_ASSERT( v2.capacity() == v1Cap );
    CPPUNIT_ASSERT( v2[0] == 32.1 );
    CPPUNIT_ASSERT( v2[1] == 40.5 );

    v2 = v1; // Assign one vector to another.

    CPPUNIT_ASSERT( v2.size() == 1 );
    CPPUNIT_ASSERT( v2[0] == 3.56 );
}

void VectorTest::vec_test_3()
{
    typedef vector<char> vec_type;

    vec_type v1; // Empty vector of characters.
    v1.push_back('h');
    v1.push_back('i');

    CPPUNIT_ASSERT( v1.size() == 2 );
    CPPUNIT_ASSERT( v1[0] == 'h' );
    CPPUNIT_ASSERT( v1[1] == 'i' );

    vec_type v2(v1.begin(), v1.end());
    v2[1] = 'o'; // Replace second character.

    CPPUNIT_ASSERT( v2.size() == 2 );
    CPPUNIT_ASSERT( v2[0] == 'h' );
    CPPUNIT_ASSERT( v2[1] == 'o' );

    CPPUNIT_ASSERT( (v1 == v2) == false );

    CPPUNIT_ASSERT( (v1 < v2) == true );
}

void VectorTest::vec_test_4()
{
    vector<short> v(4);

    v[0] = 1;
    v[1] = 4;
    v[2] = 9;
    v[3] = 16;

    CPPUNIT_ASSERT( v.front() == 1 );
    CPPUNIT_ASSERT( v.back() == 16 );

    v.push_back(25);
    CPPUNIT_ASSERT( v.back() == 25 );
    CPPUNIT_ASSERT( v.size() == 5 );

    v.pop_back();
    CPPUNIT_ASSERT( v.back() == 16 );
    CPPUNIT_ASSERT( v.size() == 4 );
}

void VectorTest::vec_test_5()
{
    unsigned short array[] = { 1, 4, 9, 16 };
    vector<unsigned short> v(array, array + 4);
    CPPUNIT_ASSERT( v.size() == 4 );
    CPPUNIT_ASSERT( v[0] == 1 );
    CPPUNIT_ASSERT( v[1] == 4 );
    CPPUNIT_ASSERT( v[2] == 9 );
    CPPUNIT_ASSERT( v[3] == 16 );
}

void VectorTest::vec_test_6()
{
    long array [] = { 1, 4, 9, 16, 25, 36 };

    vector<long> v(array, array + 6);
    vector<long>::iterator vit;

    CPPUNIT_ASSERT( v.size() == 6 );
    CPPUNIT_ASSERT( v[0] == 1 );
    CPPUNIT_ASSERT( v[1] == 4 );
    CPPUNIT_ASSERT( v[2] == 9 );
    CPPUNIT_ASSERT( v[3] == 16 );
    CPPUNIT_ASSERT( v[4] == 25 );
    CPPUNIT_ASSERT( v[5] == 36 );

    vit = v.erase( v.begin() ); // Erase first element.
    CPPUNIT_ASSERT( *vit == 4 );

    CPPUNIT_ASSERT( v.size() == 5 );
    CPPUNIT_ASSERT( v[0] == 4 );
    CPPUNIT_ASSERT( v[1] == 9 );
    CPPUNIT_ASSERT( v[2] == 16 );
    CPPUNIT_ASSERT( v[3] == 25 );
    CPPUNIT_ASSERT( v[4] == 36 );

    vit = v.erase(v.end() - 1); // Erase last element.
    CPPUNIT_ASSERT( vit == v.end() );

    CPPUNIT_ASSERT( v.size() == 4 );
    CPPUNIT_ASSERT( v[0] == 4 );
    CPPUNIT_ASSERT( v[1] == 9 );
    CPPUNIT_ASSERT( v[2] == 16 );
    CPPUNIT_ASSERT( v[3] == 25 );

    v.erase(v.begin() + 1, v.end() - 1); // Erase all but first and last.

    CPPUNIT_ASSERT( v.size() == 2 );
    CPPUNIT_ASSERT( v[0] == 4 );
    CPPUNIT_ASSERT( v[1] == 25 );
}

void VectorTest::vec_test_7()
{
    unsigned long array1 [] = { 1, 4, 25 };
    unsigned long array2[] = { 9, 16 };

    vector<unsigned long> v(array1, array1 + 3);
    vector<unsigned long>::iterator vit;
    vit = v.insert(v.begin(), 0); // Insert before first element.
    CPPUNIT_ASSERT( *vit == 0 );

    vit = v.insert(v.end(), 36);  // Insert after last element.
    CPPUNIT_ASSERT( *vit == 36 );

    CPPUNIT_ASSERT( v.size() == 5 );
    CPPUNIT_ASSERT( v[0] == 0 );
    CPPUNIT_ASSERT( v[1] == 1 );
    CPPUNIT_ASSERT( v[2] == 4 );
    CPPUNIT_ASSERT( v[3] == 25 );
    CPPUNIT_ASSERT( v[4] == 36 );

    // Insert contents of array2 before fourth element.
    v.insert(v.begin() + 3, array2, array2 + 2);

    CPPUNIT_ASSERT( v.size() == 7 );

    CPPUNIT_ASSERT( v[0] == 0 );
    CPPUNIT_ASSERT( v[1] == 1 );
    CPPUNIT_ASSERT( v[2] == 4 );
    CPPUNIT_ASSERT( v[3] == 9 );
    CPPUNIT_ASSERT( v[4] == 16 );
    CPPUNIT_ASSERT( v[5] == 25 );
    CPPUNIT_ASSERT( v[6] == 36 );

    v.clear();
    CPPUNIT_ASSERT( v.empty() );

    v.insert(v.begin(), 5, 10);
    CPPUNIT_ASSERT( v.size() == 5 );
    CPPUNIT_ASSERT( v[0] == 10 );
    CPPUNIT_ASSERT( v[1] == 10 );
    CPPUNIT_ASSERT( v[2] == 10 );
    CPPUNIT_ASSERT( v[3] == 10 );
    CPPUNIT_ASSERT( v[4] == 10 );
}

struct TestStruct
{
    unsigned int a[3];
};

void VectorTest::capacity()
{
    {
        vector<LONG_LONG> v;

        CPPUNIT_ASSERT( v.capacity() == 0 );
        v.push_back(42);
        CPPUNIT_ASSERT( v.capacity() >= 1 );
        v.reserve(5000);
        CPPUNIT_ASSERT( v.capacity() >= 5000 );
    }

    {
        // Test that used to generate an assertion when using __debug_alloc.
        vector<TestStruct> va;
        va.reserve(1);
        va.reserve(2);
    }
}

void VectorTest::at()
{
    vector<unsigned int> v;
    vector<unsigned int> const& cv = v;

    v.push_back(10);
    CPPUNIT_ASSERT( v.at(0) == 10 );
    v.at(0) = 20;
    CPPUNIT_ASSERT( cv.at(0) == 20 );
}

void VectorTest::pointer()
{
    vector<int *> v1;
    vector<int *> v2 = v1;
    vector<int *> v3;

    v3.insert( v3.end(), v1.begin(), v1.end() );
}

void VectorTest::auto_ref()
{
    vector<ULONG_LONG> ref;
    for (int i = 0; i < 5; ++i)
    {
        ref.push_back(i);
    }

    vector<vector<ULONG_LONG> > v_v_int;
    v_v_int.push_back();
    v_v_int[0] = ref;
    v_v_int.push_back();
    v_v_int[1] = ref;
    v_v_int.push_back();
    v_v_int[2] = ref;
    v_v_int.push_back();
    v_v_int[3] = ref;
    v_v_int.push_back();
    v_v_int[4] = ref;

    vector<vector<ULONG_LONG> >::iterator vvit(v_v_int.begin()), vvitEnd(v_v_int.end());
    for (; vvit != vvitEnd; ++vvit)
    {
        CPPUNIT_ASSERT( *vvit == ref );
    }
}

void VectorTest::assign_check()
{
    vector<unsigned char> v(3,1);
    unsigned char array[] = { 1, 2, 3, 4, 5 };

    v.assign( array, array + 5 );
    CPPUNIT_CHECK( v[4] == 5 );
    CPPUNIT_CHECK( v[0] == 1 );
    CPPUNIT_CHECK( v[1] == 2 );
}

void VectorTest::iterators()
{
    vector<signed char> vint(10, 0);
    vector<signed char> const& crvint = vint;

    CPPUNIT_ASSERT( vint.begin() == vint.begin() );
    CPPUNIT_ASSERT( crvint.begin() == vint.begin() );
    CPPUNIT_ASSERT( vint.begin() == crvint.begin() );
    CPPUNIT_ASSERT( crvint.begin() == crvint.begin() );

    CPPUNIT_ASSERT( vint.begin() != vint.end() );
    CPPUNIT_ASSERT( crvint.begin() != vint.end() );
    CPPUNIT_ASSERT( vint.begin() != crvint.end() );
    CPPUNIT_ASSERT( crvint.begin() != crvint.end() );

    CPPUNIT_ASSERT( vint.rbegin() == vint.rbegin() );
    CPPUNIT_ASSERT( crvint.rbegin() == crvint.rbegin() );
    CPPUNIT_ASSERT( vint.rbegin() != vint.rend() );
    CPPUNIT_ASSERT( crvint.rbegin() != crvint.rend() );
}

const int DEFAULT_VALUE = 0x12345678;
struct TestElementDefConstruct
{
    TestElementDefConstruct() { x = DEFAULT_VALUE; }
    int x;
};

void VectorTest::element_default_contruct()
{
    vector<TestElementDefConstruct> v;
    v.push_back();
    CPPUNIT_ASSERT(v[0].x == DEFAULT_VALUE);
    v.insert(v.begin());
    CPPUNIT_ASSERT(v[0].x == DEFAULT_VALUE);
    CPPUNIT_ASSERT(v[1].x == DEFAULT_VALUE);

    v[0].x = 500;
    vector<TestElementDefConstruct> v2(v);
    CPPUNIT_ASSERT(v2[0].x == 500);
    CPPUNIT_ASSERT(v2[1].x == DEFAULT_VALUE);

    vector<TestElementDefConstruct> v3(3);
    CPPUNIT_ASSERT(v3[0].x == DEFAULT_VALUE);
    CPPUNIT_ASSERT(v3[1].x == DEFAULT_VALUE);
    CPPUNIT_ASSERT(v3[2].x == DEFAULT_VALUE);

    v3.resize(5);
    CPPUNIT_ASSERT(v3[3].x == DEFAULT_VALUE);
    CPPUNIT_ASSERT(v3[4].x == DEFAULT_VALUE);

    TestElementDefConstruct element;
    element.x = 0x30808080;

    v3.resize(7, element);
    CPPUNIT_ASSERT(v3[5].x == 0x30808080);
    CPPUNIT_ASSERT(v3[6].x == 0x30808080);
}

void VectorTest::empty_iterator_range()
{
    char a[3];
    vector<char> v1(a, a);
    CPPUNIT_ASSERT(v1.empty());
    vector<long> v2(0, 5);
    CPPUNIT_ASSERT(v2.empty());
    v1.assign(a, a);
    CPPUNIT_ASSERT(v1.empty());
    v2.assign(0, 5);
    CPPUNIT_ASSERT(v2.empty());
    v1.insert(v1.begin(), a, a);
    CPPUNIT_ASSERT(v1.empty());
    v2.insert(v2.begin(), 0, 5);
    CPPUNIT_ASSERT(v2.empty());
    v1.erase(v1.begin(), v1.begin());
    CPPUNIT_ASSERT(v1.empty());
}

void VectorTest::string_nested()
{
    vector<string> v;
    v.push_back(string());
    v.front() = "Memory must be allocated to put me into the string";
    CPPUNIT_ASSERT(strcmp(v.begin()->c_str(), "Memory must be allocated to put me into the string") == 0);

    vector<string> v3;
    v3.push_back();
    v3[0] = "zzz";
    v3.push_back();
    v3[1] = "cba";
    v3.push_back();
    v3[2] = "abc";
    v3.erase(v3.begin());   // 需要保证erase能够调用内部元素的析构函数，否则会内存泄露。
    CPPUNIT_ASSERT(v3[0] == "cba");
    CPPUNIT_ASSERT(v3[1] == "abc");
    v3.pop_back();  // 需要保证pop_back能够调用内部元素的析构函数，否则会内存泄露。
    v3.erase(v3.begin(), v3.end());
    CPPUNIT_ASSERT(v3.empty());

    vector<string> v2(2);
    v2.front() = "Put me into front!";
    v2.back() = "Put me into back!";
    v2.clear(); // 需要保证clear能够调用内部元素的析构函数，否则会内存泄露。
}

 // Simple compilation test: Check that nested types like iterator
 // can be access even if type used to instanciate container is not
 // yet completely defined.
class IncompleteClass
{
    vector<IncompleteClass> instances;
    typedef vector<IncompleteClass>::iterator it;
};
