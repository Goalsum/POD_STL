// 本文件定义bin_num_get类的实现。

#include "_bin_num_get.h"

namespace podstl
{

template <typename _Num>
istreambuf_iterator _do_get_binary(istreambuf_iterator __in_ite, istreambuf_iterator __end, ios_base::iostate& __err, _Num& __x)
{
    char* p = reinterpret_cast<char*>(&__x);
    for (int i = 0; i < sizeof(__x); ++i)
    {
        if (__in_ite == __end)
        {
            __err = ios_base::failbit | ios_base::eofbit;
            return __in_ite;
        }
        *p++ = *__in_ite++;
    }
    __err = ios_base::goodbit;
    if (__in_ite == __end) __err |= ios_base::eofbit;
    return __in_ite;
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, bool& __x) const
{
    char __char;
    iter_type __tmp = _do_get_binary(__in_ite, __end, __err, __char);
    if (!(__err & ios_base::failbit))
    {
        if (__char == 0) __x = false;
        else if (__char == 1) __x = true;
        else __err |= ios_base::failbit;
    }
    return __tmp;
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, short& __val) const
{
    return _do_get_binary(__in_ite, __end, __err, __val);
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, int& __val) const
{
    return _do_get_binary(__in_ite, __end, __err, __val);
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, long& __val) const
{
    return _do_get_binary(__in_ite, __end, __err, __val);
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, unsigned short& __val) const
{
    return _do_get_binary(__in_ite, __end, __err, __val);
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, unsigned int& __val) const
{
    return _do_get_binary(__in_ite, __end, __err, __val);
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, unsigned long& __val) const
{
    return _do_get_binary(__in_ite, __end, __err, __val);
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, float& __val) const
{
    return _do_get_binary(__in_ite, __end, __err, __val);
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, double& __val) const
{
    return _do_get_binary(__in_ite, __end, __err, __val);
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, long double& __val) const
{
    return _do_get_binary(__in_ite, __end, __err, __val);
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, void*& __p) const
{
    return _do_get_binary(__in_ite, __end, __err, __p);
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, LONG_LONG& __val) const
{
    return _do_get_binary(__in_ite, __end, __err, __val);
}

bin_num_get::iter_type bin_num_get::do_get(iter_type __in_ite, iter_type __end, ios_base& __istream, ios_base::iostate& __err, ULONG_LONG& __val) const
{
    return _do_get_binary(__in_ite, __end, __err, __val);
}

}
