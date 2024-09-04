#include"Production_FA.h"
#include "Parser.h"
#include <queue>
#include<cstring>
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
	string emptystr = "";
	res = step(re->start, 0, emptystr, -1);
}



bool Production_FA::step(shared_ptr<NFA_state> s, int i,string& group,int curStart)
{
	//cout <<s->c<<" "<< i << " " << group << endl;
	if (i > wlen)
		return false;
	char c =(i==wlen ? '\0':w[i]);
	if (c < 0 || c>255)
		throw 4;
	if (s == nullptr)
		return false;
	//cout << s->c << " ";
		if (visited.find(make_tuple(s->id,i,group))!=visited.end())
			return false;
		visited.insert(make_pair(make_tuple(s->id, i, group), true));
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
		if (step(s->out1, i, group,curStart) || step(s->out2,  i, group,curStart))
			return true;
		return false;
	}
	case Type::Blank:
	{
		if (step(s->out1,  i, group,curStart) || step(s->out2, i, group,curStart))
			return true;
		return false;
	}
	case Type::ReferStart:
	{
		if(strcmp(w.substr(i,i+group.length()).c_str(), group.c_str())!=0)
			return false;
		if (step(s->out1, i + group.length(), group, curStart))
			return true;
		return false;
	}
	case Type::GroupStart:
	{
		
			if(step(s->out1,  i, group,i)||(step(s->out2,i,group,i)))
				return true;
			return false;
	}
	case Type::GroupEnd:
	{
		string temp = w.substr(curStart, i-curStart);
			return step(s->out1,  i, temp,curStart);
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
		return step(s->out1, i + 1, group, curStart);
	}
	else if (s->c == Type::Scale)
	{
		shared_ptr<ScaleState> ss = dynamic_pointer_cast<ScaleState>(s);
		if (ss->scale[c])
		{
			return step(s->out1, i + 1, group, curStart);
		}
		return false;
	}
	else
		return false;
}


