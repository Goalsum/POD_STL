// 字符数组流的测试用例文件

#include "charstream.h"
#include "iomanip"
#include "string"
#include "cppunit_mini.h"

using namespace podstl;

// TestCase class
class charstream_test : public TestCase
{
    CPPUNIT_TEST_SUITE(charstream_test);
    CPPUNIT_TEST(output);
    CPPUNIT_TEST(input);
    CPPUNIT_TEST(input_char);
    CPPUNIT_TEST(io);
    CPPUNIT_TEST(err);
    CPPUNIT_TEST(seek);
    CPPUNIT_TEST(seekp);
    CPPUNIT_TEST(seek_gp);
    CPPUNIT_TEST(tellp);
    CPPUNIT_TEST(user_delimeter);
    CPPUNIT_TEST_SUITE_END();

protected:
    void output();
    void input();
    void input_char();
    void io();
    void err();
    void seek();
    void seekp();
    void seek_gp();
    void tellp();
    void user_delimeter();
};

CPPUNIT_TEST_SUITE_REGISTRATION(charstream_test);

// tests implementation
void charstream_test::output()
{
    ocharstream s;
    s << 0;
    CPPUNIT_ASSERT(s.fail());
    CPPUNIT_ASSERT(s.buffer() == 0);
    s.clear();
    char buf[64] = { 0 };
    s.buffer(buf, 64);
    s << 1 << '\n' << 2 << '\n' << "abcd\n" << "ghk lm\n" << "abcd ef";
    CPPUNIT_ASSERT(s.good());
    CPPUNIT_ASSERT(strcmp(s.buffer(), "1\n2\nabcd\nghk lm\nabcd ef") == 0);
}

void charstream_test::input()
{
    {
        const char buf[] = "1\n2\nabcd\nghk lm\nabcd ef";
        icharstream s(buf, strlen(buf));
        int i = 0;
        s >> i;
        CPPUNIT_ASSERT(s.good());
        CPPUNIT_ASSERT(i == 1);
        double d = 0.0;
        s >> d;
        CPPUNIT_ASSERT(s.good());
        CPPUNIT_ASSERT(d == 2.0);
        string buffer;
        s >> buffer;
        CPPUNIT_ASSERT(s.good());
        CPPUNIT_ASSERT(buffer == "abcd");
        char c;
        s.get(c); // extract newline, that not extracted by operator >>
        CPPUNIT_ASSERT(s.good());
        CPPUNIT_ASSERT(c == '\n');
        getline(s, buffer);
        CPPUNIT_ASSERT(s.good());
        CPPUNIT_ASSERT(buffer == "ghk lm");
        getline(s, buffer);
        CPPUNIT_ASSERT(s.eof());
        CPPUNIT_ASSERT(buffer == "abcd ef");
    }
}

void charstream_test::input_char()
{
    char buf[16] = { 0, '1', '2', '3' };
    icharstream s("0", 2);
    s >> buf;

    CPPUNIT_ASSERT(buf[0] == '0');
    CPPUNIT_ASSERT(buf[1] == 0);
    CPPUNIT_ASSERT(buf[2] == '2');
}

void charstream_test::io()
{
    char buf[64] = { 0 };
    charstream s(buf, 64);
    s << 1 << '\n' << 2 << '\n' << "abcd\n" << "ghk lm\n" << "abcd ef";
    CPPUNIT_ASSERT(s.good());

    int i = 0;
    s >> i;
    CPPUNIT_ASSERT(i == 1);
    CPPUNIT_ASSERT(s.good());
    short d = 0;
    s >> d;
    CPPUNIT_ASSERT(d == 2);
    CPPUNIT_ASSERT(s.good());
    string buffer;
    s >> buffer;
    CPPUNIT_ASSERT(buffer == "abcd");
    CPPUNIT_ASSERT(s.good());
    char c;
    s.get(c); // extract newline, that not extracted by operator >>
    CPPUNIT_ASSERT(s.good());
    CPPUNIT_ASSERT(c == '\n');
    getline(s, buffer);
    CPPUNIT_ASSERT(s.good());
    CPPUNIT_ASSERT(buffer == "ghk lm");
    getline(s, buffer, '\0');
    CPPUNIT_ASSERT(buffer == "abcd ef");
    CPPUNIT_ASSERT(!s.eof());
}

void charstream_test::err()
{
    char buf[3] = "9";
    charstream s(buf, 2);

    int i = 0;
    s >> i;
    CPPUNIT_ASSERT(!s.fail());
    CPPUNIT_ASSERT(i == 9);
    s >> i;
    CPPUNIT_ASSERT(s.fail());
    CPPUNIT_ASSERT(s.eof());
    CPPUNIT_ASSERT(i == 9);
    s.clear();
    s << 12345;
    CPPUNIT_ASSERT(s.fail());
    CPPUNIT_ASSERT(s.bad());
    CPPUNIT_ASSERT(strcmp(s.buffer(), "12") == 0);
}

void charstream_test::seek()
{
    char buf[] = "0123456789";
    charstream s(buf, sizeof(buf));

    CPPUNIT_ASSERT(s.tellg() == charstream::pos_type(0));
    s.seekg(6, ios::beg);
    CPPUNIT_ASSERT(s.tellg() == charstream::pos_type(6));
    s.seekg(-3, ios::cur);
    CPPUNIT_ASSERT(s.tellg() == charstream::pos_type(3));

    icharstream is(buf, sizeof(buf));
    CPPUNIT_ASSERT(is.tellg() == charstream::pos_type(0));
    is.seekg(6, ios::beg);
    CPPUNIT_ASSERT(is.tellg() == charstream::pos_type(6));
    is.seekg(-3, ios::cur);
    CPPUNIT_ASSERT(is.tellg() == charstream::pos_type(3));
}

void charstream_test::seekp()
{
    char buf[32] = { 0 };
    ocharstream s(buf, 32);

    s << "1234567";
    CPPUNIT_CHECK(s.tellp() == charstream::pos_type(7));
    CPPUNIT_CHECK(strcmp(s.buffer(), "1234567") == 0);
    s.seekp(0);
    s << "X";
    CPPUNIT_CHECK(strcmp(s.buffer(), "X234567") == 0);
    s.seekp(0, ios::beg);
    s << "Y";
    CPPUNIT_CHECK(strcmp(s.buffer(), "Y234567") == 0);
}

void charstream_test::seek_gp()
{
    char buf[2] = "1";
    charstream ss(buf, 2);

    ss.seekg(0, ios::beg);
    ss.seekp(-1, ios::end);

    ss << "2";

    string buffer;

    ss >> buffer;

    CPPUNIT_CHECK(buffer == "12");
}

void charstream_test::tellp()
{
    {
        char buf[32] = "1";
        ocharstream o(buf, 32);
        o << "23456";
        CPPUNIT_CHECK(o.rdbuf()->pubseekoff(0, ios_base::cur, ios_base::out) == charstream::pos_type(5));
        CPPUNIT_CHECK(o.tellp() == charstream::pos_type(5));
    }
    {
        char buf[32];
        ocharstream o(buf, 32);
        o << "123456";
        CPPUNIT_CHECK(o.rdbuf()->pubseekoff(0, ios_base::cur, ios_base::out) == charstream::pos_type(6));
        CPPUNIT_CHECK(o.tellp() == charstream::pos_type(6));
    }
    {
        char buf[32] = "1";
        ocharstream o(buf, 32);
        o << "23456789";
        CPPUNIT_CHECK(o.rdbuf()->pubseekoff(0, ios_base::cur, ios_base::out) == charstream::pos_type(8));
        CPPUNIT_CHECK(o.tellp() == charstream::pos_type(8));
    }
}

void charstream_test::user_delimeter()
{
    const char buf[] = "12, -3,0x456789, -07";
    icharstream is(buf, strlen(buf));
    is >> resetiosflags(ios::basefield);
    is.add_delimeter(',');
    int x;
    short y;
    long z;
    int w;
    is >> x >> y >> z >> w;
    CPPUNIT_CHECK(is.eof());
    CPPUNIT_CHECK(!is.fail());
    CPPUNIT_CHECK(x == 12);
    CPPUNIT_CHECK(y == -3);
    CPPUNIT_CHECK(z == 0x456789);
    CPPUNIT_CHECK(w == -7);
}