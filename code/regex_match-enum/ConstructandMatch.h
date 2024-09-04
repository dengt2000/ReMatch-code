#pragma once

#include<string>
#include<vector>
#include<map>
#include"Production_FA.h"
#include"suffix_array.h"
#include<set>
#include<unordered_map>
//#include<boost/dynamic_bitset.hpp>
//#include<boost/dynamic_bitset.hpp>
using namespace std;


class Construct_Pb
{
public:
	
	int number = 0;
	
	/*
	*
	*/
	
	
	/*int nextval[100000];*/
	int count = 0;
	//void iden(vector<pair<int, int>> &subwpair, string &w, vector<vector<pair<int, int>>> & Pb, unordered_map<pair<int,int>,bool, Hashfunc, Equalfunc>& temp_Pb);
	void iden( string& w, vector<pair<int, int>>& section, vector<vector<vector<pair<int, int>>>>& position_edges, vector<vector<vector<int>>>&groups);
	//void KMP(string &subs, string &s, vector<pair<int,int>>& position, unordered_map<pair<int, int>, bool, Hashfunc, Equalfunc>& temp_Pb);
	bool IsMatch;
	shared_ptr<vector<shared_ptr<LookAroundState>>> lookarounds;
	int referCount;
	shared_ptr<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>> check_behind_edges;
	shared_ptr<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>> check_ahead_edges;
	void checkmatch(vector<vector<unordered_set<int>>>& position_edges, int first, int second, int third, unordered_set<int>& groups);
	Construct_Pb(string &w,shared_ptr<Production_FA> p);
	//unordered_map<pair<int, int>, bool, Hashfunc, Equalfunc> visited;
	//void get_next(string &b, int nextval[]);
	//bool checkmatch(vector<set<pair<int, int>, pair_comp>> groups, unordered_map<pair<int, int>, set<pair<int, int>, pair_comp>, Hashfunc, Equalfunc> position_edges, vector<pair<int, int>> w1);
	//bool finalmatch(vector<vector<pair<int, int>>> Pb, vector<set<pair<int, int>, pair_comp>> groups, unordered_map<pair<int, int>, set<pair<int, int>, pair_comp>, Hashfunc, Equalfunc> position_edges);

	//bool w1_set_match(vector<pair<int, int>> w1, unordered_map<pair<int, int>, set<pair<int, int>, pair_comp>, Hashfunc, Equalfunc> position_edges, pair<int, int>& j1, pair<int, int>& j2, int i, unordered_map<pair<int, int>, bool, Hashfunc, Equalfunc> visited);
private:
	//void checkmatch(vector<vector<vector<pair<int, int>>>>& position_edges, int first, int second);
	int w1_len;
	int length;
	vector<vector<vector<bool>>> visited;
	//bool checkahead(vector<vector<vector<pair<int, int>>>>& position_edges,  int first, int second);
	//bool checkbehind(vector<vector<vector<pair<int, int>>>>& position_edges,  int first, int second);
	//void Multi_checkmatch();
	vector<vector<vector<bool>>> res;
	vector<vector<int>> Pb;
	vector<vector<vector<int>>> block;
	int current_group;
	vector<int> current_PbNum;
	vector<vector<int>> left;
	vector<int> leftPosition;
	vector<pair<int, int>> catchGroupPair;
	vector<int> catchLength;
	vector<int> catchGroup;
	int lastgroup=-1;
	//void getGroupNum(unordered_map<int, int> referNo2num);
	//vector<pair<int, int>, int> visited;//<i,j>group
	//unordered_map<pair<int, pair<int, int>>, bool, pairpairHashfunc, pairpairEqualfunc> visited;
};




//unordered_map<pair<int, bool>, int, pair_hash> Map;
