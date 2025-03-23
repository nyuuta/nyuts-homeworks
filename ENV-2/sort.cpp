#include "sort.h"

void bubble(ComplexNumber arr[], int size) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j].abs() > arr[j + 1].abs()) {
                ComplexNumber temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

