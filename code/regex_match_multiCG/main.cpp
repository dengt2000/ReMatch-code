  #pragma once
#include"main.h"
#include"ConstructandMatch.h"
#include"ConstructandMatch.cpp"

#include "Type.h"
#include<regex>
#include<fstream>
#include <iostream>
#include <sys/stat.h>
//#include <sys/sysinfo.h>
//#include <sys/time.h>
//#include <unistd.h>
#define VMPEAK_LINE 17
using namespace std;
clock_t total_FA_time = 0;
clock_t total_match_time = 0;
clock_t total_time = 0;
clock_t match_time=0;
clock_t total_iden=0;
clock_t total_check=0;
template <typename... TArgs>
float memory_usage;
bool match(string E,string w)
{
	//cout << E.length() <<" " << w.length() << endl;
	//cout<<E<<" "<<w<<endl;
	clock_t begin_time = clock();
	shared_ptr<Production_FA> p (new Production_FA(E, w));
	// cout<<111111111<<endl;
	//cout<< "groupNum:"<<p->groupNum<<endl;
	int num = 0;
	if (!p->hasRefer)
	{
		//cout<<p->res;
		total_time+=clock()-begin_time;
		return p->res;
	}
	clock_t FA_time = clock();
	//for (auto w : p->section)
	//	cout << w.first << "," << w.second << endl;
	total_FA_time += FA_time - begin_time;

	
switch (p->groupNum)
	{


	case 1:{
		std::shared_ptr<Construct_Pb<int, int, int>> C_P = std::make_shared<Construct_Pb<int, int,int>>();
		C_P->lookarounds = p->lookarounds;
		C_P->referCount = p->referCount;
		C_P->IsMatch = false;
		//cout << p->groups.size() << endl;
		if (p->referCount <= 1) {
			clock_t end_time = clock();
			total_match_time += end_time - FA_time;
			total_time += end_time - begin_time;
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
			// cout<<111<<endl;
			C_P->length=w.length();
			C_P->init_data_mem();
			C_P->multi_check_empty_section(p->position_edges, -1, -1, 1, p->referNo2num);
			if (C_P->IsMatch) {
				clock_t end_time = clock();
				total_match_time += end_time - FA_time;
				total_time += end_time - begin_time;
				return true;
			}else{
				clock_t end_time = clock();
				total_match_time += end_time - FA_time;
				total_time += end_time - begin_time;
				return false;
			}
		}
		C_P->multi_iden(w, p->section, p->position_edges, p->groups, p->groupNum, p->referNo2num);
		if (C_P->IsMatch) {
			clock_t end_time = clock();
			total_match_time += end_time - FA_time;
			total_time += end_time - begin_time;
			return true;
		}
		break;

	}
	case 2: {
		
		std::shared_ptr<Construct_Pb<int, int, int,int>> C_P = std::make_shared<Construct_Pb<int, int, int,int>>();
		C_P->lookarounds = p->lookarounds;
		C_P->referCount = p->referCount;
		C_P->IsMatch = false;
		// cout << p->groups.size() << endl;
		if (p->referCount <= 1) {
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
		// cout<<p->section.size()<<endl;
		 if (p->section.size() == 0)
		{
			C_P->length=w.length();
			C_P->init_data_mem();
			C_P->multi_check_empty_section(p->position_edges, -1, -1, 1, p->referNo2num);
			if (C_P->IsMatch) {
				clock_t end_time = clock();
				total_match_time += end_time - FA_time;
				total_time += end_time - begin_time;
				return true;
			}else{
				clock_t end_time = clock();
				total_match_time += end_time - FA_time;
				total_time += end_time - begin_time;
				return false;
			}
		}
		C_P->multi_iden(w, p->section, p->position_edges, p->groups, p->groupNum, p->referNo2num);
		if (C_P->IsMatch) {
			clock_t end_time = clock();
			total_match_time += end_time - FA_time;
			total_time += end_time - begin_time;
			return true;
		}
		break;

	}
	case 3: {
		std::shared_ptr<Construct_Pb<int, int, int, int,int>> C_P = std::make_shared<Construct_Pb<int, int, int, int,int>>();
		C_P->lookarounds = p->lookarounds;
		C_P->referCount = p->referCount;
		C_P->IsMatch = false;
		//cout << p->groups.size() << endl;
		if (p->referCount <= 1) {
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
		{			C_P->length=w.length();
			C_P->init_data_mem();

			C_P->multi_check_empty_section(p->position_edges, -1, -1, 1, p->referNo2num);
			if (C_P->IsMatch) {
				clock_t end_time = clock();
				total_match_time += end_time - FA_time;
				total_time += end_time - begin_time;
				return true;
			}else{
				clock_t end_time = clock();
				total_match_time += end_time - FA_time;
				total_time += end_time - begin_time;
				return false;
			}
		}
		C_P->multi_iden(w, p->section, p->position_edges, p->groups, p->groupNum, p->referNo2num);
		if (C_P->IsMatch) {
			clock_t end_time = clock();
			total_match_time += end_time - FA_time;
			total_time += end_time - begin_time;
			return true;
		}
		break;

	}
	case 4: {
		std::shared_ptr<Construct_Pb<int, int, int, int, int, int>> C_P = std::make_shared<Construct_Pb<int, int, int, int, int, int>>();
		C_P->lookarounds = p->lookarounds;
		C_P->referCount = p->referCount;
		C_P->IsMatch = false;
		//cout << p->groups.size() << endl;
		if (p->referCount <= 1) {
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
						C_P->length=w.length();
			C_P->init_data_mem();

			C_P->multi_check_empty_section(p->position_edges, -1, -1, 1, p->referNo2num);
			if (C_P->IsMatch) {
				clock_t end_time = clock();
				total_match_time += end_time - FA_time;
				total_time += end_time - begin_time;
				return true;
			}else{
				clock_t end_time = clock();
				total_match_time += end_time - FA_time;
				total_time += end_time - begin_time;
				return false;
			}
		}

		C_P->multi_iden(w, p->section, p->position_edges, p->groups, p->groupNum, p->referNo2num);
		if (C_P->IsMatch) {
			clock_t end_time = clock();
			total_match_time += end_time - FA_time;
			total_time += end_time - begin_time;
			return true;
		}

		break;
	}
	case 5: {
		std::shared_ptr<Construct_Pb<int, int, int, int, int, int, int>> C_P = std::make_shared<Construct_Pb<int, int, int, int, int, int, int>>();
		C_P->lookarounds = p->lookarounds;
		C_P->referCount = p->referCount;
		C_P->IsMatch = false;
		//cout << p->groups.size() << endl;
		if (p->referCount <= 1) {
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
						C_P->length=w.length();
			C_P->init_data_mem();

			C_P->multi_check_empty_section(p->position_edges, -1, -1, 1, p->referNo2num);
			if (C_P->IsMatch) {
				clock_t end_time = clock();
				total_match_time += end_time - FA_time;
				total_time += end_time - begin_time;
				return true;
			}else{
				clock_t end_time = clock();
				total_match_time += end_time - FA_time;
				total_time += end_time - begin_time;
				return false;
			}
		}
		C_P->multi_iden(w, p->section, p->position_edges, p->groups, p->groupNum, p->referNo2num);
		if (C_P->IsMatch) {
			clock_t end_time = clock();
			total_match_time += end_time - FA_time;
			total_time += end_time - begin_time;
			return true;
		}
		break;
	}
	default:
		break;
	}	

	
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
	// regex r(argv[1]);
	// cout << regex_match(argv[2], r)<<" ";
	cout  <<(double)total_FA_time/1000<<" "<<(double)total_match_time/1000<<" "<< (double)total_time/1000 << " ";
	float memory_usage=GetMemoryUsage(current_pid);
	std::cout <<memory_usage-init_memory << std::endl;	

	return 0;
	
}
