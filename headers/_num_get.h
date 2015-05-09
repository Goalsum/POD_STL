// 本文件定义num_get类型，属于locale中的一个facet，用于定制从输入流提取数字的算法。

#ifndef _PODSTL_NUM_GET_H
#define _PODSTL_NUM_GET_H

#include "locale"
#include "_ios_base.h"
#include "_istreambuf_iterator.h"

namespace podstl
{

class num_get : public locale::facet 
{
public:
    typedef char char_type;
    typedef istreambuf_iterator iter_type;

    num_get() {}
    ~num_get() {}

    template <typename _Num>
    iter_type get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, _Num& __val) const
    {
        return do_get(__ii, __end, __istream, __err, __val);
    }

    static const locale::id id = locale::NUM_GET_ID;

protected:
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, bool& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, long& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, unsigned short& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, unsigned int& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, unsigned long& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, short& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, int& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, float& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, double& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, void*& __p) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, long double& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, LONG_LONG& __val) const;
    virtual iter_type do_get(iter_type __ii, iter_type __end, ios_base& __istream, ios_base::iostate& __err, ULONG_LONG& __val) const;
};

}

#endif // _PODSTL_NUM_GET_H
