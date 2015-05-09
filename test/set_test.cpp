// 有序集合测试用例文件

#include "set"
#include "string"
#include "algorithm"
#include "cppunit_mini.h"

using namespace podstl;

// TestCase class
class SetTest : public TestCase
{
    CPPUNIT_TEST_SUITE(SetTest);
    CPPUNIT_TEST(set1);
    CPPUNIT_TEST(set2);
    CPPUNIT_TEST(erase);
    CPPUNIT_TEST(insert);
    CPPUNIT_TEST(find);
    CPPUNIT_TEST(bounds);
    CPPUNIT_TEST(specialized_less);
    CPPUNIT_TEST(implementation_check);
    CPPUNIT_TEST(reverse_iterator_test);
    CPPUNIT_TEST(rb_tree_strong_test);
    CPPUNIT_TEST(customed_compare);
    CPPUNIT_TEST(template_methods);
    CPPUNIT_TEST(string_nested);
    CPPUNIT_TEST_SUITE_END();

protected:
    void set1();
    void set2();
    void erase();
    void insert();
    void find();
    void bounds();
    void specialized_less();
    void implementation_check();
    void reverse_iterator_test();
    void rb_tree_strong_test();
    void customed_compare();
    void template_methods();
    void string_nested();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SetTest);

// tests implementation
void SetTest::set1()
{
    set<char, less<char> > s;
    s._M_verify();
    CPPUNIT_ASSERT(s.count(42) == 0);
    s.insert(42);
    s._M_verify();
    CPPUNIT_ASSERT(s.count(42) == 1);
    s.insert(42);
    s._M_verify();
    CPPUNIT_ASSERT(s.count(42) == 1);

    s.insert(95);
    s._M_verify();
    s.insert(96);
    s._M_verify();
    s.insert(41);
    s._M_verify();
    set<char, less<char> > s2(s);
    s2._M_verify();
    CPPUNIT_ASSERT(s2.size() == 4);
    set<char, less<char> >::iterator it = s2.begin();
    CPPUNIT_ASSERT(*it++ == 41);
    CPPUNIT_ASSERT(*it++ == 42);
    CPPUNIT_ASSERT(*it++ == 95);
    CPPUNIT_ASSERT(*it++ == 96);
    CPPUNIT_ASSERT(it == s2.end());

    size_t count = s.erase(42);
    s._M_verify();
    CPPUNIT_ASSERT(count == 1);

    s2 = s;
    s2._M_verify();
    CPPUNIT_ASSERT(s2.size() == 3);
    it = s2.begin();
    CPPUNIT_ASSERT(*it++ == 41);
    CPPUNIT_ASSERT(*it++ == 95);
    CPPUNIT_ASSERT(*it++ == 96);
    CPPUNIT_ASSERT(it == s2.end());
}

void SetTest::set2()
{
    typedef set<int, less<int> > int_set;
    int_set s;
    s._M_verify();
    pair<int_set::iterator, bool> p = s.insert(42);
    s._M_verify();
    CPPUNIT_ASSERT(p.second == true);
    p = s.insert(42);
    s._M_verify();
    CPPUNIT_ASSERT(p.second == false);

    int array1[] = { 1, 3, 6, 7 };
    s.insert(array1, array1 + 4);
    s._M_verify();
    CPPUNIT_ASSERT(distance(s.begin(), s.end()) == 5);

    int_set s2;
    s2.swap(s);
    s._M_verify();
    s2._M_verify();
    CPPUNIT_ASSERT(distance(s2.begin(), s2.end()) == 5);
    CPPUNIT_ASSERT(distance(s.begin(), s.end()) == 0);

    int_set s3;
    s3.swap(s);
    s3.swap(s2);
    s._M_verify();
    s2._M_verify();
    s3._M_verify();
    CPPUNIT_ASSERT(distance(s.begin(), s.end()) == 0);
    CPPUNIT_ASSERT(distance(s2.begin(), s2.end()) == 0);
    CPPUNIT_ASSERT(distance(s3.begin(), s3.end()) == 5);
}

void SetTest::erase()
{
    set<short, less<short> > s;
    s.insert(1);
    s._M_verify();
    s.erase(s.begin());
    s._M_verify();
    CPPUNIT_ASSERT(s.empty());

    size_t nb = s.erase(1);
    s._M_verify();
    CPPUNIT_ASSERT(nb == 0);

    short arr[8] = {1, 2, 3, 5, 4, 6, 7, 8};
    s.insert(arr, arr + 8);
    s._M_verify();
    set<short, less<short> >::iterator begin_it = s.find(6);
    set<short, less<short> >::iterator end_it = s.find(8);
    s.erase(begin_it, end_it);
    s._M_verify();
    begin_it = s.find(3);
    end_it = s.find(8);
    s.erase(begin_it, end_it);
    s._M_verify();
}

void SetTest::insert()
{
    set<long> s;
    set<long>::iterator i = s.insert(s.end(), 0);
    s._M_verify();
    CPPUNIT_ASSERT(*i == 0);
    s.insert(s.begin(), 1);
    s._M_verify();
    s.insert(s.end(), -2);
    s._M_verify();
    s.insert(s.begin(), -1);
    s._M_verify();
    CPPUNIT_ASSERT(s.size() == 4);

    set<long> s2;
    s2.insert(-1);
    s2.insert(-2);
    s2.insert(3);
    s2._M_verify();
    CPPUNIT_ASSERT(s < s2);
    CPPUNIT_ASSERT(s2 > s);
    CPPUNIT_ASSERT(s2 != s);
    s2.erase(3);
    s2._M_verify();
    s2.insert(0);
    s2.insert(1);
    s2._M_verify();
    CPPUNIT_ASSERT(s2 == s);
}

void SetTest::find()
{
    set<LONG_LONG> s;
    CPPUNIT_ASSERT(s.find(0) == s.end());

    set<LONG_LONG> const& crs = s;
    CPPUNIT_ASSERT(crs.find(0) == crs.end());
}

void SetTest::bounds()
{
    unsigned int array1[] = { 1, 3, 6, 7 };
    set<unsigned int> s(array1, array1 + sizeof(array1) / sizeof(array1[0]));
    s._M_verify();
    set<unsigned int> const& crs = s;
    set<unsigned int>::iterator sit;
    set<unsigned int>::const_iterator scit;
    pair<set<unsigned int>::iterator, set<unsigned int>::iterator> pit;
    pair<set<unsigned int>::const_iterator, set<unsigned int>::const_iterator> pcit;

    // Check iterator on mutable set
    sit = s.lower_bound(2);
    CPPUNIT_ASSERT(sit != s.end());
    CPPUNIT_ASSERT(*sit == 3);

    sit = s.upper_bound(5);
    CPPUNIT_ASSERT(sit != s.end());
    CPPUNIT_ASSERT(*sit == 6);

    pit = s.equal_range(6);
    CPPUNIT_ASSERT(pit.first != pit.second);
    CPPUNIT_ASSERT(pit.first != s.end());
    CPPUNIT_ASSERT(*pit.first == 6);
    CPPUNIT_ASSERT(pit.second != s.end());
    CPPUNIT_ASSERT(*pit.second == 7);

    pit = s.equal_range(4);
    CPPUNIT_ASSERT(pit.first == pit.second);
    CPPUNIT_ASSERT(pit.first != s.end());
    CPPUNIT_ASSERT(*pit.first == 6);
    CPPUNIT_ASSERT(pit.second != s.end());
    CPPUNIT_ASSERT(*pit.second == 6);

    // Check const_iterator on mutable set
    scit = s.lower_bound(2);
    CPPUNIT_ASSERT(scit != s.end());
    CPPUNIT_ASSERT(*scit == 3);

    scit = s.upper_bound(5);
    CPPUNIT_ASSERT(scit != s.end());
    CPPUNIT_ASSERT(*scit == 6);

    pcit = s.equal_range(6);
    CPPUNIT_ASSERT( pcit.first != pcit.second );
    CPPUNIT_ASSERT( pcit.first != s.end() );
    CPPUNIT_ASSERT( *pcit.first == 6 );
    CPPUNIT_ASSERT( pcit.second != s.end() );
    CPPUNIT_ASSERT( *pcit.second == 7 );

    // Check const_iterator on const set
    scit = crs.lower_bound(2);
    CPPUNIT_ASSERT(scit != crs.end());
    CPPUNIT_ASSERT(*scit == 3);

    scit = crs.upper_bound(5);
    CPPUNIT_ASSERT(scit != crs.end());
    CPPUNIT_ASSERT(*scit == 6);

    pcit = crs.equal_range(6);
    CPPUNIT_ASSERT(pcit.first != pcit.second);
    CPPUNIT_ASSERT(pcit.first != crs.end());
    CPPUNIT_ASSERT(*pcit.first == 6);
    CPPUNIT_ASSERT(pcit.second != crs.end());
    CPPUNIT_ASSERT(*pcit.second == 7);

    s._M_verify();
}

class SetTestClass 
{
public:
    SetTestClass(int data) : _data(data) {}

    int data() const 
    {
        return _data;
    }

private:
    int _data;
};

namespace podstl 
{
    template <>
    struct less<SetTestClass> 
    {
        bool operator () (SetTestClass const& lhs, SetTestClass const& rhs) const 
        {
            return lhs.data() < rhs.data();
        }
    };
}

void SetTest::specialized_less()
{
    set<SetTestClass> s;
    s.insert(SetTestClass(1));
    s._M_verify();
    s.insert(SetTestClass(3));
    s._M_verify();
    s.insert(SetTestClass(2));
    s._M_verify();
    s.insert(SetTestClass(0));
    s._M_verify();

    set<SetTestClass>::iterator sit(s.begin()), sitEnd(s.end());
    int i = 0;
    for (; sit != sitEnd; ++sit, ++i) 
    {
        CPPUNIT_ASSERT(sit->data() == i);
    }
    s._M_verify();
}

void SetTest::implementation_check()
{
    set<unsigned char> tree;
    tree.insert(1);
    tree._M_verify();
    set<unsigned char>::iterator it = tree.begin();
    unsigned char const& int_ref = *it++;
    CPPUNIT_ASSERT(int_ref == 1);

    CPPUNIT_ASSERT(it == tree.end());
    CPPUNIT_ASSERT(it != tree.begin());

    set<unsigned char>::const_iterator cit = tree.begin();
    unsigned char const& int_cref = *cit++;
    CPPUNIT_ASSERT(int_cref == 1);

    tree._M_verify();
}

void SetTest::reverse_iterator_test()
{
    set<unsigned short> tree;
    tree.insert(1);
    tree._M_verify();
    tree.insert(2);
    tree._M_verify();

    {
        set<unsigned short>::reverse_iterator rit(tree.rbegin());
        CPPUNIT_ASSERT(*(rit++) == 2);
        CPPUNIT_ASSERT(*(rit++) == 1);
        CPPUNIT_ASSERT(rit == tree.rend());
    }

    {
        set<unsigned short> const& ctree = tree;
        set<unsigned short>::const_reverse_iterator rit(ctree.rbegin());
        CPPUNIT_ASSERT(*(rit++) == 2);
        CPPUNIT_ASSERT(*(rit++) == 1);
        CPPUNIT_ASSERT(rit == ctree.rend());
    }

    tree._M_verify();
}

void SetTest::rb_tree_strong_test()
{
    set<unsigned long> tree;
    int i;
    for (i = 1; i <= 100; ++i)
    {
        tree.insert(i);
        tree._M_verify();
    }
    for (i = 60; i <= 70; ++i)
    {
        tree.erase(i);
        tree._M_verify();
    }
    for (i = 30; i >= 20; --i)
    {
        tree.erase(i);
        tree._M_verify();
    }
    for (i = 101; i <= 200; ++i)
    {
        tree.insert(i);
        tree._M_verify();
    }
    for (i = 2; i < 200; ++i)
    {
        tree.erase(i);
        tree._M_verify();
    }
    CPPUNIT_ASSERT(tree.size() == 2);
}

struct custom
{
    int not_to_compare;
    int to_compare;
    custom(int x, int y) : to_compare(x), not_to_compare(y) {}

    bool operator <(const custom& other) const
    {
        return to_compare < other.to_compare;
    }
};

void SetTest::customed_compare()
{
    set<custom> s;
    s.insert(custom(6, 1));
    s._M_verify();
    s.insert(custom(3, 5));
    s._M_verify();
    s.insert(custom(4, 2));
    s._M_verify();
    set<custom>::iterator it = s.begin();
    CPPUNIT_ASSERT(it->to_compare == 3);
    CPPUNIT_ASSERT(it->not_to_compare == 5);
    ++it;
    CPPUNIT_ASSERT(it->to_compare == 4);
    CPPUNIT_ASSERT(it->not_to_compare == 2);
    ++it;
    CPPUNIT_ASSERT(it->to_compare == 6);
    CPPUNIT_ASSERT(it->not_to_compare == 1);
    ++it;
    CPPUNIT_ASSERT(it == s.end());
}

struct Key
{
    Key() : m_data(0) {}
    explicit Key(int data) : m_data(data) {}

    int m_data;
};

struct KeyCmp
{
    bool operator () (Key lhs, Key rhs) const
    {
        return lhs.m_data < rhs.m_data;
    }

    bool operator () (Key lhs, int rhs) const
    {
        return lhs.m_data < rhs;
    }

    bool operator () (int lhs, Key rhs) const
    {
        return lhs < rhs.m_data;
    }
};

struct KeyCmpPtr
{
    bool operator () (Key const volatile *lhs, Key const volatile *rhs) const
    {
        return (*lhs).m_data < (*rhs).m_data;
    }

    bool operator () (Key const volatile *lhs, int rhs) const
    {
        return (*lhs).m_data < rhs;
    }

    bool operator () (int lhs, Key const volatile *rhs) const
    {
        return lhs < (*rhs).m_data;
    }
};

void SetTest::template_methods()
{
    {
        typedef set<Key, KeyCmp> KeySet;
        KeySet keySet;
        keySet._M_verify();
        keySet.insert(Key(1));
        keySet._M_verify();
        keySet.insert(Key(2));
        keySet._M_verify();
        keySet.insert(Key(3));
        keySet._M_verify();
        keySet.insert(Key(4));
        keySet._M_verify();

        CPPUNIT_ASSERT( keySet.count(Key(1)) == 1 );
    }

    {
        typedef set<Key*, KeyCmpPtr> KeySet;
        KeySet keySet;
        Key key1(1), key2(2), key3(3), key4(4);
        keySet.insert(&key1);
        keySet._M_verify();
        keySet.insert(&key2);
        keySet._M_verify();
        keySet.insert(&key3);
        keySet._M_verify();
        keySet.insert(&key4);
        keySet._M_verify();
    }
    {
        typedef multiset<Key, KeyCmp> KeySet;
        KeySet keySet;
        keySet.insert(Key(1));
        keySet._M_verify();
        keySet.insert(Key(2));
        keySet._M_verify();
        keySet.insert(Key(3));
        keySet._M_verify();
        keySet.insert(Key(4));
        keySet._M_verify();

        CPPUNIT_ASSERT( keySet.count(Key(1)) == 1 );
    }
    {
        typedef multiset<Key const volatile*, KeyCmpPtr> KeySet;
        KeySet keySet;
        Key key1(1), key2(2), key3(3), key4(4);
        keySet.insert(&key1);
        keySet._M_verify();
        keySet.insert(&key2);
        keySet._M_verify();
        keySet.insert(&key3);
        keySet._M_verify();
        keySet.insert(&key4);
        keySet._M_verify();
    }
}

void SetTest::string_nested()
{
    set<string> s;
    s._M_verify();
    pair<set<string>::iterator, bool> result = s.insert(string());
    s._M_verify();
    CPPUNIT_ASSERT(result.second == true);
    result = s.insert(string());
    s._M_verify();
    CPPUNIT_ASSERT(result.second == false);
    s.clear();
    s._M_verify();
}

// Simple compilation test: Check that nested types like iterator
// can be access even if type used to instanciate container is not
// yet completely defined.
class IncompleteClass
{
    set<IncompleteClass> instances;
    typedef set<IncompleteClass>::iterator it;
    multiset<IncompleteClass> minstances;
    typedef multiset<IncompleteClass>::iterator mit;
};
