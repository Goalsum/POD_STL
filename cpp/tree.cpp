// 本文件定义红黑树的非模板实现。

#include "_tree.h"


#define __iterator__  _STLP_TYPENAME_ON_RETURN_TYPE _RbTree<_Key, _Compare, _Value, _KeyOfValue, _Traits>::iterator
#define __size_type__  _STLP_TYPENAME_ON_RETURN_TYPE _RbTree<_Key, _Compare, _Value, _KeyOfValue, _Traits>::size_type

namespace podstl
{

_RbTreeNodeBase* _RbTreeNodeBase::_M_increment()
{
    _BasePtr __next_node = this;
    if (__next_node->__M_right != 0)
    {
        __next_node = __next_node->__M_right->_M_minimum();
    }
    else
    {
        _BasePtr __parent = __next_node->__M_parent;
        while (__next_node == __parent->__M_right)
        {
            __next_node = __parent;
            __parent = __parent->__M_parent;
        }

        // check special case: This is necessary if __next_node is the
        // _M_head and the tree contains only a single node __parent. In
        // that case parent, left and right all point to __parent!
        if (__next_node->__M_right != __parent)
        {
            __next_node = __parent;
        }
    }
    return __next_node;
}

_RbTreeNodeBase* _RbTreeNodeBase::_M_decrement()
{
    _BasePtr __pre_node = this;
    if ((__pre_node->__M_color == RB_TREE_RED) && (__pre_node->__M_parent->__M_parent == __pre_node))   // this is head
    {
        __pre_node = __pre_node->__M_right;
    }
    else if (__pre_node->__M_left != 0)
    {
        __pre_node = __pre_node->__M_left->_M_maximum();
    }
    else
    {
        _BasePtr __parent = __pre_node->__M_parent;
        while (__pre_node == __parent->__M_left)
        {
            __pre_node = __parent;
            __parent = __parent->__M_parent;
            STL_ASSERT(__pre_node != __parent->__M_parent); // 不允许遍历到头结点
        }
        __pre_node = __parent;
    }
    return __pre_node;
}

_RbTreeBase::_RbTreeBase(_DataBinaryPredicate __compare, size_t __node_count) : __M_compare(__compare), __M_node_count(__node_count)
{
    _M_empty_initialize();
}

_RbTreeBase::_BasePtr _RbTreeBase::_S_create_node(size_t __node_size, const void* __data, size_t __data_size)
{
    _BasePtr __new_node = static_cast<_BasePtr>(STL_Malloc(__node_size));
    VOS_memcpy_s(__new_node->_M_get_data(), __data_size, __data, __data_size);
    __new_node->__M_left = 0;
    __new_node->__M_right = 0;
    return __new_node;
}

_RbTreeBase::_BasePtr _RbTreeBase::_S_copy(_RbTreeNodeBase* __root, _RbTreeNodeBase* __dest_parent, size_t __node_size, size_t __data_size)
{
    // structural copy.  __root and __dest_parent must be non-null.
    _BasePtr __top = _S_clone_node(__root, __node_size, __data_size);
    __top->__M_parent = __dest_parent;

    if (__root->__M_right)
    {
        __top->__M_right = _S_copy(__root->__M_right, __top, __node_size, __data_size);
    }
    __dest_parent = __top;
    __root = __root->__M_left;

    while (__root != 0)
    {
        _BasePtr __left_child = _S_clone_node(__root, __node_size, __data_size);
        __dest_parent->__M_left = __left_child;
        __left_child->__M_parent = __dest_parent;
        if (__root->__M_right)
        {
            __left_child->__M_right = _S_copy(__root->__M_right, __left_child, __node_size, __data_size);
        }
        __dest_parent = __left_child;
        __root = __root->__M_left;
    }

    return __top;
}

void _RbTreeBase::_M_initialize(const _Self& __other, size_t __node_size, size_t __data_size)
{
    if (__other._M_root() == 0) return;

    _M_root() = _S_copy(__other._M_root(), &__M_header, __node_size, __data_size);
    _M_leftmost() = _M_root()->_M_minimum();
    _M_rightmost() = _M_root()->_M_maximum();
}

void _RbTreeBase::_M_assign(const _Self& __other, size_t __node_size, size_t __data_size)
{
    if (this != &__other)
    {
        _M_clear();
        __M_compare = __other.__M_compare;
        _M_initialize(__other, __node_size, __data_size);
        __M_node_count = __other.__M_node_count;
    }
}

void _RbTreeBase::_M_swap(_Self& __other)
{
    if (__other.empty())
    {
        if (empty()) return;
        podstl::swap(__other.__M_header, __M_header);
        __other._M_rebind(&__M_header);
        _M_empty_initialize();
    }
    else if (empty())
    {
        __other._M_swap(*this);
        return;
    }
    else
    {
        podstl::swap(__M_header, __other.__M_header);
        _M_rebind(&__other.__M_header);
        __other._M_rebind(&__M_header);
    }
    podstl::swap(__M_node_count, __other.__M_node_count);
    podstl::swap(__M_compare, __other.__M_compare);
}

void _RbTreeBase::_S_rotate_left(_RbTreeNodeBase* __old_top, _RbTreeNodeBase*& __root)
{
    _RbTreeNodeBase* __new_top = __old_top->__M_right;
    __old_top->__M_right = __new_top->__M_left;
    if (__new_top->__M_left != 0)
    {
        __new_top->__M_left->__M_parent = __old_top;
    }
    __new_top->__M_parent = __old_top->__M_parent;

    if (__old_top == __root)
    {
        __root = __new_top;
    }
    else if (__old_top == __old_top->__M_parent->__M_left)
    {
        __old_top->__M_parent->__M_left = __new_top;
    }
    else
    {
        __old_top->__M_parent->__M_right = __new_top;
    }
    __new_top->__M_left = __old_top;
    __old_top->__M_parent = __new_top;
}

void _RbTreeBase::_S_rotate_right(_RbTreeNodeBase* __old_top, _RbTreeNodeBase*& __root)
{
    _RbTreeNodeBase* __new_top = __old_top->__M_left;
    __old_top->__M_left = __new_top->__M_right;
    if (__new_top->__M_right != 0)
    {
        __new_top->__M_right->__M_parent = __old_top;
    }
    __new_top->__M_parent = __old_top->__M_parent;

    if (__old_top == __root)
    {
        __root = __new_top;
    }
    else if (__old_top == __old_top->__M_parent->__M_right)
    {
        __old_top->__M_parent->__M_right = __new_top;
    }
    else
    {
        __old_top->__M_parent->__M_left = __new_top;
    }
    __new_top->__M_right = __old_top;
    __old_top->__M_parent = __new_top;
}

void _RbTreeBase::_S_rebalance(_RbTreeNodeBase* __new_node, _RbTreeNodeBase*& __root)
{
    __new_node->__M_color = RB_TREE_RED;
    while ((__new_node != __root) && (__new_node->__M_parent->__M_color == RB_TREE_RED)) 
    {
        if ((__new_node->__M_parent) == __new_node->__M_parent->__M_parent->__M_left) 
        {
            _RbTreeNodeBase* __uncle = __new_node->__M_parent->__M_parent->__M_right;
            if (__uncle && (__uncle->__M_color == RB_TREE_RED))
            {
                __new_node->__M_parent->__M_color = RB_TREE_BLACK;
                __uncle->__M_color = RB_TREE_BLACK;
                __new_node->__M_parent->__M_parent->__M_color = RB_TREE_RED;
                __new_node = __new_node->__M_parent->__M_parent;
            }
            else 
            {
                if (__new_node == __new_node->__M_parent->__M_right) 
                {
                    __new_node = __new_node->__M_parent;
                    _S_rotate_left(__new_node, __root);
                }
                __new_node->__M_parent->__M_color = RB_TREE_BLACK;
                __new_node->__M_parent->__M_parent->__M_color = RB_TREE_RED;
                _S_rotate_right(__new_node->__M_parent->__M_parent, __root);
            }
        }
        else 
        {
            _RbTreeNodeBase* __uncle = __new_node->__M_parent->__M_parent->__M_left;
            if (__uncle && (__uncle->__M_color == RB_TREE_RED))
            {
                __new_node->__M_parent->__M_color = RB_TREE_BLACK;
                __uncle->__M_color = RB_TREE_BLACK;
                __new_node->__M_parent->__M_parent->__M_color = RB_TREE_RED;
                __new_node = __new_node->__M_parent->__M_parent;
            }
            else 
            {
                if (__new_node == __new_node->__M_parent->__M_left) 
                {
                    __new_node = __new_node->__M_parent;
                    _S_rotate_right(__new_node, __root);
                }
                __new_node->__M_parent->__M_color = RB_TREE_BLACK;
                __new_node->__M_parent->__M_parent->__M_color = RB_TREE_RED;
                _S_rotate_left(__new_node->__M_parent->__M_parent, __root);
            }
        }
    }
    __root->__M_color = RB_TREE_BLACK;
}

void _RbTreeBase::_S_rebalance_for_erase(_RbTreeNodeBase* __node_to_erase, _RbTreeNodeBase*& __root, _RbTreeNodeBase* __new_child, _RbTreeNodeBase* __new_parent)
{
    if (__node_to_erase->__M_color == RB_TREE_RED) return;

    while ((__new_child != __root) && ((__new_child == 0) || (__new_child->__M_color == RB_TREE_BLACK)))
    {
        if (__new_child == __new_parent->__M_left)
        {
            _RbTreeNodeBase* __sibling = __new_parent->__M_right;
            if (__sibling->__M_color == RB_TREE_RED)
            {
                __sibling->__M_color = RB_TREE_BLACK;
                __new_parent->__M_color = RB_TREE_RED;
                _S_rotate_left(__new_parent, __root);
                __sibling = __new_parent->__M_right;
            }
            if (((__sibling->__M_left == 0) || (__sibling->__M_left->__M_color == RB_TREE_BLACK)) 
                && ((__sibling->__M_right == 0) || (__sibling->__M_right->__M_color == RB_TREE_BLACK)))
            {
                __sibling->__M_color = RB_TREE_RED;
                __new_child = __new_parent;
                __new_parent = __new_parent->__M_parent;
            }
            else
            {
                if ((__sibling->__M_right == 0) || (__sibling->__M_right->__M_color == RB_TREE_BLACK))
                {
                    if (__sibling->__M_left) __sibling->__M_left->__M_color = RB_TREE_BLACK;
                    __sibling->__M_color = RB_TREE_RED;
                    _S_rotate_right(__sibling, __root);
                    __sibling = __new_parent->__M_right;
                }
                __sibling->__M_color = __new_parent->__M_color;
                __new_parent->__M_color = RB_TREE_BLACK;
                if (__sibling->__M_right) __sibling->__M_right->__M_color = RB_TREE_BLACK;
                _S_rotate_left(__new_parent, __root);
                break;
            }
        }
        else    // same as above, with __M_right <-> __M_left.
        {
            _RbTreeNodeBase* __sibling = __new_parent->__M_left;
            if (__sibling->__M_color == RB_TREE_RED)
            {
                __sibling->__M_color = RB_TREE_BLACK;
                __new_parent->__M_color = RB_TREE_RED;
                _S_rotate_right(__new_parent, __root);
                __sibling = __new_parent->__M_left;
            }
            if (((__sibling->__M_right == 0) || (__sibling->__M_right->__M_color == RB_TREE_BLACK))
                && ((__sibling->__M_left == 0) || (__sibling->__M_left->__M_color == RB_TREE_BLACK)))
            {
                __sibling->__M_color = RB_TREE_RED;
                __new_child = __new_parent;
                __new_parent = __new_parent->__M_parent;
            }
            else
            {
                if ((__sibling->__M_left == 0) || (__sibling->__M_left->__M_color == RB_TREE_BLACK))
                {
                    if (__sibling->__M_right) __sibling->__M_right->__M_color = RB_TREE_BLACK;
                    __sibling->__M_color = RB_TREE_RED;
                    _S_rotate_left(__sibling, __root);
                    __sibling = __new_parent->__M_left;
                }
                __sibling->__M_color = __new_parent->__M_color;
                __new_parent->__M_color = RB_TREE_BLACK;
                if (__sibling->__M_left) __sibling->__M_left->__M_color = RB_TREE_BLACK;
                _S_rotate_right(__new_parent, __root);
                break;
            }
        }
    }

    if (__new_child)
    {
        __new_child->__M_color = RB_TREE_BLACK;
    }
}

_RbTreeNodeBase* _RbTreeBase::_M_erase_node(_RbTreeNodeBase* __node_to_erase)
{
    _RbTreeNodeBase* __single_child = __node_to_erase;
    _RbTreeNodeBase* __new_child;
    _RbTreeNodeBase* __new_parent;

    if (__single_child->__M_left == 0)     // __node_to_erase has at most one non-null child. __single_child == __node_to_erase.
    {
        __new_child = __single_child->__M_right;     // __new_child might be null.
    }
    else
    {
        if (__single_child->__M_right == 0)  // __node_to_erase has exactly one non-null child. __single_child == __node_to_erase.
        {
            __new_child = __single_child->__M_left;    // __new_child is not null.
        }
        else   // __node_to_erase has two non-null children.  Set __single_child to __node_to_erase's successor. 
        {
            __single_child = __single_child->__M_right->_M_minimum();
            __new_child = __single_child->__M_right;   // __new_child might be null.
        }
    }

    if (__single_child != __node_to_erase)
    {
        // relink __single_child in place of __node_to_erase. __single_child is __node_to_erase's successor
        __node_to_erase->__M_left->__M_parent = __single_child;
        __single_child->__M_left = __node_to_erase->__M_left;
        if (__single_child != __node_to_erase->__M_right)
        {
            __new_parent = __single_child->__M_parent;
            if (__new_child != 0) __new_child->__M_parent = __single_child->__M_parent;
            __single_child->__M_parent->__M_left = __new_child;      // __single_child must be a left child
            __single_child->__M_right = __node_to_erase->__M_right;
            __node_to_erase->__M_right->__M_parent = __single_child;
        }
        else
        {
            __new_parent = __single_child;
        }
        if (_M_root() == __node_to_erase)
        {
            _M_root() = __single_child;
        }
        else if (__node_to_erase->__M_parent->__M_left == __node_to_erase)
        {
            __node_to_erase->__M_parent->__M_left = __single_child;
        }
        else
        {
            __node_to_erase->__M_parent->__M_right = __single_child;
        }
        __single_child->__M_parent = __node_to_erase->__M_parent;
        podstl::swap(__single_child->__M_color, __node_to_erase->__M_color);
    }
    else    // __single_child == __node_to_erase
    {
        __new_parent = __single_child->__M_parent;
        if (__new_child) __new_child->__M_parent = __single_child->__M_parent;
        if (_M_root() == __node_to_erase)
        {
            _M_root() = __new_child;
        }
        else
        {
            if (__node_to_erase->__M_parent->__M_left == __node_to_erase)
            {
                __node_to_erase->__M_parent->__M_left = __new_child;
            }
            else
            {
                __node_to_erase->__M_parent->__M_right = __new_child;
            }
        }

        if (_M_leftmost() == __node_to_erase)
        {
            if (__node_to_erase->__M_right == 0)        // __node_to_erase->__M_left must be null also
            {
                _M_leftmost() = __node_to_erase->__M_parent;   // makes __leftmost == __M_header if __node_to_erase == __root
            }
            else
            {
                _M_leftmost() = __new_child->_M_minimum();
            }
        }

        if (_M_rightmost() == __node_to_erase)
        {
            if (__node_to_erase->__M_left == 0)         // __node_to_erase->__M_right must be null also
            {
                _M_rightmost() = __node_to_erase->__M_parent;  // makes __rightmost == __M_header if __node_to_erase == __root
            }
            else    // __new_child == __node_to_erase->__M_left
            {
                _M_rightmost() = __new_child->_M_maximum();
            }
        }
    }

    _S_rebalance_for_erase(__node_to_erase, _M_root(), __new_child, __new_parent);
    STL_Free(__node_to_erase);
    --__M_node_count;
    return __node_to_erase;
}

void _RbTreeBase::_S_erase_tree(_RbTreeNodeBase* __root)
{
    // erase without rebalancing
    while (__root != 0)
    {
        _S_erase_tree(__root->__M_right);
        _BasePtr __left_child = __root->__M_left;
        STL_Free(__root);
        __root = __left_child;
    }
}

void _RbTreeBase::_M_clear()
{
    _S_erase_tree(_M_root());
    _M_empty_initialize();
    __M_node_count = 0;
}

// specify __on_right != 0 to bypass comparison as false or __on_left != 0 to bypass comparison as true
_RbTreeIteratorBase _RbTreeBase::_M_insert(_RbTreeNodeBase* __parent, const void* __data, size_t __node_size, size_t __data_size, _RbTreeNodeBase* __on_left /* = 0 */, _RbTreeNodeBase* __on_right /* = 0 */)
{
    _BasePtr __new_node;

    if (__parent == &__M_header) 
    {
        __new_node = _S_create_node(__node_size, __data, __data_size);
        _M_leftmost() = __new_node;
        _M_root() = __new_node;
        _M_rightmost() = __new_node;
    }
    else if ((__on_right == 0) && ((__on_left != 0) || __M_compare(__data, __parent->_M_get_data())))
    {
        __new_node = _S_create_node(__node_size, __data, __data_size);
        __parent->__M_left = __new_node;
        if (__parent == _M_leftmost())
        {
            _M_leftmost() = __new_node;   // maintain _M_leftmost() pointing to min node
        }
    }
    else 
    {
        __new_node = _S_create_node(__node_size, __data, __data_size);
        __parent->__M_right = __new_node;
        if (__parent == _M_rightmost())
        {
            _M_rightmost() = __new_node;  // maintain _M_rightmost() pointing to max node
        }
    }
    __new_node->__M_parent = __parent;
    _S_rebalance(__new_node, __M_header.__M_parent);
    ++__M_node_count;
    return _RbTreeIteratorBase(__new_node);
}

pair<_RbTreeIteratorBase, bool> _RbTreeBase::_M_insert_unique(const void* __data, size_t __node_size, size_t __data_size)
{
    _BasePtr __parent = &__M_header;
    _BasePtr __child = _M_root();
    bool __comp = true;
    while (__child != 0)
    {
        __parent = __child;
        __comp = __M_compare(__data, __child->_M_get_data());
        __child = __comp ? __child->__M_left : __child->__M_right;
    }
    _BasePtr __lower_bound = __parent;
    if (__comp)
    {
        if (__parent == _M_leftmost())
        {
            return pair<_RbTreeIteratorBase, bool>(_M_insert(__parent, __data, __node_size, __data_size, __parent), true);
        }
        else
        {
            __lower_bound = __parent->_M_decrement();
        }
    }
    if (__M_compare(__lower_bound->_M_get_data(), __data))
    {
        return pair<_RbTreeIteratorBase, bool>(_M_insert(__parent, __data, __node_size, __data_size), true);
    }
    return pair<_RbTreeIteratorBase, bool>(_RbTreeIteratorBase(__lower_bound), false);
}

_RbTreeIteratorBase _RbTreeBase::_M_insert_equal(const void* __data, size_t __node_size, size_t __data_size)
{
    _BasePtr __parent = &__M_header;
    _BasePtr __child = _M_root();
    while (__child != 0) 
    {
        __parent = __child;
        if (__M_compare(__data, __child->_M_get_data()))
        {
            __child = __child->__M_left;
        }
        else
        {
            __child = __child->__M_right;
        }
    }
    return _M_insert(__parent, __data, __node_size, __data_size);
}

_RbTreeIteratorBase _RbTreeBase::_M_insert_unique(_RbTreeIteratorBase __position, const void* __data, size_t __node_size, size_t __data_size)
{
    if (__position.__M_node == __M_header.__M_left)     // begin()
    { 
        // if the container is empty, fall back on insert_unique.
        if (empty())
        {
            return _M_insert_unique(__data, __node_size, __data_size).first;
        }

        if (__M_compare(__data, __position.__M_node->_M_get_data()))
        {
            return _M_insert(__position.__M_node, __data, __node_size, __data_size, __position.__M_node);
        }
        else 
        {
            bool __comp_pos_v = __M_compare(__position.__M_node->_M_get_data(), __data);
            if (__comp_pos_v == false)  // compare > and compare < both false so compare equal
            {
                return __position;
            }

            _BasePtr __after = __position.__M_node->_M_increment();

            // Check for only one member -- in that case, __position points to itself,
            // and attempting to increment will cause an infinite loop.
            if (__after == &__M_header)
            {
                // Check guarantees exactly one member, so comparison was already
                // performed and we know the result; skip repeating it in _M_insert
                // by specifying a non-zero fourth argument.
                return _M_insert(__position.__M_node, __data, __node_size, __data_size, 0, __position.__M_node);
            }

            // All other cases:
            // Optimization to catch insert-equivalent -- save comparison results,
            // and we get this for free.
            if (__M_compare(__data, __after->_M_get_data()))
            {
                if (__position.__M_node->__M_right == 0)
                {
                    return _M_insert(__position.__M_node, __data, __node_size, __data_size, 0, __position.__M_node);
                }
                else
                {
                    return _M_insert(__after, __data, __node_size, __data_size, __after);
                }
            }
            else 
            {
                return _M_insert_unique(__data, __node_size, __data_size).first;
            }
        }
    }
    else if (__position.__M_node == &__M_header)    // end()
    { 
        if (__M_compare(_M_rightmost()->_M_get_data(), __data)) 
        {
            return _M_insert(_M_rightmost(), __data, __node_size, __data_size, 0, __position.__M_node); // Last argument only needs to be non-null
        }
        else
        {
            return _M_insert_unique(__data, __node_size, __data_size).first;
        }
    }
    else 
    {
        _BasePtr __before = __position.__M_node->_M_decrement();
        bool __comp_v_pos = __M_compare(__data, __position.__M_node->_M_get_data());
        if (__comp_v_pos && __M_compare(__before->_M_get_data(), __data))
        {
            if (__before->__M_right == 0)
            {
                return _M_insert(__before, __data, __node_size, __data_size, 0, __before); // Last argument only needs to be non-null
            }
            else
            {
                return _M_insert(__position.__M_node, __data, __node_size, __data_size, __position.__M_node);
            }
        }
        else 
        {
            _BasePtr __after = __position.__M_node->_M_increment();;

            // Optimization to catch equivalent cases and avoid unnecessary comparisons
            bool __comp_pos_v = !__comp_v_pos;  // Stored this result earlier

            // If the earlier comparison was true, this comparison doesn't need to be
            // performed because it must be false.  However, if the earlier comparison
            // was false, we need to perform this one because in the equal case, both will
            // be false.
            if (!__comp_v_pos) 
            {
                __comp_pos_v = __M_compare(__position.__M_node->_M_get_data(), __data);
            }

            if (__comp_pos_v && ((__after == &__M_header) || __M_compare(__data, __after->_M_get_data()))) 
            {
                if (__position.__M_node->__M_right == 0)
                {
                    return _M_insert(__position.__M_node, __data, __node_size, __data_size, 0, __position.__M_node);
                }
                else
                {
                    return _M_insert(__after, __data, __node_size, __data_size, __after);
                }
            }
            else 
            {
                // Test for equivalent case
                if (__comp_v_pos == __comp_pos_v)
                {
                    return __position;
                }
                else
                {
                    return _M_insert_unique(__data, __node_size, __data_size).first;
                }
            }
        }
    }
}

_RbTreeIteratorBase _RbTreeBase::_M_insert_equal(_RbTreeIteratorBase __position, const void* __data, size_t __node_size, size_t __data_size)
{
    if (__position.__M_node == _M_leftmost())   // begin()
    { 
        // Check for zero members
        if (empty())
        {
            return _M_insert_equal(__data, __node_size, __data_size);
        }

        if (!__M_compare(__position.__M_node->_M_get_data(), __data))
        {
            return _M_insert(__position.__M_node, __data, __node_size, __data_size, __position.__M_node);
        }
        else 
        {
            _BasePtr __after = __position.__M_node->_M_increment();

            // Already know that compare(pos, v) must be true!
            // Therefore, we want to know if compare(after, v) is false.
            // (i.e., we now pos < v, now we want to know if v <= after)
            // If not, invalid hint.
            if ((__after == &__M_header) || !__M_compare(__after->_M_get_data(), __data)) 
            {
                if (__position.__M_node->__M_right == 0)
                {
                    return _M_insert(__position.__M_node, __data, __node_size, __data_size, 0, __position.__M_node);
                }
                else
                {
                    return _M_insert(__after, __data, __node_size, __data_size, __after);
                }
            }
            else   // Invalid hint
            {
                return _M_insert_equal(__data, __node_size, __data_size);
            }
        }
    }
    else if (__position.__M_node == &__M_header)  // end()
    { 
        if (!__M_compare(__data, _M_rightmost()->_M_get_data()))
        {
            return _M_insert(_M_rightmost(), __data, __node_size, __data_size, 0, __position.__M_node); // Last argument only needs to be non-null
        }
        else
        {
            return _M_insert_equal(__data, __node_size, __data_size);
        }
    }
    else 
    {
        _BasePtr __before = __position.__M_node->_M_decrement();

        // store the result of the comparison between pos and v so
        // that we don't have to do it again later.  Note that this reverses the shortcut
        // on the if, possibly harming efficiency in comparisons; I think the harm will
        // be negligible, and to do what I want to do (save the result of a comparison so
        // that it can be re-used) there is no alternative.  Test here is for before <= v <= pos.
        bool __comp_pos_v = __M_compare(__position.__M_node, __data);
        if (!__comp_pos_v && !__M_compare(__data, __before->_M_get_data())) 
        {
            if (__before->__M_right == 0)
            {
                return _M_insert(__before, __data, __node_size, __data_size, 0, __before); // Last argument only needs to be non-null
            }
            else
            {
                return _M_insert(__position.__M_node, __data, __node_size, __data_size, __position.__M_node);
            }
        }
        else 
        {
            // Test for pos < v <= after
            _BasePtr __after = __position.__M_node->_M_increment();

            if (__comp_pos_v && ((__after == &__M_header) || !__M_compare(__after->_M_get_data(), __data)))
            {
                if (__position.__M_node->__M_right == 0)
                {
                    return _M_insert(__position.__M_node, __data, __node_size, __data_size, 0, __position.__M_node);
                }
                else
                {
                    return _M_insert(__after, __data, __node_size, __data_size, __after);
                }
            }
            else   // Invalid hint
            { 
                return _M_insert_equal(__data, __node_size, __data_size);
            }
        }
    }
}

_RbTreeBase::_BasePtr _RbTreeBase::_M_find(const void* __data) const
{
    _BasePtr __greater_or_equal = _M_lower_bound(__data);
    if (__greater_or_equal != &__M_header)
    {
        if (__M_compare(__data, __greater_or_equal->_M_get_data()))
        {
            __greater_or_equal = const_cast<_BasePtr>(&__M_header);
        }
    }
    return __greater_or_equal;
}

_RbTreeBase::_BasePtr _RbTreeBase::_M_lower_bound(const void* __data) const
{
    _BasePtr __greater_or_equal = const_cast<_BasePtr>(&__M_header);
    _BasePtr __less = _M_root();

    while (__less != 0)
    {
        if (__M_compare(__less->_M_get_data(), __data))
        {
            __less = __less->__M_right;
        }
        else
        {
            __greater_or_equal = __less;
            __less = __less->__M_left;
        }
    }

    return __greater_or_equal;
}

_RbTreeBase::_BasePtr _RbTreeBase::_M_upper_bound(const void* __data) const
{
    _BasePtr __greater = const_cast<_BasePtr>(&__M_header);
    _BasePtr __less_or_equal = _M_root();

    while (__less_or_equal != 0)
    {
        if (__M_compare(__data, __less_or_equal->_M_get_data()))
        {
            __greater = __less_or_equal;
            __less_or_equal = __less_or_equal->__M_left;
        }
        else
        {
            __less_or_equal = __less_or_equal->__M_right;
        }
    }

    return __greater;
}

pair<_RbTreeBase::_BasePtr, _RbTreeBase::_BasePtr> _RbTreeBase::_M_equal_range_unique(const void* __data) const
{
    pair<_BasePtr, _BasePtr> __pair;
    __pair.second = _M_lower_bound(__data);
    if ((__pair.second != &__M_header) && !__M_compare(__data, __pair.second->_M_get_data()))
    {
        __pair.first = __pair.second;
        __pair.second = __pair.second->_M_increment();
    }
    else
    {
        __pair.first = __pair.second;
    }
    return __pair;
}

int _black_count(_RbTreeNodeBase* __node, _RbTreeNodeBase* __root) 
{
    if (__node == 0) return 0;

    int __black_nodes = __node->__M_color == RB_TREE_BLACK ? 1 : 0;
    if (__node == __root)
    {
        return __black_nodes;
    }
    return __black_nodes + _black_count(__node->__M_parent, __root);
}

void _RbTreeBase::_M_verify() const
{
    if ((__M_node_count == 0) || (__M_header.__M_parent == &__M_header))
    {
        STL_ASSERT(__M_node_count == 0);
        STL_ASSERT(__M_header.__M_parent == 0);
        STL_ASSERT(__M_header.__M_left == &__M_header);
        STL_ASSERT(__M_header.__M_right == &__M_header);
        return;
    }

    int __len = _black_count(_M_leftmost(), _M_root());
    int __node_count = 0;
    for (_BasePtr __cur_node = _M_leftmost(); __cur_node != &__M_header; __cur_node = __cur_node->_M_increment(), ++__node_count)
    {
        _BasePtr __left_child = __cur_node->__M_left;
        _BasePtr __right_child = __cur_node->__M_right;

        if (__cur_node->__M_color == RB_TREE_RED)
        {
            if (__left_child != 0)
            {
                STL_ASSERT(__left_child->__M_color != RB_TREE_RED);
            }
            if (__right_child != 0)
            {
                STL_ASSERT(__right_child->__M_color != RB_TREE_RED);
            }
        }

        if (__left_child != 0)
        {
            STL_ASSERT(__left_child->__M_parent == __cur_node);
            STL_ASSERT(!__M_compare(__cur_node->_M_get_data(), __left_child->_M_get_data()));
        }

        if (__right_child != 0)
        {
            STL_ASSERT(__right_child->__M_parent == __cur_node);
            STL_ASSERT(!__M_compare(__right_child->_M_get_data(), __cur_node->_M_get_data()));
        }

        if ((__left_child == 0) && (__right_child == 0))
        {
            STL_ASSERT(_black_count(__cur_node, _M_root()) == __len);
        }
    }

    STL_ASSERT(_M_leftmost() == _M_root()->_M_minimum());
    STL_ASSERT(_M_rightmost() == _M_root()->_M_maximum());
    STL_ASSERT(__node_count == __M_node_count);
}

}
