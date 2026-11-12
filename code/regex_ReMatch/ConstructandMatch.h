#pragma once

#include<string>
#include<vector>
#include<map>
#include"Production_FA.h"
#include"suffix_array.h"
#include<set>
#include<unordered_map>
#include<boost/dynamic_bitset.hpp>
//#include<boost/dynamic_bitset.hpp>
using namespace std;


class Construct_Pb
{
public:
	
	int number = 0;
	
	virtual void iden( string& w, vector<pair<int, int>>& section, vector<vector<vector<pair<int, int>>>>& position_edges, vector<vector<vector<bool>>>&groups);
	virtual void muti_iden(string& w, vector<pair<int, int>>& section, vector<vector<vector<pair<int, int>>>>& position_edges, vector<vector<vector<bool>>>& groups,int CGs);
	bool IsMatch;
	shared_ptr<vector<shared_ptr<LookAroundState>>> lookarounds;
	int referCount;
	

	shared_ptr<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>> check_behind_edges;
	shared_ptr<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>> check_ahead_edges;


	vector<int> BRNo;//�ҵ�ÿ���������ö�Ӧ�ĵ�һ���������λ��
	virtual bool checkahead(vector<vector<vector<pair<int, int>>>>& position_edges,  int first, int second);
	virtual bool checkbehind(vector<vector<vector<pair<int, int>>>>& position_edges,  int first, int second);
	virtual void getBRNo(int CGs,vector<int>groupNo);
	vector<vector<vector<bool>>> res;
	vector<vector<vector<bool>>> visited;//visited[group][i][j]
	int w1_len;
	int length;
	boost::dynamic_bitset<> Pb_mark;
	virtual void checkmatch(vector<vector<vector<pair<int, int>>>>& position_edges, int first, int second);
	virtual void checkEmptyGroup(vector<vector<vector<pair<int, int>>>>& position_edges, int first, int second);
};


