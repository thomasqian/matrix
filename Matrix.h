#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <iostream>
#include <string>
#include <conio.h>

namespace mtx {

void error(std::string type, std::string msg) {
	std::cerr << "Error (" << type << "): " << msg << std::endl;
	_getch();
	exit(EXIT_FAILURE);
}

template <class T>
class Matrix {
public:
	Matrix() : width(0), height(0), data(0) {}

	Matrix(int width, int height, T val = 0) : width(width), height(height) {
		if (width <= 0 || height <= 0) {
			error("ctor", "width and height must be nonzero positives");
		}
		data = new T[width * height];
		for (int i = 0; i < width * height; ++i) data[i] = val;
	}

	Matrix(Matrix<T>& mat) : width(mat.width), height(mat.height) {
		data = new T[width * height];
		for (int i = 0; i < width * height; ++i) {
			data[i] = mat.at(i / width, i % width);
		}
	}

	~Matrix() {
		if (data) delete[] data;
	}

	int get_height() { return height; }
	int get_width() { return width; }
	T* get_data() { return data; }

	T& at(int e) {
		if (!data) {
			error("at", "data is null");
		} else if (e < 0 || e >= height * width) {
			error("at", "index out of bounds");
		}

		return data[e];
	}

	T& at(int row, int col) {
		return this->at(row * width + col);
	}

	Matrix<T> row(int r) {
		if (r < 0 || r >= height) {
			error("row", "index out of bounds");
		}

		Matrix<T> ret(1, width);
	
		for (int i = 0; i < width; ++i) {
			ret.at(0, i) = this->at(r, i);
		}

		return ret;
	}

	Matrix<T> col(int c) {
		if (c < 0 || c >= width) {
			error("col", "index out of bounds");
		}

		Matrix<T> ret(height, 1);

		for (int i = 0; i < height; ++i) {
			ret.at(i, 0) = this->at(i, c);
		}

		return ret;
	}

	Matrix<T> zeroPad(int pad) {
		Matrix<T> ret(width + 2*pad, height + 2*pad);
	
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				ret.at(pad + i, pad + j) = this->at(i,j);
			}
		}

		return ret;
	}

	Matrix<T> submat(int x, int y, int w, int h) {
		if (x < 0 || x >= width || y < 0 || y >= height) {
			error("submat", "invalid starting coordinate");
		} else if (w < 1 || x + w > width || h < 1 || y + h > height) {
			error("submat", "invalid width or height for given xy");
		}

		Matrix<T> ret(w, h);

		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				ret.at(i,j) = this->at(x + i, y + j);
			}
		}

		return ret;
	}

	// matrix multiply
	Matrix<T> mul(Matrix<T>& o) {
		if (this->width != o.height) {
			error("mul", "matrix width and height must match");
		}

		Matrix<T> ret(this->height, o.width);

		for (int i = 0; i < ret.height; ++i) {
			Matrix<T> r = this->row(i);
			for (int j = 0; j < ret.width; ++j) {
				Matrix<T> c = o.col(j);
				T tot = 0;
				for (int k = 0; k < this->width; ++k) {
					tot += r.at(k) * c.at(k);
				}
				ret.at(i,j) = tot;
			}
		}

		return ret;
	}

	Matrix<T> convolve(Matrix<T> filter, int stride = 1) {
		int fw = filter.getWidth();
		
		if (fw != filter.getHeight()) {
			error("convolve", "only square filters supported");
		} else if (fw % 2 != 1) {
			error("convolve", "only odd filter sizes supported");
		}

		int pad = fw / 2;
		Matrix<T> input = this->zeroPad(pad);
		Matrix<T> ret( ((width-1)/stride) + 1, ((height-1)/stride) + 1 );

		for (int i = 0; i < ret.getHeight(); ++i) {
			for (int j = 0; j < ret.getWidth(); ++j) {
				Matrix<T> isub = input.submat(stride * i, stride * j, fw, fw);
				ret.at(i,j) = (isub * filter).sumAll();
			}
		}

		return ret;
	}

	T sumAll() {
		T sum = 0;
		for (int i = 0; i < width * height; ++i) {
			sum += data[i];
		}
		return sum;
	}

	void print(int t = 0) {
		if (data) {
			for (int i = 0; i < height; ++i) {
				for (int j = 0; j < width; ++j) {
					std::cout << data[i * width + j] << " ";
				}
				printf("\n");
			}
		}
		if (t == 1) printf("\n");
	}

	void operator=(Matrix<T>& o) {
		width = o.width;
		height = o.height;
		if (data) delete[] data;
		data = new T[height * width];

		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				this->at(i,j) = o.at(i,j);
			}
		}
	}

	bool operator==(Matrix<T>& o) {
		if (this->width != o.width || this->height != o.height) return false;
		for (int i = 0; i < this->width * this->height; ++i) {
			if (this->at(i) != o.at(i)) return false;
		}
		return true;
	}

	Matrix<T> operator+(Matrix<T>& o) {
		if (width != o.width || height != o.height) {
			error("add", "matrix dimensions must match");
		}

		Matrix<T> ret(*this);
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				ret.at(i,j) += o.at(i,j);
			}
		}

		return ret;
	}

	Matrix<T> operator*(Matrix<T>& o) {
		if (width != o.width || height != o.height) {
			error("multiply", "matrix dimensions must match");
		}

		Matrix<T> ret(*this);
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				ret.at(i,j) *= o.at(i,j);
			}
		}

		return ret;
	}

	friend std::ostream& operator<<(std::ostream& os, Matrix<T>& mat) {
		mat.print();
		return os;
	 }

private:
	T* data;
	int height, width;
};

}

#endif
