#pragma once
#include<set>
#include<stack>
#include"time.h"
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
struct VisitHashfunc {
	size_t operator() (const tuple<int, int, shared_ptr<vector<int>>>& key)const {
		
		auto seed = hash<int>()(get<0>(key));
		//seed ^= std::hash<int>()(get<1>(key));
		seed ^= std::hash<int>()(get<1>(key)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);//��ϣ���������Ż�
		for (auto i : *get<2>(key))
			/*seed ^= std::hash<int>()(i);*/
			seed ^= std::hash<int>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};
struct PathHashFunc {
	size_t operator() (const pair<shared_ptr<NFA_state>, int>& key) const {
		auto seed = hash<int>()(key.first->id);
		return seed ^= std::hash<int>()(key.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);;//��ϣ���������Ż�
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
	//��ʾ�м���capturegroup��0��ʾû��backrefer��1����1��==����֮ǰ�����Ǹ���2��ʾ������capture group��
	vector<int> groupNo;
	//groupNo[i]��ʾ��i��group������\1����\2.�������(a*)(a*).\1\2\2\1,��6��group����ôgroupNo[0]=0,groupNo[1]=1,group[2]=0,groupNo[3]=1,groupNo[4]=1,groupNo[5]=0
	//shared_ptr<vector<shared_ptr<LookAroundState>>> lookaheads = make_shared<vector<shared_ptr<LookAroundState>>>();
	//shared_ptr<vector<shared_ptr<LookAroundState>>> lookbehinds = make_shared<vector<shared_ptr<LookAroundState>>>();
	shared_ptr<vector<shared_ptr<LookAroundState>>> lookarounds = make_shared<vector<shared_ptr<LookAroundState>>>();
	//shared_ptr<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>> check_behind_edges=make_shared<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>>();
	//shared_ptr<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>> check_ahead_edges = make_shared<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>>();
	clock_t ttime;
	//multimap<pair<int, int>, pair<int, int>,pair_comp> position_edges;
	//��Ϊ��㣬�ṹΪ(���i���յ�λ��j1)��
	// ֵΪ�����������пɴ�㼯�ϣ�Ԫ��Ϊ(���i+1�����λ��j2)
	//��ʾ��i����j1Ϊ�յ��λ�öԿɴﵽ��i+1����j2Ϊ����λ�ö�
	//map<int, shared_ptr<set<int>>> groupStarts;
	//map<int, shared_ptr<set<int>>> groupEnds;
	//shared_ptr<Production_State> start;
	//shared_ptr<Production_State> matchState;
	//void reverse_step(shared_ptr<NFA_state> s, char c, int i, shared_ptr<NFA_state> cur);
	//void get_inner(shared_ptr<NFA_state>s);
	int groupNum;
	stack<pair<int, int>> st;
	shared_ptr<NFA_state> matchState;
	//map<int, int> id2num; //referID ��Ӧ��captureGroup
	unordered_map<int, int> referNo2num; //referNo������group�� ��Ӧ��captureGroup
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