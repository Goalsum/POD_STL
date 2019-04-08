/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义了一些公用初始化算法。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。

#ifndef _PODSTL_UNINITIALIZED_H
#define _PODSTL_UNINITIALIZED_H

#include "_algobase.h"
#include "_construct.h"

namespace podstl
{

// 仅仅内部初始化时使用，只能针对POD类型初始化，采用内存拷贝方式。传入迭代器必须是指针！
template <typename _Size, typename _Tp>
inline _Tp* _uinit_aux_aux(_Tp* __first, _Size __n, const _Tp& __val, const _FalseType& /* HasDefaultZero */)
{
    return static_cast<_Tp*>(_fill_n_trivial(__first, __n, &__val, sizeof(__val)));
}

template <typename _Size, typename _Tp>
inline _Tp* _uinit_aux_aux(_Tp* __first, _Size __n, const _Tp&, const _TrueType& /* HasDefaultZero */)
{
    VOS_memset_s(__first, __n * sizeof(_Tp), 0, __n * sizeof(_Tp));
    return __first + __n;
}

template <typename _Size, typename _Tp>
inline _Tp* _uinit_aux(_Tp* __first, _Size __n, const _Tp&, const _TrueType& /* TrivialInit */)
{
    return __first + __n;
}

template <typename _Size, typename _Tp>
inline _Tp* _uinit_aux(_Tp* __first, _Size __n, const _Tp& __val, const _FalseType& /* TrivialInit */)
{
    return _uinit_aux_aux(__first, __n, __val, _has_default_zero_value(__first)._Answer());
}

template <typename _Size, typename _Tp>
inline _Tp* _uninitialized_init(_Tp* __first, _Size __n, const _Tp& __val)
{
    return _uinit_aux(__first, __n, __val, _UseTrivialInit(__first)._Answer());
}

}

#endif // _PODSTL_UNINITIALIZED_H

