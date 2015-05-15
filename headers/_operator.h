// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。
// 本文件定义一些公用通用操作符，适用于任何类型。

#ifndef _PODSTL_OPERATOR_H
#define _PODSTL_OPERATOR_H

#include "_base_depends.h"

namespace podstl
{

template <typename _Tp>
inline bool operator!=(const _Tp& __x, const _Tp& __y)
{
    return !(__x == __y);
}

template <typename _Tp>
inline bool operator>(const _Tp& __x, const _Tp& __y)
{
    return __y < __x;
}

template <typename _Tp>
inline bool operator<=(const _Tp& __x, const _Tp& __y)
{
    return !(__y < __x);
}

template <typename _Tp>
inline bool operator>=(const _Tp& __x, const _Tp& __y)
{
    return !(__x < __y);
}

}

#endif // _PODSTL_OPERATOR_H
