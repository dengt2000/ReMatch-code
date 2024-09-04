#pragma once

#include<string>
#include<vector>
#include<map>
#include"Production_FA.h"
#include"suffix_array.h"
#include<set>
#include<unordered_map>
#include<tuple>
#include <functional>
#include<boost/dynamic_bitset.hpp>
//#include<boost/dynamic_bitset.hpp>
using namespace std;

template <typename... Args>
class Construct_Pb
{
public:
	
	int number = 0;
	
	void iden( string& w, vector<pair<int, int>>& section, vector<vector<vector<pair<int, int>>>>& position_edges, vector<vector<vector<int>>>&groups);
	//void KMP(string &subs, string &s, vector<pair<int,int>>& position, unordered_map<pair<int, int>, bool, Hashfunc, Equalfunc>& temp_Pb);
	bool IsMatch;
	shared_ptr<vector<shared_ptr<LookAroundState>>> lookarounds;
	int referCount;
	shared_ptr<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>> check_behind_edges;
	shared_ptr<vector<vector<unordered_set<shared_ptr<LookAroundState>>>>> check_ahead_edges;
	void multi_iden(string& w, vector<pair<int, int>>& section, vector<vector<vector<vector<tuple<int, int, int>>>>> &position_edges, vector<vector<vector<bool>>>& groups, int groupNum, unordered_map<int, int> &referNo2num);//��չ
	clock_t identime=0;
	clock_t matchtime=0;
	int groupNum;
	void init_data_mem();
	int length;
	void multi_check_empty_section(vector<vector<vector<vector<tuple<int, int, int>>>>>& position_edges, int first, int second, int third, unordered_map<int, int>& referNo2num);//选择性记录

private:
	void checkmatch(vector<vector<vector<pair<int, int>>>>& position_edges, int first, int second);
	int w1_len;
	

	template <typename... TArgs>
	struct Hashfunc {
		std::size_t operator()(const std::tuple<TArgs...>& tuple) const {
			std::size_t hash = 0;
			HashfuncImpl(hash, tuple);
			return hash;
		}

	private:
		template <std::size_t Index = 0>
		void HashfuncImpl(std::size_t& hash, const std::tuple<TArgs...>& tuple) const {
			if constexpr (Index < sizeof...(TArgs)) {
				const auto& value = std::get<Index>(tuple);
				hash ^= std::hash<std::decay_t<decltype(value)>>()(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
				HashfuncImpl<Index + 1>(hash, tuple);
			}
		}
	};


	template <typename... TArgs>
	struct Equalfunc {
		bool operator()(const std::tuple<TArgs...>& lhs, const std::tuple<TArgs...>& rhs) const {
			return EqualfuncImpl<0>(lhs, rhs);
		}

	private:
		template <std::size_t Index = 0>
		bool EqualfuncImpl(const std::tuple<TArgs...>& lhs, const std::tuple<TArgs...>& rhs) const {
			if constexpr (Index < sizeof...(TArgs)) {
				const auto& lhsValue = std::get<Index>(lhs);
				const auto& rhsValue = std::get<Index>(rhs);
				if (lhsValue != rhsValue) {
					return false;
				}
				return EqualfuncImpl<Index + 1>(lhs, rhs);
			}
			return true;
		}
	};




	template <typename... TArgs>
	using KeyType = std::tuple<TArgs...>;
	template <typename... TArgs>
	using VisitedType = std::unordered_map<KeyType<TArgs...>, bool,Hashfunc<TArgs...>,Equalfunc<TArgs...>>;
	VisitedType<Args...> visited;
	

	template <typename Tuple, std::size_t I>
	void assign_tuple_elements(Tuple& key, const std::vector<int>& indices) {
		
		if constexpr (I < std::tuple_size_v<Tuple> -3) {
			std::get<I>(key) = indices[I];
			assign_tuple_elements<Tuple, I + 1>(key, indices);
		}
	}

	boost::dynamic_bitset<> Pb_mark;
	bool checkahead(vector<vector<vector<pair<int, int>>>>& position_edges,  int first, int second);
	bool checkbehind(vector<vector<vector<pair<int, int>>>>& position_edges,  int first, int second);
	//void Multi_checkmatch();
	vector<vector<vector<bool>>> res;
	vector<vector<int>> Pb;
	vector<vector<vector<int>>> block;
	int current_group;
	vector<int> current_PbNum;
	unordered_map<int, bool> stateVisited;
	size_t hashFunction(const std::vector<int>& dimensions, const std::vector<int>& indices, size_t arraySize);
	size_t visitedsize;
	vector<int> dimensions;
	vector<int> indices;


	
	void multi_checkmatch2(vector<vector<vector<vector<tuple<int, int, int>>>>>& position_edges, int first, int second, int third, unordered_map<int, int>& referNo2num,int pathNum);//选择性记录


	vector<vector<int>> left;
	vector<int> leftPosition;
	vector<pair<int, int>> catchGroupPair;
	vector<int> catchLength;
	vector<int> catchGroup;





};





