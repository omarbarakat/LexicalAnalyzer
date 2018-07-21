#include "DFA.h"

unsigned int DFA::label_counter = 0; // static variable initialized

DFA::DFA() {
	this->starting = label_counter;
}

DFA::DFA(NFA& nfa, vector<string> priorities) {
	this->nfa = nfa;
	this->priorities = priorities;
	starting = label_counter;
	this->subset_construct(priorities);
}
/*
 * Operates on acceptors map
 * for each given pair, return the index of the set<int> in the acceptors_classes
 * 		that contains states that accept the same string in the pair
 * if none of the sets in the acceptors_classes contains states that accept the
 * 		same string as the given pair, return -1 as an indication that a new
 * 		set should be pushed in acceptors_classes
 * */
set<int>* DFA::find_acceptor_set(map<int, string>::iterator p) {
	for (vector<set<int>>::iterator it = states_classes.begin();
			it != states_classes.end(); it++) {
		/* it points to a set<int>, we need to verify whether the states in this set accepts the same
		 * string as the one encapsulated in p
		 * */
		set<int> cur_set = *it;
		int first_elem_in_cur_set = *(cur_set.begin());
		if (acceptors.find(first_elem_in_cur_set)->second.compare(p->second)
				== 0) // cur_set accepts the same string
			return &(*it);
	}
	/* if none of the classes contain states that accept this string, return nullptr as
	 * an indication that a new set (class) is to be created for this accepted string
	 * */
	return nullptr;
}

void DFA::classify_states() {
	/*
	 * for each acceptor in the DFA, we need to check whether the other acceptors that accept the same string
	 * as it does exist in a predefined class or not
	 * */
	for (map<int, string>::iterator it = acceptors.begin();
			it != acceptors.end(); it++) {
		set<int>* acceptor_set = find_acceptor_set(it);
		if (acceptor_set == nullptr) { // class was not previously declared as a set; create one and push it
			set<int> s;
			s.insert(it->first);
			states_classes.push_back(s);
			states_classes_values.push_back(it->second);
		} else { // class was already declared as a set, acceptor_set now points to it
			acceptor_set->insert(it->first);
		}
	}

	/*
	 * must create another class for the unaccepted states
	 * */
	set<int> unaccepted;
	for (unsigned int i = 0; i < label_counter; i++) {
		if (acceptors.find(i) == acceptors.end())
			unaccepted.insert(i);
	}
	states_classes.push_back(unaccepted);
}

vector<set<int>> DFA::get_acceptors_classes() {
	return states_classes;
}

int DFA::exists(set<int> u) {
	for (map<int, set<int>>::iterator entry = d_states.begin();
			entry != d_states.end(); entry++) {
		if (u == entry->second) {
			return entry->first;
		}
	}
	return -1;
}

vector<string> DFA::get_acceptors_classes_values() {
	return states_classes_values;
}

set<int> DFA::move(set<int> nfa_states, string in) {
	set<int> next;
	for (set<int>::iterator it = nfa_states.begin(); it != nfa_states.end();
			it++) {
		set<int>* res = nfa.next_states(*it, in);
		if (res != nullptr)
			next.insert(res->begin(), res->end());
	}
	return next;
}

bool DFA::is_acceptor(int state) {
	return acceptors.find(state) != acceptors.end();
}

string DFA::get_accepted_string(int state) {
	if (acceptors.find(state) != acceptors.end())
		return acceptors.find(state)->second;
	return "";
}
int DFA::get_starting() {
	return starting;
}

int DFA::get_first_unvisited_state(set<int> visited) {
	for (map<int, set<int>>::iterator it = d_states.begin();
			it != d_states.end(); it++) {
		if (visited.find(it->first) == visited.end())
			return it->first;
	}
	return -1;
}

void DFA::subset_construct(vector<string> priorities) {
	set<string> lang = nfa.get_lang();

	set<int> inits = nfa.epsilon_closure(nfa.get_starting());
	starting = add_node(inits);

	int current_state;
	set<int> visited;

	while (true) {
		current_state = get_first_unvisited_state(visited);

		if (current_state == -1) {
			break;
		}

		visited.insert(current_state);

		for (set<string>::iterator it = lang.begin(); it != lang.end(); it++) {
			if (it->compare(EPS) == 0) {
				continue;
			}

			set<int> u = nfa.epsilon_closure(
					move(d_states.at(current_state), *it));

			int dfa_state_id = exists(u);

			if (dfa_state_id == -1) {
				dfa_state_id = add_node(u);
			}
			connect(current_state, dfa_state_id, *it);
		}
	}
	classify_states();
}

void DFA::connect(int node1, int node2, string input) {
	map<string, int>* connections = &this->adj_list.at(node1);
	connections->insert(pair<string, int>(input, node2));
}

map<string, int>* DFA::get_connections(int state) {
	map<int, map<string, int>>::iterator map_it = adj_list.find(state);
	if (map_it == adj_list.end())
		return nullptr;
	return &(map_it->second);
}

// returns the label of the nfa state with the highest acceptor priority
int DFA::acceptors_tie_breaker(set<int> nfa_acceptors) {
	for (vector<string>::iterator it = priorities.begin();
			it != priorities.end(); it++) {
		// do we have a match??
		for (set<int>::iterator it2 = nfa_acceptors.begin();
				it2 != nfa_acceptors.end(); it2++) {
			string acceptor_string = nfa.get_acceptors().find(*it2)->second;
			if (it->compare(acceptor_string) == 0)
				return *it2;
		}
	}

	// non of the nfa_acceptors are actually acceptors!!!!!!!
	return -1;
}

int DFA::add_starting() {
	this->adj_list.insert(
			pair<int, map<string, int>>(label_counter,
					map<string, int>()));
	this->starting = label_counter;
	label_counter++;
	return label_counter - 1;
}

int DFA::add_acceptor(string accepted) {
	this->adj_list.insert(
			pair<int, map<string, int>>(label_counter,
					map<string, int>()));
	this->acceptors.insert(pair<int, string>(label_counter, accepted));
	this->acceptors_keys.insert(label_counter);
	label_counter++;
	return label_counter - 1;
}

int DFA::add_node() {
	this->adj_list.insert(
			pair<int, map<string, int>>(label_counter, map<string, int>()));
	this->d_states.insert(pair<int, set<int>> (label_counter, set<int>()));
	label_counter++;
	return label_counter - 1;
}

int DFA::add_node(set<int> nfa_states) {
	set<int> nfa_acceptors;
	set_intersection(nfa_states.begin(), nfa_states.end(),
			this->nfa.get_acceptors_keys().begin(),
			this->nfa.get_acceptors_keys().end(),
			inserter(nfa_acceptors, nfa_acceptors.end()));
	d_states.insert(pair<int, set<int>>(label_counter, nfa_states));
	adj_list.insert(
			pair<int, map<string, int>>(label_counter, map<string, int>()));
	if (!nfa_acceptors.empty()) {
		acceptors_keys.insert(label_counter);
		if (nfa_acceptors.size() > 1) { // more than one NFA acceptor are included
										// in this DFA state, must used tiebreaker
			int highest_priority_nfa = acceptors_tie_breaker(nfa_acceptors);
			string highest_priority_nfa_string = nfa.get_acceptors().find(
					highest_priority_nfa)->second;
			acceptors.insert(
					pair<int, string>(label_counter,
							highest_priority_nfa_string));
		} else {
			string nfa_acceptor_string = nfa.get_acceptors().find(
					*(nfa_acceptors.begin()))->second;
			acceptors.insert(
					pair<int, string>(label_counter, nfa_acceptor_string));
		}
	}
	label_counter++;
	return label_counter - 1;
}

void DFA::print_debug() {

	cout << "Starting: " << this->starting << endl;
	cout << "Accepting:";
	for (set<int>::iterator it = acceptors_keys.begin();
			it != acceptors_keys.end(); it++) {
		cout << " <" << *it << "," << acceptors.find(*it)->second << ">";
	}
	cout << endl;
	cout << "Classified in the following classes: " << endl;
	unsigned int i = 0;
	for (vector<set<int>>::iterator it = states_classes.begin();
			it != states_classes.end(); it++, i++) {
		cout << "Class:";
		for (set<int>::iterator it2 = it->begin(); it2 != it->end(); it2++) {
			cout << " " << *it2;
		}
		if (i < states_classes_values.size())
			cout << " Accepts: " << states_classes_values[i];
		cout << endl;
	}
	for (map<int, map<string, int>>::iterator it = adj_list.begin();
			it != adj_list.end(); it++) {
		cout << it->first << ":" << endl;
		for (map<string, int>::iterator it2 = it->second.begin();
				it2 != it->second.end(); it2++) {
			cout << "\t'" << it2->first << "':" << it2->second << endl;
		}
	}

}

DFA::~DFA() {
}

