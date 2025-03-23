#ifndef COMPLEXNUMBER_H
#define COMPLEXNUMBER_H
#include <iostream>
#include <cmath>
class ComplexNumber{
	private:
		int real_;
		int imag_;
	public:
		ComplexNumber(int real = 0, int imag = 0);

		ComplexNumber operator+(const ComplexNumber& other);

 		ComplexNumber operator-(const ComplexNumber& other);

   		ComplexNumber operator*(int constant);

		int abs();

		friend std::ostream& operator<<(std::ostream& os, const ComplexNumber& other);



};

#endif
