#ifndef SRC_PARSER_H
#define SRC_PARSER_H

#include <iostream>
#include <stack>
#include <string>

using namespace std;

class Parser{
	public :
		Parser();
		stack<string>* postFix_generator(string line);
		~Parser();
	private :
		bool is_operator(char c);
		int get_order(char c);
		string get_operator(char c);
};

#endif
