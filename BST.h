#ifndef BST_H
#define BST_H

#include <functional>
#include <iostream>
#include <stack>
#include <queue>

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
        explicit iterator(NodeBase* n) : node(n) {}

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = key_type;
        using difference_type   = difference_type;
        using pointer           = const_pointer;
        using reference         = const_reference;

        iterator(const iterator &other) : node(other.node) {}

        bool operator==(const iterator& other) const {return node == other.node;}
        bool operator!=(const iterator& other) const {return !(*this == other);}
        const_pointer   operator->() const {return &static_cast<Node*>(node)->data;}
        const_reference operator*() const {return static_cast<Node*>(node)->data;}
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

    template <typename Arg>
    std::pair<iterator, bool> M_insert(Arg&& value)
    {
        NodeBase* parent_node = &header;
        NodeBase* current = header.parent;
        bool is_less = true;

        while (current != nullptr)
        {
            parent_node = current;
            is_less = comp(value, static_cast<Node*>(current)->data);
            if (is_less)
                current = current->left;
            else if (comp(static_cast<Node*>(current)->data, value))
                current = current->right;
            else
                return std::make_pair(iterator(current), false);
        }

        Node* ins = new Node(std::forward<Arg>(value));
        auto ins_base = static_cast<NodeBase*>(ins);
        ins_base->parent = parent_node;

        if (parent_node == &header)
        {
            header.parent = ins_base;
            header.left = ins_base;
            header.right = ins_base;
        }
        else if (is_less)
        {
            parent_node->left = ins_base;
            if (parent_node == header.left)
                header.left = ins_base;
        }
        else
        {
            parent_node->right = ins_base;
            if (parent_node == header.right)
                header.right = ins_base;
        }

        _size++;
        return std::make_pair(iterator(ins_base), true);
    }

    void M_transplant(NodeBase* u, NodeBase* v)
    {
        if (u->parent == &header)
            header.parent = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }

    void M_erase_at_node(NodeBase* x)
    {
        if (x != nullptr)
        {
            M_erase_at_node(x->left);
            M_erase_at_node(x->right);
            delete static_cast<Node*>(x);
        }
    }

    void M_print_inorder(NodeBase* x) const
    {
        if (x != nullptr)
        {
            M_print_inorder(x->left);
            std::cout << static_cast<Node*>(x)->data << " ";
            M_print_inorder(x->right);
        }
    }

    bool M_is_equal(NodeBase* x, NodeBase* y) const
    {
        if (!x && !y)
            return true;
        if (!x || !y)
            return false;
        if (static_cast<Node*>(x)->data != static_cast<Node*>(y)->data)
            return false;
        return M_is_equal(x->left, y->left) && M_is_equal(x->right, y->right);
    }
public:
    BST() { init_empty(); }

    BST(const BST& other) : comp(other.comp)
    {
        init_empty();
        for (const auto& value : other)
            insert(value);
    }

    BST& operator=(const BST& other)
    {
        if (this != &other)
        {
            clear();
            comp = other.comp;
            for (const auto& value : other)
                insert(value);
        }
        return *this;
    }

    BST(BST&& other) noexcept : comp(std::move(other.comp))
    {
        if (other.empty())
            init_empty();
        else
        {
            header.parent = other.header.parent;
            header.left = other.header.left;
            header.right = other.header.right;
            header.parent->parent = &header;
            _size = other._size;
            other.init_empty();
        }
    }

    BST& operator=(BST&& other) noexcept
    {
        if (this != &other)
        {
            clear();
            comp = std::move(other.comp);
            if (!other.empty())
            {
                header.parent = other.header.parent;
                header.left = other.header.left;
                header.right = other.header.right;
                header.parent->parent = &header;
                _size = other._size;
                other.init_empty();
            }
        }
        return *this;
    }

    iterator begin() { return iterator(header.left); }
    iterator end() { return iterator(&header); }
    [[nodiscard]] size_type size() const { return _size; }
    [[nodiscard]] bool empty() const { return _size == 0; }

    std::pair<iterator, bool> insert(const key_type& value)
    {
        return M_insert(value);
    }

    std::pair<iterator, bool> insert(key_type&& value)
    {
        return M_insert(std::move(value));
    }

    iterator find(const key_type& value)
    {
        NodeBase* current = header.parent;

        while (current != nullptr)
        {
            if (comp(value, static_cast<Node*>(current)->data))
                current = current->left;
            else if (comp(static_cast<Node*>(current)->data, value))
                current = current->right;
            else
                return iterator(current);
        }
        return end();
    }

    iterator min_element()
    {
        if (empty())
            return end();
        return iterator(header.left);
    }

    iterator max_element()
    {
        if (empty())
            return end();
        return iterator(header.right);
    }

    iterator lower_bound(const key_type& value)
    {
        NodeBase* current = header.parent;
        NodeBase* result = &header;

        while (current != nullptr)
        {
            if (!comp(static_cast<Node*>(current)->data, value))
            {
                result = current;
                current = current->left;
            }
            else
                current = current->right;
        }
        return iterator(result);
    }

    iterator upper_bound(const key_type& value)
    {
        NodeBase* current = header.parent;
        NodeBase* result = &header;

        while (current != nullptr)
        {
            if (comp(value, static_cast<Node*>(current)->data))
            {
                result = current;
                current = current->left;
            }
            else
                current = current->right;
        }
        return iterator(result);
    }

    iterator get_next(iterator it)
    {
        return iterator(increment(it.node));
    }

    iterator get_prev(iterator it)
    {
        return iterator(decrement(it.node));
    }

    iterator erase(iterator position)
    {
        NodeBase* to_del = position.node;
        if (to_del == &header)
            return end();

        iterator result = position;
        ++result;

        if (to_del == header.left)
            header.left = to_del->right != nullptr ? minimum(to_del->right) : to_del->parent;
        if (to_del == header.right)
            header.right = to_del->left != nullptr ? maximum(to_del->left) : to_del->parent;

        if (to_del->left == nullptr)
            M_transplant(to_del, to_del->right);
        else if (to_del->right == nullptr)
            M_transplant(to_del, to_del->left);
        else
        {
            NodeBase* y = minimum(to_del->right);
            if (y->parent != to_del)
            {
                M_transplant(y, y->right);
                y->right = to_del->right;
                y->right->parent = y;
            }
            M_transplant(to_del, y);
            y->left = to_del->left;
            y->left->parent = y;
        }

        delete static_cast<Node*>(to_del);
        _size--;
        return result;
    }

    bool operator==(const BST& other) const
    {
        if (_size != other.size())
            return false;
        return M_is_equal(header.parent, other.header.parent);
    }

    void print_recursive() const
    {
        M_print_inorder(header.parent);
        std::cout << std::endl;
    }

    void print_stack() const
    {
        if (empty()) return;
        std::stack<NodeBase*> s;
        s.push(header.parent);
        while (!s.empty())
        {
            NodeBase* curr = s.top();
            s.pop();
            std::cout << static_cast<Node*>(curr)->data << " ";
            if (curr->right)
                s.push(curr->right);
            if (curr->left)
                s.push(curr->left);
        }
        std::cout << std::endl;
    }

    void print_reverse_stack() const
    {
        if (empty())
            return;
        std::stack<NodeBase*> s;
        s.push(header.parent);
        while (!s.empty())
            {
            NodeBase* curr = s.top();
            s.pop();
            std::cout << static_cast<Node*>(curr)->data << " ";
            if (curr->left)
                s.push(curr->left);
            if (curr->right)
                s.push(curr->right);
        }
        std::cout << std::endl;
    }

    void print_level_order() const
    {
        if (empty())
            return;
        std::queue<NodeBase*> q;
        q.push(header.parent);

        while (!q.empty())
        {
            NodeBase* curr = q.front();
            q.pop();

            std::cout << static_cast<Node*>(curr)->data << " ";

            if (curr->left)
                q.push(curr->left);
            if (curr->right)
                q.push(curr->right);
        }
        std::cout << std::endl;
    }

    size_type erase(const key_type& value)
    {
        iterator it = find(value);
        if (it == end())
            return 0;
        erase(it);
        return 1;
    }

    void clear()
    {
        M_erase_at_node(header.parent);
        init_empty();
    }

    ~BST()
    {
        clear();
    }
};

#endif