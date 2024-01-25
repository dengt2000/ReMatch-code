#include"Production_FA.h"
#include "Parser.h"
#include <queue>
Production_FA::Production_FA(string E, string w) {
	wlen = w.length();
	shared_ptr<Parser>p = make_shared<Parser>(E);

	lookarounds = p->lookarounds;
	shared_ptr<Regex> re = p->re;
	clist = make_shared<vector<shared_ptr<NFA_state>>>();
	this->w = w;
	this->E = E;
	matchState = p->matchState;
	matchState->referNo = referCount + 1;
	matchState->groupType = 0;
	//visited.resize(matchState->id + 1);
	//id2num = p->id2num;
	referNo2num = p->referNo2num;
	groupNum = p->groupCount;
	/*for (int i = 0; i < matchState->id + 1; i++)
	{
		visited[i].resize(w.length() + 1);
		for (int j = 0; j < w.length() + 1; j++)
			visited[i][j].resize(w.length() + 1);
	}*/

	res = step(re->start, 0,make_shared<vector<int>>(groupNum,-1), make_shared<stack<pair<int,int>>>(),false,make_pair(nullptr,-1));
}



bool Production_FA::step(shared_ptr<NFA_state> s, int i,shared_ptr<vector<int>> curgroup,shared_ptr<stack<pair<int,int>>> curStart,bool hasGroup, pair<shared_ptr<NFA_state>, int> curEnd)
{
	if (i > wlen)
		return false;
	char c =(i==wlen ? '\0':w[i]);
	if (c < 0 || c>255)
		throw 4;
	if (s == nullptr)
		return false;
	/*cout << s->c << " "<<i<<" "<<"curGroup:";
	for (auto i : *curgroup)
		cout << i << " ";
	if(!curStart->empty())	
		cout << "curStart:"<<curStart->top().first<<" "<<curStart->top().second;
	cout << endl;*/
	//clock_t begin = clock();
	if (hasGroup)
	{
		auto temp = make_tuple(s->id, i, curgroup);
		if ((visited.find(temp))!=visited.end())
			return false;
		visited.insert(make_pair(temp, true));
	}
	//clock_t end = clock();
	//ttime += end - begin;
	switch (s->c)
	{
	case 257:
	{
		if (i == w.length())
		{
			return true;
		}
		return false;
	}
	
	case Type::Split:
	{
		//cout <<"Split:" << curstate->groupType;
		/*s->lock = true;*/
		if (step(s->out1, i, curgroup,curStart,hasGroup,curEnd) || step(s->out2,  i, curgroup,curStart,hasGroup,curEnd))
			return true;
		return false;
	}
	case Type::Blank:
	{
		if (step(s->out1,  i, curgroup,curStart,hasGroup,curEnd) || step(s->out2, i, curgroup,curStart,hasGroup,curEnd))
			return true;
		return false;
	}
	case Type::ReferStart:
	{
		/*if (curEnd.first != nullptr)
		{
			auto t = paths.find(curEnd);
			if (t == paths.end())
			{
				auto v = make_shared<vector<pair<shared_ptr<NFA_state>, int>>>();
				v->push_back(make_pair(s, i));
				paths.insert(make_pair(curEnd, v));
			}
			else
			{
				t->second->push_back(make_pair(s, i));
			}
		}*/
		//cout << s->referNo << " " << referNo2num[s->referNo] << endl;
		//cout << i << " " << curgroup->at(referNo2num[s->referNo]) << endl;
		//cout << s->out1->c<<endl;
		if (step(s->out1, i + curgroup->at(referNo2num[s->referNo]), curgroup, curStart, hasGroup,curEnd))
			return true;
		return false;
	}
	case Type::GroupStart:
	{
		/*if (curEnd.first != nullptr)
		{
			auto t = paths.find(curEnd);
			if (t == paths.end())
			{
				auto v = make_shared<vector<pair<shared_ptr<NFA_state>, int>>>();
				v->push_back(make_pair(s, i));
				paths.insert(make_pair(curEnd, v));
			}
			else
			{
				t->second->push_back(make_pair(s, i));
			}
		}*/
		curStart->push(make_pair(referNo2num[s->referNo], i));
			if(step(s->out1,  i, curgroup,curStart,hasGroup,curEnd)||(step(s->out2,i,curgroup,curStart,hasGroup,curEnd)))
				return true;
			curStart->pop();
			return false;
	}
	/*case Type::ReferEnd: {
		auto nextStateL = paths.find(make_pair(s, i - 1));
		if (nextStateL != paths.end())
		{
			for (auto nextState : *nextStateL->second)
			{
				if (step(nextState.first, nextState.second, curgroup,
					curStart, hasGroup, make_pair(nullptr, -1)))
					return true;
			}
		}
		if (step(s->out1, i , curgroup, curStart, hasGroup, make_pair(s,i-1)))
			return true;
		return false;
	}*/
	case Type::GroupEnd:
	{
		auto t = curStart->top();
		int temp = t.second;
		curStart->pop();
		(*curgroup)[referNo2num[s->referNo]] = i-temp;
		//auto nextStateL = paths.find(make_pair(s, i - 1));
		//if (nextStateL != paths.end())
		//{
		//	for (auto nextState : *nextStateL->second)
		//	{
		//		if (step(nextState.first, nextState.second, curgroup,
		//			curStart, hasGroup,make_pair(nullptr, -1)))
		//			return true;
		//	}
		//}
		if (step(s->out1, i, curgroup, curStart, true,make_pair(s,i-1)))
			return true;
		else {
			curStart->push(t);
			(*curgroup)[referNo2num[s->referNo]] = -1;
			return false;
		}
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
		return step(s->out1, i + 1, curgroup, curStart,hasGroup,curEnd);
	}
	else if (s->c == Type::Scale)
	{
		shared_ptr<ScaleState> ss = dynamic_pointer_cast<ScaleState>(s);
		if (ss->scale[c])
		{
			return step(s->out1, i + 1, curgroup, curStart,hasGroup,curEnd);
		}
	}
	else
		return false;
}


