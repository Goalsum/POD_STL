/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义了一些公用基础算法。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。

#ifndef _PODSTL_ALGOBASE_H
#define _PODSTL_ALGOBASE_H

#include <limits.h>
#include "type_traits.h"
#include "_iterator.h"
#include "_function_base.h"
#include "_pair.h"

namespace podstl
{

template <typename _Tp>
inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template <typename _Tp>
inline const _Tp& min(const _Tp& __a, const _Tp& __b)
{
    return __b < __a ? __b : __a;
}

template <typename _Tp>
inline const _Tp& max(const _Tp& __a, const _Tp& __b)
{
    return  __a < __b ? __b : __a;
}

template <typename _Tp, typename _Compare>
inline const _Tp& min(const _Tp& __a, const _Tp& __b, _Compare __comp)
{
    return __comp(__b, __a) ? __b : __a;
}

template <typename _Tp, typename _Compare>
inline const _Tp& max(const _Tp& __a, const _Tp& __b, _Compare __comp)
{
    return __comp(__a, __b) ? __b : __a;
}

template <typename _InputIter, typename _OutputIter, typename _Distance>
inline _OutputIter _copy(_InputIter __first, _InputIter __last, _OutputIter __result, const input_iterator_tag&, _Distance*)
{
    for ( ; __first != __last; ++__result, ++__first)
    {
        *__result = *__first;
    }
    return __result;
}

// 对于Random Iterator，使用表示距离的整型变量作为循环控制变量可以提高性能。
template <typename _RandomAccessIter, typename _OutputIter, typename _Distance>
inline _OutputIter _copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
    {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

// 有些类型用memmove有最好的性能。注意不能用memcpy，因为指针指向区域有可能重叠。
void* _copy_trivial(const void* __first, const void* __last, void* __result);

template <typename _InputIter, typename _OutputIter>
inline _OutputIter _copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const _FalseType& /* UseTrivialCopy */)
{
    return _copy(__first, __last, __result, random_access_iterator_tag(), static_cast<ptrdiff_t*>(0));
}

template <typename _InputIter, typename _OutputIter>
inline _OutputIter _copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const _TrueType& /* UseTrivialCopy */)
{
    return static_cast<_OutputIter>(_copy_trivial(__first, __last, __result));
}

template <typename _InputIter, typename _OutputIter>
inline _OutputIter _copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const _TrueType& /* BothPtrType */)
{
    return _copy_ptrs(__first, __last, __result, _use_trivial_copy(_PODSTL_VALUE_TYPE(_InputIter), _PODSTL_VALUE_TYPE(_OutputIter))._Answer());
}

template <typename _InputIter, typename _OutputIter>
inline _OutputIter _copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const _FalseType& /* BothPtrType */)
{
    return _copy(__first, __last, __result, _PODSTL_ITERATOR_CATEGORY(_InputIter), _PODSTL_DISTANCE_TYPE(_InputIter));
}

template <typename _InputIter, typename _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return _copy_aux(__first, __last, __result, _BothPtrType<_InputIter, _OutputIter>::_Answer());
}

template <typename _BidirectionalIter1, typename _BidirectionalIter2, typename _Distance>
inline _BidirectionalIter2 _copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                           const bidirectional_iterator_tag&, _Distance*)
{
    while (__first != __last)
    {
        *--__result = *--__last;
    }
    return __result;
}

// 对于Random Iterator，使用表示距离的整型变量作为循环控制变量可以提高性能。
template <typename _RandomAccessIter, typename _BidirectionalIter, typename _Distance>
inline _BidirectionalIter _copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
    {
        *--__result = *--__last;
    }
    return __result;
}

// 有些类型用memmove有最好的性能。注意不能用memcpy，因为指针指向区域有可能重叠。
void* _copy_trivial_backward(const void* __first, const void* __last, void* __result);

template <typename _InputIter, typename _OutputIter>
inline _OutputIter _copy_backward_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const _FalseType& /* UseTrivialCopy */)
{
    return _copy_backward(__first, __last, __result, _PODSTL_ITERATOR_CATEGORY(_InputIter), _PODSTL_DISTANCE_TYPE(_InputIter));
}

template <typename _InputIter, typename _OutputIter>
inline _OutputIter _copy_backward_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const _TrueType& /* UseTrivialCopy */)
{
    return static_cast<_OutputIter>(_copy_trivial_backward(__first, __last, __result));
}

template <typename _InputIter, typename _OutputIter>
inline _OutputIter _copy_backward_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const _FalseType& /* BothPtrType */)
{
    return _copy_backward(__first, __last, __result, _PODSTL_ITERATOR_CATEGORY(_InputIter), _PODSTL_DISTANCE_TYPE(_InputIter));
}

template <typename _InputIter, typename _OutputIter>
inline _OutputIter _copy_backward_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const _TrueType& /* BothPtrType */)
{
    return _copy_backward_ptrs(__first, __last, __result, _use_trivial_copy(_PODSTL_VALUE_TYPE(_InputIter), _PODSTL_VALUE_TYPE(_OutputIter))._Answer());
}

template <typename _InputIter, typename _OutputIter>
inline _OutputIter copy_backward(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return _copy_backward_aux(__first, __last, __result, _BothPtrType<_InputIter, _OutputIter>::_Answer());
}

template <typename _ForwardIter, typename _Tp>
inline void _fill_fwd(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
    {
        *__first = __val;
    }
}

template <typename _ForwardIter, typename _Tp, typename _Distance>
inline void _fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, const input_iterator_tag&, _Distance*)
{
    _fill_fwd(__first, __last, __val);
}

// 对于Random Iterator，使用表示距离的整型变量作为循环控制变量可以提高性能。
template <typename _RandomAccessIter, typename _Tp, typename _Distance>
inline void _fill(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first ; __n > 0; ++__first, --__n)
    {
        *__first = __val;
    }
}

template <typename _ForwardIter, typename _Tp>
inline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    _fill(__first, __last, __val, _PODSTL_ITERATOR_CATEGORY(_ForwardIter), _PODSTL_DISTANCE_TYPE(_ForwardIter));
}

inline void fill(unsigned char* __first, unsigned char* __last, const unsigned char& __val)
{
    VOS_memset_s(__first, __last - __first, __val, __last - __first);
}

inline void fill(signed char* __first, signed char* __last, const signed char& __val)
{
    VOS_memset_s(__first, __last - __first, __val, __last - __first);
}

inline void fill(char* __first, char* __last, const char& __val)
{
    VOS_memset_s(__first, __last - __first, __val, __last - __first);
}

void* _fill_n_trivial(void* __first, size_t __n, const void* __val, size_t __val_size);

template <typename _OutputIter, typename _Size, typename _Tp>
inline _OutputIter _fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
    {
        *__first = __val;
    }
    return __first;
}

template <typename _Size>
inline unsigned char* _fill_n(unsigned char* __first, _Size __n, const unsigned char& __val)
{
    fill(__first, __first + __n, __val);
    return __first + __n;
}

template <typename _Size>
inline signed char* _fill_n(signed char* __first, _Size __n, const signed char& __val)
{
    fill(__first, __first + __n, __val);
    return __first + __n;
}

template <typename _Size>
inline char* _fill_n(char* __first, _Size __n, const char& __val)
{
    fill(__first, __first + __n, __val);
    return __first + __n;
}

template <typename _OutputIter, typename _Size, typename _Tp>
inline void fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    _fill_n(__first, __n, __val);
}

template <typename _InputIter1, typename _InputIter2>
inline pair<_InputIter1, _InputIter2> mismatch(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2)
{
    while ((__first1 != __last1) && (*__first1 == *__first2))
    {
        ++__first1;
        ++__first2;
    }
    return pair<_InputIter1, _InputIter2>(__first1, __first2);
}

template <typename _InputIter1, typename _InputIter2, typename _BinaryPredicate>
inline pair<_InputIter1, _InputIter2> mismatch(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _BinaryPredicate __binary_pred)
{
    while ((__first1 != __last1) && __binary_pred(*__first1, *__first2))
    {
        ++__first1;
        ++__first2;
    }
    return pair<_InputIter1, _InputIter2>(__first1, __first2);
}

template <typename _InputIter1, typename _InputIter2>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2)
{
    for ( ; __first1 != __last1; ++__first1, ++__first2)
    {
        if (!(*__first1 == *__first2))
        {
            return false;
        }
    }
    return true;
}

template <typename _InputIter1, typename _InputIter2, typename _BinaryPredicate>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _BinaryPredicate __binary_pred)
{
    for ( ; __first1 != __last1; ++__first1, ++__first2)
    {
        if (!__binary_pred(*__first1, *__first2))
        {
            return false;
        }
    }
    return true;
}

template <typename _InputIter1, typename _InputIter2>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2)
{
    for ( ; (__first1 != __last1) && (__first2 != __last2); ++__first1, ++__first2)
    {
        if (*__first1 < *__first2)
        {
            return true;
        }
        if (*__first2 < *__first1)
        {
            return false;
        }
    }
    return (__first1 == __last1) && (__first2 != __last2);
}

template <typename _InputIter1, typename _InputIter2, typename _Compare>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2, _Compare __comp)
{
    for ( ; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2)
    {
        if (__comp(*__first1, *__first2))
        {
            return true;
        }
        if (__comp(*__first2, *__first1))
        {
            return false;
        }
    }
    return (__first1 == __last1) && (__first2 != __last2);
}

inline bool lexicographical_compare(const unsigned char* __first1, const unsigned char* __last1, const unsigned char* __first2, const unsigned char* __last2)
{
    const size_t __len1 = __last1 - __first1;
    const size_t __len2 = __last2 - __first2;

    const int __result = VOS_MemCmp(__first1, __first2, min(__len1, __len2));
    return (__result != 0) ? (__result < 0) : (__len1 < __len2);
}

inline bool lexicographical_compare(const char* __first1, const char* __last1, const char* __first2, const char* __last2)
{
    const size_t __len1 = __last1 - __first1;
    const size_t __len2 = __last2 - __first2;

    const int __result = VOS_MemCmp(__first1, __first2, min(__len1, __len2));
    return (__result != 0) ? (__result < 0) : (__len1 < __len2);
}

template <typename _InputIter, typename _Tp>
inline _PODSTL_DIFFERENCE_TYPE(_InputIter) count(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    _PODSTL_DIFFERENCE_TYPE(_InputIter) __n = 0;
    for ( ; __first != __last; ++__first)
    {
        if (*__first == __val)
        {
            ++__n;
        }
    }
    return __n;
}

template <typename _RandomAccessIter, typename _Tp>
inline _RandomAccessIter _find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val, const random_access_iterator_tag&)
{
    _PODSTL_DIFFERENCE_TYPE(_RandomAccessIter) __trip_count = __last - __first;

    for ( ; __trip_count > 0; --__trip_count, ++__first)
    {
        if (*__first == __val)
        {
            return __first;
        }
    }
    return __last;
}

inline char* _find(char* __first, char* __last, char __val, const random_access_iterator_tag&)
{
    void* res = VOS_MemChr(__first, __val, __last - __first);
    return (res != 0) ? static_cast<char*>(res) : __last;
}

inline const char* _find(const char* __first, const char* __last, char __val, const random_access_iterator_tag&)
{
    const void *res = VOS_MemChr(__first, __val, __last - __first);
    return (res != 0) ? static_cast<const char*>(res) : __last;
}

template <typename _InputIter, typename _Tp>
inline _InputIter _find(_InputIter __first, _InputIter __last, const _Tp& __val, const input_iterator_tag&)
{
    while ((__first != __last) && !(*__first == __val))
    {
        ++__first;
    }
    return __first;
}

template <typename _InputIter, typename _Tp>
_InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return _find(__first, __last, __val, _PODSTL_ITERATOR_CATEGORY(_InputIter));
}

template <typename _RandomAccessIter, typename _Predicate>
inline _RandomAccessIter _find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred, const random_access_iterator_tag&)
{
    _PODSTL_DIFFERENCE_TYPE(_RandomAccessIter) __trip_count = __last - __first;

    for ( ; __trip_count > 0; --__trip_count, ++__first)
    {
        if (__pred(*__first))
        {
            return __first;
        }
    }
    return __last;
}

template <typename _InputIter, typename _Predicate>
inline _InputIter _find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&)
{
    while ((__first != __last) && !__pred(*__first))
    {
        ++__first;
    }
    return __first;
}

template <typename _InputIter, typename _Predicate>
_InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return _find_if(__first, __last, __pred, _PODSTL_ITERATOR_CATEGORY(_InputIter));
}

template <typename _ForwardIter1, typename _ForwardIter2>
_ForwardIter1 search(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2, _ForwardIter2 __last2)
{
    // 检查空集合
    if ((__first1 == __last1) || (__first2 == __last2))
    {
        return __first1;
    }

    // 对于被查找串只有一个元素的情况，特殊处理。
    _ForwardIter2 __p1(__first2);
    if (++__p1 == __last2)
    {
        while ((__first1 != __last1) && (*__first1 != *__first2))
        {
            ++__first1;
        }
        return __first1;
    }

    for (;;)
    {
        while ((__first1 != __last1) && (*__first1 != *__first2))
        {
            ++__first1;
        }
        if (__first1 == __last1)
        {
            return __last1;
        }
        _ForwardIter2 __p = __p1;
        _ForwardIter1 __current = __first1;
        if (++__current == __last1) return __last1;

        while (*__current == *__p)
        {
            if (++__p == __last2) return __first1;
            if (++__current == __last1) return __last1;
        }
        ++__first1;
    }
    return __first1;
}

template <typename _ForwardIter1, typename _ForwardIter2, typename _BinaryPred>
_ForwardIter1 search(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2, _ForwardIter2 __last2, _BinaryPred __predicate)
{
    // 检查空集合
    if ((__first1 == __last1) || (__first2 == __last2))
    {
        return __first1;
    }

    // 对于被查找串只有一个元素的情况，特殊处理。
    _ForwardIter2 __p1(__first2);
    if (++__p1 == __last2)
    {
        while ((__first1 != __last1) && !__predicate(*__first1, *__first2))
        {
            ++__first1;
        }
        return __first1;
    }

    for ( ; ; )
    {
        while ((__first1 != __last1) && !__predicate(*__first1, *__first2))
        {
            ++__first1;
        }
        if (__first1 == __last1)
        {
            return __last1;
        }
        _ForwardIter2 __p = __p1;
        _ForwardIter1 __current = __first1;
        if (++__current == __last1) return __last1;

        while (__predicate(*__current, *__p))
        {
            if (++__p == __last2) return __first1;
            if (++__current == __last1) return __last1;
        }
        ++__first1;
    }
    return __first1;
}

template <typename _Tp>
struct _IsCharLikeType
{
    typedef _FalseType _Ret;
};

template <>
struct _IsCharLikeType<char>
{
    typedef _TrueType _Ret;
};

template <>
struct _IsCharLikeType<unsigned char>
{
    typedef _TrueType _Ret;
};

template <>
struct _IsCharLikeType<signed char>
{
    typedef _TrueType _Ret;
};

template <typename _Tp1, typename _Tp2>
inline bool _same_type_or_eq(_Tp1 __val1, _Tp2 __val2)
{
    return __val1 == __val2;
}

template <typename _Tp>
inline bool _same_type_or_eq(_Tp, _Tp)
{
    return true;
}

template <typename _InputIter, typename _ForwardIter, typename _Tp2, typename _Predicate>
inline _InputIter _find_first_of_aux2(_InputIter __first1, _InputIter __last1, _ForwardIter __first2, _ForwardIter __last2, 
                                      _Tp2*, _Predicate __pred, const _TrueType& /* UseStrcspnLikeAlgo */)
{
    unsigned char __hints[(UCHAR_MAX + 1) / CHAR_BIT] = {0};
    for (; __first2 != __last2; ++__first2)
    {
        unsigned char __tmp = (unsigned char)*__first2;
        __hints[__tmp / CHAR_BIT] |= (1 << (__tmp % CHAR_BIT));
    }

    for (; __first1 != __last1; ++__first1)
    {
        _Tp2 __tmp = (_Tp2)*__first1;
        if (_same_type_or_eq(*__first1, __tmp) && __pred((__hints[(unsigned char)__tmp / CHAR_BIT] & (1 << ((unsigned char)__tmp % CHAR_BIT))) != 0))
        {
            break;
        }
    }
    return __first1;
}

template <typename _InputIter, typename _ForwardIter, typename _Tp2, typename _Predicate>
inline _InputIter _find_first_of_aux2(_InputIter __first1, _InputIter __last1, _ForwardIter __first2, _ForwardIter __last2, 
                                      _Tp2*, _Predicate, const _FalseType& /* UseStrcspnLikeAlgo */)
{
    for ( ; __first1 != __last1; ++__first1)
    {
        for (_ForwardIter __iter = __first2; __iter != __last2; ++__iter)
        {
            if (*__first1 == *__iter)
            {
                return __first1;
            }
        }
    }
    return __last1;
}

template <typename _InputIter, typename _ForwardIter, typename _Tp1, typename _Tp2>
inline _InputIter _find_first_of_aux1(_InputIter __first1, _InputIter __last1, _ForwardIter __first2, _ForwardIter __last2, _Tp1* __pt1, _Tp2* __pt2)
{
    typedef typename _IsIntegral<_Tp1>::_Ret _IsIntegral;
    typedef typename _IsCharLikeType<_Tp2>::_Ret _IsCharLike;
    typedef typename _Land2<_IsIntegral, _IsCharLike>::_Ret _UseStrcspnLikeAlgo;
    return _find_first_of_aux2(__first1, __last1, __first2, __last2, __pt2, _Identity<bool>(), _UseStrcspnLikeAlgo());
}

template <typename _InputIter, typename _ForwardIter>
_InputIter _find_first_of(_InputIter __first1, _InputIter __last1, _ForwardIter __first2, _ForwardIter __last2)
{
    return _find_first_of_aux1(__first1, __last1, __first2, __last2, _PODSTL_VALUE_TYPE(_InputIter), _PODSTL_VALUE_TYPE(_ForwardIter));
}

template <typename _InputIter, typename _ForwardIter, typename _BinaryPredicate>
_InputIter _find_first_of(_InputIter __first1, _InputIter __last1, _ForwardIter __first2, _ForwardIter __last2, _BinaryPredicate __comp)
{
    for ( ; __first1 != __last1; ++__first1)
    {
        for (_ForwardIter __iter = __first2; __iter != __last2; ++__iter)
        {
            if (__comp(*__first1, *__iter))
            {
                return __first1;
            }
        }
    }
    return __last1;
}

template <class _InputIter, class _ForwardIter>
inline _InputIter find_first_of(_InputIter __first1, _InputIter __last1, _ForwardIter __first2, _ForwardIter __last2) 
{
    return _find_first_of(__first1, __last1, __first2, __last2);
}

template <class _InputIter, class _ForwardIter, class _BinaryPredicate>
inline _InputIter find_first_of(_InputIter __first1, _InputIter __last1, _ForwardIter __first2, _ForwardIter __last2, _BinaryPredicate __comp) 
{
    return __find_first_of(__first1, __last1, __first2, __last2, __comp);
}

template <typename _ForwardIter1, typename _ForwardIter2, typename _BinaryPredicate>
_ForwardIter1 _find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2, _ForwardIter2 __last2,
                         const forward_iterator_tag&, const forward_iterator_tag&, _BinaryPredicate __comp)
{
    if (__first2 == __last2)
    {
        return __last1;
    }

    _ForwardIter1 __result = __last1;
    for (;;)
    {
        _ForwardIter1 __new_result = search(__first1, __last1, __first2, __last2, __comp);
        if (__new_result == __last1)
        {
            return __result;
        }
        else
        {
            __result = __new_result;
            __first1 = __new_result;
            ++__first1;
        }
    }
}

template <typename _BidirectionalIter1, typename _BidirectionalIter2, typename _BinaryPredicate>
_BidirectionalIter1 _find_end(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1, _BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
                               const bidirectional_iterator_tag&, const bidirectional_iterator_tag&, _BinaryPredicate __comp)
{
    typedef reverse_iterator<_BidirectionalIter1> _RevIter1;
    typedef reverse_iterator<_BidirectionalIter2> _RevIter2;

    _RevIter1 __rlast1(__first1);
    _RevIter2 __rlast2(__first2);
    _RevIter1 __rresult = search(_RevIter1(__last1), __rlast1, _RevIter2(__last2), __rlast2, __comp);

    if (__rresult == __rlast1)
    {
        return __last1;
    }

    _BidirectionalIter1 __result = __rresult.base();
    advance(__result, -distance(__first2, __last2));
    return __result;
}

template <typename _ForwardIter1, typename _ForwardIter2, typename _BinaryPredicate>
_ForwardIter1 find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2, _ForwardIter2 __last2, _BinaryPredicate __comp)
{
    return _find_end(__first1, __last1, __first2, __last2, _PODSTL_ITERATOR_CATEGORY(_ForwardIter1), _PODSTL_ITERATOR_CATEGORY(_ForwardIter2), __comp);
}

template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter1 find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2, _ForwardIter2 __last2) 
{
    return _find_end(__first1, __last1, __first2, __last2, _PODSTL_ITERATOR_CATEGORY(_ForwardIter1), _PODSTL_ITERATOR_CATEGORY(_ForwardIter2),
                      _equal_to(_PODSTL_VALUE_TYPE(_ForwardIter1)));
}

template <typename _ForwardIter, typename _Tp>
inline void replace(_ForwardIter __first, _ForwardIter __last, const _Tp& __old_value, const _Tp& __new_value)
{
    for ( ; __first != __last; ++__first)
    {
        if (*__first == __old_value)
        {
            *__first = __new_value;
        }
    }
}

template <typename _ForwardIter, typename _Tp, typename _Compare, typename _Distance>
_ForwardIter _lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp, _Distance*)
{
    _Distance __len = distance(__first, __last);
    _Distance __half;
    _ForwardIter __middle;

    while (__len > 0)
    {
        __half = __len >> 1;
        __middle = __first;
        advance(__middle, __half);
        if (__comp1(*__middle, __val))
        {
            __first = __middle;
            ++__first;
            __len = __len - __half - 1;
        }
        else
        {
            __len = __half;
        }
    }
    return __first;
}

}

#endif // _PODSTL_ALGOBASE_H

