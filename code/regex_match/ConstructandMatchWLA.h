#pragma once

#include<string>
#include<vector>
#include<map>
#include"Production_FA.h"
#include"suffix_array.h"
#include<set>
#include<unordered_map>
#include<boost/dynamic_bitset.hpp>
#include "ConstructandMatch.h"
//#include<boost/dynamic_bitset.hpp>
using namespace std;

class Construct_Pb_WLA :public Construct_Pb {
public:
	shared_ptr<GroupInfo> groupInfo;
	bool inLa=false;
	bool LaMatch = false;
	int LaPos=-1;
	Construct_Pb_WLA() {}
	Construct_Pb_WLA(shared_ptr<GroupInfo> groupInfo):groupInfo(groupInfo){}
	void iden(string& w, vector<pair<int, int>>& section, vector<vector<vector<pair<int, int>>>>& position_edges, vector<vector<vector<bool>>>& groups);
	void muti_iden(string& w, vector<pair<int, int>>& section, vector<vector<vector<pair<int, int>>>>& position_edges, vector<vector<vector<bool>>>& groups, int CGs);



	vector<int> BRNo;//�ҵ�ÿ���������ö�Ӧ�ĵ�һ���������λ��
	bool checkahead(vector<vector<vector<pair<int, int>>>>& position_edges, int first, int second);
	bool checkbehind(vector<vector<vector<pair<int, int>>>>& position_edges, int first, int second);
	void getBRNo(int CGs, vector<int>groupNo);
	void checkmatch(vector<vector<vector<pair<int, int>>>>& position_edges, int first, int second);
	void checkEmptyGroup(vector<vector<vector<pair<int, int>>>>& position_edges, int first, int second);

};