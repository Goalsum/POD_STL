/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义ios类的非模板实现。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

#include "ios"

namespace podstl
{

// Public constructor, taking a streambuf.

ios::ios(streambuf* __streambuf) : __M_fill(0), __M_streambuf(0), __M_tied_ostream(0) 
{
    init(__streambuf);
}

streambuf* ios::rdbuf(streambuf* __buf) 
{
    streambuf* __tmp = __M_streambuf;
    __M_streambuf = __buf;
    clear();
    return __tmp;
}

ios& ios::copyfmt(const ios& __other) 
{
    _M_copy_state(__other);           // Inherited from ios_base.
    __M_fill = __other.__M_fill;
    __M_tied_ostream = __other.__M_tied_ostream;
    _M_set_exception_mask(__other.exceptions());
    return *this;
}

// Protected constructor and initialization functions. The default
// constructor creates an uninitialized ios, and init() initializes
// all of the members to the values in Table 89 of the C++ standard.
ios::ios() : __M_fill(0), __M_streambuf(0), __M_tied_ostream(0)
{}

void ios::init(streambuf* __streambuf)
{
    rdbuf(__streambuf);
    _M_set_exception_mask(ios_base::goodbit);
    _M_clear_nothrow((__streambuf != 0) ? ios_base::goodbit : ios_base::badbit);
    flags(ios_base::skipws | ios_base::dec);
    width(0);
    precision(6);
    fill(widen(' '));
}

}
