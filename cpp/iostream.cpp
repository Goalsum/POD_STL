// ���ļ�����iostream��ķ�ģ��ʵ�֡�

#include "iostream"

namespace podstl
{

iostream::iostream(streambuf* __buf) : ios(), istream(__buf), ostream(__buf)
{
    init(__buf);
}

iostream::~iostream() {}

}
