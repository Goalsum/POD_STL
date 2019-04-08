/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义了一些迭代器类型。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。

#ifndef _PODSTL_ITERATOR_H
#define _PODSTL_ITERATOR_H

#include "_iterator_base.h"

namespace podstl
{

template <typename _Iterator>
class reverse_iterator : public iterator<typename iterator_traits<_Iterator>::iterator_category, typename iterator_traits<_Iterator>::value_type,
                                         typename iterator_traits<_Iterator>::difference_type, typename iterator_traits<_Iterator>::pointer,
                                         typename iterator_traits<_Iterator>::reference>
{
protected:
    _Iterator current;
    typedef reverse_iterator<_Iterator> _Self;

public:
    typedef typename iterator_traits<_Iterator>::difference_type difference_type;
    typedef typename iterator_traits<_Iterator>::pointer pointer;
    typedef typename iterator_traits<_Iterator>::reference reference;
    typedef _Iterator iterator_type;

public:
    reverse_iterator() {}
    explicit reverse_iterator(iterator_type __x) : current(__x) {}
    reverse_iterator(const _Self& __x) : current(__x.current) {}
    _Self& operator=(const _Self& __x)
    {
        current = __x.base();
        return *this;
    }

    template <typename _Iter>
    reverse_iterator(const reverse_iterator<_Iter>& __x) : current(__x.base()) {}
    template <typename _Iter>
    _Self& operator=(const reverse_iterator<_Iter>& __x)
    {
        current = __x.base();
        return *this;
    }

    iterator_type base() const { return current; }
    reference operator*() const
    {
        _Iterator __tmp = current;
        return *--__tmp;
    }
    pointer operator->() const { return &(operator*()); }

    _Self& operator++()
    {
        --current;
        return *this;
    }
    _Self operator++(int)
    {
        _Self __tmp = *this;
        --current;
        return __tmp;
    }
    _Self& operator--()
    {
        ++current;
        return *this;
    }
    _Self operator--(int)
    {
        _Self __tmp = *this;
        ++current;
        return __tmp;
    }

    _Self operator+(difference_type __n) const { return _Self(current - __n); }
    _Self& operator+=(difference_type __n)
    {
        current -= __n;
        return *this;
    }
    _Self operator-(difference_type __n) const { return _Self(current + __n); }
    _Self& operator-=(difference_type __n)
    {
        current += __n;
        return *this;
    }
    reference operator[](difference_type __n) const { return *(*this + __n); }
};

template <typename _Iterator>
inline bool operator==(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y)
{
    return __x.base() == __y.base();
}

template <typename _Iterator>
inline bool operator<(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y)
{
    return __y.base() < __x.base();
}

template <typename _Iterator>
inline bool operator!=(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y)
{
    return !(__x == __y);
}

template <typename _Iterator>
inline bool operator>(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y)
{
    return __y < __x;
}

template <typename _Iterator>
inline bool operator<=(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y)
{
    return !(__y < __x);
}

template <typename _Iterator>
inline bool operator>=(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y)
{
    return !(__x < __y);
}

template <typename _Iterator>
inline typename reverse_iterator<_Iterator>::difference_type operator-(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y)
{
    return __y.base() - __x.base();
}

template <typename _Iterator, typename _DifferenceType>
inline reverse_iterator<_Iterator> operator+(_DifferenceType n, const reverse_iterator<_Iterator>& x)
{
    return x.operator+(n);
}

template <typename _Container>
class back_insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
{
    typedef back_insert_iterator<_Container> _Self;

protected:
    _Container *container;

public:
    typedef _Container          container_type;
    typedef output_iterator_tag iterator_category;

    explicit back_insert_iterator(_Container& __x) : container(&__x) {}

    _Self& operator=(const _Self& __other)
    {
        container = __other.container;
        return *this;
    }

    _Self& operator=(const typename _Container::value_type& __val)
    {
        container->push_back(__val);
        return *this;
    }

    _Self& operator*() { return *this; }
    _Self& operator++() { return *this; }
    _Self  operator++(int) { return *this; }
};

template <typename _Container>
inline back_insert_iterator<_Container> back_inserter(_Container& __x)
{
    return back_insert_iterator<_Container>(__x);
}

template <typename _Container>
class front_insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
{
    typedef front_insert_iterator<_Container> _Self;

protected:
    _Container *container;

public:
    typedef _Container          container_type;
    typedef output_iterator_tag iterator_category;
    explicit front_insert_iterator(_Container& __x) : container(&__x) {}

    _Self& operator=(const _Self& __other)
    {
        container = __other.container;
        return *this;
    }

    _Self& operator=(const typename _Container::value_type& __val)
    {
        container->push_front(__val);
        return *this;
    }

    _Self& operator*() { return *this; }
    _Self& operator++() { return *this; }
    _Self  operator++(int) { return *this; }
};

template <typename _Container>
inline front_insert_iterator<_Container> front_inserter(_Container& __x)
{
    return front_insert_iterator<_Container>(__x);
}

template <typename _Container>
class insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
{
    typedef insert_iterator<_Container> _Self;

protected:
    _Container *container;
    typename _Container::iterator _M_iter;

public:
    typedef _Container          container_type;
    typedef output_iterator_tag iterator_category;
    insert_iterator(_Container& __x, typename _Container::iterator __i) : container(&__x), _M_iter(__i) {}

    _Self& operator=(_Self const& __other)
    {
        container = __other.container;
        _M_iter = __other._M_iter;
        return *this;
    }

    _Self& operator=(const typename _Container::value_type& __val)
    {
        _M_iter = container->insert(_M_iter, __val);
        ++_M_iter;
        return *this;
    }

    _Self& operator*() { return *this; }
    _Self& operator++() { return *this; }
    _Self& operator++(int) { return *this; }
};

template <typename _Container, typename _Iterator>
inline insert_iterator<_Container> inserter(_Container& __x, _Iterator __i)
{
    typedef typename _Container::iterator __iter;
    return insert_iterator<_Container>(__x, __iter(__i));
}

}

#endif // _PODSTL_ITERATOR_H

