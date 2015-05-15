// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。
// 本文件定义string类的基类，包含缓冲区管理操作。

#ifndef _PODSTL_STRING_BASE_H
#define _PODSTL_STRING_BASE_H

#include "_base_depends.h"

namespace podstl
{

class _StringBase 
{
    typedef _StringBase _Self;

public:
    enum { _DEFAULT_SIZE = 16 };
    typedef size_t size_type;

protected:
    char const* _M_start() const 
    { 
        return __M_start; 
    }

    char* _M_start()
    {
        return __M_start;
    }

    char const* _M_finish() const
    {
        return __M_finish;
    }

    char* _M_finish()
    {
        return __M_finish;
    }

    char const* _M_end_of_storage() const
    { 
        return __M_end_of_storage; 
    }

    char* _M_end_of_storage()
    { 
        return __M_end_of_storage; 
    }

    size_type _M_capacity() const
    { 
        return _M_end_of_storage() - _M_start();
    }

    size_type _M_rest() const
    { 
        return  _M_end_of_storage() - _M_finish();
    }

    size_t max_size() const 
    {
        return (static_cast<size_type>(-1) / sizeof(char)) - 1;
    }

    _StringBase() : __M_start(&s_shared_null), __M_finish(&s_shared_null), __M_end_of_storage(&s_shared_null + 1) {}

    void _M_allocate_block(size_t __block_size = _DEFAULT_SIZE)
    {
        if (__block_size < _DEFAULT_SIZE)
        {
            __block_size = _DEFAULT_SIZE;
        }
        __M_start = static_cast<char*>(STL_Malloc(__block_size));
        __M_finish = __M_start;
        __M_end_of_storage = __M_start + __block_size;
    }

    _StringBase(size_t __block_size);

    void _M_deallocate_block()
    {
        STL_ASSERT(__M_start != NULL);
        if (__M_start != &s_shared_null)
        {
            STL_Free(__M_start);
        }
    }

    ~_StringBase();

    void _M_reset(char* __start, char* __finish, char* __end_of_storage) 
    {
        __M_start = __start;
        __M_finish = __finish;
        __M_end_of_storage = __end_of_storage;
    }

    void _M_swap(_Self& __other_string)
    {
        swap(__M_end_of_storage, __other_string.__M_end_of_storage);
        swap(__M_start, __other_string.__M_start);
        swap(__M_finish, __other_string.__M_finish);
    }

    char* __M_start;
    char* __M_finish;
    char* __M_end_of_storage;

    static char s_shared_null; // 供所有刚初始化的空字符串公用
};

}

#endif // _PODSTL_STRING_BASE_H
