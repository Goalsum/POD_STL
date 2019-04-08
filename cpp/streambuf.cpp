/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义streambuf类的非模板实现。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

#include "streambuf"
#include "_algobase.h"

namespace podstl
{

streambuf::streambuf() : __M_gbegin(0), __M_gnext(0), __M_gend(0), __M_pbegin(0), __M_pnext(0), __M_pend(0)
{}

streambuf::~streambuf() {}

streamsize streambuf::xsgetn(char_type* __buf, streamsize __buf_size)
{
    streamsize __get_count = 0;

    while (__get_count < __buf_size) 
    {
        if (__M_gnext < __M_gend) 
        {
            size_t __chunk = min(static_cast<size_t>(__M_gend - __M_gnext), static_cast<size_t>(__buf_size - __get_count));
            VOS_memcpy_s(__buf, __buf_size - __get_count, __M_gnext, __chunk);
            __get_count += __chunk;
            __buf += __chunk;
            __M_gnext += __chunk;
        }
        else 
        {
            int_type __char = sbumpc();
            if (__char != EOF)
            {
                *__buf = static_cast<char>(__char);
                ++__get_count;
                ++__buf;
            }
            else
            {
                break;
            }
        }
    }

    return __get_count;
}

streamsize streambuf::xsputn(const char_type* __src_buf, streamsize __buf_size)
{
    streamsize __put_count = 0;

    while (__put_count < __buf_size)
    {
        if (__M_pnext < __M_pend) 
        {
            size_t __chunk = min(static_cast<size_t>(__M_pend - __M_pnext), static_cast<size_t>(__buf_size - __put_count));
            VOS_memcpy_s(__M_pnext, __M_pend - __M_pnext, __src_buf, __chunk);
            __put_count += __chunk;
            __src_buf += __chunk;
            __M_pnext += __chunk;
        }
        else if (overflow(static_cast<int>(*__src_buf)) != EOF)
        {
            ++__put_count;
            ++__src_buf;
        }
        else
        {
            break;
        }
    }
    return __put_count;
}

streamsize streambuf::_M_xsputnc(char_type __char, streamsize __count)
{
    streamsize __put_count = 0;

    while (__put_count < __count) 
    {
        if (__M_pnext < __M_pend) 
        {
            size_t __chunk = min(static_cast<size_t>(__M_pend - __M_pnext), static_cast<size_t>(__count - __put_count));
            VOS_memset_s(__M_pnext, __M_pend - __M_pnext, __char, __chunk);
            __put_count += __chunk;
            __M_pnext += __chunk;
        }
        else if (overflow(static_cast<int>(__char)) != EOF)
        {
            ++__put_count;
        }
        else
        {
            break;
        }
    }
    return __put_count;
}

streambuf::int_type streambuf::_M_snextc_underflow()
{
    if (__M_gnext == __M_gend)
    {
        return (uflow() == EOF) ? EOF : sgetc();
    }
    else 
    {
        __M_gnext = __M_gend;
        return underflow();
    }
}

streambuf::int_type streambuf::pbackfail(int_type /* __char = EOF */)
{
    return EOF;
}

streambuf::int_type streambuf::overflow(int_type /* __char = EOF */)
{
    return EOF;
}

streambuf::int_type streambuf::uflow() 
{
    return (underflow() == EOF) ? EOF : static_cast<int>(*__M_gnext++);
}

streambuf::int_type streambuf::underflow()
{
    return EOF;
}

streamsize streambuf::showmanyc()
{
    return 0;
}

int streambuf::sync() { return 0; }

streambuf::pos_type streambuf::seekpos(pos_type, ios_base::openmode /* = ios_base::in | ios_base::out */)
{
    return pos_type(-1);
}

streambuf::pos_type streambuf::seekoff(off_type, ios_base::seekdir, ios_base::openmode /* = ios_base::in | ios_base::out */)
{
    return pos_type(-1);
}

streambuf* streambuf::setbuf(char_type*, streamsize)
{
    return this;
}

}
