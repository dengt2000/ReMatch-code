
#include"ConstructandMatch.h"
#include"suffix_array.h"
#include <string>
#include <vector>
#include<map>
#include <iostream>
#include<stack>
#include<tuple>
using namespace std;





/// <summary>
/// 
/// </summary>
/// <param name="w"></param>
/// <param name="section"></param>
/// <param name="position_edges"></param>
/// <param name="groups"></param>
/// <param name="groupNum"></param>
template <typename... Args>void Construct_Pb<Args...>::multi_iden(string& w, vector<pair<int, int>>& section,vector<vector<vector<vector<tuple<int, int, int>>>>> &position_edges, vector<vector<vector<bool>>>& groups,int groupNum, unordered_map<int, int> &referNo2num) {
	int PbNum = 1;
	int sum = 0, n;
	unsigned char* Text;
	vector<pair<int, int>> temp_s;
	vector<pair<int, int>> Text_pair;//ÿһ Ե λ  
	
	//unordered_map<int, pair<int,int>> subwposition;//Textλ    w  λ ö Ӧ
		/*if (!section.size()) {
			IsMatch = true;
			return;
	}*/
	for (int i = 0; i < section.size(); i++) {//      к ׺
		for (int j = section[i].first; j <= section[i].second; j++) {
			temp_s.emplace_back(make_pair(j, section[i].second));
			//cout <<"section " << j << " " << section[i].second << endl;
		}
	}
	string* all_suffix = new string[temp_s.size() + 1];
	const int temp_s_size = temp_s.size();
	for (int i = 0; i < temp_s_size; i++) {//   к ׺ ַ   
		string a;
		if ((temp_s[i].second - temp_s[i].first) >= 0) {
			a.assign(w, temp_s[i].first, temp_s[i].second - temp_s[i].first + 1);
			//cout << a << endl;
			string b(a.rbegin(), a.rend());
			all_suffix[i].assign(b);
		}
	}

	for (int i = 0; i < temp_s_size; i++) {
		sum += all_suffix[i].length() + 1;
	}
	n = sum + 1;
	Text = (unsigned char*)malloc(n * sizeof(unsigned char));
	sum = 0;
	

	for (int i = 0; i < temp_s.size(); i++) {
		/*for (int j = sum; j < sum + all_suffix[i].length(); j++) {
			subwposition[j].first = i;
			subwposition[j].second = (temp_s[i].second - temp_s[i].first) - (sum - j);
		}*/
		std::copy(all_suffix[i].begin(), all_suffix[i].end(), Text + sum);
		int a = sum;
		sum += all_suffix[i].length() + 1;
		Text[sum - 1] = 1;//separator
		//cout<<"jocfejf" << endl;
		int b = sum - 2;
		Text_pair.emplace_back(make_pair(a, b));
	}
	Text[n - 1] = 0;
	//printf("N = %d\n", n);
	//////////////////////////////////////////////////
	//int i, j;
	//printf("T^{cat} = ");
	//for (i = 0; i < n - 1; i++) {
	//	if (Text[i] == 1) printf("$");
	//	else printf("%c", Text[i]);
	//}
	//printf("#\n");
	/////////////////////////////////////////////////////

	uint_t* SA = (uint_t*)malloc(n * sizeof(uint_t));
	int_t* LCP = (int_t*)malloc(n * sizeof(int_t));
	int_t* DA = (int_t*)malloc(n * sizeof(int_t));

	// sort
	gsacak((unsigned char*)Text, (uint_t*)SA, LCP, (int_da*)DA, n);
	////////////////////////////////////////////////////////
	//printf("i\tSA\tDA\tLCP\tBWT\tsuffixes\n");
	//for (int i = 0; i < n; ++i) {
	//	char j = (SA[i]) ? Text[SA[i] - 1] : '#';
	//	if (j == 1) j = '$';
	//	printf("%d\t%d\t%d\t%d\t%c\t", i, SA[i], DA[i], LCP[i], j);
	//	for (j = SA[i]; j < n; ++j) {
	//		if (Text[j] == 1) printf("$");
	//		else printf("%c", Text[j]);
	//	}
	//	printf("#\n");
	//}
		//checkmatch_time = 0;
	vector<pair<int, int>> t_Pb;
	length = w.length();
	//vector<vector<bool>> Pb_mark;
	//Pb_mark.resize(length);
	//for (int i = 0; i < length; i++) {
	//	Pb_mark[i].resize(length);
	//}
	//unordered_map<pair<int, int>, bool, Hashfunc, Equalfunc> Pb_mark;
	//visited.clear();
	//vector<vector<bool>> Pb_mark(length, vector<bool>(length, false));
	//vector<bool> Pb_mark(length*length,false);
	Pb_mark.resize(length * length);
	Pb.resize(length+1);
	for (int i = 0; i < length+1; i++) {
		Pb[i].resize(length+1);
	}
	//boost::dynamic_bitset<> visited(groups.size()*length * length);
	//visited.resize(groups.size()*length * length);
	//visited.assign(length, vector<vector<bool>>(length, vector<bool>(length, false)));
	

	for (int i = temp_s.size() + 1; i < n; ++i) {

		//cout << temp_s.size() + 1;
		int a, b;

		// ҵ      ı  λ  
		if (LCP[i] != Text_pair[DA[i]].second - SA[i] + 1) {//ǰ  ͺ     ȵ   LCP[i]         ַ      Ȳ   

			if (t_Pb.size()) {
				
				for (int j = 0; j < t_Pb.size(); j++) {


					
					
					Pb[t_Pb[j].first][t_Pb[j].second] = PbNum;
					
				}
				//clock_t begin = clock();

				t_Pb.clear();
				PbNum++;

				/*clock_t end = clock();
				checkmatch_time += end - begin;*/
			}

		}
		a = temp_s[DA[i]].first;
		b = temp_s[DA[i]].first + Text_pair[DA[i]].second - SA[i];
		t_Pb.emplace_back(make_pair(a, b));
		if (t_Pb.size() == 1) {
			w1_len = b - a;
		}
		Pb_mark[a * length + b] = true;
		//cout << a << " " << b << endl;

	}

	if (t_Pb.size()) {
		//for (int i = 0; i < t_Pb.size(); i++) {
		//	cout << "<" << t_Pb[i].first << " " << t_Pb[i].second << ">";
		//}
		//cout << endl;
		//Pb.emplace_back(t_Pb);
		//visited.clear();
		for (int j = 0; j < t_Pb.size(); j++) {
			//cout << t_Pb[j].first << " " << t_Pb[j].second <<" " <<PbNum<< endl;
			Pb[t_Pb[j].first][t_Pb[j].second] = PbNum;

			
		}
		//clock_t begin = clock();

		t_Pb.clear();
		PbNum++;
		//for (auto& subVector :Pb_mark) {
		//	subVector.erase(subVector.begin(), subVector.end());
		//}

		//Pb_mark.erase(Pb_mark.begin(), Pb_mark.end());
	}

	leftPosition.resize(referCount);
	current_PbNum.resize(referCount);
	IsMatch = false;
	left.resize(referCount);
	catchGroupPair.resize(referCount);
	catchGroup.resize(referCount);
	catchLength.resize(referCount);
	for (int i = 0; i < referCount; i++) {
		current_PbNum[i] = 0;
		catchGroupPair[i].first = -1;
		catchGroupPair[i].second = -1;
		catchGroup[i] = -1;
	}
	
	multi_checkmatch2(position_edges, -1, -1, 1, referNo2num, 0);


	

}









template <typename... TArgs>void Construct_Pb<TArgs...>::multi_checkmatch2(vector<vector<vector<vector<tuple<int, int, int>>>>>& position_edges, int first, int second, int third, unordered_map<int, int>& referNo2num, int pathNum) {
	
	int referNo = referNo2num[first];
	//cout <<first<<" " << referNo << endl;
	if (IsMatch) {
		return;
	}
	int len_lim = position_edges[0].size();

	if (second + 1 >= len_lim) {
		return;
	}

	
	if ((first != -1 && second != -1) && third == 0) {
		leftPosition[first] = second;

		if (catchGroup[referNo] != first && catchGroup[referNo] != -1 ) {
			
			
			if (second + catchLength[referNo] >= length) {
				return;
			}
			using KeyType = std::tuple<TArgs...>;
			KeyType key;
			std::vector<int> indices;
			for (int i = 0; i < groupNum; i++) {
				if (i != referNo) {
					if (i < referNo) {
						indices.emplace_back(current_PbNum[i]);
					}
					else {
						indices.emplace_back(-1);

					}
				}
			}
			if (indices.size()) {
				assign_tuple_elements<KeyType, 0>(key, indices);

			}
			std::get<(sizeof...(TArgs)) - 1>(key) = first;
			std::get<(sizeof...(TArgs)) - 2>(key) = second;
			std::get<(sizeof...(TArgs)) - 3>(key) = second + catchLength[referNo];

			//if (pathNum > 1) {
			if (visited[key]) {
				return;
			}
			else {
				visited[key] = true;
			}
			////}
			//else {
			//	if (visited[key]) {
			//		return;
			//	}
			//}
			if (catchLength[referNo] == -1) {
				if (current_PbNum[referNo] != -1) {
					return;
				}
			}
			else {
				if (current_PbNum[referNo] != Pb[second][second + catchLength[referNo]]) {
					return;
				}
			}
			

		
		}
	}
	else if ((first != -1 || second != -1) && third == 1)
	{
	
		int lposition = leftPosition[first];
		if (second < lposition) {
			if (first == catchGroup[referNo] || catchGroup[referNo] == -1) {

				current_PbNum[referNo2num[first]] =-1;
				catchGroupPair[referNo2num[first]].first = lposition;
				catchGroupPair[referNo2num[first]].second = second;
				catchGroup[referNo2num[first]] = first;
				catchLength[referNo2num[first]] = -1;
			}
			else {
				if (current_PbNum[referNo] != -1)
					return;
			}
		}
		else {
			if (first == catchGroup[referNo] || catchGroup[referNo] == -1) {

				current_PbNum[referNo2num[first]] = Pb[lposition][second];
				catchGroupPair[referNo2num[first]].first = lposition;
				catchGroupPair[referNo2num[first]].second = second;
				catchGroup[referNo2num[first]] = first;
				catchLength[referNo2num[first]] = second - lposition;
			}
			else {
				if (current_PbNum[referNo] != Pb[lposition][second])
					return;
			}
		}
		
		
		
	}
	
	if (catchGroup[referNo2num[first]] != first && first != -1 && catchGroup[referNo] != -1) {//
		
		for (auto temp : position_edges[first + 1][second + catchLength[referNo2num[first]] + 1][1]) {
			pathNum = position_edges[first + 1][second + catchLength[referNo2num[first]] + 1][1].size();
			int ffirst = get<0>(temp);
			int ssecond = get<1>(temp);
			int tthird = get<2>(temp);

			if (ffirst == -1 && ssecond == -1) {
				IsMatch = true;
				
				return;
			}
			if (IsMatch)
				return;
			multi_checkmatch2(position_edges, ffirst, ssecond, tthird, referNo2num, pathNum);
		}
		
		
	}
	else {
		for (auto temp : position_edges[first + 1][second + 1][third]) {
			pathNum = position_edges[first + 1][second + 1][third].size();
			int ffirst = get<0>(temp);
			int ssecond = get<1>(temp);
			int tthird = get<2>(temp);
			//cout << ffirst << " " << ssecond << " " << tthird << endl;
			
			if (ffirst == -1 && ssecond == -1) {
				IsMatch = true;
				//cout << 22222222 << endl;
				return;
			}
			if (IsMatch)
				return;
			multi_checkmatch2(position_edges, ffirst, ssecond, tthird, referNo2num, pathNum);
		}
	}
}






