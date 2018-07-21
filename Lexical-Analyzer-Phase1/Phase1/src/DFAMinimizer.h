/*
 * DFAMinimizer.h
 *
 *  Created on: Mar 13, 2015
 *      Author: omar
 */

#ifndef SRC_DFAMINIMIZER_H_
#define SRC_DFAMINIMIZER_H_

#include "DFA.h"
#include <map>
#include <utility>
#include <set>



class DFAMinimizer {
public:
	static void _minimize_dfa(DFA* in, DFA* out);
private:
	static void _reduce(map<int,map<int,set<pair<int,int> > > >* in, DFA* out);
	static void _build_map(DFA* in, map<int,map<int,set<pair<int,int> > > >* out);
};

#endif /* SRC_DFAMINIMIZER_H_ */
