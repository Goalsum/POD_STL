// 链表测试用例文件

#include "list"
#include "string"
#include "algorithm"
#include "functional"
#include "cppunit_mini.h"

using namespace podstl;

// TestCase class
class ListTest : public TestCase
{
    CPPUNIT_TEST_SUITE(ListTest);
    CPPUNIT_TEST(list1);
    CPPUNIT_TEST(list2);
    CPPUNIT_TEST(list3);
    CPPUNIT_TEST(list4);
    CPPUNIT_TEST(erase);
    CPPUNIT_TEST(resize);
    CPPUNIT_TEST(push_back);
    CPPUNIT_TEST(push_front);
    CPPUNIT_TEST(swap);
    CPPUNIT_TEST(adl);
    CPPUNIT_TEST(element_default_contruct);
    CPPUNIT_TEST(string_nested);
    CPPUNIT_TEST_SUITE_END();

protected:
    void list1();
    void list2();
    void list3();
    void list4();
    void erase();
    void resize();
    void push_back();
    void push_front();
    void swap();
    void adl();
    void element_default_contruct();
    void string_nested();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ListTest);

// tests implementation
void ListTest::list1()
{
    int array1 [] = { 9, 16, 36 };
    int array2 [] = { 1, 4 };

    list<int> l1(array1, array1 + 3);
    list<int> l2(array2, array2 + 2);
    list<int>::iterator i1 = l1.begin();
    list<int>::iterator i2 = l2.begin();
    list<int>::const_iterator ci(i1);
    list<int>::const_iterator ci1(ci);
    l1.splice(i1, l2);
    i1 = l1.begin();
    CPPUNIT_ASSERT( *i1++ == 1 );
    CPPUNIT_ASSERT( *i1++ == 4 );
    CPPUNIT_ASSERT( *i1++ == 9 );
    CPPUNIT_ASSERT( *i1++ == 16 );
    CPPUNIT_ASSERT( *i1++ == 36 );
    CPPUNIT_ASSERT( i2 == l1.begin() );

    // Default construct check
    list<int> l(2);
    i1 = l.begin();
    CPPUNIT_ASSERT( *(i1++) == 0 );
    CPPUNIT_ASSERT( *i1 == 0 );
}

void ListTest::list2()
{
    unsigned char array1 [] = { 1, 16 };
    unsigned char array2[] = { 4, 9 };

    list<unsigned char> l1(array1, array1 + 2);
    list<unsigned char> l2(array2, array2 + 2);
    list<unsigned char>::iterator i = l1.begin();
    i++;
    l1.splice(i, l2, l2.begin(), l2.end());
    i = l1.begin();
    CPPUNIT_ASSERT(*i++==1);
    CPPUNIT_ASSERT(*i++==4);
    CPPUNIT_ASSERT(*i++==9);
    CPPUNIT_ASSERT(*i++==16);
}

void ListTest::list3()
{
    char array [] = { 'x', 'l', 'x', 't', 's', 's' };

    list<char> str(array, array + 6);
    list<char>::iterator i;

    str.reverse();
    i = str.begin();
    CPPUNIT_ASSERT(*i++=='s');
    CPPUNIT_ASSERT(*i++=='s');
    CPPUNIT_ASSERT(*i++=='t');
    CPPUNIT_ASSERT(*i++=='x');
    CPPUNIT_ASSERT(*i++=='l');
    CPPUNIT_ASSERT(*i++=='x');

    str.remove('x');
    i = str.begin();
    CPPUNIT_ASSERT(*i++=='s');
    CPPUNIT_ASSERT(*i++=='s');
    CPPUNIT_ASSERT(*i++=='t');
    CPPUNIT_ASSERT(*i++=='l');

    str.unique();
    i = str.begin();
    CPPUNIT_ASSERT(*i++=='s');
    CPPUNIT_ASSERT(*i++=='t');
    CPPUNIT_ASSERT(*i++=='l');

    str.sort();
    i = str.begin();
    CPPUNIT_ASSERT(*i++=='l');
    CPPUNIT_ASSERT(*i++=='s');
    CPPUNIT_ASSERT(*i++=='t');
}

void ListTest::list4()
{
    short array1 [] = { 1, 3, 6, 7 };
    short array2[] = { 2, 4 };

    list<short> l1(array1, array1 + 4);
    list<short> l2(array2, array2 + 2);
    l1.merge(l2);
    list<short>::iterator i = l1.begin();
    CPPUNIT_ASSERT(*i++==1);
    CPPUNIT_ASSERT(*i++==2);
    CPPUNIT_ASSERT(*i++==3);
    CPPUNIT_ASSERT(*i++==4);
    CPPUNIT_ASSERT(*i++==6);
    CPPUNIT_ASSERT(*i++==7);

    //We use distance to avoid a simple call to an internal counter
    CPPUNIT_ASSERT(distance(l1.begin(), l1.end()) == 6);
    CPPUNIT_ASSERT(distance(l2.begin(), l2.end()) == 0);

    l1.swap(l2);

    CPPUNIT_ASSERT(distance(l1.begin(), l1.end()) == 0);
    CPPUNIT_ASSERT(distance(l2.begin(), l2.end()) == 6);
}

void ListTest::erase()
{
    list<long> l;
    l.push_back( 1 );
    l.erase(l.begin());
    CPPUNIT_ASSERT( l.empty() );

    l.assign(4, 5);
    CPPUNIT_ASSERT(l.size() == 4);

    long array[] = { 0, 1, 2, 3 };
    l.assign(array, array + 4);
    CPPUNIT_ASSERT(l.size() == 4);
    list<long>::iterator lit;
    lit = l.erase(l.begin());
    CPPUNIT_ASSERT(l.size() == 3);
    CPPUNIT_ASSERT( *lit == 1 );

    lit = l.erase(l.begin(), --l.end());
    CPPUNIT_ASSERT( *lit == 3 );

    l.clear();
    CPPUNIT_ASSERT( l.empty() );
}

void ListTest::resize()
{
    {
        list<LONG_LONG> l;
        l.resize(5, 1);

        size_t i;
        list<LONG_LONG>::iterator lit(l.begin());
        for (i = 0; i < 5; ++i) 
        {
            CPPUNIT_ASSERT( lit != l.end() );
            CPPUNIT_ASSERT( *(lit++) == 1 );
        }
        CPPUNIT_ASSERT( lit == l.end() );

        l.resize(3);
        lit = l.begin();
        for (i = 0; i < 3; ++i) 
        {
            CPPUNIT_ASSERT( lit != l.end() );
            CPPUNIT_ASSERT( *(lit++) == 1 );
        }
        CPPUNIT_ASSERT( lit == l.end() );
    }

    {
        list<LONG_LONG> l;
        l.resize(5);

        size_t i;
        list<LONG_LONG>::iterator lit(l.begin());
        for (i = 0; i < 5; ++i) 
        {
            CPPUNIT_ASSERT( lit != l.end() );
            CPPUNIT_ASSERT( *(lit++) == 0 );
        }
        CPPUNIT_ASSERT( lit == l.end() );
    }
}

void ListTest::push_back()
{
    list<ULONG_LONG> l;
    l.push_back( 1 );
    l.push_back( 2 );
    l.push_back( 3 );

    list<ULONG_LONG>::reverse_iterator r = l.rbegin();

    CPPUNIT_ASSERT( *r == 3 );
    l.push_back( 4 );
}

void ListTest::push_front()
{
    list<unsigned int> l;
    l.push_back( 1 );
    l.push_back( 2 );
    l.push_back( 3 );

    list<unsigned int>::iterator i = l.begin();

    CPPUNIT_ASSERT( *i == 1 );
    l.push_front( 0 );
    CPPUNIT_ASSERT( *i == 1 );
    ++i;
    CPPUNIT_ASSERT( *i == 2 );
}

void ListTest::swap()
{
    list<unsigned short> lst1;
    list<unsigned short> lst2;

    lst1.push_back(1);
    lst2.push_back(2);

    lst1.swap( lst2 );

    CPPUNIT_CHECK( lst1.front() == 2 );
    CPPUNIT_CHECK( lst2.front() == 1 );
    CPPUNIT_CHECK( lst1.size() == 1 );
    CPPUNIT_CHECK( lst2.size() == 1 );

    lst1.pop_front();
    lst2.pop_front();

    CPPUNIT_CHECK( lst1.empty() );
    CPPUNIT_CHECK( lst2.empty() );
}

namespace foo 
{
    class bar {};

    template <class _It>
    size_t distance(_It, _It);
}

void ListTest::adl()
{
    list<foo::bar> lbar;
    CPPUNIT_ASSERT( lbar.size() == 0);
}

const int DEFAULT_VALUE = 0x12345678;
struct TestElementDefConstruct
{
    TestElementDefConstruct() { x = DEFAULT_VALUE; }
    int x;
};

void ListTest::element_default_contruct()
{
    list<TestElementDefConstruct> v;
    v.push_back(TestElementDefConstruct());
    CPPUNIT_ASSERT(v.front().x == DEFAULT_VALUE);
    v.insert(v.begin(), TestElementDefConstruct());
    list<TestElementDefConstruct>::iterator it = v.begin();
    CPPUNIT_ASSERT((*it++).x == DEFAULT_VALUE);
    CPPUNIT_ASSERT(it->x == DEFAULT_VALUE);

    v.front().x = 500;
    list<TestElementDefConstruct> v2(v);
    CPPUNIT_ASSERT(v2.front().x == 500);
    CPPUNIT_ASSERT((++v2.begin())->x == DEFAULT_VALUE);

    list<TestElementDefConstruct> v3(3);
    it = v3.begin();
    CPPUNIT_ASSERT((*it++).x == DEFAULT_VALUE);
    CPPUNIT_ASSERT((*it++).x == DEFAULT_VALUE);
    CPPUNIT_ASSERT(it->x == DEFAULT_VALUE);

    v3.resize(5);
    CPPUNIT_ASSERT((++it)->x == DEFAULT_VALUE);
    CPPUNIT_ASSERT((++it)->x == DEFAULT_VALUE);

    TestElementDefConstruct element;
    element.x = 0x30808080;

    v3.resize(7, element);
    CPPUNIT_ASSERT((++it)->x == 0x30808080);
    CPPUNIT_ASSERT((++it)->x == 0x30808080);
}

void ListTest::string_nested()
{
    list<string> v;
    v.push_back(string());
    v.front() = "Memory must be allocated to put me into the string";
    CPPUNIT_ASSERT(strcmp(v.begin()->c_str(), "Memory must be allocated to put me into the string") == 0);

    list<string> v3(3);
    list<string>::iterator it = v3.begin();
    *it++ = "zzz";
    *it++ = "cba";
    *it = "abc";
    v3.sort();
    it = v3.begin();
    CPPUNIT_ASSERT(*it++ == "abc");
    CPPUNIT_ASSERT(*it++ == "cba");
    CPPUNIT_ASSERT(*it == "zzz");
    v3.erase(v3.begin());   // 需要保证erase能够调用内部元素的析构函数，否则会内存泄露。
    it = v3.begin();
    CPPUNIT_ASSERT(*it++ == "cba");
    CPPUNIT_ASSERT(*it++ == "zzz");
    v3.pop_front();  // 需要保证pop_front能够调用内部元素的析构函数，否则会内存泄露。
    v3.pop_back();  // 需要保证pop_back能够调用内部元素的析构函数，否则会内存泄露。
    v3.resize(3);
    it = v3.begin();
    *it++ = "zzz";
    *it++ = "cba";
    v3.erase(v3.begin(), v3.end());

    list<string> v2(2);
    v2.front() = "Put me into front!";
    v2.back() = "Put me into back!";
    v2.clear(); // 需要保证clear能够调用内部元素的析构函数，否则会内存泄露。
}

// Simple compilation test: Check that nested types like iterator
// can be access even if type used to instanciate container is not
// yet completely defined.
class IncompleteClass
{
    list<IncompleteClass> instances;
    typedef list<IncompleteClass>::iterator it;
};
