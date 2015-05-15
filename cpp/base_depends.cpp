// 本文件定义被用到的基础库函数的实现，通常是直接调用对应平台的库函数。不同平台使用者应当自行修改此文件来适应对应的平台。

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

