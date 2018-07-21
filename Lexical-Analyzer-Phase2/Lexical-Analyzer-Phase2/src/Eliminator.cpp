/*
 * Eliminator.cpp
 *
 *  Created on: 12 Apr 2015
 *      Author: ahmedaboemera
 */
#include "Eliminator.h"

Eliminator::Eliminator(Parser* p) {
	parser = p;
}
int Eliminator::find_index(string s) {
	for (int i = 0; i < parser->non_terminals->size(); i++) {
		if (s.compare(parser->non_terminals->at(i)) == 0) {
			return i;
		}
	}
	return (1 << 31 - 1);
}

vector<vector<string> > Eliminator::replace(int i) {

	vector<vector<string> > updated;

	for (int j = 0; j < (parser->non_terminal_defs->at(i)).size(); j++) {
		if ((parser->non_terminal_defs->at(i)).at(j).at(0).compare(
				parser->non_terminals->at(i)) != 0) {
			int index = find_index(
					(parser->non_terminal_defs->at(i)).at(j).at(0));
			if (index < i) {
				for (int k = 0;
						k < (parser->non_terminal_defs->at(index)).size();
						k++) {
					vector<string> vec2;
//					vec.push_back(parser->non_terminal_defs.at(index).at(k));
					for (int z = 0;
							z
									< (parser->non_terminal_defs->at(index)).at(
											k).size(); z++) {
						vec2.push_back(
								(parser->non_terminal_defs->at(index)).at(k).at(
										z));
					}

					for (int z = 0;
							z < (parser->non_terminal_defs->at(i)).at(j).size();
							z++) {
						vec2.push_back(
								(parser->non_terminal_defs->at(i)).at(j).at(z));
					}
					updated.push_back(vec2);
				}

			} else {
				updated.push_back((parser->non_terminal_defs->at(i)).at(j));
			}
		} else {
			updated.push_back((parser->non_terminal_defs->at(i)).at(j));
		}
	}

	return updated;
}

void Eliminator::eliminate_LF() {

}

void Eliminator::eliminate_from_updated(vector<vector<string> > replaced,
		int index) {

	string non_terminal_name = parser->non_terminals->at(index);
	vector<int> lr_indecies;
	vector<int> n_lr_indecies;
	for (int i = 0; i < (parser->non_terminal_defs->at(index)).size(); i++) {
		if (non_terminal_name.compare(
				(parser->non_terminal_defs->at(index)).at(i).at(0)) == 0) {
			lr_indecies.push_back(i);
		} else {
			n_lr_indecies.push_back(i);
		}
	}
	if (lr_indecies.size() == 0) {
		return;
	} else {
		string new_non_terminal = non_terminal_name + "\'";
		parser->non_terminals->push_back(new_non_terminal);
		vector<vector<string> > updated;

		for (int i = 0; i < n_lr_indecies.size(); i++) {
			vector<string> vec = (parser->non_terminal_defs->at(index)).at(
					n_lr_indecies.at(i));
			vec.push_back(new_non_terminal);
			updated.push_back(vec);
		}

		vector<vector<string> > new_update;

		vector<string> epsilon;
		epsilon.push_back("\\L");
		new_update.push_back(epsilon);

		for (int i = 0; i < lr_indecies.size(); i++) {
			vector<string> vec;
			for (int k = 1;
					k
							< (parser->non_terminal_defs->at(index)).at(
									lr_indecies.at(i)).size(); k++) {
				vec.push_back(
						(parser->non_terminal_defs->at(index)).at(
								lr_indecies.at(i)).at(k));
			}
			vec.push_back(new_non_terminal);
			new_update.push_back(vec);
		}

		parser->non_terminal_defs->push_back(new_update);
		parser->non_terminal_defs->at(index) = updated;
	}
}

void Eliminator::print_grammer() {
	for (int i = 0; i < parser->non_terminals->size(); i++) {
		cout << parser->non_terminals->at(i) << " --> ";
		for (int j = 0; j < parser->non_terminal_defs->at(i).size(); j++) {
			for (int l = 0; l < parser->non_terminal_defs->at(i).at(j).size();
					l++) {
				cout <<parser->non_terminal_defs->at(i).at(j).at(l);
			}
			cout << " | ";
		}
		cout << endl;
	}
}

void Eliminator::eliminate_LR() {

	print_grammer();

	for (int i = 0; i < parser->non_terminals->size(); i++) {
		vector<vector<string> > updated = replace(i);
		eliminate_from_updated(updated, i);
	}

	cout<<"=========================================================="<<endl;
	print_grammer();
}

