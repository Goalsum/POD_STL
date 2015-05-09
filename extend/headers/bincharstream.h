// 本文件是对外头文件，允许外部使用者直接包含。
// 本文件定义流缓存为字符数组，并且数字输入输出方式为二进制的相关类，包括ibincharstream、obincharstream、bincharstream类型。

#ifndef _PODSTL_BINCHARSTREAM_H
#define _PODSTL_BINCHARSTREAM_H

#include "charstream.h"
#include "_bin_num_get.h"
#include "_bin_num_put.h"

namespace podstl
{

class ibincharstream : public icharstream
{
public:
    ibincharstream(ios_base::openmode __mode = ios_base::in) : icharstream(__mode), __M_bin_delim(true)
    {
        __M_locale._M_insert(&__M_bin_delim, _Delimiter::id);
        __M_locale._M_insert(&__M_bin_num_get, bin_num_get::id);
    }

    ibincharstream(const char* __buf, size_t __size, ios_base::openmode __mode = ios_base::in)
        : icharstream(__buf, __size, __mode), __M_bin_delim(true)
    {
        __M_locale._M_insert(&__M_bin_delim, _Delimiter::id);
        __M_locale._M_insert(&__M_bin_num_get, bin_num_get::id);
    }

private:
    _Delimiter __M_bin_delim;
    bin_num_get __M_bin_num_get;
};

class obincharstream : public ocharstream
{
public:
    obincharstream(ios_base::openmode __mode = ios_base::out) : ocharstream(__mode)
    {
        __M_locale._M_insert(&__M_bin_num_put, bin_num_put::id);
    }

    obincharstream(char* __buf, size_t __size, ios_base::openmode __mode = ios_base::out) : ocharstream(__buf, __size, __mode)
    {
        __M_locale._M_insert(&__M_bin_num_put, bin_num_put::id);
    }

private:
    bin_num_put __M_bin_num_put;
};

class bincharstream : public charstream
{
public:
    bincharstream(openmode __mode = ios_base::in | ios_base::out) : charstream(__mode), __M_bin_delim(true)
    {
        __M_locale._M_insert(&__M_bin_delim, _Delimiter::id);
        __M_locale._M_insert(&__M_bin_num_get, bin_num_get::id);
        __M_locale._M_insert(&__M_bin_num_put, bin_num_put::id);
    }

    bincharstream(char* __buf, size_t __size, openmode __mode = ios_base::in | ios_base::out) : charstream(__buf, __size, __mode), __M_bin_delim(true)
    {
        __M_locale._M_insert(&__M_bin_delim, _Delimiter::id);
        __M_locale._M_insert(&__M_bin_num_get, bin_num_get::id);
        __M_locale._M_insert(&__M_bin_num_put, bin_num_put::id);
    }

private:
    _Delimiter __M_bin_delim;
    bin_num_get __M_bin_num_get;
    bin_num_put __M_bin_num_put;
};

}

#endif // _PODSTL_BINCHARSTREAM_H
