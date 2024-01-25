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

	//cout << p->groups[0].size() << endl;
	//for (int k = 0; k <= referCount; k++)
	//{
	//	for (int i = 0; i < p->groups.size(); i++)
	//	{
	//		for (int j = i; j < p->groups[i].size(); j++)
	//			if (p->groups[k][i][j]) {
	//				cout << k << " " << i << " " << j << endl;
	//			}
	//	}
	//}
	//for (int i = 0; i < p->position_edges.size(); i++)
	//{
	//	for (int j = 0; j < p->position_edges[i].size(); j++)
	//		for (auto temp : p->position_edges[i][j])
	//			cout << i-1 << " " << j-1 << " " << temp.first << " " << temp.second << endl;
	//}




	
	shared_ptr<Construct_Pb> C_P (new Construct_Pb);
	C_P->lookarounds=p->lookarounds;
	C_P->referCount=referCount;
	C_P->IsMatch = false;
	if (p->groups.size() <= 1) {
		clock_t end_time = clock();
		total_match_time += end_time - FA_time;
		total_time += end_time - begin_time;
		//cout << p->groups.size() << endl;
		//cout << p->position_edges.size() << endl;
		if (p->position_edges.size() != 0) {
			//cout << "匹配" << endl;
			return true;
		}
		else
			return false;
		//return true;
	}
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
	//cout <<"checkmatch:"<< C_P->checkmatch_time << endl;
	/*kkmptime += C_P->kmptime;
	ttemptime += C_P->temptime;*/
	

	//clock_t iden_time = clock();
	//cout << "number" << C_P->number << endl;
	if (C_P->IsMatch) {
		clock_t end_time = clock();
		total_match_time += end_time - FA_time;
		total_time += end_time - begin_time;
		return true;
	}
	//cout << C_P->temp_Pb.size() << endl;
	//cout << C_P->Pb.size() << endl;
	//int inn = -1;
		//for (auto& inner : C_P->Pb) {    // loop over the outer vector
		//	auto it = inner.begin();    // get iterator for the inner vector
		//	std::cout << '<';
		//	if (it != inner.end()) {
		//		// print the first pair
		//		std::cout << '(' << it->first << ',' << it->second << ')';
		//		// print the rest of the pairs
		//		for (++it; it != inner.end(); ++it) {
		//			std::cout << " (" << it->first << ',' << it->second << ')';
		//		}
		//	}
		//	std::cout << ">\n";
		//}
	//for (int i = 0; i < C_P->Pb.size(); i++) {
	//	for (int j = 0; j < C_P->Pb[i].size(); j++) {
	//		vector<pair<int, int>> w1;
	//		w1.assign(C_P->Pb[i].begin() + j, C_P->Pb[i].end());
	//		//cout << C_P->Pb[i].size() << endl;
	//	//	auto inner = p->groups[0].begin();
	//	//	bool firstflag=false;
	//		//cout << w1[0].first <<"  " << w1[0].second << endl;	
	//		C_P->visited.clear();
	//		C_P->IsMatch = false;
	//		clock_t checkbegin = clock();
	//		//cout << " michu" << endl;
	//		/*for (int t = 0; t < w1.size(); t++) {
	//			cout<< w1[t].first << " " << w1[t].second << endl;
	//		}*/
	//		C_P->checkmatch(p->position_edges, w1, 0, w1[0].second);

	//		clock_t checkend = clock();
	//		checktime += checkend - checkbegin;
	//		if (C_P->IsMatch) {
	//			//cout << "匹配" << endl;
	//			clock_t end_time = clock();
	//			total_match_time += end_time - FA_time;
	//			total_time += end_time - begin_time;
	//			return true;
	//		}
	//	
	//	}
	//}
	
	clock_t end_time=clock();
	total_match_time += end_time - FA_time;
	total_time += end_time - begin_time;
	
	/*cout << "FA_time:" << FA_time - begin_time<<endl;
	cout << "iden_time:" << iden_time - FA_time << endl;
	cout << "checkmatch_time" << end_time - iden_time << endl;
	cout <<"total_time:"<< end_time - begin_time << endl;*/
	return false;
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
	cout << match(argv[1], argv[2])<<" ";
	cout  <<(double)total_FA_time/1000<<" "<<(double)total_match_time/1000<<" "<< (double)total_time/1000 << " ";
	float memory_usage = GetMemoryUsage(current_pid);
	std::cout <<memory_usage-init_memory << std::endl;	
	return 0;

	
}
