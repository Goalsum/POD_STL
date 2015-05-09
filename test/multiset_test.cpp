// 有序集合（允许重复元素）测试用例文件

#include "set"
#include "functional"
#include "cppunit_mini.h"

using namespace podstl;

// TestCase class
class MultisetTest : public TestCase
{
    CPPUNIT_TEST_SUITE(MultisetTest);
    CPPUNIT_TEST(mset1);
    CPPUNIT_TEST(mset3);
    CPPUNIT_TEST(mset5);
    CPPUNIT_TEST_SUITE_END();

protected:
    void mset1();
    void mset3();
    void mset5();
};

CPPUNIT_TEST_SUITE_REGISTRATION(MultisetTest);

// tests implementation
void MultisetTest::mset1()
{
    typedef multiset<int, less<int> > mset;
    mset s;
    s._M_verify();
    CPPUNIT_ASSERT(s.count(42) == 0);
    s.insert(42);
    s._M_verify();
    CPPUNIT_ASSERT(s.count(42) == 1);
    s.insert(s.begin(), 42);
    s._M_verify();
    CPPUNIT_ASSERT(s.count(42) == 2);

    mset::iterator i = s.find(40);
    CPPUNIT_ASSERT(i == s.end());

    i = s.find(42);
    CPPUNIT_ASSERT(i != s.end());
    size_t count = s.erase(42);
    s._M_verify();
    CPPUNIT_ASSERT(count == 2);
}

void MultisetTest::mset3()
{
    typedef multiset<char> mset;
    char array[] = { 3, 6, 1, 2, 3, 2, 6, 7, 9 };

    //Check iterator on a mutable set
    mset s(array, array + 9);
    s._M_verify();
    mset::iterator i;
    i = s.lower_bound(3);
    CPPUNIT_ASSERT(*i == 3);
    i = s.upper_bound(3);
    CPPUNIT_ASSERT(*i == 6);
    pair<mset::iterator, mset::iterator> p = s.equal_range(5);
    CPPUNIT_ASSERT(*(p.first) == 6);
    CPPUNIT_ASSERT(*(p.second) == 6);

    //Check const_iterator on a mutable multiset
    mset::const_iterator ci;
    ci = s.lower_bound(3);
    CPPUNIT_ASSERT(*ci == 3);
    ci = s.upper_bound(3);
    CPPUNIT_ASSERT(*ci == 6);
    pair<mset::const_iterator, mset::const_iterator> cp;
    cp = s.equal_range(5);
    CPPUNIT_ASSERT(*(cp.first) == 6);
    CPPUNIT_ASSERT(*(cp.second) == 6);

    //Check const_iterator on a const multiset
    mset const& crs = s;
    ci = crs.lower_bound(3);
    CPPUNIT_ASSERT(*ci == 3);
    ci = crs.upper_bound(3);
    CPPUNIT_ASSERT(*ci == 6);
    cp = crs.equal_range(5);
    CPPUNIT_ASSERT(*(cp.first) == 6);
    CPPUNIT_ASSERT(*(cp.second) == 6);

    s._M_verify();
    crs._M_verify();
}
void MultisetTest::mset5()
{
    short array[] = { 3, 6, 1, 9 };
    int j;

    typedef multiset<short, less<short> > fn_mset;
    fn_mset s1(array + 0, array + 4);
    fn_mset::const_iterator i = s1.begin();
    for (j = 0; i != s1.end(); ++i, ++j) 
    {
        CPPUNIT_ASSERT(j != 0 || *i == 1);
        CPPUNIT_ASSERT(j != 1 || *i == 3);
        CPPUNIT_ASSERT(j != 2 || *i == 6);
        CPPUNIT_ASSERT(j != 3 || *i == 9);
    }

    typedef multiset<short, greater<short> > fn_mset_g;
    fn_mset_g s2(array, array + 4);
    i = s2.begin();
    for (j = 0; i != s2.end(); ++i, ++j) 
    {
        CPPUNIT_ASSERT(j != 0 || *i == 9);
        CPPUNIT_ASSERT(j != 1 || *i == 6);
        CPPUNIT_ASSERT(j != 2 || *i == 3);
        CPPUNIT_ASSERT(j != 3 || *i == 1);
    }
}
