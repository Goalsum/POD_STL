/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义一些公用基础算法的实现。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

#include "_algobase.h"

namespace podstl
{

void* _copy_trivial(const void* __first, const void* __last, void* __result)
{
    size_t __n = static_cast<const char*>(__last) - static_cast<const char*>(__first);
    if (__n <= 0) return __result;
    VOS_memmove_s(__result, __n, __first, __n);
    return static_cast<char*>(__result) + __n;
}

void* _copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const ptrdiff_t _Num = static_cast<const char*>(__last) - static_cast<const char*>(__first);
    if (_Num <= 0) return __result;
    VOS_memmove_s(static_cast<char*>(__result) - _Num, _Num, __first, _Num);
    return static_cast<char*>(__result) - _Num;
}

void* _fill_n_trivial(void* __first, size_t __count, const void* __pointer, size_t __unit_size)
{
    for ( ; __count > 0; --__count, __first = static_cast<char*>(__first) + __unit_size)
    {
        VOS_memcpy_s(__first, __unit_size, __pointer, __unit_size);
    }
    return __first;
}

}

