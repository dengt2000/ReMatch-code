
#include"ConstructandMatch.h"
#include"suffix_array.h"
#include <string>
#include <vector>
#include<map>
#include <iostream>
#include<stack>
#include<tuple>
using namespace std;

Construct_Pb::Construct_Pb(string &w,shared_ptr<Production_FA> p)
{
	IsMatch = false;
	w1_len = w.length();
	referCount = p->referCount;
	visited.resize(referCount+2);
	for (int i = 0; i < referCount+2; i++)
	{
		visited[i].resize(w1_len + 2);
		for (int j = 0; j < w1_len + 2; j++)
			visited[i][j].resize(w1_len + 2);
	}
}

void Construct_Pb::checkmatch(vector<vector<unordered_set<int>>> &position_edges, int first, int second, int third,unordered_set<int> &groups) {
	int len_lim = position_edges[0].size();
	//cout << IsMatch << endl;
	if (IsMatch) {
		return;
	}
	//cout << first << " " << second << endl;
	if (visited[first + 1][second + 1][third])
		return;
	visited[first + 1][second + 1][third] = true;
	if (first == 0)
	{
		for (int i : groups)
		{
			if (second + i >= w1_len)
				continue;
			for (int j=0;j< position_edges[first + 1].size();j++)
			{
				for (int k : position_edges[first + 1][j])
				{
					if (second + i + k > w1_len)
						continue;
					checkmatch(position_edges, j - 1, second + i + k, i, groups);
				}
			}
		}
	}
	else {
		for (int j = 0; j < position_edges[first + 1].size(); j++)
		{
			for (int k : position_edges[first + 1][j])
			{
				if (second + third + k > w1_len)
					continue;
				if (j == 0)
				{
					if (second + third + k == w1_len)
					{
						IsMatch = true;
					}
						return;
				}
				if (visited[j][second + third + k + 1][third])
					continue;
				checkmatch(position_edges, j - 1, second + third + k, third, groups);
			}
		}
	}
	
}
	//for (auto it = edges.begin(); it != edges.end(); ++it) {

	//	int ffirst = it->first;
	//	int ssecond = it->second;
	//	if (ssecond + w1_len >= length) {
	//		continue;
	//	}
	//	//cout << ffirst << " " << ssecond   << endl;
	//	if (ffirst == -1 && ssecond == -1) {
	//		IsMatch = true;
	//		return;
	//	}
	//	if (!Pb_mark[ssecond * length + ssecond + w1_len]) {
	//		//cout << "111" <<endl;
	//		continue;
	//	}
	//	//cout << "111" << endl;
	//	if (IsMatch) {
	//		return;
	//	}
	//	checkmatch(position_edges, ffirst, ssecond + w1_len);
	//}


//bool Construct_Pb::checkahead(vector<vector<vector<pair<int, int>>>>& position_edges,int first, int second) {
//	int len_lim = position_edges[0].size();
//	if (second + 1-w1_len <0 || position_edges[first + 1][second + 1-w1_len].empty()) {
//		return false;
//	}
//	if (first > -1)
//	{
//		if (visited[first][second - w1_len][second]) {
//			return res[first][second - w1_len][second];
//		}
//		visited[first][second - w1_len][second] = true;
//	}
//	auto& edges = position_edges[first + 1][second + 1-w1_len];
//	for (auto it = edges.begin(); it != edges.end(); ++it) {
//
//		int ffirst = it->first;
//		int ssecond = it->second;
//		if (ffirst == -1 && ssecond == -1) {
//			res[first][second - w1_len][second] = true;
//			return true;
//		}
//		if (ssecond - w1_len < 0) {
//			continue;
//		}
//		//cout << ssecond << " " << ssecond + w1_len << endl;
//		if (!Pb_mark[(ssecond-w1_len) * length + ssecond]) {
//			//cout << "111" <<endl;
//			continue;
//		}
//		if (checkahead(position_edges, ffirst, ssecond))
//		{
//			if (first>-1)
//			res[first][second - w1_len][second] = true;
//			return true;
//		}
//	}
//	return false;
//}
//bool Construct_Pb::checkbehind(vector<vector<vector<pair<int, int>>>>& position_edges, int first, int second) {
//	int len_lim = position_edges[0].size();
//	if (second + 1 >= len_lim || position_edges[first + 1][second + 1].empty()) {
//		return false;
//	}
//	if (first > -1)
//	{
//		if (visited[first][second - w1_len][second]) {
//			return res[first][second - w1_len][second];
//		}
//		visited[first][second - w1_len][second] = true;
//	}
//	auto& edges = position_edges[first + 1][second + 1];
//	for (auto it = edges.begin(); it != edges.end(); ++it) {
//
//		int ffirst = it->first;
//		int ssecond = it->second;
//		if (ssecond + w1_len >= length) {
//			continue;
//		}
//		if (ffirst == -1 && ssecond == -1) {
//			if(first>-1)
//			res[first][second - w1_len][second] = true;
//			return true;
//		}
//		//cout << ssecond << " " << ssecond + w1_len << endl;
//		if (!Pb_mark[ssecond * length + ssecond + w1_len]) {
//			//cout << "111" <<endl;
//			continue;
//		}
//		if( checkbehind(position_edges,  ffirst, ssecond + w1_len))
//		{
//			if(first>-1)
//				res[first][second - w1_len][second] = true;
//			return true;
//		}
//	}
//	return false;
//}
//
//
//



