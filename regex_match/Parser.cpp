#include "Parser.h"
#include <bitset>
Parser::Parser(string in)
{
	this->input = in;
	if (input.length() > 0 && input[0] == '^')
	{
		isPrefix = true;
		input = input.substr(1);
	}
	if (input.length() > 0 && input[input.length()-1] == '$')
	{
		isSuffix = true;
		int i = 1;
		while (input[input.length() - 1 - i] == '\\')
			i++;
		if(i%2)
			input = input.substr(0, input.length() - 1);
	}
	for (int i=0;i<input.length()-1;i++)
	{
		if (input[i] == '\\' &&(input[i+1]>=48&&input[i+1] <= 57))
		{
			int num=0;
			for (int j = i + 1; input[j] >= 48 && input[j]<=57; j++)
				num = num * 10 + input[j]-'0';
			hasRefer = true;
			referID = num;
			referCount++;
		}


	}
	Regex::blank->start->cur_set->resize(rawsize * colsize);
	Regex::blank->start->next_set->resize(rawsize * colsize);
	/*
	if (!isPrefix)
		input = ".*" + input;
	if (!isSuffix)
		input = input + ".*";
	*/
	re = regex();
	matchState = make_shared<NFA_state>(257);
	stateNo = matchState->id - re->start->id + 1;
	patch1(re->out1, matchState);
	patch2(re->out2, matchState);
	re->setIsPrefix(isPrefix);
	re->setIsSuffix(isSuffix);
}

shared_ptr<Regex> Parser::parse()
{
	return re;
}

char Parser::peek()
{
	return input.at(position);
}

void Parser::eat(char c)
{
	char next = peek();
	if (next == c) position++;
	else {
		//cout << "Expected " <<c << ";got "<< next;
		//abort();
	}
}

char Parser::next()
{
	char c = peek();
	eat(c);
	return c;
}

bool Parser::more()
{
	return position < input.length();
}

shared_ptr<Regex> Parser::regex()
{
	shared_ptr<Regex> term = Parser::term();
	if (more() && peek() == '|')
	{
		eat('|');
		shared_ptr<Regex> regex = Parser::regex();
		shared_ptr<Regex> temp=static_pointer_cast<Regex>(make_shared<Choice>(term, regex));
		return temp;
	}
	else {
		return term;
	}
}

shared_ptr<Regex> Parser::term()
{
	shared_ptr<Regex> factor(new Blank());
	while (more() &&( peek() != '|') && peek() != ')')
	{
		shared_ptr<Regex> nextFactor = Parser::factor();
		factor =make_shared<Sequence>(factor, nextFactor);
	}
	return factor;
}

shared_ptr<Regex> Parser::factor()
{
	shared_ptr<Regex> base = Parser::base();
	while (more() && (peek() == '*' || peek() == '+' || peek() == '?'))
	{
		char c = peek();
		switch (c)
		{
		case '*':
			base = make_shared<StarRepetition>(base);
			eat('*');
			break;
		case '+':
			base = make_shared<PlusRepetition>(base);
			eat('+');
			break;
		case '?':
			base = make_shared<QuoraRepetition>(base);
			eat('?');
			break;
		default:
			break;
		}
	}
	return base;
}

shared_ptr<Regex> Parser::base()
{
	bool uncapure = false;
	switch (peek())
	{
	case '(':
	{
		eat('(');
		char nxt = peek();
		if (nxt == '?')
		{
			eat('?');
			if(peek()==':')
			{ 
				eat(':');
				uncapure = true;
			}
			else
			{
				bool ahead = false;
				bool positive;
				if (peek() == '<')
				{
					eat('<');
					ahead = true;
				}
				if (peek() == '=')
				{
					eat('=');
					positive = true;
				}
				else if (peek() == '!')
				{
					eat('!');
					positive = false;
				}
				else
				{
					cout << "illegal expression\n";
					abort();
				}
				int start = position;
				shared_ptr<Regex> r = regex();
				int end = position;
				eat(')');
				bool hasrefer=false;
				int referID = 0;
				for (int j = start; j <= end - 1; j++)
				{
					if (input[j] == '\\' && (input[j + 1] >= 48 && input[j + 1] <= 57))
					{
						hasrefer = true;
						int num = 0;
						for (int k = j + 1; k<=end&&input[k] >= 48 && input[k] <= 57; k++)
							num = num * 10 + input[j] - '0';
						referID = num;
					}
				}
				auto lookaround= make_shared<class::LookAround>(r, ahead, positive,hasrefer);
				if (hasrefer)
				{
						lookarounds->emplace_back(dynamic_pointer_cast<LookAroundState>(lookaround->start));
						lookaround->start->look_aroundno = lookarounds->size();
				}
				return lookaround;
			}
		}
		if (!uncapure)
		{
			int tempid = ++groupID;
			shared_ptr<Regex> r = regex();
			group.insert(make_pair(tempid, r));
			if (hasRefer && referID == tempid)
				r->setGroupState(true, false, 0);
			r->setGroupID(tempid);
			eat(')');
			return r;
		}
		else
		{
			shared_ptr<Regex> r = regex();
			eat(')');
			return r;
		}
	}
	case '\\':
	{
		eat('\\');
		char esc = next();
		if (esc > 255 || esc <= 0)
			throw 1;
		if (isdigit(esc))
		{
			int id=esc-'0';
			while (more()&&isdigit(peek()))
			{
				esc = next();
				id = id * 10 + esc - '0';
			}
			if (id > groupID)
				throw 3;
			referID = id;
			shared_ptr<Regex> re = group[id]->copy();
			re->setGroupState(false, true, curreferCount++);
			//re->referNo = referCount++;
			//re->setIsReference(true);
			return re;
		}
		return make_shared<SpecialChars>(esc);
	}
	case '.':
	{
		eat('.');
		return make_shared<class::AnyChar>();
	}
	case '[':
	{
		eat('[');
		shared_ptr<Regex> re = multichoice();
		eat(']');
		return re;
	}
	default:
	{	
		shared_ptr<Regex> nex =make_shared<Primitives>(next());
	return nex;
	}
	}
}

shared_ptr<Regex> Parser::multichoice()
{
	bool negative = false;
	if (peek() == '^')
	{
		negative = true;
		eat('^');
	}
	shared_ptr<MultiChoice> ans(new MultiChoice());
	while (more() && peek() != ']')
	{
		shared_ptr<MultiChoice> single = dynamic_pointer_cast<MultiChoice>(Parser::singlechoice(true));
		ans->orOp(single);
		while (peek() == '&')
		{
			eat('&');
			eat('&');
			shared_ptr<MultiChoice> second = dynamic_pointer_cast<MultiChoice>(Parser::andchoice());
			ans->andOp(second);
		}
	}
	if (negative)
		ans->reverse();
	//cout << bitset<256>(ans->scale) << endl;
	ans->generateNFA();
	return ans;
}

shared_ptr<Regex> Parser::singlechoice(bool in)
{
	if (peek() == '[' && !in)
	{
		eat('[');
		shared_ptr<Regex> re = multichoice();
		eat(']');
		return re;
	}
	char c1 = next();
	if (c1 == '\\')
	{
		eat('\\');
		c1 = next();
		switch (c1)
		{															case 's':														{
			auto p = make_shared<MultiChoice>(' ', ' ');
			p->orOp(make_shared<MultiChoice>('\t', '\t'));
			p->orOp(make_shared<MultiChoice>('\n', '\n'));										return p;
		}
		case 'S':														{
			auto p = make_shared<MultiChoice>(' ', ' ');
			p->orOp(make_shared<MultiChoice>('\t', '\t'));
			p->orOp(make_shared<MultiChoice>('\n', '\n'));										p->reverse();
			return  p;
		}
		case 'w':
		{
			auto p = make_shared<MultiChoice>('0', '9');										
			p->orOp(make_shared<MultiChoice>('a', 'z'));	
			p->orOp(make_shared<MultiChoice>('A', 'Z'));
			p->orOp(make_shared<MultiChoice>('_', '_'));
			return p;
		}
		case 'W':														{																auto p = make_shared<MultiChoice>('0', '9');
			p->orOp(make_shared<MultiChoice>('a', 'z'));										p->orOp(make_shared<MultiChoice>('A', 'Z'));										p->reverse();
			return p;
		}
		case 'd':
		{
			return make_shared<MultiChoice>('0', '9');
		}
		default:
			break;									
		}
	}
	if (peek() == '-')
	{
		eat('-');
		if (peek() == ']')
		{
			shared_ptr<MultiChoice> single1( new MultiChoice(c1, c1));
			single1->orOp(make_shared<MultiChoice>('-', '-'));
			return single1;
		}
		char c2 = next();
		return make_shared<MultiChoice>(c1, c2);
	}
	return make_shared<MultiChoice>(c1, c1);
}

shared_ptr<Regex> Parser::andchoice()
{
	shared_ptr<MultiChoice> ans = make_shared<MultiChoice>();
	while (more() && peek() != ']' && peek() != '&')
	{
		shared_ptr<MultiChoice> re = dynamic_pointer_cast<MultiChoice>(singlechoice());
		ans->orOp(re);
	}
	return ans;
}

vector<shared_ptr<NFA_state>> Parser::list1(shared_ptr<NFA_state> outp)
{
	vector<shared_ptr<NFA_state>> list;
	list.push_back(outp);
	return list;
}

static vector<shared_ptr<NFA_state>> append(vector<shared_ptr<NFA_state>> l1, vector<shared_ptr<NFA_state>> l2)
{
	for (auto& state : l2)
		l1.push_back(state);
	return l1;
}

void Parser::patch2(shared_ptr <vector<shared_ptr<NFA_state>>>& l, shared_ptr<NFA_state> s)
{
	for (auto& t : *l)
	{
		t->out2 = s;
		s->reverse_out.emplace_back(t);
	}
}

void Parser::patch1(shared_ptr<vector<shared_ptr<NFA_state>>>& l, shared_ptr<NFA_state> s)
{
	for (auto& t : *l)
	{
		t->out1 = s;
		s->reverse_out.emplace_back(t);
	}
}
