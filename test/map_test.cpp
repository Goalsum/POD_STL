// ×ÖµäÈÝÆ÷²âÊÔÓÃÀýÎÄ¼þ

#include "map"
#include "string"
#include "algorithm"
#include "cppunit_mini.h"

using namespace podstl;

// TestCase class
class MapTest : public TestCase
{
    CPPUNIT_TEST_SUITE(MapTest);
    CPPUNIT_TEST(map1);
    CPPUNIT_TEST(mmap1);
    CPPUNIT_TEST(mmap2);
    CPPUNIT_TEST(iterators);
    CPPUNIT_TEST(equal_range);
    CPPUNIT_TEST(template_methods);
    CPPUNIT_TEST(erase_algorithm);
    CPPUNIT_TEST_SUITE_END();

protected:
    void map1();
    void mmap1();
    void mmap2();
    void iterators();
    void equal_range();
    void template_methods();
    void erase_algorithm();
};

CPPUNIT_TEST_SUITE_REGISTRATION(MapTest);

// tests implementation
void MapTest::map1()
{
    typedef map<char, int, less<char> > maptype;
    maptype m;
    m._M_verify();

    // Store mappings between roman numerals and decimals.
    m['l'] = 50;
    m._M_verify();
    m['x'] = 20; // Deliberate mistake.
    m._M_verify();
    m['v'] = 5;
    m._M_verify();
    m['i'] = 1;
    m._M_verify();

    CPPUNIT_ASSERT(m['x'] == 20);
    m['x'] = 10; // Correct mistake.
    m._M_verify();
    CPPUNIT_ASSERT(m['x'] == 10);
    CPPUNIT_ASSERT(m['z'] == 0);
    CPPUNIT_ASSERT(m.count('z') == 1);

    pair<maptype::iterator, bool> p = m.insert(pair<const char, int>('c', 100));
    m._M_verify();
    CPPUNIT_ASSERT(p.second);
    CPPUNIT_ASSERT(p.first != m.end());
    CPPUNIT_ASSERT((*p.first).first == 'c');
    CPPUNIT_ASSERT((*p.first).second == 100);

    p = m.insert(pair<const char, int>('c', 100));
    m._M_verify();
    CPPUNIT_ASSERT(!p.second); // already existing pair
    CPPUNIT_ASSERT(p.first != m.end());
    CPPUNIT_ASSERT((*p.first).first == 'c');
    CPPUNIT_ASSERT((*p.first).second == 100);
}

void MapTest::mmap1()
{
    typedef multimap<char, short, less<char> > mmap;
    mmap m;
    m._M_verify();
    CPPUNIT_ASSERT(m.count('X') == 0);

    m.insert(pair<const char, int>('X', 10)); // Standard way.
    m._M_verify();
    CPPUNIT_ASSERT(m.count('X') == 1);

    m.insert(pair<const char, int>('X', 20));
    m._M_verify();
    CPPUNIT_ASSERT(m.count('X') == 2);

    m.insert(pair<const char, int>('Y', 32));
    m._M_verify();
    mmap::iterator it = m.find('X'); // Find first match.

    pair<const char, short> p('X', 10);
    CPPUNIT_ASSERT(*it == p);
    CPPUNIT_ASSERT((*it).first == 'X');
    CPPUNIT_ASSERT((*it).second == 10);
    it++;
    CPPUNIT_ASSERT((*it).first == 'X');
    CPPUNIT_ASSERT((*it).second == 20);
    it++;
    CPPUNIT_ASSERT((*it).first == 'Y');
    CPPUNIT_ASSERT((*it).second == 32);
    it++;
    CPPUNIT_ASSERT(it == m.end());

    size_t count = m.erase('X');
    m._M_verify();
    CPPUNIT_ASSERT(count == 2);
}

void MapTest::mmap2()
{
    typedef pair<const int, char> pair_type;

    pair_type p1(3, 'c');
    pair_type p2(6, 'f');
    pair_type p3(1, 'a');
    pair_type p4(2, 'b');
    pair_type p5(3, 'x');
    pair_type p6(6, 'f');

    typedef multimap<int, char, less<int> > mmap;

    pair_type array[] = {p1, p2, p3, p4, p5, p6};

    mmap m(array + 0, array + 6);
    m._M_verify();
    mmap::iterator it;
    it = m.lower_bound(3);
    CPPUNIT_ASSERT((*it).first == 3);
    CPPUNIT_ASSERT((*it).second == 'c');

    it = m.upper_bound(3);
    CPPUNIT_ASSERT((*it).first == 6);
    CPPUNIT_ASSERT((*it).second == 'f');
}

void MapTest::iterators()
{
    typedef map<short, char> int_map;
    int_map imap;
    {
        int_map::iterator ite(imap.begin());
        int_map::const_iterator cite(imap.begin());
        CPPUNIT_ASSERT(ite == cite);
        CPPUNIT_ASSERT(!(ite != cite));
        CPPUNIT_ASSERT(cite == ite);
        CPPUNIT_ASSERT(!(cite != ite));
    }

    typedef multimap<short, char> mmap;
    typedef mmap::value_type pair_type;

    pair_type p1(3, 'c');
    pair_type p2(6, 'f');
    pair_type p3(1, 'a');
    pair_type p4(2, 'b');
    pair_type p5(3, 'x');
    pair_type p6(6, 'f');

    pair_type array[] = { p1, p2, p3, p4, p5, p6 };

    mmap m(array + 0, array + 6);
    m._M_verify();
    {
        mmap::iterator ite(m.begin());
        mmap::const_iterator cite(m.begin());

        //test compare between const_iterator and iterator
        CPPUNIT_ASSERT(ite == cite);
        CPPUNIT_ASSERT(!(ite != cite));
        CPPUNIT_ASSERT(cite == ite);
        CPPUNIT_ASSERT(!(cite != ite));
    }

    mmap::reverse_iterator ri = m.rbegin();
    CPPUNIT_ASSERT(ri != m.rend());
    CPPUNIT_ASSERT(ri == m.rbegin());
    CPPUNIT_ASSERT((*ri).first == 6);
    CPPUNIT_ASSERT((*ri++).second == 'f');
    CPPUNIT_ASSERT((*ri).first == 6);
    CPPUNIT_ASSERT((*ri).second == 'f');

    mmap const& cm = m;
    mmap::const_reverse_iterator rci = cm.rbegin();
    CPPUNIT_ASSERT(rci != cm.rend());
    CPPUNIT_ASSERT((*rci).first == 6);
    CPPUNIT_ASSERT((*rci++).second == 'f');
    CPPUNIT_ASSERT((*rci).first == 6);
    CPPUNIT_ASSERT((*rci).second == 'f');

    m._M_verify();
}

void MapTest::equal_range()
{
    typedef map<char, long> maptype;
    {
        maptype m;
        m['x'] = 10;
        m._M_verify();

        pair<maptype::iterator, maptype::iterator> ret;
        ret = m.equal_range('x');
        CPPUNIT_ASSERT(ret.first != ret.second);
        CPPUNIT_ASSERT((*(ret.first)).first == 'x');
        CPPUNIT_ASSERT((*(ret.first)).second == 10);
        CPPUNIT_ASSERT(++(ret.first) == ret.second);
    }
    {
        {
            maptype m;

            maptype::iterator it = m.lower_bound('x');
            CPPUNIT_ASSERT(it == m.end());

            it = m.upper_bound('x');
            CPPUNIT_ASSERT(it == m.end());

            pair<maptype::iterator, maptype::iterator> ret;
            ret = m.equal_range('x');
            CPPUNIT_ASSERT(ret.first == ret.second);
            CPPUNIT_ASSERT(ret.first == m.end());

            m._M_verify();
        }

        {
            const maptype m;
            pair<maptype::const_iterator, maptype::const_iterator> ret;
            ret = m.equal_range('x');
            CPPUNIT_ASSERT(ret.first == ret.second);
            CPPUNIT_ASSERT(ret.first == m.end());

            m._M_verify();
        }
    }
}

struct Key
{
    int not_to_compare;
    int to_compare;

    Key(int x, int y) : to_compare(x), not_to_compare(y) {}
    bool operator <(const Key& other) const { return to_compare < other.to_compare; }
};

void MapTest::template_methods()
{
    typedef map<Key, string> Container;
    typedef Container::value_type value;
    Container cont;
    cont.insert(value(Key(4, 7), string()));
    cont._M_verify();
    cont.insert(value(Key(3, 8), string()));
    cont._M_verify();
    cont.insert(value(Key(1, 10), string()));
    cont._M_verify();
    cont.insert(value(Key(2, 9), string()));
    cont._M_verify();
    cont[Key(2, 6)] = "Put me in 2!";
    cont._M_verify();
    CPPUNIT_ASSERT(cont.size() == 4);
    Container::iterator it = cont.begin();
    CPPUNIT_ASSERT(it->first.to_compare == 1);
    CPPUNIT_ASSERT(it->first.not_to_compare == 10);
    ++it;
    CPPUNIT_ASSERT(it->first.to_compare == 2);
    CPPUNIT_ASSERT(it->first.not_to_compare == 9);
    CPPUNIT_ASSERT(it->second == "Put me in 2!");
    ++it;
    CPPUNIT_ASSERT(it->first.to_compare == 3);
    CPPUNIT_ASSERT(it->first.not_to_compare == 8);
    ++it;
    CPPUNIT_ASSERT(it->first.to_compare == 4);
    CPPUNIT_ASSERT(it->first.not_to_compare == 7);
    ++it;
    cont.erase(Key(2, 6));
    cont._M_verify();
    cont[Key(3, 6)] = "Put me in 3!";
    cont._M_verify();
    cont[Key(1, 6)] = "Put me in 1!";
    cont._M_verify();
    cont.clear();
    cont[Key(4, 6)] = "Put me in 4!";
    cont._M_verify();
}

void MapTest::erase_algorithm()
{
    map<long, long> victim;
    for (size_t i = 0; i < 1000; ++i)
    {
        victim[-2 * i];
        victim[-2 * i - 1];
        victim[-2 * i + 1];
        victim.erase(-2 * i + 1);
        victim.erase(-2 * i - 1);
        victim._M_verify();
    }
    map<long, long> cry;
    cry = victim;
    CPPUNIT_ASSERT(cry.size() == 1000);
}

class IncompleteClass
{
    map<IncompleteClass, IncompleteClass> instances;
    typedef map<IncompleteClass, IncompleteClass>::iterator it;
    multimap<IncompleteClass, IncompleteClass> minstances;
    typedef multimap<IncompleteClass, IncompleteClass>::iterator mit;
};
