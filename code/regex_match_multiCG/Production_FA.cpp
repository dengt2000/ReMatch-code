#include"Production_FA.h"
#include "Parser.h"
#include <queue>
using namespace std;
Production_FA::Production_FA(string E, string w) {
	wlen = w.length();
	shared_ptr<Parser>p = make_shared<Parser>(E);
	referCount = p->referCount;
	hasRefer = p->hasRefer;
	referNo2num = p->referNo2num;
	groupNum = p->groupCount;
	group_start_states = p->group_start_states;
	group_start_states_map = p->group_start_states_map;
	lookarounds = p->lookarounds;
	shared_ptr<Regex> re = p->re;
	clist = make_shared<vector<shared_ptr<NFA_state>>>();
	this->w = w;
	this->E = E;
	position_edges.resize(referCount + lookarounds->size() + 2);
	for (int i = 0; i < referCount + lookarounds->size() + 2; i++)
	{
		position_edges[i].resize(w.length() + 3);
		for (int j = 0; j < w.length() + 3; j++)
			position_edges[i][j].resize(2);
	}
	nlist = make_shared<vector<shared_ptr<NFA_state>>>();
	tlist = make_shared<vector<shared_ptr<NFA_state>>>();
	NFA_statesets = make_shared<unordered_set<shared_ptr<NFA_state>>>();
	int w_length = w.length();
	
	re->start->cur_set = make_shared<unordered_set<tuple<int, int,int>, Tuplefunc>>();
	re->start->referNo = -1;
	re->start->groupType = 1;
	matchState = p->matchState;
	matchState->referNo = referCount + 1;
	matchState->groupType = 0;
	visited.resize(matchState->id + 1);
	inner.resize(matchState->id + 1);
	vector<pair<int, int>> interval;
	bool first = true;

	if (hasRefer)
	{
		referEndMap.resize(w.length() + 1);
		min_t.resize(w.length() + 1);
		
		fill(min_t.begin(), min_t.end(), INT_MAX);
		search_group_maps(w);
	}
	

	get_inner(matchState);
	
	clist->emplace_back(re->start);
	for (int i = 0; i <= w.length(); i++)
	{
		char c = ' ';
		if (i < w.length())
			c = w[i];
		std::fill(visited.begin(), visited.end(), 0);
		if (clist->size())
		{
			for (auto& cur : *clist)
			{
				step(cur->out1, c, i, cur);
				step(cur->out2, c, i, cur);
			}
		}
		if (hasRefer)
		{
			for (auto& cur : referEndMap[i])
			{
				step(cur, c, i, cur);
			}
			for (auto& node : *NFA_statesets)
			{
				switch (node->c)
				{
				case Type::LookAround:
				{
					auto la = dynamic_pointer_cast<LookAroundState>(node);
					auto pair = make_pair(referCount + la->look_aroundno, i);
					la->start_positions.emplace_back(i);
					for (auto t : *node->next_set)
					{
						if (get<0>(t) > -1)
						{
							//(*check_behind_edges)[t.first + 1][t.second + 1].insert(la);
							addEdge(make_tuple(get<0>(t), get<1>(t), get<2>(t)), make_tuple(pair.first, pair.second, 0));
						}
					}
					break;
				}
				case Type::GroupStart:
				case Type::ReferStart:
				{
					addEdge(make_tuple(-1, -1, 1), make_tuple(node->referNo, i, 0));
					int nodeGroupNo = referNo2num[node->referNo];
					auto group_map = group_maps[nodeGroupNo];
					auto it = group_map.find(i);
					if (it != group_map.end()) {
						auto pair = make_pair(node->referNo, i);
						for (auto t:*node->next_set)
						{
							addEdge(make_tuple(get<0>(t), get<1>(t), get<2>(t)), make_tuple(node->referNo,i,0));
						}
						for (int j : it->second) {
							if (j > i) {
								referEndMap[j].insert(node->out1);
							}
						}
					}
					node->next_set->clear();
					node->cur_set->clear();
					break;
				}
				case Type::GroupEnd:
				{
					for (auto t : *node->next_set)
					{
						if (get<1>(t) < min_t[i] && get<1>(t) >= 0)
							min_t[i] = get<1>(t);
						addEdge(make_tuple(get<0>(t),get<1>(t), get<2>(t)), make_tuple(node->referNo,i-1,1));
					}
					node->next_set->clear();
					node->cur_set->clear();
					break;
				}
				case Type::ReferEnd:
				{
					for (auto t : *node->next_set)
					{
						if (get<1>(t) < min_t[i]&& get<1>(t)>=0)
							min_t[i] = get<1>(t);
						//groups[node->referNo][get<1>(t)][i]=1;//����ҿ�
					}
					node->next_set->clear();
					node->cur_set->clear();
					break;
				}
				case Type::MatchState:
				{
					if (i == w.length())
					{
						if (!hasRefer)
						{
							res = true;
							return;
						}
						for (auto t : *matchState->next_set)
						{
							addEdge(make_tuple(get<0>(t), get<1>(t), get<2>(t)), make_tuple(-1, -1, 0));
						}
					}
					node->next_set->clear();
					node->cur_set->clear();
					break;
				}
				}
			}
			NFA_statesets->clear();
			if (nlist->size())
			{
				for (auto& nxt : *nlist)
				{
					auto temp = nxt->cur_set;
					nxt->cur_set = nxt->next_set;
					nxt->next_set = temp;
					nxt->next_set->clear();
				}
			}
		}
		auto t1 = clist;
		clist = nlist;
		nlist = t1;
		nlist->clear();
	}

	if (!hasRefer)
	{
		return;
	}
	clist->clear();
	nlist->clear();
	//clist->emplace_back(matchState);
	//for (int i = w.length() - 1; i >= 0; i--)
	//{
	//	char c = w[i];
	//	std::fill(visited.begin(), visited.end(), 0);
	//	if (clist->size())
	//	{
	//		for (auto& cur : *clist)
	//		{
	//			//shared_ptr<Production_State> curstate= cmap->at(cur->id);
	//			for (auto& state : cur->reverse_out)
	//				reverse_step(state, c, i, cur);
	//		}
	//	}
	//	auto t1 = clist;
	//	clist = nlist;
	//	nlist = t1;
	//	nlist->clear();
	//}
	//std::fill(visited.begin(), visited.end(), 0);
	//if (clist->size())
	//{
	//	for (auto& cur : *clist)
	//	{
	//		//shared_ptr<Production_State> curstate= cmap->at(cur->id);
	//		for (auto& state : cur->reverse_out)
	//			reverse_step(state, ' ', -1, cur);
	//	}
	//}
	for (auto& la : *lookarounds)
	{
		if (!la->ahead)
			la->refer_bfs(w, min_t, groups, referCount);
		else
			la->refer_reverse_bfs(w, min_t, groups, referCount);
	}
	for (int i = 0; i <= w.length(); i++)
		if (min_t[i] < INT_MAX)
			interval.emplace_back(make_pair(min_t[i], i - 1));
	if (hasRefer)
	{
		int j = interval.size() - 1;
		pair<int, int> temp;
		if (j >= 0)
			temp = interval[j--];
		for (j; j >= 0; j--)
		{
			if (interval[j].first < temp.first)
			{
				if (interval[j].second < temp.first - 1)
				{
					if (temp.first <= temp.second)
						section.emplace_back(temp);
					temp = interval[j];
				}
				else
					temp.first = interval[j].first;
			}
		}
		if (temp.first <= temp.second)
			section.emplace_back(temp);
		reverse(section.begin(), section.end());
	}
}

void Production_FA::get_inner(shared_ptr<NFA_state>start)
{
	std::queue<shared_ptr<NFA_state>> Queue;
	Queue.push(start);
	while (!Queue.empty())
	{
		auto s = Queue.front();
		Queue.pop();
		if (s->c == Type::GroupEnd || s->c == Type::ReferEnd)
			inner[s->id] = 1;
		if (s->c == Type::LookAround)
		{
			auto la = dynamic_pointer_cast<LookAroundState>(s);
			if (la->hasRefer)
				inner[la->id] = 1;
		}
		visited[s->id]++;
		if (inner[s->id] == 0)
		{
			for (auto state : s->reverse_out)
			{
				if (visited[state->id] == 0)
				{
					Queue.push(state);
					visited[state->id]++;
				}
			}
		}
		else
		{
			for (auto state : s->reverse_out)
			{
				if (inner[state->id] == 0)
				{
					inner[state->id] = 1;
					if (visited[state->id] == 2)
						visited[state->id] = 0;
				}
				if (visited[state->id] == 0)
				{
					Queue.push(state);
					visited[state->id]++;
				}
			}
		}
	}

}

bool statecmp(shared_ptr<NFA_state> state1, shared_ptr<NFA_state> state2)
{
	return state1->groupNo < state2->groupNo;
}
void Production_FA::search_group_maps(string &w)
{

	group_visited.resize(group_start_states.size());

	sort(group_start_states.begin(), group_start_states.end(), statecmp);

	for (auto group_start_state : group_start_states)
	{
		//cout << group_start_state->groupNo << endl;
		search_group_map(group_start_state, w);
	}
	/*
	for (auto kv: group_maps) {
		cout <<"GROUP_ID:"<< kv.first << " ";
		for (auto kv2 : kv.second) {
			cout << endl;
			cout << kv2.first<<" ";
			for (auto v : kv2.second) {
				cout << v << " ";
			}
		}
	}
	*/
}

void Production_FA::search_group_map(shared_ptr<NFA_state> group_start_state, string &w)
{	
	int groupNo = referNo2num[group_start_state->referNo];
	unordered_map<int, vector<int>> group_map;
	for (int i = 0; i <= w.length(); i++)
	{
		char c = ' ';
		if (i < w.length()) {
			c = w[i];
		}
		std::fill(visited.begin(), visited.end(), 0);
		group_start_state->cur_set->clear();
		group_start_state->first_set->clear();
		group_start_state->cur_set->insert(make_tuple(group_start_state->referNo,i,0));
		group_start_state->first_set->insert(make_tuple(group_start_state->referNo, i, 0));
		clist->emplace_back(group_start_state);
		for (auto& cur : referEndMap[i])
		{
			step(cur, c, i, cur);
		}
		if (clist->size())
		{
			for (auto& cur : *clist)
			{
				step(cur->out1, c, i, cur);
				step(cur->out2, c, i, cur);
			}
			for (auto& node : *NFA_statesets)
			{
				switch (node->c) {
				case Type::MatchState:
				{
					for (auto t:*node->next_set)
					{
						/*int t0 = get<0>(t);
						int t1 = get<1>(t);
						if (t1 < min_t[i]&&t1>=0)
							min_t[i] = t1;*/
						addEdge(make_tuple(get<0>(t), get<1>(t), get<2>(t)), make_tuple(node->referNo, i - 1, 1));
					}
					for (auto t : *node->first_set) {
						int t0 = get<0>(t);
						int t1 = get<1>(t);
						if (t1 < min_t[i] && t1 >= 0)
							min_t[i] = t1;
						if (t1 == i)
						{
							//cout << "groupNo could be empty:" << groupNo << endl;
							group_can_be_empty[groupNo] = true;
						}
						auto it = group_map.find(t1);
						if (it != group_map.end())
						{
							it->second.emplace_back(i);
						}
						else {
							group_map.emplace(t1, vector<int>{i});
						}
					}
					break;
				}
				case Type::GroupStart:
				case Type::ReferStart:
				{
					if (node->referNo != group_start_state->referNo)
					{
						int nodeGroupNo = referNo2num[node->referNo];
						auto group_map = group_maps[nodeGroupNo];
						auto it = group_map.find(i);
						if (it != group_map.end()) {
							auto pair = make_pair(node->referNo, i);
							for (auto t:*node->next_set)
							{
								addEdge(make_tuple(get<0>(t), get<1>(t), get<2>(t)), make_tuple(node->referNo,i,0));
							}
							for (int j : it->second) {
								if (j > i) {
									node->out1->first_set->insert(node->first_set->begin(), node->first_set->end());
									referEndMap[j].insert(node->out1);
								}
							}
						}
					}
					break;
				}
				}
				node->next_set->clear();
				node->cur_set->clear();
				node->first_set->clear();
			}
			if (nlist->size())
			{
				for (auto& nxt : *nlist)
				{
					auto temp = nxt->cur_set;
					nxt->cur_set = nxt->next_set;
					nxt->next_set = temp;
					nxt->next_set->clear();

					temp = nxt->first_set;
					nxt->first_set = nxt->next_first_set;
					nxt->next_first_set = temp;
					nxt->next_first_set->clear();
				}
			}
			auto tlist = clist;
			clist = nlist;
			nlist = tlist;
			nlist->clear();
		}
	}
	clist->clear();
	nlist->clear();
	std::fill(visited.begin(), visited.end(), 0);
	NFA_statesets->clear();
	group_maps.insert(make_pair(groupNo, group_map));
}
void Production_FA::step(shared_ptr<NFA_state> s, char c, int i, shared_ptr<NFA_state> cur)
{
	if (c < 0 || c>255)
		throw 4;
	if (s == nullptr)
		return;
	if (s->lock)
		return;
	if (!hasRefer && visited[s->id] >= 1)
		return;
	visited[s->id]++;
	switch (s->c)
	{
	case Type::MatchState:
	{
		s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
		s->first_set->insert(cur->first_set->begin(), cur->first_set->end());
		if (visited[s->id] == 1) {
			NFA_statesets->insert(s);
		}
		return;
	}
	case Type::ReferStart:
	case Type::GroupStart:
	{
		s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
		s->first_set->insert(cur->first_set->begin(), cur->first_set->end());
		if (visited[s->id] == 1)
		{
			NFA_statesets->insert(s);
		}
		if (group_can_be_empty[referNo2num[s->referNo]])
		{
			s->lock = true;
			if (s->out1)
			{
				s->out1->first_set->insert(s->first_set->begin(), s->first_set->end());
				step(s->out1, c, i, s);
			}
			if (s->out2)
			{
				s->out2->first_set->insert(s->first_set->begin(), s->first_set->end());
				step(s->out2, c, i, s);
			}
			s->lock = false;
		}
		return;
	}
	case Type::ReferEnd:
	case Type::GroupEnd:
	{
		s->cur_set->insert(make_tuple(s->referNo, i - 1, 1));
		s->lock = true;
		step(s->out1, c, i, s);
		s->lock = false;
		s->cur_set->clear();
		s->first_set->clear();
		return;
	}
	case Type::LookAround:
	{
		shared_ptr<LookAroundState> la = dynamic_pointer_cast<LookAroundState>(s);
		if (la->hasRefer)
		{
			la->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
			la->first_set->insert(cur->first_set->begin(), cur->first_set->end());
			s->cur_set->insert(make_tuple(referCount + la->look_aroundno, i, 1));
			if (visited[s->id] == 1)
			{
				NFA_statesets->insert(s);

			}
		}
		else if (!la->match(w, i))
			return;
		s->groupType = 1;
		s->lock = true;
		step(s->out1, c, i, s);
		s->lock = false;
		s->cur_set->erase(make_tuple(referCount + la->look_aroundno, i, 1));
		return;
	}
	case Type::Split:
	{
		s->lock = true;
		step(s->out1, c, i, cur);
		step(s->out2, c, i, cur);
		s->lock = false;
		return;
	}
	case Type::MetaCharacter:
	{
		bool meta = false;
		if (i > 0 && i < w.length())
		{
			if (isalnum(w[i - 1]) && isalnum(w[i]))
				return;
			if (!isalnum(w[i - 1]) && !isalnum(w[i]))
				return;
		}
		s->lock = true;
		step(s->out1, c, i, cur);
		step(s->out2, c, i, cur);
		s->lock = false;
		return;
	}
	case Type::Blank:
	{
		s->lock = true;
		step(s->out1, c, i, cur);
		s->lock = false;
		return;
	}

	default:
		break;
	}
	if (s->c == c - ' ' || (s->c == Type::AnyChar && c != '\n')
		|| (s->c == Type::SingleLetter && (isspace(c)))
		|| (s->c == Type::NonSingleLetter && !(isspace(c)))
		|| (s->c == Type::Digit && c >= -1 && c <= 200 && isdigit(c))
		|| (s->c == Type::Word && c >= -1 && c <= 200 && (isdigit(c) || isupper(c) || islower(c) || c == '_')))
	{
		if (hasRefer)
		{
			s->groupType = cur->groupType;
			s->referNo = cur->referNo;
			s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
			s->next_first_set->insert(cur->first_set->begin(), cur->first_set->end());
		}
		if (visited[s->id] == 1)
			nlist->emplace_back(s);
	}
	else if (s->c == Type::Scale)
	{
		shared_ptr<ScaleState> ss = dynamic_pointer_cast<ScaleState>(s);
		if (ss->scale[c])
		{
			if (hasRefer)
			{
				s->groupType = cur->groupType;
				s->referNo = cur->referNo;
				s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
				s->next_first_set->insert(cur->first_set->begin(), cur->first_set->end());
			}
			if (visited[s->id] == 1)
				nlist->emplace_back(s);
		}
	}
}
//void Production_FA::step(shared_ptr<NFA_state> s, char c, int i,
//	shared_ptr<NFA_state> cur)
//{
//	if (c < 0 || c>255)
//		throw 4;
//	if (s == nullptr)
//		return;
//	/*if (hasRefer && inner[s->id] == 0)
//		return;*/
//	if (!hasRefer && visited[s->id] >= 1)
//		return;
//	if (s->lock)
//		return;
//	visited[s->id]++;
//	switch (s->c)
//	{
//	case Type::MatchState:
//	{
//		if (i == w.length())
//		{
//			res = true;
//		}
//		return;
//	}
//	case Type::LookAround:
//	{
//		shared_ptr<LookAroundState> la = dynamic_pointer_cast<LookAroundState>(s);
//		if (la->hasRefer)
//		{
//			la->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
//			s->cur_set->insert(make_tuple(referCount + la->look_aroundno, i,1));
//			if (visited[s->id] == 1)
//			{
//				NFA_statesets->insert(s);
//
//			}
//		}
//		else if (!la->match(w, i))
//			return;
//		s->groupType = 1;
//		s->lock = true;
//		step(s->out1, c, i, s);
//		s->lock = false;
//		s->cur_set->erase(make_tuple(referCount + la->look_aroundno, i,1));
//		return;
//	}
//	case Type::Split:
//	{
//		s->lock = true;
//		step(s->out1, c, i, cur);
//		step(s->out2, c, i, cur);
//		s->lock = false;
//		return;
//	}
//	case Type::Blank:
//	{
//		s->lock = true;
//		step(s->out1, c, i, cur);
//		s->lock = false;
//		return;
//	}
//	case Type::ReferStart:
//	case Type::GroupStart:
//	{
//			NFA_statesets->insert(s);
//			s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
//			s->cur_set->insert(make_tuple(s->referNo, i,0));
//			s->groupType = 0;
//			s->lock = true;
//			step(s->out1, c, i, s);
//			step(s->out2, c, i, s);
//			s->lock = false;
//			return;
//	}
//	case Type::ReferEnd:
//	case Type::GroupEnd:
//	{
//			for (auto t : *cur->cur_set)
//				s->next_set->insert(t);
//			s->cur_set->insert(make_tuple(s->referNo, i - 1,1));
//			NFA_statesets->insert(s);
//			s->groupType = 1;
//			s->lock = true;
//			step(s->out1, c, i, s);
//			s->lock = false;
//			return;
//	}
//	default:
//		break;
//	}
//	if (s->c == c - ' ' || (s->c == Type::AnyChar && c != '\n')
//		|| (s->c == Type::SingleLetter && (isspace(c)))
//		|| (s->c == Type::NonSingleLetter && !(isspace(c)))
//		|| (s->c == Type::Digit && c >= -1 && c <= 200 && isdigit(c))
//		|| (s->c == Type::Word && c >= -1 && c <= 200 && (isdigit(c) || isupper(c) || islower(c) || c == '_')))
//	{
//		if (hasRefer)
//		{
//			s->groupType = cur->groupType;
//			s->referNo = cur->referNo;
//			s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
//		}
//		if (visited[s->id] == 1)
//			nlist->emplace_back(s);
//	}
//	else if (s->c == Type::Scale)
//	{
//		shared_ptr<ScaleState> ss = dynamic_pointer_cast<ScaleState>(s);
//		if (ss->scale[c])
//		{
//			if (hasRefer)
//			{
//				s->groupType = cur->groupType;
//				s->referNo = cur->referNo;
//				s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
//			}
//			if (visited[s->id] == 1)
//				nlist->emplace_back(s);
//		}
//	}
//}
void Production_FA::addEdge(tuple<int, int, int>v1, tuple<int, int, int>v2)
{
	if (get<0>(v2) == -1 && get<1>(v2)>= 0)
		return;
	position_edges[get<0>(v1)+ 1][get<1>(v1) + 1][get<2>(v1)].push_back(v2);
}
