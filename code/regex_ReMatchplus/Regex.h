#pragma once
#include <string>
#include<iostream>
#include<bitset>
#include"NFA_state.h"
#include"Type.h"
using namespace std;

class Parser;
class Regex
{
public :
	int min_len = 0;
	int max_len = 0;
	bool isPrefix = false;
	bool isSuffix = false;
	int groupID = -1;
	bool isReference = false;
	int referNo = -1;
	shared_ptr<NFA_state> start;
	shared_ptr<Regex> group_copy;
	shared_ptr<vector<shared_ptr<NFA_state>>> out1=make_shared<vector<shared_ptr<NFA_state>>>();
	shared_ptr<vector<shared_ptr<NFA_state>>> out2=make_shared<vector<shared_ptr<NFA_state>>>();
	shared_ptr<vector<shared_ptr<NFA_state>>> oldout1;
	shared_ptr<vector<shared_ptr<NFA_state>>> oldout2;
	virtual string toString()=0;
	virtual shared_ptr<Regex> copy()=0;
	bool getIsPrefix();
	bool getIsSuffix();
	bool getIsReference();
	int getGroupID();
	void setIsPrefix(bool b);
	void setIsSuffix(bool b);
	void setGroupID(int id);
	void setIsReference(bool b);
	enum RegexType{Blank,Choice,PlusRepetition,StarRepetition,QuoraRepetition,
		Sequence,Primitive,SpecialChars,AnyChar,Prefix,Suffix,MultiChoice,LookAround};
	RegexType type;
	static shared_ptr<Regex> blank;
	shared_ptr<vector<shared_ptr<NFA_state>>> list1(shared_ptr<NFA_state> outp);
	shared_ptr<vector<shared_ptr<NFA_state>>> append(shared_ptr <vector<shared_ptr<NFA_state>>>l1, shared_ptr<vector<shared_ptr<NFA_state>>
	>l2);
	void patch1(shared_ptr<vector<shared_ptr<NFA_state>>>& l, shared_ptr<NFA_state> s);
	void patch2(shared_ptr<vector<shared_ptr<NFA_state>>>& l, shared_ptr<NFA_state> s);
	void setGroupState(bool isGroup, bool isReference, int referNo);
};
class Blank :public Regex
{
public:
	Blank() {
		this->type = RegexType::Blank; 
		shared_ptr<NFA_state> blank(new NFA_state(Type::Blank));
		this->start = blank;
		this->out1 = list1(blank);
	}
	string toString();
	shared_ptr<Regex> copy();
};
class AnyChar :public Regex
{
public:
	AnyChar() { this->type = RegexType::AnyChar; 
	shared_ptr<NFA_state> s(new NFA_state(Type::AnyChar));
	this->start = s;
	this->out1 = list1(s);
	min_len = max_len = 1;
	}
	string toString();
	shared_ptr<Regex> copy();
};

class Choice :public Regex
{
public:
	shared_ptr<Regex>thisOne;
	shared_ptr<Regex>thatOne;
	Choice(shared_ptr<Regex> thisOne, shared_ptr<Regex> thatOne)
	{
		this->thisOne = thisOne;
		this->thatOne = thatOne;
		shared_ptr<NFA_state> s = make_shared<NFA_state>(Type::Split, thisOne->start, thatOne->start);
		this->start = s;
		if (thisOne->start) {
			thisOne->start->reverse_out.emplace_back(s);
		}
		if (thatOne->start) {
			thatOne->start->reverse_out.emplace_back(s);
		}
		this->out1 = append(thisOne->out1,thatOne->out1);
		this->out2 = append(thisOne->out2, thatOne->out2);
		this->type = RegexType::Choice;
		min_len = min(thisOne->min_len, thatOne->min_len);
		max_len = max(thisOne->max_len, thatOne->max_len);
	}
	string toString();
	shared_ptr<Regex> copy();
};

class MultiChoice :public Regex
{
#define ARRAY_SIZE  256
private:
	int c1 =  1;
	int c2 = 0;
public:
	bitset<256> scale;
	MultiChoice() {
		this->type = RegexType::MultiChoice;
		min_len = max_len = 1;
	}
	MultiChoice(char c1, char c2):c1(c1),c2(c2)
	{
		this->type = RegexType::MultiChoice;
		if (c1 < 0 || c2>255)
			throw 2;
		for (int i = c1;i <= c2;i++)
		{
			scale .set(i);
		}
	}
	string toString();
	void andOp(shared_ptr<MultiChoice> other);
	void orOp(shared_ptr<MultiChoice> other);
	void reverse();
	void generateNFA();
	shared_ptr<Regex> copy();
};

class PlusRepetition :public Regex
{
public:
	shared_ptr<Regex>internal;
	PlusRepetition(shared_ptr<Regex> re)
	{

		internal = re;
		this->type = RegexType::PlusRepetition;
		if (internal->start->c == Type::GroupStart)
		{
			this->start = internal->group_copy->start;
			shared_ptr<NFA_state> s = make_shared<NFA_state>(Type::Split, this->start, nullptr);
			if (start) {
				start->reverse_out.emplace_back(s);
			}
			patch1(internal->group_copy->out1, s);
			patch2(internal->group_copy->out2, s);
			s->out2 = internal->start;
			if (internal->start)
				internal->start->reverse_out.emplace_back(s);
			this->start = s;
			this->out1 = internal->out1;
			this->out2 = internal->out2;

		}
		else
		{
			shared_ptr<NFA_state> s = make_shared<NFA_state>(Type::Split, internal->start, nullptr);
			if (internal->start) {
				internal->start->reverse_out.emplace_back(s);
			}
			patch1(internal->out1, s);
			patch2(internal->out2, s);
			this->start = internal->start;
			vector<shared_ptr<NFA_state>> new_vector;
			this->out1 = make_shared<vector<shared_ptr<NFA_state>>>(new_vector);
			this->out2 = list1(s);
			min_len = re->min_len;
			max_len = INT_MAX;
		}
	}
	shared_ptr<Regex> copy();
	string toString();
};
class Prefix :public Regex
{
public:
	shared_ptr<Regex> internal;
	Prefix(shared_ptr<Regex> re) {
		this->internal = re;
		this->type = RegexType::Prefix;
	}
	string toString();
	shared_ptr<Regex> copy();
};

class Primitives :public Regex
{
public:
	char c;
	Primitives(char c)
	{
		this->c = c;
		this->type = RegexType::Primitive;
		shared_ptr<NFA_state> s = make_shared<NFA_state>(c);
		this->start = s;
		this->out1 = list1(s);
		min_len = max_len = 1;
	}
	string toString();
	shared_ptr<Regex> copy();
};

class QuoraRepetition :public Regex
{
public:
	shared_ptr<Regex> internal;
	QuoraRepetition(shared_ptr<Regex> re)
	{
		this->internal = re;
		this->type = RegexType::QuoraRepetition;
			shared_ptr<NFA_state> s = make_shared<NFA_state>(Type::Split, re->start, nullptr);
			if (re->start)
				re->start->reverse_out.emplace_back(s);
			this->start = s;
			this->out1 = re->out1;
			this->out2 = append(re->out2, list1(s));
			min_len = 0;
			max_len = re->max_len;
	}
	string toString();
	shared_ptr<Regex> copy();
};
class Sequence :public Regex
{
public:
	shared_ptr<Regex> first;
	shared_ptr<Regex> second;
	Sequence(shared_ptr<Regex> first, shared_ptr<Regex> second)
	{
		this->first = first;
		this->second = second;
		this->type = RegexType::Sequence;
		if (first == blank)
		{
			this->start = second->start;
			this->out1 = second->out1;
			this->out2 = second->out2;
		}
		this->start = first->start;
		patch1(first->out1, second->start);
		patch2(first->out2, second->start);
		this->out1 = second->out1;
		this->out2 = second->out2;
		min_len = first->min_len + second->min_len;
		if (first->max_len == INT_MAX || second->max_len == INT_MAX)
			max_len = INT_MAX;
		else
			max_len = first->max_len + second->max_len;
	}
	string toString();
	shared_ptr<Regex> copy();
};

class SpecialChars :public Regex
{
public:
	char special;
	SpecialChars(char c)
	{
		special = c;
		this->type = RegexType::SpecialChars;
		shared_ptr<NFA_state> s;
		switch (c)
		{
		case 'b':
			s = make_shared<NFA_state>(Type::MetaCharacter);
			break;
		case 's':
			s = make_shared<NFA_state>(Type::SingleLetter);
			break;
		case 'S':
			s = make_shared<NFA_state>(Type::NonSingleLetter);
			break;
		case 'w':
			s = make_shared<NFA_state>(Type:: Word);
			break;
		case 'W':
			s = make_shared<NFA_state>(Type::NonWord);
			break;
		case 'd':
			s = make_shared<NFA_state>(Type::Digit);
			break;
		case 'D':
			s = make_shared<NFA_state>(Type::NonDigit);
			break;
		case 'r':
			s = make_shared<NFA_state>('\r');
			break;
		case 'n':
			s = make_shared<NFA_state>('\n');
			break;
		case 't':
			s = make_shared<NFA_state>('\t');
			break;
		case '\\':
			s = make_shared<NFA_state>('\\');
			break;
		case '\'':
			s = make_shared<NFA_state>('\'');
			break;
		case '\"':
			s = make_shared<NFA_state>('\"');
			break;
		case '?':
		case '.':
		case '*':
		case '|':
		case '+':
		case '(':
		case ')':
		case '{':
		case '}':
		case '^':
		case '$':
		case '/':
		case '[':
		case ']':
			s = make_shared<NFA_state>(c);
			break;
		case 'v':
			s = make_shared<NFA_state>('\v');
			break;
		case 'f':
			s = make_shared<NFA_state>('\f');
			break;
		default:
			s = make_shared<NFA_state>(c);
			break;
		}
		this->start = s;
		this->out1 = list1(s);
	}
	shared_ptr<Regex> copy();
	string toString();
};

class StarRepetition :public Regex
{
public:
	shared_ptr<Regex> internal;
	StarRepetition(shared_ptr<Regex> internal)
	{
		this->internal = internal;
		this->type = RegexType::StarRepetition;
		if (internal->start->c == Type::GroupStart)
		{
			this->start = internal->group_copy->start;
			shared_ptr<NFA_state> s = make_shared<NFA_state>(Type::Split, this->start, nullptr);
			if (start) {
				start->reverse_out.emplace_back(s);
			}
			patch1(internal->group_copy->out1, s);
			patch2(internal->group_copy->out2, s);
			shared_ptr<NFA_state> s2 = make_shared<NFA_state>(Type::Split, internal->start, nullptr);
			if (internal->start)
				internal->start->reverse_out.emplace_back(s2);
			s->out2 = s2;
			s2->reverse_out.emplace_back(s);

			this->start = s;
			this->out1 = internal->out1;
			this->out2 = append(internal->out2, list1(s2));
			
		}
		else
		{
			shared_ptr<NFA_state> s = make_shared<NFA_state>(Type::Split, internal->start, nullptr);
			if (internal->start) {
				internal->start->reverse_out.emplace_back(s);
			}
			patch1(internal->out1, s);
			patch2(internal->out2, s);
			this->start = s;
			this->out1 = make_shared<vector<shared_ptr<NFA_state>>>();
			this->out2 = list1(s);
		}
	}
	shared_ptr<Regex> copy();
	string toString();
};

class Suffix :public Regex
{
public:
	shared_ptr<Regex> internal;
	Suffix(shared_ptr<Regex> re)
	{
		this->internal = re;
		this->type = RegexType::Suffix;
	}
	shared_ptr<Regex> copy();
	string toString();
};

class LookAround :public Regex
{
public:
	bool ahead;
	bool positive;
	bool hasRefer;
	bool hasGroup;
	string in;
	shared_ptr<Regex> r;
	LookAround(shared_ptr<Regex>, bool ahead, bool positive,bool hasRefer=false,bool hasGroup=false);
	shared_ptr<Regex> copy();
	string toString();
};

class BackRefer :public Regex
{
	shared_ptr<Regex> copy();
	string toString();
};