/*
 * NFABuilder.cpp
 *
 *  Created on: 12 Mar 2015
 *      Author: ahmedaboemera
 */

#include "NFABuilder.h"

NFA_Builder::NFA_Builder(char* s) {
	this->in.open(s);
	this->p = new Parser();
}

vector<NFA*> NFA_Builder::process_file() {

	while (!in.eof()) {
		NFA* temp = process_next_line();
		if (temp != NULL)				// non-terminal: should be added
		{
			this->all_nfa.push_back(temp);
		}
	}

	return all_nfa;
}

NFA* NFA_Builder::process_next_line() {
	int i = 0, j = 0;
	string line = read_next_line();

	if(line[0]=='{'){
		vector<string> key_words;
		string s="";
		for(int i=1;i<line.length()-1;i++){
			if(line[i]==' '){
				key_words.push_back(s);
				s=" ";
			}
			else{
				s += line[i];
			}
		}
		key_words.push_back(s);
		for(vector<string>::iterator it = key_words.begin(); it!=key_words.end();++it){
			string temp = "";
			string y = *it;
			for(int i=0 ; i<y.length()-1 ; i++){
				temp += y[i];
				temp += ".";
			}
			temp += y[y.length()-1];
			stack<string>*s = this->p->postFix_generator(temp);
			stack<string>*t = new stack<string> ();
			while(!s->empty()){
				t->push(s->top());
				s->pop();
			}
			delete s;
			NFA* tempo = generate_NFA(t);
			this->all_nfa.push_back(tempo);
		}
		return NULL;
	}

	string x = "";
	for (i = 0; i < line.length(); i++) {
		if (line[i] != ' ')
			x += line[i];
	}
	line = x;
	for (j = 0; j < line.length() && line[j] != '=' && line[j] != ':'; j++)
		;	// pass all spaces before name
	string lable = line.substr(0, j);
	j++;

	NFA* nfa;
	stack<string>* s = this->p->postFix_generator(
			line.substr(j, line.length() - j));
	stack<string>*t = new stack<string>();
	while (!s->empty()) {
		t->push(s->top());
		s->pop();
	}
	if (line.find(':') != string::npos) {	// defining nonterminal
		nonterminal_NFA_map[lable] = generate_NFA(t);
		return NULL;
	} else {								// defining terminal
		nfa = generate_NFA(t);
	}
	return nfa;
}

string NFA_Builder::read_next_line() {
	string line;
	getline(in, line);
	return line;
}

bool NFA_Builder::is_operation(string s) {
	return (s.compare("|") == 0 || s.compare("*") == 0 || s.compare("+") == 0
			|| s.compare(".") == 0);
}

bool NFA_Builder::is_unary_operation(string s) {
	return s.compare("*") == 0 || s.compare("+") == 0;
}

NFA* NFA_Builder::parse_operand_to_nfa(string s) {
	map<string, NFA*>::iterator it = nonterminal_NFA_map.find(s);
	if (nonterminal_NFA_map.find(s) != nonterminal_NFA_map.end())
		return nonterminal_NFA_map[s];

	bool range_flag = false;
	int i = 0;
	char start, end;
	for (; i < s.length(); i++) {
		if (s[i] == '-') {
			range_flag = true;
			start = s[i - 1];
			end = s[i + 1];
			break;
		}
	}
	if (range_flag) {
		vector<NFA*> range;
		for (int i = start; i <= end; i++) {
			char now = i;
			string in = "";
			in += now;
			NFA* nfa = new NFA();
			nfa->connect(// connect NFA of the terminal in single edge:	S------>A
					nfa->add_starting(), nfa->add_acceptor(in), in);
			range.push_back(nfa);
		}

		return NFA::_union(range);
	}
	NFA* nfa = new NFA();
	nfa->connect(	// connect NFA of the terminal in single edge:	S------>A
			nfa->add_starting(), nfa->add_acceptor(s), s);

	return nfa;
}

NFA* NFA_Builder::connect_operand_with_operation(NFA* op1, NFA* op2,
		string operation) {
	vector<NFA*> tmp;

	if (operation.compare("+") == 0) {
		NFA t = *op1;					// // // WT.. :D :D :D
		tmp.push_back(&t);
		tmp.push_back(NFA::_close(*op1));
		return NFA::_concatenate(tmp);
	} else if (operation.compare("*") == 0) {
		return NFA::_close(*op2);
	}

	tmp.push_back(op1);
	tmp.push_back(op2);

	if (operation.compare("|") == 0) {
		return NFA::_union(tmp);
	} else if (operation.compare(".") == 0) {
		return NFA::_concatenate(tmp);
	}
	// do we need to call distructors of original NFA's??
	return new NFA();	// wrong operation
}

NFA* NFA_Builder::generate_NFA(stack<string>* s) {
	stack<NFA*> tmp;
	NFA *op1 = new NFA(), *op2 = new NFA();
	while (!s->empty()) {
		if ((s->top()).compare("") == 0) {
			s->pop();
			continue;
		} else {
			op1 = NULL;
			if (is_operation(s->top())) {		// make an operation
				op2 = tmp.top();
				tmp.pop();
				if (!is_unary_operation(s->top())) {
					op1 = tmp.top();
					tmp.pop();
				}
				string tt = s->top();
				NFA* tmp_NFA = connect_operand_with_operation(op1, op2, tt);
				tmp.push(tmp_NFA);
			} else {						// add operand on top of tmp stack
				NFA* tmp_NFA = parse_operand_to_nfa(s->top());	// push NFA
				tmp.push(tmp_NFA);
			}
			s->pop();
		}
	}
	return tmp.top();
}

NFA_Builder::~NFA_Builder() {
	// TODO Auto-generated destructor stub
}

