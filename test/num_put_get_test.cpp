// 对数字输入输出的测试用例文件

#include "limits"
#include "iomanip"
#include "string"
#include "sstream"
#include "cppunit_mini.h"

using namespace podstl;

// TestCase class
class NumPutGetTest : public TestCase
{
    CPPUNIT_TEST_SUITE(NumPutGetTest);
    //CPPUNIT_TEST(num_put_float);
    CPPUNIT_TEST(num_put_integer);
    //CPPUNIT_TEST(num_get_float);
    CPPUNIT_TEST(num_get_integer);
    CPPUNIT_TEST(inhex);
    CPPUNIT_TEST(pointer);
    //CPPUNIT_TEST(fix_float_long);
    CPPUNIT_TEST_SUITE_END();

private:
    void num_put_float();
    void num_put_integer();
    void num_get_float();
    void num_get_integer();
    void inhex();
    void pointer();
    void fix_float_long();

    static bool check_float(float val, float ref)
    {
        float epsilon = numeric_limits<float>::epsilon();
        return val <= ref + epsilon && val >= ref - epsilon;
    }

    static bool check_double(double val, double ref)
    {
        double epsilon = numeric_limits<double>::epsilon();
        return val <= ref + epsilon && val >= ref - epsilon;
    }

    static string reset_stream(ostringstream &ostr)
    {
        string tmp = ostr.str();
        ostr.str("");
        return tmp;
    }

    template <class F>
    void check_get_float(F v)
    {
        F in_val_d = v;
        typedef numeric_limits<F> limits;
        {
            stringstream str;

            str << "1E+" << limits::max_exponent10;

            str >> in_val_d;
            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(str.eof());
            CPPUNIT_CHECK(in_val_d == in_val_d);
            CPPUNIT_CHECK(in_val_d != limits::infinity());
        }
        {
            stringstream str;

            str << "-1E+" << limits::max_exponent10;

            str >> in_val_d;
            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(str.eof());
            CPPUNIT_CHECK(in_val_d == in_val_d);
            CPPUNIT_CHECK(in_val_d != -limits::infinity());
        }
        {
            stringstream str;

            str << "1E" << limits::min_exponent10;

            str >> in_val_d;
            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(str.eof());
            CPPUNIT_CHECK(in_val_d == in_val_d);
            CPPUNIT_CHECK(in_val_d != F(0.0));
        }
        {
            stringstream str;

            str << "1E+" << (limits::max_exponent10 + 1);

            str >> in_val_d;
            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(str.eof());
            CPPUNIT_CHECK(in_val_d == in_val_d);
            CPPUNIT_CHECK(in_val_d == limits::infinity());
        }
        {
            stringstream str;

            str << "-1E+" << (limits::max_exponent10 + 1);

            str >> in_val_d;
            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(str.eof());
            CPPUNIT_CHECK(in_val_d == in_val_d);
            CPPUNIT_CHECK(in_val_d == -limits::infinity());
        }
        {
            stringstream str;

            str << "1E" << (limits::min_exponent10 - 1);

            str >> in_val_d;
            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(str.eof());
            CPPUNIT_CHECK(in_val_d == in_val_d);
            CPPUNIT_CHECK(in_val_d >= F(0.0) && in_val_d <= limits::min());
        }
        {
            stringstream str;

            str << limits::max();

            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_CHECK(str.str() != "inf");
            CPPUNIT_CHECK(str.str() != "-inf");
            CPPUNIT_CHECK(str.str() != "nan");
            CPPUNIT_CHECK(str.str() != "-nan");
            //CPPUNIT_MESSAGE( str.str().c_str() );

            //str.str("");
            //str << limits::max_exponent10;
            //CPPUNIT_MESSAGE( str.str().c_str() );

            str >> in_val_d;

            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(str.eof());
            CPPUNIT_CHECK(in_val_d == in_val_d);
            CPPUNIT_CHECK(in_val_d != limits::infinity());
        }
        {
            stringstream str;

            str << fixed << limits::max();

            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_CHECK(str.str() != "inf");
            CPPUNIT_CHECK(str.str() != "-inf");
            CPPUNIT_CHECK(str.str() != "nan");
            CPPUNIT_CHECK(str.str() != "-nan");
            //CPPUNIT_MESSAGE( str.str().c_str() );

            //str.str("");
            //str << limits::max_exponent10;
            //CPPUNIT_MESSAGE( str.str().c_str() );

            str >> in_val_d;

            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(str.eof());
            CPPUNIT_CHECK(in_val_d == in_val_d);
            CPPUNIT_CHECK(in_val_d != limits::infinity());
        }
        {
            stringstream str;

            str << scientific << setprecision(50) << limits::max();

            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_CHECK(str.str() != "inf");
            CPPUNIT_CHECK(str.str() != "-inf");
            CPPUNIT_CHECK(str.str() != "nan");
            CPPUNIT_CHECK(str.str() != "-nan");
            //CPPUNIT_MESSAGE( str.str().c_str() );

            //str.str("");
            //str << limits::max_exponent10;
            //CPPUNIT_MESSAGE( str.str().c_str() );

            str >> in_val_d;

            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(str.eof());
            CPPUNIT_CHECK(in_val_d == in_val_d);
            CPPUNIT_CHECK(in_val_d != limits::infinity());
        }
        {
            stringstream str;

            str << limits::infinity();

            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(!limits::has_infinity || str.str() == "inf");
        }
        {
            stringstream str;

            str << -limits::infinity();

            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(!limits::has_infinity || str.str() == "-inf");
        }
        {
            stringstream str;

            str << limits::quiet_NaN();

            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(!limits::has_quiet_NaN || str.str() == "nan");
        }
        {
            stringstream str;

            str << -limits::quiet_NaN();

            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(!limits::has_quiet_NaN || str.str() == "-nan");
        }
        {
            stringstream str;

            str << "0." << string(limits::max_exponent10, '0') << "1e" << (limits::max_exponent10 + 1);
            CPPUNIT_ASSERT(!str.fail());

            str >> in_val_d;
            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(str.eof());
            CPPUNIT_CHECK(in_val_d == 1);
        }
        {
            stringstream str;

            str << "1" << string(-(limits::min_exponent10 - 1), '0') << "e" << (limits::min_exponent10 - 1);
            CPPUNIT_ASSERT(!str.fail());

            str >> in_val_d;
            CPPUNIT_ASSERT(!str.fail());
            CPPUNIT_ASSERT(str.eof());
            CPPUNIT_CHECK(in_val_d == 1);
        }
        // The following tests are showing that simply changing stream
        // precision lead to different result. Do not seems to be a real
        // problem, simply rounding approximation but additional study should
        // be done after 5.2 release.
        {
            stringstream str;
            str << setprecision(limits::digits10 + 2) << limits::max();

            CPPUNIT_MESSAGE(str.str().c_str());
            CPPUNIT_ASSERT( !str.fail() );

            F val;
            str >> val;

            CPPUNIT_ASSERT( !str.fail() );
            CPPUNIT_ASSERT( limits::infinity() > val );
        }
        {
            stringstream str;
            str << setprecision(limits::digits10 + 1) << limits::max();

            CPPUNIT_MESSAGE(str.str().c_str());
            CPPUNIT_ASSERT( !str.fail() );

            F val;
            str >> val;

            CPPUNIT_ASSERT( !str.fail() );
            CPPUNIT_ASSERT( limits::infinity() > val );
        }
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(NumPutGetTest);

// tests implementation
void NumPutGetTest::num_put_float()
{
    {
        string output, digits;

        {
            ostringstream ostr;
            ostr << 1.23457e+17f;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            digits = "17";
            CPPUNIT_CHECK(output == string("1.23457e+") + digits);
        }

        {
            ostringstream ostr;
            ostr << setprecision(200) << 1.23457e+17f;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            CPPUNIT_CHECK(output.size() < 200);
        }

        {
            ostringstream ostr;
            ostr << setprecision(200) << numeric_limits<float>::min();
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            CPPUNIT_CHECK(output.size() < 200);
        }

        {
            ostringstream ostr;
            ostr << fixed << 1.23457e+17f;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            CPPUNIT_CHECK(output.size() == 25);
            CPPUNIT_CHECK(output.substr(0, 5) == "12345");
            CPPUNIT_CHECK(output.substr(18) == ".000000");
        }

        {
            ostringstream ostr;
            ostr << fixed << showpos << 1.23457e+17f;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            CPPUNIT_CHECK(output.size() == 26);
            CPPUNIT_CHECK(output.substr(0, 6) == "+12345");
            CPPUNIT_CHECK(output.substr(19) == ".000000");
        }

        {
            ostringstream ostr;
            ostr << fixed << showpos << setprecision(100) << 1.23457e+17f;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            CPPUNIT_CHECK(output.size() == 120);
            CPPUNIT_CHECK(output.substr(0, 6) == "+12345");
            CPPUNIT_CHECK(output.substr(19) == ".0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
        }

        {
            ostringstream ostr;
            ostr << scientific << setprecision(8) << 0.12345678f;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            digits = "01";
            CPPUNIT_CHECK(output == string("1.23456780e-") + digits);
        }

        {
            ostringstream ostr;
            ostr << fixed << setprecision(8) << setw(30) << setfill('0') << 0.12345678f;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            CPPUNIT_CHECK(output == "000000000000000000000.12345678");
        }

        {
            ostringstream ostr;
            ostr << fixed << showpos << setprecision(8) << setw(30) << setfill('0') << 0.12345678f;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            CPPUNIT_CHECK(output == "0000000000000000000+0.12345678");
        }

        {
            ostringstream ostr;
            ostr << fixed << showpos << setprecision(8) << setw(30) << left << setfill('0') << 0.12345678f;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            CPPUNIT_CHECK(output == "+0.123456780000000000000000000");
        }

        {
            ostringstream ostr;
            ostr << fixed << showpos << setprecision(8) << setw(30) << internal << setfill('0') << 0.12345678f;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            CPPUNIT_CHECK(output == "+00000000000000000000.12345678");
        }

        {
            ostringstream ostr;
            ostr << fixed << showpos << setprecision(100) << 1.234567e+17;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            CPPUNIT_CHECK(output.size() == 120);
            CPPUNIT_CHECK(output.substr(0, 6) == "+12345");
            CPPUNIT_CHECK(output.substr(19) == ".0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
        }

        {
            ostringstream ostr;
            ostr << fixed << showpos << setprecision(100) << 1.234567e+17l;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            CPPUNIT_CHECK(output.size() == 120);
            CPPUNIT_CHECK(output.substr(0, 6) == "+12345");
            CPPUNIT_CHECK(output.substr(19) == ".0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
        }

        {
            ostringstream ostr;
            ostr << scientific << setprecision(50) << 0.0;
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
            CPPUNIT_CHECK(output == "0.00000000000000000000000000000000000000000000000000e+00");
        }
        {
            ostringstream ostr;
            ostr << fixed << setprecision(100) << numeric_limits<float>::max();
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
        }

        {
            ostringstream ostr;
            ostr << setprecision(100) << numeric_limits<double>::max();
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
        }

        {
            ostringstream ostr;
            ostr << setprecision(100) << numeric_limits<long double>::max();
            CPPUNIT_ASSERT(ostr.good());
            output = reset_stream(ostr);
        }
    }

    {
        ostringstream str;

        str.setf(ios::fixed, ios::floatfield);
        str << 1.0e+5;
        CPPUNIT_CHECK(str.str() == "100000.000000");

        reset_stream(str);
        str.precision(0);
        str << 1.0e+5;
        CPPUNIT_CHECK(str.str() == "100000");

        reset_stream(str);
        str.precision(4);
        str << 1.0e+5;
        CPPUNIT_CHECK(str.str() == "100000.0000");

        reset_stream(str);
        str.precision(0);
        str << 1.0e+83;
        CPPUNIT_CHECK(str.str().size() == 84);
    }
}

#define CHECK_COMPLETE(type, val, base, showbase, showpos, casing, width, adjust, expected) \
{ \
    type tmp = val; \
    ostringstream ostr; \
    ostr << base << showbase << showpos << casing << setw(width) << adjust << tmp; \
    CPPUNIT_CHECK(ostr.str() == expected); \
}

#define CHECK(type, val, base, expected) \
    CHECK_COMPLETE(type, val, base, noshowbase, noshowpos, nouppercase, 0, right, expected)

void NumPutGetTest::num_put_integer()
{
    // octal outputs
    {
        CHECK(short, 0, oct, "0")
        CHECK(short, 1, oct, "1")
        CHECK(short, 12345, oct, "30071")
        if (sizeof(short) == 2) 
        {
            CHECK(short, -1, oct, "177777")
            CHECK(short, -12345, oct, "147707")
        }

        CHECK(unsigned short, 0, oct, "0")
        CHECK(unsigned short, 12345, oct, "30071")

        CHECK(int, 0, oct, "0")
        CHECK(int, 12345678, oct, "57060516")
        if (sizeof(int) == 4) 
        {
            CHECK(int, -1, oct, "37777777777")
            CHECK(int, -12345678, oct, "37720717262")
        }

        CHECK(unsigned int, 0, oct, "0")
        CHECK(unsigned int, 12345678, oct, "57060516")

        CHECK(long, 0, oct, "0")
        CHECK(long, 12345678, oct, "57060516")
        if (sizeof(long) == 4) 
        {
            CHECK(long, -1, oct, "37777777777")
            CHECK(long, -12345678, oct, "37720717262")
        }

        CHECK(unsigned long, 0, oct, "0")
        CHECK(unsigned long, 12345678, oct, "57060516")

        CHECK(LONG_LONG, 0, oct, "0")
        CHECK(LONG_LONG, 12345678, oct, "57060516")
        if (sizeof(LONG_LONG) == 8) 
        {
            CHECK(LONG_LONG, -1, oct, "1777777777777777777777")
            CHECK(LONG_LONG, -12345678, oct, "1777777777777720717262")
        }

        CHECK(ULONG_LONG, 0, oct, "0")
        CHECK(ULONG_LONG, 12345678, oct, "57060516")

        //Even with showbase, 0 value gives "0" (see printf documentation)
        CHECK_COMPLETE(short, 0, oct, showbase, noshowpos, nouppercase, 0, right, "0")
        CHECK_COMPLETE(short, 0, oct, showbase, showpos, nouppercase, 6, right, "     0")

        CHECK_COMPLETE(short, 1, oct, showbase, noshowpos, nouppercase, 6, right, "    01")
        CHECK_COMPLETE(short, 1, oct, showbase, noshowpos, nouppercase, 6, left, "01    ")
        CHECK_COMPLETE(short, 1, oct, showbase, showpos, nouppercase, 6, internal, "    01")
    }

    //decimal outputs
    {
        CHECK(short, 0, dec, "0")
        CHECK(short, -1, dec, "-1")
        CHECK(short, 12345, dec, "12345")
        CHECK(short, -12345, dec, "-12345")

        CHECK(unsigned short, 0, dec, "0")
        CHECK(unsigned short, 12345, dec, "12345")

        CHECK(int, 0, dec, "0")
        CHECK(int, -1, dec, "-1")
        CHECK(int, 12345678, dec, "12345678")
        CHECK(int, -12345678, dec, "-12345678")

        CHECK(unsigned int, 0, dec, "0")
        CHECK(unsigned int, 12345678, dec, "12345678")

        CHECK(long, 0, dec, "0")
        CHECK(long, -1, dec, "-1")
        CHECK(long, 12345678, dec, "12345678")
        CHECK(long, -12345678, dec, "-12345678")

        CHECK(unsigned long, 0, dec, "0")
        CHECK(unsigned long, 12345678, dec, "12345678")

        CHECK(LONG_LONG, 0, dec, "0")
        CHECK(LONG_LONG, -1, dec, "-1")
        CHECK(LONG_LONG, 12345678, dec, "12345678")
        CHECK(LONG_LONG, -12345678, dec, "-12345678")

        CHECK(ULONG_LONG, 0, dec, "0")
        CHECK(ULONG_LONG, 12345678, dec, "12345678")

        CHECK_COMPLETE(short, 0, dec, showbase, showpos, nouppercase, 0, right, "+0")
        CHECK_COMPLETE(short, 0, dec, showbase, showpos, nouppercase, 6, right, "    +0")
        CHECK_COMPLETE(short, 1, dec, showbase, showpos, nouppercase, 6, right, "    +1")
        CHECK_COMPLETE(short, 1, dec, showbase, showpos, nouppercase, 6, left, "+1    ")
        CHECK_COMPLETE(short, 1, dec, showbase, showpos, nouppercase, 6, internal, "+    1")
    }

    //hexadecimal outputs
    {
        CHECK(short, 0, hex, "0")
        CHECK(short, 12345, hex, "3039")
        if (sizeof(short) == 2) 
        {
            CHECK(short, -1, hex, "ffff")
            CHECK(short, -12345, hex, "cfc7")
        }

        CHECK(unsigned short, 0, hex, "0")
        CHECK(unsigned short, 12345, hex, "3039")

        CHECK(int, 0, hex, "0")
        CHECK(int, 12345678, hex, "bc614e")
        if (sizeof(int) == 4) 
        {
            CHECK(int, -1, hex, "ffffffff")
            CHECK(int, -12345678, hex, "ff439eb2")
        }

        CHECK(unsigned int, 0, hex, "0")
        CHECK(unsigned int, 12345678, hex, "bc614e")

        CHECK(long, 0, hex, "0")
        CHECK(long, 12345678, hex, "bc614e")
        if (sizeof(long) == 4) 
        {
            CHECK(long, -1, hex, "ffffffff")
            CHECK(long, -12345678, hex, "ff439eb2")
        }

        CHECK(unsigned long, 0, hex, "0")
        CHECK(unsigned long, 12345678, hex, "bc614e")

        CHECK(LONG_LONG, 0, hex, "0")
        CHECK(LONG_LONG, 12345678, hex, "bc614e")
        if (sizeof(LONG_LONG) == 8) 
        {
            CHECK(LONG_LONG, -1, hex, "ffffffffffffffff")
            CHECK(LONG_LONG, -12345678, hex, "ffffffffff439eb2")
        }

        CHECK(ULONG_LONG, 0, hex, "0")
        CHECK(ULONG_LONG, 12345678, hex, "bc614e")

        //Even with showbase, 0 value gives "0" output (see printf documentation)
        CHECK_COMPLETE(short, 0, hex, showbase, showpos, nouppercase, 0, right, "0")
        CHECK_COMPLETE(short, 0, hex, showbase, noshowpos, nouppercase, 6, right, "     0")
        CHECK_COMPLETE(short, 0, hex, showbase, noshowpos, nouppercase, 6, internal, "     0")

        CHECK_COMPLETE(short, 1, hex, showbase, noshowpos, nouppercase, 6, right, "   0x1")
        CHECK_COMPLETE(short, 1, hex, showbase, noshowpos, nouppercase, 6, left, "0x1   ")
        CHECK_COMPLETE(short, 1, hex, showbase, noshowpos, nouppercase, 6, internal, "0x   1")
        CHECK_COMPLETE(short, 1, hex, showbase, noshowpos, uppercase, 6, left, "0X1   ")
        CHECK_COMPLETE(short, 1, hex, showbase, showpos, uppercase, 6, internal, "0X   1")
    }
}

void NumPutGetTest::num_get_float()
{
    float in_val;

    istringstream istr;

    istr.str("1.2345");
    istr >> in_val;
    CPPUNIT_ASSERT(!istr.fail());
    CPPUNIT_ASSERT(istr.eof());
    CPPUNIT_ASSERT(check_float(in_val, 1.2345f));
    istr.clear();

    istr.str("-1.2345");
    istr >> in_val;
    CPPUNIT_ASSERT(!istr.fail());
    CPPUNIT_ASSERT(istr.eof());
    CPPUNIT_ASSERT(check_float(in_val, -1.2345f));
    istr.clear();

    istr.str("+1.2345");
    istr >> in_val;
    CPPUNIT_ASSERT(!istr.fail());
    CPPUNIT_ASSERT(check_float(in_val, 1.2345f));
    istr.clear();

    istr.str("000000000000001.234500000000");
    istr >> in_val;
    CPPUNIT_ASSERT(!istr.fail());
    CPPUNIT_ASSERT(istr.eof());
    CPPUNIT_ASSERT(check_float(in_val, 1.2345f));
    istr.clear();

    istr.str("1.2345e+04");
    istr >> in_val;
    CPPUNIT_ASSERT(!istr.fail());
    CPPUNIT_ASSERT(istr.eof());
    CPPUNIT_ASSERT(check_float(in_val, 12345.0f));
    istr.clear();

    CPPUNIT_MESSAGE("float");
    check_get_float(0.0F);
    CPPUNIT_MESSAGE("double");
    check_get_float(0.0);

    CPPUNIT_MESSAGE("long double");
    check_get_float(0.0L);
    {
        stringstream str;

        str << "1e" << numeric_limits<double>::max_exponent10;
        CPPUNIT_ASSERT(!str.fail());

        float val;
        str >> val;
        CPPUNIT_ASSERT(!str.fail());
        CPPUNIT_ASSERT(str.eof());
        CPPUNIT_ASSERT(numeric_limits<double>::max_exponent10 <= numeric_limits<float>::max_exponent10 ||
            val == numeric_limits<float>::infinity());
    }
    {
        stringstream str;

        str << "1e" << numeric_limits<double>::min_exponent10;
        CPPUNIT_ASSERT(!str.fail());

        float val;
        str >> val;
        CPPUNIT_ASSERT(!str.fail());
        CPPUNIT_ASSERT(str.eof());
        CPPUNIT_ASSERT(numeric_limits<double>::min_exponent10 >= numeric_limits<float>::min_exponent10 ||
            val == 0.0f);
    }
    {
        stringstream str;

        str << "1e" << numeric_limits<long double>::max_exponent10;
        CPPUNIT_ASSERT(!str.fail());

        double val;
        str >> val;
        CPPUNIT_ASSERT(!str.fail());
        CPPUNIT_ASSERT(str.eof());
        CPPUNIT_ASSERT(numeric_limits<long double>::max_exponent10 <= numeric_limits<double>::max_exponent10 ||
            val == numeric_limits<double>::infinity());
    }
    {
        stringstream str;

        str << "1e" << numeric_limits<long double>::min_exponent10;
        CPPUNIT_ASSERT(!str.fail());

        double val;
        str >> val;
        CPPUNIT_ASSERT(!str.fail());
        CPPUNIT_ASSERT(str.eof());
        CPPUNIT_ASSERT(numeric_limits<long double>::min_exponent10 >= numeric_limits<double>::min_exponent10 ||
            val == 0.0);
    }
    {
        const char* p = "2.718281828459045235360287471352662497757247093e0";
        stringstream s;
        s << p;
        long double x;
        s >> x;
        CPPUNIT_ASSERT(x > 2.70l && x < 2.72l);
    }
}

void NumPutGetTest::num_get_integer()
{
    // octal input
    {
        istringstream istr;
        istr.str("30071");
        short val;
        istr >> oct >> val;
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(val == 12345);
        istr.clear();

        if (sizeof(short) == 2) 
        {
            istr.str("177777");
            istr >> oct >> val;
            CPPUNIT_ASSERT(!istr.fail());
            CPPUNIT_ASSERT(istr.eof());
            CPPUNIT_ASSERT(val == -1);
            istr.clear();
        }
    }

    //decimal input
    {
        istringstream istr;
        istr.str("10000");
        short val = -1;
        istr >> val;
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(val == 10000);
        istr.clear();

        istr.str("+10000");
        val = -1;
        istr >> val;
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(val == 10000);
        istr.clear();

        if (sizeof(short) == 2) 
        {
            val = -1;
            istr.str("10000000");
            istr >> val;
            CPPUNIT_ASSERT(istr.fail());
            CPPUNIT_ASSERT(istr.eof());
            CPPUNIT_ASSERT(val == 32767);
            istr.clear();

            val = -1;
            istr.str("-32769");
            istr >> val;
            CPPUNIT_ASSERT(istr.fail());
            CPPUNIT_ASSERT(istr.eof());
            CPPUNIT_ASSERT(val == -32768);
            istr.clear();
        }

        val = -1;
        istr.str("0x0");
        istr >> val;
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(!istr.eof());
        CPPUNIT_ASSERT(val == 0);
        istr.clear();

        val = -1;
        istr.str("000001");
        istr >> val;
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(val == 1);
        istr.clear();
    }

    // hexadecimal input
    {
        istringstream istr;
        istr.str("3039");
        short val = -1;
        istr >> hex >> val;
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(val == 12345);
        istr.clear();

        istr.str("x3039");
        val = -1;
        istr >> hex >> val;
        CPPUNIT_ASSERT(istr.fail());
        CPPUNIT_ASSERT(!istr.eof());
        CPPUNIT_ASSERT(val == -1);
        istr.clear();

        istr.str("03039");
        val = -1;
        istr >> hex >> val;
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(val == 12345);
        istr.clear();

        istr.str("0x3039");
        istr >> hex >> val;
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(val == 12345);
        istr.clear();

        if (sizeof(short) == 2)
        {
            val = -1;
            istr.str("cfc7");
            istr >> hex >> val;
            CPPUNIT_ASSERT(!istr.fail());
            CPPUNIT_ASSERT(istr.eof());
            CPPUNIT_ASSERT(val == -12345);
            istr.clear();

            val = -1;
            istr.str("-7FFF");
            istr >> hex >> val;
            CPPUNIT_ASSERT(!istr.fail());
            CPPUNIT_ASSERT(istr.eof());
            CPPUNIT_ASSERT(val == -32767);
            istr.clear();

            val = -1;
            istr.str("-8000");
            istr >> hex >> val;
            CPPUNIT_ASSERT(!istr.fail());
            CPPUNIT_ASSERT(istr.eof());
            CPPUNIT_ASSERT(val == -32768);
            istr.clear();

            val = -1;
            istr.str("10000");
            istr >> hex >> val;
            CPPUNIT_ASSERT(istr.fail());
            CPPUNIT_ASSERT(istr.eof());
            CPPUNIT_ASSERT(val == 32767);
            istr.clear();
        }
    }

    // unsigned
    {
        istringstream istr;
        istr >> resetiosflags(ios::basefield);
        istr.str(" -1");
        unsigned long val = 345;
        istr >> val;
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(val == 0xFFFFFFFF);
        istr.clear();

        istr.str(" -0 ");
        val = 345;
        istr >> val;
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(!istr.eof());
        CPPUNIT_ASSERT(val == 0);
        istr.clear();

        istr.str("-0xFFFFFFFF");
        val = 345;
        istr >> val;
        CPPUNIT_ASSERT(!istr.fail());
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(val == 1);
        istr.clear();

        istr.str("-0x100000000");
        val = 345;
        istr >> val;
        CPPUNIT_ASSERT(istr.fail());
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(val == 0xFFFFFFFF);
        istr.clear();

        istr.str("0x100000000");
        val = 345;
        istr >> val;
        CPPUNIT_ASSERT(istr.fail());
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(val == 0xFFFFFFFF);
        istr.clear();

        istr.str("  0x");
        val = 345;
        istr >> val;
        CPPUNIT_ASSERT(istr.fail());
        CPPUNIT_ASSERT(istr.eof());
        CPPUNIT_ASSERT(val == 345);
        istr.clear();
    }
}

void NumPutGetTest::inhex()
{
    {
        ostringstream s;
        s << hex << 0;
        CPPUNIT_CHECK(s.str() == "0");
    }
    {
        ostringstream s;
        s << hex << 0xff;
        CPPUNIT_CHECK(s.str() == "ff");
    }
    {
        ostringstream s;
        s << hex << setw(4) << 0xff;
        CPPUNIT_CHECK(s.str() == "  ff");
    }
    {
        ostringstream s;
        s << hex << setw(4) << 0;
        CPPUNIT_CHECK(s.str() == "   0");
    }
    {
        ostringstream s;
        s << hex << showbase << 0;
        CPPUNIT_CHECK(s.str() == "0");
    }
    {
        ostringstream s;
        s << hex << showbase << 0xff;
        CPPUNIT_CHECK(s.str() == "0xff");
    }
    {
        ostringstream s;
        s << hex << showbase << setw(4) << 0xff;
        CPPUNIT_CHECK(s.str() == "0xff");
    }
    { // special case for regression (partially duplicate CHECK_COMPLETE above):
        ostringstream s;
        s.setf(ios_base::internal, ios_base::adjustfield);
        s << hex << showbase << setw(8 + 2) << 0;
        CPPUNIT_CHECK(s.str() == "         0");
    }
}

void NumPutGetTest::pointer()
{
    {
        ostringstream s;
        void *p = (void *)0xff00;
        s << p;
        CPPUNIT_ASSERT(s.good());
        if (sizeof(p) == 2) 
        {
            CPPUNIT_ASSERT(s.str() == "0xff00");
        }
        else if (sizeof(p) == 4) 
        {
            CPPUNIT_ASSERT(s.str() == "0x0000ff00"); // this pass
        }
        else if (sizeof(p) == 8) 
        {
            CPPUNIT_ASSERT(s.str() == "0x000000000000ff00");
        }
        else 
        {
            CPPUNIT_CHECK(sizeof(p) == 2 || sizeof(p) == 4 || sizeof(p) == 8);
        }
    }
    {
        ostringstream s;
        void *p = 0;
        s << p;
        CPPUNIT_ASSERT(s.good());
        if (sizeof(p) == 2) 
        {
            CPPUNIT_ASSERT(s.str() == "0x0000");
        }
        else if (sizeof(p) == 4) 
        {
            CPPUNIT_ASSERT(s.str() == "0x00000000"); // but this will fail, if follow %p
        }
        else if (sizeof(p) == 8) 
        {
            CPPUNIT_ASSERT(s.str() == "0x0000000000000000");
        }
        else 
        {
            CPPUNIT_CHECK(sizeof(p) == 2 || sizeof(p) == 4 || sizeof(p) == 8);
        }
    }
}

void NumPutGetTest::fix_float_long()
{
    ostringstream str;

    str.setf(ios::fixed, ios::floatfield);
    str << 1.0e+5;
    CPPUNIT_CHECK(str.str() == "100000.000000");

    reset_stream(str);
    str.precision(0);
    str << 1.0e+5;
    CPPUNIT_CHECK(str.str() == "100000");

    reset_stream(str);
    str.precision(4);
    str << 1.0e+5;
    CPPUNIT_CHECK(str.str() == "100000.0000");

    reset_stream(str);
    str.precision(0);
    str << 1.0e+83;
    {
        istringstream istr(str.str());
        double f;
        istr >> f;
        CPPUNIT_CHECK(!istr.fail());
        if (int(numeric_limits<double>::digits10) < 83) 
        {
            double delta = 1.0;
            for (int ee = 83 - int(numeric_limits<double>::digits10); ee > 0; --ee) 
            {
                delta *= 10.0;
            }

            // we may loss some digits here, but not more than mantissa:
            CPPUNIT_CHECK((f > (1.0e+83 - delta)) && (f < (1.0e+83 + delta)));
        }
        else 
        {
            CPPUNIT_CHECK(check_double(f, 1.0e+83));
        }
    }

    reset_stream(str);
    str.precision(0);
    str << numeric_limits<double>::max();
    {
        istringstream istr(str.str());
        double f;
        istr >> f;
        CPPUNIT_CHECK(!istr.fail());
        if (int(numeric_limits<double>::digits10) < int(numeric_limits<double>::max_exponent10)) 
        {
            double delta = 9.0;
            for (int ee = int(numeric_limits<double>::max_exponent10) - int(numeric_limits<double>::digits10); ee > 0; --ee) 
            {
                delta *= 10.0;
            }

            // we may loss some digits here, but not more than mantissa:
            CPPUNIT_CHECK((f > (numeric_limits<double>::max() - delta)));
        }
    }
}
