#include"Production_FA.h"
#include "Parser.h"
#include <queue>
Production_FA::Production_FA(string E, string w) {
	wlen = w.length();
	//shared_ptr<Pattern> p=Type::compile(E);
	shared_ptr<Parser>p = make_shared<Parser>(E);
	//cout << p->re->toString() << endl;
	hasRefer = p->hasRefer;
	group_start_state=p->group_start_state;
	lookarounds = p->lookarounds;
	lookaroundNum=p->lookarounds->size();
	shared_ptr<Regex> re = p->re;
	clist = make_shared<vector<shared_ptr<NFA_state>>>();
	this->w = w;
	this->E = E;
	position_edges.resize(referCount + lookarounds->size() + 2);
	//check_behind_edges->resize(referCount + 2);
	//check_ahead_edges->resize(referCount + 2);
	for (int i = 0; i < referCount + lookarounds->size() + 2; i++)
	{
		position_edges[i].resize(w.length() + 3);
	}
	nlist = make_shared<vector<shared_ptr<NFA_state>>>();
	tlist = make_shared<vector<shared_ptr<NFA_state>>>();
	NFA_statesets = make_shared<unordered_set<shared_ptr<NFA_state>>>();
	int w_length = w.length();
	groups.resize(1);
	for (int i = 0; i < 1; i++)
	{
		groups[i].resize(w_length + 2);
		for (int j = 0; j < w_length + 2; j++)
			groups[i][j].resize(w_length + 2);
	}
	re->start->cur_set = make_shared<boost::dynamic_bitset<>>();
	re->start->cur_set->resize((max(colsize * rawsize, 0)));
	re->start->next_set->resize((max(colsize * rawsize, 0)));
	re->start->referNo = -1;
	re->start->groupType = 1;
	matchState = p->matchState;
	matchState->referNo = referCount + 1;
	matchState->groupType = 0;
	visited.resize(matchState->id + 1);
	inner.resize(matchState->id + 1);
	vector<pair<int, int>> interval;
	bool first = true;
	if(hasRefer)
	{
		referEndMap.resize(w.length()+1);
		min_t.resize(w.length() + 1);
		fill(min_t.begin(), min_t.end(), INT_MAX);
		search_group_map(group_start_state,w);
	}
	//cmap->insert(make_pair(re->start->id, pstart));
	get_inner(matchState);
	clist->emplace_back(re->start);
	for (int i = 0; i <= w.length(); i++)
	{
		char c = ' ';
		if (i<w.length())
			c=w[i];
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
			for (auto &cur:referEndMap[i])
			{
				step(cur,c,i,cur);
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
					for(int j=node->next_set->find_first();j!=node->next_set->npos;)
					{
						int t0 = j / colsize-1;
						int t1 = j % colsize - 1;
						if (t0>-1)
						{
							//(*check_behind_edges)[t.first + 1][t.second + 1].insert(la);
							addEdge(make_pair(t0,t1), pair);
						}
						j = node->next_set->find_next(j);
					}
				}
				case Type::GroupStart:
				case Type::ReferStart:
				{
					auto pair = make_pair(node->referNo, i);
					for (int j = node->next_set->find_first(); j != node->next_set->npos;)
					{
						int t0 = j / colsize - 1;
						int t1 = j % colsize - 1;
						if (t0 > -1)
						{
							//(*check_behind_edges)[t.first + 1][t.second + 1].insert(la);
							addEdge(make_pair(t0, t1), pair);
						}
						j = node->next_set->find_next(j);
					}
					auto it=group_map.find(i);
					if (it!=group_map.end()){
						for (int j:it->second){
							if (node->referNo==0){
							groups[0][i][j]=1;
							}
							if (j>i){	
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
					node->next_set->reset();
					node->cur_set->reset();
					break;
				}
				// case Type::GroupEnd:
				// case Type::ReferEnd:
				// {
				// 	for (int j = node->next_set->find_first(); j != node->next_set->npos;)
				// 	{
				// 		int t0 = j / colsize - 1;
				// 		int t1 = j % colsize - 1;
				// 		if (t1 < min_t[i])
				// 			min_t[i] = t1;
				// 		j = node->next_set->find_next(j);
				// 		//cout << node->referNo << " " << t1 << " " << i<<endl;
				// 		groups[node->referNo][t1][i] = 1;
				// 	}
				// 	node->next_set->reset();
				// 	node->cur_set->reset();
				// 	break;
				// }
				case Type::MatchState:
					if (!hasRefer)
					{
						res = true;
						return;
					}
					for (int j = matchState->next_set->find_first(); j != matchState->next_set->npos;)
					{
						int t0 = j / colsize - 1;
						int t1 = j % colsize - 1;
						if (t0>=0)
						{
							//(*check_behind_edges)[t.first + 1][t.second + 1].insert(la);
							addEdge(make_pair(t0, t1), make_pair(-1, -1));
						}
						j = matchState->next_set->find_next(j);
					}
					matchState->next_set->reset();
					matchState->cur_set->reset();
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
					nxt->next_set->reset();
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
	for (auto& la : *lookarounds)
	{
		if (!la->ahead)
			la->refer_bfs(w, min_t, groups, referCount);
		else
			la->refer_reverse_bfs(w, min_t, groups, referCount);
	}
	if (hasRefer){
	for (int i = 0; i <= w.length(); i++)
		if (min_t[i] < INT_MAX)
			interval.emplace_back(make_pair(min_t[i], i-1));
	}
	//cout << "interval:";
	//for (int i = 0; i < interval.size(); i++)
	//	cout << interval[i].first << " " << interval[i].second << endl;
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
void Production_FA::step(shared_ptr<NFA_state> s, char c, int i,
	shared_ptr<NFA_state> cur)
{
	if (c < 0 || c>255)
		throw 4;
	if (s == nullptr)
		return;
	if (hasRefer && inner[s->id] == 0&&visited[s->id]>=1)
		return;
	if (!hasRefer && visited[s->id] >= 1)
		return;
	//cout <<curstate->id.first<<" "<<curstate->id.second<<" "<<s->id<<" " << i << " " << s->c << endl;
	if (s->lock)
		return;
	//s->visited=1;
	visited[s->id]++;
	//if (debug) {
	//	cout << s->c << " " << c-' ' <<" "<<i << endl;
	//}
	switch (s->c)
	{
	case 257:
	{
		//cout << i<<" ";
		if (i == w.length())
		{
			*s->next_set |= *cur->cur_set;
			//*s->next_set |= *cur->cur_set;
			NFA_statesets->insert(s);
			res = true;
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
			//la->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
			*la->next_set |= *cur->cur_set;
			//s->cur_set->insert(make_pair(referCount + la->look_aroundno, i));
			s->cur_set->set((referCount + la->look_aroundno + 1) * colsize + i + 1);
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
		//s->cur_set->erase(make_pair(referCount + la->look_aroundno, i));
		s->cur_set->reset((referCount + la->look_aroundno+1)*colsize+i+1);
		return;
	}
	case Type::Split:
	{
		//cout <<"Split:" << curstate->groupType;
		s->lock = true;
		//cout << s->out1->c << " " << s->out2->c << endl;
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
		//cout << s->out1->c << " " << s->out2->c << endl;
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
		// if (cur->groupType == 0)
		// {
		// 	if (cur->referNo != s->referNo)
		// 		return;
		// 	cur->cur_set->set((s->referNo + 1) * colsize + i + 1);
		// 	//(* cur->cur_set)[i+1] = 1;
		// 	//groupStarts[s->referNo]->insert(i);
		// 	s->lock = true;
		// 	step(s->out1, c, i, cur);
		// 	step(s->out2, c, i, cur);
		// 	//cur->cur_set->erase(make_pair(s->referNo, i));
		// 	cur->cur_set->reset((s->referNo + 1) * colsize + i + 1);
		// 	//(*cur->cur_set)[i+1] = 0;
		// 	s->lock = false;
		// 	return;
		// }
		if (cur->groupType == 1)
		{
			*s->next_set |= *cur->cur_set;
			// s->cur_set->set((s->referNo+1)*colsize+i+1);
			if (visited[s->id]==1)
			{
				NFA_statesets->insert(s);
			}
			if (group_can_be_empty){
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
		// if (cur->groupType == 1)
		// {
		// 	cur->cur_set->set((s->referNo+1)*colsize+i);
		// 	s->lock = true;
		// 	step(s->out1, c, i, cur);
		// 	cur->cur_set->reset((s->referNo + 1)* colsize + i);
		// 	s->lock = false;
		// 	return;
		// }
		// else if (cur->groupType == 0)
		// {
		// 	*s->next_set |= *cur->cur_set;
		// 	s->cur_set->set((s->referNo + 1)* colsize + i);
		// 	NFA_statesets->insert(s);
		s->groupType = 1;
		s->cur_set->set((s->referNo + 1)* colsize + i);
		s->lock = true;
		step(s->out1, c, i, s);
		step(s->out2, c, i, s);
		s->lock = false;
		s->cur_set->reset((s->referNo + 1)* colsize + i);
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
			//state->addPlaces(curstate->places);
			//s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
			*s->next_set |= *cur->cur_set;
			//*s->next_set |= *cur->cur_set;
			//curstate->out.push_back(state);
		}
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
				//s->next_set->insert(cur->cur_set->begin(), cur->cur_set->end());
				*s->next_set |= *cur->cur_set;
				//*s->next_set |= *cur->cur_set;
			}
			if (visited[s->id] == 1)
				nlist->emplace_back(s);
		}
	}
}
void Production_FA::addEdge(pair<int, int> v1, pair<int, int> v2)
{
	if (v2.first == -1 && v2.second >= 0)
		return;
	position_edges[v1.first + 1][v1.second + 1].push_back(v2);
}


void Production_FA::search_group_map(shared_ptr<NFA_state> group_start_state,string w)
{
	for (int i = 0; i <= w.length(); i++)
	{
		char c=' ';
		if (i<w.length()){
			c = w[i];
		}
		std::fill(visited.begin(), visited.end(), 0);
		group_start_state->cur_set->reset();
		group_start_state->cur_set->set(i);
		clist->emplace_back(group_start_state);
		if (clist->size())
		{
			for (auto& cur : *clist)
			{
				//shared_ptr<Production_State> curstate= cmap->at(cur->id);
				simple_step(cur->out1, c, i, cur);
				simple_step(cur->out2, c, i, cur);
			}
			for (auto& node : *NFA_statesets)
			{
				for (int j = node->next_set->find_first(); j != node->next_set->npos;)
					{
						int t0 = j / colsize ;
						int t1 = j % colsize ;
						if (t1 < min_t[i])
							min_t[i] = t1;
						if (t1==i)
						{
							group_can_be_empty=true;
						}
						auto it=group_map.find(t1);
						if (it!= group_map.end())
						{
							it->second.emplace_back(i);
						}else{
							group_map.emplace(t1,vector<int>{i});
						}
						j = node->next_set->find_next(j);
					}
					node->next_set->reset();
					node->cur_set->reset();
			}
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
			auto tlist = clist;
			clist = nlist;
			nlist = tlist;
			nlist->clear();
		}
	}
	clist->clear();
	nlist->clear();
	std::fill(visited.begin(), visited.end(), 0);
}

void Production_FA::simple_step(shared_ptr<NFA_state> s, char c,int i,shared_ptr<NFA_state> cur)
{
	if (c < 0 || c>255)
		throw 4;
	if (s == nullptr)
		return;
	if (s->lock)
		return;
	visited[s->id]++;
	switch (s->c)
	{
		case 257:
		{
				*s->next_set |= *cur->cur_set;
			if (visited[s->id]==1){
				NFA_statesets->insert(s);
			}
				return;
		}
		case Type::LookAround:
		{
			shared_ptr<LookAroundState> la = dynamic_pointer_cast<LookAroundState>(s);
			if (la->hasRefer || !la->match(w, i)){
				return;
			}
			s->lock = true;
			simple_step(s->out1, c, i, s);
			s->lock = false;
			return;
		}
		case Type::Split:
		{
			s->lock = true;
			simple_step(s->out1, c, i, cur);
			simple_step(s->out2, c, i, cur);
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
			//cout << s->out1->c << " " << s->out2->c << endl;
			simple_step(s->out1, c, i, cur);
			simple_step(s->out2, c, i, cur);
			s->lock = false;
			return;
		}
		case Type::Blank:
		{
			s->lock = true;
			simple_step(s->out1, c, i, cur);
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
			*s->next_set |= *cur->cur_set;
			if (visited[s->id] == 1)
				nlist->emplace_back(s);
		}
		else if (s->c == Type::Scale)
		{
			shared_ptr<ScaleState> ss = dynamic_pointer_cast<ScaleState>(s);
			if (ss->scale[c])
			{
				*s->next_set |= *cur->cur_set;
				if (visited[s->id] == 1)
					nlist->emplace_back(s);
			}
		}
}