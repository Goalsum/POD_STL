// 本文件是对外头文件，允许外部使用者直接包含。
// 本文件定义基本类型属性识别。

#ifndef _PODSTL_TYPE_TRAITS_H
#define _PODSTL_TYPE_TRAITS_H

#include "type_manips.h"

namespace podstl
{

template <typename _Tp>
struct _IsIntegral { typedef _FalseType _Ret; };

template <>
struct _IsIntegral<bool> { typedef _TrueType _Ret; };

template <>
struct _IsIntegral<char> { typedef _TrueType _Ret; };

template <>
struct _IsIntegral<signed char> { typedef _TrueType _Ret; };

template <>
struct _IsIntegral<unsigned char> { typedef _TrueType _Ret; };

template <>
struct _IsIntegral<short> { typedef _TrueType _Ret; };

template <>
struct _IsIntegral<unsigned short> { typedef _TrueType _Ret; };

template <>
struct _IsIntegral<int> { typedef _TrueType _Ret; };

template <>
struct _IsIntegral<unsigned int> { typedef _TrueType _Ret; };

template <>
struct _IsIntegral<long> { typedef _TrueType _Ret; };

template <>
struct _IsIntegral<unsigned long> { typedef _TrueType _Ret; };

template <>
struct _IsIntegral<LONG_LONG> { typedef _TrueType _Ret; };

template <>
struct _IsIntegral<ULONG_LONG> { typedef _TrueType _Ret; };

template <typename _Tp>
struct _IsRational { typedef _FalseType _Ret; };

template <>
struct _IsRational<float> { typedef _TrueType _Ret; };

template <>
struct _IsRational<double> { typedef _TrueType _Ret; };

template <>
struct _IsRational<long double> { typedef _TrueType _Ret; };

template <typename _Tp>
struct _IsPtr { typedef _FalseType _Ret; };

template <typename _Tp>
struct _IsPtr<_Tp*> { typedef _TrueType _Ret; };

template <typename _Tp>
struct _IsRef { typedef _FalseType _Ret; };

template <typename _Tp>
struct _IsRef<_Tp&> { typedef _TrueType _Ret; };

template <typename _IsPOD>
struct _TypeTraitsAux
{
    typedef _FalseType    has_trivial_default_constructor;
    typedef _FalseType    has_trivial_copy_constructor;
    typedef _FalseType    has_trivial_assignment_operator;
    typedef _FalseType    has_trivial_destructor;
    typedef _FalseType    is_POD_type;
};

template <>
struct _TypeTraitsAux<_FalseType>
{
    typedef _FalseType    has_trivial_default_constructor;
    typedef _FalseType    has_trivial_copy_constructor;
    typedef _FalseType    has_trivial_assignment_operator;
    typedef _FalseType    has_trivial_destructor;
    typedef _FalseType    is_POD_type;
};

template <>
struct _TypeTraitsAux<_TrueType>
{
    typedef _TrueType    has_trivial_default_constructor;
    typedef _TrueType    has_trivial_copy_constructor;
    typedef _TrueType    has_trivial_assignment_operator;
    typedef _TrueType    has_trivial_destructor;
    typedef _TrueType    is_POD_type;
};

template <typename _Tp>
struct type_traits
{
    // The following restrictions should be observed for the sake of
    // compilers which automatically produce type specific specializations
    // of this class:
    // - You may reorder the members below if you wish
    // - You may remove any of the members below if you wish
    // - You must not rename members without making the corresponding name change in the compiler
    // - Members you add will be treated like regular members unless you add the appropriate support in the compiler.
    typedef _FalseType    has_trivial_default_constructor;
    typedef _FalseType    has_trivial_copy_constructor;
    typedef _FalseType    has_trivial_assignment_operator;
    typedef _FalseType    has_trivial_destructor;
    typedef _FalseType    is_POD_type;
};

template <typename _Tp>
struct type_traits<_Tp*> : _TypeTraitsAux<_TrueType> {};

#define _PODSTL_DEFINE_TYPE_TRAITS_FOR(Type) \
    template <> struct type_traits<Type> : _TypeTraitsAux<_TrueType> {}; \
    template <> struct type_traits<const Type> : _TypeTraitsAux<_TrueType> {}; \
    template <> struct type_traits<volatile Type> : _TypeTraitsAux<_TrueType> {}; \
    template <> struct type_traits<const volatile Type> : _TypeTraitsAux<_TrueType> {}

_PODSTL_DEFINE_TYPE_TRAITS_FOR(bool);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(char);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(signed char);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(unsigned char);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(short);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(unsigned short);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(int);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(unsigned int);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(long);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(unsigned long);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(LONG_LONG);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(ULONG_LONG);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(float);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(double);
_PODSTL_DEFINE_TYPE_TRAITS_FOR(long double);

template <class _Tp> struct _PointedType {};
template <class _Tp> struct _PointedType<_Tp*> { typedef _Tp _Type; };

template <class _Tp> struct _UnQual { typedef _Tp _Type; };
template <class _Tp> struct _UnQual<const _Tp> { typedef _Tp _Type; };
template <class _Tp> struct _UnQual<volatile _Tp> { typedef _Tp _Type; };
template <class _Tp> struct _UnQual<const volatile _Tp> { typedef _Tp _Type; };

template <class _Tp1, class _Tp2>
struct _AreSameTypes
{
    typedef _FalseType _Ret;
};

template <class _Tp>
struct _AreSameTypes<_Tp, _Tp>
{
    typedef _TrueType _Ret;
};

template <typename _Tp1, typename _Tp2>
struct _BothPtrType
{
    typedef typename _IsPtr<_Tp1>::_Ret _IsPtr1;
    typedef typename _IsPtr<_Tp2>::_Ret _IsPtr2;

    typedef typename _Land2<_IsPtr1, _IsPtr2>::_Ret _Ret;
    static _Ret _Answer() { return _Ret(); }
};

template <typename _BothPtrs, typename _Src, typename _Dst>
struct _IsPointedTypeSame
{
    typedef _FalseType _Ret;
};

template <typename _Src, typename _Dst>
struct _IsPointedTypeSame<_TrueType, _Src, _Dst>
{
    typedef typename _PointedType<_Src>::_Type _SrcPointedType;
    typedef typename _UnQual<_SrcPointedType>::_Type _SrcUnQualType;
    typedef typename _PointedType<_Dst>::_Type _DstPointedType;
    typedef typename _UnQual<_DstPointedType>::_Type _DstUnQualType;
    typedef typename _AreSameTypes<_SrcUnQualType, _DstUnQualType>::_Ret _Ret;
};

template <typename _Src, typename _Dst>
struct _TrivialNativeTypeCopy
{
    typedef typename _BothPtrType<_Src, _Dst>::_Ret _BothPtrs;
    typedef typename _IsPointedTypeSame<_BothPtrs, _Src, _Dst>::_Ret _PointedTypeSame;

    typedef typename _Bool2Type<(sizeof(_Src) == sizeof(_Dst))>::_Ret _SameSize;
    typedef typename _IsIntegral<_Src>::_Ret _Int1;
    typedef typename _IsIntegral<_Dst>::_Ret _Int2;
    typedef typename _Land2<_Int1, _Int2>::_Ret _BothInts;
    typedef typename _IsRational<_Src>::_Ret _Rat1;
    typedef typename _IsRational<_Dst>::_Ret _Rat2;
    typedef typename _Land2<_Rat1, _Rat2>::_Ret _BothRats;
    typedef typename _Lor2<_BothInts, _BothRats>::_Ret _BothNatives;
    typedef typename _Land2<_BothNatives, _SameSize>::_Ret _TrivialValues;

    typedef typename _Lor2<_PointedTypeSame, _TrivialValues>::_Ret _Ret;
};

template <typename _Src, typename _Dst>
struct _TrivialCopy
{
    typedef typename _TrivialNativeTypeCopy<_Src, _Dst>::_Ret _NativeRet;

    typedef typename type_traits<_Src>::has_trivial_assignment_operator _Tr1;
    typedef typename _UnQual<_Src>::_Type _SrcUnQualType;
    typedef typename _UnQual<_Dst>::_Type _DstUnQualType;
    typedef typename _AreSameTypes<_SrcUnQualType, _DstUnQualType>::_Ret _Tr2;
    typedef typename _Land2<_Tr1, _Tr2>::_Ret _UserRet;

    typedef typename _Lor2<_NativeRet, _UserRet>::_Ret _Ret;
    static _Ret _Answer(){ return _Ret(); }
};

template <typename _Tp>
struct _DefaultZeroValue
{
    typedef typename _IsIntegral<_Tp>::_Ret _Tr1;
    typedef typename _IsRational<_Tp>::_Ret _Tr2;
    typedef typename _IsPtr<_Tp>::_Ret _Tr3;
    typedef typename _Lor3<_Tr1, _Tr2, _Tr3>::_Ret _Ret;
};

template <typename _Tp>
struct _TrivialInit
{
    typedef typename type_traits<_Tp>::has_trivial_default_constructor _Tr1;
    typedef typename _DefaultZeroValue<_Tp>::_Ret _Tr2;
    typedef typename _Not<_Tr2>::_Ret _Tr3;
    typedef typename _Land2<_Tr1, _Tr3>::_Ret _Ret;
    static _Ret _Answer() { return _Ret(); }
};

template <typename _Tp>
struct _IsPtrType
{
    typedef typename _IsPtr<_Tp>::_Ret _Type;
    static _Type _Ret() { return _Type(); }
};

template <typename _Tp>
struct _IsRefType
{
    typedef typename _IsRef<_Tp>::_Ret _Type;
    static _Type _Ret() { return _Type();}
};

template <typename _Tp>
struct _CallTraits
{
    typedef const _Tp& _ConstParamType;
    typedef _Tp& _ParamType;
};

template <typename _Tp>
struct _CallTraits<_Tp&>
{
    typedef _Tp& _ParamType;
    typedef const _Tp& _ConstParamType;
};

template <typename _Tp>
struct _CallTraits<const _Tp&>
{
    typedef _Tp& _ParamType;
    typedef const _Tp& _ConstParamType;
};

template <typename _Src, typename _Dst>
inline _TrivialCopy<_Src, _Dst> _use_trivial_copy(_Src*, _Dst*)
{
    return _TrivialCopy<_Src, _Dst>();
}

struct _NegativeAnswer
{
    typedef _FalseType _Ret;
    static _Ret _Answer() { return _Ret(); }
};

template <typename _Src, typename _Dst>
inline _NegativeAnswer _use_trivial_copy(_Src*, const _Dst*) { return _NegativeAnswer(); }

template <typename _Src, typename _Dst>
inline _NegativeAnswer _use_trivial_copy(_Src*, volatile _Dst*) { return _NegativeAnswer(); }

template <typename _Src, typename _Dst>
inline _NegativeAnswer _use_trivial_copy(volatile _Src*, _Dst*) { return _NegativeAnswer(); }

template <typename _Src, typename _Dst>
inline _NegativeAnswer _use_trivial_copy(_Src*, const volatile _Dst*) { return _NegativeAnswer(); }

template <typename _Tp>
inline _TrivialInit<_Tp> _UseTrivialInit(_Tp*) { return _TrivialInit<_Tp>(); }

template <typename _Tp>
struct _IsPOD
{
    typedef typename type_traits<_Tp>::is_POD_type _Type;
    static _Type _Answer() { return _Type(); }
};

template <typename _Tp>
inline _IsPOD<_Tp> _is_pod(_Tp*) { return _IsPOD<_Tp>(); }

template <typename _Tp>
struct _DefaultZeroValueQuestion
{
    typedef typename _DefaultZeroValue<_Tp>::_Ret _Ret;
    static _Ret _Answer() { return _Ret(); }
};

template <typename _Tp>
inline _DefaultZeroValueQuestion<_Tp> _has_default_zero_value(_Tp*)
{
    return _DefaultZeroValueQuestion<_Tp>();
}

}

#endif // _PODSTL_TYPE_TRAITS_H

