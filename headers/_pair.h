// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。
// 本文件定义pair类型。

#ifndef _PODSTL_PAIR_H
#define _PODSTL_PAIR_H

#include "type_traits.h"
#include "_operator.h"

namespace podstl
{

template <typename _T1, typename _T2>
struct pair
{
    typedef _T1 first_type;
    typedef _T2 second_type;

    _T1 first;
    _T2 second;

    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}

    template <typename _U1, typename _U2>
    pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}

    pair(const pair<_T1,_T2>& __o) : first(__o.first), second(__o.second) {}
};

template <typename _T1, typename _T2>
inline bool operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return (__x.first == __y.first) && (__x.second == __y.second);
}

template <typename _T1, typename _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
  return (__x.first < __y.first) || (!(__y.first < __x.first) && (__x.second < __y.second));
}

template <typename _T1, typename _T2>
inline bool operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return !(__x == __y);
}

template <typename _T1, typename _T2>
inline bool operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __y < __x;
}

template <typename _T1, typename _T2>
inline bool operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return !(__y < __x);
}

template <typename _T1, typename _T2>
inline bool operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return !(__x < __y);
}

template <typename _T1, typename _T2>
inline pair<_T1, _T2> make_pair(_T1 __x, _T2 __y)
{
    return pair<_T1, _T2>(__x, __y);
}

template <typename _T1, typename _T2>
struct type_traits<pair<_T1, _T2> >
{
    typedef type_traits<_T1> _T1Traits;
    typedef type_traits<_T2> _T2Traits;
    typedef typename _Land2<typename _T1Traits::has_trivial_default_constructor, typename _T2Traits::has_trivial_default_constructor>::_Ret has_trivial_default_constructor;
    typedef typename _Land2<typename _T1Traits::has_trivial_copy_constructor, typename _T2Traits::has_trivial_copy_constructor>::_Ret has_trivial_copy_constructor;
    typedef typename _Land2<typename _T1Traits::has_trivial_assignment_operator, typename _T2Traits::has_trivial_assignment_operator>::_Ret has_trivial_assignment_operator;
    typedef typename _Land2<typename _T1Traits::has_trivial_destructor, typename _T2Traits::has_trivial_destructor>::_Ret has_trivial_destructor;
    typedef _FalseType is_POD_type;
};

}

#endif // _PODSTL_PAIR_H
