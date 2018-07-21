#ifndef SRC_DFA_H_
#define SRC_DFA_H_

#include "NFA.h"

class DFA {
public:
	DFA();

	DFA(NFA& nfa, vector<string> priorities);

	void print_debug();

	int acceptors_tie_breaker(set<int> nfa_acceptors);

	int add_node(set<int> nfa_states); // adds a regular node in the graph

	int add_node(); // adds a regular node in the graph

	int add_starting(); // adds a starting node to the graph
	//todo

	int add_acceptor(string accepted = "");

	void connect(int node1, int node2, string input);

	map<string, int>* get_connections(int state);

	vector<set<int>> get_acceptors_classes();

	vector<string> get_acceptors_classes_values();

	int get_starting();

	bool is_acceptor(int state);

	string get_accepted_string(int state);

	~DFA();
private:
	NFA nfa;
	vector<string> priorities;
	static unsigned int label_counter;
	int starting;
	map<int, set<int>> d_states;
	map<int, map<string, int>> adj_list;

	/* new functions - 15/3 @ 7PM */
	set<int>* find_acceptor_set(map<int, string>::iterator p);
	void classify_states();
	vector<set<int>> states_classes;
	vector<string> states_classes_values;
	/*****************************/

	void subset_construct(vector<string> priorities);
	int exists(set<int> u);
	int get_first_unvisited_state(set<int> visited);
	set<int> move(set<int> nfa_states, string in);
	set<int> acceptors_keys;
	map<int, string> acceptors;
};

#endif /* SRC_DFA_H_ */
