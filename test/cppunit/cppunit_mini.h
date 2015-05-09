// CPPUnit mini 测试框架头文件，本文件仅在对STL做单元测试时才需要。

#ifndef _CPPUNITMPFR_H_
#define _CPPUNITMPFR_H_

#include <string.h>
#include <stdio.h>

class Reporter
{
public:
    virtual ~Reporter() {}
    virtual void error(const char * /*macroName*/, const char * /*in_macro*/, const char * /*in_file*/, int /*in_line*/) {}
    virtual void message( const char * /*msg*/ ) {}
    virtual void progress( const char * /*in_className*/, const char * /*in_testName*/, bool /*ignored*/, bool /* explicit */) {}
    virtual void end() {}
    virtual void printSummary() {}
};

class TestFixture
{
public:
    virtual ~TestFixture() {}

    //! \brief Set up context before running a test.
    virtual void setUp() {}

    //! Clean up after the test run.
    virtual void tearDown() {}
};

struct MallocRecord
{
    MallocRecord() : m_buf_pointer(0), m_size(0) {}
    void set(void* buf_pointer, size_t buf_size)
    {
        m_buf_pointer = buf_pointer;
        m_size = buf_size;
    }
    void clear() 
    { 
        m_buf_pointer = 0;
        m_size = 0;
    }
    bool isIdle()
    {
        return (m_buf_pointer == 0) && (m_size == 0);
    }

    void* m_buf_pointer;
    size_t m_size;
};

class TestCase : public TestFixture
{
public:
    TestCase() { registerTestCase(this); }

    void setUp() 
    { 
        m_failed = false; 
        m_alloc_free_error = false;
    }
    static int run(Reporter *in_reporter = 0, const char *in_testName = "", bool invert = false);
    int numErrors() { return m_numErrors; }
    static void registerTestCase(TestCase *in_testCase);

    virtual void myRun(const char * /*in_name*/, bool /*invert*/ = false) {}

    virtual void error(const char *in_macroName, const char *in_macro, const char *in_file, int in_line)
    {
        m_failed = true;
        if (m_reporter)
        {
            m_reporter->error(in_macroName, in_macro, in_file, in_line);
        }
    }

    static void message(const char *msg)
    {
        if (m_reporter)
        {
            m_reporter->message(msg);
        }
    }

    bool equalDoubles(double in_expected, double in_real, double in_maxErr)
    {
        double diff = in_expected - in_real;
        if (diff < 0.)
        {
            diff = -diff;
        }
        return diff < in_maxErr;
    }

    virtual void progress(const char *in_className, const char *in_functionName, bool ignored, bool explicitTest)
    {
        ++m_numTests;
        if (m_reporter)
        {
            m_reporter->progress(in_className, in_functionName, ignored, explicitTest);
        }
    }

    bool shouldRunThis(const char *in_desiredTest, const char *in_className, const char *in_functionName,
                       bool invert, bool explicit_test, bool &do_progress)
    {
        if ((in_desiredTest) && (in_desiredTest[0] != '\0'))
        {
            do_progress = false;
            const char *ptr = strstr(in_desiredTest, "::");
            if (ptr)
            {
                bool match = (strncmp(in_desiredTest, in_className, strlen(in_className)) == 0) &&
                             (strncmp(ptr + 2, in_functionName, strlen(in_functionName)) == 0);

                // Invert shall not make explicit test run:
                return invert ? (match ? !match : !explicit_test) : match;
            }
            bool match = (strcmp(in_desiredTest, in_className) == 0);
            do_progress = match;
            return !explicit_test && (match == !invert);
        }
        do_progress = true;
        return !explicit_test;
    }

    void tearDown()
    {
        checkLeak();
        if (m_failed) ++m_numErrors;
        m_reporter->end();
    }

    static void recordMalloc(void* buf_pointer, size_t buf_size)
    {
        if (buf_size <= 0)
        {
            message("malloc buf_size == 0 error!");
            m_alloc_free_error = true;
            return;
        }
        
        for (int i = 0; i < sizeof(m_malloc_records) / sizeof(m_malloc_records[0]); ++i)
        {
            if (m_malloc_records[i].isIdle())
            {
                m_malloc_records[i].set(buf_pointer, buf_size);
                return;
            }
        }

        printf("\n\ttoo many mallocs beyond %d! buf_pointer : %p, buf_size : %u", sizeof(m_malloc_records) / sizeof(m_malloc_records[0]), buf_pointer, buf_size);
        m_alloc_free_error = true;
    }

    static void recordFree(void* __buf)
    {
        if (__buf == 0)
        {
            message("free pointer == NULL error!");
            m_alloc_free_error = true;
            return;
        }

        for (int i = 0; i < sizeof(m_malloc_records) / sizeof(m_malloc_records[0]); ++i)
        {
            if (m_malloc_records[i].m_buf_pointer == __buf)
            {
                m_malloc_records[i].clear();
                return;
            }
        }
        printf("\n\tthe pointer to free %p not allocated!", __buf);
        m_alloc_free_error = true;
    }

    void checkLeak()
    {
        for (int i = 0; i < sizeof(m_malloc_records) / sizeof(m_malloc_records[0]); ++i)
        {
            if (!m_malloc_records[i].isIdle())
            {
                m_alloc_free_error = true;
                printf("\n\tMem leak! pointer : %p, size : %u", m_malloc_records[i].m_buf_pointer, m_malloc_records[i].m_size);
                m_malloc_records[i].clear();
            }
        }

        if (m_alloc_free_error)
        {
            error("CPPUNIT_FAIL", "MEM_ERROR", __FILE__, __LINE__);
        }
    }

protected:
    static int m_numErrors;
    static int m_numTests;

private:
    static TestCase *m_root;
    TestCase *m_next;
    bool m_failed;

    static Reporter *m_reporter;
    static const int RECORD_SIZE = 10000;
    static MallocRecord m_malloc_records[RECORD_SIZE];
    static bool m_alloc_free_error;
};

#define CPPUNIT_TEST_SUITE(X) \
    typedef TestCase Base; \
    virtual void myRun(const char *in_name, bool invert = false) \
    { \
        const char *className = #X; \
        bool ignoring = false;

#define CPPUNIT_TEST_BASE(X, Y) \
    { \
        bool do_progress; \
        bool shouldRun = shouldRunThis(in_name, className, #X, invert, Y, do_progress); \
        if (shouldRun || do_progress) \
        { \
            setUp(); \
            progress(className, #X, ignoring || !shouldRun, !ignoring && Y); \
            if (shouldRun && !ignoring) X(); \
            tearDown(); \
        } \
    }


#define CPPUNIT_TEST(X) CPPUNIT_TEST_BASE(X, false)
#define CPPUNIT_EXPLICIT_TEST(X) CPPUNIT_TEST_BASE(X, true)

#define CPPUNIT_IGNORE ignoring = true

#define CPPUNIT_STOP_IGNORE ignoring = false

#define CPPUNIT_TEST_SUITE_END() }

#define CPPUNIT_TEST_SUITE_REGISTRATION(X) static X local

#define CPPUNIT_CHECK(X) \
    if (!(X)) \
    { \
        Base::error("CPPUNIT_CHECK", #X, __FILE__, __LINE__); \
    }

#define CPPUNIT_ASSERT(X) \
    if (!(X)) \
    { \
        Base::error("CPPUNIT_ASSERT", #X, __FILE__, __LINE__); \
        return; \
    }

#define CPPUNIT_FAIL \
    { \
        Base::error("CPPUNIT_FAIL", "", __FILE__, __LINE__); \
        return; \
    }

#define CPPUNIT_ASSERT_EQUAL(X, Y) \
    if ((X) != (Y)) \
    { \
        Base::error("CPPUNIT_ASSERT_EQUAL", #X","#Y, __FILE__, __LINE__); \
        return; \
    }

#define CPPUNIT_ASSERT_DOUBLES_EQUAL(X, Y, Z) \
    if (!equalDoubles((X), (Y), (Z))) \
    { \
        Base::error("CPPUNIT_ASSERT_DOUBLES_EQUAL", #X","#Y","#Z, __FILE__, __LINE__); \
        return; \
    }

#define CPPUNIT_MESSAGE(m) TestCase::message(m)

#endif

