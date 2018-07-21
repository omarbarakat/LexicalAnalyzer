/*
 * Eliminator.h
 *
 *  Created on: 13 Apr 2015
 *      Author: ahmedaboemera
 */

#ifndef ELIMINATOR_H_
#define ELIMINATOR_H_

#include "Parser.h"
using namespace std;
class Eliminator{
public:
	Parser* parser;
	Eliminator(Parser* p);
	void eliminate_LF() ;
	void eliminate_LR();
	void print_grammer();
private:
	int find_index(string s);
	vector<vector<string > > replace(int i);
	void eliminate_from_updated(vector<vector<string> > replaced, int index);
};



#endif /* ELIMINATOR_H_ */
