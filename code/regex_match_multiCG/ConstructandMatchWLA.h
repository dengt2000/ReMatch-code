#pragma once

#include<string>
#include<vector>
#include<map>
#include"Production_FA.h"
#include"suffix_array.h"
#include"ConstructandMatch.h"
#include<set>
#include<unordered_map>
#include<tuple>
#include <functional>
#include<boost/dynamic_bitset.hpp>
//#include<boost/dynamic_bitset.hpp>
using namespace std;

template <typename... Args>
class Construct_Pb_WLA:public Construct_Pb<Args...>
{
public:
	bool inLa;
	int LaPos;
	bool LaMatch = false;
	void multi_iden(string& w, vector<pair<int, int>>& section, vector<vector<vector<vector<tuple<int, int, int>>>>>& position_edges, vector<vector<vector<bool>>>& groups, int groupNum, unordered_map<int, int>& referNo2num);
	void init_data_mem();
	void multi_check_empty_section(vector<vector<vector<vector<tuple<int, int, int>>>>>& position_edges, int first, int second, int third, unordered_map<int, int>& referNo2num);




	void multi_checkmatch2(vector<vector<vector<vector<tuple<int, int, int>>>>>& position_edges, int first, int second, int third, unordered_map<int, int>& referNo2num);//选择性记录


	vector<vector<int>> left;
	vector<int> leftPosition;
	vector<pair<int, int>> catchGroupPair;
	vector<int> catchLength;
	vector<int> catchGroup;

	

	template <typename Tuple, std::size_t I>
	void assign_tuple_elements(Tuple& key, const std::vector<int>& indices) {
		
		if constexpr (I < std::tuple_size_v<Tuple> -2) {
			std::get<I>(key) = indices[I];
			assign_tuple_elements<Tuple, I + 1>(key, indices);
		}
	}
};





