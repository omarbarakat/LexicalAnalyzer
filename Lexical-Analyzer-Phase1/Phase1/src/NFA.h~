/*
 * NFA.h
 *
 *  Created on: Mar 9, 2015
 *      Author: karim
 */

#ifndef SRC_NFA_H_
#define SRC_NFA_H_

#include <string>
#include <set>
#include <map>

using namespace std;

class Acceptor {
public:
	Acceptor(int id, string accepted_expression) {
		this->id = id;
		this->accepted_expr = accepted_expression;
	}
	int get_id() {
		return this->id;
	}
	string get_accepted_expression() {
		return this->accepted_expr;
	}
	~Acceptor() {}
private:
	int id;
	string accepted_expr;
};

class NFA {
public:
	NFA();
	static NFA* _concatenate(NFA* g1, NFA* g2);
	static NFA* _union(NFA* g1, NFA* g2);
	static NFA* _close(NFA* g1, NFA* g2);
	static const unsigned char STARTING = 0;
	static const unsigned char REGULAR = 1;
	static const unsigned char ACCEPTOR = 2;
	void add_node(); // adds a regular node in the graph
	void add_starting(); // adds a starting node to the graph
	void add_acceptor(string accepted_expression);
	void connect(int node1, int node2, string input);
	void print_adj_list();
	~NFA();
private:
	static unsigned int label_counter;
	set<int>* starting_points;
	set<Acceptor*>* acceptors;
	map<int, map<string, set<int>*>*>* adj_list;
};

#endif /* SRC_NFA_H_ */
