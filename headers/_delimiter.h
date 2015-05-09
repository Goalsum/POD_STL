// ���ļ�����_Delimiter���ͣ�����locale�е�һ��facet�����ڶ����������ķָ����š�
// _Delimiter������STL��׼�淶����PODSTL���Զ�����չ���ܡ�

#ifndef _PODSTL_DELIMITER_H
#define _PODSTL_DELIMITER_H

#include "locale"

namespace podstl
{

class _Delimiter : public locale::facet
{
public:
    _Delimiter(bool __binary_mode = false);
    _Delimiter(char __delim);
    _Delimiter(const _Delimiter& __other, bool __reverse = false);

    void _M_add_delim(char __delim);

    bool _M_is_delim(char ___char) const;
    const char* _M_scan_for_delim(const char* __first, const char* __last) const;

    static const locale::id id = locale::DELIMITER_ID;

private:
    static const int DELIMS_MAX_COUNT = 16;
    char __M_delim_set[DELIMS_MAX_COUNT];
    char* __M_delim_set_end;
    bool __M_reverse;   // ��Ϊtrueʱ��_M_is_delim���жϱ�׼��ת�������зǷָ�������true���ָ�������false��
};

}

#endif   // _PODSTL_DELIMITER_H
