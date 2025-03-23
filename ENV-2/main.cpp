#include <iostream>
#include "complexNumber.h"
#include "sort.h"

int main() {
    const int size = 3;
    ComplexNumber array[size] = {
        ComplexNumber(0, 0),
        ComplexNumber(2, 4),
        ComplexNumber(0, 1)
    };

    for (int i = 0; i < size; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    bubble(array, size);

    for (int i = 0; i < size; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

