/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义字符串的输入输出操作。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。

#ifndef _PODSTL_STRING_IO_H
#define _PODSTL_STRING_IO_H

#include "ostream"
#include "istream"
#include "string"

namespace podstl
{

ostream& operator<<(ostream& __ostream, const string& __str);
istream& operator>>(istream& __istream, string& __str);

istream& getline(istream& __istream, string& __str, char __delim);

inline istream& getline(istream& __istream, string& __str) 
{
    return getline(__istream, __str, __istream.widen('\n'));
}

}

#endif // _PODSTL_STRING_IO_H
