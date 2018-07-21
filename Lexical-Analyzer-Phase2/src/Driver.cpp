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
#include <stack>

#include "Parser.h"
#include "Eliminator.h"

using namespace std;

vector<string>* modify_grammar_file(const char* file_path) {
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
	vector<string>* modified = modify_grammar_file("src/CFG.txt");
	Parser* p = new Parser();
	p->get_terminals_and_nonterminals(modified);
	Eliminator* e = new Eliminator(p);
	e->eliminate_LR();
	e->eliminate_LF();

	p->built_predictive_table();
}

/*
 * Final code should be using a lexical analyzer instance that supplies
 * the driver with tokens by a function "get_next_token()"
 *
 * The lexical analyzer will simply receive the text file containing
 * the source code required to be parsed, and using a serialized DFA
 * header file it's going to extract tokens out of this source code and
 * supply the driver with them
 *
 * Meanwhile, we're going to hard code the tokens in a text file "tokens.txt"
 * and the driver will simulate the lexical analyzer's "get_next_token"
 * by simply reading one line at a time from the text file
 * */
//int main() {
//	vector<string>* modified = modify_grammar_file("src/CFG.txt");
//	Parser* p = new Parser();
//	p->get_terminals_and_nonterminals(modified);
//	Eliminator* e = new Eliminator(p);
//	e->eliminate_LR();
//	e->eliminate_LF();
//
//	p->built_predictive_table();
//
//	/* change this part with function calls from the lexical analyzer */
//
//	ifstream in; // ip
//	in.open("tokens.txt"); // w
//	stack<string> s;
//	string top = s.top(); // X
//	while(!in.eof()) {
//
//	}
//}
	/* ************************************************************** */
