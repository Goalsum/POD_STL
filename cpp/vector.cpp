/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义vector容器的非模板实现。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

#include "vector"

namespace podstl
{

_VectorBase::_VectorBase(size_t __count, size_t __unit_size)
{
    __M_start = (__count == 0) ? 0 : STL_Malloc(__count * __unit_size);
    __M_finish = __M_start;
    __M_end_of_storage = _M_get_start() + (__count * __unit_size);
}

// 传入的两个参数分别是待加入的新元素的个数和大小。
size_t _VectorBase::_M_compute_next_size(size_t __count, size_t __unit_size) const
{
    STL_ASSERT((_M_get_finish() >= _M_get_start()) && (__count <= _M_max_size(__unit_size)));
    const size_t __cur_size = _M_get_finish() - _M_get_start();
    size_t __next_size = __cur_size + max(__cur_size, __count * __unit_size);
    if ((__next_size > _M_max_size(__unit_size)) || (__next_size < __cur_size))   // overflow
    {
        __next_size = _M_max_size(__unit_size);
    }
    return __next_size;
}

void _VectorBase::_M_insert_overflow(void* __pos, size_t __count, const void* __data, size_t __unit_size)
{
    STL_ASSERT((static_cast<char*>(__pos) >= _M_get_start()) && (static_cast<char*>(__pos) <= _M_get_finish()) && (__data != NULL));
    size_t __new_size = _M_compute_next_size(__count, __unit_size);
    STL_ASSERT(__new_size > 0);
    void* __new_start = STL_Malloc(__new_size);
    void* __new_finish = _copy_trivial(_M_get_start(), __pos, __new_start);
    __new_finish = _fill_n_trivial(__new_finish, __count, __data, __unit_size);
    __new_finish = _copy_trivial(__pos, _M_get_finish(), __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, static_cast<char*>(__new_start) + __new_size);
}

void _VectorBase::_M_fill_insert_aux(void* __pos, size_t __count, const void* __data, size_t __unit_size)
{
    STL_ASSERT((static_cast<char*>(__pos) >= _M_get_start()) && (static_cast<char*>(__pos) <= _M_get_finish()) && (__data != NULL));
    bool __been_moved = false;  // 解决待插入的元素在原始容器内部并且被移走的问题，false表示待插入的元素不会被移走
    if ((static_cast<const char*>(__data) >= static_cast<const char*>(__pos))
        && (static_cast<const char*>(__data) < _M_get_finish()))
    {
        __been_moved = true;
    }
    __M_finish = _copy_trivial(__pos, _M_get_finish(), static_cast<char*>(__pos) + (__count * __unit_size));
    __data = __been_moved ? (static_cast<const char*>(__data) + (__count * __unit_size)) : __data;
    _fill_n_trivial(__pos, __count, __data, __unit_size);
}

void _VectorBase::_M_fill_insert(void* __pos, size_t __count, const void* __data, size_t __unit_size)
{
    STL_ASSERT((static_cast<char*>(__pos) >= _M_get_start()) && (static_cast<char*>(__pos) <= _M_get_finish()) 
               && (_M_get_end_of_storage() >= _M_get_finish()) && (__data != NULL));
    if (__count <= 0)
    {
        return;
    }

    if (static_cast<size_t>(_M_get_end_of_storage() - _M_get_finish()) >= (__count * __unit_size))
    {
        _M_fill_insert_aux(__pos, __count, __data, __unit_size);
    }
    else
    {
        _M_insert_overflow(__pos, __count, __data, __unit_size);
    }
}

void _VectorBase::_M_reserve(size_t __count)
{
    STL_ASSERT((_M_get_finish() >= _M_get_start()) && (_M_get_end_of_storage() >= _M_get_finish()));
    if (static_cast<size_t>(_M_get_end_of_storage() - _M_get_start()) >= __count)
    {
        return;
    }

    void* __new_start = STL_Malloc(__count);
    __M_finish = _copy_trivial(_M_get_start(), _M_get_finish(), __new_start);
    _M_clear();
    _M_set(__new_start, __M_finish, static_cast<char*>(__new_start) + __count);
}

void _VectorBase::_M_reserve_and_clear(size_t __count)
{
    STL_ASSERT((_M_get_finish() >= _M_get_start()) && (_M_get_end_of_storage() >= _M_get_finish()));
    if (static_cast<size_t>(_M_get_end_of_storage() - _M_get_start()) >= __count)
    {
        return;
    }

    _M_clear();
    __M_start = __M_finish = STL_Malloc(__count);
    __M_end_of_storage = _M_get_start() + __count;
}

void _VectorBase::_M_fill_assign(size_t __count, const void* __data, size_t __unit_size)
{
    _M_reserve_and_clear(__unit_size* __count);
    __M_finish = _fill_n_trivial(_M_get_start(), __count, __data, __unit_size);
}

void _VectorBase::_M_assgin(const void* __start, const void* __finish)
{
    size_t __new_size = static_cast<const char*>(__finish) - static_cast<const char*>(__start);
    _M_reserve_and_clear(__new_size);
    __M_finish = _copy_trivial(__start, __finish, _M_get_start());
}

void _VectorBase::_M_push_back(const void* __data, size_t __unit_size)
{
    STL_ASSERT((_M_get_finish() >= _M_get_start()) && (_M_get_end_of_storage() >= _M_get_finish()) && (__data != NULL));
    if (__M_finish != __M_end_of_storage)
    {
        VOS_memcpy_s(__M_finish, __unit_size, __data, __unit_size);
        __M_finish = _M_get_finish() + __unit_size;
    }
    else
    {
        _M_insert_overflow(__M_finish, 1, __data, __unit_size);
    }
}

bool _VectorBase::operator==(const _VectorBase& __other_vector) const
{
    STL_ASSERT((_M_get_finish() >= _M_get_start()) && (__other_vector._M_get_finish() >= __other_vector._M_get_start()));
    return ((_M_get_finish() - _M_get_start()) == (__other_vector._M_get_finish() - __other_vector._M_get_start()))
           && equal(_M_get_start(), _M_get_finish(), __other_vector._M_get_start());
}

bool _VectorBase::operator<(const _VectorBase& __other_vector) const
{
    STL_ASSERT((_M_get_finish() >= _M_get_start()) && (__other_vector._M_get_finish() >= __other_vector._M_get_start()));
    return lexicographical_compare(_M_get_start(), _M_get_finish(), __other_vector._M_get_start(), __other_vector._M_get_finish());
}

}

