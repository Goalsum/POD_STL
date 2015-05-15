// ���ļ����屻�õ��Ļ����⺯����ʵ�֣�ͨ����ֱ�ӵ��ö�Ӧƽ̨�Ŀ⺯������ͬƽ̨ʹ����Ӧ�������޸Ĵ��ļ�����Ӧ��Ӧ��ƽ̨��

#include "_base_depends.h"
#include "cppunit_mini.h"

using namespace std;

void* STL_Malloc(size_t __size)
{
    void* p = malloc(__size);
    TestCase::recordMalloc(p, __size);
    return p;
}

void STL_Free(void* __buf)
{
    TestCase::recordFree(__buf);
    return free(__buf);
}

