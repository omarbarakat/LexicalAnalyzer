#include "Parser.h"

Parser::Parser() {

}

bool Parser::is_operator(char c) {
	if (c != '(' && c != ')' && c != '|' && c != '.') {
		return false;
	}
	return true;
}
string Parser::get_operator(char c) {
	switch (c) {
	case '+':
		return "+";
	case '|':
		return "|";
	case '*':
		return "*";
	case '.':
		return ".";
	default:
		return "";
	}
}
int Parser::get_order(char c) {
	switch (c) {
	case '|':
		return 1;
	case '.':
		return 2;
	default:
		return -1;
	}
}
stack<string>* Parser::postFix_generator(string line) {
	std::stack<string>* post_fix = new stack<string>();
	std::stack<char> operators;

	string to_push = "";
	for (std::string::iterator it = line.begin(); it != line.end(); ++it) {
		if((*it) == '\\'){
			it++;
			switch((*it)){
			default:
				to_push += "\\";
				to_push += (*it);
			}
			continue;
		}
		if ((*it) == '*') {
			if((to_push).compare("")!=0){
				post_fix->push(to_push);
				to_push = "";
			}
			post_fix->push("*");
			continue;
		}
		if ((*it) == '+') {
			if((to_push).compare("")!=0){
				post_fix->push(to_push);
				to_push = "";
			}
			post_fix->push("+");
			continue;
		}
		if (!is_operator(*it)) {
			to_push = to_push + *it;
		} else {
			if(to_push.compare("") != 0)
				post_fix->push(to_push);
			to_push = "";
			operators.push(*it);
			if (operators.empty()) {
				operators.push(*it);
			} else {
				if (*it == '(') {
					operators.push(*it);
				} else if (*it == ')') {
					while (operators.top() != '(') {

						post_fix->push(get_operator(operators.top()));
						operators.pop();
					}
					operators.pop();
				} else {
					while (get_order((operators.top())) > get_order(*it)) {
						post_fix->push(get_operator(operators.top()));
						operators.pop();
					}
					if (get_order(operators.top()) < get_order(*it))
						operators.push(*it);
				}
			}
		}
	}

	if (to_push.compare("") != 0) {
		post_fix->push(to_push);
	}

	while (!operators.empty()) {
		if (operators.top() == '.')
			post_fix->push(".");
		else if (operators.top() == '|')
			post_fix->push("|");
		operators.pop();
	}

	return post_fix;
}

Parser::~Parser() {

}
