#ifndef _MAT_TEST_H_
#define _MAT_TEST_H_

#include <iostream>
#include <string>
#include "Matrix.h"

using namespace mtx;
typedef Matrix<double> Mat;

namespace tester {

static int num_tests;
static int num_fails;

static void fail(std::string test_case, std::string msg) {
	std::cerr << "Test case for (" << test_case << ") failed: " << msg << "\n";
	++num_fails;
}

static void test_ctor_1() {
	Mat m;

	if (m.get_width() != 0) fail("default ctor", "matrix width should be 0");
	if (m.get_height() != 0) fail("default ctor", "matrix height should be 0");
	if (m.get_data() != 0) fail("default ctor", "matrix data should be null");

	num_tests += 3;
}

static void test_ctor_2() {
	Mat m1(1,1);
	Mat m2(3,3,9);

	if (m1.get_width() != 1) fail("normal ctor", "matrix width should be 1");
	if (m1.get_height() != 1) fail("normal ctor", "matrix height should be 1");
	if (m1.get_data()[0] != 0) fail("normal ctor", "matrix data should be 0");

	if (m2.get_width() != 3) fail("normal ctor", "matrix width should be 3");
	if (m2.get_height() != 3) fail("normal ctor", "matrix height should be 3");
	if (m2.get_data()[0] != 9) fail("normal ctor", "matrix data should be 9");
	if (m2.get_data()[8] != 9) fail("normal ctor", "matrix data should be 9");

	num_tests += 7;
}

static void test_ctor_3() {
	Mat m(4,4,5);
	Mat mm(m);

	if (mm.get_width() != m.get_width()) fail("copy ctor", "matrix widths should be equal");
	if (mm.get_height() != m.get_height()) fail("copy ctor", "matrix heights should be equal");
	if (mm.get_data()[0] != m.get_data()[0]) fail("copy ctor", "matrix data should be equal");
	if (mm.get_data()[8] != m.get_data()[8]) fail("copy ctor", "matrix data should be equal");

	num_tests += 4;
}

static void test_at_1() {
	Mat m(3,3);
	for (int i = 0; i < 9; ++i) m.get_data()[i] = i;
	
	if (m.at(0) != 0) fail("at 1-arg", "reading data using at failed");
	if (m.at(4) != 4) fail("at 1-arg", "reading data using at failed");

	m.at(8) = 99;

	if (m.get_data()[8] != 99) fail("at 1-arg", "setting data using at failed");

	num_tests += 3;
}

static void test_at_2() {
	Mat m(3,3);
	for (int i = 0; i < 9; ++i) m.get_data()[i] = i;
	
	if (m.at(0,1) != 1) fail("at 2-arg", "reading data using at failed");
	if (m.at(2,2) != 8) fail("at 2-arg", "reading data using at failed");

	m.at(1,1) = 99;

	if (m.get_data()[4] != 99) fail("at 2-arg", "setting data using at failed");

	num_tests += 3;
}

static void test_row() {
	Mat m(3,3);
	Mat n(3,1);
	for (int i = 0; i < 9; ++i) m.at(i) = i;
	for (int i = 0; i < 3; ++i) n.at(i) = i + 3;

	if (m.row(1) != n) fail("row", "row returned incorrect matrix");

	num_tests += 1;
}

static void test_col() {
	Mat m(3,3);
	Mat n(1,3);
	for (int i = 0; i < 9; ++i) m.at(i) = i;
	for (int i = 0; i < 3; ++i) n.at(i) = i * 3;

	if (m.col(0) != n) fail("col", "col returned incorrect matrix");

	num_tests += 1;
}

static void test_zero_pad() {
	Mat m(3,3,9);
	Mat z = m.zero_pad(2);

	if (z.get_width() != m.get_width() + 4) fail("zero_pad", "incorrect width after padding");
	if (z.get_height() != m.get_height() + 4) fail("zero_pad", "incorrect height after padding");
	
	bool pass = true;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (z.at(i + 2, j + 2) != 9) {
				fail("zero_pad", "original matrix data altered");
				pass = false;
				break;
			}
		}
		if (!pass) break;
	}

	num_tests += 3;
}

static void test_submat() {
	Mat m(5,5);
	for (int i = 0; i < 25; ++i) m.at(i) = i;

	Mat m1(2,2);
	m1.at(0,0) = 0;
	m1.at(0,1) = 1;
	m1.at(1,0) = 5;
	m1.at(1,1) = 6;

	Mat x = m.submat(0,0,2,2);
	if (x.get_width() != 2) fail("submat", "submatrix width incorrect");
	if (x.get_height() != 2) fail("submat", "submatrix height incorrect");
	if (x != m1) fail("submat", "submatrix data incorrect");

	Mat m2(3,3);
	m2.at(0,0) = 12;
	m2.at(0,1) = 13;
	m2.at(0,2) = 14;
	m2.at(1,0) = 17;
	m2.at(1,1) = 18;
	m2.at(1,2) = 19;
	m2.at(2,0) = 22;
	m2.at(2,1) = 23;
	m2.at(2,2) = 24;

	x = m.submat(2,2,3,3);
	if (x.get_width() != 3) fail("submat", "submatrix width incorrect");
	if (x.get_height() != 3) fail("submat", "submatrix height incorrect");
	if (x != m2) fail("submat", "submatrix data incorrect");

	num_tests += 6;
}

static void test_mat_multiply() {

}

static void test_convolve() {

}

static void test_sum_all() {

}

static void run_all_tests() {
	// before tests
	num_tests = 0;
	num_fails = 0;

	// run tests
	test_ctor_1();
	test_ctor_2();
	test_ctor_3();
	test_at_1();
	test_at_2();
	test_row();
	test_col();
	test_zero_pad();
	test_submat();

	// after tests
	printf("%d tests run.\n", num_tests);

	if (num_fails == 0) {
		std::cout << "All tests passed!\n";
	} else {
		std::cout << num_fails << " tests failed.\n";
	}
}

}

#endif