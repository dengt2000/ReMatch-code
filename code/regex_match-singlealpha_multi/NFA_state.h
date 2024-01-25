#pragma once
#include<unordered_map>
#include<unordered_set>
#include<vector>
#include<string>
#include<algorithm>
#include<memory>
#include<bitset>
//#include<boost/dynamic_bitset.hpp>
extern int wlen;
using namespace std;

struct pair_comp {
	bool operator() (const std::pair<int, int>& A, const std::pair<int, int>& B)const {
		if (A.first < B.first) {
			return true;
		}
		else if (A.first == B.first) {
			return A.second < B.second;
		}
		else {
			return false;
		}
	}
};
struct Hashfunc {
	size_t operator() (const pair<int, int>& key) const {
		auto seed = hash<int>()(key.first);
		return seed ^= std::hash<int>()(key.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);;//哈希函数可以优化
	}
};
struct Tuplefunc {
	size_t operator() (const tuple<int, int,int>& key) const {
		auto seed = hash<int>()(get<0>(key));
		seed ^= std::hash<int>()(get<1>(key)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);//哈希函数可以优化
		return seed ^= hash<int>()(get<2>(key)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
};
struct Equalfunc {
	bool operator() (const pair<int, int>& a, const pair<int, int>& b) const {
		return a.first == b.first && a.second == b.second;
	}
};

//表示NFA的状态
class Pattern;
class NFA_state {
public:
	int look_aroundno = -1;
	int id;
	int lastlist1;
	NFA_state(int c);
	NFA_state(char c);
	NFA_state(int c, shared_ptr<NFA_state> out1, shared_ptr<NFA_state> out2);
	NFA_state(char c, shared_ptr<NFA_state> out1, shared_ptr<NFA_state> out2);
	virtual string toString();
	bool operator<(const NFA_state& rhs)const { return this->id < rhs.id; }
	bool operator==(const NFA_state& rhs)const { return this->id == rhs.id; }
	void setGroupstate(int i);
	void setReferstate(int i);
	shared_ptr<unordered_set<tuple<int, int,int>, Tuplefunc>> cur_set;
	shared_ptr<unordered_set<tuple<int, int,int>, Tuplefunc>> next_set;
	int groupType;
	int groupNo;
	int c;
	vector<shared_ptr<NFA_state>> reverse_out;
	shared_ptr<NFA_state> out1;
	shared_ptr<NFA_state> out2;
	int group_state = 0;//  0:normal; 1:(; 2:); 
	int reference_state = 0;//1:refernece start;2: reference end;
	int referNo = -1;
	bool lock = false;
	static int count;
private:
	const static int Split = 256;
};
struct stateptrCmp
{
	bool operator()(const shared_ptr<NFA_state>& s1, const shared_ptr<NFA_state>& s2)const;
};
class ScaleState :public NFA_state {
public:
	bitset<256> scale;
	ScaleState(bitset<256> s);
	string toString();
};

class LookAroundState :public NFA_state {
public:
	bool ahead; //true:lookahead;false:lookback;
	bool positive; //true:positive;false:negtive;
	int startNo;
	bool hasRefer;
	shared_ptr<NFA_state> startState;
	shared_ptr<NFA_state> matchState;
	unordered_map<int, bool> result;
	unordered_map<pair<int, int>, bool, Hashfunc, Equalfunc> visited;
	unordered_map<int, bool> revered_visited;

	//parameters for has_refer condition
	//vector<vector<pair<int,int>>> start_positions;
	vector<int> start_positions;
	shared_ptr<vector<shared_ptr<NFA_state>>> clist;
	shared_ptr<vector<shared_ptr<NFA_state>>> nlist;
	shared_ptr<vector<shared_ptr<NFA_state>>> tlist;
	vector<int> refer_visited;
	shared_ptr<unordered_set<shared_ptr<NFA_state>>> NFA_statesets = make_shared<unordered_set<shared_ptr<NFA_state>>>();
	vector<vector<vector<vector<tuple<int, int,int>>>>> position_edges;

	void reverse(shared_ptr<NFA_state>& start);
	bool match(string s, int i);
	bool reverse_dfs(string s, int i, shared_ptr<NFA_state>& cur_state);
	bool dfs(string s, int i, shared_ptr<NFA_state>& cur_state);

	//methods for refer conditions
	void refer_reverse_bfs(string s, vector<int>& min_t, vector<vector<vector<bool>>>& groups, int referCount);
	void refer_bfs(string s, vector<int>& min_t, vector<vector<vector<bool>>>& groups, int referCount);
	void step(shared_ptr<NFA_state> s, char c, int i, shared_ptr<NFA_state> cur);
	void reverse_step(shared_ptr<NFA_state> s, char c, int i, shared_ptr<NFA_state> cur);
	void addEdge(tuple<int, int, int> v1, tuple<int, int, int> v2);

	LookAroundState(bool ahead, bool positive, shared_ptr<NFA_state>& startState, shared_ptr<NFA_state>& matchState, bool hasRefer = false);
};

class NfaList
{
public:
	vector<shared_ptr<NFA_state>>ss;
	NfaList(vector<shared_ptr<NFA_state>> states);
	bool operator<(const NfaList& o)const;

};
struct NfalistCmp
{
	bool operator()(const shared_ptr<NfaList>& n1, const shared_ptr<NfaList>& n2)const;
};
class DState
{
public:
	shared_ptr<NfaList> nfalist;
	shared_ptr<DState> next[256];
	shared_ptr<DState> left;
	shared_ptr<DState> right;
	DState(shared_ptr<NfaList> list);
	DState() {}
};

class Frag
{
public:
	shared_ptr<NFA_state> start;
	vector<shared_ptr<NFA_state>> out1;
	vector<shared_ptr<NFA_state>> out2;
	Frag(shared_ptr<NFA_state> s, vector<shared_ptr<NFA_state>> o);
	Frag(shared_ptr<NFA_state> s);
	Frag(shared_ptr<NFA_state> s, vector<shared_ptr<NFA_state>> o1, vector<shared_ptr<NFA_state>>o2);
	string toString();
};

class Hasher
{
public:
	size_t operator()(const shared_ptr<NFA_state>& rhs)const;
	size_t operator()(const shared_ptr<NfaList>& rhs)const;
};
