// ���ļ����ڲ�ͷ�ļ�(�ļ������»��߿�ͷ)���������ⲿʹ����ֱ�Ӱ�����
// ���ļ����屻�õ��Ļ����⺯������ͬƽ̨ʹ����Ӧ�������޸Ĵ��ļ�����Ӧ��Ӧ��ƽ̨��

#ifndef _PODSTL_BASE_DEPENDS_H
#define _PODSTL_BASE_DEPENDS_H

#include <stdlib.h>
#include <assert.h>

typedef int ptrdiff_t;
typedef __int64 LONG_LONG;
typedef unsigned __int64 ULONG_LONG;
typedef int mbstate_t;

const int EOF = -1;

int VOS_memcpy_s(void* __dest, size_t __dest_max, const void* __src, size_t __count);

int VOS_memmove_s(void* __dest, size_t __dest_max, const void* __src, size_t __count);

int VOS_memset_s(void* __dest, size_t __dest_max, int __content, size_t __count);

int VOS_MemCmp(const void* __dest, const void* __src, size_t __count);

const void* VOS_MemChr(const void* __buf, char __char, int __size);

void* VOS_MemChr(void* __buf, char __char, int __size);

size_t VOS_StrLen(const char* __c_string);

void* STL_Malloc(size_t __size);

void STL_Free(void* __buf);

#define STL_ASSERT assert

#endif  // _PODSTL_BASE_DEPENDS_H

