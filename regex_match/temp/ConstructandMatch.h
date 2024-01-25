#pragma once

#include<string>
#include<vector>
#include<map>
#include"Production_FA.h"
#include<set>
#include<unordered_map>
using namespace std;
class Construct_Pb
{
public:
	
	
	/*
	*
	*/
	
	
	int nextval[100000];
	vector<vector<pair<int, int>>> Pb;
	void iden(vector<pair<int, int>> &subwpair, string &w, vector<vector<pair<int, int>>> & Pb, unordered_map<pair<int, int>, int, Hashfunc, Equalfunc>& temp_Pb);

	void KMP(string &subs, string &s, vector<int>& position);
	bool IsMatch;
	unordered_map<pair<int, int>, bool, Hashfunc, Equalfunc> visited;
	unordered_map<pair<int, int>, int, Hashfunc, Equalfunc> temp_Pb;
	void get_next(string &b, int nextval[]);
	//bool checkmatch(vector<set<pair<int, int>, pair_comp>> groups, unordered_map<pair<int, int>, set<pair<int, int>, pair_comp>, Hashfunc, Equalfunc> position_edges, vector<pair<int, int>> w1);
	//bool finalmatch(vector<vector<pair<int, int>>> Pb, vector<set<pair<int, int>, pair_comp>> groups, unordered_map<pair<int, int>, set<pair<int, int>, pair_comp>, Hashfunc, Equalfunc> position_edges);
	void checkmatch(vector<vector<pair<int, int>>>& groups, unordered_map<pair<int, int>, vector<pair<int, int>>, Hashfunc, Equalfunc>& position_edges, vector<pair<int, int>>& w1, int first, int second);

	//bool w1_set_match(vector<pair<int, int>> w1, unordered_map<pair<int, int>, set<pair<int, int>, pair_comp>, Hashfunc, Equalfunc> position_edges, pair<int, int>& j1, pair<int, int>& j2, int i, unordered_map<pair<int, int>, bool, Hashfunc, Equalfunc> visited);
private:

};