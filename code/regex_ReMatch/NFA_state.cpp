#pragma once
#include "NFA_state.h"
#include "Type.h"
#include <queue>
#include <iostream>

int NFA_state::count = 1;
int wlen;
int lookaroundNum;
int referCount;
bool stateptrCmp::operator()(const shared_ptr<NFA_state>& s1, const shared_ptr<NFA_state>& s2)const
{
	return s1->id < s2->id;
}
NFA_state::NFA_state(int c) :c(c), id(count++), out1(nullptr)
{
	cur_set = make_shared<boost::dynamic_bitset<>>();
	next_set = make_shared<boost::dynamic_bitset<>>();
	cur_set->resize(max(colsize*rawsize, 0));
	next_set->resize(max(colsize*rawsize,0));
}

NFA_state::NFA_state(char c) :c(c - ' '), id(count++), out1(nullptr)
{
	cur_set = make_shared<boost::dynamic_bitset<>>();
	next_set = make_shared<boost::dynamic_bitset<>>();
	cur_set->resize(max(colsize * rawsize, 0));
	next_set->resize(max(colsize * rawsize, 0));
}

NFA_state::NFA_state(int c, shared_ptr<NFA_state> out1, shared_ptr <NFA_state> out2) :c(c), id(count++), out1(out1), out2(out2)
{
	cur_set = make_shared<boost::dynamic_bitset<>>();
	next_set = make_shared<boost::dynamic_bitset<>>();
	cur_set->resize(max(colsize * rawsize, 0));
	next_set->resize(max(colsize * rawsize, 0));
}

NFA_state::NFA_state(char c, shared_ptr<NFA_state> out1, shared_ptr<NFA_state> out2) :c(c - ' '), id(count++), out1(out1), out2(out2)
{
	cur_set = make_shared<boost::dynamic_bitset<>>();
	next_set = make_shared<boost::dynamic_bitset<>>();
	cur_set->resize(max(colsize * rawsize, 0));
	next_set->resize(max(colsize * rawsize, 0));
}

string NFA_state::toString()
{
	shared_ptr<NFA_state>out2 = this->out2;
	return "[c:" + std::to_string(id)+"("+to_string(c)+")" + ",out1:" +
		(out1 == NULL ? "NULL" : out1->toString()) +
		",out2:" + (out2 == NULL ? "NULL" : out2->toString()) + "]";
}

void NFA_state::setGroupstate(int i)
{
	shared_ptr<NFA_state>out2 = this->out2;
	if (i <= 2 && i >= 0)
		group_state = i;
	if (this->c == Type::Blank)
		out1->setGroupstate(i);
	else if (this->c == Type::Split)
	{
		out1->setGroupstate(i);
		out2->setGroupstate(i);
	}
}

void NFA_state::setReferstate(int i)
{
	shared_ptr<NFA_state>out2 = this->out2;
	if (i <= 2 && i >= 0)
		group_state = i;
	if (this->c == Type::Blank)
		out1->setGroupstate(i);
	else if (this->c == Type::Split)
	{
		out1->setGroupstate(i);
		out2->setGroupstate(i);
	}
}

ScaleState::ScaleState(bitset<256> s) :NFA_state(Type::Scale), scale(s)
{
}

string ScaleState::toString()
{
	return string();
}
bool LookAroundState::match(string s, int i)
{
	bool res;
	auto temp = result.find(i);
	if (temp != result.end())
		return temp->second;

	if (ahead)
	{
		res = reverse_dfs(s, i - 1, matchState);
	}
	else
		res = dfs(s, i, startState);
	if (!positive)
		res = !res;
	result.insert(make_pair(i, res));
	return res;
}

void LookAroundState::reverse(shared_ptr<NFA_state>& start)
{
	queue<shared_ptr<NFA_state>> Queue;
	Queue.push(start);
	revered_visited.insert(make_pair(start->id, true));
	while (!Queue.empty())
	{
		auto& cur_state = Queue.front();
		Queue.pop();
		auto& out1 = cur_state->out1;
		auto& out2 = cur_state->out2;
		if (out1 != nullptr)
		{

			out1->reverse_out.emplace_back(cur_state);
			if (revered_visited.find(out1->id) == revered_visited.end())
			{
				Queue.push(out1);
				revered_visited.insert(make_pair(out1->id, true));
			}
		}
		if (out2 != nullptr)
		{
			out2->reverse_out.emplace_back(cur_state);
			if (revered_visited.find(out2->id) == revered_visited.end())
			{
				Queue.push(out2);
				revered_visited.insert(make_pair(out2->id, true));
			}
		}
	}
}


bool LookAroundState::reverse_dfs(string s, int i, shared_ptr<NFA_state> cur_state)
{
	if (cur_state == nullptr)
		return false;
	if (cur_state->c == 258)
		return true;
	if (cur_state->c == Type::Split || cur_state->c == Type::Blank)
	{
		for (auto& state : cur_state->reverse_out)
			if (reverse_dfs(s, i, state.lock()))
				return true;
		return false;
	}
	if (i < -1)
		return false;
	char c = ' ';
	if(i>=0)
		c = s[i];
	if (c < 0 || c>255)
		throw 4;
	if (visited.find(make_pair(cur_state->id, i)) != visited.end())
		return false;
	visited.insert(make_pair(make_pair(cur_state->id, i), true));
	switch (cur_state->c)
	{
	case Type::Split:
	case 257:
	{
		for (auto& state : cur_state->reverse_out)
			if (reverse_dfs(s, i, state.lock()))
				return true;
		return false;
	}
	case Type::Blank:
	{
		for (auto& state : cur_state->reverse_out)
			if (reverse_dfs(s, i, state.lock()))
				return true;
		return false;
	}
	default:
	{
		if (i == -1)
			return false;
		if ((i>=0&&cur_state->c == s[i] - ' ') || (cur_state->c == Type::AnyChar && c != '\n' - ' '&&i!=-1)
			|| (cur_state->c == Type::SingleLetter && (isspace(c)))
			|| (cur_state->c == Type::NonSingleLetter && !(isspace(c)))
			|| (cur_state->c == Type::Digit && c > -1 && c <= 200 && isdigit(c))
			|| (cur_state->c == Type::NonDigit && !isdigit(c))
			|| (cur_state->c == Type::NonWord && !(isalnum(c) || c == '_'))
			|| (cur_state->c == Type::Word && c > -1 && c <= 200 && (isdigit(c) || isupper(c) || islower(c) || c == '_')))
		{
			for (auto& state : cur_state->reverse_out)
				if (reverse_dfs(s, i - 1, state.lock()))
					return true;
		}
		else if (cur_state->c == Type::Scale)
		{
			shared_ptr<ScaleState> ss = dynamic_pointer_cast<ScaleState>(cur_state);
			if (ss->scale[s[i]])
			{
				for (auto& state : cur_state->reverse_out)
					if (reverse_dfs(s, i - 1, state.lock()))
						return true;
			}
		}
		return false;
	}
	}
}
bool LookAroundState::dfs(string s, int i, shared_ptr<NFA_state>& cur_state)
{
	if (!cur_state)
		return false;
	if (cur_state->c == 257)
		return true;
	if (i > s.length())
		return false;
	char c = ' ';
	if(i<s.length())
		c = s[i];
	if (c < 0 || c>255)
		throw 4;
	if (visited.find(make_pair(cur_state->id, i)) != visited.end())
		return false;
	visited.insert(make_pair(make_pair(cur_state->id, i), true));
	switch (cur_state->c)
	{

	case Type::Split:
	{
		if (dfs(s, i, cur_state->out2))
			return true;
		if (dfs(s, i, cur_state->out1))
			return true;
		return false;
	}
	case Type::MetaCharacter:
	{
		bool meta = false;
		if (i > 0 && i < s.length())
		{
			if ((isalnum(s[i - 1]) || s[i - 1] == '_') && (isalnum(s[i]) || s[i] == '_'))
				return false;
			if (!(isalnum(s[i - 1]) || s[i - 1] == '_') && !(isalnum(s[i]) || s[i] == '_'))
				return false;
		}
		if (dfs(s, i, cur_state->out2))
			return true;
		if (dfs(s, i, cur_state->out1))
			return true;
		return false;
	}
	case Type::Blank:
	{
		if (dfs(s, i, cur_state->out1))
			return true;
		return false;
	}
	case Type::LookAround:
	{
		shared_ptr<LookAroundState> la = dynamic_pointer_cast<LookAroundState>(cur_state);
		if ((la->hasRefer || la->hasGroup)|| !la->match(s, i)) {
			return false;
		}
		return dfs(s,i,cur_state->out1);
	}
	default:
	{
		if (i == s.length())
			return false;
		if (cur_state->c == s[i] - ' ' || (cur_state->c == Type::AnyChar && c != '\n' - ' '&&i!=s.length())
			|| (cur_state->c == Type::SingleLetter && (isspace(c)))
			|| (cur_state->c == Type::NonSingleLetter && !(isspace(c)))
			|| (cur_state->c == Type::Digit && c > -1 && c <= 200 && isdigit(c))
			|| (cur_state->c == Type::NonDigit && !isdigit(c))
			||(cur_state->c == Type::NonWord && !(isalnum(c) || c == '_'))
			|| (cur_state->c == Type::Word && c > -1 && c <= 200 && (isdigit(c) || isupper(c) || islower(c) || c == '_')))
		{
			return dfs(s, i + 1, cur_state->out1);
		}
		else if (cur_state->c == Type::Scale)
		{
			shared_ptr<ScaleState> ss = dynamic_pointer_cast<ScaleState>(cur_state);
			if (ss->scale[s[i]])
			{
				return dfs(s, i + 1, cur_state->out1);
			}
		}
		return false;
	}
	}
}

void LookAroundState::refer_reverse_bfs(string s, vector<int>& min_t, vector<vector<vector<bool>>>& groups, int referCount)
{
	clist = make_shared<vector<shared_ptr<NFA_state>>>();
	nlist = make_shared<vector<shared_ptr<NFA_state>>>();
	tlist = make_shared<vector<shared_ptr<NFA_state>>>();

	position_edges.resize(referCount + 2);
	for (int i = 0; i < referCount + 2; i++)
		position_edges[i].resize(s.length() + 2);
	refer_visited.resize(startState->id + 1);
	if (start_positions.size() < 0)
		return;
	int i = s.length() - 1;
	int position = start_positions.size() - 1;
	for (i = start_positions[position] - 1; i >= 0; i--)
	{
		char c = s[i];
		if (i == start_positions[position] - 1)
		{
			position--;
			matchState->cur_set->reset();
			//for(auto pair:start_positions[i])
			//matchState->cur_set->insert(make_pair(-1, i));
			matchState->cur_set->set(i);
			matchState->groupType = 0;
			clist->emplace_back(matchState);
		}
		std::fill(refer_visited.begin(), refer_visited.end(), 0);
		if (clist->size())
		{
			for (auto& cur : *clist)
			{
				//shared_ptr<Production_State> curstate= cmap->at(cur->id);
				for (auto& state : cur->reverse_out)
					reverse_step(state.lock(), c, i, cur);
			}
		}
		for (auto& node : *NFA_statesets)
		{

			switch (node->c)
			{
			case Type::GroupStart:
			case Type::ReferStart:
			{
				for (int j=node->next_set->find_first();j!=boost::dynamic_bitset<>::npos;j=node->next_set->find_next(j))
				{
					int pos = j % colsize-1;
					if (i + 1 < min_t[pos])
						min_t[pos] = i + 1;
					//groups[node->referNo][i + 1][pos+1]=1;
				}
				/*
				for (int j = node->next_set->find_first(); j < node->next_set->size(); j = node->next_set->find_next(j))
				{
						addEdge(make_pair(node->referNo - 1, j - 1), make_pair(node->referNo, i));
				}
				*/
				break;
			}
			case Type::GroupEnd:
			case Type::ReferEnd:
			{
				for (int j = node->next_set->find_first(); j != boost::dynamic_bitset<>::npos; j = node->next_set->find_next(j))
				{
					auto t = make_pair<int,int>(j / colsize-1, j % colsize-1);
					addEdge(t, make_pair(node->referNo, i),position_edges);
				}
				break;
			}
			case Type::RevMatchState:
				for (int j = node->next_set->find_first(); j != boost::dynamic_bitset<>::npos; j = node->next_set->find_next(j))
				{
					int t0 = j / colsize - 1;
					int t1 = j % colsize - 1;
					addEdge(make_pair(t0,t1), make_pair(-1, -1),position_edges);
				}
			}
			node->next_set->reset();
			node->cur_set->reset();
		}
		NFA_statesets->clear();
		if (nlist->size())
		{
			for (auto& nxt : *nlist)
			{
				auto temp = nxt->cur_set;
				nxt->cur_set = nxt->next_set;
				nxt->next_set = temp;
				nxt->next_set->reset();
			}
		}
		auto t1 = clist;
		clist = nlist;
		nlist = t1;
		nlist->clear();
	}
	//cout << nlist->size();
	//auto t2 = cmap;
	///cmap = nmap;
	//nmap = t2;
	//nmap->clear();
	//cout << clist->size() << "¡¡" << cmap->size()<<endl;
	std::fill(refer_visited.begin(), refer_visited.end(), 0);
	NFA_statesets->clear();
	for (auto& cur : *clist)
	{
		//shared_ptr<Production_State> &curstate = cmap->at(cur->id);
		//cout <<"\nfinal:"  << cur->c << " " << curstate->id.first;
		for (auto& state : cur->reverse_out)
			reverse_step(state.lock(), ' ', -1, cur);
	}
	for (auto& node : *NFA_statesets)
	{
		if (node->c == Type::ReferStart)
		{
			for (int j = node->next_set->find_first(); j != boost::dynamic_bitset<>::npos; j = node->next_set->find_next(j))
			{
				int t0 = j / colsize - 1;
				int t1 = j % colsize - 1;
				min_t[t1] = 0;
				groups[node->referNo][0][t1+1]=1;
			}
		}
	}
	for (int j = startState->next_set->find_first(); j != boost::dynamic_bitset<>::npos; j = startState->next_set->find_next(j))
	{
		int t0 = j / colsize - 1;
		int t1 = j % colsize - 1;
		//cout << j << endl;
		addEdge(make_pair(t0,t1), make_pair(-1, -1),position_edges);
	}
	return;
}

//NFA_state::~NFA_state() {
//	cout << this->c<<" "<<this->id << endl;
//}

void LookAroundState::refer_bfs(string s, vector<int>& min_t, vector<vector<vector<bool>>>& groups, int referCount, unordered_map<int, vector<int>>&group_map, vector<unordered_set<shared_ptr<NFA_state>>> &referEndMap, vector<vector<vector<pair<int,int>>>>&position_edges,bool group_can_be_empty)
{
	clist = make_shared<vector<shared_ptr<NFA_state>>>();
	nlist = make_shared<vector<shared_ptr<NFA_state>>>();
	tlist = make_shared<vector<shared_ptr<NFA_state>>>();
	if (hasGroup) {
		this->groupInfo = make_shared<GroupInfo>(s.length()+2,ahead,positive);
	}
	this->group_can_be_empty = group_can_be_empty;
	referNo = referCount+1+look_aroundno*2;
	//position_edges.resize(referCount + 2);
	//for (int i = 0; i < ; i++)
	//	position_edges[i].resize(s.length() + 2);
	refer_visited.resize(NFA_state::count+1);
	if (start_positions.size() <= 0)
		return;
	int position = 0;
	for (int i = start_positions[position]; i <= s.length(); i++)
	{
		char c = ' ';
		if (i < s.length())
			c = s[i];
		if (position<start_positions.size() && i == start_positions[position])
		{
			position++;
			startState->cur_set->reset();
			//for(auto pair:start_positions[i])
			//startState->cur_set->insert(make_pair(-1, i));
			startState->cur_set->set((referNo + 1) * colsize + i + 1);
			startState->groupType = 1;
			clist->emplace_back(startState);
		}
		std::fill(refer_visited.begin(), refer_visited.end(), 0);
		if (clist->size())
		{
			for (auto& cur : *clist)
			{
				//shared_ptr<Production_State> curstate= cmap->at(cur->id);
				step(cur->out1, c, i, cur);
				step(cur->out2, c, i, cur);
			}
		}
		for (auto& cur : referEndMap[i])
		{
			step(cur, c, i, cur);
		}
		for (auto& node : *NFA_statesets)
		{

			switch (node->c)
			{
			case Type::GroupStart:
			case Type::ReferStart:
			{
				auto pair = make_pair(node->referNo, i);
				for (int j = node->next_set->find_first(); j != boost::dynamic_bitset<>::npos; j = node->next_set->find_next(j))
				{
					int t0 = j / colsize - 1;
					int t1 = j % colsize - 1;
					addEdge(make_pair(t0,t1), pair,position_edges);
					if (node->referNo == 0) {
						this->groupInfo->lookaroundPos[i].emplace_back(make_pair(t0,t1));
					}
				}
				auto it = group_map.find(i);
				if (it != group_map.end()) {
					for (int j : it->second) {
						if (node->referNo == 0) {
							groups[0][i][j] = 1;
						}
						if (j > i) {
							referEndMap[j].insert(node->out1);
						}
					}
				}
				/*
				for (int j = node->next_set->find_first(); j < node->next_set->size(); j = node->next_set->find_next(j))
				{
						addEdge(make_pair(node->referNo - 1, j - 1), make_pair(node->referNo, i));
				}
				*/
				break;
			}
			//case Type::GroupEnd:
			//case Type::ReferEnd:
			//{
			//	for (int j = node->next_set->find_first(); j != boost::dynamic_bitset<>::npos; j = node->next_set->find_next(j))
			//	{
			//		int t0 = j / colsize - 1;
			//		int t1 = j % colsize - 1;
			//		if (t1 < min_t[i])
			//			min_t[i] = t1;
			//		//groups[node->referNo][t1][i]=1;
			//	}
			//	break;
			//}
			case Type::MatchState:
				for (int j = matchState->next_set->find_first(); j != boost::dynamic_bitset<>::npos; j = matchState->next_set->find_next(j))
				{
					int t0 = j / colsize-1;
					int t1 = j % colsize-1;
					addEdge(make_pair(t0,t1), make_pair(referNo+1, -1),position_edges);
				}
			}
			node->next_set->reset();
			node->cur_set->reset();
		}
		NFA_statesets->clear();
		if (nlist->size())
		{
			for (auto& nxt : *nlist)
			{
				auto temp = nxt->cur_set;
				nxt->cur_set = nxt->next_set;
				nxt->next_set = temp;
				nxt->next_set->reset();
			}
		}
		auto t1 = clist;
		clist = nlist;
		nlist = t1;
		nlist->clear();
	}
	//cout << nlist->size();
	//auto t2 = cmap;
	///cmap = nmap;
	//nmap = t2;
	//nmap->clear();
	//cout << clist->size() << "¡¡" << cmap->size()<<endl;
	std::fill(refer_visited.begin(), refer_visited.end(), 0);
	NFA_statesets->clear();
	for (auto& cur : *clist)
	{
		//shared_ptr<Production_State> &curstate = cmap->at(cur->id);
		//cout <<"\nfinal:"  << cur->c << " " << curstate->id.first;
		step(cur->out1, ' ', s.length(), cur);
		step(cur->out2, ' ', s.length(), cur);
	}
	if (!hasRefer)
	{
		return;
	}
	for (auto& node : *NFA_statesets)
	{
		if (node->c == Type::ReferEnd)
		{
			for (int j = node->next_set->find_first(); j != boost::dynamic_bitset<>::npos; j = node->next_set->find_next(j))
			{
				int t0 = j / colsize - 1;
				int t1 = j % colsize - 1;
				if (t1 < min_t[s.length()])
					min_t[s.length()] = t1;
				groups[node->referNo][t1][s.length()]=1;
			}
		}
	}
	for(int j=matchState->next_set->find_first();j!=boost::dynamic_bitset<>::npos;j=matchState->next_set->find_next(j))
	{
		//cout << j << endl;
		addEdge(make_pair(j/colsize-1,j%colsize-1), make_pair(-1, -1),position_edges);
	}
	return;
}
void LookAroundState::reverse_step(shared_ptr<NFA_state> s, char c, int i,
	shared_ptr<NFA_state> cur)
{
	if (c < 0 || c>255)
		throw 4;
	if (s == nullptr)
		return;
	//cout <<curstate->id.first<<" "<<curstate->id.second<<" "<<s->id<<" " << i << " " << s->c << endl;
	if (s->lock)
		return;
	//s->visited=1;
	refer_visited[s->id]++;
	switch (s->c)
	{
	case 258:
	{
		//s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
		*s->next_set |= *cur->cur_set;
		NFA_statesets->insert(s);
		return;
	}
	case Type::Split:
	{
		//cout <<"Split:" << curstate->groupType;
		s->lock = true;
		for (auto& state : s->reverse_out)
			reverse_step(state.lock(), c, i, cur);
		s->lock = false;
		return;
	}
	case Type::Blank:
	{
		s->lock = true;
		for (auto& state : s->reverse_out)
			reverse_step(state.lock(), c, i, cur);
		s->lock = false;
		return;
	}
	case Type::ReferStart:
	case Type::GroupStart:
	{
		//cout << "groupStart";
		if (cur->groupType == 0)
		{
			if (cur->referNo != s->referNo)
				return;
			clock_t begin = clock();
			//curstate->addPlace(s->referNo, i);
			//cur->cur_set->insert(make_pair(s->referNo, i + 1));
			cur->cur_set->set((s->referNo + 1)* colsize + i + 2);
			//(* cur->cur_set)[i+1] = 1;
			//groupStarts[s->referNo]->insert(i);
			s->lock = true;
			for (auto& state : s->reverse_out)
				reverse_step(state.lock(), c, i, cur);
			//cur->cur_set->erase(make_pair(s->referNo, i + 1));
			cur->cur_set->reset((s->referNo + 1) * colsize + i + 2);
			//(*cur->cur_set)[i+1] = 0;
			s->lock = false;
			return;
		}
		else if (cur->groupType == 1)
		{
			NFA_statesets->insert(s);
			//s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
			*s->next_set |= *cur->cur_set;
			s->cur_set->set((s->referNo + 1) * colsize + i + 2);
			//s->cur_set->insert(make_pair(s->referNo, i + 1));
			s->groupType = 0;
			s->lock = true;
			for (auto& state : s->reverse_out)
				reverse_step(state.lock(), c, i, s);
			s->lock = false;
			return;
		}
	}
	case Type::ReferEnd:
	case Type::GroupEnd:
	{
		//cout << "groupEnd";
		if (cur->groupType == 1)
		{
			//cur->cur_set->insert(make_pair(s->referNo, i));
			cur->cur_set->set((s->referNo + 1) * colsize + i + 1);
			//curstate->addPlace(s->referNo, i-1);
			//groupEnds[s->referNo]->insert(i - 1);
			s->lock = true;
			for (auto& state : s->reverse_out)
				reverse_step(state.lock(), c, i, cur);
			//cur->cur_set->erase(make_pair(s->referNo, i));
			cur->cur_set->reset((s->referNo + 1) * colsize + i + 1);
			//(*cur->cur_set)[i] = 0;
			s->lock = false;
			return;
		}
		else if (cur->groupType == 0)
		{
			/*
			for (auto t : curstate->places)
			{
				groups[s->referNo].insert(make_pair(t.second, i - 1));
			}
			*/
			//for (auto t : *cur->cur_set)
			//	s->next_set->insert(t);
			*s->next_set |= *cur->cur_set;
			s->cur_set->set((s->referNo + 1) * colsize + i + 1);
			//s->cur_set->insert(make_pair(s->referNo, i));
			//(*s->cur_set)[i] = 1;
			NFA_statesets->insert(s);
			//shared_ptr<Production_State> nextstate (new Production_State(1, i, -1));
			//nextstate->addPlace(s->referNo, i - 1);
			//groupEnds[s->referNo]->insert(i - 1);
			//curstate->out.push_back(nextstate);
			s->groupType = 1;
			s->lock = true;
			for (auto& state : s->reverse_out)
				reverse_step(state.lock(), c, i, s);
			s->lock = false;
			return;
		}
	}
	default:
		break;
	}
	if (s->c == c - ' ' || (s->c == Type::AnyChar && c != '\n' && i !=-1)
		|| (s->c == Type::SingleLetter && (isspace(c)))
		|| (s->c == Type::NonSingleLetter && !(isspace(c)))
		|| (s->c == Type::Digit && c >= -1 && c <= 200 && isdigit(c))
		|| (s->c == Type::NonDigit && !isdigit(c))
		|| (s->c == Type::NonWord && !(isalnum(c) || c == '_'))
		|| (s->c == Type::Word && c >= -1 && c <= 200 && (isdigit(c) || isupper(c) || islower(c) || c == '_')))
	{
		s->groupType = cur->groupType;
		s->referNo = cur->referNo;
		//state->addPlaces(curstate->places);
		*s->next_set |= *cur->cur_set;
		//s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
		//*s->next_set |= *cur->cur_set;
		//curstate->out.push_back(state);
		if (refer_visited[s->id] == 1)
			nlist->emplace_back(s);
	}
	else if (s->c == Type::Scale)
	{
		shared_ptr<ScaleState> ss = dynamic_pointer_cast<ScaleState>(s);
		if (ss->scale[c])
		{
			//shared_ptr<Production_State> state = getState(nmap, i, s->id);
			//state->groupType = curstate->groupType;
			s->groupType = cur->groupType;
			s->referNo = cur->referNo;
			//state->addPlaces(curstate->places);
			//s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
			*s->next_set |= *cur->cur_set;
			//*s->next_set |= *cur->cur_set;
			clock_t begin = clock();
			if (refer_visited[s->id] == 1)
				nlist->emplace_back(s);



		}
	}
}

void LookAroundState::step(shared_ptr<NFA_state> s, char c, int i,
	shared_ptr<NFA_state> cur)
{
	if (c < 0 || c>255)
		throw 4;
	if (s == nullptr)
		return;
	//cout <<curstate->id.first<<" "<<curstate->id.second<<" "<<s->id<<" " << i << " " << s->c << endl;
	if (s->lock)
		return;
	//s->visited=1;
	refer_visited[s->id]++;
	switch (s->c)
	{
	case 257:
	{
		//s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
		*s->next_set |= *cur->cur_set;
		NFA_statesets->insert(s);
		return;
	}
	case Type::Split:
	{
		//cout <<"Split:" << curstate->groupType;
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
	case Type::ReferStart:
	case Type::GroupStart:
	{
		if (cur->groupType == 1)
		{
			*s->next_set |= *cur->cur_set;
			// s->cur_set->set((s->referNo+1)*colsize+i+1);
			if (refer_visited[s->id] == 1)
			{
				NFA_statesets->insert(s);
			}
			if (group_can_be_empty) {
				s->lock = true;
				step(s->out1, c, i, s);
				step(s->out2, c, i, s);
				s->lock = false;
			}
			return;
		}
	}
	case Type::ReferEnd:
	case Type::GroupEnd:
	{
		s->groupType = 1;
		s->cur_set->set((s->referNo + 1) * colsize + i);
		s->lock = true;
		step(s->out1, c, i, s);
		step(s->out2, c, i, s);
		s->lock = false;
		s->cur_set->reset((s->referNo + 1) * colsize + i);
	}
	default:
		break;
	}
	if (s->c == c - ' ' || (s->c == Type::AnyChar && c != '\n' && c!=' ')
		|| (s->c == Type::SingleLetter && (isspace(c)))
		|| (s->c == Type::NonSingleLetter && !(isspace(c)))
		|| (s->c == Type::Digit && c >= -1 && c <= 200 && isdigit(c))
		|| (s->c == Type::NonDigit && !isdigit(c))
		|| (s->c == Type::NonWord && !(isalnum(c) || c == '_'))
		|| (s->c == Type::Word && c >= -1 && c <= 200 && (isdigit(c) || isupper(c) || islower(c) || c == '_')))
	{
		s->groupType = cur->groupType;
		s->referNo = cur->referNo;
		//state->addPlaces(curstate->places);
		//s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
		*s->next_set |= *cur->cur_set;
		//*s->next_set |= *cur->cur_set;
		//curstate->out.push_back(state);
		if (refer_visited[s->id] == 1)
			nlist->emplace_back(s);
	}
	else if (s->c == Type::Scale)
	{
		shared_ptr<ScaleState> ss = dynamic_pointer_cast<ScaleState>(s);
		if (ss->scale[c])
		{
			s->groupType = cur->groupType;
			s->referNo = cur->referNo;
			*s->next_set |= *cur->cur_set;
			clock_t begin = clock();
			if (refer_visited[s->id] == 1)
				nlist->emplace_back(s);
		}
	}
}

void LookAroundState::addEdge(pair<int, int> v1, pair<int, int> v2,vector<vector<vector<pair<int,int>>>> &position_edges)
{
	//position_edges.insert(make_pair(v1, v2));
	if (v1.first < -1)
		return;
	position_edges[v1.first + 1][v1.second + 1].push_back(v2);
}

NfaList::NfaList(vector<shared_ptr<NFA_state>>states) :ss(states)
{
	sort(ss.begin(), ss.end(), stateptrCmp());
}

DState::DState(shared_ptr<NfaList> list) :nfalist(list)
{
}
Frag::Frag(shared_ptr<NFA_state> s, vector<shared_ptr<NFA_state>> o) :start(s), out1(o)
{
}
Frag::Frag(shared_ptr<NFA_state> s) :start(s)
{
}
Frag::Frag(shared_ptr<NFA_state> s, vector<shared_ptr<NFA_state>> o1, vector<shared_ptr<NFA_state>>o2) :start(s), out1(o1), out2(o2)
{
}
string Frag::toString()
{
	string s = "[Frag start:]" + start->toString() + "[Frag out:";
	for (auto& w : out1)
		s = s + w->toString();
	return s;
}

bool NfalistCmp::operator()(const shared_ptr<NfaList>& n1, const shared_ptr<NfaList>& n2)const
{
	return *n1 < *n2;
}

size_t Hasher::operator()(const shared_ptr<NFA_state>& rhs)const
{
	return rhs->id;
}

size_t Hasher::operator()(const shared_ptr<NfaList>& rhs)const
{
	int ans = 0;
	for (int i = 0; i < rhs->ss.size(); i++)
		ans += hash<shared_ptr<NFA_state>>()(rhs->ss[i]);
	return ans;
}

bool NfaList::operator<(const NfaList& o) const
{
	if (ss.size() != o.ss.size())
		return ss.size() < o.ss.size();
	else
	{
		for (int i = 0; i < ss.size(); i++)
		{
			if ((ss[i]) < (o.ss[i]))
				return true;
			if ((o.ss[i]) < (ss[i]))
				return false;
		}
		return false;
	}
}

LookAroundState::LookAroundState(bool ahead, bool positive, shared_ptr<NFA_state>& startState, shared_ptr<NFA_state>& matchState, bool hasRefer,bool hasGroup):NFA_state(Type::LookAround), ahead(ahead), positive(positive), matchState(matchState), hasRefer(hasRefer),hasGroup(hasGroup)
{
	cur_set = make_shared<boost::dynamic_bitset<>>();
	next_set = make_shared<boost::dynamic_bitset<>>();
	cur_set->resize(max(colsize * rawsize, 0));
	next_set->resize(max(colsize * rawsize, 0));
	//this->start_positions.resize(wlen + 3);
	if (ahead)
	{
		auto temp = make_shared<NFA_state>(Type::RevMatchState);
		startState->reverse_out.emplace_back(temp);
		//reverse(startState);
		this->startState = temp;
	}
	else
		this->startState = startState;

}
