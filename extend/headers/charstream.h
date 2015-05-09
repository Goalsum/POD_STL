// 本文件是对外头文件，允许外部使用者直接包含。
// 本文件定义流缓存为用户指定大小字符数组的相关类，包括charbuf、icharstream、ocharstream、charstream类型。

#ifndef _PODSTL_CHARSTREAM_H
#define _PODSTL_CHARSTREAM_H

#include "streambuf"
#include "iostream"

namespace podstl
{

class charbuf : public streambuf
{
public:                         // Typedefs.
    typedef char char_type;
    typedef int int_type;
    typedef streampos pos_type;
    typedef streamoff off_type;

    typedef streambuf _Base;
    typedef charbuf _Self;

    // Constructors, destructor.
    explicit charbuf(ios_base::openmode __mode = ios_base::in | ios_base::out);
    explicit charbuf(char* __buf, size_t __size, ios_base::openmode __mode = ios_base::in | ios_base::out);
    virtual ~charbuf();

    // Get or set the buffer.
    char* buffer() const { return __M_buf; }
    void buffer(char* __buf, size_t __size);

protected:                      // Overridden virtual member functions.
    virtual int_type uflow();
    virtual int_type pbackfail(int_type __char);
    int_type pbackfail() { return pbackfail(EOF); }

    virtual _Base* setbuf(char_type* __buf, streamsize __size);
    virtual pos_type seekoff(off_type __off, ios_base::seekdir __dir, ios_base::openmode __mode = ios_base::in | ios_base::out);
    virtual pos_type seekpos(pos_type __pos, ios_base::openmode __mode = ios_base::in | ios_base::out);

private:                        // Helper functions.
    void _M_set_ptrs();

private:
    ios_base::openmode __M_mode;
    char* __M_buf;
    size_t __M_size;
};

// Class icharstream, an input stream that uses a charbuf.
class icharstream : public istream
{
public:                         // Typedefs
    typedef char char_type;
    typedef int int_type;
    typedef streampos pos_type;
    typedef streamoff off_type;
    typedef istream _Base;
    typedef charbuf _Buf;

    // Constructors, destructor.
    icharstream(ios_base::openmode __mode = ios_base::in);
    icharstream(const char* __buf, size_t __size, ios_base::openmode __mode = ios_base::in);
    ~icharstream();

    charbuf* rdbuf() const
    {
        return const_cast<_Buf*>(&__M_buf);
    }

    const char* buffer() const { return __M_buf.buffer(); }
    void buffer(const char* __buf, size_t __size) { __M_buf.buffer(const_cast<char*>(__buf), __size); }

private:
    charbuf __M_buf;
};

// Class ocharstream, an output stream that uses a charbuf.
class ocharstream : public ostream
{
public:                         // Typedefs
    typedef char char_type;
    typedef int int_type;
    typedef streampos pos_type;
    typedef streamoff off_type;
    typedef ostream _Base;
    typedef charbuf _Buf;

    // Constructors, destructor.
    ocharstream(ios_base::openmode __mode = ios_base::out);
    ocharstream(char* __buf, size_t __size, ios_base::openmode __mode = ios_base::out);
    ~ocharstream();

    charbuf* rdbuf() const
    {
        return const_cast<_Buf*>(&__M_buf);
    }

    char* buffer() const { return __M_buf.buffer(); }
    void buffer(char* __buf, size_t __size) { __M_buf.buffer(__buf, __size); }

private:
    charbuf __M_buf;
};

// Class charstream, a bidirectional stream that uses a charbuf.
class charstream : public iostream
{
public:                         // Typedefs
    typedef char char_type;
    typedef int int_type;
    typedef streampos pos_type;
    typedef streamoff off_type;
    typedef iostream _Base;
    typedef charbuf _Buf;
    typedef ios_base::openmode openmode;

    // Constructors, destructor.
    charstream(openmode __mod = ios_base::in | ios_base::out);
    charstream(char* __buf, size_t __size, openmode __mod = ios_base::in | ios_base::out);
    ~charstream();

    charbuf* rdbuf() const
    {
        return const_cast<_Buf*>(&__M_buf);
    }

    char* buffer() const { return __M_buf.buffer(); }
    void buffer(char* __buf, size_t __size) { __M_buf.buffer(__buf, __size); }

private:
    charbuf __M_buf;
};

}

#endif // _PODSTL_CHARSTREAM_H
