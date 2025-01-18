#pragma once
#include"Regex.h"
#include<stdlib.h>
#include<iostream>
#include<vector>
#include<assert.h>
#include<map>
class Parser
{
private:
	int position = 0;
	int groupID = 0;
	bool isPrefix = false;//�Ƿ�Ϊǰ׺
	bool isSuffix = false;//�Ƿ�Ϊ��׺
	string input;
	char peek();
	void eat(char c);
	char next();
	bool more();
	shared_ptr<Regex> regex();
	shared_ptr<Regex> term();
	shared_ptr<Regex> factor();
	shared_ptr<Regex> base();
	shared_ptr<Regex> multichoice();
	shared_ptr<Regex> singlechoice(bool in=false);
	shared_ptr<Regex> andchoice();
public:
	shared_ptr<Regex> re = nullptr;
	shared_ptr<NFA_state> group_start_state;
	shared_ptr<NFA_state> matchState;
	int referID = -1;
	int curreferCount = 0;
	int stateNo;
	int CGs;
	bool hasRefer = false;
	shared_ptr<vector<shared_ptr<LookAroundState>>> lookarounds = make_shared<vector<shared_ptr<LookAroundState>>>();
	Parser(string in);
	shared_ptr<Regex>parse();
	//shared_ptr<Regex> blank = static_pointer_cast<Regex>(make_shared<Blank>());
	map<int, shared_ptr<Regex>> group;
	vector<shared_ptr<NFA_state>> list1(shared_ptr<NFA_state> outp);
	vector<shared_ptr<NFA_state>> append(vector<shared_ptr<NFA_state>>l1, vector<shared_ptr<NFA_state>>l2);
	static void patch1(shared_ptr<vector<shared_ptr<NFA_state>>>& l, shared_ptr<NFA_state> s);
	static void patch2(shared_ptr<vector<shared_ptr<NFA_state>>>& l, shared_ptr<NFA_state> s);
};
