// string≤‚ ‘”√¿˝Œƒº˛

#include "vector"
#include "string"
#include "algorithm"
#include "cppunit_mini.h"

using namespace podstl;

// TestCase class
class StringTest : public TestCase
{
    CPPUNIT_TEST_SUITE(StringTest);
    CPPUNIT_TEST(trivial_char_compare);
    CPPUNIT_TEST(short_string);
    CPPUNIT_TEST(erase);
    CPPUNIT_TEST(data);
    CPPUNIT_TEST(c_str);
    CPPUNIT_TEST(null_char);
    CPPUNIT_TEST(insert);
    CPPUNIT_TEST(replace);
    CPPUNIT_TEST(resize);
    CPPUNIT_TEST(find);
    CPPUNIT_TEST(bogus_edge_find);
    CPPUNIT_TEST(rfind);
    CPPUNIT_TEST(find_last_of);
    CPPUNIT_TEST(find_last_not_of);
    CPPUNIT_TEST(copy);
    CPPUNIT_TEST(assign);
    CPPUNIT_TEST(short_string_optim_bug);
    CPPUNIT_TEST(compare);
    CPPUNIT_TEST(template_expression);
    CPPUNIT_TEST(oper_tmp);
    CPPUNIT_TEST(capacity);
    CPPUNIT_TEST(concat24);
    CPPUNIT_TEST_SUITE_END();

protected:
    void trivial_char_compare();
    void short_string();
    void erase();
    void data();
    void c_str();
    void null_char();
    void insert();
    void replace();
    void resize();
    void find();
    void bogus_edge_find();
    void rfind();
    void find_last_of();
    void find_last_not_of();
    void copy();
    void assign();
    void short_string_optim_bug();
    void compare();
    void template_expression();
    void oper_tmp();
    void capacity();
    void concat24();
};

CPPUNIT_TEST_SUITE_REGISTRATION(StringTest);

void StringTest::trivial_char_compare()
{
    string s("message");

    CPPUNIT_CHECK(s == "message");
    CPPUNIT_CHECK("message" == s);
}

void StringTest::short_string()
{
    string const ref_short_str1("str1"), ref_short_str2("str2");
    string short_str1(ref_short_str1), short_str2(ref_short_str2);
    string const ref_long_str1("str                                                  1");
    string const ref_long_str2("str                                                  2");
    string long_str1(ref_long_str1), long_str2(ref_long_str2);

    CPPUNIT_ASSERT(short_str1 == ref_short_str1);
    CPPUNIT_ASSERT(long_str1 == ref_long_str1);

    {
        string str1(short_str1);
        str1 = long_str1;
        CPPUNIT_ASSERT(str1 == ref_long_str1);
    }

    {
        string str1(long_str1);
        str1 = short_str1;
        CPPUNIT_ASSERT(str1 == ref_short_str1);
    }

    {
        short_str1.swap(short_str2);
        CPPUNIT_ASSERT((short_str1 == ref_short_str2) && (short_str2 == ref_short_str1));
        short_str1.swap(short_str2);
    }

    {
        long_str1.swap(long_str2);
        CPPUNIT_ASSERT((long_str1 == ref_long_str2) && (long_str2 == ref_long_str1));
        long_str1.swap(long_str2);
    }

    {
        short_str1.swap(long_str1);
        CPPUNIT_ASSERT((short_str1 == ref_long_str1) && (long_str1 == ref_short_str1));
        short_str1.swap(long_str1);
    }

    {
        long_str1.swap(short_str1);
        CPPUNIT_ASSERT((short_str1 == ref_long_str1) && (long_str1 == ref_short_str1));
        long_str1.swap(short_str1);
    }

    {
        string a(256, 'a');
        string b(256, 'b');
        const char* as = a.c_str();
        const char* bs = b.c_str();
        swap(a, b);
        CPPUNIT_ASSERT(a.c_str() == bs);
        CPPUNIT_ASSERT(b.c_str() == as);
    }

    {
        //This is to test move constructor
        vector<string> str_vect(4);
        str_vect[0] = short_str1;
        str_vect[1] = long_str1;
        str_vect[2] = short_str2;
        str_vect[3] = long_str2;
        CPPUNIT_ASSERT((str_vect[0] == ref_short_str1) &&
                       (str_vect[1] == ref_long_str1) &&
                       (str_vect[2] == ref_short_str2) &&
                       (str_vect[3] == ref_long_str2));
    }
}

void StringTest::erase()
{
    char const* c_str = "Hello, World!";
    string str(c_str);
    CPPUNIT_ASSERT(str == c_str);

    str.erase(str.begin() + 1, str.end() - 1); // Erase all but first and last.

    size_t i;
    for (i = 0; i < str.size(); ++i) 
    {
        switch (i) 
        {
        case 0:
            CPPUNIT_ASSERT(str[i] == 'H');
            break;
        case 1:
            CPPUNIT_ASSERT(str[i] == '!');
            break;
        default:
            CPPUNIT_FAIL;
        }
    }

    str.insert(1, c_str);
    str.erase(str.begin()); // Erase first element.
    str.erase(str.end() - 1); // Erase last element.
    CPPUNIT_ASSERT(str == c_str);
    str.clear(); // Erase all.
    CPPUNIT_ASSERT(str.empty());

    str = c_str;
    CPPUNIT_ASSERT(str == c_str);

    str.erase(1, str.size() - 1); // Erase all but first and last.
    for (i = 0; i < str.size(); i++) 
    {
        switch (i) 
        {
        case 0:
            CPPUNIT_ASSERT(str[i] == 'H');
            break;
        case 1:
            CPPUNIT_ASSERT(str[i] == '!');
            break;
        default:
            CPPUNIT_FAIL;
        }
    }

    str.erase(1);
    CPPUNIT_ASSERT(str == "H");
}

void StringTest::data()
{
    string xx;

    CPPUNIT_ASSERT( xx.data() != 0 );  // ISO-IEC-14882:1998(E), 21.3.6, paragraph 3

    string low( "2004-01-01" );
    xx = "123456";
    xx += low;
    CPPUNIT_ASSERT(strcmp(xx.data(), "1234562004-01-01") == 0);

    xx = "1234";
    xx += ";";
    CPPUNIT_ASSERT(strcmp(xx.data(), "1234;") == 0);
}

void StringTest::c_str()
{
    string low( "2004-01-01" );
    string xx;
    string yy;

    CPPUNIT_ASSERT( *(yy.c_str()) == '\0' ); // ISO-IEC-14882:1998(E), 21.3.6, paragraph 1

    // Blocks A and B should follow each other.
    // Block A:
    xx = "123456";
    xx += low;
    CPPUNIT_ASSERT( strcmp( xx.c_str(), "1234562004-01-01" ) == 0 );
    // End of block A

    // Block B:
    xx = "1234";
    xx += ";";
    CPPUNIT_ASSERT( strcmp( xx.c_str(), "1234;" ) == 0 );
    // End of block B
}

void StringTest::null_char()
{
    // ISO/IEC 14882:1998(E), ISO/IEC 14882:2003(E), 21.3.4 ('... the const version')
    const string s( "123456" );
    CPPUNIT_CHECK( s[s.size()] == '\0' );
}

void StringTest::insert()
{
    string strorg = "This is test string for string calls";
    string str;
    
    // In case of reallocation there is no auto reference problem
    // so we reserve a big enough string to be sure to test this
    // particular point.
    str.reserve(100);
    str = strorg;

    //test self insertion:
    str.insert(10, str.c_str() + 5, 15);
    CPPUNIT_ASSERT( str == "This is teis test string st string for string calls" );

    str = strorg;
    str.insert(15, str.c_str() + 5, 25);
    CPPUNIT_ASSERT( str == "This is test stis test string for stringring for string calls" );

    str = strorg;
    str.insert(0, str.c_str() + str.size() - 4, 4);
    CPPUNIT_ASSERT( str == "allsThis is test string for string calls" );

    str = strorg;
    str.insert(0, str.c_str() + str.size() / 2 - 1, str.size() / 2 + 1);
    CPPUNIT_ASSERT( str == "ng for string callsThis is test string for string calls" );

    str = strorg;
    string::iterator b = str.begin();
    string::const_iterator s = str.begin() + str.size() / 2 - 1;
    string::const_iterator e = str.end();
    str.insert( b, s, e );
    CPPUNIT_ASSERT( str == "ng for string callsThis is test string for string calls" );

    str = strorg;
    str.insert(str.begin(), str.begin() + str.size() / 2 - 1, str.end());
    CPPUNIT_ASSERT( str == "ng for string callsThis is test string for string calls" );

    vector<int> int_vect;

    //Just a compile time test:
    str.insert(str.end(), int_vect.begin(), int_vect.end());

    string str0;
    str0.insert(str0.begin(), 5, '0');
    CPPUNIT_ASSERT( str0 == "00000" );

    string str1;
    {
        string::size_type pos = 0, nb = 2;
        str1.insert(pos, nb, '1');
    }
    CPPUNIT_ASSERT( str1 == "11" );

    str0.insert(0, str1);
    CPPUNIT_ASSERT( str0 == "1100000" );

    string str2("2345");
    str0.insert(str0.size(), str2, 1, 2);
    CPPUNIT_ASSERT( str0 == "110000034" );

    str1.insert(str1.begin() + 1, 2, '2');
    CPPUNIT_ASSERT( str1 == "1221" );

    str1.insert(2, "333333", 3);
    CPPUNIT_ASSERT( str1 == "1233321" );

    str1.insert(4, "4444");
    CPPUNIT_ASSERT( str1 == "12334444321" );

    str1.insert(str1.begin() + 6, '5');
    CPPUNIT_ASSERT( str1 == "123344544321" );
}

void StringTest::replace()
{
    // This test case is for the non template string::replace method,
    // this is why we play with the const iterators and reference to guaranty
    // that the right method is called.
    const string v( "78" );
    string s( "123456" );
    string const& cs = s;

    string::iterator i = s.begin() + 1;
    s.replace(i, i + 3, v.begin(), v.end());
    CPPUNIT_ASSERT( s == "17856" );

    s = "123456";
    i = s.begin() + 1;
    s.replace(i, i + 1, v.begin(), v.end());
    CPPUNIT_ASSERT( s == "1783456" );

    s = "123456";
    i = s.begin() + 1;
    string::const_iterator ci = s.begin() + 1;
    s.replace(i, i + 3, ci + 3, cs.end());
    CPPUNIT_ASSERT( s == "15656" );

    s = "123456";
    i = s.begin() + 1;
    ci = s.begin() + 1;
    s.replace(i, i + 3, ci, ci + 2);
    CPPUNIT_ASSERT( s == "12356" );

    s = "123456";
    i = s.begin() + 1;
    ci = s.begin() + 1;
    s.replace(i, i + 3, ci + 1, cs.end());
    CPPUNIT_ASSERT( s == "1345656" );

    s = "123456";
    i = s.begin();
    ci = s.begin() + 1;
    s.replace(i, i, ci, ci + 1);
    CPPUNIT_CHECK( s == "2123456" );

    s = "123456";
    s.replace(s.begin() + 4, s.end(), cs.begin(), cs.end());
    CPPUNIT_ASSERT( s == "1234123456" );

    // This is the test for the template replace method.
    s = "123456";
    string::iterator b = s.begin() + 4;
    string::iterator e = s.end();
    string::const_iterator rb = s.begin();
    string::const_iterator re = s.end();
    s.replace(b, e, rb, re);
    CPPUNIT_ASSERT( s == "1234123456" );

    s = "123456";
    s.replace(s.begin() + 4, s.end(), s.begin(), s.end());
    CPPUNIT_ASSERT( s == "1234123456" );

    string strorg("This is test string for string calls");
    string str = strorg;
    str.replace(5, 15, str.c_str(), 10);
    CPPUNIT_ASSERT( str == "This This is tefor string calls" );

    str = strorg;
    str.replace(5, 5, str.c_str(), 10);
    CPPUNIT_ASSERT( str == "This This is test string for string calls" );
}

void StringTest::resize()
{
    string s;
    s.resize(0);
    CPPUNIT_ASSERT( *s.c_str() == 0 );

    s = "1234567";
    s.resize(0);
    CPPUNIT_ASSERT( *s.c_str() == 0 );

    s = "1234567";
    s.resize(1);
    CPPUNIT_ASSERT( s.size() == 1 );
    CPPUNIT_ASSERT( *s.c_str() == '1' );
    CPPUNIT_ASSERT( *(s.c_str() + 1) == 0 );

    s = "1234567";
    s.resize(10);
    CPPUNIT_ASSERT( s.size() == 10 );
    CPPUNIT_ASSERT( s[6] == '7' );
    CPPUNIT_ASSERT( s[7] == 0 );
}

void StringTest::find()
{
    string s("one two three one two three");
    CPPUNIT_ASSERT( s.find("one") == 0 );
    CPPUNIT_ASSERT( s.find('t') == 4 );
    CPPUNIT_ASSERT( s.find('t', 5) == 8 );
    CPPUNIT_ASSERT(s.find("four") == string::npos);
    CPPUNIT_ASSERT(s.find("one", string::npos) == string::npos);
    CPPUNIT_ASSERT( s.find_first_of("abcde") == 2 );
    CPPUNIT_ASSERT( s.find_first_not_of("enotw ") == 9 );

    string empty;
    CPPUNIT_ASSERT( s.substr(s.find(empty), empty.size()) == empty );
}

void StringTest::bogus_edge_find()
{
    {
        string s;
        string::size_type p = s.find( "", 0, 0 );
        CPPUNIT_CHECK( p == 0 ); // bogus result, isn't it?
    }
    {
        string s( "123" );
        string::size_type p = s.find( "", 0, 0 );
        CPPUNIT_CHECK( p == 0 );
    }
    {
        string s( "123" );
        string::size_type p = s.find( "", 1, 0 );
        CPPUNIT_CHECK( p == 1 );
    }
    {
        string s( "" );
        string::size_type p = s.find( "", 1, 0 );
        CPPUNIT_CHECK( p == string::npos );
    }
    {
        string s( "123" );
        string::size_type p = s.find( "", 3, 0 );
        CPPUNIT_CHECK( p == 3 ); // bogus result, isn't it?
    }
    {
        string s;
        string::size_type p = s.rfind( "", 0, 0 );
        CPPUNIT_CHECK( p == 0 ); // bogus result, isn't it?
    }
    {
        string s( "123" );
        string::size_type p = s.rfind( "", 0, 0 );
        CPPUNIT_CHECK( p == 0 );
    }
    {
        string s( "123" );
        string::size_type p = s.rfind( "", 1, 0 );
        CPPUNIT_CHECK( p == 1 );
    }
    {
        string s( "" );
        string::size_type p = s.rfind( "", 1, 0 );
        CPPUNIT_CHECK( p == 0 ); // bogus result, isn't it?
    }
    {
        string s( "123" );
        string::size_type p = s.rfind( "", 3, 0 );
        CPPUNIT_CHECK( p == 3 ); // bogus result, isn't it?
    }
}

void StringTest::rfind()
{
    // 21.3.6.2
    string s("one two three one two three");

    CPPUNIT_ASSERT( s.rfind("two") == 18 );
    CPPUNIT_ASSERT( s.rfind("two", 0) == string::npos );
    CPPUNIT_ASSERT( s.rfind("two", 11) == 4 );
    CPPUNIT_ASSERT( s.rfind('w') == 19 );

    string test( "aba" );

    CPPUNIT_CHECK( test.rfind( "a", 2, 1 ) == 2 );
    CPPUNIT_CHECK( test.rfind( "a", 1, 1 ) == 0 );
    CPPUNIT_CHECK( test.rfind( "a", 0, 1 ) == 0 );

    CPPUNIT_CHECK( test.rfind( 'a', 2 ) == 2 );
    CPPUNIT_CHECK( test.rfind( 'a', 1 ) == 0 );
    CPPUNIT_CHECK( test.rfind( 'a', 0 ) == 0 );
}

void StringTest::find_last_of()
{
    // 21.3.6.4
    string s("one two three one two three");
    CPPUNIT_ASSERT( s.find_last_of("abcde") == 26 );

    string test( "aba" );
    CPPUNIT_CHECK( test.find_last_of( "a", 2, 1 ) == 2 );
    CPPUNIT_CHECK( test.find_last_of( "a", 1, 1 ) == 0 );
    CPPUNIT_CHECK( test.find_last_of( "a", 0, 1 ) == 0 );

    CPPUNIT_CHECK( test.find_last_of( 'a', 2 ) == 2 );
    CPPUNIT_CHECK( test.find_last_of( 'a', 1 ) == 0 );
    CPPUNIT_CHECK( test.find_last_of( 'a', 0 ) == 0 );
}

void StringTest::find_last_not_of()
{
    // 21.3.6.6
    string s("one two three one two three");

    CPPUNIT_ASSERT( s.find_last_not_of("ehortw ") == 15 );

    string test( "aba" );

    CPPUNIT_CHECK( test.find_last_not_of( "a", 2, 1 ) == 1 );
    CPPUNIT_CHECK( test.find_last_not_of( "b", 2, 1 ) == 2 );
    CPPUNIT_CHECK( test.find_last_not_of( "a", 1, 1 ) == 1 );
    CPPUNIT_CHECK( test.find_last_not_of( "b", 1, 1 ) == 0 );
    CPPUNIT_CHECK( test.find_last_not_of( "a", 0, 1 ) == string::npos );
    CPPUNIT_CHECK( test.find_last_not_of( "b", 0, 1 ) == 0 );

    CPPUNIT_CHECK( test.find_last_not_of( 'a', 2 ) == 1 );
    CPPUNIT_CHECK( test.find_last_not_of( 'b', 2 ) == 2 );
    CPPUNIT_CHECK( test.find_last_not_of( 'a', 1 ) == 1 );
    CPPUNIT_CHECK( test.find_last_not_of( 'b', 1 ) == 0 );
    CPPUNIT_CHECK( test.find_last_not_of( 'a', 0 ) == string::npos );
    CPPUNIT_CHECK( test.find_last_not_of( 'b', 0 ) == 0 );
}

void StringTest::copy()
{
    string s("foo");
    char dest[4];
    dest[0] = dest[1] = dest[2] = dest[3] = 1;
    CPPUNIT_ASSERT(s.copy(dest, 4) == 3);
    int pos = 0;
    CPPUNIT_ASSERT( dest[pos++] == 'f' );
    CPPUNIT_ASSERT( dest[pos++] == 'o' );
    CPPUNIT_ASSERT( dest[pos++] == 'o' );
    CPPUNIT_ASSERT( dest[pos++] == 1 );

    dest[0] = dest[1] = dest[2] = dest[3] = 1;
    CPPUNIT_ASSERT(s.copy(dest, 4, 2) == 1);
    pos = 0;
    CPPUNIT_ASSERT( dest[pos++] == 'o' );
    CPPUNIT_ASSERT( dest[pos++] == 1 );
}

void StringTest::assign()
{
    string s;
    char const* cstr = "test string for assign";

    s.assign(cstr, cstr + 22);
    CPPUNIT_ASSERT( s == "test string for assign" );

    string s2("other test string");
    s.assign(s2);
    CPPUNIT_ASSERT( s == s2 );

    string str1;
    string str2;

    str1 = "123456";
    str2 = "1234567890123456789012345678901234567890";

    CPPUNIT_ASSERT(str1[5] == '6');
    CPPUNIT_ASSERT(str2[29] == '0');
}

void StringTest::short_string_optim_bug()
{
    string teststr("shortest");
    bool short_string_optim_bug_helper(string teststr);
    CPPUNIT_ASSERT(true == short_string_optim_bug_helper(teststr));
}

bool short_string_optim_bug_helper(string teststr)
{
    size_t ss = teststr.size();
    return (ss == 8);
}

void StringTest::compare()
{
    string str1("abcdef");
    string str2;

    str2 = "abcdef";
    CPPUNIT_ASSERT( str1.compare(str2) == 0 );
    str2 = "abcde";
    CPPUNIT_ASSERT( str1.compare(str2) > 0 );
    str2 = "abcdefg";
    CPPUNIT_ASSERT( str1.compare(str2) < 0 );

    CPPUNIT_ASSERT( str1.compare("abcdef") == 0 );
    CPPUNIT_ASSERT( str1.compare("abcde") > 0 );
    CPPUNIT_ASSERT( str1.compare("abcdefg") < 0 );

    str2 = "cde";
    CPPUNIT_ASSERT( str1.compare(2, 3, str2) == 0 );
    str2 = "cd";
    CPPUNIT_ASSERT( str1.compare(2, 3, str2) > 0 );
    str2 = "cdef";
    CPPUNIT_ASSERT( str1.compare(2, 3, str2) < 0 );

    str2 = "abcdef";
    CPPUNIT_ASSERT( str1.compare(2, 3, str2, 2, 3) == 0 );
    CPPUNIT_ASSERT( str1.compare(2, 3, str2, 2, 2) > 0 );
    CPPUNIT_ASSERT( str1.compare(2, 3, str2, 2, 4) < 0 );

    CPPUNIT_ASSERT( str1.compare(2, 3, "cdefgh", 3) == 0 );
    CPPUNIT_ASSERT( str1.compare(2, 3, "cdefgh", 2) > 0 );
    CPPUNIT_ASSERT( str1.compare(2, 3, "cdefgh", 4) < 0 );
}

void StringTest::template_expression()
{
    string one("one"), two("two"), three("three");
    string space(1, ' ');

    // check availability of [un]equality operators
    {
        // string-string
        one == two;
        one != two;
        // string-literal
        one == "two";
        one != "two";
        // literal-string
        "one" == two;
        "one" != two;
        // strsum-string
        (one + two) == three;
        (one + two) != three;
        // string-strsum
        one == (two + three);
        one != (two + three);
        // strsum-literal
        (one + two) == "three";
        (one + two) != "three";
        // literal-strsum
        "one" == (two + three);
        "one" != (two + three);
        // strsum-strsum
        (one + two) == (two + three);
        (one + two) != (two + three);
    }

    {
        string result(one + ' ' + two + ' ' + three);
        CPPUNIT_CHECK( result == "one two three" );
    }

    {
        string result(one + ' ' + two + ' ' + three, 4);
        CPPUNIT_CHECK( result == "two three" );
    }

    {
        string result(one + ' ' + two + ' ' + three, 4, 3);
        CPPUNIT_CHECK( result == "two" );
    }

    //2 members expressions:
    CPPUNIT_CHECK( (' ' + one) == " one" );
    CPPUNIT_CHECK( (one + ' ') == "one " );
    CPPUNIT_CHECK( (one + " two") == "one two" );
    CPPUNIT_CHECK( ("one " + two) == "one two" );
    CPPUNIT_CHECK( (one + space) == "one " );

    //3 members expressions:
    CPPUNIT_CHECK( ((one + space) + "two") == "one two" );
    CPPUNIT_CHECK( ("one" + (space + two)) == "one two" );
    CPPUNIT_CHECK( ((one + space) + two) == "one two" );
    CPPUNIT_CHECK( (one + (space + two)) == "one two" );
    CPPUNIT_CHECK( ((one + space) + 't') == "one t" );
    CPPUNIT_CHECK( ('o' + (space + two)) == "o two" );

    //4 members expressions:
    CPPUNIT_CHECK( ((one + space) + (two + space)) == "one two " );

    //special operators
    {
        string result;
        result = one + space + two;
        CPPUNIT_CHECK( result == "one two" );

        result += space + three;
        CPPUNIT_CHECK( result == "one two three" );
    }

    //special append method
    {
        string result;

        //Use reserve to avoid reallocation and really test auto-referencing problems:
        result.reserve(64);

        result.append(one + space + two);
        CPPUNIT_CHECK( result == "one two" );

        result.append(space + result + space + three);
        CPPUNIT_CHECK( result == "one two one two three" );

        result = "one two";
        result.append(space + three, 1, 2);
        CPPUNIT_ASSERT( result == "one twoth" );

        result.append(space + result);
        CPPUNIT_CHECK( result == "one twoth one twoth" );
    }

    //special assign method
    {
        string result;

        //Use reserve to avoid reallocation and really test auto-referencing problems:
        result.reserve(64);

        result.assign(one + space + two + space + three);
        CPPUNIT_CHECK( result == "one two three" );

        result.assign(one + space + two + space + three, 3, 5);
        CPPUNIT_CHECK( result == " two " );

        result.assign(one + result + three);
        CPPUNIT_CHECK( result == "one two three" );
    }

    {
        CPPUNIT_CHECK( !(one + ' ' + two).empty() );

        char result = (one + ' ' + two)[3];
        CPPUNIT_CHECK( result == ' ' );

        result = (one + ' ' + two).at(3);
        CPPUNIT_CHECK( result == ' ' );
    }
}

class mypath
{
public:
    mypath( const string& s ) : p( s ) {}

    const mypath& operator / ( const string& );
    const string& str() const { return p; }
 
private:
    string p;
};

const mypath& mypath::operator /( const string& s )
{
    p += '/';
    p += s;
    return *this;
}

void StringTest::oper_tmp()
{
    string s1( "path1" );
    string s2( ".ext" );

    string& rs1 = s1;
    string& rs2 = s2;

    CPPUNIT_CHECK( (mypath( string( "/root" ) ) / (rs1 + rs2)).str() == "/root/path1.ext" );
}

void StringTest::capacity()
{
    string s;

    CPPUNIT_CHECK( s.capacity() >= 0 );
    CPPUNIT_CHECK( s.capacity() < s.max_size() );
    CPPUNIT_CHECK( s.capacity() >= s.size() );

    for ( int i = 0; i < 16 + 2; ++i ) 
    {
        s += ' ';
        CPPUNIT_CHECK( s.capacity() > 0 );
        CPPUNIT_CHECK( s.capacity() < s.max_size() );
        CPPUNIT_CHECK( s.capacity() >= s.size() );
    }
}

void StringTest::concat24()
{
    string s = string( "123456789012345678901234" ) + string( "123456789012345678901234" );

    CPPUNIT_CHECK( s.length() == 48 );
    CPPUNIT_CHECK( s[23] == '4' );
    CPPUNIT_CHECK( s[24] == '1' );
    CPPUNIT_CHECK( s[47] == '4' );
}
