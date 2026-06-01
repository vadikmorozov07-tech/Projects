#include "HashTable.h"
#include <cstdint>
#include <stdexcept>

HashTable::HashTable(size_t size) noexcept : _capacity(static_cast<int32_t>(size)),_filled(0),table(size) {}

HashTable::~HashTable() = default;

size_t HashTable::hash_function(const KeyType& key) const
{
    constexpr uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL; // стартовое значение хеша
    constexpr uint64_t FNV_PRIME = 1099511628211ULL; // специальное простое число

    uint64_t hash = FNV_OFFSET_BASIS; // текущее состояние хеша

    for (unsigned char c : key) // прохидимся по каждому символу
    {
        hash ^= c; // XOR
        hash *= FNV_PRIME; // умножение
    }

    return static_cast<size_t>(hash % _capacity); // намер корзины
}

double HashTable::getLoadFactor()
{
    if (_capacity == 0)
    {
        return 0.0;
    }

    return static_cast<double>(_filled) / _capacity;
}

void HashTable::insert(const KeyType& key, const ValueType& value)
{
    size_t index = hash_function(key);

    // Если ключ уже существует — обновляем значение
    for (auto& element : table[index])
    {
        if (element.first == key)
        {
            element.second = value;
            return;
        }
    }

    // Добавляем новый элемент
    table[index].push_back({ key, value });
    ++_filled;

    // Увеличиваем размер таблицы при LoadFactor > 0.75
    if (getLoadFactor() > 0.75)
    {
        int32_t old_capacity = _capacity;
        auto old_table = table;

        _capacity *= 2;

        table.clear();
        table.resize(_capacity);

        _filled = 0;

        // Перехеширование всех элементов
        for (int32_t i = 0; i < old_capacity; ++i)
        {
            for (const auto& element : old_table[i])
            {
                insert(element.first, element.second);
            }
        }
    }
}

bool HashTable::find(const KeyType& key, ValueType& value) const
{
    size_t index = hash_function(key);

    for (const auto& element : table[index])
    {
        if (element.first == key)
        {
            value = element.second;
            return true;
        }
    }

    return false;
}

void HashTable::remove(const KeyType& key)
{
    size_t index = hash_function(key);

    auto& bucket = table[index];

    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
        if (it->first == key)
        {
            bucket.erase(it);
            --_filled;
            return;
        }
    }
}

ValueType& HashTable::operator[](const KeyType& key)
{
    size_t index = hash_function(key);

    // Ищем существующий элемент
    for (auto& element : table[index])
    {
        if (element.first == key)
        {
            return element.second;
        }
    }

    // Если элемента нет — создаём его
    insert(key, ValueType{});

    index = hash_function(key);

    for (auto& element : table[index])
    {
        if (element.first == key)
        {
            return element.second;
        }
    }

    throw std::runtime_error("HashTable::operator[] failed");
}
