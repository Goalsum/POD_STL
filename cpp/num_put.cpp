/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 数字输出的实现。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

#include <stdlib.h>
#include "_num_put.h"
#include "limits"
#include "_algobase.h"
#include "_ostreambuf_iterator.h"

namespace podstl
{

const locale::id num_put::id;

ostreambuf_iterator _put_float(char* __str, ostreambuf_iterator __oi, ios_base& __ostream, char __fill) 
{
    size_t __str_len = VOS_StrLen(__str);
    char* __str_end = __str + __str_len;
    streamsize __width = __ostream.width(0);
    STL_ASSERT(__width >= 0);
    if (static_cast<size_t>(__width) <= __str_len)
    {
        return copy(__str, __str_end, __oi);
    }

    streamsize __pad = __width - __str_len;
    ios_base::fmtflags __dir = __ostream.flags() & ios_base::adjustfield;

    if (__dir == ios_base::left) 
    {
        __oi = copy(__str, __str_end, __oi);
        return _fill_n(__oi, __pad, __fill);
    }

    if ((__dir == ios_base::internal) && (__str_len != 0) && ((*__str == '+') || (*__str == '-')))
    {
        *__oi++ = *__str++;
        __oi = _fill_n(__oi, __pad, __fill);
        return copy(__str, __str_end, __oi);
    }

    __oi = _fill_n(__oi, __pad, __fill);
    return copy(__str, __str_end, __oi);
}

template <typename _Float>
ostreambuf_iterator _do_put_float(ostreambuf_iterator __it, ios_base& __ostream, char __fill, _Float __x)
{
    int __dec, __sign;
    char* __buf = _fcvt(__x, static_cast<int>(__ostream.precision()), &__dec, &__sign);
    return _put_float(__buf, __it, __ostream, __fill);
}

ostreambuf_iterator _put_integer(char* __buf, char* __buf_end, ostreambuf_iterator __it, ios_base& __ostream, ios_base::fmtflags __flags, char __fill)
{
    ptrdiff_t __len = __buf_end - __buf;
    streamsize __width = __ostream.width(0);

    if (__len >= __width)
    {
        return copy(__buf, __buf_end, __it);
    }

    ptrdiff_t __pad = __width - __len;
    ios_base::fmtflags __dir = __flags & ios_base::adjustfield;

    if (__dir == ios_base::left) 
    {
        __it = copy(__buf, __buf_end, __it);
        return _fill_n(__it, __pad, __fill);
    }

    if ((__dir == ios_base::internal) && (__len != 0) && ((__buf[0] == '+') || (__buf[0] == '-'))) 
    {
        *__it++ = __buf[0];
        __it = _fill_n(__it, __pad, __fill);
        return copy(__buf + 1, __buf_end, __it);
    }

    if ((__dir == ios_base::internal) && (__len >= 2) && (__flags & ios_base::showbase) && ((__flags & ios_base::basefield) == ios_base::hex))
    {
        *__it++ = __buf[0];
        *__it++ = __buf[1];
        __it = _fill_n(__it, __pad, __fill);
        return copy(__buf + 2, __buf + __len, __it);
    }

    __it = _fill_n(__it, __pad, __fill);
    return copy(__buf, __buf + __len, __it);
}

const char* _hex_char_table_lo()
{ 
    return "0123456789abcdefx"; 
}

const char* _hex_char_table_hi()
{
    return "0123456789ABCDEFX";
}

template <typename _Integer>
inline char* _write_decimal_backward(char* __ptr, _Integer __x, ios_base::fmtflags __flags, const _TrueType& /* is_signed */) 
{
    const bool __negative = __x < 0;
    LONG_LONG __temp = __x;
    ULONG_LONG __utemp = __negative ? -__temp : __temp;

    for (; __utemp != 0; __utemp /= 10)
    {
        *--__ptr = static_cast<char>((__utemp % 10) + '0');
    }

    // put sign if needed or requested
    if (__negative) *--__ptr = '-';
    else if (__flags & ios_base::showpos) *--__ptr = '+';

    return __ptr;
}

template <typename _Integer>
inline char* _write_decimal_backward(char* __ptr, _Integer __x, ios_base::fmtflags __flags, const _FalseType& /* is_signed */) 
{
    for (; __x != 0; __x /= 10)
    {
        *--__ptr = static_cast<char>((__x % 10) + '0');
    }

    // put sign if requested
    if (__flags & ios_base::showpos) *--__ptr = '+';
    return __ptr;
}

template <typename _Integer>
char* _write_integer_backward(char* __buf, ios_base::fmtflags __flags, _Integer __x) 
{
    char* __ptr = __buf;

    if (__x == 0) 
    {
        *--__ptr = '0';

        // oct or hex base shall not be added to the 0 value (see '#' flag in C formating strings)
        if ((__flags & ios_base::showpos) && ((__flags & (ios_base::oct | ios_base::hex)) == 0)) *--__ptr = '+';
    }
    else 
    {
        typedef typename numeric_limits<_Integer>::_UnsignedType _UnsignedInteger;

        switch (__flags & ios_base::basefield) 
        {
        case ios_base::oct:
            {
                _UnsignedInteger __temp = static_cast<_UnsignedInteger>(__x);
                for (; __temp != 0; __temp >>= 3)
                {
                    *--__ptr = static_cast<char>((__temp & 0x7) + '0');
                }

                // put leading '0' if showbase is set
                if (__flags & ios_base::showbase) *--__ptr = '0';
            }
            break;

        case ios_base::hex:
            {
                const char* __table_ptr = (__flags & ios_base::uppercase) ? _hex_char_table_hi() : _hex_char_table_lo();
                _UnsignedInteger __temp = static_cast<_UnsignedInteger>(__x);
                for (; __temp != 0; __temp >>= 4)
                {
                    *--__ptr = __table_ptr[__temp & 0xF];
                }

                if (__flags & ios_base::showbase) 
                {
                    *--__ptr = __table_ptr[16];
                    *--__ptr = '0';
                }
            }
            break;

        default:    // ios_base::dec:
            {
                typedef typename _Bool2Type<numeric_limits<_Integer>::is_signed>::_Ret _IsSigned;
                __ptr = _write_decimal_backward(__ptr, __x, __flags, _IsSigned());
            }
            break;
        }
    }

    // return pointer to beginning of the string
    return __ptr;
}

template <typename _Integer>
ostreambuf_iterator _do_put_integer(ostreambuf_iterator __it, ios_base& __ostream, char __fill, _Integer __x)
{
    // buffer size = number of bytes * number of digit necessary in the smallest Standard base (base 8, 3 digits/byte)
    //               plus the longest base representation '0x'
    char __buf[sizeof(_Integer) * 3 + 2];
    const ptrdiff_t __buf_size = sizeof(__buf) / sizeof(char);
    ios_base::fmtflags __flags = __ostream.flags();
    char* __ibeg = _write_integer_backward(__buf + __buf_size, __flags, __x);
    return _put_integer(__ibeg, __buf + __buf_size, __it, __ostream, __flags, __fill);
}

ostreambuf_iterator _do_put_bool(ostreambuf_iterator __it, ios_base& __ostream, char __fill, bool __x) 
{
    const char __true_name[] = "true";
    const char __false_name[] = "false";
    const char* __str = __x ? __true_name : __false_name;
    const size_t __str_size = VOS_StrLen(__str);

    streamsize __width = __ostream.width(0);
    if (__str_size >= static_cast<size_t>(__width))
    {
        return copy(__str, __str + __str_size, __it);
    }
    else 
    {
        streamsize __pad = __width - __str_size;
        ios_base::fmtflags __dir = __ostream.flags() & ios_base::adjustfield;

        if (__dir == ios_base::left) 
        {
            __it = copy(__str, __str + __str_size, __it);
            return _fill_n(__it, __pad, __fill);
        }
        else   // covers right and internal padding
        {
            __it = _fill_n(__it, __pad, __fill);
            return copy(__str, __str + __str_size, __it);
        }
    }
}

num_put::iter_type num_put::do_put(iter_type __it, ios_base& __ostream, char_type __fill, bool __val) const 
{
    if (!(__ostream.flags() & ios_base::boolalpha))
    {
        return do_put(__it, __ostream, __fill, static_cast<long>(__val));
    }
    return  _do_put_bool(__it, __ostream, __fill, __val);
}

num_put::iter_type num_put::do_put(iter_type __it, ios_base& __ostream, char_type __fill, short __val) const
{
    return _do_put_integer(__it, __ostream, __fill, __val);
}

num_put::iter_type num_put::do_put(iter_type __it, ios_base& __ostream, char_type __fill, unsigned short __val) const
{
    return _do_put_integer(__it, __ostream, __fill, __val);
}

num_put::iter_type num_put::do_put(iter_type __it, ios_base& __ostream, char_type __fill, int __val) const
{
    return _do_put_integer(__it, __ostream, __fill, __val);
}

num_put::iter_type num_put::do_put(iter_type __it, ios_base& __ostream, char_type __fill, unsigned int __val) const
{
    return _do_put_integer(__it, __ostream, __fill, __val);
}

num_put::iter_type num_put::do_put(iter_type __it, ios_base& __ostream, char_type __fill, long __val) const
{
    return _do_put_integer(__it, __ostream, __fill, __val);
}

num_put::iter_type num_put::do_put(iter_type __it, ios_base& __ostream, char_type __fill, unsigned long __val) const
{
    return _do_put_integer(__it, __ostream, __fill, __val);
}

num_put::iter_type num_put::do_put(iter_type __it, ios_base& __ostream, char_type __fill, double __val) const
{
    return _do_put_float(__it, __ostream, __fill, __val);
}

num_put::iter_type num_put::do_put(iter_type __it, ios_base& __ostream, char_type __fill, long double __val) const
{
    return _do_put_float(__it, __ostream, __fill, __val);
}

num_put::iter_type num_put::do_put(iter_type __it, ios_base& __ostream, char_type __fill, LONG_LONG __val) const
{ 
    return _do_put_integer(__it, __ostream, __fill, __val); 
}

num_put::iter_type num_put::do_put(iter_type __it, ios_base& __ostream, char_type __fill, ULONG_LONG __val) const
{ 
    return _do_put_integer(__it, __ostream, __fill, __val); 
}

num_put::iter_type num_put::do_put(iter_type __it, ios_base& __ostream, char_type /* __fill */, const void* __val) const 
{
    ios_base::fmtflags __save_flags = __ostream.flags();

    __ostream.setf(ios_base::hex, ios_base::basefield);
    __ostream.setf(ios_base::showbase);
    __ostream.setf(ios_base::internal, ios_base::adjustfield);
    __ostream.width((sizeof(void*) * 2) + 2);   // digits in pointer type plus '0x' prefix
    if (__val == 0) 
    {
        // base ('0x') not shown for null, but I really want to type it
        // for pointer. Print it first in this case.
        const char* __table_ptr = (__save_flags & ios_base::uppercase) ? _hex_char_table_hi() : _hex_char_table_lo();
        __it++ = '0';
        __it++ = __table_ptr[16];
        __ostream.width((sizeof(void*) * 2)); // digits in pointer type
    }
    else 
    {
        __ostream.width((sizeof(void*) * 2) + 2); // digits in pointer type plus '0x' prefix
    }

    iter_type result = _do_put_integer(__it, __ostream, '0', reinterpret_cast<unsigned long>(__val));

    __ostream.flags(__save_flags);
    return result;
}

}
