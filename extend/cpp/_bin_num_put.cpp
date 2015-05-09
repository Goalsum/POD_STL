// 本文件定义bin_num_put类的实现。

#include "_bin_num_put.h"
#include "_algobase.h"

namespace podstl
{

template <typename _Integer>
ostreambuf_iterator _do_put_binary(ostreambuf_iterator __it, _Integer __x)
{
    char* p = reinterpret_cast<char*>(&__x);
    return copy(p, p + sizeof(__x), __it);
}

bin_num_put::iter_type bin_num_put::do_put(iter_type __it, ios_base& /* __ostream */, char_type /* __fill */, bool __val) const
{
    char __char = __val ? 1 : 0;
    return _do_put_binary(__it, __char);
}

bin_num_put::iter_type bin_num_put::do_put(iter_type __it, ios_base& /* __ostream */, char_type /* __fill */, short __val) const
{
    return _do_put_binary(__it, __val);
}

bin_num_put::iter_type bin_num_put::do_put(iter_type __it, ios_base& /* __ostream */, char_type /* __fill */, unsigned short __val) const
{
    return _do_put_binary(__it, __val);
}

bin_num_put::iter_type bin_num_put::do_put(iter_type __it, ios_base& /* __ostream */, char_type /* __fill */, int __val) const
{
    return _do_put_binary(__it, __val);
}

bin_num_put::iter_type bin_num_put::do_put(iter_type __it, ios_base& /* __ostream */, char_type /* __fill */, unsigned int __val) const
{
    return _do_put_binary(__it, __val);
}

bin_num_put::iter_type bin_num_put::do_put(iter_type __it, ios_base& /* __ostream */, char_type /* __fill */, long __val) const
{
    return _do_put_binary(__it, __val);
}

bin_num_put::iter_type bin_num_put::do_put(iter_type __it, ios_base& /* __ostream */, char_type /* __fill */, unsigned long __val) const
{
    return _do_put_binary(__it, __val);
}

bin_num_put::iter_type bin_num_put::do_put(iter_type __it, ios_base& /* __ostream */, char_type /* __fill */, double __val) const
{
    return _do_put_binary(__it, __val);
}

bin_num_put::iter_type bin_num_put::do_put(iter_type __it, ios_base& /* __ostream */, char_type /* __fill */, long double __val) const
{
    return _do_put_binary(__it, __val);
}

bin_num_put::iter_type bin_num_put::do_put(iter_type __it, ios_base& /* __ostream */, char_type /* __fill */, LONG_LONG __val) const
{
    return _do_put_binary(__it, __val);
}

bin_num_put::iter_type bin_num_put::do_put(iter_type __it, ios_base& /* __ostream */, char_type /* __fill */, ULONG_LONG __val) const
{
    return _do_put_binary(__it, __val);
}

bin_num_put::iter_type bin_num_put::do_put(iter_type __it, ios_base& /* __ostream */, char_type /* __fill */, const void* __val) const
{
    return _do_put_binary(__it, __val);
}

}
