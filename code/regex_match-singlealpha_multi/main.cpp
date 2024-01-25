  #pragma once
#include"main.h"
#include"ConstructandMatch.h"
#include "Type.h"
#include<regex>
#include<fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
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
bool match(string E,string w)
{
	//cout << E.length() <<" " << w.length() << endl;
	//cout<<E<<" "<<w<<endl;
	clock_t begin_time = clock();
	shared_ptr<Production_FA> p (new Production_FA(E, w));
	int num = 0;
	
	clock_t FA_time = clock();
	total_FA_time=p->ttime;
	//for (auto w : p->section)
	//	cout << w.first << "," << w.second << endl;
	total_time += FA_time - begin_time;
	return p->res;
	/*cout << endl;
	for (auto t : p->groups)
		cout << t << " ";
	cout << endl;
	for (int i = 0; i < p->position_edges.size(); i++)
	{
		for (int j = 0; j < p->position_edges[i].size();j++)
		{
			for (auto temp : p->position_edges[i][j])
				cout << i - 1 << " " << j - 1 << " " << temp<<endl;
		}
	}*/
	/*Construct_Pb construct_Pb(w, p);
	construct_Pb.checkmatch(p->position_edges, -1, 0, 0, p->groups);
	cout << construct_Pb.count<<endl;
	clock_t end_time=clock();
	total_match_time += end_time - FA_time;
	total_time += end_time - begin_time;
	return construct_Pb.IsMatch;*/
	/*cout << "FA_time:" << FA_time - begin_time<<endl;
	cout << "iden_time:" << iden_time - FA_time << endl;
	cout << "checkmatch_time" << end_time - iden_time << endl;
	cout <<"total_time:"<< end_time - begin_time << endl;*/
	
	
	/*
(a|aa)*a*(aa)*\1
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaab
  */
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
	int current_pid = GetCurrentPid();
	float init_memory=GetMemoryUsage(current_pid);
	string E = argv[1];
	string w = argv[2];
	 cout<<match(E,w) << endl;
	 //regex r(E);
	 //cout << regex_match(w, r)<<endl;
	 cout  <<(double)total_time/1000 << " "<<(double)total_FA_time/1000<<" ";
	float memory_usage = GetMemoryUsage(current_pid);
        std::cout <<memory_usage-init_memory << std::endl;	
	return 0;

	
}
