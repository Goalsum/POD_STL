/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义了一些公用迭代器类型。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。

#ifndef _PODSTL_ITERATOR_BASE_H
#define _PODSTL_ITERATOR_BASE_H

#include "_base_depends.h"

namespace podstl
{

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <typename _Category, typename _Tp, typename _Distance = ptrdiff_t, typename _Pointer = _Tp*, typename _Reference = _Tp& >
struct iterator
{
    typedef _Category  iterator_category;
    typedef _Tp        value_type;
    typedef _Distance  difference_type;
    typedef _Pointer   pointer;
    typedef _Reference reference;
};

template <>
struct iterator<output_iterator_tag, void, void, void, void>
{
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;
};

template <typename _Iterator>
struct iterator_traits
{
    typedef typename _Iterator::iterator_category iterator_category;
    typedef typename _Iterator::value_type        value_type;
    typedef typename _Iterator::difference_type   difference_type;
    typedef typename _Iterator::pointer           pointer;
    typedef typename _Iterator::reference         reference;
};

template <typename _Tp>
struct iterator_traits<const _Tp*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef _Tp                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const _Tp*                  pointer;
    typedef const _Tp&                  reference;
};

template <typename _Tp>
struct iterator_traits<_Tp*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef _Tp                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef _Tp*                        pointer;
    typedef _Tp&                        reference;
};

#define _PODSTL_VALUE_TYPE(_Tp)        (typename iterator_traits< _Tp >::value_type*)0
#define _PODSTL_DISTANCE_TYPE(_Tp)     (typename iterator_traits< _Tp >::difference_type*)0
#define _PODSTL_ITERATOR_CATEGORY(_Tp) (typename iterator_traits< _Tp >::iterator_category())
#define _PODSTL_DIFFERENCE_TYPE(_Iterator) typename iterator_traits<_Iterator>::difference_type

template <typename _Tp>
struct _NonConstTraits;

template <typename _Tp>
struct _ConstTraits
{
    typedef _Tp value_type;
    typedef const _Tp&  reference;
    typedef const _Tp*  pointer;
    typedef _ConstTraits<_Tp> _Const;
    typedef _NonConstTraits<_Tp> _NonConst;
};

template <typename _Tp>
struct _NonConstTraits
{
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
    typedef _ConstTraits<_Tp> _Const;
    typedef _NonConstTraits<_Tp> _NonConst;
};

template <typename _Tp>
struct _AlwaysConstTraits   // 给set容器使用，只允许const迭代器
{
    typedef _Tp value_type;
    typedef const _Tp&  reference;
    typedef const _Tp*  pointer;
    typedef _AlwaysConstTraits<_Tp> _Const;
    typedef _AlwaysConstTraits<_Tp> _NonConst;
};

template <typename _InputIterator>
inline _PODSTL_DIFFERENCE_TYPE(_InputIterator) _distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&)
{
    _PODSTL_DIFFERENCE_TYPE(_InputIterator) __n = 0;
    _InputIterator __it(__first);
    while (__it != __last)
    {
        ++__it;
        ++__n;
    }
    return __n;
}

template <typename _RandomAccessIterator>
inline _PODSTL_DIFFERENCE_TYPE(_RandomAccessIterator) _distance(const _RandomAccessIterator& __first, const _RandomAccessIterator& __last, const random_access_iterator_tag&)
{
    return __last - __first;
}

template <typename _InputIterator>
inline _PODSTL_DIFFERENCE_TYPE(_InputIterator) distance(_InputIterator __first, _InputIterator __last)
{
    return _distance(__first, __last, _PODSTL_ITERATOR_CATEGORY(_InputIterator));
}

template <typename _InputIter, typename _Distance>
inline void _advance(_InputIter& __i, _Distance __n, const input_iterator_tag&)
{
    while (__n--) ++__i;
}

template <typename _OutputIter, typename _Distance>
inline void _advance(_OutputIter& __i, _Distance __n, const output_iterator_tag &)
{
    while (__n--) ++__i;
}

template <typename _BidirectionalIterator, typename _Distance>
inline void _advance(_BidirectionalIterator& __i, _Distance __n, const bidirectional_iterator_tag&)
{
    if (__n > 0)
    {
        while (__n--) ++__i;
    }
    else
    {
        while (__n++) --__i;
    }
}

template <typename _RandomAccessIterator, typename _Distance>
inline void _advance(_RandomAccessIterator& __i, _Distance __n, const random_access_iterator_tag&)
{
    __i += __n;
}

template <typename _Iterator, typename _Distance>
inline void advance(_Iterator& __i, _Distance __n)
{
    _advance(__i, __n, _PODSTL_ITERATOR_CATEGORY(_Iterator));
}

}

#endif // _PODSTL_ITERATOR_BASE_H

