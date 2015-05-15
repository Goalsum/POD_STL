// ���ļ����ڲ�ͷ�ļ�(�ļ������»��߿�ͷ)���������ⲿʹ����ֱ�Ӱ�����
// ���ļ�����һЩ����ͨ�ò��������������κ����͡�

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
