// 本文件定义字符串的输入输出操作的实现。

#include "_string_io.h"

namespace podstl
{


inline bool _fill_pad(ostream& __ostream, streambuf* __buf, streamsize __length) 
{
    char __f = __ostream.fill();
    for (streamsize __i = 0; __i < __length; ++__i) 
    {
        if (__buf->sputc(__f) == EOF) return false;
    }
    return true;
}

ostream& operator <<(ostream& __ostream, const string& __str) 
{
    typedef string::size_type size_type;

    ostream::sentry __sentry(__ostream);
    bool __ok = false;

    if (__sentry) 
    {
        __ok = true;
        size_type __length = __str.size();
        const bool __left = (__ostream.flags() & ostream::left) != 0;
        const streamsize __width = __ostream.width(0);
        streambuf* __buf = __ostream.rdbuf();
        streamsize __pad_len = (__length < static_cast<size_t>(__width)) ? (__width - __length) : 0;

        if (!__left) __ok = _fill_pad(__ostream, __buf, __pad_len);
        __ok = __ok && (__buf->sputn(__str.data(), static_cast<streamsize>(__length)) == static_cast<streamsize>(__length));
        if (__left) __ok = __ok && _fill_pad(__ostream, __buf, __pad_len);
    }

    if (!__ok) __ostream.setstate(ostream::failbit);
    return __ostream;
}

istream& operator >>(istream& __istream, string& __str) 
{
    typedef istream istream;
    typedef string::size_type size_type;

    istream::sentry __sentry(__istream);
    if (__sentry) 
    {
        streambuf* __buf = __istream.rdbuf();

        const locale& __loc = __istream.getloc();
        const _Delimiter& __delim = use_facet<_Delimiter>(__loc);

        __str.clear();
        streamsize __width = __istream.width(0);
        size_type __length;
        if (__width <= 0)
        {
            __length = __str.max_size();
        }
        else    // __width > 0
        {
            __length = static_cast<size_type>(__width);
            __str.reserve(__length);
        }

        while (__length-- > 0) 
        {
            int __int_char = __buf->sbumpc();
            if (__int_char == EOF) 
            {
                __istream.setstate(istream::eofbit);
                break;
            }

            char __char = static_cast<char>(__int_char);

            if (__delim._M_is_delim(__char))
            {
                if (__buf->sputbackc(__char) == EOF) __istream.setstate(istream::failbit);
                break;
            }
            else
            {
                __str.push_back(__char);
            }
        }

        // If we have read no characters, then set failbit.
        if (__str.empty()) __istream.setstate(istream::failbit);
    }
    else
    {
        __istream.setstate(istream::failbit);
    }

    return __istream;
}


istream& getline(istream& __istream, string& __str, char __delim) 
{
    typedef string::size_type size_type;
    size_type __nread = 0;
    istream::sentry __sentry(__istream, true);
    if (__sentry) 
    {
        streambuf* __buf = __istream.rdbuf();
        __str.clear();

        while (__nread < __str.max_size()) 
        {
            int __int_char = __buf->sbumpc();
            if (__int_char == EOF)
            {
                __istream.setstate(istream::eofbit);
                break;
            }

            ++__nread;
            char __char = static_cast<char>(__int_char);
            if (__char != __delim) __str.push_back(__char);
            else break;    // Delimiter character is extracted but not appended.
        }
    }

    if ((__nread == 0) || (__nread >= __str.max_size()))
    {
        __istream.setstate(istream::failbit);
    }

    return __istream;
}

}
