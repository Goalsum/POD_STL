// 本文件定义iostream类的非模板实现。

#include "iostream"

namespace podstl
{

iostream::iostream(streambuf* __buf) : ios(), istream(__buf), ostream(__buf)
{
    init(__buf);
}

iostream::~iostream() {}

}
