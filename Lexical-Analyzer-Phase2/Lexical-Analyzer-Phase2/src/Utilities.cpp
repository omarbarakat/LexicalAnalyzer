
#include "Utilities.h"


vector<string>::iterator Utilities::vector_contains(vector<string> vec, string str){
	for(vector<string>::iterator s=vec.begin(); s!=vec.end(); s++){
		if(s->compare(str) == 0)
			return s;
	}
	return vec.end();
}


vector<string> Utilities::get_first(string non_terminal){

	vector<string> firsts;
	vector<vector<string> > sop = Parser.get_non_terminal_def(non_terminal);

	for(vector<string> mul_terms : sop){

		vector<string>::iterator term_it = mul_terms.begin();
		for(; term_it!=mul_terms.end(); term_it++){
			if(is_terminal(*term_it)){
				firsts.push_back(*term_it);
			}
			else{
				vector<string> term_first = get_first(*term_it);
				vector<string>::iterator it = vector_contains(term_first, "\L");

				if(it != term_first.end()){
					term_first.erase(it);
					firsts.insert(firsts.end(), term_first.begin(), term_first.end());
				}else{
					firsts.insert(firsts.end(), term_first.begin(), term_first.end());
					break;
				}
			}
		}
		if(term_it == mul_terms.end() && vector_contains(firsts, "\L")==firsts.end())
			firsts.push_back("\L");
	}
}


map<string, vector<string>> Utilities::compute_first(vector<string> non_terminals){

	map<string, vector<string> > non_terminals_first;

	for(string s : non_terminals){
		non_terminals_first[s] = get_first(s);

	}
}
