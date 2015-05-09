// 本文件定义ios_base类的非模板实现。

#include "algorithm"
#include "_ios_base.h"

namespace podstl
{

locale ios_base::imbue(const locale& __loc) 
{
    if (__loc != __M_locale)
    {
        locale previous = __M_locale;
        __M_locale = __loc;
        return previous;
    }
    else
    {
        return __M_locale;
    }
}

// Copy __other's state to *this.  This member function is used in the
// implementation of basic_ios::copyfmt. Does not copy __M_exception_mask
// or __M_iostate.
void ios_base::_M_copy_state(const ios_base& __other) 
{
    __M_fmtflags = __other.__M_fmtflags;
    __M_openmode = __other.__M_openmode;
    __M_seekdir = __other.__M_seekdir;
    __M_precision = __other.__M_precision;
    __M_width = __other.__M_width;
    __M_locale = __other.__M_locale;
}

ios_base::ios_base() : __M_fmtflags(0), __M_iostate(0), __M_openmode(0), __M_seekdir(0),
                       __M_exception_mask(0), __M_precision(0), __M_width(0)
{}

}