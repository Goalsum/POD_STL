// 本文件定义bin_num_put类型，用于定制将数字用二进制放入输出流的算法。

#ifndef _PODSTL_BIN_NUM_PUT_H
#define _PODSTL_BIN_NUM_PUT_H

#include "_num_put.h"

namespace podstl
{

class bin_num_put : public num_put
{
public:
    bin_num_put() {}
    ~bin_num_put() {}

protected:
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, bool __val) const;
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, short __val) const;
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, unsigned short __val) const;
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, int __val) const;
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, unsigned int __val) const;
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, long __val) const;
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, unsigned long __val) const;
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, double __val) const;
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, long double __val) const;
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, LONG_LONG __val) const;
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, ULONG_LONG __val) const;
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, const void* __val) const;
};

}

#endif // _PODSTL_BIN_NUM_PUT_H
