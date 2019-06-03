#include <iostream>
#include <vector>

using namespace std;

class block {
public:
	char* dir;
	int number;
	int sum;
	vector <int> cells;
	vector <int> cell_s_coord;
	vector<vector<int>> candidates;
	bool fully_isolated;

	bool is_cell_have_a_candidate(int m, int cand) {
		for (int k = 0; k < (candidates[m]).size(); k++) {
			if (candidates[m][k] == cand) {
				return true;
			}
		}
		return false;
	}
	int max_cand() {
		int max = 0;
		for (int k = 0; k < candidates.size(); k++) {
			for (int n = 0; n < (candidates[k]).size(); n++) {
				if (candidates[k][n] > max) max = candidates[k][n];
			}
		}
		return max;
	}
	int min_cand() {
		int min = max_cand();
		for (int k = 0; k < candidates.size(); k++) {
			for (int n = 0; n < (candidates[k]).size(); n++) {
				if (candidates[k][n] < min) min = candidates[k][n];
			}
		}
		return min;
	}
	int get_value_s_pos(int v) {
		for (int k = 0; k < cells.size(); k++) {
			if (cells[k] == v) {
				return k;
			}
		}
		return -1;
	}
	bool is_all_empty(int pos) {
		if (pos == -1) {
			return false;
		}

		for (int k = 0; k < pos; k++) {
			if (cells[k] != 0) {
				return false;
			}
		}
		return true;
	}
	int calc_real_sum() {
		int last = 0, count = 0;
		for (int j = 0; j < cells.size(); j++) {
			if (cells[j] > last) {
				count++; last = cells[j];
			}
		}
		return count;
	}
	int calc_empty_places_until_noempty() {
		int empty_places_count = 0;
		for (int j = 0; j < cells.size(); j++) {
			if (cells[j] == 0) empty_places_count++; else return empty_places_count;
		}
	}

	block() {
		sum = -1;
		dir = "NULL";
		number = -1;
	}
	block(bool _fully_isolated, int _sum, char* _dir, int _number, vector <int> _cell_s_coord, vector <int> _cells, vector<vector<int>> _candidates) {
		fully_isolated = _fully_isolated;
		sum = _sum;
		dir = _dir;
		number = _number;
		cell_s_coord = _cell_s_coord;
		cells = _cells;
		candidates = _candidates;
	}
	block& operator=(block &another) //перегрузка оператора присваивани€
	{
		fully_isolated = another.fully_isolated;
		sum = another.sum;
		dir = another.dir;
		number = another.number;
		cells = another.cells;
		cell_s_coord = another.cell_s_coord;
		candidates = another.candidates;
		return *this;
	}
	~block() {}

	void print() {
		cout << "| " << fully_isolated << " " << dir << " " << number << ", €чейки: ";
		for (int i = 0; i < cells.size(); i++) {
			cout << cell_s_coord[i];
		}
		cout << endl << sum << " | ";
		for (int i = 0; i < cells.size(); i++) {
			cout << " " << cells[i];
		}
		cout << endl;
		for (int i = 0; i < candidates.size(); i++) {
			if (candidates[i].size() == 0) {
				cout << "none";
			}
			else {
				for (int j = 0; j < candidates[i].size(); j++) {
					cout << candidates[i][j];
				}
			}
			cout << " ";
		}
		cout << endl;
	}
};