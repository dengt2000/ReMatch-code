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
	visited.resize(matchState->id + 1);
	for (int i = 0; i < matchState->id + 1; i++)
	{
		visited[i].resize(w.length() + 1);
		for (int j = 0; j < w.length() + 1; j++)
			visited[i][j].resize(w.length() + 1);
	}
	res = step(re->start, 0, -1, -1);
}



bool Production_FA::step(shared_ptr<NFA_state> s, int i,int curgroup,int curStart)
{
	if (i > wlen)
		return false;
	char c =(i==wlen ? '\0':w[i]);
	if (c < 0 || c>255)
		throw 4;
	if (s == nullptr)
		return false;
	//cout << s->c << " ";
	if (curgroup >= 0)
	{
		if (visited[s->id][i][curgroup])
			return false;
		visited[s->id][i][curgroup] = true;
	}
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
		if (step(s->out1, i, curgroup,curStart) || step(s->out2,  i, curgroup,curStart))
			return true;
		return false;
	}
	case Type::Blank:
	{
		if (step(s->out1,  i, curgroup,curStart) || step(s->out2, i, curgroup,curStart))
			return true;
		return false;
	}
	case Type::ReferStart:
	{
		if (step(s->out1, i + curgroup, curgroup, curStart))
			return true;
		return false;
	}
	case Type::GroupStart:
	{
		
			if(step(s->out1,  i, curgroup,i)||(step(s->out2,i,curgroup,i)))
				return true;
			return false;
	}
	case Type::GroupEnd:
	{
			return step(s->out1,  i, i-curStart,curStart);
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
		return step(s->out1, i + 1, curgroup, curStart);
	}
	else if (s->c == Type::Scale)
	{
		shared_ptr<ScaleState> ss = dynamic_pointer_cast<ScaleState>(s);
		if (ss->scale[c])
		{
			return step(s->out1, i + 1, curgroup, curStart);
		}
	}
	else
		return false;
}


