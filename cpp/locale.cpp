// 本文件定义locale类的非模板实现。

#include "locale"
#include "_algobase.h"
#include "_num_get.h"
#include "_num_put.h"

namespace podstl
{

const locale::category locale::none;
const locale::category locale::collate;
const locale::category locale::ctype;
const locale::category locale::monetary;
const locale::category locale::numeric;
const locale::category locale::time;
const locale::category locale::messages;
const locale::category locale::all;

locale::facet::~facet() {}

locale::locale() 
{
    fill(__M_facets, __M_facets + FACET_COUNT, static_cast<facet*>(0));
}

locale::locale(const locale& __other)
{
    copy(__other.__M_facets, __other.__M_facets + FACET_COUNT, __M_facets);
}

void locale::_M_insert(facet* __facet, id __id)
{
    if ((__facet != 0) && (__id < size()))
    {
        __M_facets[__id] = __facet;
    }
}

// Contruct a new locale where all facets that aren't in category __exclusions
// come from __other, and all those that are in category __exclusions come from __substitute.
locale::locale(const locale& __other, const locale& __substitute, category __exclusions)
{
    copy(__other.__M_facets, __other.__M_facets + FACET_COUNT, __M_facets);
    if (__exclusions & numeric)
    {
        __M_facets[num_get::id] = __substitute.__M_facets[num_get::id];
        __M_facets[num_put::id] = __substitute.__M_facets[num_put::id];
    }
}

const locale& locale::operator=(const locale& __other)
{
    if (&__other != this)
    {
        copy(__other.__M_facets, __other.__M_facets + FACET_COUNT, __M_facets);
    }
    return *this;
}

// Compare two locales for equality.
bool locale::operator==(const locale& __other) const
{
    return equal(__M_facets, __M_facets + FACET_COUNT, __other.__M_facets);
}

bool locale::operator!=(const locale& __other) const
{
    return !(*this == __other);
}

}
