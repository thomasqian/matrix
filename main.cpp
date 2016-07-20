#include <iostream>
#include <conio.h>
#include "Matrix.h"

using namespace mtx;
typedef Matrix<double> Mat;

int main(int argc, char** argv) {

	Mat z(10, 10, 1);
	for (int i = 0; i < 100; ++i) z.at(i) = i % 10;
	z.print(1);

	Mat filter(3,3,0);
	filter.at(0, 1) = -1;
	filter.at(2, 1) = -1;
	filter.at(1, 0) = -1;
	filter.at(1, 2) = -1;
	filter.at(1, 1) = 5;
	filter.print(1);

	z.convolve(filter, 2).print();

	Mat m(4, 4, 1);
	Mat mm(4, 4, 2);
	Mat c(2, 3, 1);
	Mat d(3, 2, 2);
	Mat x;
	Mat a(4,4);
	Mat e(16,1);
	Mat f(3, 1);

	for (int i = 0; i < 16; ++i) {
		a.at(i / 4, i % 4) = i;
		e.at(i) = i;
	}

	/*for (int i = 0; i < 6; ++i) {
		c.at(i) = 1 + i;
	}

	f.at(0) = 9;
	f.at(1) = 8;
	f.at(2) = 7;*/

	//e.print();
	/*a.print();
	std::cout << a.sumAll() << std::endl;
	a.submat(1, 1, 3, 3).print();*/

	//a.row(1).print();
	//a.col(3).print();
	//std::cout << e.at(-1) << std::endl;

	/*
	m.at(4, 5) = 5;

	m.at(1, 1) = 4.0;	
	std::cout << "Matrix 1:\n";
	m.print();

	std::cout << "Matrix 2:\n";
	mm.print();

	std::cout << "Matrix 1 + 2:\n";
	x = m + mm;
	x.print();

	std::cout << "Matrix 1 * 2:\n";
	x = x * mm;
	x.print();
	*/
	/*
	std::cout << "Matrix 1:\n";
	c.print();

	std::cout << "Matrix 2:\n";
	f.print();

	std::cout << "Matrix 1 x 2:\n";
	x = c.mul(f);
	x.print();*/

	_getch();
	return 0;
}