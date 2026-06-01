#include "bst.h"

#include <iostream>

// ======================================================
// Helpers
// ======================================================

BinarySearchTree::Node *
BinarySearchTree::minNode(Node *node)
{
    if (!node)
    {
        return nullptr;
    }

    while (node->left)
    {
        node = node->left;
    }

    return node;
}

BinarySearchTree::Node *
BinarySearchTree::maxNode(Node *node)
{
    if (!node)
    {
        return nullptr;
    }

    while (node->right)
    {
        node = node->right;
    }

    return node;
}

const BinarySearchTree::Node *
BinarySearchTree::minNode(
    const Node *node) const
{
    if (!node)
    {
        return nullptr;
    }

    while (node->left)
    {
        node = node->left;
    }

    return node;
}

const BinarySearchTree::Node *
BinarySearchTree::maxNode(
    const Node *node) const
{
    if (!node)
    {
        return nullptr;
    }

    while (node->right)
    {
        node = node->right;
    }

    return node;
}

void BinarySearchTree::deleteSubtree(
    Node *node)
{
    if (!node)
    {
        return;
    }

    deleteSubtree(node->left);
    deleteSubtree(node->right);

    delete node;
}

// ======================================================
// Node
// ======================================================

BinarySearchTree::Node::Node(
    Key key,
    Value value,
    Node *parent,
    Node *left,
    Node *right)
    : keyValuePair(key, value),
      parent(parent),
      left(left),
      right(right)
{
}

BinarySearchTree::Node::Node(
    const Node &other)
    : keyValuePair(other.keyValuePair)
{
    if (other.left)
    {
        left = new Node(*other.left);
        left->parent = this;
    }

    if (other.right)
    {
        right = new Node(*other.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(
    const Node &other) const
{
    return keyValuePair ==
           other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree() const
{
    if (left)
    {
        left->output_node_tree();
    }

    std::cout
        << keyValuePair.first
        << " "
        << keyValuePair.second
        << '\n';

    if (right)
    {
        right->output_node_tree();
    }
}

void BinarySearchTree::Node::insert(
    const Key &key,
    const Value &value)
{
    if (key < keyValuePair.first)
    {
        if (left)
        {
            left->insert(key, value);
        }
        else
        {
            left = new Node(
                key,
                value,
                this);
        }
    }
    else
    {
        if (right)
        {
            right->insert(key, value);
        }
        else
        {
            right = new Node(
                key,
                value,
                this);
        }
    }
}

void BinarySearchTree::erase(
    const Key& key)
{
    while (true)
    {
        Node* node = _root;

        // поиск узла
        while (node)
        {
            if (key <
                node->keyValuePair.first)
            {
                node = node->left;
            }
            else if (
                key >
                node->keyValuePair.first)
            {
                node = node->right;
            }
            else
            {
                break;
            }
        }

        // больше нет такого ключа
        if (!node)
        {
            return;
        }

        // 2 потомка
        if (node->left &&
            node->right)
        {
            Node* succ =
                node->right;

            while (succ->left)
            {
                succ = succ->left;
            }

            node->keyValuePair =
                succ->keyValuePair;

            node = succ;
        }

        // 0 или 1 потомок
        Node* child =
            node->left
            ? node->left
            : node->right;

        if (child)
        {
            child->parent =
                node->parent;
        }

        // удаляем корень
        if (!node->parent)
        {
            _root = child;
        }
        else if (
            node ==
            node->parent->left)
        {
            node->parent->left =
                child;
        }
        else
        {
            node->parent->right =
                child;
        }

        delete node;

        --_size;
    }
}

// ======================================================
// BST
// ======================================================

BinarySearchTree::BinarySearchTree(
    const BinarySearchTree &other)
{
    if (other._root)
    {
        _root = new Node(*other._root);
    }

    _size = other._size;
}

BinarySearchTree &
BinarySearchTree::operator=(
    const BinarySearchTree &other)
{
    if (this == &other)
    {
        return *this;
    }

    deleteSubtree(_root);

    _root = nullptr;

    if (other._root)
    {
        _root = new Node(*other._root);
    }

    _size = other._size;

    return *this;
}

BinarySearchTree::BinarySearchTree(
    BinarySearchTree &&other) noexcept
{
    _root = other._root;
    _size = other._size;

    other._root = nullptr;
    other._size = 0;
}

BinarySearchTree &
BinarySearchTree::operator=(
    BinarySearchTree &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    deleteSubtree(_root);

    _root = other._root;
    _size = other._size;

    other._root = nullptr;
    other._size = 0;

    return *this;
}

BinarySearchTree::~BinarySearchTree()
{
    deleteSubtree(_root);
}

void BinarySearchTree::insert(
    const Key &key,
    const Value &value)
{
    if (!_root)
    {
        _root = new Node(
            key,
            value);
    }
    else
    {
        _root->insert(
            key,
            value);
    }

    ++_size;
}


BinarySearchTree::Iterator
BinarySearchTree::find(
    const Key &key)
{
    Node *node = _root;

    while (node)
    {
        if (key <
            node->keyValuePair.first)
        {
            node = node->left;
        }
        else if (
            key >
            node->keyValuePair.first)
        {
            node = node->right;
        }
        else
        {
            return Iterator(
                node,
                _root);
        }
    }

    return end();
}

BinarySearchTree::ConstIterator
BinarySearchTree::find(
    const Key &key) const
{
    const Node *node = _root;

    while (node)
    {
        if (key <
            node->keyValuePair.first)
        {
            node = node->left;
        }
        else if (
            key >
            node->keyValuePair.first)
        {
            node = node->right;
        }
        else
        {
            return ConstIterator(
                node,
                _root);
        }
    }

    return cend();
}

std::pair<
    BinarySearchTree::Iterator,
    BinarySearchTree::Iterator>
    BinarySearchTree::equalRange(
        const Key& key)
{
    Iterator first = find(key);

    if (first == end())
    {
        return { end(), end() };
    }

    Iterator second = first;

    while (second != end())
    {
        if ((*second).first != key)
        {
            break;
        }

        ++second;
    }

    return { first, second };
}

std::pair<
    BinarySearchTree::ConstIterator,
    BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(
    const Key &key) const
{
    ConstIterator first = find(key);

    if (first == cend())
    {
        return {cend(), cend()};
    }

    ConstIterator second = first;

    while (second != cend())
    {
        if ((*second).first != key)
        {
            break;
        }

        ++second;
    }

    return {first, second};
}

BinarySearchTree::ConstIterator
BinarySearchTree::min() const
{
    return ConstIterator(
        minNode(_root),
        _root);
}

BinarySearchTree::ConstIterator
BinarySearchTree::max() const
{
    return ConstIterator(
        maxNode(_root),
        _root);
}

BinarySearchTree::ConstIterator
BinarySearchTree::min(
    const Key &key) const
{
    return equalRange(key).first;
}

BinarySearchTree::ConstIterator
BinarySearchTree::max(
    const Key &key) const
{
    auto range =
        equalRange(key);

    if (range.first ==
        range.second)
    {
        return cend();
    }

    auto it = range.first;
    auto last = it;

    while (it != range.second)
    {
        last = it;
        ++it;
    }

    return last;
}

BinarySearchTree::Iterator
BinarySearchTree::begin()
{
    return Iterator(
        minNode(_root),
        _root);
}

BinarySearchTree::Iterator
BinarySearchTree::end()
{
    return Iterator(
        nullptr,
        _root);
}

BinarySearchTree::ConstIterator
BinarySearchTree::cbegin() const
{
    return ConstIterator(
        minNode(_root),
        _root);
}

BinarySearchTree::ConstIterator
BinarySearchTree::cend() const
{
    return ConstIterator(
        nullptr,
        _root);
}

size_t BinarySearchTree::size() const
{
    return _size;
}

void BinarySearchTree::output_tree()
{
    if (_root)
    {
        _root->output_node_tree();
    }
}

// ======================================================
// Iterator
// ======================================================

BinarySearchTree::Iterator::Iterator(
    Node *node,
    Node *root)
    : _node(node),
      _root(root)
{
}

std::pair<Key, Value> &
BinarySearchTree::Iterator::operator*()
{
    return _node->keyValuePair;
}

const std::pair<Key, Value> &
BinarySearchTree::Iterator::operator*() const
{
    return _node->keyValuePair;
}

std::pair<Key, Value> *
BinarySearchTree::Iterator::operator->()
{
    return &_node->keyValuePair;
}

const std::pair<Key, Value> *
BinarySearchTree::Iterator::operator->() const
{
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator
BinarySearchTree::Iterator::operator++()
{
    // end()
    if (_node == nullptr)
    {
        return *this;
    }

    // есть правое поддерево
    if (_node->right)
    {
        _node = _node->right;

        while (_node->left)
        {
            _node = _node->left;
        }

        return *this;
    }

    // идём вверх
    Node* parent = _node->parent;

    while (
        parent &&
        _node == parent->right)
    {
        _node = parent;
        parent = parent->parent;
    }

    _node = parent;

    return *this;
}

BinarySearchTree::Iterator
BinarySearchTree::Iterator::operator++(int)
{
    Iterator temp(*this);

    ++(*this);

    return temp;
}

BinarySearchTree::Iterator
BinarySearchTree::Iterator::operator--()
{
    if (!_node)
    {
        _node = _root;

        if (_node)
        {
            while (_node->right)
            {
                _node = _node->right;
            }
        }

        return *this;
    }

    if (_node->left)
    {
        _node = _node->left;

        while (_node->right)
        {
            _node = _node->right;
        }

        return *this;
    }

    Node *p = _node->parent;

    while (
        p &&
        _node == p->left)
    {
        _node = p;
        p = p->parent;
    }

    _node = p;

    return *this;
}

BinarySearchTree::Iterator
BinarySearchTree::Iterator::operator--(int)
{
    Iterator temp(*this);

    --(*this);

    return temp;
}

bool BinarySearchTree::Iterator::operator==(
    const Iterator &other) const
{
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(
    const Iterator &other) const
{
    return _node != other._node;
}

// ======================================================
// ConstIterator
// ======================================================

BinarySearchTree::ConstIterator::ConstIterator(
    const Node *node,
    const Node *root)
    : _node(node),
      _root(root)
{
}

const std::pair<Key, Value> &
BinarySearchTree::ConstIterator::operator*() const
{
    return _node->keyValuePair;
}

const std::pair<Key, Value> *
BinarySearchTree::ConstIterator::operator->() const
{
    return &_node->keyValuePair;
}
BinarySearchTree::ConstIterator
BinarySearchTree::ConstIterator::operator++()
{
    if (_node == nullptr)
    {
        return *this;
    }

    if (_node->right)
    {
        _node = _node->right;

        while (_node->left)
        {
            _node = _node->left;
        }

        return *this;
    }

    const Node* parent = _node->parent;

    while (
        parent &&
        _node == parent->right)
    {
        _node = parent;
        parent = parent->parent;
    }

    _node = parent;

    return *this;
}

BinarySearchTree::ConstIterator
BinarySearchTree::ConstIterator::operator++(int)
{
    ConstIterator temp(*this);

    ++(*this);

    return temp;
}

BinarySearchTree::ConstIterator
BinarySearchTree::ConstIterator::operator--()
{
    if (!_node)
    {
        _node = _root;

        if (_node)
        {
            while (_node->right)
            {
                _node = _node->right;
            }
        }

        return *this;
    }

    if (_node->left)
    {
        _node = _node->left;

        while (_node->right)
        {
            _node = _node->right;
        }

        return *this;
    }

    const Node *p =
        _node->parent;

    while (
        p &&
        _node == p->left)
    {
        _node = p;
        p = p->parent;
    }

    _node = p;

    return *this;
}

BinarySearchTree::ConstIterator
BinarySearchTree::ConstIterator::operator--(int)
{
    ConstIterator temp(*this);

    --(*this);

    return temp;
}

bool BinarySearchTree::ConstIterator::operator==(
    const ConstIterator &other) const
{
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(
    const ConstIterator &other) const
{
    return _node != other._node;
}
