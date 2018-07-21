/*
 * Parser.h
 *
 *  Created on: 12 Apr 2015
 *      Author: ahmedaboemera
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <map>

using namespace std;
class Parser {
public:
	Parser();
	string starting;
	vector<string>* terminals;
	vector<string>* non_terminals;
	vector<vector<vector<string> > >* non_terminal_defs;
	void get_terminals_and_nonterminals(vector<string>* lines);
	bool is_terminal(string s);
	bool is_non_terminal(string s);
	string get_starting();
	map<string,map<string, vector<string>>> built_predictive_table();
	vector<vector<string> > get_def(string s);
private:
	Utilities util;
	void fill_map(vector<string>* lines);
	void finalize(vector<vector<string> > non_t_defs);
	vector<string> split(string s, int start, char regex);
	int get_eq(string s);
	vector<string>* get_terminals(string s);
	string get_nonterminal(string s);
	string trim(string x);
};
#endif /* PARSER_H_ */
