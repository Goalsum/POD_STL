// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。
// 本文件定义被用到的基础库函数。不同平台使用者应当自行修改此文件来适应对应的平台。

#ifndef _PODSTL_BASE_DEPENDS_H
#define _PODSTL_BASE_DEPENDS_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef int ptrdiff_t;
typedef __int64 LONG_LONG;
typedef unsigned __int64 ULONG_LONG;
typedef int mbstate_t;

const int EOF = -1;

inline int VOS_memcpy_s(void* __dest, size_t __dest_max, const void* __src, size_t __count)
{
    return memcpy_s(__dest, __dest_max, __src, __count);
}

inline int VOS_memmove_s(void* __dest, size_t __dest_max, const void* __src, size_t __count)
{
    return memmove_s(__dest, __dest_max, __src, __count);
}

inline int VOS_memset_s(void* __dest, size_t __dest_max, int __content, size_t __count)
{
    if (__dest_max < __count) return -1;
    memset(__dest, __content, __count);
    return 0;
}

inline int VOS_MemCmp(const void* __dest, const void* __src, size_t __count)
{
    return memcmp(__dest, __src, __count);
}

inline const void* VOS_MemChr(const void* __buf, char __char, int __size)
{
    return memchr(__buf, __char, __size);
}

inline void* VOS_MemChr(void* __buf, char __char, int __size)
{
    return memchr(__buf, __char, __size);
}

inline size_t VOS_StrLen(const char* __c_string)
{
    return strlen(__c_string);
}

void* STL_Malloc(size_t __size);

void STL_Free(void* __buf);

#define STL_ASSERT assert

#endif  // _PODSTL_BASE_DEPENDS_H

