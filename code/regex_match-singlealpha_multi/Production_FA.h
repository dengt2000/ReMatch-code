#pragma once
#include<set>
#include<stack>
#include"time.h"
#include"NFA_state.h"
struct VisitHashfunc {
	size_t operator() (const tuple<int, int, shared_ptr<vector<int>>>& key)const {
		
		auto seed = hash<int>()(get<0>(key));
		//seed ^= std::hash<int>()(get<1>(key));
		seed ^= std::hash<int>()(get<1>(key)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		for (auto i : *get<2>(key))
			/*seed ^= std::hash<int>()(i);*/
			seed ^= std::hash<int>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};
struct PathHashFunc {
	size_t operator() (const pair<shared_ptr<NFA_state>, int>& key) const {
		auto seed = hash<int>()(key.first->id);
		return seed ^= std::hash<int>()(key.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
};

struct PathEqualfunc {
	bool operator() (const pair<shared_ptr<NFA_state>, int>& a, const pair<shared_ptr<NFA_state>, int>& b) const {
		return a.first == b.first && a.second == b.second;
	}
};
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
	clock_t ttime;
	int groupNum;
	stack<pair<int, int>> st;
	shared_ptr<NFA_state> matchState;
	unordered_map<int, int> referNo2num; 
	const static int Normal = 0;
	const static int GroupStart = 1;
	const static int GroupEnd = 2;
	const static int ReferStart = 3;
	const static int ReferEnd = 4;
	const static int LookAround = 5;
	int  referCount = 0;
private:
	bool step(shared_ptr<NFA_state> s,  int i, shared_ptr<vector<int>> curgroup, shared_ptr<stack<pair<int, int>>> curStart, bool hasGroup, pair<shared_ptr<NFA_state>, int> curEnd);
	unordered_map<pair<shared_ptr<NFA_state>, int>, shared_ptr<vector<pair<shared_ptr<NFA_state>, int>>>, PathHashFunc, PathEqualfunc> paths;
	shared_ptr<vector<shared_ptr<NFA_state>>> clist;
	shared_ptr<vector<shared_ptr<NFA_state>>> nlist;
	shared_ptr<vector<shared_ptr<NFA_state>>> tlist;
	unordered_map<tuple<int,int,shared_ptr<vector<int>>>,bool,VisitHashfunc> visited;
	vector<int> min_t;
	string w;
	string E;
	//shared_ptr<unordered_map<int, shared_ptr<Production_State>>> cmap;
	//shared_ptr<unordered_map<int, shared_ptr<Production_State>>> nmap;
	shared_ptr<unordered_set<shared_ptr<NFA_state>>> NFA_statesets;
	//unordered_map<pair<int, int>, shared_ptr<Production_State>, Hashfunc, Equalfunc> id2state;
	//void addState(vector<NFA_state*>*& l, NFA_state* s);
	//shared_ptr<Production_State> getState(shared_ptr<unordered_map<int, shared_ptr<Production_State>>>& map, int id1, int id2);
};
