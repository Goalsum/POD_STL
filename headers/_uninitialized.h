// ���ļ����ڲ�ͷ�ļ�(�ļ������»��߿�ͷ)���������ⲿʹ����ֱ�Ӱ�����
// ���ļ�������һЩ���ó�ʼ���㷨��

#ifndef _PODSTL_UNINITIALIZED_H
#define _PODSTL_UNINITIALIZED_H

#include "_algobase.h"
#include "_construct.h"

namespace podstl
{

// �����ڲ���ʼ��ʱʹ�ã�ֻ�����POD���ͳ�ʼ���������ڴ濽����ʽ�����������������ָ�룡
template <typename _Size, typename _Tp>
inline _Tp* _uinit_aux_aux(_Tp* __first, _Size __n, const _Tp& __val, const _FalseType& /* HasDefaultZero */)
{
    return static_cast<_Tp*>(_fill_n_trivial(__first, __n, &__val, sizeof(__val)));
}

template <typename _Size, typename _Tp>
inline _Tp* _uinit_aux_aux(_Tp* __first, _Size __n, const _Tp&, const _TrueType& /* HasDefaultZero */)
{
    VOS_memset_s(__first, __n * sizeof(_Tp), 0, __n * sizeof(_Tp));
    return __first + __n;
}

template <typename _Size, typename _Tp>
inline _Tp* _uinit_aux(_Tp* __first, _Size __n, const _Tp&, const _TrueType& /* TrivialInit */)
{
    return __first + __n;
}

template <typename _Size, typename _Tp>
inline _Tp* _uinit_aux(_Tp* __first, _Size __n, const _Tp& __val, const _FalseType& /* TrivialInit */)
{
    return _uinit_aux_aux(__first, __n, __val, _has_default_zero_value(__first)._Answer());
}

template <typename _Size, typename _Tp>
inline _Tp* _uninitialized_init(_Tp* __first, _Size __n, const _Tp& __val)
{
    return _uinit_aux(__first, __n, __val, _UseTrivialInit(__first)._Answer());
}

}

#endif // _PODSTL_UNINITIALIZED_H

