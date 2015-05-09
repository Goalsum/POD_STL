// 本文件定义流缓存为用户指定大小字符数组的相关类的实现。

// 本文件定义流缓存为string的相关类的实现。

#include "charstream.h"

namespace podstl
{

// Constructors. Note that the base class constructor sets all of the get and area pointers to null.
charbuf::charbuf(ios_base::openmode __mode /* = ios_base::in | ios_base::out */) : __M_mode(__mode), __M_buf(0), __M_size(0) {}

charbuf::charbuf(char* __buf, size_t __size, ios_base::openmode __mode /* = ios_base::in | ios_base::out */) : __M_mode(__mode), __M_buf(__buf), __M_size(__size)
{
    _M_set_ptrs();
}

charbuf::~charbuf() {}

void charbuf::buffer(char* __buf, size_t __size)
{
    __M_buf = __buf;
    __M_size = __size;
    _M_set_ptrs();
}

void charbuf::_M_set_ptrs()
{
    char_type* __data_end = __M_buf + __M_size;

    if (__M_mode & ios_base::in)
    {
        setg(__M_buf, (__M_mode & ios_base::ate) ? __data_end : __M_buf, __data_end);
    }

    if (__M_mode & ios_base::out)
    {
        setp(__M_buf, __data_end);
    }
}

// Precondition: gptr() >= egptr().
charbuf::int_type charbuf::uflow()
{
    return EOF;
}

charbuf::int_type charbuf::pbackfail(int_type __char)
{
    if (gptr() == eback()) return EOF;
    STL_ASSERT(gptr() > eback());

    if (__M_mode & ios_base::out)    // 要放回的字符与流中的不同，覆盖掉流中的字符。
    {
        gbump(-1);
        *gptr() = static_cast<char_type>(__char);
        return __char;
    }

    return EOF;
}

streambuf* charbuf::setbuf(char_type* __buf, streamsize __size)
{
    buffer(__buf, __size);
    return this;
}

charbuf::pos_type charbuf::seekoff(off_type __off, ios_base::seekdir __dir, ios_base::openmode __mode /* = ios_base::in | ios_base::out */)
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
        __newoff = __M_size;
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

charbuf::pos_type charbuf::seekpos(pos_type __pos, ios_base::openmode __mode /* = ios_base::in | ios_base::out */)
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
        if ((__offset < 0) || (size_t(__offset) > __M_size)) return pos_type(-1);
        setp(__M_buf, __M_buf + __M_size);
        pbump(static_cast<int>(__offset));
    }

    return __pos;
}

// icharstream member functions.
icharstream::icharstream(ios_base::openmode __mode /* = ios_base::in */) : istream(0), __M_buf(__mode | ios_base::in)
{
    init(&__M_buf);
}

icharstream::icharstream(const char* __buf, size_t __size, ios_base::openmode __mode /* = ios_base::in */)
    : istream(0), __M_buf(const_cast<char*>(__buf), __size, __mode | ios_base::in)
{
    init(&__M_buf);
}

icharstream::~icharstream() {}

// ocharstream member functions.
ocharstream::ocharstream(ios_base::openmode __mode /* = ios_base::out */) : ostream(0), __M_buf(__mode | ios_base::out)
{
    init(&__M_buf);
}

ocharstream::ocharstream(char* __buf, size_t __size, ios_base::openmode __mode /* = ios_base::out */) : ostream(0), __M_buf(__buf, __size, __mode | ios_base::out)
{
    init(&__M_buf);
}

ocharstream::~ocharstream() {}

// charstream member functions.
charstream::charstream(ios_base::openmode __mode /* = ios_base::in | ios_base::out */) : iostream(0), __M_buf(__mode)
{
    init(&__M_buf);
}

charstream::charstream(char* __buf, size_t __size, ios_base::openmode __mode /* = ios_base::in | ios_base::out */) : iostream(0), __M_buf(__buf, __size, __mode)
{
    init(&__M_buf);
}

charstream::~charstream() {}

}
