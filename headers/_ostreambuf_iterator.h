// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。
// 本文件定义ostreambuf_iterator类型。

#ifndef _PODSTL_OSTREAMBUF_ITERATOR_H
#define _PODSTL_OSTREAMBUF_ITERATOR_H

#include "_iterator_base.h"
#include "streambuf"

namespace podstl
{

class ostreambuf_iterator : public iterator<output_iterator_tag, void, void, void, void>
{
public:
    typedef char char_type;
    typedef int int_type;
    typedef streambuf streambuf_type;

    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

public:
    ostreambuf_iterator(streambuf_type* __buf) : __M_buf(__buf), __M_ok(__buf != 0) {}

    ostreambuf_iterator& operator=(char_type __c) 
    {
        __M_ok = __M_ok && (__M_buf->sputc(__c) != EOF);
        return *this;
    }

    ostreambuf_iterator& operator*() { return *this; }
    ostreambuf_iterator& operator++() { return *this; }
    ostreambuf_iterator& operator++(int) { return *this; }

    bool failed() const { return !__M_ok; }

private:
    streambuf_type* __M_buf;
    bool __M_ok;
};

}

#endif // _PODSTL_OSTREAMBUF_ITERATOR_H
