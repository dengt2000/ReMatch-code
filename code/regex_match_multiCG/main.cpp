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
	//placetime1 += p->placetime1;
	// cout<<"section_size:"<<p->section.size()<<endl; 
	// for (int i = 0; i < p->section.size(); i++) {
	//  	cout << "sec" << p->section[i].first << p->section[i].second << endl;
	//  }
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
	// 		for(int k=0;k<p->position_edges[i][j].size();k++){
	// 			// cout<<k<<endl;
	// 			for (auto temp : p->position_edges[i][j][k])
	// 			cout << i-1 << " " << j-1 <<" "<< k << "->" << get<0>(temp) << " " << get<1>(temp) << " " << get<2>(temp) << endl;
	// 		}
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
	// cout << p->referCount << "\n";
	
	// for (int i = 0;i <= p->referCount;i++)
	// {
	// 	cout << "groups[" << i << "]";
	// 	for (auto w : p->groups[i])
	// 		cout << "(" << w.first << "," << w.second << ")"<<" ";
	// 	cout << "\n";
	// }
	// for (auto &w : p->position_edges)
	// {
	// 	cout <<"("<< w.first.first << "," << w.first.second<<")" << "->";
	// 	for (auto &w2 : w.second)
	// 		cout << "(" << w2.first << "," << w2.second << ")"<<"匹配";
	// 	cout << "\n";
		
	// }
// cout << "groupNum" << p->groupNum << endl;	
// p->section.push_back(make_pair(0, 1));
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
			C_P->multi_check_empty_section(p->position_edges, -1, -1, 1, p->referNo2num, 0);
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
			C_P->multi_check_empty_section(p->position_edges, -1, -1, 1, p->referNo2num, 0);
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

			C_P->multi_check_empty_section(p->position_edges, -1, -1, 1, p->referNo2num, 0);
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

			C_P->multi_check_empty_section(p->position_edges, -1, -1, 1, p->referNo2num, 0);
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

			C_P->multi_check_empty_section(p->position_edges, -1, -1, 1, p->referNo2num, 0);
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
	//int tp = 0, fn = 0, error = 0;
	//ifstream infile, strfile;
	//ofstream slenfile, mtfile, ffile, errorfile, tfile;
	//clock_t match_time = 0;
	//clock_t tmp_time = 0;
	//infile.open("../exp_data/backref_exp_dataset.txt", ios::in);
	//strfile.open("../exp_data/backref_str.txt", ios::in);
	//slenfile.open("../exp_data/strlen.txt");
	//mtfile.open("../exp_data/match_time.txt");
	//ffile.open("../exp_data/false.txt");
	//errorfile.open("../exp_data/error.txt");
	//tfile.open("../exp_data/true.txt");

	//std::cout << 1 << endl;
	//string e, w;
	//int i = 0;
	//while (getline(infile, e))
	//{
	//	i++;
	//	//cout << i << endl;
	//	/*if (i > 1) {
	//		break;
	//	}*/
	//	if (i % 1000 == 0)
	//		cout << i << endl;
	//	if (infile.eof())
	//		break;
	//	if (e == "")
	//	{
	//		break;
	//	}
	//	if (e.length() == 1) {
	//		getline(strfile, w);
	//		continue;
	//	}
	//	if (e.length() == 2) {
	//		getline(strfile, w);
	//		continue;
	//	}
	//	if (e.length() == 3) {
	//		getline(strfile, w);
	//		continue;
	//	}
	//	if (e.length() >= 150) {
	//		getline(strfile, w);
	//		continue;
	//	}

	//	/*if (i==617||i==2363||i == 3026||i==3674 || i == 4278) {
	//		w = getstring(strfile);
	//		while (w != "escend")
	//		{
	//			w = getstring(strfile);
	//		}
	//		continue;
	//	}*/
	//	e = e.substr(0, e.length());
	//	getline(strfile, w);
	//	//cout << e << " " << w<<endl;
	//		clock_t begin = clock();
	//		try {
	//			//cout << e << "     " << w << endl;

	//			clock_t my_begin = clock();
	//			
	//			cout << e  << endl;
	//			cout << w << endl;
	//			bool b = match(e, w);
	//			//cout << "-----" << endl;
	//			//bool b = std::regex_match(w,regex(e));
	//			clock_t end = clock();
	//			tmp_time = end - begin;
	//			match_time += end - begin;

	//			if (b) {
	//				tp += 1;
	//				slenfile << w.length() << endl;
	//				mtfile << tmp_time << endl;
	//				tfile << e << " " << w << endl;
	//			}

	//			else
	//			{
	//				//cout << e << "   " << w << endl;
	//				ffile << e << " " << w << endl;
	//				fn += 1;
	//			}
	//		}
	//		catch (...)
	//		{
	//			//clock_t end = clock();
	//			//match_time += end - begin;
	//			cout << "int";
	//			errorfile << e << " " << w << endl;
	//			error += 1;
	//		}
	//	/*if (tp == 60000)
	//		break;*/
	//}
	//cout << "match_time:" << match_time << endl;
	//std::cout << "total_fa_time:" << total_FA_time << endl;
	//std::cout << "total_match_time:" << total_match_time << endl;
	//std::cout << "total_time;" << total_time << endl;
	//cout << "error:" << error << endl;
	//std::cout << "tp fn accu:" << tp << " " << fn << " " << double(tp) / double(tp + fn + error);

	//ffile.close();
	//errorfile.close();
	//return 0;
	
}
