/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义iostream类的非模板实现。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

#include "iostream"

namespace podstl
{

iostream::iostream(streambuf* __buf) : ios(), istream(__buf), ostream(__buf)
{
    init(__buf);
}

iostream::~iostream() {}

}
