#pragma once
#include<set>
#include"NFA_state.h"
/*
class Production_State{
public:
	//int group_state;
	pair<int, int> id;
	Production_State(int id1, int id2);
	Production_State(int type, int id1, int id2);
	//void setGroupState(int state);
	vector<shared_ptr<Production_State>> out;
	int isMatch = false;
	bool visited = false;
	int groupType;//0:'(',1:')'
	set<pair<int,int>,pair_comp> places;
	void addPlace(int groupno,int place);
	void addPlaces(set<pair<int, int>,pair_comp> &places);
	bool operator<(const Production_State& rhs)const { return this->id.first < rhs.id.first||
		(this->id.first==rhs.id.first && this->id.second<rhs.id.second); }
	bool operator==(const Production_State& rhs)const { return this->id.first == rhs.id.first &&
		this->id.second==rhs.id.second; }
};
class LookAround_State :public Production_State {
public:
	shared_ptr<LookAroundState> s;
	LookAround_State(shared_ptr<LookAroundState> p, int group_state, int id1, int id2);
};
/*
class Position_Node
{
private:
	int type;//0:start;1:end;2:left;3:right
	vector<Position_Node*> next;
public:
	Position_Node(int type);
	void addNext(Position_Node* p);
};
*/
class Production_FA {
public:
	bool hasRefer;
	bool res = false;
	vector<vector<vector<bool>>> groups;
	//unordered_map<pair<int, int>, vector<pair<int,int>>,Hashfunc, Equalfunc> position_edges;
	vector<vector<vector<vector<tuple<int,int,int>>>>> position_edges;
	int CGs = 0;
	//表示有几个capturegroup，0表示没有backrefer，1就是1个==我们之前做的那个，2表示有两个capture group；
	vector<int> groupNo;
	//groupNo[i]表示第i个group是属于\1还是\2.例如对于(a*)(a*).\1\2\2\1,有6个group，那么groupNo[0]=0,groupNo[1]=1,group[2]=0,groupNo[3]=1,groupNo[4]=1,groupNo[5]=0
	//shared_ptr<vector<shared_ptr<LookAroundState>>> lookaheads = make_shared<vector<shared_ptr<LookAroundState>>>();
	//shared_ptr<vector<shared_ptr<LookAroundState>>> lookbehinds = make_shared<vector<shared_ptr<LookAroundState>>>();
	shared_ptr<vector<shared_ptr<LookAroundState>>> lookarounds = make_shared<vector<shared_ptr<LookAroundState>>>();
	//shared_ptr<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>> check_behind_edges=make_shared<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>>();
	//shared_ptr<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>> check_ahead_edges = make_shared<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>>();

	//multimap<pair<int, int>, pair<int, int>,pair_comp> position_edges;
	//键为起点，结构为(组号i，终点位置j1)。
	// 值为起点出发的所有可达点集合，元素为(组号i+1，起点位置j2)
	//表示第i组以j1为终点的位置对可达到第i+1组以j2为起点的位置对
	//map<int, shared_ptr<set<int>>> groupStarts;
	//map<int, shared_ptr<set<int>>> groupEnds;
	//shared_ptr<Production_State> start;
	//shared_ptr<Production_State> matchState;
	void reverse_step(shared_ptr<NFA_state> s, char c, int i, shared_ptr<NFA_state> cur);
	void get_inner(shared_ptr<NFA_state>s);
	unordered_map<int, int> referNo2num;
	vector<int> inner;
	int groupNum;
	vector<pair<int, int>> section; //一片从小到大的连续区域
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
	void step(shared_ptr<NFA_state> s, char c, int i, shared_ptr<NFA_state> cur);
	shared_ptr<vector<shared_ptr<NFA_state>>> clist;
	shared_ptr<vector<shared_ptr<NFA_state>>> nlist;
	shared_ptr<vector<shared_ptr<NFA_state>>> tlist;
	vector<int> visited;
	vector<int> min_t;
	void addEdge(tuple<int,int,int>v1, tuple<int,int,int>v2);
	string w;
	string E;
	//shared_ptr<unordered_map<int, shared_ptr<Production_State>>> cmap;
	//shared_ptr<unordered_map<int, shared_ptr<Production_State>>> nmap;
	shared_ptr<unordered_set<shared_ptr<NFA_state>>> NFA_statesets;
	//unordered_map<pair<int, int>, shared_ptr<Production_State>, Hashfunc, Equalfunc> id2state;
	//void addState(vector<NFA_state*>*& l, NFA_state* s);
	//shared_ptr<Production_State> getState(shared_ptr<unordered_map<int, shared_ptr<Production_State>>>& map, int id1, int id2);
};
/*
struct Production_StateHash {
	size_t operator()(const shared_ptr<Production_State> &s)const
	{
		return s->id.first ^ s->id.second;
	}
};
struct Production_StateEqual {
	bool operator()(const shared_ptr<Production_State> &s1, shared_ptr<Production_State> &s2)const
	{
		return (s1->id.first == s2->id.first) && (s1->id.second == s2->id.second);
	}

};
*/