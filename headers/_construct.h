// ���ļ����ڲ�ͷ�ļ�(�ļ������»��߿�ͷ)���������ⲿʹ����ֱ�Ӱ�����
// ���ļ�������һЩ���ù���������㷨��

#ifndef _PODSTL_CONSTRUCT_H
#define _PODSTL_CONSTRUCT_H

#include "_iterator_base.h"
#include "type_traits.h"

namespace podstl
{

template <typename _Tp>
inline void _destroy_aux(_Tp* __pointer, const _FalseType& /* TrivialDestructor */)
{
    __pointer->~_Tp();
}

template <typename _Tp>
inline void _destroy_aux(_Tp*, const _TrueType& /* TrivialDestructor */) {}

template <typename _Tp>
inline void _destroy(_Tp* __pointer)
{
    typedef typename type_traits<_Tp>::has_trivial_destructor _TrivialDestructor;
    _destroy_aux(__pointer, _TrivialDestructor());
}

template <typename _ForwardIterator, typename _Tp>
inline void _destroy_range_aux(_ForwardIterator __first, _ForwardIterator __last, _Tp*, const _FalseType& /* TrivialDestructor */)
{
    for ( ; __first != __last; ++__first)
    {
        _destroy_aux(&(*__first), _FalseType());
    }
}

template <typename _ForwardIterator, typename _Tp>
inline void _destroy_range_aux(_ForwardIterator, _ForwardIterator, _Tp*, const _TrueType& /* TrivialDestructor */) {}

template <typename _ForwardIterator, typename _Tp>
inline void _destroy_range(_ForwardIterator __first, _ForwardIterator __last, _Tp *__ptr)
{
    typedef typename type_traits<_Tp>::has_trivial_destructor _TrivialDestructor;
    _destroy_range_aux(__first, __last, __ptr, _TrivialDestructor());
}

template <typename _ForwardIterator>
inline void _destroy_range(_ForwardIterator __first, _ForwardIterator __last)
{
    _destroy_range(__first, __last, _PODSTL_VALUE_TYPE(_ForwardIterator));
}

}

#endif // _PODSTL_CONSTRUCT_H

