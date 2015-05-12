// 本文件定义_Delimiter类的非模板实现。

#include "_delimiter.h"
#include "_algobase.h"
#include "functional"

namespace podstl
{

const locale::id _Delimiter::id;
const int _Delimiter::DELIMS_MAX_COUNT;

_Delimiter::_Delimiter(bool __binary_mode /* = false */) : __M_delim_set_end(__M_delim_set), __M_reverse(false)
{
    if (__binary_mode) return;  // 没有任何分隔符

    *__M_delim_set_end++ = '\0';
    *__M_delim_set_end++ = ' ';
    *__M_delim_set_end++ = '\t';
    *__M_delim_set_end++ = '\n';
    *__M_delim_set_end++ = '\r';
}

_Delimiter::_Delimiter(char __delim) : __M_delim_set_end(__M_delim_set), __M_reverse(false)
{
    *__M_delim_set_end++ = '\0';
    *__M_delim_set_end++ = __delim;
}

_Delimiter::_Delimiter(const _Delimiter& __other, bool __reverse /* = false */) : __M_reverse(__reverse)
{
    __M_delim_set_end = copy(__other.__M_delim_set, const_cast<const char*>(__other.__M_delim_set_end), __M_delim_set);
}

void _Delimiter::_M_add_delim(char __delim)
{
    if (__M_delim_set_end < (__M_delim_set + DELIMS_MAX_COUNT))
    {
        *__M_delim_set_end++ = __delim;
    }
}

bool _Delimiter::_M_is_delim(char ___char) const
{
    bool __is_delim = (find(__M_delim_set, const_cast<const char*>(__M_delim_set_end), ___char) != __M_delim_set_end);
    return __M_reverse ? (!__is_delim) : __is_delim;
}

const char* _Delimiter::_M_scan_for_delim(const char* __first, const char* __last) const
{
    return find_if(__first, __last, bind1st(mem_fun(&_Delimiter::_M_is_delim), this));
}

}