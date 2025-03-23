#include "complexNumber.h"

ComplexNumber::ComplexNumber(int real, int imag) : real_(real), imag_(imag) {}

ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) {
    return ComplexNumber(real_ + other.real_, imag_ + other.imag_);
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber& other) {
    return ComplexNumber(real_ - other.real_, imag_ - other.imag_);
}

ComplexNumber ComplexNumber::operator*(int n) {
    return ComplexNumber(real_ * n, imag_ * n);
}

int ComplexNumber::abs() {
    return std::sqrt(real_ * real_ + imag_ * imag_);
}

std::ostream& operator<<(std::ostream& os, const ComplexNumber& other) {
    os << "(" << other.real_ << " + " << other.imag_ << "i)";
    return os;
}
