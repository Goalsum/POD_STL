// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。
// 本文件定义输入输出流迭代器。

#ifndef _PODSTL_STREAM_ITERATOR_H
#define _PODSTL_STREAM_ITERATOR_H

#include "_iterator_base.h"
#include "_algobase.h"
#include "_ostreambuf_iterator.h"
#include "_istreambuf_iterator.h"
#include "istream"
#include "_operator.h"

namespace podstl
{

template <typename _Tp>
class istream_iterator : public iterator<input_iterator_tag, _Tp, ptrdiff_t, const _Tp*, const _Tp& > 
{
    typedef istream_iterator<_Tp> _Self;

public:
    typedef char char_type;
    typedef istream istream_type;
    typedef input_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef ptrdiff_t difference_type;
    typedef const _Tp* pointer;
    typedef const _Tp& reference;

    istream_iterator() : __M_stream(0), __M_ok(false), __M_read_done(true) {}
    istream_iterator(istream_type& __stream) : __M_stream(&__stream), __M_ok(false), __M_read_done(false) {}

    reference operator*() const 
    {
        if (!__M_read_done) 
        {
            _M_read();
        }
        return __M_value;
    }

    pointer operator->() const { return &(operator*()); }

    _Self& operator++() 
    {
        _M_read();
        return *this;
    }

    _Self operator++(int)  
    {
        _Self __tmp = *this;
        _M_read();
        return __tmp;
    }

    bool _M_equal(const _Self& __other) const 
    {
        if (!__M_read_done) 
        {
            _M_read();
        }
        if (!__other.__M_read_done) 
        {
            __other._M_read();
        }
        return (__M_ok == __other.__M_ok) && (!__M_ok || __M_stream == __other.__M_stream);
    }

    inline bool operator==(const _Self& __other) const
    {
        return _M_equal(__other);
    }

private:
    istream_type* __M_stream;
    mutable _Tp __M_value;
    mutable bool __M_ok;
    mutable bool __M_read_done;

    void _M_read() const 
    {
        __M_ok = ((__M_stream != 0) && !__M_stream->fail());
        if (__M_ok) 
        {
            *__M_stream >> __M_value;
            __M_ok = !__M_stream->fail();
        }
        __M_read_done = true;
    }
};

template <typename _Tp>
class ostream_iterator : public iterator<output_iterator_tag, void, void, void, void>
{
    typedef ostream_iterator<_Tp> _Self;

public:
    typedef char char_type;
    typedef ostream ostream_type;
    typedef output_iterator_tag iterator_category;

    ostream_iterator(ostream_type& __stream) : __M_stream(&__stream), __M_string(0) {}
    ostream_iterator(ostream_type& __stream, const char_type* __c) : __M_stream(&__stream), __M_string(__c) {}

    _Self& operator=(const _Tp& __val) 
    {
        *__M_stream << __val;
        if (__M_string) *__M_stream << __M_string;
        return *this;
    }

    _Self& operator*() { return *this; }
    _Self& operator++() { return *this; }
    _Self& operator++(int) { return *this; }

private:
    ostream_type* __M_stream;
    const char_type* __M_string;
};

}

#endif // _PODSTL_STREAM_ITERATOR_H
