#include"Production_FA.h"
#include "Parser.h"
#include <queue>
Production_FA::Production_FA(string E, string w) {
	wlen = w.length();
	//shared_ptr<Pattern> p=Type::compile(E);
	shared_ptr<Parser>p = make_shared<Parser>(E);
	//cout << p->re->toString() << endl;
	referCount = p->referCount;
	hasRefer = p->hasRefer;
	referNo2num = p->referNo2num;
	groupNum = p->groupCount;
	//lookaheads = p->lookaheads;
	//lookbehinds = p->lookbehinds;
	lookarounds = p->lookarounds;
	shared_ptr<Regex> re = p->re;
	/*
	if (referCount == 0)
		return;
	*/
	//cout << "01\n";
	clist = make_shared<vector<shared_ptr<NFA_state>>>();
	this->w = w;
	this->E = E;
	position_edges.resize(referCount + lookarounds->size() + 2);
	//check_behind_edges->resize(referCount + 2);
	//check_ahead_edges->resize(referCount + 2);
	for (int i = 0; i < referCount + lookarounds->size() + 2; i++)
	{
		position_edges[i].resize(w.length() + 3);
		for (int j = 0; j < w.length() + 3; j++)
			position_edges[i][j].resize(2);
		//(*check_ahead_edges)[i].resize(w.length() + 3);
		//(*check_behind_edges)[i].resize(w.length()+3);
	}
	nlist = make_shared<vector<shared_ptr<NFA_state>>>();
	tlist = make_shared<vector<shared_ptr<NFA_state>>>();
	//cmap=shared_ptr<unordered_map<int, shared_ptr<Production_State>>>(new unordered_map<int,shared_ptr<Production_State>>);
	//nmap = shared_ptr<unordered_map<int, shared_ptr<Production_State>>>(new unordered_map<int, shared_ptr<Production_State>>);
	NFA_statesets = make_shared<unordered_set<shared_ptr<NFA_state>>>();
	int w_length = w.length();
	//groups.resize(referCount + 1);
	/*for (int i = 0; i < referCount + 1; i++)
	{
		groups[i].resize(w_length + 2);
		for (int j = 0; j < w_length + 2; j++)
			groups[i][j].resize(w_length + 2);
	}*/
	re->start->cur_set = make_shared<unordered_set<tuple<int, int,int>, Tuplefunc>>();
	re->start->referNo = -1;
	//re->start->cur_set->insert(make_pair(-1, 0));
	re->start->groupType = 1;
	//shared_ptr<Production_State> pstart (new Production_State(1,-1, re->start->id));
	//clock_t begin = clock();
	//pstart->addPlace(-1,-1);
	//start = pstart;
	//matchState = make_shared<Production_State>(w.length(), -1);
	//matchState->isMatch = true;
	matchState = p->matchState;
	matchState->referNo = referCount + 1;
	matchState->groupType = 0;
	visited.resize(matchState->id + 1);
	inner.resize(matchState->id + 1);
	vector<pair<int, int>> interval;
	bool first = true;
	min_t.resize(w.length() + 1);
	fill(min_t.begin(), min_t.end(), INT_MAX);
	//cmap->insert(make_pair(re->start->id, pstart));
	get_inner(matchState);
	clist->emplace_back(re->start);
	for (int i = 0; i < w.length(); i++)
	{
		char c = w[i];
		std::fill(visited.begin(), visited.end(), 0);
		if (clist->size())
		{
			for (auto& cur : *clist)
			{
				//shared_ptr<Production_State> curstate= cmap->at(cur->id);
				step(cur->out1, c, i, cur);
				step(cur->out2, c, i, cur);
			}
		}
		if (hasRefer)
		{
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
					if (node->referNo == 0)
					{
						addEdge(make_tuple(-1, -1, 1), make_tuple(node->referNo, i, 0));
					}
					for (auto t : *node->next_set)
					{
						addEdge(make_tuple(get<0>(t), get<1>(t), get<2>(t)), make_tuple(node->referNo,i, 0));
					}
					/*
					for (int j = node->next_set->find_first(); j < node->next_set->size(); j = node->next_set->find_next(j))
					{
						addEdge(make_pair(node->referNo - 1, j - 1), make_pair(node->referNo, i));
					}
					*/
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
					if (!hasRefer)
					{
						res = true;
						return;
					}
					for (auto t : *matchState->next_set)
					{
						addEdge(make_tuple(get<0>(t), get<1>(t), get<2>(t)), make_tuple(-1, -1, 0));
					}
					node->next_set->clear();
					node->cur_set->clear();
					break;
				}
				}
			}
			NFA_statesets->clear();
		}
		if (hasRefer)
		{
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
		//cout << nlist->size();
		//auto t2 = cmap;
		///cmap = nmap;
		//nmap = t2;
		//nmap->clear();
		//cout << clist->size() << "��" << cmap->size()<<endl;
	}
	std::fill(visited.begin(), visited.end(), 0);
	NFA_statesets->clear();
	for (auto& cur : *clist)
	{
		//shared_ptr<Production_State> &curstate = cmap->at(cur->id);
		//cout <<"\nfinal:"  << cur->c << " " << curstate->id.first;
		step(cur->out1, ' ', w.length(), cur);
		step(cur->out2, ' ', w.length(), cur);
	}

	if (!hasRefer)
	{
		return;
	}

	for (auto& node : *NFA_statesets)
	{
		if(node->c==Type::ReferStart)
		{
			if (node->referNo == 0)
			{
				addEdge(make_tuple(-1, -1, 1), make_tuple(node->referNo, w.length(), 0));
			}
			for (auto t : *node->next_set)
			{
				addEdge(make_tuple(get<0>(t), get<1>(t), get<2>(t)), make_tuple(node->referNo, w.length(), 0));
			}
			/*
			for (int j = node->next_set->find_first(); j < node->next_set->size(); j = node->next_set->find_next(j))
			{
				addEdge(make_pair(node->referNo - 1, j - 1), make_pair(node->referNo, i));
			}
			*/
			node->next_set->clear();
			node->cur_set->clear();
		}
		if (node->c == Type::ReferEnd)
		{
			for (auto t : *node->next_set)
			{
				if (get<1>(t) < min_t[w.length()])
					min_t[w.length()] = get<1>(t);
				//groups[node->referNo][get<1>(t)][w.length()] = 1;
			}
		}
	}
	clist->clear();
	nlist->clear();
	clist->emplace_back(matchState);
	for (int i = w.length() - 1; i >= 0; i--)
	{
		char c = w[i];
		std::fill(visited.begin(), visited.end(), 0);
		if (clist->size())
		{
			for (auto& cur : *clist)
			{
				//shared_ptr<Production_State> curstate= cmap->at(cur->id);
				for (auto& state : cur->reverse_out)
					reverse_step(state, c, i, cur);
			}
		}
		auto t1 = clist;
		clist = nlist;
		nlist = t1;
		nlist->clear();
	}
	std::fill(visited.begin(), visited.end(), 0);
	if (clist->size())
	{
		for (auto& cur : *clist)
		{
			//shared_ptr<Production_State> curstate= cmap->at(cur->id);
			for (auto& state : cur->reverse_out)
				reverse_step(state, ' ', -1, cur);
		}
	}
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
	//cout << "\n";
}
void Production_FA::reverse_step(shared_ptr<NFA_state> s, char c, int i,
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
	if (visited[s->id] > 0)
		return;
	visited[s->id]++;
	switch (s->c)
	{
	case Type::Split:
	{
		//cout <<"Split:" << curstate->groupType;
		s->lock = true;
		for (auto& state : s->reverse_out)
			reverse_step(state, c, i, cur);
		s->lock = false;
		return;
	}
	case Type::LookAround:
	{
		auto la = dynamic_pointer_cast<LookAroundState>(s);
		if (!la->hasRefer)
			if (!la->match(w, i + 1))
				return;
		for (auto& state : s->reverse_out)
		{
			reverse_step(state, c, i, cur);
		}
		return;
	}
	case Type::Blank:
	{
		for (auto& state : s->reverse_out)
		{
			reverse_step(state, c, i, cur);
		}
		return;
	}
	case Type::ReferEnd:
	case Type::GroupEnd:
	{
		//cout << "groupEnd";
		addEdge(make_tuple(s->referNo, i,1), make_tuple(-1, -1,0));
		return;
	}
	default:
		break;
	}
	if (s->c == c - ' ' || (s->c == Type::AnyChar && c != '\n')
		|| (s->c == Type::SingleLetter && (islower(c) || isupper(c)))
		|| (s->c == Type::NonSingleLetter && !(islower(c) || isupper(c)))
		|| (s->c == Type::Digit && c >= -1 && c <= 200 && isdigit(c))
		|| (s->c == Type::Word && c >= -1 && c <= 200 && (isdigit(c) || isupper(c) || islower(c) || c == '_')))
	{
		if (visited[s->id] == 1)
			nlist->emplace_back(s);
	}
	else if (s->c == Type::Scale)
	{
		shared_ptr<ScaleState> ss = dynamic_pointer_cast<ScaleState>(s);
		if (ss->scale[c])
		{
			s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
			if (visited[s->id] == 1)
				nlist->emplace_back(s);
		}
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
void Production_FA::step(shared_ptr<NFA_state> s, char c, int i,
	shared_ptr<NFA_state> cur)
{
	if (c < 0 || c>255)
		throw 4;
	if (s == nullptr)
		return;
	if (hasRefer && inner[s->id] == 0)
		return;
	if (!hasRefer && visited[s->id] >= 1)
		return;

	//cout <<curstate->id.first<<" "<<curstate->id.second<<" "<<s->id<<" " << i << " " << s->c << endl;
	if (s->lock)
		return;
	//s->visited=1;
	visited[s->id]++;
	switch (s->c)
	{
	case 257:
	{
		if (i == w.length())
		{
			res = true;
			/*
			if (cur->referNo == referCount||referCount==0)
			{
				s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
				//*s->next_set |= *cur->cur_set;
				NFA_statesets->insert(s);
			}
			*/
			/*
			for (auto t:curstate->places)
			{
				addEdge(t, make_pair(-1, i));
			}
			*/

		}
		return;
	}
	case Type::LookAround:
	{
		shared_ptr<LookAroundState> la = dynamic_pointer_cast<LookAroundState>(s);
		//Production_State* t = new Production_State(la,LookAround, i, -1);
		//curstate->out.push_back(t);
		if (la->hasRefer)
		{
			/*
			if (!la->ahead)
				la->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
			else
			{
				cur->cur_set->insert(make_pair(referCount+la->look_aroundno, i-1));
			}
			*/
			la->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
			s->cur_set->insert(make_tuple(referCount + la->look_aroundno, i,1));
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
		s->cur_set->erase(make_tuple(referCount + la->look_aroundno, i,1));
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
		//cout << "groupStart";
		/*
		if (cur->groupType == 0)
		{
			if (cur->referNo != s->referNo)
				return;
			//clock_t begin = clock();
			//curstate->addPlace(s->referNo, i);
			cur->cur_set->insert(make_pair(s->referNo, i));
			//(* cur->cur_set)[i+1] = 1;
			//groupStarts[s->referNo]->insert(i);
			s->lock = true;
			step(s->out1, c, i, cur);
			step(s->out2, c, i, cur);
			cur->cur_set->erase(make_pair(s->referNo, i));
			//(*cur->cur_set)[i+1] = 0;
			s->lock = false;
			return;
		}
		*/
			/*
			for (auto t : curstate->places)
			{
				addEdge(t, make_pair(s->referNo, i));
			}
			*/
			//shared_ptr<Production_State> nextstate (new Production_State(0, i, -1));
			NFA_statesets->insert(s);
			s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
			//*s->next_set |= *cur->cur_set;
			//(*s->cur_set)[i+1] = 1;
			s->cur_set->insert(make_tuple(s->referNo, i,0));
			//nextstate->addPlace(s->referNo, i);
			//groupStarts[s->referNo]->insert(i);
			//curstate->out.push_back(nextstate);
			s->groupType = 0;
			s->lock = true;
			step(s->out1, c, i, s);
			step(s->out2, c, i, s);
			s->lock = false;
			return;
	}
	case Type::ReferEnd:
	case Type::GroupEnd:
	{
		//cout << "groupEnd";
		/*
		if (cur->groupType == 1)
		{
			cur->cur_set->insert(make_pair(s->referNo, i - 1));
			//curstate->addPlace(s->referNo, i-1);
			//groupEnds[s->referNo]->insert(i - 1);
			s->lock = true;
			step(s->out1, c, i, cur);
			cur->cur_set->erase(make_pair(s->referNo, i - 1));
			//(*cur->cur_set)[i] = 0;
			s->lock = false;
			return;
		}*/
			/*
			for (auto t : curstate->places)
			{
				groups[s->referNo].insert(make_pair(t.second, i - 1));
			}
			*/
			for (auto t : *cur->cur_set)
				s->next_set->insert(t);
			s->cur_set->insert(make_tuple(s->referNo, i - 1,1));
			//(*s->cur_set)[i] = 1;
			NFA_statesets->insert(s);
			//shared_ptr<Production_State> nextstate (new Production_State(1, i, -1));
			//nextstate->addPlace(s->referNo, i - 1);
			//groupEnds[s->referNo]->insert(i - 1);
			//curstate->out.push_back(nextstate);
			s->groupType = 1;
			s->lock = true;
			step(s->out1, c, i, s);
			s->lock = false;
			return;
	}
	/*
	case Type::ReferStart:
	{
		cout << "referStart";
		Production_State* t = new Production_State(ReferStart, i, -1);
		curstate->out.push_back(t);
		step(s->out1, c, i, t);
		s->visited = false;
		return;
	}
	case Type::ReferEnd:
	{
		cout << "referEnd";
		Production_State* t = new Production_State(ReferEnd, i-1, -1);
		curstate->out.push_back(t);
		step(s->out1, c, i, t);
		s->visited = false;
		return;
	}
	*/
	default:
		break;
	}
	if (s->c == c - ' ' || (s->c == Type::AnyChar && c != '\n')
		|| (s->c == Type::SingleLetter && (isspace(c)))
		|| (s->c == Type::NonSingleLetter && !(isspace(c)))
		|| (s->c == Type::Digit && c >= -1 && c <= 200 && isdigit(c))
		|| (s->c == Type::Word && c >= -1 && c <= 200 && (isdigit(c) || isupper(c) || islower(c) || c == '_')))
	{
		//cout << "0"<< c << s->c<<" ";
		//shared_ptr<Production_State> state = getState(nmap, i, s->id);
		//state->groupType = cur->groupType;
		if (hasRefer)
		{
			s->groupType = cur->groupType;
			s->referNo = cur->referNo;
			//state->addPlaces(curstate->places);
			s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
			//*s->next_set |= *cur->cur_set;
			//curstate->out.push_back(state);
		}
		//clock_t begin = clock();
		if (visited[s->id] == 1)
			nlist->emplace_back(s);
		//placetime1 += clock() - begin;
	}
	else if (s->c == Type::Scale)
	{
		shared_ptr<ScaleState> ss = dynamic_pointer_cast<ScaleState>(s);
		if (ss->scale[c])
		{
			//shared_ptr<Production_State> state = getState(nmap, i, s->id);
			//state->groupType = curstate->groupType;
			if (hasRefer)
			{
				s->groupType = cur->groupType;
				s->referNo = cur->referNo;
				//state->addPlaces(curstate->places);
				s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
				//*s->next_set |= *cur->cur_set;
			}
			if (visited[s->id] == 1)
				nlist->emplace_back(s);
		}
	}
}
void Production_FA::addEdge(tuple<int, int, int>v1, tuple<int, int, int>v2)
{
	//position_edges.insert(make_pair(v1, v2));
	if (get<0>(v2) == -1 && get<1>(v2)>= 0)
		return;
	//if (v1.first >referCount)
	//{
		//shared_ptr<LookAroundState> la = lookaheads->at(-v1.first - 2);
		//(*check_ahead_edges)[v2.first][v2.second].insert(la);
		//la->start_positions[v1.second+1].emplace_back(make_pair(v2.first, v2.second));
	//}
	//else
	position_edges[get<0>(v1)+ 1][get<1>(v1) + 1][get<2>(v1)].push_back(v2);
	/*
	if (res == position_edges.end())
	{
		vector<pair<int,int>> t;
		t.emplace_back(v2);
		position_edges.insert(make_pair(v1, t));
	}
	else
	{
		res->second.emplace_back(v2);
	}
	*/
}
/*
shared_ptr<Production_State> Production_FA::getState(shared_ptr<unordered_map<int, shared_ptr<Production_State>>>& map,int id1,int id2)
{
	auto s = map->find(id2);
	if (s !=map->end())
		return s->second;
	else
	{
		shared_ptr<Production_State> state (new Production_State(id1, id2));
		map->insert(make_pair(id2, state));
		return state;
	}
}

Production_State::Production_State(int id1, int id2):id(make_pair(id1,id2)),groupType(0)
{
}

Production_State::Production_State(int type, int id1, int id2):id(make_pair(id1,id2)),groupType(type)
{
}
/*
void Production_State::addPlace(int groupno,int place)
{
	places.insert(make_pair(groupno,place));
}
void Production_State::addPlaces(set<pair<int, int>,pair_comp> &places)
{
	for (auto w : places)
		this->places.insert(w);
}
/*
Position_Node::Position_Node(int type)
{
	this->type = type;
}

void Position_Node::addNext(Position_Node* p)
{
	next.push_back(p);
}
*/
/*
LookAround_State::LookAround_State(shared_ptr<LookAroundState> p, int group_state, int id1, int id2):Production_State(group_state,id1,id2),s(p)
{
}
*/
