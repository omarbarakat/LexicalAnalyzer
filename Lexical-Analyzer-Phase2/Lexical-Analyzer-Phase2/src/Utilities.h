
#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <vector>
#include <map>

using namespace std;


class Utilities{
	public:
		map<string, vector<string>> compute_first(vector<string> non_terminal);
		map<string, vector<string>> compute_follow(vector<vector<vector<string> > > non_terminal_defs);
	private:
		vector<string>::iterator vector_contains(vector<string> vec, string str);
		vector<string> get_first(string non_terminal);
};


#endif  UTILITIES_H_
