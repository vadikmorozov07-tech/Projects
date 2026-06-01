#include "Vector.h"
#include <algorithm>
#include <stdexcept>

// конструкторы

Vector::Vector(const ValueType* rawArray, const size_t size, float coef)
    : _size(size), _capacity(size), _multiplicativeCoef(coef)
{
    if (size > 0)
    {
        _data = new ValueType[_capacity];
        std::copy(rawArray, rawArray + size, _data);
    }
}

Vector::Vector(const Vector& other)
    : _size(other._size), _capacity(other._size), _multiplicativeCoef(other._multiplicativeCoef)
{
    if (_size > 0)
    {
        _data = new ValueType[_capacity];
        std::copy(other._data, other._data + _size, _data);
    }
}

Vector& Vector::operator=(const Vector& other)
{
    if (this != &other)
    {
        delete[] _data;

        _size = other._size;
        _capacity = other._size;
        _multiplicativeCoef = other._multiplicativeCoef;

        if (_size > 0)
        {
            _data = new ValueType[_capacity];
            std::copy(other._data, other._data + _size, _data);
        }
        else
        {
            _data = nullptr;
        }
    }
    return *this;
}

Vector::Vector(Vector&& other) noexcept
    : _data(other._data), _size(other._size), _capacity(other._capacity),
    _multiplicativeCoef(other._multiplicativeCoef)
{
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}

Vector& Vector::operator=(Vector&& other) noexcept
{
    if (this != &other)
    {
        delete[] _data;

        _data = other._data;
        _size = other._size;
        _capacity = other._capacity;
        _multiplicativeCoef = other._multiplicativeCoef;

        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }
    return *this;
}

Vector::~Vector()
{
    delete[] _data;
}

// функции

void Vector::pushBack(const ValueType& value)
{
    insert(value, _size);
}

void Vector::pushFront(const ValueType& value)
{
    insert(value, 0);
}

void Vector::insert(const ValueType& value, size_t pos)
{
    if (pos > _size)
        throw std::out_of_range("Index out of range");

    if (_size == _capacity)
    {
        size_t newCapacity = (_capacity == 0) ? 1 : static_cast<size_t>(_capacity * _multiplicativeCoef);
        reserve(newCapacity);
    }

    std::move_backward(_data + pos, _data + _size, _data + _size + 1);
    _data[pos] = value;
    ++_size;
}

void Vector::insert(const ValueType* values, size_t size, size_t pos)
{
    if (pos > _size)
        throw std::out_of_range("Index out of range");

    if (size == 0)
        return;

    if (_size + size > _capacity)
    {
        size_t newCapacity = (_capacity == 0) ? size : _capacity;
        while (newCapacity < _size + size)
            newCapacity = static_cast<size_t>(newCapacity * _multiplicativeCoef);
        reserve(newCapacity);
    }

    std::move_backward(_data + pos, _data + _size, _data + _size + size);
    std::copy(values, values + size, _data + pos);
    _size += size;
}

void Vector::insert(const Vector& vector, size_t pos)
{
    if (vector._size == 0)
        return;
    insert(vector._data, vector._size, pos);
}

void Vector::popBack()
{
    if (_size == 0)
        throw std::out_of_range("Vector is empty");
    --_size;
}

void Vector::popFront()
{
    erase(0, 1);
}

void Vector::erase(size_t pos, size_t count)
{
    if (pos >= _size)
        throw std::out_of_range("Index out of range");

    if (pos + count > _size)
        count = _size - pos;

    std::move(_data + pos + count, _data + _size, _data + pos);
    _size -= count;
}

void Vector::eraseBetween(size_t beginPos, size_t endPos)
{
    if (beginPos >= _size)
        throw std::out_of_range("Index out of range");

    if (endPos > _size)
        endPos = _size;

    if (endPos <= beginPos)
        return;

    erase(beginPos, endPos - beginPos);
}

size_t Vector::size() const
{
    return _size;
}

size_t Vector::capacity() const
{
    return _capacity;
}

double Vector::loadFactor() const
{
    return (_capacity == 0) ? 0.0 : static_cast<double>(_size) / _capacity;
}

ValueType& Vector::operator[](size_t idx)
{
    return _data[idx];
}

const ValueType& Vector::operator[](size_t idx) const
{
    return _data[idx];
}

long long Vector::find(const ValueType& value) const
{
    for (size_t i = 0; i < _size; ++i)
    {
        if (_data[i] == value)
            return static_cast<long long>(i);
    }
    return -1;
}

void Vector::reserve(size_t newCapacity)
{
    if (newCapacity <= _capacity)
        return;

    ValueType* newData = new ValueType[newCapacity];
    std::move(_data, _data + _size, newData);
    delete[] _data;

    _data = newData;
    _capacity = newCapacity;
}

void Vector::shrinkToFit()
{
    if (_capacity > _size)
    {
        if (_size == 0)
        {
            delete[] _data;
            _data = nullptr;
            _capacity = 0;
        }
        else
        {
            ValueType* newData = new ValueType[_size];
            std::move(_data, _data + _size, newData);
            delete[] _data;

            _data = newData;
            _capacity = _size;
        }
    }
}

// =итераторы

Vector::Iterator::Iterator(ValueType* ptr)
    : _ptr(ptr)
{
}

ValueType& Vector::Iterator::operator*()
{
    return *_ptr;
}

const ValueType& Vector::Iterator::operator*() const
{
    return *_ptr;
}

ValueType* Vector::Iterator::operator->()
{
    return _ptr;
}

const ValueType* Vector::Iterator::operator->() const
{
    return _ptr;
}

Vector::Iterator Vector::Iterator::operator++()
{
    ++_ptr;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int)
{
    Iterator temp = *this;
    ++_ptr;
    return temp;
}

bool Vector::Iterator::operator==(const Iterator& other) const
{
    return _ptr == other._ptr;
}

bool Vector::Iterator::operator!=(const Iterator& other) const
{
    return _ptr != other._ptr;
}


Vector::Iterator Vector::begin()
{
    return Iterator(_data);
}

Vector::Iterator Vector::end()
{
    return Iterator(_data + _size);
}
