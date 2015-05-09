// 本文件定义num_get类的实现。

#include <stdlib.h>
#include "limits"
#include "_num_get.h"
#include "_istreambuf_iterator.h"

namespace podstl
{

int _get_digit(char __char)
{
    if ((__char >= '0') && (__char <= '9')) return __char - '0';
    if ((__char >= 'a') && (__char <= 'f')) return (__char - 'a') + 10;
    if ((__char >= 'A') && (__char <= 'F')) return (__char - 'A') + 10;
    return 0xFF;
}

// _do_get_integer, _do_get_float and its helper functions.
inline bool _is_digit(char __char)
{
    return (__char >= '0') && (__char <= '9');
}

// 返回值是bit集合：
// bit0为1时表示读到了一个字符0（可能是表示8进制，也可能是数字0）；
// bit1为1时表示是负数，否则是正数。
// bit2以上的位表示进制，取值有8、10、16。
int _get_base_or_zero(istreambuf_iterator& __in_ite, istreambuf_iterator& __end, ios_base::fmtflags __flags)
{
    bool __negative = false;
    char __char = *__in_ite;

    if (__char == '-' )
    {
        __negative = true;
        ++__in_ite;
    }
    else if (__char == '+')
    {
        ++__in_ite;
    }

    int __base;
    int __valid_zero = 0;

    ios_base::fmtflags __basefield = __flags & ios_base::basefield;
    switch (__basefield) 
    {
    case ios_base::oct:
        __base = 8;
        break;

    case ios_base::dec:
        __base = 10;
        break;

    case ios_base::hex:
        __base = 16;
        if ((__in_ite != __end) && (*__in_ite == '0')) 
        {
            ++__in_ite;
            if ((__in_ite != __end) && (*__in_ite == 'x') || (*__in_ite == 'X')) ++__in_ite;
            else __valid_zero = 1;   // That zero is valid by itself.
        }
        break;

    default:
        if ((__in_ite != __end) && (*__in_ite == '0'))
        {
            ++__in_ite;
            if ((__in_ite != __end) && ((*__in_ite == 'x') || (*__in_ite == 'X')))
            {
                ++__in_ite;
                __base = 16;
            }
            else
            {
                __base = 8;
                __valid_zero = 1; // That zero is still valid by itself.
            }
        }
        else __base = 10;
        break;
    }

    return (__base << 2) | (static_cast<int>(__negative) << 1) | __valid_zero;
}

template <typename _Integer>
bool _get_integer(istreambuf_iterator& __first, istreambuf_iterator& __last, int __base, _Integer& __val, int& __got, bool __is_negative, const _TrueType& /* _IsSigned */) 
{
    typedef typename numeric_limits<_Integer>::_UnsignedType _Unsigned;
    _Unsigned __unsigned;;
    bool __ok = _get_integer(__first, __last, __base, __unsigned, __got, __is_negative, _FalseType());
    if (__got <= 0) return false;    // 不修改__val的值。

    __val = static_cast<_Integer>(__unsigned);
    if (!__ok || (__is_negative && (__val > 0)))    // 溢出，按有符号数最大最小值赋值。
    {
        __val = __is_negative ? numeric_limits<_Integer>::min() : numeric_limits<_Integer>::max();
        return false;
    }
    return __ok;
}

template <typename _Integer>
bool _get_integer(istreambuf_iterator& __first, istreambuf_iterator& __last, int __base, _Integer& __val, int& __got, bool __is_negative, const _FalseType& /* _IsSigned */)
{
    bool __overflow = false;
    _Integer __result = 0;
    for (; __first != __last; ++__first) 
    {
        const char __char = *__first;
        int __digit = _get_digit(__char);
        if (__digit >= __base) break;
        ++__got;

        _Integer __next = static_cast<_Integer>((__base * __result) + __digit);
        if (__result != 0) __overflow = __overflow || (__next <= __result);
        __result = __next;
    }

    if (__got > 0) 
    {
        __val = __overflow ? numeric_limits<_Integer>::max()
                           : (__is_negative ? (0 - __result) : __result);
    }

    // overflow is being treated as failure
    return (__got > 0) && !__overflow;
}

template <typename _Integer>
istreambuf_iterator _do_get_integer(istreambuf_iterator& __in_ite, istreambuf_iterator& __end, ios_base& __istream, ios_base::iostate& __err, _Integer& __val) 
{
    typedef typename _Bool2Type<numeric_limits<_Integer>::is_signed>::_Ret _IsSigned;

    const int __base_or_zero = _get_base_or_zero(__in_ite, __end, __istream.flags());
    int __got = __base_or_zero & 1;    // 记录已经提取的字符数
    bool __result;

    if (__in_ite == __end) 
    {
        // We may have already read a 0.  If so, the result is 0 even if we're at eof.
        if (__got > 0) 
        {
            __val = 0;
            __result = true;
        }
        else __result = false;
    }
    else 
    {
        const bool __negative = (__base_or_zero & 2) != 0;
        const int __base = __base_or_zero >> 2;
        __result = _get_integer(__in_ite, __end, __base, __val, __got, __negative, _IsSigned());
    }

    __err = __result ? ios_base::goodbit : ios_base::failbit;
    if (__in_ite == __end) __err |= ios_base::eofbit;

    return __in_ite;
}

// _read_float and its helper functions.
istreambuf_iterator _copy_sign(istreambuf_iterator __first, istreambuf_iterator __last, char*& __buf, char* __buf_end)
{
    if (__first != __last) 
    {
        char __char = *__first;
        if (__char == '+') ++__first;
        else if (__char == '-') 
        {
            if (__buf < __buf_end) *__buf++ = '-';
            ++__first;
        }
    }
    return __first;
}

bool _copy_digits(istreambuf_iterator& __first, istreambuf_iterator __last, char*& __buf, char* __buf_end)
{
    bool __ok = false;

    for (; __first != __last; ++__first) 
    {
        char __char = *__first;
        if (_is_digit(__char)) 
        {
            if (__buf < __buf_end) *__buf++ = __char;
            __ok = true;
        }
        else break;
    }
    return __ok;
}

template <typename iter_type>
bool _read_float(char* __buf, char* __buf_end, iter_type& __in_ite, iter_type& __end)
{
    // Create a string, copying characters of the form
    // [+-]? [0-9]* .? [0-9]* ([eE] [+-]? [0-9]+)?
    bool __digits_before_dot;
    bool __digits_after_dot = false;
    bool __ok;

    // Get an optional sign
    __in_ite = _copy_sign(__in_ite, __end, __buf, __buf_end);

    // Get an optional string of digits.
    __digits_before_dot = _copy_digits(__in_ite, __end, __buf, __buf_end);

    // Get an optional decimal point, and an optional string of digits.
    if ((__in_ite != __end) && (*__in_ite == '.'))
    {
        if (__buf < __buf_end) *__buf++ = '.';
        ++__in_ite;
        __digits_after_dot = _copy_digits(__in_ite, __end, __buf, __buf_end);
    }

    // There have to be some digits, somewhere.
    __ok = __digits_before_dot || __digits_after_dot;

    // Get an optional exponent.
    if (__ok && (__in_ite != __end) && ((*__in_ite == 'e') || (*__in_ite == 'E')))
    {
        if (__buf < __buf_end) *__buf++ = 'e';
        ++__in_ite;
        __in_ite = _copy_sign(__in_ite, __end, __buf, __buf_end);

        // If we have an exponent then the sign
        // is optional but the digits aren't.
        __ok = _copy_digits(__in_ite, __end, __buf, __buf_end);
    }

    return __ok && (__buf < __buf_end);
}

template <typename _Float>
istreambuf_iterator _do_get_float(istreambuf_iterator& __in_ite, istreambuf_iterator& __end, ios_base& __istream, ios_base::iostate& __err, _Float& __val)
{
    const int MAX_FLOAT_CHARS = 256;
    char __buf[MAX_FLOAT_CHARS];
    bool __ok = _read_float(__buf, __buf + sizeof(__buf), __in_ite, __end);
    if (__ok) 
    {
        __buf[sizeof(__buf) - 1] = '\0';
        __val = static_cast<_Float>(atof(__buf));
        __err = ios_base::goodbit;
    }
    else 
    {
        __err = ios_base::failbit;
    }
    if (__in_ite == __end) __err |= ios_base::eofbit;
    return __in_ite;
}

istreambuf_iterator _do_get_alphabool(istreambuf_iterator& __in_ite, istreambuf_iterator& __end, ios_base::iostate& __err, bool& __x)
{
    const char __truename[] = "true";
    const char __falsename[] = "false";
    bool __true_ok = true;
    bool __false_ok = true;

    size_t __count = 0;
    for (; __in_ite != __end; ++__in_ite) 
    {
        char __char = *__in_ite;
        __true_ok = __true_ok && (__char == __truename[__count]);
        __false_ok = __false_ok && (__char == __falsename[__count]);
        ++__count;

        if ((!__true_ok && !__false_ok) || (__true_ok && (__truename[__count] == '\0')) || (__false_ok && (__falsename[__count] == '\0')))
        {
            ++__in_ite;
            break;
        }
    }

    // 检查字符串未结束就遇到流末尾的情况。
    if (__true_ok && (__truename[__count] != '\0')) __true_ok = false;
    if (__false_ok && (__falsename[__count] != '\0')) __false_ok = false;

    if (__true_ok || __false_ok) 
    {
        __err = ios_base::goodbit;
        __x = __true_ok;
    }
    else
    {
        __err = ios_base::failbit;
    }

    if (__in_ite == __end) __err |= ios_base::eofbit;

    return __in_ite;
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, bool& __x) const
{
    if (__istream.flags() & ios_base::boolalpha) 
    {
        return _do_get_alphabool(__in_ite, __end, __err, __x);
    }

    long __lx;
    iter_type __tmp =  _do_get_integer(__in_ite, __end, __istream, __err, __lx);
    if (!(__err & ios_base::failbit)) 
    {
        if (__lx == 0) __x = false;
        else if (__lx == 1) __x = true;
        else __err |= ios_base::failbit;
    }
    return __tmp;
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, short& __val) const
{ 
    return _do_get_integer(__in_ite, __end, __istream, __err, __val); 
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, int& __val) const
{ 
    return _do_get_integer(__in_ite, __end, __istream, __err, __val); 
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, long& __val) const
{
    return _do_get_integer(__in_ite, __end, __istream, __err, __val);
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, unsigned short& __val) const
{
    return _do_get_integer(__in_ite, __end, __istream, __err, __val);
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, unsigned int& __val) const
{
    return _do_get_integer(__in_ite, __end, __istream, __err, __val);
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, unsigned long& __val) const
{
    return _do_get_integer(__in_ite, __end, __istream, __err, __val);
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, float& __val) const
{
    return _do_get_float(__in_ite, __end, __istream, __err, __val);
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, double& __val) const
{
    return _do_get_float(__in_ite, __end, __istream, __err, __val);
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, long double& __val) const
{
    return _do_get_float(__in_ite, __end, __istream, __err, __val);
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, void*& __p) const 
{
    ULONG_LONG __val;
    iter_type __tmp = _do_get_integer(__in_ite, __end, __istream, __err, __val);
    if (!(__err & ios_base::failbit))
    {
        __p = reinterpret_cast<void*>(__val);
    }
    return __tmp;
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, LONG_LONG& __val) const
{ 
    return _do_get_integer(__in_ite, __end, __istream, __err, __val); 
}

num_get::iter_type num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, ULONG_LONG& __val) const
{ 
    return _do_get_integer(__in_ite, __end, __istream, __err, __val);
}

}
