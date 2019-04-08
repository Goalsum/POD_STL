/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义istreambuf_iterator类型。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。

#ifndef _PODSTL_ISTREAMBUF_ITERATOR_H
#define _PODSTL_ISTREAMBUF_ITERATOR_H

#include "_iterator_base.h"
#include "streambuf"

namespace podstl
{

// We do not read any characters until operator* is called. operator* calls sgetc
// unless the iterator is unchanged from the last call in which case a cached value is
// used. Calls to operator++ use sbumpc.
class istreambuf_iterator : public iterator<input_iterator_tag, char, streamsize, char*, char&>
{
public:
    typedef char char_type;
    typedef int int_type;
    typedef streambuf streambuf_type;

    typedef input_iterator_tag iterator_category;
    typedef char_type value_type;
    typedef streamoff difference_type;
    typedef const char_type* pointer;
    typedef const char_type& reference;

public:
    istreambuf_iterator(streambuf_type* __buf = 0) { _M_init(__buf); }

    char_type operator*() const 
    { 
        _M_getc(); 
        return __M_char; 
    }

    istreambuf_iterator& operator++() 
    {
        __M_buf->sbumpc();
        __M_have_char = false;
        return *this;
    }

    istreambuf_iterator operator++(int)
    {
        // __tmp should avoid any future actions under underlined buffer---during call of operator *()
        // (due to buffer for *this and __tmp are the same).
        _M_getc();
        istreambuf_iterator __tmp = *this;
        __M_buf->sbumpc();
        __M_have_char = false;
        return __tmp;
    }

    bool equal(const istreambuf_iterator& __other) const 
    {
        if (__M_buf) _M_getc();
        if (__other.__M_buf) __other._M_getc();
        return __M_eof == __other.__M_eof;
    }

    bool operator==(const istreambuf_iterator& __other) const
    {
        return equal(__other);
    }

private:
    void _M_init(streambuf_type* __buf) 
    {
        __M_buf = __buf;
        __M_eof = (__buf == 0);
        __M_have_char = false;
    }

    void _M_getc() const 
    {
        if (__M_have_char) return;
        int_type __char = __M_buf->sgetc();
        __M_char = static_cast<char>(__char);
        __M_eof = (__char == EOF);
        __M_have_char = true;
    }

    streambuf_type* __M_buf;
    mutable char_type __M_char;
    mutable bool __M_eof;
    mutable bool __M_have_char;
};

}

#endif // _PODSTL_ISTREAMBUF_ITERATOR_H
