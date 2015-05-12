// 本文件定义双端队列容器的非模板实现。

#include "deque"

namespace podstl
{

_DequeIteratorBase::difference_type _DequeIteratorBase::_M_subtract(const _Self& __other, size_t __unit_size) const
{
    return (static_cast<difference_type>(_S_buffer_size(__unit_size)) * (__M_node - __other.__M_node - 1))
           + ((_M_get_cur() - _M_get_first()) / __unit_size) 
           + ((__other._M_get_last() - __other._M_get_cur()) / __unit_size);
}

void _DequeIteratorBase::_M_increment(size_t __unit_size)
{
    __M_cur = _M_get_cur() + __unit_size;
    if (__M_cur == __M_last)
    {
        _M_set_node(__M_node + 1, __unit_size);
        __M_cur = __M_first;
    }
}

void _DequeIteratorBase::_M_decrement(size_t __unit_size)
{
    if (__M_cur == __M_first)
    {
        _M_set_node(__M_node - 1, __unit_size);
        __M_cur = __M_last;
    }
    __M_cur = _M_get_cur() - __unit_size;
}

void _DequeIteratorBase::_M_advance(difference_type __count, size_t __unit_size)
{
    const difference_type BUFFER_SIZE = static_cast<difference_type>(_S_buffer_size(__unit_size));
    difference_type __offset = __count + ((_M_get_cur() - _M_get_first()) / __unit_size);
    if ((__offset >= 0) && (__offset < BUFFER_SIZE))
    {
        __M_cur = _M_get_cur() + (__count * __unit_size);
    }
    else
    {
        difference_type __node_offset = (__offset > 0) ? (__offset / BUFFER_SIZE)
                                                       : (-((-__offset - 1) / BUFFER_SIZE) - 1);
        _M_set_node(__M_node + __node_offset, __unit_size);
        __M_cur = _M_get_first() + ((__offset - (__node_offset * BUFFER_SIZE)) * __unit_size);
    }
}

const size_t _DequeBase::INITIAL_MAP_SIZE;

_DequeBase::~_DequeBase()
{
    if (__M_map)
    {
        _M_destroy_nodes(__M_start.__M_node, __M_finish.__M_node + 1);
        STL_Free(__M_map);
    }
}

void _DequeBase::_M_initialize_map(size_t __num_elements, size_t __unit_size)
{
    size_t __num_nodes = (__num_elements / _S_buffer_size(__unit_size)) + 1;

    __M_map_size = max(INITIAL_MAP_SIZE, __num_nodes + 2);
    __M_map = static_cast<void**>(STL_Malloc(__M_map_size * sizeof(void*)));

    void** __nstart = __M_map + (__M_map_size - __num_nodes) / 2;
    void** __nfinish = __nstart + __num_nodes;

    _M_create_nodes(__nstart, __nfinish, __unit_size);
    __M_start._M_set_node(__nstart, __unit_size);
    __M_finish._M_set_node(__nfinish - 1, __unit_size);
    __M_start.__M_cur = __M_start.__M_first;
    __M_finish.__M_cur = __M_finish._M_get_first() + ((__num_elements % _S_buffer_size(__unit_size)) * __unit_size);
}

void _DequeBase::_M_create_nodes(void** __nstart, void** __nfinish, size_t __unit_size)
{
    for (void** __cur = __nstart; __cur < __nfinish; ++__cur)
    {
        *__cur = STL_Malloc(_S_buffer_size(__unit_size) * __unit_size);
    }
}

void _DequeBase::_M_destroy_nodes(void** __nstart, void** __nfinish)
{
    for (void** __cur = __nstart; __cur < __nfinish; ++__cur)
    {
        STL_Free(*__cur);
    }
}

void _DequeBase::swap(_Self& __other)
{
    podstl::swap(__M_start, __other.__M_start);
    podstl::swap(__M_finish, __other.__M_finish);
    podstl::swap(__M_map, __other.__M_map);
    podstl::swap(__M_map_size, __other.__M_map_size);
}

void _DequeBase::_M_fill_initialize(const void* __data, size_t __unit_size, const _FalseType& /* _TrivialInit */)
{
    void** __cur = __M_start.__M_node;
    for (; __cur < __M_finish.__M_node; ++__cur)
    {
        _fill_n_trivial(*__cur, _S_buffer_size(__unit_size), __data, __unit_size);
    }
    _fill_n_trivial(__M_finish.__M_first, (__M_finish._M_get_cur() - __M_finish._M_get_first()) / __unit_size, __data, __unit_size);
}

void _DequeBase::_M_reserve_map_at_back(size_t __unit_size, size_t __nodes_to_add /* = 1 */)
{
    STL_ASSERT((__M_finish.__M_node >= __M_map) && (__M_finish.__M_node < (__M_map + __M_map_size)));
    if (__nodes_to_add + (__M_finish.__M_node - __M_map) >= __M_map_size)
    {
        _M_reallocate_map(__nodes_to_add, false, __unit_size);
    }
}

void _DequeBase::_M_reserve_map_at_front(size_t __unit_size, size_t __nodes_to_add /* = 1 */)
{
    STL_ASSERT(__M_start.__M_node >= __M_map);
    if (__nodes_to_add > static_cast<size_t>(__M_start.__M_node - __M_map))
    {
        _M_reallocate_map(__nodes_to_add, true, __unit_size);
    }
}

void _DequeBase::_M_reallocate_map(size_t __nodes_to_add, bool __add_at_front, size_t __unit_size)
{
    size_t __old_num_nodes = __M_finish.__M_node - __M_start.__M_node + 1;
    size_t __new_num_nodes = __old_num_nodes + __nodes_to_add;

    void** __new_nstart;
    if (__M_map_size > 2 * __new_num_nodes) // 指针map的空间足够大，不用重新分配。只需要把使用区段移到中间。
    {
        __new_nstart = __M_map + ((__M_map_size - __new_num_nodes) / 2) + (__add_at_front ? __nodes_to_add : 0);
        if (__new_nstart < __M_start.__M_node)
        {
            copy(__M_start.__M_node, __M_finish.__M_node + 1, __new_nstart);
        }
        else
        {
            copy_backward(__M_start.__M_node, __M_finish.__M_node + 1, __new_nstart + __old_num_nodes);
        }
    }
    else    // 指针map的空间不足，需要重新分配。
    {
        size_t __new_map_size = __M_map_size + max(__M_map_size, __nodes_to_add) + 2;
        void** __new_map = static_cast<void**>(STL_Malloc(__new_map_size * __unit_size));
        __new_nstart = __new_map + (__new_map_size - __new_num_nodes) / 2 + (__add_at_front ? __nodes_to_add : 0);
        copy(__M_start.__M_node, __M_finish.__M_node + 1, __new_nstart);
        STL_Free(__M_map);
        __M_map = __new_map;
        __M_map_size = __new_map_size;
    }

    __M_start._M_set_node(__new_nstart, __unit_size);
    __M_finish._M_set_node(__new_nstart + __old_num_nodes - 1, __unit_size);
}

void _DequeBase::_M_push_back_node_end(const void* __data, size_t __unit_size)
{
    _M_reserve_map_at_back(__unit_size);
    *(__M_finish.__M_node + 1) = STL_Malloc(_S_buffer_size(__unit_size) * __unit_size);
    VOS_memcpy_s(__M_finish.__M_cur, __unit_size, __data, __unit_size);
    __M_finish._M_set_node(__M_finish.__M_node + 1, __unit_size);
    __M_finish.__M_cur = __M_finish.__M_first;
}

void _DequeBase::_M_push_back(const void* __data, size_t __unit_size)
{
    if (__M_finish.__M_cur != __M_finish._M_get_last() - __unit_size)
    {
        VOS_memcpy_s(__M_finish.__M_cur, __unit_size, __data, __unit_size);
        __M_finish.__M_cur = __M_finish._M_get_cur() + __unit_size;
    }
    else
    {
        _M_push_back_node_end(__data, __unit_size);
    }
}

void _DequeBase::_M_push_front_node_start(const void* __data, size_t __unit_size)
{
    _M_reserve_map_at_front(__unit_size);
    *(__M_start.__M_node - 1) = STL_Malloc(_S_buffer_size(__unit_size) * __unit_size);
    __M_start._M_set_node(__M_start.__M_node - 1, __unit_size);
    __M_start.__M_cur = __M_start._M_get_last() - __unit_size;
    VOS_memcpy_s(__M_start.__M_cur, __unit_size, __data, __unit_size);
}

void _DequeBase::_M_push_front(const void* __data, size_t __unit_size)
{
    if (__M_start.__M_cur != __M_start.__M_first)
    {
        __M_start.__M_cur = __M_start._M_get_cur() - __unit_size;
        VOS_memcpy_s(__M_start.__M_cur, __unit_size, __data, __unit_size);
    }
    else
    {
        _M_push_front_node_start(__data, __unit_size);
    }
}

void _DequeBase::_M_pop_back(size_t __unit_size)
{
    if (__M_finish.__M_cur != __M_finish.__M_first)
    {
        __M_finish.__M_cur = __M_finish._M_get_cur() - __unit_size;
    }
    else
    {
        STL_Free(__M_finish.__M_first);
        __M_finish._M_set_node(__M_finish.__M_node - 1, __unit_size);
        __M_finish.__M_cur = __M_finish._M_get_last() - __unit_size;
    }
}

void _DequeBase::_M_pop_front(size_t __unit_size)
{
    if (__M_start.__M_cur != __M_start._M_get_last() - __unit_size)
    {
        __M_start.__M_cur = __M_start._M_get_cur() + __unit_size;
    }
    else 
    {
        STL_Free(__M_start.__M_first);
        __M_start._M_set_node(__M_start.__M_node + 1, __unit_size);
        __M_start.__M_cur = __M_start.__M_first;
    }
}

void _DequeBase::_M_new_elements_at_front(size_t __new_elems, size_t __unit_size)
{
    size_t __new_nodes = (__new_elems + _S_buffer_size(__unit_size) - 1) / _S_buffer_size(__unit_size);
    _M_reserve_map_at_front(__unit_size, __new_nodes);
    for (size_t __i = 1; __i <= __new_nodes; ++__i)
    {
        *(__M_start.__M_node - __i) = STL_Malloc(_S_buffer_size(__unit_size) * __unit_size);
    }
}

void _DequeBase::_M_new_elements_at_back(size_t __new_elems, size_t __unit_size)
{
    size_t __new_nodes = (__new_elems + _S_buffer_size(__unit_size) - 1) / _S_buffer_size(__unit_size);
    _M_reserve_map_at_back(__unit_size, __new_nodes);
    for (size_t __i = 1; __i <= __new_nodes; ++__i)
    {
        *(__M_finish.__M_node + __i) = STL_Malloc(_S_buffer_size(__unit_size) * __unit_size);
    }
}

_DequeIteratorBase _DequeBase::_M_reserve_elements_at_front(size_t __count, size_t __unit_size)
{
    size_t __vacancies = (__M_start._M_get_cur() - __M_start._M_get_first()) / __unit_size;
    if (__count > __vacancies)
    {
        _M_new_elements_at_front(__count - __vacancies, __unit_size);
    }
    _DequeIteratorBase __tmp = __M_start;
    __tmp._M_advance(-static_cast<ptrdiff_t>(__count), __unit_size);
    return __tmp;
}

_DequeIteratorBase _DequeBase::_M_reserve_elements_at_back(size_t __count, size_t __unit_size)
{
    size_t __vacancies = ((__M_finish._M_get_last() - __M_finish._M_get_cur()) / __unit_size) - 1;
    if (__count > __vacancies)
    {
        _M_new_elements_at_back(__count - __vacancies, __unit_size);
    }
    _DequeIteratorBase __tmp = __M_finish;
    __tmp._M_advance(static_cast<ptrdiff_t>(__count), __unit_size);
    return __tmp;
}

_DequeIteratorBase _DequeBase::_M_fill_insert_aux(_DequeIteratorBase& __pos, size_t __count, const void* __data, size_t __unit_size)
{
    const ptrdiff_t __elems_before = __pos._M_subtract(__M_start, __unit_size);
    const ptrdiff_t __length = static_cast<ptrdiff_t>(_M_size(__unit_size));
    if (__elems_before <= (__length / 2))
    {
        _DequeIteratorBase __new_start = _M_reserve_elements_at_front(__count, __unit_size);
        __pos = __M_start;
        __pos._M_advance(__elems_before, __unit_size);
        _DequeIteratorBase __dst = __new_start;
        _DequeIteratorBase __src = __M_start;
        for (; __src != __pos; __dst._M_increment(__unit_size), __src._M_increment(__unit_size))
        {
            VOS_memcpy_s(__dst.__M_cur, __unit_size, __src.__M_cur, __unit_size);
        }
        __pos = __dst;
        for (; __count > 0; __dst._M_increment(__unit_size), --__count)
        {
            VOS_memcpy_s(__dst.__M_cur, __unit_size, __data, __unit_size);
        }
        __M_start = __new_start;
    }
    else
    {
        _DequeIteratorBase __new_finish = _M_reserve_elements_at_back(__count, __unit_size);
        const ptrdiff_t __elems_after = __length - __elems_before;
        __pos = __M_finish;
        __pos._M_advance(-__elems_after, __unit_size);
        _DequeIteratorBase __dst = __new_finish;
        _DequeIteratorBase __src = __M_finish;
        for (__src._M_decrement(__unit_size), __dst._M_decrement(__unit_size); __src >= __pos; __src._M_decrement(__unit_size), __dst._M_decrement(__unit_size))
        {
            VOS_memcpy_s(__dst.__M_cur, __unit_size, __src.__M_cur, __unit_size);
        }
        for (; __count > 0; __dst._M_decrement(__unit_size), --__count)
        {
            VOS_memcpy_s(__dst.__M_cur, __unit_size, __data, __unit_size);
        }
        __M_finish = __new_finish;
    }
    return __pos;
}

_DequeIteratorBase _DequeBase::_M_insert_node(_DequeIteratorBase& __pos, const void* __data, size_t __unit_size)
{
    if (__pos == __M_start)
    {
        _M_push_front(__data, __unit_size);
        return __M_start;
    }
    else if (__pos == __M_finish)
    {
        _M_push_back(__data, __unit_size);
        _DequeIteratorBase __tmp = __M_finish;
        __tmp._M_decrement(__unit_size);
        return __tmp;
    }
    else
    {
        return _M_fill_insert_aux(__pos, 1, __data, __unit_size);
    }
}

void _DequeBase::_M_fill_insert(_DequeIteratorBase& __pos, size_t __count, const void* __data, size_t __unit_size)
{
    if (__pos == __M_start) 
    {
        __M_start = _M_reserve_elements_at_front(__count, __unit_size);
        _fill_n_trivial(__M_start.__M_cur, __count, __data, __unit_size);
    }
    else if (__pos == __M_finish)
    {
        _DequeIteratorBase __new_finish = _M_reserve_elements_at_back(__count, __unit_size);
        _fill_n_trivial(__M_finish.__M_cur, __count, __data, __unit_size);
        __M_finish = __new_finish;
    }
    else
    {
        _M_fill_insert_aux(__pos, __count, __data, __unit_size);
    }
}

_DequeIteratorBase _DequeBase::_M_erase(_DequeIteratorBase& __pos, size_t __unit_size)
{
    STL_ASSERT((__pos >= __M_start) && (__pos < __M_finish) && (__M_start <= __M_finish));
    ptrdiff_t __index = __pos._M_subtract(__M_start, __unit_size);
    _DequeIteratorBase __src = __pos;
    _DequeIteratorBase __dst = __pos;
    if (static_cast<size_t>(__index) < (_M_size(__unit_size) / 2))
    {
        __pos._M_increment(__unit_size);
        for (__src._M_decrement(__unit_size); __dst != __M_start; __src._M_decrement(__unit_size), __dst._M_decrement(__unit_size))
        {
            VOS_memcpy_s(__dst.__M_cur, __unit_size, __src.__M_cur, __unit_size);
        }
        _M_pop_front(__unit_size);
    }
    else 
    {
        for (__src._M_increment(__unit_size); __src != __M_finish; __src._M_increment(__unit_size), __dst._M_increment(__unit_size))
        {
            VOS_memcpy_s(__dst.__M_cur, __unit_size, __src.__M_cur, __unit_size);
        }
        _M_pop_back(__unit_size);
    }
    return __pos;
}

_DequeIteratorBase _DequeBase::_M_erase(_DequeIteratorBase& __first, _DequeIteratorBase& __last, size_t __unit_size)
{
    STL_ASSERT((__first >= __M_start) && (__first <= __M_finish) && (__last >= __M_start) && (__last <= __M_finish) && (__first <= __last));
    if (__first == __last) return __first;

    ptrdiff_t _length = __last._M_subtract(__first, __unit_size);
    ptrdiff_t __elems_before = __first._M_subtract(__M_start, __unit_size);
    _DequeIteratorBase __return;
    if (__elems_before <= static_cast<ptrdiff_t>(_M_size(__unit_size) - _length) / 2)
    {
        _DequeIteratorBase __src = __first;
        _DequeIteratorBase __dst = __last;
        for (__src._M_decrement(__unit_size), __dst._M_decrement(__unit_size); __src >= __M_start; __src._M_decrement(__unit_size), __dst._M_decrement(__unit_size))
        {
            VOS_memcpy_s(__dst.__M_cur, __unit_size, __src.__M_cur, __unit_size);
        }
        __dst._M_increment(__unit_size);
        _M_destroy_nodes(__M_start.__M_node, __dst.__M_node);
        __M_start = __dst;
        __return = __last;
    }
    else 
    {
        _DequeIteratorBase __src = __last;
        _DequeIteratorBase __dst = __first;
        for (; __src != __M_finish; __src._M_increment(__unit_size), __dst._M_increment(__unit_size))
        {
            VOS_memcpy_s(__dst.__M_cur, __unit_size, __src.__M_cur, __unit_size);
        }
        _M_destroy_nodes(__dst.__M_node + 1, __M_finish.__M_node + 1);
        __M_finish = __dst;
        __return = __first;
    }
    return __return;
}

_DequeBase& _DequeBase::_M_assign(const _Self& __other, size_t __unit_size)
{
    if (&__other != this)
    {
        const size_t __length = _M_size(__unit_size);
        const size_t __other_length = __other._M_size(__unit_size);
        if (__length < __other_length)
        {
            __M_finish = _M_reserve_elements_at_back(__other_length - __length, __unit_size);
        }

        _DequeIteratorBase __src = __other.__M_start;
        _DequeIteratorBase __dst = __M_start;
        for (; __src != __other.__M_finish; __src._M_increment(__unit_size), __dst._M_increment(__unit_size))
        {
            VOS_memcpy_s(__dst.__M_cur, __unit_size, __src.__M_cur, __unit_size);
        }

        if (__dst != __M_finish)
        {
            _M_erase(__dst, __M_finish, __unit_size);
        }
    }
    return *this;
}

void _DequeBase::_M_fill_assign(size_t __count, const void* __data, size_t __unit_size)
{
    const size_t __length = _M_size(__unit_size);
    if (__length < __count)
    {
        __M_finish = _M_reserve_elements_at_back(__count - __length, __unit_size);
    }

    _DequeIteratorBase __cur = __M_start;
    for (; (__cur != __M_finish) && (__count > 0); __cur._M_increment(__unit_size), --__count)
    {
        VOS_memcpy_s(__cur.__M_cur, __unit_size, __data, __unit_size);
    }

    if (__cur != __M_finish)
    {
        _M_erase(__cur, __M_finish, __unit_size);
    }
}

void _DequeBase::_M_resize(size_t __new_size, const void* __fill_data, size_t __unit_size)
{
    const size_t __length = _M_size(__unit_size);
    if (__length == __new_size) return;
    if (__length < __new_size)
    {
        _DequeIteratorBase __new_finish = _M_reserve_elements_at_back(__new_size - __length, __unit_size);
        for (; __M_finish != __new_finish; __M_finish._M_increment(__unit_size))
        {
            VOS_memcpy_s(__M_finish.__M_cur, __unit_size, __fill_data, __unit_size);
        }
    }
    else // __length > __new_size
    {
        _DequeIteratorBase __new_finish = __M_start;
        __new_finish._M_advance(__new_size, __unit_size);
        _M_erase(__new_finish, __M_finish, __unit_size);
    }
}

}