/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义istream类的非模板实现。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

#include "limits"
#include "istream"
#include "_istreambuf_iterator.h"
#include "_num_get.h"
#include "_delimiter.h"
#include "_algobase.h"

namespace podstl
{

istream& istream::operator>>(short& __val) 
{
    _M_get_num(__val);
    return *this;
}

istream& istream::operator>>(int& __val) 
{
    _M_get_num(__val);
    return *this;
}

istream& istream::operator>>(unsigned short& __val) 
{
    _M_get_num(__val);
    return *this;
}

istream& istream::operator>>(unsigned int& __val) 
{
    _M_get_num(__val);
    return *this;
}

istream& istream::operator>>(long& __val) 
{
     _M_get_num(__val);
    return *this;
}

istream& istream::operator>>(unsigned long& __val) 
{
     _M_get_num(__val);
    return *this;
}

istream& istream::operator>>(LONG_LONG& __val) 
{
     _M_get_num(__val);
    return *this;
}

istream& istream::operator>>(ULONG_LONG& __val) 
{
     _M_get_num(__val);
    return *this;
}

istream& istream::operator>>(float& __val) 
{
     _M_get_num(__val);
    return *this;
}

istream& istream::operator>>(double& __val) 
{
     _M_get_num(__val);
    return *this;
}

istream& istream::operator>>(long double& __val) 
{
     _M_get_num(__val);
    return *this;
}

istream& istream::operator>>(bool& __val) 
{
     _M_get_num(__val);
    return *this;
}

istream& istream::operator>>(void*& __val) 
{
     _M_get_num(__val);
    return *this;
}

// Unformatted input
istream::int_type istream::peek()
{
    int_type __tmp = EOF;
    __M_gcount = 0;
    sentry __sentry(*this, _NoSkipWS());
    if (__sentry)
    {
        __tmp = rdbuf()->sgetc();
        if (_S_eof(__tmp)) setstate(ios_base::eofbit);
    }
    return __tmp;
}

istream::int_type istream::get() 
{
    int_type __tmp = EOF;
    __M_gcount = 0;
    sentry __sentry(*this, _NoSkipWS());
    if (__sentry) 
    {
        __tmp = rdbuf()->sbumpc();
        if (!_S_eof(__tmp)) __M_gcount = 1;
    }
    if (__M_gcount == 0)
    {
        setstate(ios_base::eofbit | ios_base::failbit);
    }
    return __tmp;
}

istream& istream::get(char_type& __char) 
{
    __char = static_cast<char_type>(get());
    return *this;
}

// Read characters and discard them. The standard specifies a single
// function with two arguments, each with a default.  We instead use
// three overloded functions, because it's possible to implement the
// first two more efficiently than the fully general third version.
istream& istream::ignore()
{
    __M_gcount = 0;
    sentry __sentry(*this, _NoSkipWS());
    if (__sentry) 
    {
        int_type __char;
        __char = rdbuf()->sbumpc();
        if (!_S_eof(__char))
        {
            __M_gcount = 1;
        }
        else
        {
            setstate(ios_base::eofbit);
        }
    }
    return *this;
}

istream& istream::putback(char_type __char) 
{
    __M_gcount = 0;
    sentry __sentry(*this, _NoSkipWS());
    if (__sentry)
    {
        int_type __tmp = EOF;
        streambuf* __buf = rdbuf();
        if (__buf) 
        {
            __tmp = __buf->sputbackc(__char);
        }
        if (_S_eof(__tmp))
        {
            setstate(ios_base::badbit);
        }
    }
    else
    {
        setstate(ios_base::failbit);
    }
    return *this;
}

istream& istream::unget() 
{
    __M_gcount = 0;
    sentry __sentry(*this, _NoSkipWS());
    if (__sentry)
    {
        streambuf* __buf = rdbuf();
        if (__buf) 
        {
            if (_S_eof(__buf->sungetc()))
            setstate(ios_base::badbit);
        }
        else
        {
            setstate(ios_base::badbit);
        }
    }
    else
    {
        setstate(ios_base::failbit);
    }
    return *this;
}

// Positioning and buffer control.
int istream::sync() 
{
    sentry __sentry(*this, _NoSkipWS());
    streambuf* __buf = rdbuf();
    if (__buf) 
    {
        if (__buf->pubsync() == -1)
        {
            setstate(ios_base::badbit);
            return -1;
        }
        return 0;
    }
    return -1;
}

istream::pos_type istream::tellg() 
{
    sentry __sentry(*this, _NoSkipWS());
    streambuf* __buf = rdbuf();
    return (__buf && !fail()) ? __buf->pubseekoff(0, ios_base::cur, ios_base::in) : pos_type(-1);
}

istream& istream::seekg(pos_type __pos) 
{
    sentry __sentry(*this, _NoSkipWS());
    streambuf* __buf = rdbuf();
    if (!fail() && __buf)
    {
        if (__buf->pubseekpos(__pos, ios_base::in) == pos_type(-1)) 
        {
            setstate(ios_base::failbit);
        }
    }
    return *this;
}

istream& istream::seekg(off_type __off, ios_base::seekdir __dir) 
{
    sentry __sentry(*this, _NoSkipWS());
    streambuf* __buf = rdbuf();
    if (!fail() && __buf)
    {
        __buf->pubseekoff(__off, __dir, ios_base::in);
    }
    return *this;
}

// Formatted input of characters and character arrays.
void istream::_M_formatted_get(char_type& __char) 
{
    sentry __sentry(*this); // Skip whitespace.
    if (__sentry) 
    {
        int_type __tmp = rdbuf()->sbumpc();
        if (!_S_eof(__tmp))
        {
            __char = static_cast<char_type>(__tmp);
        }
        else
        {
            setstate(ios_base::eofbit | ios_base::failbit);
        }
    }
}

// 从输入流中尝试提取指定个数的字符，并写入输出缓冲区。
// 遇到分隔符，或者EOF都会停止。当__extract_delim为true，且遇到分隔符停止时，提取分隔符（但不将分隔符写入缓冲区）。
// __output_count在输入时指定最大提取字符数，返回时赋为实际往缓冲区中写入的字符数。
// 返回从流中提取的字符总数，注意在__extract_delim为true时，有可能和__output_count的输出值不相等。
// 本函数只用于流缓冲区已用尽的场景。
streamsize istream::_M_read_unbuffered(streamsize& __output_count, char_type* __output_buf, const _Delimiter& __delim, bool __extract_delim)
{
    streambuf* __buf = rdbuf();
    STL_ASSERT(__buf != 0);
    streamsize __count = 0;
    ios_base::iostate __status = 0;
    typedef istream::int_type int_type;

    for (; __count < __output_count; ++__count)
    {
        int_type __char = __buf->sbumpc();
        if (_S_eof(__char)) 
        {
            setstate(eofbit);
            __output_count = __count;
            break;
        }
        if (__delim._M_is_delim(static_cast<char_type>(__char)))
        {
            __output_count = __count;
            if (__extract_delim)   // Extract and discard current character.
            { 
                ++__count;
            }
            else   // leave delimiter
            {
                if (_S_eof(__buf->sputbackc(static_cast<char_type>(__char)))) setstate(failbit);
            }
            break;
        }

        // regular character
        *__output_buf++ = static_cast<char_type>(__char);
    }

    return __count;
}

// 从输入流中尝试提取指定个数的字符，并写入输出缓冲区。
// 遇到分隔符，或者EOF都会停止。当__extract_delim为true，且遇到分隔符停止时，提取分隔符（但不将分隔符写入缓冲区）。
// __output_count在输入时指定最大提取字符数，返回时赋为实际往缓冲区中写入的字符数。
// 返回从流中提取的字符总数，注意在__extract_delim为true时，有可能和__output_count的输出值不相等。
// 本函数用于所有场景。如果流缓冲区用尽，跳到_M_read_unbuffered函数继续执行。
streamsize istream::_M_read_buffered(streamsize& __output_count, char_type* __output_buf, const _Delimiter& __delim, bool __extract_delim)
{
    streambuf* __buf = rdbuf();
    STL_ASSERT(__buf != 0);
    streamsize __count = 0;

    while (__buf->gptr() != __buf->egptr())
    {
        const char_type* __first = __buf->gptr();
        const char_type* __last = __buf->egptr();
        ptrdiff_t __request = __output_count - __count;

        const char_type* __delim_pos = __delim._M_scan_for_delim(__first, __last);
        ptrdiff_t __chunk = min(__delim_pos - __first, __request);
        VOS_memcpy_s(__output_buf, __request, __first, __chunk);
        __output_buf += __chunk;
        __count += __chunk;
        __buf->gbump(static_cast<int>(__chunk));

        // We terminated by finding delim.
        if ((__delim_pos != __last) && ((__delim_pos - __first) <= __request))
        {
            __output_count = __count;
            if (__extract_delim) 
            {
                ++__count;
                __buf->gbump(1);
            }
            return __count;
        }

        if (__count == __output_count)   // We terminated by reading all the characters we were asked for.
        {
            return __count;
        }

        // The buffer contained fewer than __output_count - __count characters.  Either we're
        // at eof, or we should refill the buffer and try again.
        if (_S_eof(__buf->sgetc())) 
        {
            setstate(eofbit);
            __output_count = __count;
            return __count;
        }
    }

    // If execution has reached this point, then we have an empty buffer but
    // we have not reached eof.  What that means is that the streambuf has
    // decided to switch from buffered to unbuffered input. We switch to __read_unbuffered.
    __output_count -= __count;
    streamsize __more = _M_read_unbuffered(__output_count, __output_buf, __delim, __extract_delim);
    __output_count += __count;
    return __count + __more;
}
    
istream& istream::get(char_type* __output_buf, streamsize __count, char_type __delim) 
{
    __M_gcount = 0;
    sentry __sentry(*this, _NoSkipWS());
    if (__sentry) 
    {
        if (__count > 0) 
        {
            --__count;
            __M_gcount = _M_read_buffered(__count, __output_buf, _Delimiter(__delim), false);
            __output_buf[__count] = '\0';
        }
    }

    if (__M_gcount == 0)
    {
        setstate(failbit);
    }

    return *this;
}

// Getline is essentially identical to get, except that it extracts the delimiter.
istream& istream::getline(char_type* __output_buf, streamsize __count, char_type __delim) 
{
    __M_gcount = 0;
    sentry __sentry(*this, _NoSkipWS());
    if (__sentry) 
    {
        if (__count > 0) 
        {
            _Delimiter __delimiter(__delim);
            streamsize __output_count = __count - 1;
            __M_gcount = _M_read_buffered(__output_count, __output_buf, __delimiter, true);
            if ((__output_count == (__count - 1)) && (__M_gcount == __output_count))  // 缓冲区读满，看看是否正好遇到EOF或者分隔符。
            {
                streambuf* __buf = rdbuf();
                int_type __next_char = __buf->sgetc();
                if (_S_eof(__next_char))
                {
                    setstate(eofbit);
                }
                else if (__delimiter._M_is_delim(static_cast<char_type>(__next_char)))
                {
                    __buf->gbump(1);
                }
                else
                {
                    setstate(failbit);
                }
            }
            __output_buf[__output_count] = '\0';
        }
    }

    if (__M_gcount == 0)
    {
        setstate(failbit);
    }

    return *this;
}

// Read n characters.  We don't look for any delimiter, and we don't put in a terminating null character.
istream& istream::read(char_type* __output_buf, streamsize __count) 
{
    __M_gcount = 0;
    sentry __sentry(*this, _NoSkipWS());
    if (__sentry)
    {
        __M_gcount = _M_read_buffered(__count, __output_buf, _Delimiter(true), false);
    }
    else
    {
        setstate(ios_base::failbit);
    }

    if (eof())
    {
        setstate(ios_base::eofbit | ios_base::failbit);
    }

    return *this;
}

// Read n or fewer characters. We don't look for any delimiter, and we don't put in a terminating null character.
streamsize istream::readsome(char_type* __output_buf, streamsize __nmax) 
{
    __M_gcount = 0;
    sentry __sentry(*this, _NoSkipWS());
    if (__sentry)
    {
        streambuf* __buf = rdbuf();
        streamsize __avail = __buf->in_avail();
        if (__avail == -1)
        {
            _M_setstate_nothrow(eofbit);
        }
        else if (__avail == 0) 
        {
            return __M_gcount;
        }
        else   // __avail > 0
        {
            streamsize __output_count = min(__avail, __nmax);
            __M_gcount = _M_read_buffered(__output_count, __output_buf, _Delimiter(true), false);
        }
    }
    else 
    {
        if (eof())
        {
            setstate(ios_base::eofbit | ios_base::failbit);
        }
        else
        {
            setstate(ios_base::failbit);
        }
    }

    return __M_gcount;
}

void istream::_M_formatted_get(char_type* __output_buf) 
{
    sentry __sentry(*this); // Skip whitespace.
    if (__sentry) 
    {
        streamsize __nmax = (width() > 0) ? (width() - 1) : (numeric_limits<streamsize>::max() - 1);
        streamsize __count = _M_read_buffered(__nmax, __output_buf, use_facet<_Delimiter>(getloc()), false);
        __output_buf[__nmax] = '\0';
        if (__count == 0)
        {
            setstate(ios_base::failbit);
        }
    }
    width(0);
}

// 忽略指定个数的字符。遇到分隔符，或者EOF都会停止。当__extract_delim为true，且遇到分隔符停止时，提取分隔符。
// 返回从流中提取的字符总数。
// 本函数只用于流缓冲区已用尽的场景。
streamsize istream::_M_ignore_unbuffered(streamsize __max_count, const _Delimiter& __delim, bool __extract_delim)
{
    streambuf* __buf = rdbuf();
    STL_ASSERT(__buf != 0);
    streamsize __count = 0;
    for (; __count < __max_count; ++__count)
    {
        int_type __char = __buf->sbumpc();
        if (_S_eof(__char)) 
        {
            setstate(eofbit);
            break;
        }
        if (__delim._M_is_delim(static_cast<char_type>(__char)))
        {
            if (__extract_delim)
            {
                ++__count;
            }
            else
            {
                if (_S_eof(__buf->sputbackc(static_cast<char_type>(__char)))) setstate(failbit);
            }
            break;
        }
    }

    return __count;
}

// 忽略指定个数的字符。遇到分隔符，或者EOF都会停止。当__extract_delim为true，且遇到分隔符停止时，提取分隔符。
// 返回从流中提取的字符总数。
// 本函数用于所有场景。如果流缓冲区用尽，跳到_M_ignore_unbuffered函数继续执行。
streamsize istream::_M_ignore_buffered(streamsize __max_count, const _Delimiter& __delim, bool __extract_delim)
{
    streambuf* __buf = rdbuf();
    STL_ASSERT(__buf != 0);
    streamsize __count = 0;

    while (__buf->gptr() != __buf->egptr())
    {
        const char_type* __last = __buf->gptr() + min(__max_count - __count, __buf->egptr() - __buf->gptr());
        const char_type* __delim_pos = __delim._M_scan_for_delim(__buf->gptr(), __last);

        const streamsize __chunk = __delim_pos - __buf->gptr();
        __count += __chunk;
        __buf->gbump(static_cast<int>(__chunk));

        if (__delim_pos != __last)   // 遇到分隔符停止
        {
            if (__extract_delim)
            {
                ++__count;
                __buf->gbump(1);
            }
            return __count;
        }

        if (__count == __max_count)     // 达到指定字符数停止
        {
            return __count;
        }

        STL_ASSERT(__buf->gptr() == __buf->egptr());

        // 到达buffer末尾时还没有达到指定字符数，检查是否文件结束。
        if (_S_eof(__buf->sgetc()))
        {
            setstate(eofbit);
            return __count;
        }
    }

    // If execution has reached this point, then we have an empty buffer but
    // we have not reached eof.  What that means is that the streambuf has
    // decided to switch from buffered to unbuffered input. We switch to _M_ignore_unbuffered.
    return __count + _M_ignore_unbuffered(__max_count, __delim, __extract_delim);
}

istream& istream::ignore(streamsize __count) 
{
    __M_gcount = 0;
    sentry __sentry(*this, _NoSkipWS());
    if (__sentry) 
    {
        __M_gcount = _M_ignore_buffered(__count, _Delimiter(true), false);
    }

    return *this;
}

istream& istream::ignore(streamsize __count, int_type __delim) 
{
    __M_gcount = 0;
    sentry __sentry(*this, _NoSkipWS());
    if (__sentry) 
    {
        __M_gcount = _M_ignore_buffered(__count, _Delimiter(static_cast<char_type>(__delim)), true);
    }

    return *this;
}

// This member function does not construct a sentry object, because it is called from sentry's constructor.
void istream::_M_skip_whitespace(bool __set_failbit) 
{
    if (rdbuf() == 0)
    {
        setstate(ios_base::badbit);
        return;
    }

    // 此函数用于跳过分隔符，遇到非分隔符停止。需要传入一个反转判断分隔符对象。
    _M_ignore_buffered(numeric_limits<streamsize>::max(), _Delimiter(use_facet<_Delimiter>(getloc()), true), false);
    if (__set_failbit && eof())
    {
        setstate(failbit);
    }
}

// 将字符输出到__dest流中，遇到分隔符，或者EOF，或者输出失败时停止。
// 本函数只用于流缓冲区已用尽的场景。
streamsize istream::_M_copy_unbuffered(streambuf* __dest, const _Delimiter& __delim, bool __extract_delim) 
{
    streambuf* __src = rdbuf();
    STL_ASSERT(__src != 0);
    streamsize __extracted = 0;

    for (;; ++__extracted)
    {
        int_type __char = __src->sbumpc();

        // If we failed to get a character, then quit.
        if (_S_eof(__char)) 
        {
            setstate(eofbit);
            break;
        }

        // If it's the delimiter, then quit.
        if (__delim._M_is_delim(static_cast<char_type>(__char)))
        {
            if (!__extract_delim && _S_eof(__src->sputbackc(static_cast<char_type>(__char)))) setstate(failbit);
            break;
        }

        // Try to put the character in the output streambuf.
        if (_S_eof(__dest->sputc(static_cast<char_type>(__char))))
        {
            // If we failed to put the character in the output streambuf, then try to push it back to the input streambuf.
            if (_S_eof(__src->sputbackc(static_cast<char_type>(__char)))) setstate(failbit);
            break;
        }
    }

    return __extracted;
}

// 将字符输出到__dest流中，遇到分隔符，或者EOF，或者输出失败时停止。
// 本函数用于所有场景。如果流缓冲区用尽，跳到_M_copy_unbuffered函数继续执行。
streamsize istream::_M_copy_buffered(streambuf* __dest, const _Delimiter& __delim, bool __extract_delim)
{
    streambuf* __src = rdbuf();
    STL_ASSERT(__src != 0);
    streamsize __extracted = 0;

    int_type __char = __src->sgetc();   // 先抽取一个字符，在缓冲区为空的时候可以触发重新充填。
    for (; __src->gptr() < __src->egptr(); __char = __src->sgetc())
    {
        char_type* __first = __src->gptr();
        ptrdiff_t __avail = __src->egptr() - __first;

        const char_type* __last = __delim._M_scan_for_delim(__first, __src->egptr());
        ptrdiff_t __chunk = __last - __first;
        if ((__last != __src->egptr()) && __extract_delim) ++__chunk;   // 连分隔符一起抽取

        // Try to copy the entire input buffer to the output buffer.
        streamsize __count = __dest->sputn(__first, __chunk);
        __src->gbump(static_cast<int>(__count));
        __extracted += __count;

        // We found the delimiter, or else failed to copy some characters.
        if (__count < __avail)
        {
            return __extracted;
        }
    }

    if (_S_eof(__char))
    {
        setstate(eofbit);
        return __extracted;
    }

    return __extracted + _M_copy_unbuffered(__dest, __delim, __extract_delim);
}
    
istream& istream::get(streambuf& __dest, char_type __delim)
{
    __M_gcount = 0;
    sentry __sentry(*this, _NoSkipWS());
    if (__sentry) 
    {
        __M_gcount = _M_copy_buffered(&__dest, _Delimiter(__delim), false);
    }

    if (__M_gcount == 0)
    {
        setstate(ios_base::failbit);
    }

    return *this;
}

// Copying characters into a streambuf.
istream& istream::operator>>(streambuf* __dest) 
{
    streamsize __count = 0;
    typedef istream::sentry _Sentry;
    _Sentry __sentry(*this);
    if (__sentry && (__dest != 0))
    {
        __count = _M_copy_buffered(__dest, _Delimiter(true), false);
    }

    if (__count == 0)
    {
        setstate(ios_base::failbit);
    }

    return *this;
}

}
