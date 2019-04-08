/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义bin_num_get类型，用于定制用二进制从输入流提取数字的算法。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

#ifndef _PODSTL_BIN_NUM_GET_H
#define _PODSTL_BIN_NUM_GET_H

#include "_num_get.h"

namespace podstl
{

class bin_num_get : public num_get
{
public:
    bin_num_get() {}
    ~bin_num_get() {}

protected:
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, bool& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, long& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, unsigned short& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, unsigned int& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, unsigned long& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, short& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, int& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, float& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, double& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, void*& __p) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, long double& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, LONG_LONG& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, ULONG_LONG& __val) const;
};

}

#endif // _PODSTL_BIN_NUM_GET_H
