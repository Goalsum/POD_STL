/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义链表容器的非模板实现。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

#include "list"

namespace podstl
{

size_t _ListBase::size() const
{
    size_t __size = 0;
    _NodeBase* __current = __M_node.__M_next;
    while (__current != &__M_node)
    {
        __current = __current->__M_next;
        ++__size;
    }
    return __size;
}

void _ListBase::reverse()
{
    _NodeBase* __tmp = &__M_node;
    do
    {
        swap(__tmp->__M_next, __tmp->__M_prev);
        __tmp = __tmp->__M_prev;     // Old next node is now prev.
    } while (__tmp != &__M_node);
}

_ListBase& _ListBase::_M_assign(const _ListBase& __other, size_t __node_size, size_t __data_size)
{
    if (this != &__other)
    {
        _NodeBase* __first1 = __M_node.__M_next;
        _NodeBase* __last1 = &__M_node;
        const _NodeBase* __first2 = __other.__M_node.__M_next;
        const _NodeBase* __last2 = &(__other.__M_node);
        while ((__first1 != __last1) && (__first2 != __last2))
        {
            VOS_memcpy_s(__first1->_M_get_data(), __data_size, __first2->_M_get_data(), __data_size);
            __first1 = __first1->__M_next;
            __first2 = __first2->__M_next;
        }
        if (__first1 != __last1)
        {
            _M_erase(__first1, __last1);
        }
        else if (__first2 != __last2)
        {
            _M_insert_nodes(__last1, __first2, __last2, __node_size, __data_size);
        }
    }
    return *this;
}

void _ListBase::_M_swap_to_empty(_ListBase& __other)
{
    __other.__M_node.__M_next = __M_node.__M_next;
    __other.__M_node.__M_next->__M_prev = &__other.__M_node;
    __other.__M_node.__M_prev = __M_node.__M_prev;
    __other.__M_node.__M_prev->__M_next = &__other.__M_node;
    _M_empty_initialize();
}

void _ListBase::_M_swap(_ListBase& __other)
{
    if (__other.empty())
    {
        if (empty())
        {
            return;
        }
        _M_swap_to_empty(__other);
    }
    else if (empty())
    {
        __other._M_swap_to_empty(*this);
    }
    else
    {
        swap(__M_node, __other.__M_node);
        swap(__M_node.__M_prev->__M_next, __other.__M_node.__M_prev->__M_next);
        swap(__M_node.__M_next->__M_prev, __other.__M_node.__M_next->__M_prev);
    }
}

_ListBase::_NodeBase* _ListBase::_M_create_node(size_t __node_size, const void* __data, size_t __data_size)
{
    STL_ASSERT(__node_size >= (__data_size + sizeof(_NodeBase)));
    _NodeBase* __pointer = static_cast<_NodeBase*>(STL_Malloc(__node_size));
    VOS_memcpy_s(__pointer->_M_get_data(), __data_size, __data, __data_size);
    return __pointer;
}

_ListBase::_NodeBase* _ListBase::_M_insert_node(_NodeBase* __pos, size_t __node_size, const void* __data, size_t __data_size)
{
    _NodeBase* __new_node = _M_create_node(__node_size, __data, __data_size);
    _NodeBase* __next = __pos;
    _NodeBase* __previous = __next->__M_prev;
    __new_node->__M_next = __next;
    __new_node->__M_prev = __previous;
    __previous->__M_next = __new_node;
    __next->__M_prev = __new_node;
    return __new_node;
}

void _ListBase::_M_insert_nodes(_NodeBase* __pos, const _NodeBase* __first, const _NodeBase* __last, size_t __node_size, size_t __data_size)
{
    for ( ; __first != __last; __first = __first->__M_next)
    {
        _M_insert_node(__pos, __node_size, __first->_M_get_data(), __data_size);
    }
}

void _ListBase::_M_fill_insert(_NodeBase* __pos, size_t __count, size_t __node_size, const void* __data, size_t __data_size)
{
    for ( ; __count > 0; --__count)
    {
        _M_insert_node(__pos, __node_size, __data, __data_size);
    }
}

_ListBase::_NodeBase* _ListBase::_M_erase(_NodeBase* __pos)
{
    _NodeBase* __next_node = __pos->__M_next;
    _NodeBase* __prev_node = __pos->__M_prev;
    __prev_node->__M_next = __next_node;
    __next_node->__M_prev = __prev_node;
    STL_Free(__pos);
    return __next_node;
}

_ListBase::_NodeBase* _ListBase::_M_erase(_NodeBase* __first, _NodeBase* __last)
{
    while (__first != __last)
    {
        __first = _M_erase(__first);
    }
    return __last;
}

void _ListBase::_M_resize(size_t __new_size, size_t __node_size, const void* __fill_data, size_t __data_size)
{
    _NodeBase* __first = __M_node.__M_next;
    size_t __len = 0;
    for (; (__first != &__M_node) && (__len < __new_size); __first = __first->__M_next, ++__len);

    if (__first != &__M_node)
    {
        _M_erase(__first, &__M_node);
    }
    else if (__len < __new_size)
    {
        _M_fill_insert(&__M_node, __new_size - __len, __node_size, __fill_data, __data_size);
    }
}

void _ListBase::_M_fill_assign(size_t __count, size_t __node_size, const void* __data, size_t __data_size)
{
    _NodeBase* __first = __M_node.__M_next;
    for (; (__first != &__M_node) && (__count > 0); __first = __first->__M_next, --__count)
    {
        VOS_memcpy_s(__first->_M_get_data(), __data_size, __data, __data_size);
    }

    if (__count > 0)
    {
        _M_fill_insert(&__M_node, __count, __node_size, __data, __data_size);
    }
    else if (__first != &__M_node)
    {
        _M_erase(__first, &__M_node);
    }
}

void _ListBase::_M_transfer(_NodeBase* __position, _NodeBase* __first, _NodeBase* __last)
{
    if ((__position == __first) || (__position == __last) || (__first == __last)) return;

    __last->__M_prev->__M_next = __position;
    __first->__M_prev->__M_next = __last;
    __position->__M_prev->__M_next = __first;

    _NodeBase* __tmp = __position->__M_prev;
    __position->__M_prev = __last->__M_prev;
    __last->__M_prev = __first->__M_prev;
    __first->__M_prev = __tmp;
}

void _ListBase::_M_remove(const void* __data, _DataBinaryPredicate __equal_pfn)
{
    _NodeBase* __first = __M_node.__M_next;
    _NodeBase* __last = &__M_node;
    while (__first != __last)
    {
        if (__equal_pfn(__data, __first->_M_get_data()))
        {
            __first = _M_erase(__first);
        }
        else
        {
            __first = __first->__M_next;
        }
    }
}

void _ListBase::_M_remove_if(_DataUnaryPredicate __pred_pfn)
{
    _NodeBase* __first = __M_node.__M_next;
    _NodeBase* __last = &__M_node;
    while (__first != __last) 
    {
        if (__pred_pfn(__first->_M_get_data()))
        {
            __first = _M_erase(__first);
        }
        else
        {
            __first = __first->__M_next;
        }
    }
}

void _ListBase::_M_unique(_DataBinaryPredicate __binary_pred_pfn)
{
    _NodeBase* __first = __M_node.__M_next;
    _NodeBase* __last = &__M_node;
    if (__first == __last) return;
    _NodeBase* __next = __first->__M_next;
    while (__next != __last) 
    {
        if (__binary_pred_pfn(__first->_M_get_data(), __next->_M_get_data()))
        {
            __next = _M_erase(__next);
        }
        else
        {
            __first = __next;
            __next = __next->__M_next;
        }
    }
}

void _ListBase::_M_merge(_ListBase& __other, _DataBinaryPredicate __less_pfn)
{
    _NodeBase* __first1 = __M_node.__M_next;
    _NodeBase* __last1 = &__M_node;
    _NodeBase* __first2 = __other.__M_node.__M_next;
    _NodeBase* __last2 = &(__other.__M_node);

    while ((__first1 != __last1) && (__first2 != __last2))
    {
        if (__less_pfn(__first2->_M_get_data(), __first1->_M_get_data()))
        {
            _NodeBase* __next = __first2->__M_next;
            _M_transfer(__first1, __first2, __next);
            __first2 = __next;
        }
        else
        {
            __first1 = __first1->__M_next;
        }
    }

    if (__first2 != __last2)
    {
        _M_transfer(__last1, __first2, __last2);
    }
}

void _ListBase::_M_sort(_DataBinaryPredicate __less_pfn)
{
    // Do nothing if the list has length 0 or 1.
    if (empty() || (__M_node.__M_next->__M_next == &__M_node))
    {
        return;
    }

    _ListBase __carry;
    const int COUNTER_SIZE = 64;
    _ListBase __counter[COUNTER_SIZE];
    int __fill = 0;
    while (!empty())
    {
        _M_transfer(__carry.__M_node.__M_next, __M_node.__M_next, (__M_node.__M_next)->__M_next);
        int __pos = 0;
        while (!__counter[__pos].empty())
        {
            __counter[__pos]._M_merge(__carry, __less_pfn);
            __carry._M_swap(__counter[__pos++]);
            STL_ASSERT(__pos < COUNTER_SIZE);
        }
        __carry._M_swap(__counter[__pos]);
        if (__pos == __fill)
        {
            ++__fill;
        }
    }

    for (int __i = 1; __i < __fill; ++__i)
    {
        __counter[__i]._M_merge(__counter[__i - 1], __less_pfn);
    }
    
    _M_swap(__counter[__fill - 1]);
}

bool _ListBase::_M_equal_to(const _ListBase& __other, _DataBinaryPredicate __equal_pfn) const
{
    const _NodeBase* __first1 = __M_node.__M_next;
    const _NodeBase* __last1 = &__M_node;
    const _NodeBase* __first2 = __other.__M_node.__M_next;
    const _NodeBase* __last2 = &(__other.__M_node);
    while ((__first1 != __last1) && (__first2 != __last2) && __equal_pfn(__first1->_M_get_data(), __first2->_M_get_data()))
    {
        __first1 = __first1->__M_next;
        __first2 = __first2->__M_next;
    }
    return (__first1 == __last1) && (__first2 == __last2);
}

bool _ListBase::_M_less_than(const _ListBase& __other, _DataBinaryPredicate __less_pfn) const
{
    const _NodeBase* __first1 = __M_node.__M_next;
    const _NodeBase* __last1 = &__M_node;
    const _NodeBase* __first2 = __other.__M_node.__M_next;
    const _NodeBase* __last2 = &(__other.__M_node);
    for (; (__first1 != __last1) && (__first2 != __last2); __first1 = __first1->__M_next, __first2 = __first2->__M_next)
    {
        if (__less_pfn(__first1->_M_get_data(), __first2->_M_get_data()))
        {
            return true;
        }
        if (__less_pfn(__first2->_M_get_data(), __first1->_M_get_data()))
        {
            return false;
        }
    }
    return (__first1 == __last1) && (__first2 != __last2);
}

}
