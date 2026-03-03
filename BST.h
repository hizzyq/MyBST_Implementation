#ifndef BST_H
#define BST_H
#include <functional>

template <typename Key, typename Compare = std::less<Key>>
class BST
{
public:
    using value_type      = Key;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;
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

        explicit Node(const value_type& value) : data(value) {}
        explicit Node(value_type&& value) : data(std::move(value)) {}
    };
public:
    class iterator
    {
        friend class BST;

        NodeBase* node;
        iterator(NodeBase* n) : node(n) {}
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = value_type;
        using difference_type   = difference_type;
        using pointer           = pointer;
        using reference         = reference;

        iterator(const iterator &other) : node(other.node) {}

        bool operator==(const iterator& other) const {return node == other.node;}
        bool operator!=(const iterator& other) const {return !(*this == other);}
        pointer   operator->() {return &static_cast<Node*>(node)->data;}
        iterator& operator--() {node = node->parent; return *this;}
        iterator  operator--(int) {iterator result(*this); node = node -> parent; return result;}
        iterator& operator++() {node = node->next; return *this;}
        iterator  operator++(int) {iterator result(*this); node = node -> next; return result;}
        reference operator*() {return static_cast<Node*>(node)->data;}
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
private:
    NodeBase root;
    size_type _size;

    void init_empty()
    {
        root.parent = &root;
        root.left = &root;
        root.right = &root;
        _size = 0;
    }
};

#endif