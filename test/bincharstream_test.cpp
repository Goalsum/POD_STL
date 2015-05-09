// 字符数组流的测试用例文件

#include "bincharstream.h"
#include "iomanip"
#include "cppunit_mini.h"

using namespace podstl;

// TestCase class
class bincharstream_test : public TestCase
{
    CPPUNIT_TEST_SUITE(bincharstream_test);
    CPPUNIT_TEST(output);
    CPPUNIT_TEST(input);
    CPPUNIT_TEST(err);
    CPPUNIT_TEST_SUITE_END();

protected:
    void output();
    void input();
    void err();
};

CPPUNIT_TEST_SUITE_REGISTRATION(bincharstream_test);

// tests implementation
void bincharstream_test::output()
{
    char buf[64] = { 0 };
    obincharstream s(buf, 64);
    s << static_cast<char>(1) << static_cast<short>(2) << static_cast<long>(0x12345678);
    CPPUNIT_ASSERT(s.good());
    CPPUNIT_ASSERT(buf[0] == 1);
    CPPUNIT_ASSERT(buf[1] == 2);
    CPPUNIT_ASSERT(buf[2] == 0);
    CPPUNIT_ASSERT(buf[3] == 0x78);
    CPPUNIT_ASSERT(buf[4] == 0x56);
    CPPUNIT_ASSERT(buf[5] == 0x34);
    CPPUNIT_ASSERT(buf[6] == 0x12);
}

void bincharstream_test::input()
{
    const char buf[64] = {0x78, 0x56, 0x34, 0x12, 0x0, 0x5, 0x3, 0x4};
    ibincharstream s(buf, 64);
    unsigned int i = 0;
    s >> i;
    CPPUNIT_ASSERT(s.good());
    CPPUNIT_ASSERT(i == 0x12345678);

    unsigned short c = 66;
    s >> c;
    CPPUNIT_ASSERT(s.good());
    CPPUNIT_ASSERT(c == 0x500);

    short sh;
    s >> sh;
    CPPUNIT_ASSERT(s.good());
    CPPUNIT_ASSERT(sh == 0x403);
    CPPUNIT_ASSERT(!s.eof());
}

void bincharstream_test::err()
{
    char buf[2] = {9, 7};
    bincharstream s(buf, 2);

    int i = 0;
    s >> i;
    CPPUNIT_ASSERT(s.fail());
    CPPUNIT_ASSERT(s.eof());
    CPPUNIT_ASSERT(i == 0x709);
    s.clear();

    s << 0x1020304;
    CPPUNIT_ASSERT(s.fail());
    CPPUNIT_ASSERT(s.bad());
    CPPUNIT_ASSERT(buf[0] == 4);
    CPPUNIT_ASSERT(buf[1] == 3);
}
