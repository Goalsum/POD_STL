// 本文件是对外头文件，允许外部使用者直接包含。
// 本文件定义基本类型操作。

#ifndef _PODSTL_TYPE_MANIPS_H
#define _PODSTL_TYPE_MANIPS_H

namespace podstl
{

struct _TrueType {};
struct _FalseType {};

// bool to type
template <int _Is>
struct _Bool2Type { typedef _TrueType _Ret; };

template <>
struct _Bool2Type<1> { typedef _TrueType _Ret; };

template <>
struct _Bool2Type<0> { typedef _FalseType _Ret; };

// type to bool
template <typename _BoolType>
struct _Type2Bool { enum {_Ret = 1}; };

template <>
struct _Type2Bool<_TrueType> { enum {_Ret = 1}; };

template <>
struct _Type2Bool<_FalseType> { enum {_Ret = 0}; };

// Negation
template <typename _BoolType>
struct _Not { typedef _FalseType _Ret; };

template <>
struct _Not<_FalseType> { typedef _TrueType _Ret; };

// logical and of 2 predicated
template <typename _P1, typename _P2>
struct _Land2 { typedef _FalseType _Ret; };

template <>
struct _Land2<_TrueType, _TrueType> { typedef _TrueType _Ret; };

// logical and of 3 predicated
template <typename _P1, typename _P2, typename _P3>
struct _Land3 { typedef _FalseType _Ret; };

template <>
struct _Land3<_TrueType, _TrueType, _TrueType> { typedef _TrueType _Ret; };

//logical or of 2 predicated
template <typename _P1, typename _P2>
struct _Lor2 { typedef _TrueType _Ret; };

template <>
struct _Lor2<_FalseType, _FalseType> { typedef _FalseType _Ret; };

// logical or of 3 predicated
template <typename _P1, typename _P2, typename _P3>
struct _Lor3 { typedef _TrueType _Ret; };

template <>
struct _Lor3<_FalseType, _FalseType, _FalseType> { typedef _FalseType _Ret; };

// Selects one of two types based upon a boolean constant
template <bool _Cond, typename _Tp1, typename _Tp2>
struct _Select { typedef _Tp1 _Ret; };

template <typename _Tp1, typename _Tp2>
struct _Select<false, _Tp1, _Tp2> { typedef _Tp2 _Ret; };

}

#endif  // _PODSTL_TYPE_MANIPS_H

