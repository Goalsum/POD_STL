/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义流缓存为string的相关类的实现。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

#include "sstream"

namespace podstl
{

// Constructors. Note that the base class constructor sets all of the get and area pointers to null.
stringbuf::stringbuf(ios_base::openmode __mode /* = ios_base::in | ios_base::out */) : __M_mode(__mode) {}

stringbuf::stringbuf(const string& __string, ios_base::openmode __mode /* = ios_base::in | ios_base::out */) : __M_mode(__mode), __M_str(__string)
{
    _M_set_ptrs();
}

stringbuf::~stringbuf() {}

// Set the underlying string to a new value.
void stringbuf::str(const string& __string)
{
    __M_str = __string;
    _M_set_ptrs();
}

void stringbuf::_M_set_ptrs()
{
    char_type* __data_ptr = __M_str.begin();
    char_type* __data_end = __M_str.end();

    // The initial read position is the beginning of the string.
    if (__M_mode & ios_base::in) 
    {
        setg(__data_ptr, (__M_mode & ios_base::ate) ? __data_end : __data_ptr, __data_end);
    }

    // The initial write position is the beginning of the string.
    if (__M_mode & ios_base::out) 
    {
        setp(__data_ptr, __data_end);
        pbump(static_cast<int>(__M_str.size()));   // initial write position, if we initialized with string 
    }
}

// Precondition: gptr() >= egptr(). Returns a character, if one is available.
stringbuf::int_type stringbuf::underflow() 
{
    return (gptr() < egptr()) ? static_cast<int_type>(*gptr()) : EOF;
}

// Precondition: gptr() >= egptr().
stringbuf::int_type stringbuf::uflow() 
{
    if (gptr() < egptr()) 
    {
        int_type __char = static_cast<int_type>(*gptr());
        gbump(1);
        return __char;
    }
    return EOF;
}

stringbuf::int_type stringbuf::pbackfail(int_type __char) 
{
    if (gptr() == eback()) return EOF;
    STL_ASSERT(gptr() > eback());

    if (__char == EOF)
    {
        gbump(-1);
        return 0;
    }

    if (static_cast<char_type>(__char) == gptr()[-1])
    {
        gbump(-1);
        return __char;
    }

    if (__M_mode & ios_base::out)    // 要放回的字符与流中的不同，覆盖掉流中的字符。
    {
        gbump(-1);
        *gptr() = static_cast<char_type>(__char);
        return __char;
    }

    return EOF;
}

stringbuf::int_type stringbuf::overflow(int_type __char)
{
    if (__char == EOF) return 0;     // __char is EOF, so we don't have to do anything

    if (__M_mode & ios_base::out) 
    {
        if (pptr() < epptr())    // just put back in any case
        { 
            __M_str.push_back(static_cast<char_type>(__char));
            pbump(1);
        }
        else if (__M_mode & ios_base::in) 
        {
            ptrdiff_t __offset = gptr() - eback();
            __M_str.push_back(static_cast<char_type>(__char));
            char_type* __data_ptr = __M_str.begin();
            setg(__data_ptr, __data_ptr + __offset, __M_str.end());
            setp(__data_ptr, __M_str.end());
            pbump(static_cast<int>(__M_str.size()));
        }
        else 
        {
            __M_str.push_back(static_cast<char_type>(__char));
            setp(__M_str.begin(), __M_str.end());
            pbump(static_cast<int>(__M_str.size()));
        }
        return __char;
    }
    return EOF;     // Overflow always fails if it's read-only
}

streamsize stringbuf::xsputn(const char_type* __string, streamsize __length) 
{
    if (((__M_mode & ios_base::out) == 0) || (__length <= 0)) return 0;

    streamsize __nwritten = 0;

    // If the put pointer is somewhere in the middle of the string, then overwrite instead of append.
    if (!__M_str.empty() && (pbase() == __M_str.begin()))
    {
        ptrdiff_t __avail = __M_str.end() - pptr();
        if (__avail >= __length)
        {
            VOS_memcpy_s(pptr(), epptr() - pptr(), __string, static_cast<size_t>(__length));
            pbump(static_cast<int>(__length));
            return __length;
        }
        else 
        {
            VOS_memcpy_s(pptr(), epptr() - pptr(), __string, __avail);
            __nwritten += __avail;
            __length -= __avail;
            __string += __avail;
        }
    }

    // At this point we know we're appending.
    char_type* __data_ptr;
    if (__M_mode & ios_base::in) 
    {
        ptrdiff_t __get_offset = gptr() - eback();
        __M_str.append(__string, __string + static_cast<ptrdiff_t>(__length));
        __data_ptr = __M_str.begin();
        setg(__data_ptr, __data_ptr + __get_offset, __M_str.end());
    }
    else 
    {
        __M_str.append(__string, __string + static_cast<ptrdiff_t>(__length));
        __data_ptr = __M_str.begin();
    }

    setp(__data_ptr, __M_str.end());
    pbump(static_cast<int>(__M_str.size()));
    __nwritten += __length;
    return __nwritten;
}

streamsize stringbuf::_M_xsputnc(char_type __char, streamsize __count) 
{
    if (((__M_mode & ios_base::out) == 0) || (__count <= 0)) return 0;

    streamsize __nwritten = 0;

    // If the put pointer is somewhere in the middle of the string, then overwrite instead of append.
    if (pbase() == __M_str.begin()) 
    {
        ptrdiff_t __avail = __M_str.end() - pptr();
        if (__avail >= __count) 
        {
            VOS_memset_s(pptr(), epptr() - pptr(), __char, static_cast<size_t>(__count));
            pbump(static_cast<int>(__count));
            return __count;
        }
        else 
        {
            VOS_memset_s(pptr(), epptr() - pptr(), __char, __avail);
            __nwritten += __avail;
            __count -= __avail;
        }
    }

    // At this point we know we're appending.
    size_t __app_size = static_cast<size_t>(__count);
    char_type* __data_ptr;
    if (__M_mode & ios_base::in) 
    {
        ptrdiff_t __get_offset = gptr() - eback();
        __M_str.append(__app_size, __char);
        __data_ptr = __M_str.begin();
        setg(__data_ptr, __data_ptr + __get_offset, __M_str.end());
    }
    else 
    {
        __M_str.append(__app_size, __char);
        __data_ptr = __M_str.begin();
    }

    setp(__data_ptr, __M_str.end());
    pbump(static_cast<int>(__M_str.size()));
    __nwritten += __app_size;
    return __nwritten;
}

// According to the C++ standard the effects of setbuf are implementation
// defined, except that setbuf(0, 0) has no effect.  In this implementation,
// setbuf(<anything>, __length), for __length > 0, calls reserve(__length) on the underlying
// string.
streambuf* stringbuf::setbuf(char_type*, streamsize __length) 
{
    if (__length > 0) 
    {
        bool __do_get_area = false;
        bool __do_put_area = false;
        ptrdiff_t __offg = 0;
        ptrdiff_t __offp = 0;

        if (pbase() == __M_str.begin()) 
        {
            __do_put_area = true;
            __offp = pptr() - pbase();
        }

        if (eback() == __M_str.begin()) 
        {
            __do_get_area = true;
            __offg = gptr() - eback();
        }

        __M_str.reserve(static_cast<size_t>(__length));

        char_type* __data_ptr = __M_str.begin();
        if (__do_get_area) 
        {
            setg(__data_ptr, __data_ptr + __offg, __M_str.end());
        }
        if (__do_put_area) 
        {
            setp(__data_ptr, __M_str.end());
            pbump(static_cast<int>(__offp));
        }
    }

    return this;
}

stringbuf::pos_type stringbuf::seekoff(off_type __off, ios_base::seekdir __dir, ios_base::openmode __mode /* = ios_base::in | ios_base::out */) 
{
    __mode &= __M_mode;
    bool __imode = (__mode & ios_base::in) != 0;
    bool __omode = (__mode & ios_base::out) != 0;

    if ((__imode == false) && (__omode == false)) return pos_type(-1);
    if ((__imode && (gptr() == 0)) || (__omode && (pptr() == 0))) return pos_type(-1);
    STL_ASSERT((eback() <= gptr()) && (gptr() <= egptr()) && (pbase() <= pptr()) && (pptr() <= epptr()));

    streamoff __newoff;
    switch (__dir) 
    {
    case ios_base::beg:
        __newoff = 0;
        break;
    case ios_base::end:
        __newoff = __M_str.size();
        break;
    case ios_base::cur:
        __newoff = __imode ? (gptr() - eback()) : (pptr() - pbase());
        if (__off == 0) return pos_type(__newoff);
        break;
    default:
        return pos_type(-1);
    }

    __off += __newoff;

    if (__imode) 
    {
        ptrdiff_t __size = egptr() - eback();
        if ((__off < 0) || (__off > __size)) return pos_type(-1);
        setg(eback(), eback() + static_cast<ptrdiff_t>(__off), eback() + static_cast<ptrdiff_t>(__size));
    }

    if (__omode) 
    {
        ptrdiff_t __size = epptr() - pbase();
        if ((__off < 0) || (__off > __size)) return pos_type(-1);
        setp(pbase(), pbase() + __size);
        pbump(static_cast<int>(__off));
    }

    return pos_type(__off);
}

stringbuf::pos_type stringbuf::seekpos(pos_type __pos, ios_base::openmode __mode /* = ios_base::in | ios_base::out */)
{
    __mode &= __M_mode;

    bool __imode = (__mode & ios_base::in) != 0;
    bool __omode = (__mode & ios_base::out) != 0;

    if ((__imode == false) && (__omode == false)) return pos_type(-1);
    if ((__imode && (gptr() == 0)) || (__omode && (pptr() == 0))) return pos_type(-1);

    const off_type __offset = __pos - pos_type(0);

    if (__imode) 
    {
        if ((__offset < 0) || (__offset > (egptr() - eback()))) return pos_type(-1);
        setg(eback(), eback() + static_cast<ptrdiff_t>(__offset), egptr());
    }

    if (__omode) 
    {
        if ((__offset < 0) || (size_t(__offset) > __M_str.size())) return pos_type(-1);
        setp(__M_str.begin(), __M_str.end());
        pbump(static_cast<int>(__offset));
    }

    return __pos;
}

// istringstream member functions.
istringstream::istringstream(ios_base::openmode __mode /* = ios_base::in */) : istream(0), __M_buf(__mode | ios_base::in) 
{
    init(&__M_buf);
}

istringstream::istringstream(const _String& __str, ios_base::openmode __mode /* = ios_base::in */) : istream(0), __M_buf(__str, __mode | ios_base::in) 
{
    init(&__M_buf);
}

istringstream::~istringstream() {}

// ostringstream member functions.
ostringstream::ostringstream(ios_base::openmode __mode /* = ios_base::out */) : ostream(0), __M_buf(__mode | ios_base::out) 
{
    init(&__M_buf);
}

ostringstream::ostringstream(const _String& __str, ios_base::openmode __mode /* = ios_base::out */) : ostream(0), __M_buf(__str, __mode | ios_base::out)
{
    init(&__M_buf);
}

ostringstream::~ostringstream() {}

// stringstream member functions.
stringstream::stringstream(ios_base::openmode __mode /* = ios_base::in | ios_base::out */) : iostream(0), __M_buf(__mode) 
{
    init(&__M_buf);
}

stringstream::stringstream(const _String& __str, ios_base::openmode __mode /* = ios_base::in | ios_base::out */) : iostream(0), __M_buf(__str, __mode)
{
    init(&__M_buf);
}

stringstream::~stringstream() {}

}
