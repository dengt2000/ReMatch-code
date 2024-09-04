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
	//shared_ptr<vector<shared_ptr<LookAroundState>>> lookaheads = make_shared<vector<shared_ptr<LookAroundState>>>();
	//shared_ptr<vector<shared_ptr<LookAroundState>>> lookbehinds = make_shared<vector<shared_ptr<LookAroundState>>>();
	shared_ptr<vector<shared_ptr<LookAroundState>>> lookarounds = make_shared<vector<shared_ptr<LookAroundState>>>();
	Parser(string in);
	shared_ptr<Regex>parse();
	//shared_ptr<Regex> blank = static_pointer_cast<Regex>(make_shared<Blank>());
	map<int, shared_ptr<Regex>> group;
	vector<shared_ptr<NFA_state>> list1(shared_ptr<NFA_state> outp);
	vector<shared_ptr<NFA_state>> append(vector<shared_ptr<NFA_state>>l1, vector<shared_ptr<NFA_state>>l2);
	static void patch1(shared_ptr<vector<shared_ptr<NFA_state>>>& l, shared_ptr<NFA_state> s);
	static void patch2(shared_ptr<vector<shared_ptr<NFA_state>>>& l, shared_ptr<NFA_state> s);
	/*
	const static int Split = 256;
	const static int Tab = 280;
	const static int Newline = 281;
	const static int Return = 282;
	const static int Digit = 283;
	const static int WhiteSpace = 284;
	const static int NonWhiteSpace = 285;
	const static int SingleLetter = 286;
	const static int NonSingleLetter = 287;
	const static int NewPage = 288;
	const static int Escape = 289;
	const static int Boundary = 290;
	const static int NonBoundary = 291;
	const static int AnyChar = 292;
	const static int UnSupported = 293;
	const static int Scale = 295;
	const static int NegScale = 298;
	const static int Word = 296;
	const static int NonWord = 297;
	const static int Blank = 294;
	const static int GroupStart = 299;
	const static int GroupEnd = 300;
	const static int ReferStart = 301;
	const static int ReferEnd = 302;
	const static int LookAround = 303;
	//const static int Digit=304;
	*/
};
