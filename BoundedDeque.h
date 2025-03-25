#include <iostream>

#ifndef SYNTAX_ANALYZER_BOUNDEDDEQUE_H
#define SYNTAX_ANALYZER_BOUNDEDDEQUE_H


template<typename T, size_t Capacity = 10>
class BoundedDeque{
private:
    T data[Capacity];
    size_t frontIndex = 0;
    size_t backIndex = 0;
    size_t count = 0;
};

#endif
