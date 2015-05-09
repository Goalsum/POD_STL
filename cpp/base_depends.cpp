// 本文件定义被用到的基础库函数的实现，通常是直接调用对应平台的库函数。不同平台使用者应当自行修改此文件来适应对应的平台。

#include "_base_depends.h"
#include "string.h"
#include "cppunit_mini.h"

using namespace std;

int VOS_memcpy_s(void* __dest, size_t __dest_max, const void* __src, size_t __count)
{
    return memcpy_s(__dest, __dest_max, __src, __count);
}

int VOS_memmove_s(void* __dest, size_t __dest_max, const void* __src, size_t __count)
{
    return memmove_s(__dest, __dest_max, __src, __count);
}

int VOS_memset_s(void* __dest, size_t __dest_max, int __content, size_t __count)
{
    if (__dest_max < __count) return -1;
    memset(__dest, __content, __count);
    return 0;
}

int VOS_MemCmp(const void* __dest, const void* __src, size_t __count)
{
    return memcmp(__dest, __src, __count);
}

const void* VOS_MemChr(const void* __buf, char __char, int __size)
{
    return memchr(__buf, __char, __size);
}

void* VOS_MemChr(void* __buf, char __char, int __size)
{
    return memchr(__buf, __char, __size);
}

size_t VOS_StrLen(const char* __c_string)
{
    return strlen(__c_string);
}

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

