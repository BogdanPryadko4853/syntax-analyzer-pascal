#ifndef BOUNDEDDEQUE_H
#define BOUNDEDDEQUE_H

#include <stdexcept>
#include <algorithm>

template <typename T, size_t Capacity = 10>
class BoundedDeque {
private:
    T data[Capacity];
    size_t frontIndex = 0;
    size_t backIndex = 0;
    size_t count = 0;

    size_t nextIndex(size_t index) const {
        return (index + 1) % Capacity;
    }

    size_t prevIndex(size_t index) const {
        return (index - 1 + Capacity) % Capacity;
    }

public:
    BoundedDeque() = default;

    bool empty() const { return count == 0; }
    bool full() const { return count == Capacity; }
    size_t size() const { return count; }
    size_t capacity() const { return Capacity; }

    void push_front(const T& value) {
        if (full()) {
            throw std::out_of_range("Deque is full");
        }

        frontIndex = prevIndex(frontIndex);
        data[frontIndex] = value;
        ++count;
    }

    void push_back(const T& value) {
        if (full()) {
            throw std::out_of_range("Deque is full");
        }

        data[backIndex] = value;
        backIndex = nextIndex(backIndex);
        ++count;
    }

    T pop_front() {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }

        T value = data[frontIndex];
        frontIndex = nextIndex(frontIndex);
        --count;
        return value;
    }

    T pop_back() {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }

        backIndex = prevIndex(backIndex);
        T value = data[backIndex];
        --count;
        return value;
    }

    T& front() {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        return data[frontIndex];
    }

    const T& front() const {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        return data[frontIndex];
    }

    T& back() {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        return data[prevIndex(backIndex)];
    }

    const T& back() const {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        return data[prevIndex(backIndex)];
    }

    void clear() {
        frontIndex = backIndex = count = 0;
    }
};

#endif