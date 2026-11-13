  #pragma once
#include"main.h"
#include "../cli_output.h"
#include"ConstructandMatch.h"
#include"ConstructandMatchWLA.h"
#include "Type.h"
#include<regex>
#include<fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/sysinfo.h>
//#include <sys/time.h>
#include <unistd.h>
using namespace std;
clock_t total_FA_time = 0;
bool phase_timing_available = false;
clock_t total_match_time = 0;
clock_t total_time = 0;
clock_t kkmptime=0;
clock_t Identime = 0;
clock_t checktime = 0;
clock_t ttemptime=0;
clock_t placetime1 = 0;
bool match(string E, string w)
{
	//cout << E.length() <<" " << w.length() << endl;
	//cout<<E<<" "<<w<<endl;
	clock_t begin_time = clock();
	shared_ptr<Production_FA> p(new Production_FA(E, w));
	phase_timing_available = p->hasRefer;
	if (!p->hasRefer)
	{
		//cout<<p->res;
		total_time += clock() - begin_time;
		return p->res;
	}
	clock_t FA_time = clock();
	total_FA_time=FA_time-begin_time;

	// cout<<"debug00 "<<"referEndMaptime "<<p->referEndmap_time<<endl;

	for (auto temp : p->position_edges[0][0])
		if (temp.first == -1 && temp.second == -1)
			return true;


	
	shared_ptr<Construct_Pb> C_P (new Construct_Pb);
	if (p->lookarounds->size() > 0) {
		C_P = make_shared<Construct_Pb_WLA>(p->groupInfo);
	}
	C_P->lookarounds=p->lookarounds;
	C_P->referCount=referCount;
	C_P->IsMatch = false;
	if (p->section.size() == 0)
	{
		clock_t end_time=clock();
        	total_match_time += end_time - FA_time;
       		total_time += end_time - begin_time;
		return p->res;
	}
	clock_t idenbegin = clock();
	//C_P->checkmatch_time = 0;
	C_P->iden(w,p->section,p->position_edges,p->groups);
	
	
	
	
	
	
	
	if (C_P->IsMatch) {
		clock_t end_time = clock();
		total_match_time += end_time - FA_time;
		total_time += end_time - begin_time;
		return true;
	}
	
	
	clock_t end_time=clock();
	total_match_time += end_time - FA_time;
	total_time += end_time - begin_time;
	
	return false;
}

string getString(ifstream  &infile)
{
	string w = "";
	char c;
	while ((c = infile.get()) != '\0')
	{
		w += c;
	}
	c = infile.get();
	return w;
}

int main(int argc, char* argv[])
{
    return rematch_cli::run(argc, argv, match, &total_FA_time, &phase_timing_available);
}
