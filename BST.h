#ifndef BST_H
#define BST_H

#include <functional>

template <typename Key, typename Compare = std::less<Key>>
class BST
{
public:
    using key_type        = Key;
    using key_compare     = Compare;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using pointer         = key_type*;
    using const_pointer   = const key_type*;
    using reference       = key_type&;
    using const_reference = const key_type&;
private:
    struct NodeBase
    {
        NodeBase* parent;
        NodeBase* left;
        NodeBase* right;

        NodeBase() : parent(nullptr), left(nullptr), right(nullptr) {}
    };

    struct Node : NodeBase
    {
        Key data;

        explicit Node(const key_type& value) : data(value) {}
        explicit Node(key_type&& value) : data(std::move(value)) {}
    };
public:
    class iterator
    {
        friend class BST;
        NodeBase* node;
        iterator(NodeBase* n) : node(n) {}

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = key_type;
        using difference_type   = difference_type;
        using pointer           = const_pointer;
        using reference         = const_reference;

        iterator(const iterator &other) : node(other.node) {}

        bool operator==(const iterator& other) const {return node == other.node;}
        bool operator!=(const iterator& other) const {return !(*this == other);}
        pointer   operator->() {return &static_cast<Node*>(node)->data;}
        reference operator*() {return static_cast<Node*>(node)->data;}
        iterator& operator--()
        {
            node = decrement(node);
            return *this;
        }
        iterator  operator--(int)
        {
            iterator result(*this);
            node = decrement(node);
            return result;
        }
        iterator& operator++()
        {
            node = increment(node);
            return *this;
        }
        iterator  operator++(int)
        {
            iterator result(*this);
            node = increment(node);
            return result;
        }
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
private:
    NodeBase header;
    size_type _size;
    key_compare comp;

    void init_empty()
    {
        header.parent = nullptr;
        header.left = &header;
        header.right = &header;
        _size = 0;
    }

    static NodeBase* minimum(NodeBase* x)
    {
        while (x->left != nullptr)
            x = x->left;
        return x;
    }

    static NodeBase* maximum(NodeBase* x)
    {
        while (x->right != nullptr)
            x = x->right;
        return x;
    }

    static NodeBase* increment(NodeBase* x)
    {
        if (x->right != nullptr)
        {
            return minimum(x->right);
        }
        NodeBase* y = x->parent;
        while (x == y->right)
        {
            x = y;
            y = y->parent;
        }
        if (x->right != y)
        {
            x = y;
        }
        return x;
    }

    static NodeBase* decrement(NodeBase* x) {
        if (x->parent != nullptr && x->parent->parent == x)
        {
            return x->right;
        }
        if (x->left != nullptr)
        {
            return maximum(x->left);
        }
        NodeBase* y = x->parent;
        while (x == y->left)
        {
            x = y;
            y = y->parent;
        }
        return y;
    }

public:
    BST() { init_empty(); }

    iterator begin() { return iterator(header.left); }
    iterator end() { return iterator(&header); }
    [[nodiscard]] size_type size() const { return _size; }
    [[nodiscard]] bool empty() const { return _size == 0; }

    std::pair<iterator, bool> insert(const key_type& value)
    {

    }

    std::pair<iterator, bool> insert(key_type&& value)
    {

    }
};

#endif