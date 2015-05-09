// 本文件是内部头文件(文件名以下划线开头)，不允许外部使用者直接包含。
// 本文件定义红黑树数据结构。

#ifndef _PODSTL_TREE_H
#define _PODSTL_TREE_H

// Red-black tree class, designed for use in implementing STL
// associative containers (set, multiset, map, and multimap). The
// insertion and deletion algorithms are based on those in Cormen,
// Leiserson, and Rivest, Introduction to Algorithms (MIT Press, 1990),
// except that
//
// (1) the header cell is maintained with links not only to the root
// but also to the leftmost node of the tree, to enable constant time
// begin(), and to the rightmost node of the tree, to enable linear time
// performance when used with the generic set algorithms (set_union,
// etc.);
//
// (2) when a node being deleted has two children its successor node is
// relinked into its place, rather than copied, so that the only
// iterators invalidated are those referring to the deleted node.

#include "_pair.h"
#include "_algobase.h"
#include "_iterator.h"
#include "_function_base.h"
#include "_operator.h"
#include "_construct.h"

namespace podstl
{

typedef bool _RbTreeColorType;
const _RbTreeColorType RB_TREE_RED = false;
const _RbTreeColorType RB_TREE_BLACK = true;

struct _RbTreeNodeBase 
{
    typedef _RbTreeColorType _ColorType;
    typedef _RbTreeNodeBase* _BasePtr;

    _ColorType __M_color;
    _BasePtr __M_parent;
    _BasePtr __M_left;
    _BasePtr __M_right;

    _BasePtr _M_minimum() 
    {
        _BasePtr __node = this;
        while (__node->__M_left != 0)
        {
            __node = __node->__M_left;
        }
        return __node;
    }

    _BasePtr _M_maximum()
    {
        _BasePtr __node = this;
        while (__node->__M_right != 0)
        {
            __node = __node->__M_right;
        }
        return __node;
    }

    _RbTreeNodeBase* _M_increment();
    _RbTreeNodeBase* _M_decrement();

    // 用于获取_RbTreeNode中__M_value_field的指针，仅能用于子类。由于__M_value_field肯定跟在基类成员后面，所以可以采取这种算法。
    void* _M_get_data() { return static_cast<void*>(this + 1); }
    const void* _M_get_data() const { return static_cast<const void*>(this + 1); }
};

template <typename _Value>
struct _RbTreeNode : public _RbTreeNodeBase 
{
    _Value __M_value_field;
};

struct _RbTreeIteratorBase 
{
    typedef _RbTreeNodeBase* _BasePtr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
    _BasePtr __M_node;
    _RbTreeIteratorBase() : __M_node(0) {}
    _RbTreeIteratorBase(_BasePtr __node) : __M_node(__node) {}
};

template <typename _Value, typename _Traits>
struct _RbTreeIterator : public _RbTreeIteratorBase 
{
    typedef _Value value_type;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _RbTreeIterator<_Value, _Traits> _Self;
    typedef _RbTreeNodeBase* _BasePtr;
    typedef _RbTreeNode<_Value>* _LinkType;

    typedef typename _Traits::_NonConst _NonConstTraits;
    typedef _RbTreeIterator<_Value, _NonConstTraits> iterator;
    typedef typename _Traits::_Const _ConstTraits;
    typedef _RbTreeIterator<_Value, _ConstTraits> const_iterator;

    _RbTreeIterator() {}
    explicit _RbTreeIterator(_BasePtr __node) : _RbTreeIteratorBase(__node) {}
    _RbTreeIterator(const iterator& __it) : _RbTreeIteratorBase(__it.__M_node) {}

    reference operator*() const 
    {
        return static_cast<_LinkType>(__M_node)->__M_value_field;
    }

    pointer operator->() const { return &(operator*()); }

    _Self& operator++() 
    {
        __M_node = __M_node->_M_increment();
        return *this;
    }

    _Self operator++(int) 
    {
        _Self __tmp = *this;
        ++(*this);
        return __tmp;
    }

    _Self& operator--() 
    {
        __M_node = __M_node->_M_decrement();
        return *this;
    }

    _Self operator--(int) 
    {
        _Self __tmp = *this;
        --(*this);
        return __tmp;
    }

    bool operator == (const_iterator __other) const 
    {
        return __M_node == __other.__M_node;
    }

    bool operator != (const_iterator __other) const
    {
        return __M_node != __other.__M_node;
    }
};

template <typename _Value, typename _Traits>
struct type_traits<_RbTreeIterator<_Value, _Traits> > 
{
    typedef _FalseType   has_trivial_default_constructor;
    typedef _TrueType    has_trivial_copy_constructor;
    typedef _TrueType    has_trivial_assignment_operator;
    typedef _TrueType    has_trivial_destructor;
    typedef _FalseType   is_POD_type;
};

class _RbTreeBase 
{
public:
    typedef _RbTreeNodeBase _NodeBase;

private:
    typedef _RbTreeBase _Self;

protected:
    typedef _RbTreeNodeBase* _BasePtr;

public:
    size_t size() const { return __M_node_count; }
    size_t max_size() const { return static_cast<size_t>(-1); }
    bool empty() const { return size() == 0; }
    void _M_verify() const; // 仅用于DEBUG模式下的数据一致性校验

protected:
    _BasePtr _M_root() const { return __M_header.__M_parent; }
    _BasePtr _M_leftmost() const { return __M_header.__M_left; }
    _BasePtr _M_rightmost() const { return __M_header.__M_right; }
    _BasePtr& _M_root() { return __M_header.__M_parent; }
    _BasePtr& _M_leftmost() { return __M_header.__M_left; }
    _BasePtr& _M_rightmost() { return __M_header.__M_right; }

    _RbTreeBase(_DataBinaryPredicate __compare, size_t __node_count);

    void _M_empty_initialize() 
    {
        __M_header.__M_color = RB_TREE_RED; // used to distinguish header from __root, in iterator.operator++
        __M_header.__M_parent = 0;
        __M_header.__M_left = &__M_header;
        __M_header.__M_right = &__M_header;
    }

    void _M_rebind(_NodeBase* __static_node) 
    {
        if (__M_header.__M_parent != 0) 
        {
            __M_header.__M_parent->__M_parent = &__M_header;
        }
        if (__M_header.__M_right == __static_node) 
        {
            __M_header.__M_right = &__M_header;
        }
        if (__M_header.__M_left == __static_node) 
        {
            __M_header.__M_left = &__M_header;
        }
    }

    static void _S_rotate_left(_BasePtr __x, _BasePtr& __root);
    static void _S_rotate_right(_BasePtr __x, _BasePtr& __root);
    static void _S_rebalance(_BasePtr __new_node, _BasePtr& __root);
    static void _S_rebalance_for_erase(_RbTreeNodeBase* __node_to_erase, _RbTreeNodeBase*& __root, _RbTreeNodeBase* __new_child, _RbTreeNodeBase* __new_parent);

    static _BasePtr _S_create_node(size_t __node_size, const void* __data, size_t __data_size);

    static _BasePtr _S_clone_node(_BasePtr __node, size_t __node_size, size_t __data_size)
    {
        _BasePtr __tmp = _S_create_node(__node_size, __node->_M_get_data(), __data_size);
        __tmp->__M_color = __node->__M_color;
        return __tmp;
    }

    static _BasePtr _S_copy(_BasePtr __root, _BasePtr __dest_parent, size_t __node_size, size_t __data_size);
    void _M_initialize(const _Self& __other, size_t __node_size, size_t __data_size);
    static void _S_erase_tree(_RbTreeNodeBase *__root);
    _RbTreeNodeBase* _M_erase_node(_RbTreeNodeBase* __node_to_erase);
    void _M_clear();
    void _M_assign(const _Self& __other, size_t __node_size, size_t __data_size);
    void _M_swap(_Self& __other);

    _RbTreeIteratorBase _M_insert(_RbTreeNodeBase* __parent, const void* __data, size_t __node_size, size_t __data_size, _RbTreeNodeBase* __on_left = 0, _RbTreeNodeBase* __on_right = 0);
    pair<_RbTreeIteratorBase, bool> _M_insert_unique(const void* __data, size_t __node_size, size_t __data_size);
    _RbTreeIteratorBase _M_insert_equal(const void* __data, size_t __node_size, size_t __data_size);
    _RbTreeIteratorBase _M_insert_unique(_RbTreeIteratorBase __position, const void* __data, size_t __node_size, size_t __data_size);
    _RbTreeIteratorBase _M_insert_equal(_RbTreeIteratorBase __position, const void* __data, size_t __node_size, size_t __data_size);

    _BasePtr _M_find(const void* __data) const;
    _BasePtr _M_lower_bound(const void* __data) const;
    _BasePtr _M_upper_bound(const void* __data) const;
    pair<_BasePtr, _BasePtr> _M_equal_range_unique(const void* __data) const;

    _NodeBase __M_header;
    _DataBinaryPredicate __M_compare;
    size_t __M_node_count; // keeps track of size of tree
};

template <typename _Key, typename _Compare, typename _Value, typename _Traits>
class _RbTree : public _RbTreeBase
{
    typedef _RbTreeBase _Base;
    typedef _RbTree<_Key, _Compare, _Value, _Traits> _Self;

protected:
    typedef _RbTreeNode<_Value> _Node;
    typedef _Node* _LinkType;
    typedef _RbTreeColorType _ColorType;

public:
    typedef _Key key_type;
    typedef _Value value_type;
    typedef typename _Traits::pointer pointer;
    typedef const value_type* const_pointer;
    typedef typename _Traits::reference reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef bidirectional_iterator_tag _IteratorCategory;

    typedef typename _Traits::_NonConst _NonConstTraits;
    typedef typename _Traits::_Const _ConstTraits;
    typedef _RbTreeIterator<value_type, _NonConstTraits> iterator;
    typedef _RbTreeIterator<value_type, _ConstTraits> const_iterator;
    typedef podstl::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef podstl::reverse_iterator<iterator> reverse_iterator;

    _Compare key_comp() const { return _Compare(); }

    iterator begin() { return iterator(_M_leftmost()); }
    const_iterator begin() const { return const_iterator(_M_leftmost()); }
    iterator end() { return iterator(&__M_header); }
    const_iterator end() const { return const_iterator(const_cast<_BasePtr>(&__M_header)); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    _RbTree() : _Base(_data_binary_predicate<value_type, _Compare>, 0) {}

    _RbTree(const _Self& __other) : _Base(__other.__M_compare, __other.__M_node_count)
    {
        _M_initialize(__other, sizeof(_Node), sizeof(value_type));
    }

    ~_RbTree() { clear(); }

    _Self& operator=(const _Self& __other)
    {
        _M_assign(__other, sizeof(_Node), sizeof(value_type));
        return *this;
    }

    void swap(_Self& __other) { _M_swap(__other); }

    pair<iterator, bool> insert_unique(const value_type& __data)
    {
        pair<_RbTreeIteratorBase, bool> __return = _M_insert_unique(&__data, sizeof(_Node), sizeof(value_type));
        return pair<iterator, bool>(static_cast<iterator&>(__return.first), __return.second);
    }

    iterator insert_equal(const value_type& __data)
    {
        return static_cast<iterator&>(_M_insert_equal(&__data, sizeof(_Node), sizeof(value_type)));
    }

    iterator insert_unique(iterator __pos, const value_type& __data)
    {
        return static_cast<iterator&>(_M_insert_unique(__pos, &__data, sizeof(_Node), sizeof(value_type)));
    }

    iterator insert_equal(iterator __pos, const value_type& __data)
    {
        return static_cast<iterator&>(_M_insert_equal(__pos, &__data, sizeof(_Node), sizeof(value_type)));
    }

    template<typename _InputIterator> 
    void insert_equal(_InputIterator __first, _InputIterator __last)
    {
        for (; __first != __last; ++__first)
        {
            insert_equal(*__first);
        }
    }

    template<typename _InputIterator> 
    void insert_unique(_InputIterator __first, _InputIterator __last)
    {
        for (; __first != __last; ++__first)
        {
            insert_unique(*__first);
        }
    }

    void erase(iterator __pos) 
    {
        _destroy(&(*__pos));
        _M_erase_node(__pos.__M_node);
    }

    size_type erase(const key_type& __data) 
    {
        pair<iterator, iterator> __pair = equal_range(__data);
        size_type __count = distance(__pair.first, __pair.second);
        erase(__pair.first, __pair.second);
        return __count;
    }

    size_type erase_unique(const key_type& __data)
    {
        iterator __it = find(__data);
        if (__it.__M_node != &__M_header) 
        {
            erase(__it);
            return 1;
        }
        return 0;
    }

    void erase(iterator __first, iterator __last) 
    {
        if ((__first.__M_node == __M_header.__M_left) && (__last.__M_node == &__M_header))
        {
            clear();
        }
        else
        {
            while (__first != __last) erase(__first++);
        }
    }

    void erase(const key_type* __first, const key_type* __last) 
    {
        while (__first != __last) erase(*__first++);
    }

    void clear() 
    {
        _destroy_range(begin(), end());
        _M_clear();
    }

public:
    iterator find(const key_type& __data) { return iterator(_M_find(&__data)); }
    const_iterator find(const key_type& __data) const { return const_iterator(_M_find(&__data)); }

    size_type count(const key_type& __data) const 
    {
        pair<const_iterator, const_iterator> __pair = equal_range(__data);
        return distance(__pair.first, __pair.second);
    }
    
    iterator lower_bound(const key_type& __data) { return iterator(_M_lower_bound(&__data)); }
    const_iterator lower_bound(const key_type& __data) const { return const_iterator(_M_lower_bound(&__data)); }
    iterator upper_bound(const key_type& __data) { return iterator(_M_upper_bound(&__data)); }
    const_iterator upper_bound(const key_type& __data) const { return const_iterator(_M_upper_bound(&__data)); }
    
    pair<iterator, iterator> equal_range(const key_type& __data)
    {
        return pair<iterator, iterator>(lower_bound(__data), upper_bound(__data));
    }
    
    pair<const_iterator, const_iterator> equal_range(const key_type& __data) const
    {
        return pair<const_iterator, const_iterator>(lower_bound(__data), upper_bound(__data));
    }
    
    pair<iterator, iterator> equal_range_unique(const key_type& __data)
    {
        pair<_BasePtr, _BasePtr> __pair = _M_equal_range_unique(&__data);
        return pair<iterator, iterator>(iterator(__pair.first), iterator(__pair.second));
    }
    
    pair<const_iterator, const_iterator> equal_range_unique(const key_type& __data) const
    {
        pair<_BasePtr, _BasePtr> __pair = _M_equal_range_unique(&__data);
        return pair<const_iterator, const_iterator>(const_iterator(__pair.first), const_iterator(__pair.second));
    }

    bool operator==(const _Self& __other) const
    {
        return (size() == __other.size()) && equal(begin(), end(), __other.begin());
    }

    bool operator<(const _Self& __other) const
    {
        return lexicographical_compare(begin(), end(), __other.begin(), __other.end());
    }
};

template <typename _Key, typename _Compare, typename _Value, typename _Traits>
inline void swap(_RbTree<_Key, _Compare, _Value, _Traits>& __x, _RbTree<_Key, _Compare, _Value, _Traits>& __y)
{
    __x.swap(__y);
}

}

#endif // _PODSTL_TREE_H
