#pragma once
#include<climits>
#include<ctime>
class Type {
public:
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
	const static int MatchState = 257;
	const static int RevMatchState = 258;
	const static int MetaCharacter = 304;
};
