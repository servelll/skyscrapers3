#include "table.h"
//-> block.h
//-> input.h

input* examples(int _number) {
	switch (_number) {
	case 1: {
		int size = 5;
		//решается 3x Ариадной
		int left_col[] = { 2, 2, 4, 1, 3 };
		int right_col[] = { 2, 2, 2, 4, 1 };
		int top_row[] = { 3, 1, 2, 2, 2 };
		int bottom_row[] = { 2, 3, 4, 2, 1 };
		return new input(size, left_col, right_col, top_row, bottom_row);
	}

	case 2: {
		int size = 5;
		//решается без Ариадны
		int left_col[] = { 1, 2, 2, 3, 3 };
		int right_col[] = { 3, 2, 3, 3, 1 };
		int top_row[] = { 1, 2, 2, 2, 3 };
		int bottom_row[] = { 4, 2, 2, 3, 1 };
		return new input(size, left_col, right_col, top_row, bottom_row);
	}

	case 3: {
		int size = 5;
		//решается без min_stairs и без Ариадны
		int left_col[] = { 4, 3, 2, 1, 2 };
		int right_col[] = { 1, 3, 2, 3, 2 };
		int top_row[] = { 4, 4, 2, 3, 1 };
		int bottom_row[] = { 2, 1, 4, 2, 2 };
		return new input(size, left_col, right_col, top_row, bottom_row);
	}

	case 4: {
		int size = 6;
		//решается полностью реализованной Ариадной
		int left_col[] = { 3, 1, 2, 2, 5, 3 };
		int right_col[] = { 1, 3, 2, 4, 2, 3 };
		int top_row[] = { 2, 2, 3, 3, 3, 1 };
		int bottom_row[] = { 4, 2, 4, 1, 2, 3 };
		return new input(size, left_col, right_col, top_row, bottom_row);
	}

	case 5: {
		int size = 6;
		//решается множественной Ариадной до 2го уровня
		int left_col[] = { 1, 4, 2, 3, 2, 2 };
		int right_col[] = { 5, 1, 4, 2, 2, 2 };
		int top_row[] = { 1, 2, 3, 4, 3, 2 };
		int bottom_row[] = { 2, 4, 1, 2, 3, 3 };
		input* inp_obj = new input(size, left_col, right_col, top_row, bottom_row);
		return inp_obj;
	}

	case 6: {
		int size = 7;
		//
		int left_col[] = { 5, 2, 3, 3, 3, 1, 2 };
		int right_col[] = { 1, 2, 2, 3, 3, 4, 3 };
		int top_row[] = { 4, 3, 2, 4, 2, 2, 1 };
		int bottom_row[] = { 2, 1, 4, 2, 3, 2, 4 };
		input* inp_obj = new input(size, left_col, right_col, top_row, bottom_row);
		return inp_obj;
	}

	case 7: {
		int size = 5;
		//первый пример с неполными данными
		int left_col[] = { 3, 2, 3, 0, 2 };
		int right_col[] = { 1, 3, 0, 3, 2 };
		int top_row[] = { 0, 2, 3, 4, 1 };
		int bottom_row[] = { 2, 0, 3, 0, 0 };
		return new input(size, left_col, right_col, top_row, bottom_row);
	}

	case 8: {
		int size = 7;
		//второй мощный пример с неполными данными и с начальными значениями
		int left_col[] = { 1, 3, 0, 4, 3, 0, 2 };
		int right_col[] = { 3, 2, 3, 2, 2, 1, 3 };
		int top_row[] = { 1, 0, 2, 3, 4, 2, 4 };
		int bottom_row[] = { 3, 0, 0, 4, 2, 3, 2 };
		input* inp_obj = new input(size, left_col, right_col, top_row, bottom_row);
		inp_obj->add_started_cell(0, 1, 1);
		inp_obj->add_started_cell(0, 3, 3);
		inp_obj->add_started_cell(2, 1, 3);
		inp_obj->add_started_cell(2, 3, 7);
		inp_obj->add_started_cell(3, 0, 2);
		inp_obj->add_started_cell(5, 1, 5);
		inp_obj->add_started_cell(5, 5, 4);
		inp_obj->add_started_cell(6, 5, 1);
		return inp_obj;
	}

	}
}

input* generation() {
	int i, j;
	int number = rand() % 3 + 5;
	cout << "n = " << number << endl;
	int* left_col = new int[number];
	int* right_col = new int[number];
	int* top_row = new int[number];
	int* bottom_row = new int[number];

	int* B = new int[number + 1];
	int* C = new int[number + 1];
	int* D = new int[number + 1];

	//выделение памяти
	int** mas = new int*[number + 1];
	for (i = 0; i <= number; i++) {
		mas[i] = new int[number + 1];
	}
	//генерация латинского квадрата
	for (i = 1; i <= number; ++i) {
		B[i] = i;
		C[i] = i;
		D[i] = i;
	}
	
	//формируем «случайный» столбец: 
	int n = number;
	for (int i = 1; i <= number; ++i) {
		int x = rand() % n + 1;
		B[i] = C[x];
		--n;
		for (int k = x; k <= n; ++k) {
			C[k] = C[k + 1];
		}
	}
	
	//формируем «случайный» строку: 
	n = number;
	for (int i = 1; i <= number; ++i) {
		int x = rand() % n + 1;
		C[i] = D[x];
		--n;
		for (int k = x; k <= n; ++k) {
			D[k] = D[k + 1];
		}
	}

	//составляем случайный латинский квадрат:
	for (int j = 1; j <= number; ++j) {
		for (int i = 1; i <= number; ++i) {
			n = B[j] + C[i];
			if (n > number){
				n -= number;
			}
			mas[j][i] = n;
		}
	}

	//подсчет суммы
	for (i = 1; i <= number; ++i) {
		int max = 0, count = 0;
		for (j = 1; j <= number; ++j) {
			if (mas[i][j] > max) {
				max = mas[i][j];
				count++;
			}
		}
		left_col[i-1] = count;

		max = 0, count = 0;
		for (j = number; j >= 1; --j) {
			if (mas[i][j] > max) {
				max = mas[i][j];
				count++;
			}
		}
		right_col[i - 1] = count;

		max = 0, count = 0;
		for (j = 1; j <= number; ++j) {
			if (mas[j][i] > max) {
				max = mas[j][i];
				count++;
			}
		}
		top_row[i - 1] = count;

		max = 0, count = 0;
		for (j = number; j >= 1; --j) {
			if (mas[j][i] > max) {
				max = mas[j][i];
				count++;
			}
		}
		bottom_row[i - 1] = count;
	}
	return new input(number, left_col, right_col, top_row, bottom_row);
}

table* choice_data(int _choice, int _number) {

	switch (_choice) {
	case 1: {
		//авто-генерация
		return new table(generation());
	}
	case 2: {
		//примеры
		return new table(examples(_number));
	}

	//ручное заполнение
	}
}