// ���ļ����ڲ�ͷ�ļ�(�ļ������»��߿�ͷ)���������ⲿʹ����ֱ�Ӱ�����
// ���ļ������ַ������������������

#ifndef _PODSTL_STRING_IO_H
#define _PODSTL_STRING_IO_H

#include "ostream"
#include "istream"
#include "string"

namespace podstl
{

ostream& operator<<(ostream& __ostream, const string& __str);
istream& operator>>(istream& __istream, string& __str);

istream& getline(istream& __istream, string& __str, char __delim);

inline istream& getline(istream& __istream, string& __str) 
{
    return getline(__istream, __str, __istream.widen('\n'));
}

}

#endif // _PODSTL_STRING_IO_H
