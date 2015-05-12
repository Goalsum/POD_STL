// 本文件定义字符串类的实现。

#include "string"
#include "functional"

namespace podstl
{

char _StringBase::s_shared_null = '\0'; // 供所有刚初始化的空字符串公用

_StringBase::_StringBase(size_t __block_size)
{
    _M_allocate_block(__block_size);
}

_StringBase::~_StringBase() { _M_deallocate_block(); }

const size_t string::npos;

string::string(const _Self& __string)
{
    _M_range_initialize(__string.begin(), __string.end());
}

string::string(const _Self& __other_string, size_type __pos, size_type __string_length /* = npos */)
{
    STL_ASSERT(__pos <= __other_string.size());
    _M_range_initialize(__other_string.begin() + __pos, __other_string.begin() + __pos + min(__string_length, __other_string.size() - __pos));
}

string::string(const char* __c_string, size_type __string_length)
{
    _M_range_initialize(__c_string, __c_string + __string_length);
}

string::string(const char* __c_string)
{
    _M_range_initialize(__c_string, __c_string + VOS_StrLen(__c_string));
}

string::string(size_type __count, char __char) : _StringBase(__count + 1)
{
    __M_finish = _fill_n(begin(), __count, __char);
    _M_terminate_string();
}

string::string(const char* __first, const char* __last)
{
    _M_range_initialize(__first, __last);
}

string::size_type string::_M_compute_next_size(size_type __extra_need)
{
    const size_type __size = size();
    STL_ASSERT(__extra_need <= (max_size() - __size));

    size_type __len = __size + max(__extra_need, __size) + 1;
    if ((__len > max_size()) || (__len < __size))
    {
        __len = max_size(); // overflow
    }
    else if (__len < _DEFAULT_SIZE)
    {
        __len = _DEFAULT_SIZE;
    }
    return __len;
}

void string::_M_reserve(size_type __size)
{
    pointer __new_start = static_cast<pointer>(STL_Malloc(__size));
    pointer __new_finish = podstl::copy(begin(), end(), __new_start);
    _M_construct_null(__new_finish);
    _M_deallocate_block();
    _M_reset(__new_start, __new_finish, __new_start + __size);
}

void string::reserve(size_type __length /* = 0 */)
{
    STL_ASSERT(__length <= max_size());
    size_type __actual_len = max(__length, size()) + 1;
    if (__actual_len < _M_capacity()) return;
    _M_reserve((__actual_len < _DEFAULT_SIZE) ? _DEFAULT_SIZE : __actual_len);
}

void string::clear()
{
    if (empty()) return;
    _M_construct_null(begin());
    __M_finish = begin();
}

string::_Self& string::_M_append(const char* __first, const char* __last)
{
    if (__first != __last)
    {
        STL_ASSERT(__first < __last);
        size_type __append_len = static_cast<size_type>(__last - __first);
        if (__append_len >= _M_rest())
        {
            _M_reserve(_M_compute_next_size(__append_len));
        }
        __M_finish = podstl::copy(__first, __last, end());
        _M_terminate_string();
    }
    return *this;
}

string& string::append(size_type __count, char __char)
{
    if (__count > 0)
    {
        STL_ASSERT(__count <= (max_size() - size()));
        if (__count >= _M_rest())
        {
            _M_reserve(_M_compute_next_size(__count));
        }
        VOS_memset_s(end(), _M_rest(), __char, __count);
        __M_finish += __count;
        _M_terminate_string();
    }
    return *this;
}

void string::push_back(char __char)
{
    if (_M_rest() == 1)
    {
        _M_reserve(_M_compute_next_size(1));
    }
    *end() = __char;
    ++__M_finish;
    _M_terminate_string();
}

void string::pop_back()
{
    if (empty()) return;
    --__M_finish;
    _M_terminate_string();
}

string& string::assign(size_type __count, char __char) 
{
    _M_reserve_and_clear(__count + 1);
    VOS_memset_s(begin(), _M_capacity(), __char, __count);
    __M_finish = begin() + __count;
    _M_terminate_string();

    return *this;
}

string::_Self& string::_M_assign(const char* __first, const char* __last)
{
    STL_ASSERT(__first <= __last);
    ptrdiff_t __length = __last - __first;
    _M_reserve_and_clear(__length + 1);
    __M_finish = podstl::copy(__first, __last, begin());
    _M_terminate_string();
    return *this;
}

string::_Self& string::insert(size_type __pos, const _Self& __other_string)
{
    STL_ASSERT((__pos <= size()) && (__other_string.size() <= (max_size() - size())));
    _M_insert(begin() + __pos, __other_string.begin(), __other_string.end(), &__other_string == this);
    return *this;
}

string::_Self& string::insert(size_type __pos, const _Self& __string, size_type __begin, size_type __length)
{
    STL_ASSERT((__pos <= size()) && (__begin <= __string.size()));
    size_type __real_len = min(__length, __string.size() - __begin);
    STL_ASSERT(__real_len <= max_size() - size());
    _M_insert(begin() + __pos, __string.begin() + __begin, __string.begin() + __begin + __length, &__string == this);
    return *this;
}

string::_Self& string::insert(size_type __pos, const char* __c_string, size_type __length)
{
    STL_ASSERT((__pos <= size()) && (__length <= (max_size() - size())));
    _M_insert(begin() + __pos, __c_string, __c_string + __length, _M_inside(__c_string));
    return *this;
}

string::_Self& string::insert(size_type __pos, size_type __count, char __char)
{
    STL_ASSERT((__pos <= size()) && (__count <= (max_size() - size())));
    insert(begin() + __pos, __count, __char);
    return *this;
}

string::iterator string::insert(iterator __pos, char __char)
{
    if (__pos == end())
    {
        push_back(__char);
        return end() - 1;
    }
    else
    {
        return _M_insert_aux(__pos, __char);
    }
}

void string::insert(iterator __pos, size_t __count, char __char)
{
    if (__count == 0) return;

    pointer __new_pos = __pos;
    if (_M_rest() > __count)
    {
        VOS_memmove_s(__pos + __count, _M_end_of_storage() - (__pos + __count), __pos, end() - __pos + 1); // 包括NULL
        VOS_memset_s(__pos, _M_end_of_storage() - __pos, __char, __count);
        __M_finish += __count;
    }
    else
    {
        size_type __new_len = _M_compute_next_size(__count);
        pointer __new_start = static_cast<pointer>(STL_Malloc(__new_len));
        pointer __new_finish = podstl::copy(begin(), __pos, __new_start);
        VOS_memset_s(__new_finish, __new_len - (__pos - begin()), __char, __count);
        __new_finish += __count;
        __new_finish = podstl::copy(__pos, end() + 1, __new_finish) - 1;    // 包括NULL
        _M_deallocate_block();
        _M_reset(__new_start, __new_finish, __new_start + __new_len);
    }
}

void string::_M_insert(iterator __pos, const char* __first, const char* __last, bool __self_ref)
{
    STL_ASSERT((__pos >= begin()) && (__pos <= end()) && (__first <= __last));
    if (__first == __last) return;

    const size_t __length = __last - __first;
    if (_M_rest() > __length)
    {
        podstl::copy(__pos, end() + 1, __pos + __length); // 包括NULL
        if (!__self_ref || (__first < __pos))
        {
            podstl::copy(__first, __last, __pos);
        }
        else    // 原始数据随着被移走了
        {
            podstl::copy(__first + __length, __last + __length, __pos);
        }
        __M_finish += __length;
    }
    else
    {
        size_type __new_len = _M_compute_next_size(__length);
        pointer __new_start = static_cast<pointer>(STL_Malloc(__new_len));
        pointer __new_finish = podstl::copy(begin(), __pos, __new_start);
        __new_finish = podstl::copy(__first, __last, __new_finish);
        __new_finish = podstl::copy(__pos, end() + 1, __new_finish) - 1;    // 包括NULL
        _M_deallocate_block();
        _M_reset(__new_start, __new_finish, __new_start + __new_len);
    }
}

char* string::_M_insert_aux(char* __pos, char __char)
{
    pointer __new_pos = __pos;
    if (_M_rest() > 1)
    {
        VOS_memmove_s(__pos + 1, _M_end_of_storage() - (__pos + 1), __pos, end() - __pos + 1); // 包括NULL
        *__pos = __char;
        ++__M_finish;
    }
    else 
    {
        size_type __new_len = _M_compute_next_size(1);
        pointer __new_start = static_cast<pointer>(STL_Malloc(__new_len));
        __new_pos = podstl::copy(begin(), __pos, __new_start);
        *__new_pos = __char;
        pointer __new_finish = __new_pos + 1;
        __new_finish = podstl::copy(__pos, end() + 1, __new_finish) - 1;    // 包括NULL
        _M_deallocate_block();
        _M_reset(__new_start, __new_finish, __new_start + __new_len);
    }
    return __new_pos;
}

string::_Self& string::erase(size_type __pos /* = 0 */, size_type __length /* = npos */)
{
    STL_ASSERT(__pos <= size());
    erase(begin() + __pos, begin() + __pos + min(__length, size() - __pos));
    return *this;
}

string::iterator string::erase(iterator __pos)
{
    STL_ASSERT((__pos >= begin()) && (__pos < end()) && (end() < _M_end_of_storage()));

    // The move includes the terminating char().
    VOS_memmove_s(__pos, _M_end_of_storage() - __pos, __pos + 1, end() - __pos);
    --__M_finish;
    return __pos;
}

string::iterator string::erase(iterator __first, iterator __last)
{
    STL_ASSERT((__first >= begin()) && (__first <= end()) && (__last >= begin()) && (__last <= end()) && (__first <= __last) && (end() < _M_end_of_storage()));
    if (__first != __last)
    {
        // The move includes the terminating char().
        VOS_memmove_s(__first, _M_end_of_storage() - __first, __last, (end() - __last) + 1);
        __M_finish -= (__last - __first);
    }
    return __first;
}

string::_Self& string::replace(size_type __pos, size_type __length, const _Self& __string)
{
    const size_type __size = size();
    STL_ASSERT(__pos <= __size);
    const size_type __replace_len = min(__length, __size - __pos);
    STL_ASSERT(__string.size() <= (max_size() - (__size - __replace_len)));
    return _M_replace(begin() + __pos, begin() + __pos + __replace_len, __string.begin(), __string.end(), &__string == this);
}

string::_Self& string::replace(size_type __pos1, size_type __length1, const _Self& __string, size_type __pos2, size_type __length2)
{
    const size_type __size1 = size();
    const size_type __size2 = __string.size();
    STL_ASSERT((__pos1 <= __size1) && (__pos2 <= __size2));
    const size_type __actual_len1 = min(__length1, __size1 - __pos1);
    const size_type __actual_len2 = min(__length2, __size2 - __pos2);
    STL_ASSERT(__actual_len2 <= (max_size() - (__size1 - __actual_len1)));
    return _M_replace(begin() + __pos1, begin() + __pos1 + __actual_len1,
                      __string.begin() + __pos2, __string.begin() + __pos2 + __actual_len2, &__string == this);
}

string::_Self& string::replace(size_type __pos, size_type __length1, const char* __c_string, size_type __length2)
{
    const size_type __size = size();
    STL_ASSERT(__pos <= __size);
    const size_type __actual_len = min(__length1, __size - __pos);
    STL_ASSERT(__length2 <= (max_size() - (__size - __actual_len)));
    return _M_replace(begin() + __pos, begin() + __pos + __actual_len, __c_string, __c_string + __length2, _M_inside(__c_string));
}

string::_Self& string::replace(size_type __pos, size_type __length, size_type __count, char __char)
{
    const size_type __size = size();
    STL_ASSERT(__pos <= __size);
    const size_type __actual_len = min(__length, __size - __pos);
    STL_ASSERT(__count <= (max_size() - (__size - __actual_len)));
    return replace(begin() + __pos, begin() + __pos + __actual_len, __count, __char);
}

string& string::replace(iterator __first, iterator __last, size_type __count, char __char) 
{
    STL_ASSERT((__first >= begin()) && (__first <= end()) && (__last >= begin()) && (__last <= end()) && (__first <= __last));
    size_type __length = static_cast<size_type>(__last - __first);
    STL_ASSERT(__count <= (max_size() - (size() - __length)));

    if (__length >= __count) 
    {
        VOS_memset_s(__first, __length, __char, __count);
        erase(__first + __count, __last);
    }
    else 
    {
        VOS_memset_s(__first, __length, __char, __length);
        insert(__last, __count - __length, __char);
    }
    return *this;
}

string::_Self& string::_M_replace(iterator __first, iterator __last, const char* __new_first, const char* __new_last, bool __self_ref)
{
    STL_ASSERT((__first >= begin()) && (__first <= end()) && (__last >= begin()) && (__last <= end()) && (__first <= __last) && (__new_first <= __new_last));
    const ptrdiff_t __new_length = __new_last - __new_first;
    const difference_type __len = __last - __first;
    if (__len >= __new_length) 
    {
        podstl::copy(__new_first, __new_last, __first);
        erase(__first + __new_length, __last);
    }
    else if (__self_ref && (__new_first < __first))
    {
        // We have to deal with possible reallocation because we do insert first.
        const difference_type __offset_dest = __first - begin();
        const difference_type __offset_src = __new_first - begin();
        _M_insert(__last, __new_first + __len, __new_last, true);
        VOS_memmove_s(begin() + __offset_dest, _M_capacity() - __offset_dest, begin() + __offset_src, __len);
    } 
    else 
    {
        VOS_memmove_s(__first, _M_end_of_storage() - __first, __new_first, __len);
        _M_insert(__last, __new_first + __len, __new_last, __self_ref);
    }
    return *this;
}

string::size_type string::copy(char* __c_string, size_type __length, size_type __pos /* = 0 */) const
{
    STL_ASSERT(__pos <= size());
    const size_type __actual_len = min(__length, size() - __pos);
    VOS_memcpy_s(__c_string, __length, begin() + __pos, __actual_len);
    return __actual_len;
}

void string::swap(_Self& __other_string)
{ 
    _M_swap(__other_string); 
}

string::size_type string::find(const char* __c_string, size_type __pos, size_type __length) const
{
    const size_t __this_length = size();
    if ((__pos >= __this_length) || ((__pos + __length) > __this_length))
    {
        if ((__length == 0) && (__pos <= __this_length))
        {
            return __pos;
        }
        return npos;
    }

    const_pointer __result = search(begin() + __pos, end(), __c_string, __c_string + __length);
    return (__result != end()) ? (__result - begin()) : npos;
}

string::size_type string::find(char __char, size_type __pos) const
{
    if (__pos >= size()) 
    {
        return npos;
    }

    const_pointer __result = podstl::find(begin() + __pos, end(), __char);
    return (__result != end()) ? (__result - begin()) : npos;
}

string::size_type string::rfind(const char* __c_string, size_type __pos, size_type __length) const
{
    const size_type __this_len = size();
    if (__this_len < __length)
    {
        return npos;
    }
    const_pointer __last = begin() + min(__this_len - __length, __pos) + __length;
    if (__length == 0)
    {
        return __last - begin();
    }
    const_pointer __result = podstl::find_end(begin(), __last, __c_string, __c_string + __length);
    return (__result != __last) ? (__result - begin()) : npos;
}

string::size_type string::rfind(char __char, size_type __pos) const
{
    const size_type __this_length = size();
    if (__this_length < 1) 
    {
        return npos;
    }
    const_iterator __last = begin() + min(__this_length - 1, __pos) + 1;
    const_reverse_iterator __rresult = podstl::find(const_reverse_iterator(__last), rend(), __char);
    return (__rresult != rend()) ? ((__rresult.base() - 1) - begin()) : npos;
}

string::size_type string ::find_first_of(const char* __c_string, size_type __pos, size_type __length) const 
{
    if (__pos >= size()) return npos;

    const_iterator __result = podstl::find_first_of(begin() + __pos, end(), __c_string, __c_string + __length);
    return (__result != end()) ? (__result - begin()) : npos;
}

string::size_type string::find_last_of(const char* __c_string, size_type __pos, size_type __length) const
{
    const size_type __this_length = size();
    if (__this_length < 1) return npos;

    const const_iterator __last = begin() + min(__this_length - 1, __pos) + 1;
    const const_reverse_iterator __rresult = podstl::find_first_of(const_reverse_iterator(__last), rend(), __c_string, __c_string + __length);
    return (__rresult != rend()) ? ((__rresult.base() - 1) - begin()) : npos;
}

string::size_type string::find_first_not_of(const char* __c_string, size_type __pos, size_type __length) const
{
    if (__pos >= size()) return npos;

    const_pointer __result = _find_first_of_aux2(begin() + __pos, end(), __c_string, __c_string + __length, __c_string, not1(_Identity<bool>()), _TrueType());
    return (__result != end()) ? (__result - begin()) : npos;
}

string::size_type string ::find_first_not_of(char __char, size_type __pos) const 
 {
    if (__pos >= size()) return npos;

    const_pointer __result = find_if(begin() + __pos, end(), bind2nd(not_equal_to<char>(), __char));
    return (__result != end()) ? (__result - begin()) : npos;
}

string::size_type string::find_last_not_of(const char* __c_string, size_type __pos, size_type __length) const
{
    const size_type __this_length = size();
    if (__this_length < 1) return npos;

    const_iterator __last = begin() + min(__this_length - 1, __pos) + 1;
    const_reverse_iterator __rresult = _find_first_of_aux2(const_reverse_iterator(__last), rend(), __c_string, __c_string + __length, 
                                                           __c_string, not1(_Identity<bool>()), _TrueType());
    return (__rresult != rend()) ? ((__rresult.base() - 1) - begin()) : npos;
}

string::size_type string::find_last_not_of(char __char, size_type __pos) const
{
    const size_type __this_length = size();
    if (__this_length < 1) return npos;
    const_iterator __last = begin() + min(__this_length - 1, __pos) + 1;
    const_reverse_iterator __rresult = find_if(const_reverse_iterator(__last), rend(), bind2nd(not_equal_to<char>(), __char));
    return (__rresult != rend()) ? ((__rresult.base() - 1) - begin()) : npos;
}

int string::_M_compare(const char* __first1, const char* __last1, const char* __first2, const char* __last2)
{
    STL_ASSERT((__first1 <= __last1) && (__first2 <= __last2));
    const ptrdiff_t __length1 = __last1 - __first1;
    const ptrdiff_t __length2 = __last2 - __first2;
    const int cmp = VOS_MemCmp(__first1, __first2, min(__length1, __length2));
    return (cmp != 0) ? cmp : ((__length1 < __length2) ? -1 : ((__length1 > __length2) ? 1 : 0));
}

string operator+(const string& __left, const string& __right)
{
    typedef string::_ReserveT _ReserveT;
    string __result(_ReserveT(), __left.size() + __right.size());
    __result.append(__left);
    __result.append(__right);
    return __result;
}

string operator+(const char* __c_string, const string& __right)
{
    typedef string::_ReserveT _ReserveT;
    const size_t __left_length = VOS_StrLen(__c_string);
    string __result(_ReserveT(), __left_length + __right.size());
    __result.append(__c_string, __c_string + __left_length);
    __result.append(__right);
    return __result;
}

string operator+(char __char, const string& __right)
{
    typedef string::_ReserveT _ReserveT;
    string __result(_ReserveT(), 1 + __right.size());
    __result.push_back(__char);
    __result.append(__right);
    return __result;
}

string operator+(const string& __left, const char* __c_string)
{
    typedef string::_ReserveT _ReserveT;
    const size_t __length = VOS_StrLen(__c_string);
    string __result(_ReserveT(), __left.size() + __length);
    __result.append(__left);
    __result.append(__c_string, __c_string + __length);
    return __result;
}

string operator+(const string& __left, const char __char)
{
    typedef string::_ReserveT _ReserveT;
    string __result(_ReserveT(), __left.size() + 1);
    __result.append(__left);
    __result.push_back(__char);
    return __result;
}

}
