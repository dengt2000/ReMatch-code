
#include"ConstructandMatch.h"
#include <string>
#include <vector>
#include<map>
#include <iostream>
#include<set>
#include<queue>
using namespace std;

void Construct_Pb::get_next(string &b, int nextval[]) {
	int i = -1;
	int j = 0;

	nextval[0] = -1;
	while (j < b.length())
	{

		if (i == -1 || b[i] == b[j])
		{
			i++;
			j++;
			if (b[i] != b[j])
			{
				nextval[j] = i;
			}
			else
			{
				nextval[j] = nextval[i];
			}
		}
		else
			i = nextval[i];
	}

}

void Construct_Pb::KMP(string &str, string &p, vector<int>& position) {
	int i = 0;
	int j = 0;
	get_next(p, nextval);

	//vector<int> result;
	int result = 0;		//һ���ж��ٸ�ƥ��

	while (i < str.length())
	{
		if (j == -1 || str[i] == p[j])
		{
			i++;
			j++;
		}
		else
		{
			j = nextval[j];
		}

		if (j == p.length())			//��ʾ���һ��ƥ��
		{
			result++;

			position.push_back(i - j);//������ƥ����������ʼ��λ��(ע�⣬�����Ǵ�0��ʼ��)


			j = nextval[j];
		}
	}


	//return result;


}
//����Pb
void Construct_Pb::iden(vector<pair<int, int>> &subwpair, string &w, vector<vector<pair<int, int>>>& Pb, unordered_map<pair<int, int>, int, Hashfunc, Equalfunc>& temp_Pb) {
	Pb.clear();
	//int subwindex = findmatch(subwpair);
	int index = 0;
	for (auto i=subwpair.begin(); i != subwpair.end(); i++) {
		string subw, subrefer;
		int len = i->second - (int)i->first + 1;
		subw.assign(w, i->first, len);
		//cout << "subw:" << subw << endl;
		//subrefer.assign(w, subreferpair[i].first, (subreferpair[i].second - subreferpair[i].first));
		vector<int> position;
		position.clear();
		KMP(w, subw, position);//index��\1�ڣ�����ƥ�䵽��λ�ã�
		//cout << "iden" << position.size() << endl;
		if (position.size() == 0) {//û�ҵ�
			cout << "û�ҵ�" << endl;
			continue;
		}
		else {
			vector<pair<int, int>> temp;
			temp.push_back(make_pair(i->first, i->second));
			
			for (int it = 0; it < position.size(); it++) {
				if (position[it] != i->first || (position[it] + len - 1) != i->second) {
					temp.push_back(make_pair(position[it], (position[it] + len - 1)));
				}
			}
			Pb.push_back(temp);

			//Pb.insert(pair<int,int>(index, index + ((subreferpair[i].second - subreferpair[i].first))));
		}
	}
	int inn = -1;
	for (auto& inner : Pb) {
		inn++;
		auto it = inner.begin();
		if (it != inner.end()) {
			for (it; it != inner.end(); ++it) {
				temp_Pb[{it->first, it->second}] = inn;
				//cout << it->first << " " << it->second << " " << inn << endl;
			}
		}
	}

	//for (auto& inner : Pb) {    // loop over the outer vector
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


}
//vector<vector<pair<int, int>>> paths;
//for (auto p : paths)
//{
//	for(i=1;i<p.size();i++)
//		if p[i] not in set(p[0])

//}
//������������ƥ��
//bool Construct_Pb checkmatch(vector<vector<pair<int, int>>> paths, vector<pair<int, int>> w1) {//·��ƥ��W1
//	//const int size = w1.size();
//	set<int> visit;
//	//int visit[];
//	for (auto p : paths) {
//		int size = int(w1.size());
//		
//		auto it = p.begin();
//		if (p.begin()->first != w1[0].first || p.begin()->second != w1[0].second) {//·����һ������Ȼ���һ��·��
//			continue;
//		}
//		visit.insert(0);
//		//
//		bool flag = false;
//		// print the rest of the pairs
//		for (++it; it != p.end(); it++) {
//			
//			for (int i = 1;i < w1.size();i++) {
//				if (it->first == w1[i].first && it->second == w1[i].second) {
//					visit.insert(i);
//					flag = true;
//					break;
//				}
//				
//			}
//			if (!flag) {
//				return false;
//			}
//		}
//
//	}
//	if (visit.size() < w1.size()) {
//		return false;
//	}
//	return true ;
//}
// ���û���
//bool checkmatch(vector<vector<pair<int, int>>> paths, vector<pair<int, int>> w1) {//w1ƥ��·��
//	int w1_size = w1.size();
//	for (auto p : paths) {
//		auto it = p.begin();
//		if (w1[0].first = it->first || w1[0].second != it->second) {//˵�����·������
//			continue;
//		}
//		
//	}
//	for (int i = 0;i < w1_size;i++) {
//		for (auto p : paths) {
//			auto it = p.begin();
//			if (w1[0].first != it->first || w1[0].second != it->second) {//˵�����·������
//				continue;
//			}
//			for()
//		}
//	}
//}
//bool Construct_Pb:: checkmatch(vector<vector<pair<int, int>>> paths, vector<pair<int, int>> w1) {//ͼdfs·��ƥ��
//
//}

/*	vector<set<pair<int, int>,pair_comp>> groups;
	//groups[0]��ʾ��()����ƥ����Ӵ����ϡ�groups[i](i>=1)��ʾ���i��\1ƥ����Ӵ�����
	unordered_map<pair<int, int>, set<pair<int, int>,pair_comp>,Hashfunc,Equalfunc> position_edges;
	//��Ϊ��㣬�ṹΪ(���i���յ�λ��j1)��
	// ֵΪ�����������пɴ�㼯�ϣ�Ԫ��Ϊ(���i+1�����λ��j2)
	//��ʾ��i����j1Ϊ�յ��λ�öԿɴﵽ��i+1����j2Ϊ����λ�ö�
*/
//bool Construct_Pb::w1_set_match(vector<pair<int, int>>w1, unordered_map<pair<int, int>, set<pair<int, int>, pair_comp>, Hashfunc, Equalfunc> position_edges, pair<int, int>& j1, pair<int, int>& j2, int i, unordered_map<pair<int, int>, bool, Hashfunc, Equalfunc> visited) {
//
//	if (position_edges.find({ j1.first,j1.second }) != position_edges.end()) {
//		auto it = position_edges[{j1.first, j1.second}].begin();
//		for (it; it != position_edges[{j1.first, j1.second}].end(); it++) {
//			//cout << "visited[{it->second, it->second + (w1[0].second - w1[0].first)}]" << it->second << " " << it->second + (w1[0].second - w1[0].first) << " " << visited[{it->second, it->second + (w1[0].second - w1[0].first)}] << endl;
//			//cout<<
//			if (visited[{it->second, it->second + (w1[0].second - w1[0].first)}]) {
//				//cout << it->second << "," << it->second + (w1[0].second - w1[0].first)<<"�ѷ��ʹ�" << endl;
//				continue;
//			}
//
//			//cout << temp_Pb[{it->second, it->second + (w1[0].second - w1[0].first)}] << " " << temp_Pb[{w1[0].first, w1[0].second}] << endl;
//			if (temp_Pb[{it->second, it->second + (w1[0].second - w1[0].first)}] == temp_Pb[{w1[0].first, w1[0].second}]) {//�ҵ�ƥ���
//				//cout << i << " " << it->second << endl;
//				j2.first = i;
//				j2.second = it->second;
//				return true;
//			}
//		}
//	}
//
//
//	return false;
//}
//void DFS(unordered_map<pair<int, int>, set<pair<int, int>, pair_comp>, Hashfunc, Equalfunc> position_edges, vector<pair<int, int>> w1,pair<int,int>j1,int w1_index) {//j
//	pair<int, int>j2;
//	if (!position_edges.find({ j1.first,j1.second })) {//���յ���
//		return;
//	}
//	if()
//	DFS()
//}
//bool Construct_Pb::checkmatch(vector<set<pair<int, int>>> groups, unordered_map<pair<int, int>, set<pair<int, int>, pair_comp>, Hashfunc, Equalfunc> position_edges, vector<pair<int, int>> w1) {
//	//������ƥ��groups[0]
//	int w1_len = w1[0].second - w1[0].first + 1;
//	auto it = groups[0].begin();
//	for (it;it != groups[0].end();it++) {
//		if (w1[0].first == it->first && w1[0].second == it->second) {
//			break;//��ʱitָ��group[0]�ж�Ӧλ��
//		}
//	}
//	if (it == groups[0].end()) {//w1��groups[0]��û�ҵ�
//		return false;
//	}
//	//w1��groups[0]���ҵ���
//	vector<vector<pair<int, int>>> path;//������ܵ�·�����
//	vector<int> find;//�Ѿ��ҹ���pair
//	queue<int> visit;//���ʵ�pair//�Ż����洢<group,w1>��֤���Ѿ����ʹ�
//	find.push_back(0);//w1[0]�ҵ����Ȱ�0push��ȥ
//	pair<int, int> j1;
//	pair<int, int> j2;
//	j2.first = 0;
//	j2.second = w1[0].second;
//	for (int i = 1;i < groups.size();) {
//		j1.first = i - 1;
//		j1.second = j2.second;
//		bool flag=w1_set_match( w1,position_edges,j1, j2, i);
//		if (flag) {
//			vector
//		}
//	}
//}


void Construct_Pb::checkmatch(vector<vector<pair<int, int>>> &groups, unordered_map<pair<int, int>, vector<pair<int, int>>, Hashfunc, Equalfunc> &position_edges, vector<pair<int, int>> &w1,int first,int second) {//first��groups��index��second�Ƕ��к���
	
	//cout << second - w1[0].second + w1[0].first << "," << second << endl;
	if (IsMatch) {//�Ѿ��ҵ�һ��·�����˳�
		//cout << "�Ѿ��ҵ�·����" << endl;
		return;
	}
	if (position_edges.find({ first,second }) == position_edges.end()) {//�ߵ�Ҷ�ӽڵ�����Ѿ����ʹ���
		//cout << "�ߵ�Ҷ�ӽڵ�����Ѿ�����" << endl;
		return;
	}
	if (visited[{first, second}]) {//�ߵ�Ҷ�ӽڵ�����Ѿ����ʹ���
		//cout << "�Ѿ�����" << endl;
		return;
	}
	if (temp_Pb[{second - w1[0].second + w1[0].first, second}] != temp_Pb[{w1[0].first, w1[0].second}]) {//����ڵ㲻ƥ��
		//cout << "����ڵ㲻ƥ��" << endl;
		return;
	}
	//����ڵ�ƥ��
	visited[{first, second}] = true;
	//cout << second - w1[0].second + w1[0].first << "," << second << endl;
	auto it = position_edges[{first, second}].begin();
	for (it; it != position_edges[{first, second}].end(); it++) {//��������������յ�
		if (it->first == -1) {
			IsMatch = true;
			//cout << "ƥ��" << endl;
			return;
		}
		
	}
	it = position_edges[{first, second}].begin();
	for (it; it != position_edges[{first, second}].end(); it++) {
		//cout << it->first << "   " << it->second + w1[0].second - w1[0].first << endl;
		checkmatch(groups, position_edges, w1, it->first,it->second+w1[0].second-w1[0].first);
		if (IsMatch)
			return;
	}
	


}

//bool Construct_Pb::checkmatch(vector<set<pair<int, int>, pair_comp>> groups, unordered_map<pair<int, int>, set<pair<int, int>, pair_comp>, Hashfunc, Equalfunc> position_edges, vector<pair<int, int>> w1) {
//	//������ƥ��groups[0]
//	int w1_len = w1[0].second - w1[0].first + 1;
//	auto it = groups[0].begin();
//	for (it; it != groups[0].end(); it++) {
//
//		if (w1[0].first == it->first && w1[0].second == it->second) {
//			//cout << "w1[0].first " << w1[0].first << " w1[0].second " << w1[0].second << endl;
//			break;//��ʱitָ��group[0]�ж�Ӧλ��
//		}
//	}
//	if (it == groups[0].end()) {//w1��groups[0]��û�ҵ�
//		return false;
//	}
//	pair<int, int> past_j1;
//	pair<int, int> j1;//��ɴ��Թ�ϵ��ǰһ��
//	pair<int, int> j2;//��ɴ��Թ�ϵ�ĺ�һ��
//	unordered_map<pair<int, int>, bool, Hashfunc, Equalfunc> visited;//�Ƿ��ʹ�
//	visited.clear();
//	j1.first = 0;
//	j1.second = w1[0].second;
//	past_j1.first = j1.first;
//	past_j1.second = j1.second;
//	for (int i = 1; i < groups.size();i++) {
//		
//		cout << "j1" << j1.first << "," << j1.second << endl;
//		bool flag = w1_set_match(w1, position_edges, j1, j2, i, visited);
//		if (flag) {
//			i++;
//			//cout <<"j1" << j1.first << " " << j1.second << endl;
//			past_j1.first = j1.first;
//			past_j1.second = j1.second;
//			j1.first = i - 1;
//			j1.second = j2.second + w1_len - 1;
//			//cout << j1.first << ", " << j1.second << endl;
//			/*if ((j1.first + 1) == groups.size()) {
//				return true;
//			}*/
//			
//			if (position_edges.find({ j1.first,j1.second }) != position_edges.end()) {
//				auto end = position_edges[{j1.first, j1.second}].begin();
//				for (end; end != position_edges[{j1.first, j1.second}].end(); end++) {
//					if (end->first == -1) {
//						return true;
//					}
//				}
//				
//			}
//			else {
//				visited[{j1.first, j1.second}] = true;
//				//cout << j1.first << " " << j1.second << "true" << endl;
//				i--;
//				j1.first = past_j1.first;
//				j1.second = past_j1.second;
//			}
//		}
//		else {
//			visited[{j1.first, j1.second}] = true;
//			cout << j1.first << " " << j1.second<<"true" << endl;
//			i--;
//			j1.first = past_j1.first;
//			j1.second = past_j1.second;
//		}
//	}
//	return false;
//
//}




