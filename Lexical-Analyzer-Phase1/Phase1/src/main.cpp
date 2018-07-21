#include <iostream>
#include "NFA.h"
#include "DFA.h"
#include "DFAMinimizer.h";
#include <algorithm>

using namespace std;

//NFA* foo() {
//	NFA* t = new NFA();
//
//	int st = t->add_starting();
//	int x1 = t->add_node();
//	int x2 = t->add_node();
//	int x3 = t->add_node();
//	int x4 = t->add_node();
//	int acc = t->add_acceptor("a|b");
//
//	t->connect(st, x1, "");
//	t->connect(st, x2, "");
//
//	t->connect(x1, x3, "a");
//
//	t->connect(x2, x4, "b");
//
//	t->connect(x3, acc, "");
//	t->connect(x4, acc, "");
//
//	return t;
//}

NFA* xyz() {
	NFA* g1 = new NFA();

	int st = g1->add_starting();
	int x1 = g1->add_node();
	int x2 = g1->add_node();
	int x3 = g1->add_node();
	int x4 = g1->add_node();
	int x5 = g1->add_node();
	int x6 = g1->add_node();
	int x7 = g1->add_node();
	int x8 = g1->add_node();
	int x9 = g1->add_node();
	int end = g1->add_acceptor("hey");
	int end2 = g1->add_acceptor("there");
	int end3 = g1->add_acceptor("k");
	int end4 = g1->add_acceptor("hey");
	int end5 = g1->add_acceptor("there");

	g1->connect(st, x1, EPS);
	g1->connect(st, x7, EPS);
	g1->connect(x1, x2, EPS);
	g1->connect(x1, x4, EPS);
	g1->connect(x2, x3, "a");
	g1->connect(x3, x6, EPS);
	g1->connect(x4, x5, "b");
	g1->connect(x5, x6, EPS);
	g1->connect(x6, x1, EPS);
	g1->connect(x6, x7, EPS);
	g1->connect(x7, x8, "a");
	g1->connect(x8, x9, "b");
	g1->connect(x9, end, "b");
	g1->connect(x9, end2, "a");
	g1->connect(x9, end3, EPS);
	g1->connect(x9, end4, "a");
	g1->connect(x9, end5, "b");

	return g1;
}

NFA* zyx() {
	NFA* g1 = new NFA();

	int A = g1->add_starting();
	int B = g1->add_acceptor("s1");
	int D = g1->add_node();
	int C = g1->add_acceptor("s1");

	g1->connect(A, B, "0");
	g1->connect(A, C, "0");
	g1->connect(B, A, "1");
	g1->connect(B, C, "1");
	g1->connect(D, D, "1");
	g1->connect(D, B, "0");
	g1->connect(C, D, "0");
	g1->connect(C, D, "1");
	return g1;
}

NFA* foo() {
	NFA g1, g2, g3;

	int x0 = g1.add_starting();
	int x1 = g1.add_node();
	int x2 = g1.add_node();
	int x3 = g1.add_node();
	int x4 = g1.add_node();
	int x5 = g1.add_acceptor();

	int x6 = g2.add_starting();
	int x7 = g2.add_acceptor();

	int x8 = g3.add_starting();
	int x9 = g3.add_acceptor();
	int x10 = g3.add_acceptor();

	g1.connect(x0, x1, "");
	g1.connect(x0, x2, "");
	g1.connect(x1, x3, "a");
	g1.connect(x2, x4, "b");
	g1.connect(x3, x5, "");
	g1.connect(x4, x5, "");

	g2.connect(x6, x7, "b");

	g3.connect(x8, x9, "a");
	g3.connect(x8, x10, "b");

	vector<NFA*> v;
	v.push_back(&g1);
	v.push_back(&g2);
	v.push_back(&g3);

	NFA* cat = NFA::_union(v);

	return cat;
}

//set<int> set_difference(set<int> s1, set<int> s2) {
//	set<int> difference;
//	for (set<int>::iterator it = s1.begin(); it != s1.end(); it++) {
//		for (set<int>::iterator it2 = s2.begin(); it2 != s2.end(); it2++) {
//
//		}
//	}
//}

int main() {

//	NFA* t = foo();
////	t->print_debug();
//
//	vector<int> states;
//	states.push_back(11);
//	states.push_back(3);
//	states.push_back(4);
//	states.push_back(7);
//	states.push_back(9);
//	states.push_back(10);
//
//	set<int> eps = t->epsilon_closure(states);
//
//	cout << "===================================================" << endl;
//
//	for (set<int>::iterator it = eps.begin(); it != eps.end(); it++)
//		cout << *it << endl;
//
//	cout << "size: " << eps.size() << endl;

//	set<int> s1;
//	s1.insert(3);
//	s1.insert(4);
//	s1.insert(1);
//	s1.insert(2);
//	s1.insert(0);
//
//	set<int> s2;
//
//	s2.insert(4);
//	s2.insert(10);
//	s2.insert(6);
//	s2.insert(2);
//	s2.insert(8);
//
//	set<int> result;
//	set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),
//			inserter(result, result.end()));
//
//	for (set<int>::iterator it = result.begin(); it != result.end(); it++)
//		cout << *it << endl;

//	NFA* g = zyx();
	NFA* g = xyz();

	vector<string> priorities;
//	priorities.push_back("s1");
	priorities.push_back("hey");
	priorities.push_back("there");
	priorities.push_back("k");

	DFA* d = new DFA(*g, priorities);

	d->print_debug();

	//--------------- MY CODE -------------\\

	DFA* out;
	cout<<"-----------------------\n";
	DFAMinimizer::_minimize_dfa(d, out);
	out->print_debug();

	//--------------- _______ -------------\\

	return 0;
}
