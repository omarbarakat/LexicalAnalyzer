/*
 * Driver.cpp
 *
 *  Created on: 12 Apr 2015
 *      Author: ahmedaboemera
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "Parser.h"
#include "Eliminator.h"

using namespace std;

vector<string>* modify_file(const char* file_path) {
	vector<string>* to_return = new vector<string>();
	ifstream in;
	in.open(file_path);
	string to_push = "";
	while (!in.eof()) {
		string line;
		getline(in, line);
//		cout<<line<<endl;
		if (line[0] == '#') {
			// concatenate current line with to push
			if (to_push.compare("") != 0) {
				to_return->push_back(to_push);
			}
			to_push = line;
		} else {
			to_push = to_push + line;
		}
	}
	to_return->push_back(to_push);
	return to_return;
}

int main() {
	vector<string>* modified = modify_file("src/CFG.txt");
	Parser* p = new Parser();
	p->get_terminals_and_nonterminals(modified);
//	cout<<terminals->size()<<endl;
//	cout<<non_terminals->size()<<endl;
	Eliminator* e = new Eliminator(p);
	e->eliminate_LR();
}
