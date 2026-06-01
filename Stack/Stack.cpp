#include "Stack.h"
#include "StackImplementation.h"
#include "Vector.h"
#include <stdexcept>

class VectorStackImplementation : public IStackImplementation {
public:
    Vector data;
    void push(const ValueType& value) override { data.pushBack(value); }
    void pop() override {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        data.popBack();
    }
    const ValueType& top() const override {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        return data[data.size() - 1];
    }
    bool isEmpty() const override { return data.size() == 0; }
    size_t size() const override { return data.size(); }
};

static IStackImplementation* createImplementation(StackContainer container) {
    return new VectorStackImplementation();
}

// Конструкторы
Stack::Stack(StackContainer container)
    : _containerType(container), _pimpl(createImplementation(container)) {
}

Stack::Stack(const ValueType* valueArray, const size_t arraySize, StackContainer container)
    : _containerType(container), _pimpl(createImplementation(container)) {
    for (size_t i = 0; i < arraySize; ++i) {
        _pimpl->push(valueArray[i]);
    }
}

// Деструктор
Stack::~Stack() { delete _pimpl; }

// Глубокое копирование через static_cast
Stack::Stack(const Stack& copyStack)
    : _containerType(copyStack._containerType), _pimpl(createImplementation(copyStack._containerType)) {
    auto* src = static_cast<VectorStackImplementation*>(copyStack._pimpl);
    auto* dst = static_cast<VectorStackImplementation*>(this->_pimpl);
    dst->data = src->data; 
}

Stack& Stack::operator=(const Stack& copyStack) {
    if (this != &copyStack) {
        auto* src = static_cast<VectorStackImplementation*>(copyStack._pimpl);
        auto* dst = static_cast<VectorStackImplementation*>(this->_pimpl);
        dst->data = src->data;
        _containerType = copyStack._containerType;
    }
    return *this;
}

// Перемещение
Stack::Stack(Stack&& moveStack) noexcept
    : _pimpl(moveStack._pimpl), _containerType(moveStack._containerType) {
    moveStack._pimpl = nullptr;
}

Stack& Stack::operator=(Stack&& moveStack) noexcept {
    if (this != &moveStack) {
        delete _pimpl;
        _pimpl = moveStack._pimpl;
        _containerType = moveStack._containerType;
        moveStack._pimpl = nullptr;
    }
    return *this;
}

void Stack::push(const ValueType& value) { _pimpl->push(value); }
void Stack::pop() { _pimpl->pop(); }
const ValueType& Stack::top() const { return _pimpl->top(); }
bool Stack::isEmpty() const { return _pimpl->isEmpty(); }
size_t Stack::size() const { return _pimpl->size(); }
