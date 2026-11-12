  #pragma once
#include"main.h"
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
#define VMPEAK_LINE 17
using namespace std;
clock_t total_FA_time = 0;
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
	if (!p->hasRefer)
	{
		//cout<<p->res;
		total_time += clock() - begin_time;
		return p->res;
	}
	clock_t FA_time = clock();
	total_FA_time=FA_time-begin_time;

	// cout<<"debug00 "<<"referEndMaptime "<<p->referEndmap_time<<endl;

	 //cout << p->groups[0].size() << endl;
	 for (int k = 0; k < 1; k++)
	 {
	 	for (int i = 0; i < p->groups[k].size(); i++)
	 	{
	 		for (int j = i; j < p->groups[k][i].size(); j++)
	 			if (p->groups[k][i][j]) {
	 				cout << k << " " << i << " " << j << endl;
	 			}
	 	}
	 }
	
	for (int i = 0; i < p->position_edges.size(); i++) {
		for (int j = 0; j < p->position_edges[i].size(); j++) {
			for (auto temp : p->position_edges[i][j]) {
				cout << i - 1 << " " << j - 1 << " " << temp.first << " " << temp.second << endl;
			}
		}
	}

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

 inline int GetCurrentPid()
 {
 	    return getpid();
 }
 inline float GetMemoryUsage(int pid)
 {
 	char file_name[64] = { 0 };
 	FILE* fd;
 	char line_buff[512] = { 0 };
 	sprintf(file_name, "/proc/%d/status", pid);
 	fd = fopen(file_name, "r");
 	if (nullptr == fd)
 		return 0;
 	char name[64];
 	int vmrss = 0;
 	for (int i = 0; i < VMPEAK_LINE - 1; i++)
 		fgets(line_buff, sizeof(line_buff), fd);
 	fgets(line_buff, sizeof(line_buff), fd);
 	sscanf(line_buff, "%s %d", name, &vmrss);
 	fclose(fd);
 	// cnvert VmRSS from KB to MB
 	return vmrss / 1024.0;
}
int main(int argc,char* argv[])
{
	string exp = argv[1];
	string w = argv[2];
	cout << match(exp, w) << " ";
	return 0;
}
