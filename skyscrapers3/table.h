#include <iostream>
#include <vector>

#include "block.h"
#include "input.h"

using namespace std;
bool g_d = true;

class table {
	int size;
	vector<block> blocks_table;
	int entry_level;
public:
	int **fast_table;
	vector<int> **fast_candidates;
	vector<int> sum_left;
	vector<int> sum_right;
	vector<int> sum_top;
	vector<int> sum_bottom;

	//���������
	table() {}
	table(input* inp_obj) {
		size = inp_obj->size;
		//��������� ������
		fast_table = new int*[size];
		fast_candidates = new vector<int>*[size];
		for (int i = 0; i < size; i++) {
			fast_table[i] = new int[size];
			fast_candidates[i] = new vector<int>[size];
		}
		//���������� ���� ���������� ������ - ����� � ����������
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				fast_table[i][j] = 0;
				for (int k = 0; k < size; k++) {
					fast_candidates[i][j].push_back(k + 1);
				}
			}
		}
		//���������� ����
		for (int i = 0; i < size; i++) {
			sum_left.push_back(inp_obj->left_col[i]);
			sum_right.push_back(inp_obj->right_col[i]);
			sum_top.push_back(inp_obj->top_row[i]);
			sum_bottom.push_back(inp_obj->bottom_row[i]);
		}
		//���������� ��������� �����
		for (int i = 0; i < inp_obj->started_cells.size(); i++) {
			set_cell(inp_obj->started_cells[i].x, inp_obj->started_cells[i].y, inp_obj->started_cells[i].value);
		}
	}
	table(const table& object) {
		size = object.size;

		//��������� ������
		fast_table = new int*[size];
		fast_candidates = new vector<int>*[size];
		for (int i = 0; i < size; i++) {
			fast_table[i] = new int[size];
			fast_candidates[i] = new vector<int>[size];
		}

		//������������
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				fast_table[i][j] = object.fast_table[i][j];
				fast_candidates[i][j] = object.fast_candidates[i][j];
			}
		}
		sum_left = object.sum_left;
		sum_right = object.sum_right;
		sum_top = object.sum_top;
		sum_bottom = object.sum_bottom;

		entry_level = object.entry_level;
	}
	table(table* object) {
		size = object->size;

		//��������� ������
		fast_table = new int*[size];
		fast_candidates = new vector<int>*[size];
		for (int i = 0; i < size; i++) {
			fast_table[i] = new int[size];
			fast_candidates[i] = new vector<int>[size];
		}

		//������������
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				fast_table[i][j] = object->fast_table[i][j];
				fast_candidates[i][j] = object->fast_candidates[i][j];
			}
		}
		sum_left = object->sum_left;
		sum_right = object->sum_right;
		sum_top = object->sum_top;
		sum_bottom = object->sum_bottom;

		entry_level = object->entry_level;
	}
	table& operator=(table &another) //���������� ��������� ������������
	{
		//������� ������
		for (int i = 0; i < size; i++) {
			delete[]fast_table[i];
			delete[]fast_candidates[i];
		}
		size = another.size;

		//��������� ������
		fast_table = new int*[size];
		fast_candidates = new vector<int>*[size];
		for (int i = 0; i < size; i++) {
			fast_table[i] = new int[size];
			fast_candidates[i] = new vector<int>[size];
		}

		//������������
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				fast_table[i][j] = another.fast_table[i][j];
				fast_candidates[i][j] = another.fast_candidates[i][j];
			}
		}
		sum_left = another.sum_left;
		sum_right = another.sum_right;
		sum_top = another.sum_top;
		sum_bottom = another.sum_bottom;

		entry_level = another.entry_level;
		return *this;
	}
	~table() {
		//������� ������
		for (int i = 0; i < size; i++) {
			delete[]fast_table[i];
			delete[]fast_candidates[i];
		}
	}

	//��������
	void set_cell(int i, int j, int v) {
		fast_table[i][j] = v;
		fast_candidates[i][j].clear();
		//�������� ���������� ��� �������� � ������������� �������
		//������� ����� ������
		for (int k = 0; k < size; k++) {
			//������� ���������� ������ ������
			for (int n = 0; n < fast_candidates[k][j].size(); n++) {
				if (fast_candidates[k][j][n] == v) {
					fast_candidates[k][j].erase(fast_candidates[k][j].begin() + n);
				}
			}
			//�������
			for (int n = 0; n < fast_candidates[i][k].size(); n++) {
				if (fast_candidates[i][k][n] == v) {
					fast_candidates[i][k].erase(fast_candidates[i][k].begin() + n);
				}
			}
		}
	}
	void erase_cell_s_cand(int i, int j, int v){
		for (int n = 0; n < fast_candidates[i][j].size(); n++) {
			if (fast_candidates[i][j][n] == v) {
				fast_candidates[i][j].erase(fast_candidates[i][j].begin() + n);
			}
		}
	}
	int calc_real_sum(int k, char* dir) {
		int last = 0, count = 0;
		if (dir == "left") {
			//��� ������, ����� ����� �������
			for (int j = 0; j < size; j++) {
				if (fast_table[k][j] > last) {
					count++; last = fast_table[k][j];
				}
			}
			return count;
		}
		if (dir == "right") {
			//��� ������, ����� ������ ������
			for (int j = size - 1; j > -1; j--) {
				if (fast_table[k][j] > last) {
					count++; last = fast_table[k][j];
				}
			}
			return count;
		}
		if (dir == "top") {
			//��� �������, ����� ������ ����
			for (int i = 0; i < size; i++) {
				if (fast_table[i][k] > last) {
					count++; last = fast_table[i][k];
				}
			}
			return count;
		}
		if (dir == "bottom") {
			//��� �������, ����� ����� �����
			for (int i = size - 1; i > -1; i--) {
				if (fast_table[i][k] > last) {
					count++; last = fast_table[i][k];
				}
			}
			return count;
		}
	}
	int get_value_s_pos(int v, char* d, int n) {
		if (d == "row") {//��� �����
			for (int k = 0; k < size; k++) {
				if (fast_table[n][k] == v) {
					return k;
				}
			}
		}
		if (d == "col") {//��� ��������
			for (int k = 0; k < size; k++) {
				if (fast_table[k][n] == v) {
					return k;
				}
			}
		}
		return -1;
	}
	int calc_all_empty_places(int k, char* d){
		if (d == "row") {
			int empty_places_count = 0;
			for (int j = 0; j < size; j++) {
				if (is_cell_is(k, j, 0)) empty_places_count++;
			}
			return empty_places_count;
		}
		if (d == "col") {
			int empty_places_count = 0;
			for (int i = 0; i < size; i++) {
				if (is_cell_is(i, k, 0)) empty_places_count++;
			}
			return empty_places_count;
		}
	}
	int calc_empty_places_until_noempty(int k, char* d) {
		if (d == "left") {
			int empty_places_count = 0;
			for (int j = 0; j < size; j++) {
				if (is_cell_is(k, j, 0)) empty_places_count++; else return empty_places_count;
			}
		}
		if (d == "right") {
			int empty_places_count = 0;
			for (int j = size - 1; j > -1; j--) {
				if (is_cell_is(k, j, 0)) empty_places_count++; else return empty_places_count;
			}
		}
		if (d == "top") {
			int empty_places_count = 0;
			for (int i = 0; i < size; i++) {
				if (is_cell_is(i, k, 0)) empty_places_count++; else return empty_places_count;
			}
			return empty_places_count;
		}
		if (d == "bottom") {
			int empty_places_count = 0;
			for (int i = size - 1; i > -1; i--) {
				if (is_cell_is(i, k, 0)) empty_places_count++; else return empty_places_count;
			}
			return empty_places_count;
		}
	}
	int max_cand_in_a(char* d, int r) {
		if (d == "row") {//��� �����
			int max = 0;
			for (int k = 0; k < size; k++) {
				for (int n = 0; n < fast_candidates[r][k].size(); n++) {
					if (fast_candidates[r][k][n] > max) max = fast_candidates[r][k][n];
				}
			}
			return max;
		}
		if (d == "col") {//��� ��������
			int max = 0;
			for (int k = 0; k < size; k++) {
				for (int n = 0; n < fast_candidates[k][r].size(); n++) {
					if (fast_candidates[k][r][n] > max) max = fast_candidates[k][r][n];
				}
			}
			return max;
		}
	}

	//������ � �������
	void set_cell_for_block(int _block_number, int _number_in_a_block, int v) {
		int real_coord = blocks_table[_block_number].cell_s_coord[_number_in_a_block];
		int real_number = blocks_table[_block_number].number;
		if (blocks_table[_block_number].dir == "left" || blocks_table[_block_number].dir == "rigth") {
			set_cell(real_number, real_coord, v);
		}
		if (blocks_table[_block_number].dir == "top" || blocks_table[_block_number].dir == "bottom") {
			set_cell(real_coord, real_number, v);
		}

	}
	void erase_cell_s_cand_for_block(int _block_number, int _number_in_a_block, int caand){
		int real_coord = blocks_table[_block_number].cell_s_coord[_number_in_a_block];
		int real_number = blocks_table[_block_number].number;
		for (int n = 0; n < blocks_table[_block_number].candidates[_number_in_a_block].size(); n++) {
			if (blocks_table[_block_number].candidates[_number_in_a_block][n] == caand) {
				blocks_table[_block_number].candidates[_number_in_a_block].erase(blocks_table[_block_number].candidates[_number_in_a_block].begin() + n);
			}
		}

		if (blocks_table[_block_number].dir == "left" || blocks_table[_block_number].dir == "right") {
			erase_cell_s_cand(real_number, real_coord, caand);
		}

		if (blocks_table[_block_number].dir == "top" || blocks_table[_block_number].dir == "bottom") {
			erase_cell_s_cand(real_coord, real_number, caand);
		}

	}
	void creating_blocks() {
		blocks_table.clear();
		//������� ������� �� ����������� �����, ��������� ������ �����
		#pragma region left
		for (int i = 0; i < size; i++) {
			vector <int> cells;
			vector <int> cell_s_coord;
			vector<vector<int>> candidates;
			int size_s_pos = get_value_s_pos(size, "row", i);
			//�������� �� ���������������
			bool is_isolated = true;
			for (int k = size_s_pos + 1; k < size; k++) {
				if (fast_table[i][k] == 0) {
					is_isolated = false;
				}
			}
			if (size_s_pos > 0 && sum_left[i] > 1) {
				for (int j = 0; j <= size_s_pos; j++) {
					cells.push_back(fast_table[i][j]);
					cell_s_coord.push_back(j);
					candidates.push_back(fast_candidates[i][j]);
				}
				blocks_table.push_back(*new block(is_isolated, sum_left[i], "left", i, cell_s_coord, cells, candidates));
			}
		}
		#pragma endregion
		#pragma region right
		for (int i = 0; i < size; i++) {
			vector <int> cells;
			vector <int> cell_s_coord;
			vector<vector<int>> candidates;
			int size_s_pos = get_value_s_pos(size, "row", i);
			//�������� �� ���������������
			bool is_isolated = true;
			for (int k = 0; k < size_s_pos; k++) {
				if (fast_table[i][k] == 0) {
					is_isolated = false;
				}
			}
			if (size_s_pos < size - 1 && size_s_pos >= 0 && sum_right[i] > 1) {
				for (int j = size - 1; j >= size_s_pos; j--) {
					cells.push_back(fast_table[i][j]);
					cell_s_coord.push_back(j);
					candidates.push_back(fast_candidates[i][j]);
				}
				blocks_table.push_back(*new block(is_isolated, sum_right[i], "right", i, cell_s_coord, cells, candidates));
			}
		}
		#pragma endregion
		#pragma region top
		for (int i = 0; i < size; i++) {
			vector <int> cells;
			vector <int> cell_s_coord;
			vector<vector<int>> candidates;
			int size_s_pos = get_value_s_pos(size, "col", i);
			//�������� �� ���������������
			bool is_isolated = true;
			for (int k = size_s_pos + 1; k < size; k++) {
				if (fast_table[k][i] == 0) {
					is_isolated = false;
				}
			}
			if (size_s_pos > 0 && sum_top[i] > 1) {
				for (int j = 0; j <= size_s_pos; j++) {
					cells.push_back(fast_table[j][i]);
					cell_s_coord.push_back(j);
					candidates.push_back(fast_candidates[j][i]);
				}
				blocks_table.push_back(*new block(is_isolated, sum_top[i], "top", i, cell_s_coord, cells, candidates));
			}
		}
		#pragma endregion
		#pragma region bottom
		for (int i = 0; i < size; i++) {
			vector <int> cells;
			vector <int> cell_s_coord;
			vector<vector<int>> candidates;
			int size_s_pos = get_value_s_pos(size, "col", i);
			//�������� �� ���������������
			bool is_isolated = true;
			for (int k = 0; k < size_s_pos; k++) {
				if (fast_table[k][i] == 0) {
					is_isolated = false;
				}
			}
			if (size_s_pos < size - 1 && size_s_pos >= 0 && sum_bottom[i] > 1) {
				for (int j = size - 1; j >= size_s_pos; j--) {
					cells.push_back(fast_table[j][i]);
					cell_s_coord.push_back(j);
					candidates.push_back(fast_candidates[j][i]);
				}
				blocks_table.push_back(*new block(is_isolated, sum_bottom[i], "bottom", i, cell_s_coord, cells, candidates));
			}
		}
		#pragma endregion
	}
	void optimization_blocks() {
		//����� �������� ������ ���������� ��������� ��������, ����������� ��� ������� ������
		bool l_d = false;
		//��������� ������
		for (int i = 0; i < blocks_table.size(); i++) {
			vector <int> pos_for_deleting;
			int count = 0;
			//������� ������� ���������
			int last = 0;
			vector <int> change_positions;
			for (int j = 0; j < blocks_table[i].cells.size(); j++) {
				if (blocks_table[i].cells[j] > last) {
					last = blocks_table[i].cells[j];
					change_positions.push_back(j);
				}
			}

			//���������� ���������� ������� ���������
			for (int j = 0; j < change_positions.size() - 1; j++) {
				bool flag_delete = false;
				//���������� ������ �� ������ �� ���������� �������� ������� ���������
				for (int k = 0; k < change_positions[j + 1]; k++) {
					//���������� ��������� �� ���������� ��������� �� size -- �.�. ���� ������, ��������� ������� ������� �������
					for (int candd = blocks_table[i].cells[change_positions[j]]; candd < size; candd++) {
						if (blocks_table[i].is_cell_have_a_candidate(k, candd)) {
							flag_delete = true;
						}
					}
				}
				//����� ������, ������� ����� �������
				if (!flag_delete) {
					count++;
					if (g_d && l_d) cout << "������� � ����� " << i << ": ";
					for (int k = change_positions[j]; k < change_positions[j + 1]; k++) {
						if (g_d && l_d) cout << " " << k;
						pos_for_deleting.push_back(k);

					}
					if (g_d && l_d) cout << endl;
				}
			}

			//�������� ���-��, ��� �������� 3 | 4 0 0 0 0 6

			//���� �� ������ ���-������ �������� ��� ��������
			if (pos_for_deleting.size() > 0) {
				if (g_d && l_d) cout << endl << "�� ��������, ���� " << i << ": ";
				if (g_d && l_d) blocks_table[i].print();
				//������� � �������� ������� ���������� � pos_for_deleting � �������� ����� �� count
				for (int j = pos_for_deleting.size() - 1; j >= 0; j--){
					blocks_table[i].cells.erase(blocks_table[i].cells.begin() + pos_for_deleting[j]);
					blocks_table[i].cell_s_coord.erase(blocks_table[i].cell_s_coord.begin() + pos_for_deleting[j]);
					blocks_table[i].candidates.erase(blocks_table[i].candidates.begin() + pos_for_deleting[j]);
				}
				blocks_table[i].sum -= count;
				if (g_d && l_d) cout << "����� ��������, ���� " << i << ": ";
				if (g_d && l_d) blocks_table[i].print();
				if (g_d && l_d) cout << "________________________________" << endl;
			}
		}
		for (int i = blocks_table.size() - 1; i >= 0; i--) {
			//���������, ����� ��� ���� ������ ������ �� �����, ���� ��� - �� ������� �����!
			if (blocks_table[i].cells.size() <= 2)
			{
				if (g_d && l_d) cout << "������ ������� ���� " << i << " �� ��������������" << endl;
				blocks_table.erase(blocks_table.begin() + i);
			}
		}
	}
	//����� �� ���������� ������� ���������� ������ - �� �����, ����� ������ �������

	//��������
	bool check(){
		bool l_d = true;
		//�������� �� ������ �������
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (fast_table[i][j] < 0 || fast_table[i][j] > size) {
					if (g_d && l_d) cout << "� ������� ���� ������ �������" << endl;
					return false;
				}
			}
		}
		//�������� �� ������������� � ������� � ��������
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				for (int k = 0; k < size; k++) {
					if ((fast_table[i][k] == fast_table[i][j]) && (k != j) && (fast_table[i][j]) > 0) {
						if (g_d && l_d) cout << "� ������ " << i << " ���� ������������� ������" << endl;
						return false;
					}

					if ((fast_table[k][j] == fast_table[i][j]) && (k != i) && (fast_table[i][j]) > 0) {
						if (g_d && l_d) cout << "� ������� " << j << " ���� ������������� ������" << endl;
						return false;
					}
				}
			}
		}
		//�������� ����� ��� ������ ����������� �����, ������� ����� ��� �������� ����� -> �����, ����������� ������ calc �������?
		for (int n = 0; n < size; n++) {
			//�������� �� ������� ������
			bool flag_is_all_placed = true;
			for (int k = 0; k < size; k++) {
				if (fast_table[n][k] == 0) {
					flag_is_all_placed = false;
					break;
				}
			}
			//�������� �� �����, ������
			//��� ������, ����� ����� �������
			if (calc_real_sum(n, "left") < sum_left[n] && flag_is_all_placed && sum_left[n] != 0) {
				if (g_d && l_d) cout << "�������� ����� ����������� ������ " << n << " ����� ������� ������ ��������" << endl;
				return false;
			}
			//�������� �� �����, ������
			//��� ������, ����� ������ ������
			if (calc_real_sum(n, "right") < sum_right[n] && flag_is_all_placed && sum_right[n] != 0) {
				if (g_d && l_d) cout << "�������� ����� ����������� ������ " << n << " ������ ������ ������ ��������" << endl;
				return false;
			}
			//�������� �� ������������ ����, ������
			//��� ������, ����� ����� �������
			if (calc_real_sum(n, "left") > sum_left[n] && flag_is_all_placed && sum_left[n] != 0) {
				if (g_d && l_d) cout << "�������� ����� ������ " << n << " ����� ������� ������ ��������" << endl;
				return false;
			}
			//�������� �� ������������ ����, ������
			//��� ������, ����� ������ ������
			if (calc_real_sum(n, "right") > sum_right[n] && flag_is_all_placed && sum_right[n] != 0) {
				if (g_d && l_d) cout << "�������� ����� ������ " << n << " ������ ������ ������ ��������" << endl;
				return false;
			}
			//�������� �� ������� ������
			flag_is_all_placed = true;
			for (int k = 0; k < size; k++) {
				if (fast_table[k][n] == 0) {
					flag_is_all_placed = false;
					break;
				}
			}
			//�������� �� �����, ������
			//��� �������, ����� ������ ����
			if (calc_real_sum(n, "top") < sum_top[n] && flag_is_all_placed && sum_top[n] != 0) {
				if (g_d && l_d) cout << "�������� ����� ������� " << n << " ������ ���� ������ ��������" << endl;
				return false;
			}
			//�������� �� �����, ������
			//��� �������, ����� ����� �����
			if (calc_real_sum(n, "bottom") < sum_bottom[n] && flag_is_all_placed && sum_bottom[n] != 0) {
				if (g_d && l_d) cout << "�������� ����� ������� " << n << " ����� ����� ������ ��������" << endl;
				return false;
			}
			//�������� �� ������������ ����, ������
			//��� �������, ����� ������ ����
			if (calc_real_sum(n, "top") > sum_top[n] && flag_is_all_placed && sum_top[n] != 0) {
				if (g_d && l_d) cout << "�������� ����� ������� " << n << " ������ ���� ������ ��������" << endl;
				return false;
			}
			//�������� �� ������������ ����, ������
			//��� �������, ����� ����� �����
			if (calc_real_sum(n, "bottom") > sum_bottom[n] && flag_is_all_placed && sum_bottom[n] != 0) {
				if (g_d && l_d) cout << "�������� ����� ������� " << n << " ����� ����� ������ ��������" << endl;
				return false;
			}
		}
		return true;
	}
	bool is_have_cell(int v, char* d, int n) {
		if (d == "row") {//��� �����
			for (int k = 0; k < size; k++)
				if (fast_table[n][k] == v) {
				return true;
				}
		}
		if (d == "col") {//��� ��������
			for (int k = 0; k < size; k++)
				if (fast_table[k][n] == v) {
				return true;
				}
		}
		return false;
	}
	bool is_cell_is(int i, int j, int v) {
		if (fast_table[i][j] == v) {
			return true;
		}
		return false;
	}
	bool is_cell_have_a_candidate(int i, int j, int cand) {
		for (int k = 0; k < (fast_candidates[i][j]).size(); k++) {
			if (fast_candidates[i][j][k] == cand) {
				return true;
			}
		}
		return false;
	}
	bool is_all_empty(int pos, char* d, int n) {
		if (pos == -1) {
			return false;
		}
		if (d == "left") {//��� ����� ������
			for (int k = 0; k < pos; k++) {
				if (fast_table[n][k] != 0) {
					return false;
				}
			}
			return true;
		}
		if (d == "rigth") {//��� ����� �������
			for (int k = pos + 1; k < size; k++) {
				if (fast_table[n][k] != 0) {
					return false;
				}
			}
			return true;
		}
		if (d == "top") {//��� �������� ������
			for (int k = pos + 1; k < size; k++) {
				if (fast_table[k][n] != 0) {
					return false;
				}
			}
			return true;
		}
		if (d == "bottom") {//��� �������� ��������� ����
			for (int k = 0; k < pos; k++) {
				if (fast_table[k][n] != 0) {
					return false;
				}
			}
			return true;
		}
	}
	bool is_full() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (fast_table[i][j] == 0) {
					return false;
				}
			}
		}
		return true;
	}

	//������
	void print() {
		//�����
		cout << "   ";
		for (int i = 0; i < size; i++) {
			cout << sum_top[i] << " ";
		}
		cout << endl << endl;
		//��������
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (j == 0) {
					cout << sum_left[i] << "  ";
				}
				cout << fast_table[i][j] << " ";
				if (j == size - 1) {
					cout << " " << sum_right[i] << endl;
				}
			}
		}
		//�����
		cout << endl;
		cout << "   ";
		for (int i = 0; i < size; i++) {
			cout << sum_bottom[i] << " ";
		}
		cout << endl;
	}
	void print_cand() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (fast_candidates[i][j].empty()) {
					cout << "_no";
					for (int k = 0; k < size - 3; k++) {
						cout << "_";
					}
					cout << " ";
				}
				else {

					for (int k = 0; k < fast_candidates[i][j].size(); k++) {
						cout << fast_candidates[i][j][k];
					}
					for (int k = 0; k < 1 + size - fast_candidates[i][j].size(); k++) {
						cout << " ";
					}
				}
			}
			cout << endl;
		}
	}
	void print_blocks() {
		cout << "________________________________________" << endl << "����� (" << blocks_table.size() << ")" << endl;
		for (int i = 0; i < blocks_table.size(); i++) {
			cout << "���� " << i << "  ";
			blocks_table[i].print();
			cout << endl;
		}
		cout << "����� ���������� ������" << endl << "________________________________________" << endl;
	}

	//���������� � ���������� ��� ������
	void begin() {
		bool l_d = false;
		//���������� ����� ������� ����� ��������
		for (int i = 0; i < size; i++) {
			if (sum_top[i] == 1) {
				if (g_d && l_d) cout << "begin ��������� ������ (" << 0 << ", " << i << ") ������������ ��������� ��������� " << size << endl;
				set_cell(0, i, size);
				continue;
			}
			if (sum_bottom[i] == 1) {
				if (g_d && l_d) cout << "begin ��������� ������ (" << size - 1 << ", " << i << ") ������������ ��������� ��������� " << size << endl;
				set_cell(size - 1, i, size);
				continue;
			}
		}
		for (int i = 0; i < size; i++) {
			if (sum_left[i] == 1) {
				if (g_d && l_d) cout << "begin ��������� ������ (" << i << ", " << 0 << ") ������������ ��������� ��������� " << size << endl;
				set_cell(i, 0, size);
				continue;
			}
			if (sum_right[i] == 1) {
				if (g_d && l_d) cout << "begin ��������� ������ (" << i << ", " << size - 1 << ") ������������ ��������� ��������� " << size << endl;
				set_cell(i, size - 1, size);
				continue;
			}
		}
	}
	void check_set_single() {
		bool l_d = false;
		//�������� - ��������
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (fast_candidates[i][j].size() == 1) {
					if (g_d && l_d) cout << "check_set_single ��������� ������-�������� (" << i << ", " << j << ") ��������� " << fast_candidates[i][j][0] << endl;
					set_cell(i, j, fast_candidates[i][j][0]);
				}
			}
		}
		//������������ �������� � ������\�������
		for (int n = 1; n <= size; n++) {
			for (int i = 0; i < size; i++) {
				//��� �����
				int count_candidates_in_a = 0; int last_pos;
				for (int j = 0; j < size; j++) {
					for (int k = 0; k < fast_candidates[i][j].size(); k++) {
						if (fast_candidates[i][j][k] == n) {
							count_candidates_in_a++;
							last_pos = j;
						}
					}
				}
				if (count_candidates_in_a == 1) {
					if (g_d && l_d) cout << "check_set_single ��������� ������ (" << i << ", " << last_pos << ") ��������� " << n << " , �.�. ��� ������������ �������� � ������" << endl;
					set_cell(i, last_pos, n);
				}
				//��� ��������
				count_candidates_in_a = 0;
				for (int j = 0; j < size; j++) {
					for (int k = 0; k < fast_candidates[j][i].size(); k++) {
						if (fast_candidates[j][i][k] == n) {
							count_candidates_in_a++;
							last_pos = j;
						}
					}
				}
				if (count_candidates_in_a == 1) {
					if (g_d && l_d) cout << "check_set_single ��������� ������ (" << last_pos << ", " << i << ") ��������� " << n << " , �.�. ��� ������������ �������� � �������" << endl;
					set_cell(last_pos, i, n);
				}
			}
		}
	}
	void clear_cand_hard() {
		//��� ������ �����
		bool l_d = false;
		//�������� ������� ������������ ����������
		for (int i = 0; i < size; i++) {
			//fast_candidates
			//��� ������, ����� ����� �������
			if (sum_left[i] >= 2) {
				for (int n = 0; n < sum_left[i] - 1; n++) {
					if (g_d && l_d) cout << "clear_cand_hard >: ������� � ������ (" << i << ", " << n << ") ��������� ";
					for (int m = 0; m < sum_left[i] - n - 1; m++) {
						if (g_d && l_d) cout << size - m << " ";
						erase_cell_s_cand(i, n, size - m);
					}
					if (g_d && l_d) cout << endl;
				}
			}
			//��� ������, ����� ������ ������
			if (sum_right[i] >= 2) {
				for (int n = 0; n < sum_right[i] - 1; n++) {
					if (g_d && l_d) cout << "clear_cand_hard <: ������� � ������ (" << i << ", " << size - n - 1 << ") ��������� ";
					for (int m = 0; m < sum_right[i] - n - 1; m++) {
						if (g_d && l_d) cout << size - m << " ";
						erase_cell_s_cand(i, size - n - 1, size - m);
					}
					if (g_d && l_d) cout << endl;
				}
			}
			//��� �������, ����� ������ ����
			if (sum_top[i] >= 2) {
				for (int n = 0; n < sum_top[i] - 1; n++) {
					if (g_d && l_d) cout << "clear_cand_hard v: ������� � ������ (" << n << ", " << i << ") ��������� ";
					for (int m = 0; m < sum_top[i] - n - 1; m++) {
						if (g_d && l_d) cout << size - m << " ";
						erase_cell_s_cand(n, i, size - m);
					}
					if (g_d && l_d) cout << endl;
				}
			}
			//��� �������, ����� ����� �����
			if (sum_bottom[i] >= 2) {
				for (int n = 0; n < sum_bottom[i] - 1; n++) {
					if (g_d && l_d) cout << "clear_cand_hard ^: ������� � ������ (" << size - n - 1 << ", " << i << ") ��������� ";
					for (int m = 0; m < sum_bottom[i] - n - 1; m++) {
						if (g_d && l_d) cout << size - m << " ";
						erase_cell_s_cand(size - n - 1, i, size - m);
					}
					if (g_d && l_d) cout << endl;
				}
			}
		}
	}

	//��� ������
	void sum_two() {
		bool l_d = false;
		//����� �� ��������� �� �������� ��, �� �������� ������ ������������
		//�������� �����-2, ������� size, � m>=2 ������ ������ �� ����:
		// 1) ���������� m-1, m-2, ... ���������� � ������ ������ 
		// 2) ������� ����� ������ - �������� ���������� ��������� �� n �� ����, �������� ��� ����
		// 3) ������ ����� ������ - �������� �������� � ��������
		for (int i = 0; i < blocks_table.size(); i++) {
			if (blocks_table[i].sum == 2 && blocks_table[i].is_all_empty(blocks_table[i].get_value_s_pos(size))
				&& blocks_table[i].get_value_s_pos(size) > 1
				&& blocks_table[i].cells[0] == 0 && blocks_table[i].cells[1] == 0) {
				if (g_d && l_d) cout << "sum_two_1 >: ������� � ����� " << i << " � ������ " << 0 << " ��� �����-2 � ������� " << size << " ������ ���������: ";
				for (int k = 1; blocks_table[i].get_value_s_pos(size) - k > 0; k++) {
					if (g_d && l_d) cout << blocks_table[i].get_value_s_pos(size) - k << " ";
					erase_cell_s_cand_for_block(i, 0, blocks_table[i].get_value_s_pos(size) - k);
				}
				if (g_d && l_d) cout << endl;

				for (int j = blocks_table[i].get_value_s_pos(size) - 1; j > 0; j--) {
					if (g_d && l_d) cout << "sum_two_2 >: ������� � ����� " << i << " � ������ " << j << " �������� " << blocks_table[i].max_cand() << " ��� �����-2 � ������� " << size << " � �����" << endl;
					erase_cell_s_cand_for_block(i, j, blocks_table[i].max_cand());
				}

				if (blocks_table[i].is_cell_have_a_candidate(0, blocks_table[i].min_cand())) {
					if (g_d && l_d) cout << "sum_two_3 >: ������� � ����� " << i << " � ������ " << 0 << " ����������� �������� " << blocks_table[i].min_cand() << " � ������ �����" << endl;
					erase_cell_s_cand_for_block(i, 0, blocks_table[i].min_cand());
				}
			}
		}
	}
	void max_stairs() {
		bool l_d = false;
		//���������� ���������� ��������, ������� � ���������� � ������� ����������
		for (int i = 0; i < blocks_table.size(); i++) {
			if (blocks_table[i].sum == blocks_table[i].calc_real_sum() + blocks_table[i].calc_empty_places_until_noempty()
				&& blocks_table[i].cells[0] == 0 && blocks_table[i].cells[1] == 0
				&& blocks_table[i].is_all_empty(blocks_table[i].cells.size() - 1)
				) {
				if (g_d && l_d) cout << "max_stairs >: ��������� ��������� ����� " << i << " ��������" << endl;

				int max_cand = size;
				for (int j = blocks_table[i].calc_empty_places_until_noempty() - 1; j >= 0; j--) {
					//� ������ ������ �����
					//������� ������� ��������� ������ �� ���������� ��������
					for (int k = blocks_table[i].candidates[j].size() - 1; k >= 0; k--) {
						if (blocks_table[i].candidates[j][k] < max_cand) {
							max_cand = blocks_table[i].candidates[j][k];
							break;
						}
					}
					//������� � ������ ��� ���������, ������� ���������, ������ �������� ��������� - �.�. ����������
					for (int k = 0; k < blocks_table[i].candidates[j].size(); k++) {
						if (blocks_table[i].candidates[j][k] > max_cand) {
							if (g_d && l_d) cout << " (" << i << ", " << j << "): " << blocks_table[i].candidates[j][k] << endl;
							erase_cell_s_cand_for_block(i, j, blocks_table[i].candidates[j][k]);
						}
					}
				}
			}
		}
	}
	void min_stairs() {
		bool l_d = false;
		//�������� �������, ��� ������ ����� �������, ������� � ����� ����-������ - �������� ���������� ��������, ������� �� �������
		for (int i = 0; i < blocks_table.size(); i++) {
			if (blocks_table[i].sum == blocks_table[i].calc_real_sum() + blocks_table[i].calc_empty_places_until_noempty()
				&& blocks_table[i].cells[0] == 0 && blocks_table[i].cells[1] == 0
				&& blocks_table[i].is_all_empty(blocks_table[i].cells.size() - 1)
				) {
				if (g_d && l_d) cout << "min_stairs >: ������� � ����� " << i << " ��������� �������� ��������" << endl;

				int min_cand = 0;
				for (int j = 0; j < blocks_table[i].cells.size(); j++) {
					//� ������ ������ �����
					//������� ������� ��������� ������ �� ��������� ��������
					for (int k = 0; k < blocks_table[i].candidates[j].size(); k++) {
						if (blocks_table[i].candidates[j][k] > min_cand) {
							min_cand = blocks_table[i].candidates[j][k];
							break;
						}
					}
					//������� � ������ ��� ���������, ������ ��������, ������ ��������
					for (int k = blocks_table[i].candidates[j].size() - 1; k >= 0; k--) {
						if (blocks_table[i].candidates[j][k] < min_cand) {
							if (g_d && l_d) cout << " (" << i << ", " << j << "): " << blocks_table[i].candidates[j][k] << endl;
							erase_cell_s_cand_for_block(i, j, blocks_table[i].candidates[j][k]);
						}
					}
				}
			}
		}
	}
	void free_stairs() {
		//���������� clear_cand_hard ��� ������
		bool l_d = false;
		for (int i = 0; i < blocks_table.size(); i++) {
			if (blocks_table[i].sum > 2
				&& blocks_table[i].cells[0] == 0 && blocks_table[i].cells[1] == 0
				&& blocks_table[i].is_all_empty(blocks_table[i].cells.size() - 1)
				&& blocks_table[i].fully_isolated
				) {

				int max = blocks_table[i].max_cand();

				if (blocks_table[i].sum > 2) {
					for (int n = 0; n < blocks_table[i].sum - 2; n++) {
						if (g_d && l_d) cout << "free_stairs >: ������� � ������ (" << i << ", " << n << ") ��������� ";
						for (int m = 0; m < blocks_table[i].sum - n - 2; m++) {
							if (g_d && l_d) cout << max - m << " ";
							erase_cell_s_cand_for_block(i, n, max - m);
						}
						if (g_d && l_d) cout << endl;
					}
				}
			}
		}
	}

	void Ariadna_s_thread() {
		//����� �������������, ���� ����� ������������� ������� ���� ���������� - ����� ������ ��� �������������
		//��� ����������������� �������� �� ����������� ���������
		Ariadna_for_cells();
		Ariadna_for_rows_and_cols();
	}
	void Ariadna_for_cells() {
		bool l_d = false;
		//��� ����� ���������� � �������
		for (int k = 2; k < size - 2; k++) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (fast_candidates[i][j].size() == k) {
						#pragma region ���������� ������� ���������
						if (g_d && l_d) {
							cout << "��������� " << k << "; ������� " << entry_level << ": ������������� ������ (" << i << ", " << j << ")" << endl;
						}
						vector<table> mas;
						for (int n = 0; n < fast_candidates[i][j].size(); n++) {
							mas.push_back(*this);
							mas[n].entry_level++;
						}
						#pragma endregion
						for (int n = 0; n < mas.size(); n++) {
							if (Ariadna(&mas[n], new point(i, j, fast_candidates[i][j][n])) == 1) {
								return;
							}
						}
					}
				}
			}
		}
	}
	void Ariadna_for_rows_and_cols() {
		bool l_d = false;
		//��� ����� ���������� � �������/�������
		for (int cand = 1; cand < size; cand++) {//��� ���������
			for (int k = 2; k < 4; k++) {//����� ��������� � ������\������� - �����������
				for (int i = 0; i < size; i++) {
					#pragma region ������� ���������� ������������ ���������� ����������
					int count_row = 0, count_col = 0;
					//������
					for (int j = 0; j < size; j++) {
						for (int n = 0; n < fast_candidates[i][j].size(); n++) {
							if (fast_candidates[i][j][n] == cand) {
								count_row++;
							}
						}
					}

					//�������
					for (int j = 0; j < size; j++) {
						for (int n = 0; n < fast_candidates[j][i].size(); n++) {
							if (fast_candidates[j][i][n] == cand) {
								count_col++;
							}
						}
					}
					#pragma endregion
					//������
					if (count_row == k) {
						for (int j = 0; j < size; j++) {
							vector<table> mas;

							if (is_cell_have_a_candidate(i, j, cand)) {
								if (g_d && l_d) {
									cout << "�������� " << cand << "; ����� ���������� � ������ " << k << "; ������� " << entry_level << ": ������������� ������ (" << i << ", " << j << ")" << endl;
								}
								mas.push_back(*this);
								mas.back().entry_level++;
								if (Ariadna(&mas.back(), new point(i, j, cand)) == 1) {
									return;
								}
							}
						}
					}
					//�������
					if (count_col == k) {
						for (int j = 0; j < size; j++) {
							vector<table> mas2;

							if (is_cell_have_a_candidate(j, i, cand)) {
								if (g_d && l_d) {
									cout << "�������� " << cand << "; ����� ���������� � ������� " << k << "; ������� " << entry_level << ": ������������� ������ (" << j << ", " << i << ")" << endl;
								}
								mas2.push_back(*this);
								mas2.back().entry_level++;
								if (Ariadna(&mas2.back(), new point(j, i, cand)) == 1) {
									return;
								}
							}
						}
					}
				}

			}
		}
	}
	
	int Ariadna(table* t, point* p) {
		bool l_d = false;
		if (g_d && l_d) cout << "������������� ��������" << endl;
		t->set_cell(p->x, p->y, p->value);

		if (g_d && l_d) {
			t->print();
			t->print_cand();
			cout << "_________________________" << endl;
		}
		for (int k = 0; (k < 5) && t->check() && !t->is_full(); k++) {
			//������ ������ ��� �������
			t->creating_blocks(); t->optimization_blocks();
			t->sum_two(); t->check_set_single();
			t->max_stairs(); t->check_set_single();
			t->min_stairs(); t->check_set_single();
			t->free_stairs(); t->check_set_single();
			//t->print_blocks();
			//���� �������
			if (t->entry_level < 2 && k >= 4) {
				t->Ariadna_s_thread(); t->check_set_single();
			}
		}
		//cout << "����� ��������������" << endl;
		if (g_d && l_d) {
			t->print(); t->print_cand(); cout << endl;
		}
		if (t->check() == false) {
			cout << "������� " << t->entry_level << ": ������� ������� �������� " << p->value << " � ������ (" << p->x << ", " << p->y << ")" << endl;
			erase_cell_s_cand(p->x, p->y, p->value);
			return 1;
		}
		return 0;
	}
};

