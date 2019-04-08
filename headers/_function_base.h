/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: 本文件定义了一些公用函数对象类型。
 * Author: 高尚 gaoshang@huawei.com
 * Create: 2015-04-07
 */

// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。

#ifndef _PODSTL_FUNCTION_BASE_H
#define _PODSTL_FUNCTION_BASE_H

#include "type_traits.h"

namespace podstl
{

template <typename _Arg, typename _Result>
struct unary_function
{
    typedef _Arg argument_type;
    typedef _Result result_type;

protected:

    // This class purpose is to be derived but it is not polymorphic so users should never try
    // to destroy an instance of it directly. The protected non-virtual destructor make this
    // fact obvious at compilation time.
    ~unary_function() {}
};

template <typename _Arg1, typename _Arg2, typename _Result>
struct binary_function
{
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;

protected:

    // See unary_function comment.
    ~binary_function() {}
};

template <typename _Tp>
struct equal_to : public binary_function<_Tp, _Tp, bool>
{
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x == __y; }
};

template <typename _Tp>
struct less : public binary_function<_Tp, _Tp, bool>
{
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <typename _Tp>
struct type_traits<less<_Tp> >
{
    typedef _TrueType has_trivial_default_constructor;
    typedef _TrueType has_trivial_copy_constructor;
    typedef _TrueType has_trivial_assignment_operator;
    typedef _TrueType has_trivial_destructor;
    typedef _TrueType is_POD_type;
};

template <typename _Tp>
less<_Tp> _less(_Tp*) { return less<_Tp>(); }

template <typename _Tp>
equal_to<_Tp> _equal_to(_Tp*) { return equal_to<_Tp>(); }

template <typename _Tp>
struct plus : public binary_function<_Tp, _Tp, _Tp>
{
    _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x + __y; }
};

template <typename _Tp>
struct minus : public binary_function<_Tp, _Tp, _Tp>
{
    _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x - __y; }
};

template <typename _Tp>
plus<_Tp> _plus(_Tp* ) { return plus<_Tp>(); }

template <typename _Tp>
minus<_Tp> _minus(_Tp* ) { return minus<_Tp>(); }

template <typename _Tp>
struct multiplies : public binary_function<_Tp, _Tp, _Tp>
{
    _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x * __y; }
};

template <typename _Pair>
struct _Select1st : public unary_function<_Pair, typename _Pair::first_type>
{
    const typename _Pair::first_type& operator()(const _Pair& __x) const
    {
        return __x.first;
    }
};

template <typename _Pair>
struct _Select2nd : public unary_function<_Pair, typename _Pair::second_type>
{
    const typename _Pair::second_type& operator()(const _Pair& __x) const
    {
        return __x.second;
    }
};

template <typename _Tp>
struct _Identity : public unary_function<_Tp, _Tp>
{
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};

template <typename _Result, typename _Argument>
struct _ConstantUnaryFun
{
    typedef _Argument argument_type;
    typedef _Result result_type;
    result_type _M_val;

    _ConstantUnaryFun(const result_type& __v) : _M_val(__v) {}
    const result_type& operator()(const _Argument&) const { return _M_val; }
};

template <typename _Result, typename _Arg1, typename _Arg2>
struct _ConstantBinaryFun
{
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
    _Result _M_val;

    _ConstantBinaryFun(const _Result& __v) : _M_val(__v) {}
    const result_type& operator()(const _Arg1&, const _Arg2&) const
    {
        return _M_val;
    }
};

// 非对外公开接口。仅用于容器类将比较操作去模板化的目的。
typedef bool (*_DataUnaryPredicate)(const void* __data);
typedef bool (*_DataBinaryPredicate)(const void* __left, const void* __right);

template <typename _Tp, typename _UnaryPredicate>
bool _data_unary_predicate(const void* __left)
{
    return _UnaryPredicate()(*static_cast<const _Tp*>(__left));
}

template <typename _Tp, typename _BinaryPredicate>
bool _data_binary_predicate(const void* __left, const void* __right)
{
    return _BinaryPredicate()(*static_cast<const _Tp*>(__left), *static_cast<const _Tp*>(__right));
}

}

#endif // _PODSTL_FUNCTION_BASE_H

