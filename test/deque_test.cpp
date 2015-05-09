// 双端队列测试用例文件

#include "deque"
#include "string"
#include "algorithm"
#include "cppunit_mini.h"

using namespace podstl;

// TestCase class
class DequeTest : public TestCase
{
    CPPUNIT_TEST_SUITE(DequeTest);
    CPPUNIT_TEST(deque1);
    CPPUNIT_TEST(insert);
    CPPUNIT_TEST(at);
    CPPUNIT_TEST(auto_ref);
    CPPUNIT_TEST(constructors);
    CPPUNIT_TEST(erase);
    CPPUNIT_TEST(assign);
    CPPUNIT_TEST(string_nested);
    CPPUNIT_TEST_SUITE_END();

protected:
    void deque1();
    void insert();
    void at();
    void auto_ref();
    void constructors();
    void erase();
    void assign();
    void string_nested();
};

CPPUNIT_TEST_SUITE_REGISTRATION(DequeTest);

// tests implementation
void DequeTest::deque1()
{
    deque<int> d;
    CPPUNIT_ASSERT(d.empty());
    d.push_back(4);
    d.push_back(9);
    d.push_back(16);
    d.push_front(1);

    CPPUNIT_ASSERT(d[0] == 1);
    CPPUNIT_ASSERT(d[1] == 4);
    CPPUNIT_ASSERT(d[2] == 9);
    CPPUNIT_ASSERT(d[3] == 16);

    d.pop_front();
    d[2] = 25;

    CPPUNIT_ASSERT(d[0] == 4);
    CPPUNIT_ASSERT(d[1] == 9);
    CPPUNIT_ASSERT(d[2] == 25);

    d.pop_back();
    CPPUNIT_ASSERT(!d.empty());
    CPPUNIT_ASSERT(d.size() == 2);
    CPPUNIT_ASSERT(d[0] == 4);
    CPPUNIT_ASSERT(d[1] == 9);

    // Some compile time tests:
    deque<int>::iterator dit = d.begin();
    deque<int>::const_iterator cdit(d.begin());
    CPPUNIT_ASSERT((dit - cdit) == 0);
    CPPUNIT_ASSERT((cdit - dit) == 0);
    CPPUNIT_ASSERT((dit - dit) == 0);
    CPPUNIT_ASSERT((cdit - cdit) == 0);
    CPPUNIT_ASSERT(!((dit < cdit) || (dit > cdit) || (dit != cdit) || !(dit <= cdit) || !(dit >= cdit)));
}

void DequeTest::insert()
{
    deque<char> d;
    char arr[5] = { 9, 0, 1, 2, 8 };
    d.insert(d.begin(), arr + 1, arr + 4);
    CPPUNIT_ASSERT(d.size() == 3);
    CPPUNIT_CHECK(d[0] == 0);
    CPPUNIT_CHECK(d[1] == 1);
    CPPUNIT_CHECK(d[2] == 2);

    deque<char>::iterator dit;

    //Insertion before begin:
    dit = d.insert(d.begin(), 3);
    CPPUNIT_ASSERT(dit != d.end());
    CPPUNIT_CHECK(*dit == 3);
    CPPUNIT_ASSERT(d.size() == 4);
    CPPUNIT_ASSERT(d[0] == 3);

    //Insertion after begin:
    dit = d.insert(d.begin() + 1, 4);
    CPPUNIT_ASSERT(dit != d.end());
    CPPUNIT_CHECK(*dit == 4);
    CPPUNIT_ASSERT(d.size() == 5);
    CPPUNIT_ASSERT(d[1] == 4);

    //Insertion at end:
    dit = d.insert(d.end(), 5);
    CPPUNIT_ASSERT(dit != d.end());
    CPPUNIT_CHECK(*dit == 5);
    CPPUNIT_ASSERT(d.size() == 6);
    CPPUNIT_ASSERT(d[5] == 5);

    //Insertion before last element:
    dit = d.insert(d.end() - 1, 6);
    CPPUNIT_ASSERT(dit != d.end());
    CPPUNIT_CHECK(*dit == 6);
    CPPUNIT_ASSERT(d.size() == 7);
    CPPUNIT_ASSERT(d[5] == 6);

    //Insertion of several elements before begin
    d.insert(d.begin(), 2, 7);
    CPPUNIT_ASSERT(d.size() == 9);
    CPPUNIT_ASSERT(d[0] == 7);
    CPPUNIT_ASSERT(d[1] == 7);

    //Insertion of several elements after begin
    //There is more elements to insert than elements before insertion position
    d.insert(d.begin() + 1, 2, 8);
    CPPUNIT_ASSERT(d.size() == 11);
    CPPUNIT_ASSERT(d[1] == 8);
    CPPUNIT_ASSERT(d[2] == 8);

    //There is less elements to insert than elements before insertion position
    d.insert(d.begin() + 3, 2, 9);
    CPPUNIT_ASSERT(d.size() == 13);
    CPPUNIT_ASSERT(d[3] == 9);
    CPPUNIT_ASSERT(d[4] == 9);

    //Insertion of several elements at end:
    d.insert(d.end(), 2, 10);
    CPPUNIT_ASSERT(d.size() == 15);
    CPPUNIT_ASSERT(d[14] == 10);
    CPPUNIT_ASSERT(d[13] == 10);

    //Insertion of several elements before last:
    //There is more elements to insert than elements after insertion position
    d.insert(d.end() - 1, 2, 11);
    CPPUNIT_ASSERT(d.size() == 17);
    CPPUNIT_ASSERT(d[15] == 11);
    CPPUNIT_ASSERT(d[14] == 11);

    //There is less elements to insert than elements after insertion position
    d.insert(d.end() - 3, 2, 12);
    CPPUNIT_ASSERT(d.size() == 19);
    CPPUNIT_ASSERT(d[15] == 12);
    CPPUNIT_ASSERT(d[14] == 12);
}

void DequeTest::at() 
{
    deque<long> d;
    deque<long> const& cd = d;

    d.push_back(10);
    CPPUNIT_ASSERT(d.at(0) == 10);
    d.at(0) = 20;
    CPPUNIT_ASSERT(cd.at(0) == 20);
}

void DequeTest::auto_ref()
{
    unsigned int i;
    deque<unsigned int> ref;
    for (i = 0; i < 5; ++i) 
    {
        ref.push_back(i);
    }

    for (i = 0; i < 5; ++i) 
    {
        CPPUNIT_ASSERT(ref[i] == i);
    }
}

struct Point 
{
    int x, y;
    Point() : x(123), y(456) {};
    bool operator == (const Point& __other) const
    {
        return (x == __other.x) && (y == __other.y);
    }
};

void DequeTest::constructors()
{
    deque<Point> d1(1);
    CPPUNIT_ASSERT(d1.size() == 1);
    CPPUNIT_ASSERT((d1[0].x == 123) && (d1[0].y == 456));

    deque<Point> d2(d1.begin(), d1.end());
    CPPUNIT_ASSERT(d2.size() == 1);
    CPPUNIT_ASSERT((d2[0].x == 123) && (d2[0].y == 456));

    d2.resize(2);
    CPPUNIT_ASSERT(d2.size() == 2);
    CPPUNIT_ASSERT((d2[1].x == 123) && (d2[1].y == 456));

    Point pt;
    pt.x = 789;
    pt.y = 987;
    d2.resize(3, pt);
    CPPUNIT_ASSERT(d2.size() == 3);
    CPPUNIT_ASSERT((d2[2].x == 789) && (d2[2].y == 987));

    deque<Point> d3(d2);
    CPPUNIT_ASSERT(d3.size() == 3);
    CPPUNIT_ASSERT(d3 == d2);

    Point arr[3];
    deque<Point> d4(arr + 1, arr + 3);
    CPPUNIT_ASSERT(d4.size() == 2);
    CPPUNIT_ASSERT((d4[0].x == 123) && (d4[0].y == 456));

    deque<Point> d5(97, pt);
    CPPUNIT_ASSERT(d5.size() == 97);
    CPPUNIT_ASSERT((d5[96].x == 789) && (d5[96].y == 987));
    d5.resize(2);
    CPPUNIT_ASSERT(d5.size() == 2);
    CPPUNIT_ASSERT((d5[1].x == 789) && (d5[1].y == 987));
}

void DequeTest::erase()
{
    deque<LONG_LONG> dint;
    dint.push_back(3);
    dint.push_front(2);
    dint.push_back(4);
    dint.push_front(1);
    dint.push_back(5);
    dint.push_front(0);
    dint.push_back(6);

    deque<LONG_LONG>::iterator it(dint.begin() + 1);
    CPPUNIT_ASSERT(*it == 1);

    dint.erase(dint.begin());
    CPPUNIT_ASSERT(*it == 1);

    it = dint.end() - 2;
    CPPUNIT_ASSERT(*it == 5);

    dint.erase(dint.end() - 1);
    CPPUNIT_ASSERT(*it == 5);

    dint.push_back(6);
    dint.push_front(0);

    it = dint.begin() + 2;
    CPPUNIT_ASSERT(*it == 2);

    dint.erase(dint.begin(), dint.begin() + 2);
    CPPUNIT_ASSERT(*it == 2);

    it = dint.end() - 3;
    CPPUNIT_ASSERT(*it == 4);

    dint.erase(dint.end() - 2, dint.end());
    CPPUNIT_ASSERT(*it == 4);
}

void DequeTest::assign()
{
    deque<short> d1(5, 3);
    d1.assign(3, 9);
    CPPUNIT_ASSERT(d1.size() == 3);
    CPPUNIT_ASSERT(d1[2] == 9);

    short arr[6] = {1, 2, 3, 4, 5, 6};
    d1.assign(arr + 1, arr + 5);
    CPPUNIT_ASSERT(d1.size() == 4);
    CPPUNIT_ASSERT(d1[0] == 2);
    CPPUNIT_ASSERT(d1[3] == 5);

    deque<short> d2;
    d2 = d1;
    CPPUNIT_ASSERT(d2.size() == 4);
    CPPUNIT_ASSERT(d2[0] == 2);
    CPPUNIT_ASSERT(d2[3] == 5);

    d2.back() = 4;
    CPPUNIT_ASSERT(d2 < d1);
    d1.front() = 1;
    CPPUNIT_ASSERT(d2 > d1);
}

void DequeTest::string_nested()
{
    deque<string> v;
    v.push_back(string());
    v.front() = "Memory must be allocated to put me into the string";
    CPPUNIT_ASSERT(strcmp(v.begin()->c_str(), "Memory must be allocated to put me into the string") == 0);

    deque<string> v3;
    v3.push_back(string());
    v3[0] = "zzz";
    v3.push_front(string());
    v3[0] = "cba";
    v3.push_back(string());
    v3[2] = "abc";
    v3.erase(v3.begin());   // 需要保证erase能够调用内部元素的析构函数，否则会内存泄露。
    CPPUNIT_ASSERT(v3.size() == 2);
    CPPUNIT_ASSERT(v3[0] == "zzz");
    CPPUNIT_ASSERT(v3[1] == "abc");
    v3.pop_back();  // 需要保证pop_back能够调用内部元素的析构函数，否则会内存泄露。
    v3.pop_front();  // 需要保证pop_front能够调用内部元素的析构函数，否则会内存泄露。

    deque<string> v2(2);
    v2.front() = "Put me into front!";
    v2.back() = "Put me into back!";
    v2.erase(v2.begin(), v2.end());
    CPPUNIT_ASSERT(v2.empty());

    v2.resize(2);
    v2.front() = "Put me into front!";
    v2.back() = "Put me into back!";
    v2.clear(); // 需要保证clear能够调用内部元素的析构函数，否则会内存泄露。
}

// Simple compilation test: Check that nested types like iterator
// can be access even if type used to instanciate container is not
// yet completely defined.
class IncompleteClass
{
    deque<IncompleteClass> instances;
    typedef deque<IncompleteClass>::size_type size;
};
