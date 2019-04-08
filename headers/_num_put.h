/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义num_put类型，属于locale中的一个facet，用于定制将数字放入输出流的算法。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

#ifndef _PODSTL_NUM_PUT_H
#define _PODSTL_NUM_PUT_H

#include "locale"
#include "_ios_base.h"
#include "_ostreambuf_iterator.h"

namespace podstl
{

class num_put : public locale::facet 
{
public:
    typedef char char_type;
    typedef ostreambuf_iterator iter_type;

    num_put() {}
    ~num_put() {}

    template <typename _Num>
    iter_type put(iter_type __it, ios_base& __ostream, char_type __fill, _Num __val) const
    {
        return do_put(__it, __ostream, __fill, __val);
    }

    static const locale::id id = locale::NUM_PUT_ID;

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
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, ULONG_LONG __val) const ;
    virtual iter_type do_put(iter_type __it, ios_base& __ostream, char_type __fill, const void* __val) const;
};

}

#endif // _PODSTL_NUM_PUT_H
