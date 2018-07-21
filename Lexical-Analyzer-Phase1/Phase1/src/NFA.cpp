#include "NFA.h"

using namespace std;

unsigned int NFA::label_counter = 0; // static variable initialized

/*
 * those functions will only work correctly if the given NFAs have
 * exactly one starting state and one accepting state
 */
NFA* NFA::_concatenate(const vector<NFA*>& gs) {

	NFA* catNFA = new NFA();

	catNFA->starting_points.insert(gs.front()->starting_points.begin(),
			gs.front()->starting_points.end());

	catNFA->acceptors.insert(gs.back()->acceptors.begin(),
			gs.back()->acceptors.end());

	for (vector<NFA*>::const_iterator it = gs.begin(); it != gs.end(); it++) {
		NFA* cur = *it;
		catNFA->input_laguage.insert(cur->input_laguage.begin(),
				cur->input_laguage.end());
		catNFA->adj_list.insert(cur->adj_list.begin(), cur->adj_list.end());
	}

	for (vector<NFA*>::const_iterator it = gs.begin(); it != gs.end() - 1;
			it++) {
		NFA* frst = *it;
		NFA* scnd = *(it + 1);

		for (map<int, string>::const_iterator it2 = frst->acceptors.begin();
				it2 != frst->acceptors.end(); it2++) {
			for (set<int>::const_iterator it3 = scnd->starting_points.begin();
					it3 != scnd->starting_points.end(); it3++) {
				catNFA->connect(it2->first, *it3, EPS);
			}
		}
	}
	return catNFA;
}

NFA* NFA::_create_lexer(const vector<NFA*>& gs) {
	NFA* lexer = new NFA();

	int lexerSt = lexer->add_starting();
	for (vector<NFA*>::const_iterator it = gs.begin(); it != gs.end(); it++) {
		NFA* cur = *it;
		lexer->input_laguage.insert(cur->input_laguage.begin(),
				cur->input_laguage.end());
		lexer->acceptors.insert(cur->acceptors.begin(), cur->acceptors.end());
		lexer->acceptors_keys.insert(cur->acceptors_keys.begin(),
				cur->acceptors_keys.end());
		lexer->adj_list.insert(cur->adj_list.begin(), cur->adj_list.end());
		for (set<int>::const_iterator it2 = cur->starting_points.begin();
				it2 != cur->starting_points.end(); it2++) {
			lexer->connect(lexerSt, *it2, EPS);
		}
	}
	return lexer;
}

NFA* NFA::_union(const vector<NFA*>& gs) {
	NFA* unionNFA = new NFA();
	int unionSt = unionNFA->add_starting();
	//todo
	string accepted = "";
	int unionAcc = unionNFA->add_acceptor(accepted);

	for (vector<NFA*>::const_iterator it = gs.begin(); it != gs.end(); it++) {
		NFA* cur = *it;
		unionNFA->input_laguage.insert(cur->input_laguage.begin(),
				cur->input_laguage.end());
		unionNFA->adj_list.insert(cur->adj_list.begin(), cur->adj_list.end());

		for (set<int>::const_iterator it2 = cur->starting_points.begin();
				it2 != cur->starting_points.end(); it2++) {
			unionNFA->connect(unionSt, *it2, EPS);
		}

		for (map<int, string>::const_iterator it2 = cur->acceptors.begin();
				it2 != cur->acceptors.end(); it2++) {
			unionNFA->connect(it2->first, unionAcc, EPS);
		}
	}
	return unionNFA;
}

NFA* NFA::_close(const NFA& g) {
	NFA* closeNFA = new NFA();

	closeNFA->adj_list.insert(g.adj_list.begin(), g.adj_list.end());

	closeNFA->input_laguage.insert(g.input_laguage.begin(),
			g.input_laguage.end());

	int closeSt = closeNFA->add_starting();
	string accepted = "";
	//todo:
	int closeAcc = closeNFA->add_acceptor(accepted);

	closeNFA->connect(closeSt, closeAcc, EPS);

	for (set<int>::const_iterator it = g.starting_points.begin();
			it != g.starting_points.end(); it++) {
		closeNFA->connect(closeSt, *it, EPS);
	}

	for (map<int, string>::const_iterator it = g.acceptors.begin();
			it != g.acceptors.end(); it++) {
		closeNFA->connect(it->first, closeAcc, EPS);
	}

	for (map<int, string>::const_iterator it = g.acceptors.begin();
			it != g.acceptors.end(); it++) {
		for (set<int>::const_iterator it2 = g.starting_points.begin();
				it2 != g.starting_points.end(); it2++) {
			closeNFA->connect(it->first, *it2, EPS);
		}
	}
	return closeNFA;
}

set<int> NFA::get_starting() {
	return starting_points;
}

map<int, string> NFA::get_acceptors() {
	return acceptors;
}

set<int> NFA::epsilon_closure(set<int> state) {
	stack<int> stack;
	set<int> epsClos;

	// insert all given states in stack
	for (set<int>::iterator it = state.begin(); it != state.end(); it++)
		stack.push(*it);

	epsClos.insert(state.begin(), state.end());

	while (!stack.empty()) {
		int top = stack.top();
		stack.pop();
		set<int>* epsilon_trans = next_states(top, EPS);
		if (epsilon_trans == nullptr)
			continue;
		for (set<int>::iterator it = epsilon_trans->begin();
				it != epsilon_trans->end(); it++) {
			if (epsClos.find(*it) == epsClos.end()) {
				epsClos.insert(*it);
				stack.push(*it);
			}
		}
	}
	return epsClos;
}

set<int>* NFA::next_states(int cur_state, string input) {
	map<int, map<string, set<int>>> ::iterator it1 = adj_list.find(cur_state);
	if (it1 != adj_list.end()) {
		map<string, set<int>>::iterator it2 = it1->second.find(input);
		if (it2 != it1->second.end()) {
			return &(it2->second);
		}
		return nullptr;
	}
	return nullptr;
}

map<string, set<int>>* NFA::get_connections(int state) {
	if (adj_list.find(state) != adj_list.end()) {
		return &(adj_list.find(state)->second);
	}
	return nullptr;
}

int NFA::add_node() {
	this->adj_list.insert(
			pair<int, map<string, set<int> > >(label_counter,
					map<string, set<int> >()));
	label_counter++;
	return label_counter - 1;
}

int NFA::add_starting() {
	this->adj_list.insert(
			pair<int, map<string, set<int> > >(label_counter,
					map<string, set<int> >()));
	this->starting_points.insert(label_counter);
	label_counter++;
	return label_counter - 1;
}

int NFA::add_acceptor(string accepted) {
	this->adj_list.insert(
			pair<int, map<string, set<int> > >(label_counter,
					map<string, set<int> >()));
	this->acceptors.insert(pair<int, string>(label_counter, accepted));
	this->acceptors_keys.insert(label_counter);
	label_counter++;
	return label_counter - 1;
}

void NFA::connect(int node1, int node2, string input) {
	map<string, set<int>>* connections = &this->adj_list.at(node1);
	// if connections with the same input already exist
	if (connections->find(input) != connections->end()) {
		// simply push the new node into the set of nodes connected to
		// node1 with the same string input
		connections->at(input).insert(node2);
	}
	// else, this is the first connection to be inserted that connects node1
	// to node2 with the given string input
	else {
		set<int> s;
		s.insert(node2);
		connections->insert(pair<string, set<int>>(input, s));
	}
	input_laguage.insert(input);
}

set<string> NFA::get_lang() {
	return input_laguage;
}

void NFA::print_debug() {

	cout << "Starting : ";
	for (set<int>::iterator it = starting_points.begin();
			it != starting_points.end(); it++) {
		cout << *it << ", ";
	}
	cout << endl;

	cout << "Accepting : ";
	for (map<int, string>::iterator it = acceptors.begin();
			it != acceptors.end(); it++) {
		cout << it->first << ":" << it->second << ", ";
	}
	cout << endl;

	for (map<int, map<string, set<int>>> ::iterator it = adj_list.begin();
	it != adj_list.end(); it++) {
		cout << it->first << ":" << endl;
		for (map<string, set<int>>::iterator it2 = it->second.begin();
		it2 != it->second.end(); it2++) {
			cout << "\t" << it2 -> first << ":";
			for (set<int>::iterator it3 = it2->second.begin(); it3 != it2->second.end();
			it3++) {
				cout << *it3 << ", ";
			}
			cout << endl;
		}
	}

}

set<int> NFA::get_acceptors_keys() {
	return acceptors_keys;
}
NFA::NFA() {
}

NFA::~NFA() {

}

