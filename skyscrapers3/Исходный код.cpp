#include <iostream>
#include <iomanip>
#include <vector>
#include "windows.h"
#include <fstream>

#include "choice.h"
//-> table.h -> block.h
//           -> input.h
using namespace std;

void main() {
	setlocale(LC_ALL, "RUS");
	table r = new table(choice_data(1, 8));
	r.begin(); r.check_set_single(); r.clear_cand_hard(); r.check_set_single();
	r.print(); cout << endl; r.print_cand(); cout << endl << r.check() << endl;

	cout << endl << "________________________________" << endl;
	for (int i = 0; i < 30 && !r.is_full(); i++) {
		cout << "cycle " << i << endl;
		r.check_set_single();
		if (i >= 2) {
			r.creating_blocks();
			r.optimization_blocks();
			r.sum_two(); r.check_set_single();
			r.max_stairs(); r.check_set_single();
			r.min_stairs(); r.check_set_single();
			r.free_stairs(); r.check_set_single();
			r.print_blocks();
			system("pause");
		}
		if (i > 5) {
			cout << "__________ARIADNA_________________" << endl;
			r.Ariadna_s_thread(); r.check_set_single();
			cout << "__________ARIADNA_END_____________" << endl;
		}
		cout << endl; r.print(); cout << endl; r.print_cand(); cout << endl << "__" << r.check() << "________________________________" << endl;
	}
	system("PAUSE");
}