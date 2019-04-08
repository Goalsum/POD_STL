/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义ostream类的非模板实现。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

#include "ostream"

namespace podstl
{

ostream::ostream(streambuf* __buf)
{
    init(__buf);
    __M_locale._M_insert(&__M_num_put, num_put::id);
}

// Output directly from a streambuf.
ostream& ostream::operator<<(streambuf* __from) 
{
    sentry __sentry(*this);
    if (__sentry) 
    {
        if (__from) 
        {
            bool __any_inserted = _M_copy_buffered(__from, rdbuf());
            if (!__any_inserted) setstate(ios_base::failbit);
        }
        else
        {
            setstate(ios_base::badbit);
        }
    }

    return *this;
}

// Helper functions for the streambuf version of operator<<.
bool ostream::_M_copy_buffered(streambuf* __from, streambuf* __to) 
{
    bool __any_inserted = false;

    while (__from->egptr() != __from->gptr()) 
    {
        const ptrdiff_t __avail = __from->egptr() - __from->gptr();

         streamsize __nwritten = __to->sputn(__from->gptr(), __avail);
        __from->gbump(static_cast<int>(__nwritten));

        if (__nwritten == __avail) 
        {
            if (_S_eof(__from->sgetc())) return true;
            else __any_inserted = true;
        }
        else if (__nwritten > 0)
        {
            return true;
        }
        else   // __nwritten == 0
        {
            return __any_inserted;
        }
    }

    // No characters are in the buffer, but we aren't at EOF.  Switch to unbuffered mode.
    return __any_inserted || _M_copy_unbuffered(__from, __to);
}

bool ostream::_M_copy_unbuffered(streambuf* __from, streambuf* __to) 
{
    bool __any_inserted = false;

    for (;;) 
    {
        int_type __char = __from->sbumpc();
        if (_S_eof(__char)) break;

        if (_S_eof(__to->sputc(static_cast<char_type>(__char))))
        {
            __from->sputbackc(static_cast<char_type>(__char));
            break;
        }
        __any_inserted = true;
    }
    return __any_inserted;
}

ostream& ostream::operator<<(short __x) 
{
    return _M_put_num(__x);
}

ostream& ostream::operator<<(unsigned short __x) 
{
    return _M_put_num(__x);
}

ostream& ostream::operator<<(int __x) 
{
    return _M_put_num(__x);
}

ostream& ostream::operator<<(unsigned int __x) 
{
    return _M_put_num(__x);
}

ostream& ostream::operator<<(long __x)
{
    return _M_put_num(__x);
}

ostream& ostream::operator<<(unsigned long __x)
{
    return _M_put_num(__x);
}

ostream& ostream::operator<< (LONG_LONG __x)
{ 
    return _M_put_num(__x);
}

ostream& ostream::operator<< (ULONG_LONG __x)
{ 
    return _M_put_num(__x);
}

ostream& ostream::operator<<(float __x)
{
    return _M_put_num(__x);
}

ostream& ostream::operator<<(double __x)
{
    return _M_put_num(__x);
}

ostream& ostream::operator<<(long double __x)
{
    return _M_put_num(__x);
}

ostream& ostream::operator<<(const void* __x)
{
    return _M_put_num(__x);
}

ostream& ostream::operator<<(bool __x)
{
    return _M_put_num(__x);
}

void ostream::_M_put_char(char_type __char) 
{
    sentry __sentry(*this);
    if (__sentry) 
    {
        bool __failed = true;
        streamsize __npad = (width() > 0) ? (width() - 1) : 0;
        if (__npad == 0)
        {
            __failed = _S_eof(rdbuf()->sputc(__char));
        }
        else if ((flags() & ios_base::adjustfield) == ios_base::left) 
        {
            __failed = _S_eof(rdbuf()->sputc(__char));
            __failed = __failed || (rdbuf()->_M_sputnc(fill(), __npad) != __npad);
        }
        else 
        {
            __failed = rdbuf()->_M_sputnc(fill(), __npad) != __npad;
            __failed = __failed || _S_eof(rdbuf()->sputc(__char));
        }

        width(0);

        if (__failed) setstate(ios_base::badbit);
    }
}

void ostream::_M_put_string(const char_type* __string) 
{
    sentry __sentry(*this);
    if (__sentry) 
    {
        bool __failed = true;
        streamsize __length = VOS_StrLen(__string);
        streamsize __npad = (width() > __length) ? (width() - __length) : 0;

        if (__npad == 0)
        {
            __failed = rdbuf()->sputn(__string, __length) != __length;
        }
        else if ((flags() & ios_base::adjustfield) == ios_base::left) 
        {
            __failed = rdbuf()->sputn(__string, __length) != __length;
            __failed = __failed || (rdbuf()->_M_sputnc(fill(), __npad) != __npad);
        }
        else 
        {
            __failed = rdbuf()->_M_sputnc(fill(), __npad) != __npad;
            __failed = __failed || (rdbuf()->sputn(__string, __length) != __length);
        }

        width(0);

        if (__failed) setstate(ios_base::failbit);
    }
}

// Unformatted output of a single character.
ostream& ostream::put(char_type __char) 
{
    sentry __sentry(*this);
    bool __failed = true;

    if (__sentry) 
    {
        __failed = _S_eof(rdbuf()->sputc(__char));
    }

    if (__failed) setstate(ios_base::badbit);

    return *this;
}

// Unformatted output of a single character.
ostream& ostream::write(const char_type* __string, streamsize __length) 
{
    sentry __sentry(*this);
    bool __failed = true;

    if (__sentry) 
    {
        __failed = rdbuf()->sputn(__string, __length) != __length;
    }

    if (__failed) setstate(ios_base::badbit);

    return *this;
}

}
