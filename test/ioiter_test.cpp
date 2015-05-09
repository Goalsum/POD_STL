// 流迭代器测试用例文件

#include "string"
#include "sstream"
#include "vector"
#include "iterator"
#include "cppunit_mini.h"

using namespace podstl;

class IoiterTest : public TestCase
{
    CPPUNIT_TEST_SUITE(IoiterTest);
    CPPUNIT_TEST(ioiter_test);
    CPPUNIT_TEST(assign_test);
    CPPUNIT_TEST(assign2_test);
    CPPUNIT_TEST(istmit1);
    CPPUNIT_TEST(ostmit0);
    CPPUNIT_TEST(copy_n_test);
    CPPUNIT_TEST_SUITE_END();

protected:
    void ioiter_test();
    void assign_test();
    void assign2_test();
    void istmit1();
    void ostmit0();
    void copy_n_test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(IoiterTest);

void IoiterTest::ioiter_test()
{
    char c;
    const char *pc;
    const char *strorg = "abcd";
    string tmp;
    string objStr(strorg);

    istringstream objIStrStrm1(objStr);
    istringstream objIStrStrm2(objStr);
    istringstream objIStrStrm3(objStr);

    pc = strorg;
    string::size_type sz = strlen(strorg);
    string::size_type i;
    for (i = 0; i < sz; ++i) 
    {
        c = *pc++;
        tmp += c;
    }
    CPPUNIT_ASSERT(tmp == "abcd");

    istreambuf_iterator objIStrmbIt1(objIStrStrm1.rdbuf());
    istreambuf_iterator end;

    tmp.clear();

    for (i = 0; i < sz; ++i) 
    {
        c = *objIStrmbIt1++;
        tmp += c;
    }
    CPPUNIT_ASSERT(tmp == "abcd");

    tmp.clear();

    istreambuf_iterator objIStrmbIt2(objIStrStrm2.rdbuf());
    for (i = 0; i < sz; ++i) 
    {
        c = *objIStrmbIt2;
        tmp += c;
        objIStrmbIt2++;
    }
    CPPUNIT_ASSERT(tmp == "abcd");

    tmp.clear();

    istreambuf_iterator objIStrmbIt3(objIStrStrm3.rdbuf());

    while (objIStrmbIt3 != end) 
    {
        c = *objIStrmbIt3++;
        tmp += c;
    }
    CPPUNIT_ASSERT(tmp == "abcd");
}

void IoiterTest::assign_test()
{
    stringstream s("1234567890");
    vector<char> v;

    v.assign(istreambuf_iterator(s.rdbuf()), istreambuf_iterator());
    CPPUNIT_CHECK(v.size() == 10);
    if (v.size() == 10) 
    {
        CPPUNIT_CHECK(v[0] == '1');
        CPPUNIT_CHECK(v[9] == '0');
    }
}

void IoiterTest::assign2_test()
{
    stringstream s("1234567890");
    vector<char> v;

    v.assign(istreambuf_iterator(s.rdbuf()), istreambuf_iterator());
    CPPUNIT_CHECK(v.size() == 10);
    if (v.size() == 10) 
    {
        CPPUNIT_CHECK(v[0] == '1');
        CPPUNIT_CHECK(v[9] == '0');
    }
}

void IoiterTest::istmit1()
{
    const char* buff = "MyString";
    istringstream istr(buff);

    char buffer[100];
    size_t i = 0;
    istr.unsetf(ios::skipws);    // Disable white-space skipping.
    istream_iterator<char> s(istr), meos;
    while ((s != meos) && (*s != '\n') && (i < (sizeof(buffer) / sizeof(buffer[0]))))
    {
        buffer[i++] = *s++;
    }
    buffer[i] = '\0'; // Null terminate buffer.

    CPPUNIT_ASSERT(strcmp(buffer, buff) == 0);

    {
        istringstream empty_istr;
        CPPUNIT_ASSERT(istream_iterator<char>(empty_istr) == istream_iterator<char>());
    }
}

void IoiterTest::copy_n_test()
{
    // This test check that no character is lost while reading the istream through a istream_iterator.
    //{
    //    istringstream istr("aabbcd");
    //    string chars;
    //    istream_iterator<char> ite = copy_n(copy_n(istream_iterator<char>(istr),
    //        2, back_inserter(chars)).first,
    //        2, back_inserter(chars)).first;
    //    CPPUNIT_ASSERT(chars == "aabb");
    //    copy_n(ite, 2, back_inserter(chars));
    //    CPPUNIT_ASSERT(chars == "aabbcd");
    //}
    //{
    //    istringstream istr("11 22 AA BB 33 44 CC DD");
    //    vector<int> ints;
    //    vector<string> strings;

    //    copy_n(istream_int_ite(istr), 2, back_inserter(ints));
    //    CPPUNIT_ASSERT(ints.size() == 2);
    //    CPPUNIT_ASSERT(ints[0] == 11);
    //    CPPUNIT_ASSERT(ints[1] == 22);
    //    ints.clear();
    //    istr.clear();
    //    copy_n(istream_string_ite(istr), 2, back_inserter(strings));
    //    CPPUNIT_ASSERT(strings.size() == 2);
    //    CPPUNIT_ASSERT(strings[0] == "AA");
    //    CPPUNIT_ASSERT(strings[1] == "BB");
    //    strings.clear();
    //    istr.clear();
    //    /* The following code cannot work, '33' is extracted as a string
    //    * in the previous copy_n call, this value is returned in the pair
    //    * returned by copy_n but is lost as this istream_iterator is not used.
    //    * copy_n and istream_iterator can only be combined safely if:
    //    * - you always extract the same type of istream_iterator and you always reuse
    //    * the istream_iterator returned by copy_n (see previous test with "aabbcd")
    //    * - you extract different type of object and no object is convertible to an other
    //    * as in this current test when you extract int and string (when you extract ints
    //    * again it fails as int can be converted to strings.
    //    *
    //    copy_n(istream_int_ite(istr), 2, back_inserter(ints));
    //    CPPUNIT_ASSERT( ints.size() == 2 );
    //    CPPUNIT_ASSERT( ints[0] == 33 );
    //    CPPUNIT_ASSERT( ints[1] == 44 );
    //    istr.clear();
    //    copy_n(istream_string_ite(istr), 2, back_inserter(strings));
    //    CPPUNIT_ASSERT( strings.size() == 2 );
    //    CPPUNIT_ASSERT( strings[0] == "CC" );
    //    CPPUNIT_ASSERT( strings[1] == "DD" );
    //    */
    //}
    //{
    //    istringstream is("1 2 3 4 5 6 7 8 9 10");
    //    vector<int> ints;
    //    istream_iterator<int> itr(is);
    //    itr = copy_n(itr, 0, back_inserter(ints)).first;
    //    CPPUNIT_ASSERT(ints.empty());
    //    itr = copy_n(itr, -1, back_inserter(ints)).first;
    //    CPPUNIT_ASSERT(ints.empty());
    //    itr = copy_n(itr, 2, back_inserter(ints)).first;
    //    CPPUNIT_ASSERT(ints.size() == 2);
    //    CPPUNIT_ASSERT(ints[0] == 1);
    //    CPPUNIT_ASSERT(ints[1] == 2);
    //    itr = copy_n(itr, 2, back_inserter(ints)).first;
    //    CPPUNIT_ASSERT(ints.size() == 4);
    //    CPPUNIT_ASSERT(ints[2] == 3);
    //    CPPUNIT_ASSERT(ints[3] == 4);
    //    itr = copy_n(itr, 2, back_inserter(ints)).first;
    //    CPPUNIT_ASSERT(ints.size() == 6);
    //    CPPUNIT_ASSERT(ints[4] == 5);
    //    CPPUNIT_ASSERT(ints[5] == 6);
    //}
}

void IoiterTest::ostmit0()
{
    // not necessary, tested in copy_test
    int array[] = { 1, 5, 2, 4 };

    const char* text = "hello";

    ostringstream os;

    ostream_iterator<char> iter(os);
    copy(text, text + 5, iter);
    CPPUNIT_ASSERT(os.good());
    os << ' ';
    CPPUNIT_ASSERT(os.good());

    ostream_iterator<int> iter2(os);
    copy(array, array + 4, iter2);
    CPPUNIT_ASSERT(os.good());
    CPPUNIT_ASSERT(os.str() == "hello 1524");

    os << ' ';
    ostream_iterator<int> iter3(os, ",");
    copy(array, array + 4, iter3);
    CPPUNIT_ASSERT(os.good());
    CPPUNIT_ASSERT(os.str() == "hello 1524 1,5,2,4,");
}
