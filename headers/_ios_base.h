/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义ios的基类。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。

#ifndef _PODSTL_IOS_BASE_H
#define _PODSTL_IOS_BASE_H

#include "locale"
#include "_pair.h"

namespace podstl
{

typedef ptrdiff_t streamoff;
typedef ptrdiff_t streamsize;

// Class ios_base.  This is the base class of the ios hierarchy, which
// includes basic_istream and basic_ostream.  Classes in the ios
// hierarchy are actually quite simple: they are just glorified
// wrapper classes.  They delegate buffering and physical character
// manipulation to the streambuf classes, and they delegate most
// formatting tasks to a locale.
class ios_base 
{
public:
    typedef int fmtflags;
    typedef int iostate;
    typedef int openmode;
    typedef int seekdir;

    // Formatting flags.
    static const int left = 0x0001;
    static const int right = 0x0002;
    static const int internal = 0x0004;
    static const int dec = 0x0008;
    static const int hex = 0x0010;
    static const int oct = 0x0020;
    static const int fixed = 0x0040;
    static const int scientific = 0x0080;
    static const int boolalpha = 0x0100;
    static const int showbase = 0x0200;
    static const int showpoint = 0x0400;
    static const int showpos = 0x0800;
    static const int skipws = 0x1000;
    static const int unitbuf = 0x2000;
    static const int uppercase = 0x4000;
    static const int adjustfield = left | right | internal;
    static const int basefield = dec | hex | oct;
    static const int floatfield = scientific | fixed;

    // State flags.
    static const int goodbit = 0x00;
    static const int badbit = 0x01;
    static const int eofbit = 0x02;
    static const int failbit = 0x04;

    // Openmode flags.
    static const int DEFAULT_MODE = 0x0; // implementation detail
    static const int app = 0x01;
    static const int ate = 0x02;
    static const int binary = 0x04;
    static const int in = 0x08;
    static const int out = 0x10;
    static const int trunc = 0x20;

    // Seekdir flags
    static const int beg = 0x01;
    static const int cur = 0x02;
    static const int end = 0x04;

    // Flag-manipulation functions.
    fmtflags flags() const { return __M_fmtflags; }

    fmtflags flags(fmtflags __flags) 
    {
        fmtflags __tmp = __M_fmtflags;
        __M_fmtflags = __flags;
        return __tmp;
    }

    fmtflags setf(fmtflags __flag) 
    {
        fmtflags __tmp = __M_fmtflags;
        __M_fmtflags |= __flag;
        return __tmp;
    }

    fmtflags setf(fmtflags __flag, fmtflags __mask) 
    {
        fmtflags __tmp = __M_fmtflags;
        __M_fmtflags &= ~__mask;
        __M_fmtflags |= __flag & __mask;
        return __tmp;
    }

    void unsetf(fmtflags __mask) { __M_fmtflags &= ~__mask; }

    streamsize precision() const { return __M_precision; }

    streamsize precision(streamsize __newprecision) 
    {
        streamsize __tmp = __M_precision;
        __M_precision = __newprecision;
        return __tmp;
    }

    streamsize width() const { return __M_width; }

    streamsize width(streamsize __newwidth) 
    {
        streamsize __tmp = __M_width;
        __M_width = __newwidth;
        return __tmp;
    }

    // Locales
    locale imbue(const locale& __loc);
    locale getloc() const { return __M_locale; }

    virtual ~ios_base() {}

    // The C++ standard requires only that these member functions be defined in basic_ios.
    // We define them in the non-template base class to avoid code duplication.
    operator void*() const { return !fail() ? const_cast<ios_base*>(this) : 0; }
    bool operator!() const { return fail(); }

    iostate rdstate() const { return __M_iostate; }

    bool good() const { return __M_iostate == 0; }
    bool eof() const { return (__M_iostate & eofbit) != 0; }
    bool fail() const { return (__M_iostate & (failbit | badbit)) != 0; }
    bool bad() const { return (__M_iostate & badbit) != 0; }

protected:
    // Copies the state of __other to *this.  This member function makes it
    // possible to implement basic_ios::copyfmt without having to expose
    // ios_base's private data members.  Does not copy __M_exception_mask
    // or __M_iostate.
    void _M_copy_state(const ios_base& __other);

    void _M_setstate_nothrow(iostate __state) { __M_iostate |= __state; }
    void _M_clear_nothrow(iostate __state) { __M_iostate = __state; }
    iostate _M_get_exception_mask() const { return __M_exception_mask; }
    void _M_set_exception_mask(iostate __mask) { __M_exception_mask = __mask; }

    ios_base();                   // Default constructor.

    locale __M_locale;   // 子类可以访问

private:                        // Invalidate the copy constructor and assignment operator.
    ios_base(const ios_base&);
    void operator=(const ios_base&);

private:                        // Data members.
    fmtflags __M_fmtflags;         // Flags
    iostate __M_iostate;
    openmode __M_openmode;
    seekdir __M_seekdir;
    iostate __M_exception_mask;

    streamsize __M_precision;
    streamsize __M_width;
};

// ios_base manipulator functions, from section 27.4.5 of the C++ standard.
// All of them are trivial one-line wrapper functions.

// fmtflag manipulators, section 27.4.5.1
inline ios_base& boolalpha(ios_base& __ios)
{
    __ios.setf(ios_base::boolalpha); 
    return __ios;
}

inline ios_base& noboolalpha(ios_base& __ios)
{
    __ios.unsetf(ios_base::boolalpha); 
    return __ios;
}

inline ios_base& showbase(ios_base& __ios)
{
    __ios.setf(ios_base::showbase); 
    return __ios;
}

inline ios_base& noshowbase(ios_base& __ios)
{
    __ios.unsetf(ios_base::showbase); 
    return __ios;
}

inline ios_base& showpoint(ios_base& __ios)
{
    __ios.setf(ios_base::showpoint); 
    return __ios;
}

inline ios_base& noshowpoint(ios_base& __ios)
{
    __ios.unsetf(ios_base::showpoint); 
    return __ios;
}

inline ios_base& showpos(ios_base& __ios)
{
    __ios.setf(ios_base::showpos); 
    return __ios;
}

inline ios_base& noshowpos(ios_base& __ios)
{
    __ios.unsetf(ios_base::showpos); 
    return __ios;
}

inline ios_base& skipws(ios_base& __ios)
{
    __ios.setf(ios_base::skipws); 
    return __ios;
}

inline ios_base& noskipws(ios_base& __ios)
{
    __ios.unsetf(ios_base::skipws); 
    return __ios;
}

inline ios_base& uppercase(ios_base& __ios)
{
    __ios.setf(ios_base::uppercase); 
    return __ios;
}

inline ios_base& nouppercase(ios_base& __ios)
{
    __ios.unsetf(ios_base::uppercase); 
    return __ios;
}

inline ios_base& unitbuf(ios_base& __ios)
{
    __ios.setf(ios_base::unitbuf); 
    return __ios;
}

inline ios_base& nounitbuf(ios_base& __ios)
{
    __ios.unsetf(ios_base::unitbuf); 
    return __ios;
}

// adjustfield manipulators, section 27.4.5.2
inline ios_base& internal(ios_base& __ios)
{
    __ios.setf(ios_base::internal, ios_base::adjustfield); 
    return __ios;
}

inline ios_base& left(ios_base& __ios)
{
    __ios.setf(ios_base::left, ios_base::adjustfield); 
    return __ios;
}

inline ios_base& right(ios_base& __ios)
{
    __ios.setf(ios_base::right, ios_base::adjustfield); 
    return __ios;
}

// basefield manipulators, section 27.4.5.3
inline ios_base& dec(ios_base& __ios)
{
    __ios.setf(ios_base::dec, ios_base::basefield); 
    return __ios;
}

inline ios_base& hex(ios_base& __ios)
{
    __ios.setf(ios_base::hex, ios_base::basefield); 
    return __ios;
}

inline ios_base& oct(ios_base& __ios)
{
    __ios.setf(ios_base::oct, ios_base::basefield); 
    return __ios;
}

// floatfield manipulators, section 27.4.5.3
inline ios_base& fixed(ios_base& __ios)
{
    __ios.setf(ios_base::fixed, ios_base::floatfield); 
    return __ios;
}

inline ios_base& scientific(ios_base& __ios)
{
    __ios.setf(ios_base::scientific, ios_base::floatfield); 
    return __ios;
}

}

#endif // _PODSTL_IOS_BASE_H
