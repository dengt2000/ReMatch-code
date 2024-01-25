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

bool match(string E,string w)
{
	//cout << E.length() <<" " << w.length() << endl;
	//cout<<E<<" "<<w<<endl;
	clock_t begin_time = clock();
	shared_ptr<Production_FA> p (new Production_FA(E, w));
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
	//placetime1 += p->placetime1;
	/*for (int i = 0; i < p->section.size(); i++) {
		cout << "sec" << p->section[i].first << p->section[i].first << endl;
	}*/
	// cout << total_FA_time<<endl;
	// for (int i = 0; i < p->referCount; i++)
	// 	cout << "groups[" << i << "]:" << p->referNo2num[i];
	// cout << endl;
	
	/*for (int i = 0; i < p->referCount; i++)
	{
		cout << "groups[" << i << "]:";
		for (int j = 0; j < w.length() + 2; j++)
			for (int k = 0; k < w.length() + 2; k++)
				if (p->groups[i][j][k])
					cout << j << "," << k <<" ";
		cout << "\n";
	}*/
	// for (int i = 0; i < p->position_edges.size(); i++)
	// {
	// 	for (int j = 0; j < p->position_edges[i].size(); j++)
	// 		for(int k=0;k<p->position_edges[i][j].size();k++)
	// 		for (auto temp : p->position_edges[i][j][k])
	// 			cout << i-1 << " " << j-1 <<" "<< k << "->" << get<0>(temp) << " " << get<1>(temp) << " " << get<2>(temp) << endl;
	// }  

	//return true;
	/*
	if (p->referCount == 0)
	{
		regex reg(E);
		bool b= std::regex_match(w, reg);
		clock_t end_time = clock();
		total_match_time += end_time - FA_time;
		total_time += end_time - begin_time;
		return b;
	}
	*/
	/*cout << p->referCount << "\n";
	
	for (int i = 0;i <= p->referCount;i++)
	{
		cout << "groups[" << i << "]";
		for (auto w : p->groups[i])
			cout << "(" << w.first << "," << w.second << ")"<<" ";
		cout << "\n";
	}
	for (auto &w : p->position_edges)
	{
		cout <<"("<< w.first.first << "," << w.first.second<<")" << "->";
		for (auto &w2 : w.second)
			cout << "(" << w2.first << "," << w2.second << ")"<<"匹配";
		cout << "\n";
		
	}*/
	
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
			//cout << p->groups.size() << endl;
			//cout << p->position_edges.size() << endl;
			if (p->position_edges.size() != 0) {
				

				return true;
			}
			else
				return false;
			//return true;
		}
		if (p->section.size() == 0)
		{
			clock_t end_time = clock();
			total_match_time += end_time - FA_time;
			total_time += end_time - begin_time;
			return p->res;
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
		//cout << p->groups.size() << endl;
		if (p->referCount <= 1) {
			clock_t end_time = clock();
			total_match_time += end_time - FA_time;
			total_time += end_time - begin_time;
			//cout << p->groups.size() << endl;
			//cout << p->position_edges.size() << endl;
			if (p->position_edges.size() != 0) {

				return true;
			}
			else
				return false;
			//return true;
		}
		if (p->section.size() == 0)
		{
			clock_t end_time = clock();
			total_match_time += end_time - FA_time;
			total_time += end_time - begin_time;
			return p->res;
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

				return true;
			}
			else
				return false;
			//return true;
		}
		if (p->section.size() == 0)
		{
			clock_t end_time = clock();
			total_match_time += end_time - FA_time;
			total_time += end_time - begin_time;
			return p->res;
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

				return true;
			}
			else
				return false;
			//return true;
		}
		if (p->section.size() == 0)
		{
			clock_t end_time = clock();
			total_match_time += end_time - FA_time;
			total_time += end_time - begin_time;
			return p->res;
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

				return true;
			}
			else
				return false;
			//return true;
		}
		if (p->section.size() == 0)
		{
			clock_t end_time = clock();
			total_match_time += end_time - FA_time;
			total_time += end_time - begin_time;
			return p->res;
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

	//clock_t idenbegin = clock();
	//	void multi_iden(string& w, vector<pair<int, int>>& section, vector<vector<vector<vector<tuple<int, int, int>>>>> position_edges, vector<vector<vector<int>>>& groups, int groupNum);//扩展

	

	//C_P->checkmatch_time = 0;
	//cout <<"checkmatch:"<< C_P->checkmatch_time << endl;
	/*kkmptime += C_P->kmptime;
	ttemptime += C_P->temptime;*/
	

	//clock_t iden_time = clock();
	//cout << "number" << C_P->number << endl;
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
	// regex r(argv[1]);
	// cout << regex_match(argv[2], r)<<" ";
	cout  <<(double)total_FA_time/1000<<" "<<(double)total_match_time/1000<<" "<< (double)total_time/1000 << " ";
	float memory_usage = GetMemoryUsage(current_pid);
	std::cout <<memory_usage-init_memory << std::endl;	

	return 0;
	
}
