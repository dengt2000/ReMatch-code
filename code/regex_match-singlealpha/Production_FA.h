#pragma once
#include<set>
#include"NFA_state.h"
class Production_FA {
public:
	bool hasRefer;
	bool res = false;
	unordered_set<int> groups;
	//unordered_map<pair<int, int>, vector<pair<int,int>>,Hashfunc, Equalfunc> position_edges;
	vector<vector<unordered_set<int>>> position_edges;
	int CGs = 0;
	vector<int> groupNo;
	shared_ptr<vector<shared_ptr<LookAroundState>>> lookarounds = make_shared<vector<shared_ptr<LookAroundState>>>();
	int groupNum;
	shared_ptr<NFA_state> matchState;
	Production_FA(string E, string w);
	const static int Normal = 0;
	const static int GroupStart = 1;
	const static int GroupEnd = 2;
	const static int ReferStart = 3;
	const static int ReferEnd = 4;
	const static int LookAround = 5;
	int  referCount = 0;
private:
	bool step(shared_ptr<NFA_state> s,  int i, int curgroup, int curStart);
	shared_ptr<vector<shared_ptr<NFA_state>>> clist;
	shared_ptr<vector<shared_ptr<NFA_state>>> nlist;
	shared_ptr<vector<shared_ptr<NFA_state>>> tlist;
	vector<vector<vector<bool>>> visited;
	vector<int> min_t;
	string w;
	string E;
	shared_ptr<unordered_set<shared_ptr<NFA_state>>> NFA_statesets;
};