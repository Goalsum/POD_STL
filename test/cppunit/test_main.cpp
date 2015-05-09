// CPPUnit mini 测试使用的cpp文件，定义了做测试的入口main函数。本文件仅在对STL做单元测试时才需要。

#include "cppunit_mini.h"
#include "file_reporter.h"
#include "stdio.h"

int TestCase::m_numErrors = 0;
int TestCase::m_numTests = 0;

TestCase *TestCase::m_root = 0;
Reporter *TestCase::m_reporter = 0;
MallocRecord TestCase::m_malloc_records[RECORD_SIZE];
bool TestCase::m_alloc_free_error = false;

void TestCase::registerTestCase(TestCase *in_testCase)
{
    in_testCase->m_next = m_root;
    m_root = in_testCase;
}

int TestCase::run(Reporter *in_reporter, const char *in_testName, bool invert)
{
    TestCase::m_reporter = in_reporter;

    m_numErrors = 0;
    m_numTests = 0;

    TestCase *tmp = m_root;
    while (tmp != 0)
    {
        tmp->myRun(in_testName, invert);
        tmp = tmp->m_next;
    }
    return m_numErrors;
}

static void usage(const char* name)
{
    printf("Usage : %s [-t=<class>[::<test>]] [-x=<class>[::<test>]] [-f=<file>]\n", name);
    printf("\t[-t=<class>[::<test>]] : test class or class::test to execute;\n");
    printf("\t[-x=<class>[::<test>]] : test class or class::test to exclude from execution;\n");
    printf("\t[-f=<file>] : output file");
    printf("\n");
}

int main(int argc, char** argv)
{
    // CppUnit(mini) test launcher
    // command line option syntax:
    // test [OPTIONS]
    // where OPTIONS are
    //  -t=CLASS[::TEST]    run the test class CLASS or member test CLASS::TEST
    //  -x=CLASS[::TEST]    run all except the test class CLASS or member test CLASS::TEST
    //  -f=FILE             save output in file FILE instead of stdout
    //  -m                  monitor test(s) execution
    const char *fileName = 0;
    const char *testName = "";
    const char *xtestName = "";
    bool doMonitoring = false;

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            if (!strncmp(argv[i], "-t=", 3))
            {
                testName = argv[i]+3;
                continue;
            }
            else if (!strncmp(argv[i], "-f=", 3))
            {
                fileName = argv[i]+3;
                continue;
            }
            else if (!strncmp(argv[i], "-x=", 3))
            {
                xtestName = argv[i]+3;
                continue;
            }
        }

        // invalid option, we display normal usage.
        usage(argv[0]);
        return 1;
    }

    Reporter* reporter;
    if (fileName != 0)
        reporter = new FileReporter(fileName, doMonitoring);
    else
        reporter = new FileReporter(stdout, doMonitoring);

    int num_errors;
    if (xtestName[0] != 0)
    {
        num_errors = TestCase::run(reporter, xtestName, true);
    }
    else
    {
        num_errors = TestCase::run(reporter, testName);
    }

    reporter->printSummary();
    delete reporter;

    // 只是为了VS下停住看结果
    getchar();
    return num_errors;
}

