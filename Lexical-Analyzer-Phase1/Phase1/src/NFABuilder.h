/*
 * NFABuilder.h
 *
 *  Created on: 12 Mar 2015
 *      Author: ahmedaboemera
 */

#ifndef SRC_NFABUILDER_H_
#define SRC_NFABUILDER_H_

#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include "NFA.h"
#include "Parser.h"
#include <string>
#include <vector>

class NFA_Builder {
public:
	NFA_Builder(char* filename);
	vector<NFA*> process_file();
	virtual ~NFA_Builder();
private:
	vector<NFA*> all_nfa;
	Parser* p;
	ifstream in;
	map<string, NFA*> nonterminal_NFA_map;
	NFA* process_next_line();
	string read_next_line();
	bool is_operation(string s);
	bool is_unary_operation(string s);
	NFA* parse_operand_to_nfa(string s);
	NFA* connect_operand_with_operation(NFA* op1, NFA* op2, string operation);
	NFA* generate_NFA(stack <string>* s);
};

#endif /* SRC_NFABUILDER_H_ */
