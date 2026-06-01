#pragma once

#include <utility>
#include <cstdint>
#include <string>

using Key = uint32_t;
using Value = double;

class BinarySearchTree
{
    struct Node
    {
        Node(Key key,
            Value value,
            Node* parent = nullptr,
            Node* left = nullptr,
            Node* right = nullptr);

        Node(const Node& other);

        bool operator==(const Node& other) const;

        void output_node_tree() const;

        void insert(const Key& key,
            const Value& value);

        void erase(const Key& key);

        std::pair<Key, Value> keyValuePair;

        Node* parent = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;
    };

public:
    BinarySearchTree() = default;

    explicit BinarySearchTree(
        const BinarySearchTree& other);

    BinarySearchTree& operator=(
        const BinarySearchTree& other);

    explicit BinarySearchTree(
        BinarySearchTree&& other) noexcept;

    BinarySearchTree& operator=(
        BinarySearchTree&& other) noexcept;

    ~BinarySearchTree();

    class Iterator
    {
    public:
        explicit Iterator(Node* node = nullptr,
            Node* root = nullptr);

        std::pair<Key, Value>& operator*();

        const std::pair<Key, Value>& operator*() const;

        std::pair<Key, Value>* operator->();

        const std::pair<Key, Value>* operator->() const;

        Iterator operator++();

        Iterator operator++(int);

        Iterator operator--();

        Iterator operator--(int);

        bool operator==(const Iterator& other) const;

        bool operator!=(const Iterator& other) const;

    private:
        Node* _node;
        Node* _root;
    };

    class ConstIterator
    {
    public:
        explicit ConstIterator(const Node* node = nullptr,
            const Node* root = nullptr);

        const std::pair<Key, Value>& operator*() const;

        const std::pair<Key, Value>* operator->() const;

        ConstIterator operator++();

        ConstIterator operator++(int);

        ConstIterator operator--();

        ConstIterator operator--(int);

        bool operator==(const ConstIterator& other) const;

        bool operator!=(const ConstIterator& other) const;

    private:
        const Node* _node;
        const Node* _root;
    };

    void insert(const Key& key,
        const Value& value);

    void erase(const Key& key);

    ConstIterator find(const Key& key) const;

    Iterator find(const Key& key);

    std::pair<Iterator, Iterator>
        equalRange(const Key& key);

    std::pair<ConstIterator, ConstIterator>
        equalRange(const Key& key) const;

    ConstIterator min() const;

    ConstIterator max() const;

    ConstIterator min(const Key& key) const;

    ConstIterator max(const Key& key) const;

    Iterator begin();

    Iterator end();

    ConstIterator cbegin() const;

    ConstIterator cend() const;

    size_t size() const;

    void output_tree();

private:
    // helper methods

    static Node* minNode(Node* node);

    static Node* maxNode(Node* node);

    const Node* minNode(const Node* node) const;

    const Node* maxNode(const Node* node) const;

    static void deleteSubtree(Node* node);

private:
    size_t _size = 0;

    Node* _root = nullptr;
};

