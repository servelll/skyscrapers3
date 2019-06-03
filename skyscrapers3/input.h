#include <iostream>
#include <vector>

class point {
public:
	int x;
	int y;
	int value;
	point(int _x, int _y, int _value) {
		x = _x;
		y = _y;
		value = _value;
	};
	~point() {}
};

class input {
public:
	int size;
	int* left_col;
	int* right_col;
	int* top_row;
	int* bottom_row;

	vector <point> started_cells;

	input() {};
	input(int _s, int* a, int* b, int* c, int* d) {
		size = _s;
		//выделение памяти
		left_col = new int[size];
		right_col = new int[size];
		top_row = new int[size];
		bottom_row = new int[size];
		//заполнение этой выделенной памяти
		for (int i = 0; i < size; i++) {
			left_col[i] = a[i];
			right_col[i] = b[i];
			top_row[i] = c[i];
			bottom_row[i] = d[i];
		}
	};
	~input() {
		//очистка памяти
		delete[]left_col;
		delete[]right_col;
		delete[]top_row;
		delete[]bottom_row;
	};

	void add_started_cell(int _i, int _j, int _value) {
		started_cells.push_back(*new point( _i, _j, _value));
	}
};