#pragma once
#include<set>
#include"NFA_state.h"
class Production_FA {
public:
	bool hasRefer;
	bool res = false;
	vector<vector<vector<bool>>> groups;
	vector<vector<vector<vector<tuple<int,int,int>>>>> position_edges;
	int CGs = 0;
	vector<int> groupNo;
	shared_ptr<vector<shared_ptr<LookAroundState>>> lookarounds = make_shared<vector<shared_ptr<LookAroundState>>>();
	unordered_map<int, shared_ptr<NFA_state>> group_start_state;
	void reverse_step(shared_ptr<NFA_state> s, char c, int i, shared_ptr<NFA_state> cur);
	void get_inner(shared_ptr<NFA_state>s);
	unordered_map<int, int> referNo2num;
	vector<int> inner;
	int groupNum;
	vector<pair<int, int>> section; 
	shared_ptr<NFA_state> matchState;
	Production_FA(string E, string w);
	//bool ismatch(shared_ptr<NFA_state> s, int i, shared_ptr<Production_State> curstate);
	const static int Normal = 0;
	const static int GroupStart = 1;
	const static int GroupEnd = 2;
	const static int ReferStart = 3;
	const static int ReferEnd = 4;
	const static int LookAround = 5;
	int  referCount = 0;
private:
	unordered_map<int,unordered_map<int, vector<int>>> group_maps;
	vector<shared_ptr<NFA_state>> group_start_states;
	unordered_map<int, shared_ptr<NFA_state>> group_start_states_map;
	vector<unordered_set<shared_ptr<NFA_state>>> referEndMap;
	unordered_map<int, bool> group_can_be_empty;
	void search_group_maps(string &w);
	void search_group_map(shared_ptr<NFA_state> group_start_state, string& w);
	void simple_step(shared_ptr<NFA_state> s, char c, int i, shared_ptr<NFA_state> cur);
	void step(shared_ptr<NFA_state> s, char c, int i, shared_ptr<NFA_state> cur);
	shared_ptr<vector<shared_ptr<NFA_state>>> clist;
	shared_ptr<vector<shared_ptr<NFA_state>>> nlist;
	shared_ptr<vector<shared_ptr<NFA_state>>> tlist;
	vector<int> visited;
	vector<bool> group_visited;
	vector<int> min_t;
	void addEdge(tuple<int,int,int>v1, tuple<int,int,int>v2);
	string w;
	string E;
	shared_ptr<unordered_set<shared_ptr<NFA_state>>> NFA_statesets;
	};
