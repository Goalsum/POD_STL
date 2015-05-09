// 输入输出流测试用例文件

#include "string"
#include "sstream"
#include "iostream"
#include "cppunit_mini.h"

using namespace podstl;

// TestCase class
class IOStreamTest : public TestCase
{
    CPPUNIT_TEST_SUITE(IOStreamTest);
    CPPUNIT_TEST(manipulators);
    CPPUNIT_TEST_SUITE_END();

private:
    void manipulators();
};

CPPUNIT_TEST_SUITE_REGISTRATION(IOStreamTest);

// tests implementation
void IOStreamTest::manipulators()
{
    {
        istringstream istr;
        istr.str("bar");

        istr >> ws;
        CPPUNIT_ASSERT(istr.good());

        string foo;
        istr >> foo;
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(foo == "bar");

        istr >> ws;
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(!istr.fail());
        istr.clear();
    }

    {
        istringstream istr;
        istr.str("  bar  ");

        istr >> ws;
        CPPUNIT_ASSERT(istr.good());

        string foo;
        istr >> foo;
        CPPUNIT_ASSERT(!istr.eof());
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(foo == "bar");

        istr >> ws;
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(!istr.fail());
        istr.clear();
    }
}
