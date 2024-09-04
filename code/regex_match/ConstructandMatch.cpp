
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
/// Construct Pb
/// </summary>
/// <param name="subwpair"></param>
/// <param name="w"></param>
/// <param name="Pb"></param>
///int CGs = 0;
 //表示有几个capturegroup，0表示没有backrefer，1就是1个==我们之前做的那个，2表示有两个capture group；
//vector<int> groupNo;
//groupNo[i]表示第i个group是属于\1还是\2.例如对于(a*)(a*).\1\2\2\1,有6个group，那么groupNo[0]=0,groupNo[1]=1,group[2]=0,groupNo[3]=1,groupNo[4]=1,groupNo[5]=0
void Construct_Pb::getBRNo(int CGs, vector<int>groupNo) {
	int num = 0;
	int it = 0;
	while (num != CGs&&it<groupNo.size()) {
		if (num == groupNo[it]) {
			BRNo.emplace_back(it);
			num++;
			it++;
		}
		else {
			it++;
		}
	}
}
void Construct_Pb::iden( string& w, vector<pair<int, int>>& section, vector<vector<vector<pair<int, int>>>>& position_edges, vector<vector<vector<bool>>>& groups) {
	int sum = 0, n;
	unsigned char* Text;
	vector<pair<int, int>> temp_s;
	vector<pair<int, int>> Text_pair;//ÿһ�Ե�λ��

	for (int i = 0; i < section.size(); i++) {//������к�׺
		for (int j = section[i].first; j <= section[i].second; j++) {
			temp_s.emplace_back(make_pair(j, section[i].second));
		}
	}
	string* all_suffix = new string[temp_s.size() + 1];
	const int temp_s_size = temp_s.size();
	for (int i = 0; i < temp_s_size; i++) {//���к�׺�ַ���
		string a;
		a.assign(w, temp_s[i].first, temp_s[i].second - temp_s[i].first + 1);
		string b(a.rbegin(), a.rend());
		all_suffix[i].assign(b);
	}



	for (int i = 0; i < temp_s_size; i++) {
		sum += all_suffix[i].length() + 1;
	}
	n = sum + 1;
	Text = (unsigned char*)malloc(n * sizeof(unsigned char));
	sum = 0;
	for (int i = 0; i < temp_s.size(); i++) {
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
	////////////////////////////////////////////////
	/*int i, j;*/
	/*printf("T^{cat} = ");
	for (i = 0; i < n - 1; i++) {
		if (Text[i] == 1) printf("$");
		else printf("%c", Text[i]);
	}
	printf("#\n");*/
	///////////////////////////////////////////////////

	uint_t* SA = (uint_t*)malloc(n * sizeof(uint_t));
	int_t* LCP = (int_t*)malloc(n * sizeof(int_t));
	int_t* DA = (int_t*)malloc(n * sizeof(int_t));

	// sort
	gsacak((unsigned char*)Text, (uint_t*)SA, LCP, (int_da*)DA, n);
	////////////////////////////////////////////////////
	/*printf("i\tSA\tDA\tLCP\tBWT\tsuffixes\n");
	for (int i = 0; i < n; ++i) {
		char j = (SA[i]) ? Text[SA[i] - 1] : '#';
		if (j == 1) j = '$';
		printf("%d\t%d\t%d\t%d\t%c\t", i, SA[i], DA[i], LCP[i], j);
		for (j = SA[i]; j < n; ++j) {
			if (Text[j] == 1) printf("$");
			else printf("%c", Text[j]);
		}
		printf("#\n");
	}*/
	//////////////////////////////////////////////////////
	//checkmatch_time = 0;
	vector<pair<int, int>> t_Pb;
	length = w.length();
	Pb_mark.resize(length * length);
	visited.resize(referCount+2);
	for (int i = 0; i < referCount+2; i++) {
		visited[i].resize(w.length()+2);
		for (int j = 0; j < w.length()+2; j++) {
			visited[i][j].resize(w.length()+2);
		}
	}
	w1_len = -1;
	checkEmptyGroup(position_edges, 0, -1);

	for (int i = temp_s.size() + 1; i < n; ++i) {

		int a, b;

		//�ҵ������ı��λ��
		if (LCP[i] != Text_pair[DA[i]].second - SA[i] + 1) {//ǰ��ͺ�����ȵ���LCP[i]���������ַ������Ȳ���

			if (t_Pb.size()) {
				/*for (int it = 0; it < t_Pb.size(); it++) {
					cout << "<" << t_Pb[it].first << "," << t_Pb[it].second << ">";
				}*/
				/*cout << endl;*/

				int size = t_Pb.size();
				for (int j = 0; j < size; j++) {

					
					if (groups[0][t_Pb[0].first][t_Pb[0].second+1]) {//在groups0中
						//cout << 0 << " " << t_Pb[0].second << endl;
						checkmatch(position_edges, 0, t_Pb[0].second);
					}
					Pb_mark[t_Pb[0].first * length + t_Pb[0].second] = false;
					t_Pb.assign(t_Pb.begin() + 1, t_Pb.end());
					if (IsMatch) {
						return;
					}
				}

				t_Pb.clear();


			}

		}
		a = temp_s[DA[i]].first;
		b = temp_s[DA[i]].first + Text_pair[DA[i]].second - SA[i];
		t_Pb.emplace_back(make_pair(a, b));
		if (t_Pb.size() == 1) {
			w1_len = b - a;
		}
		Pb_mark[a * length + b] = true;

	}

	if (t_Pb.size()) {
		int size = t_Pb.size();
		//for (int it = 0; it < t_Pb.size(); it++) {
		//	cout << "<" << t_Pb[it].first << "," << t_Pb[it].second << ">";
		//}
		//cout << endl;
		for (int j = 0; j <size; j++) {
			if (groups[0][t_Pb[0].first][t_Pb[0].second+1]) {//在groups0中
				
				//cout << 0 << " " << t_Pb[0].second << endl;
				checkmatch(position_edges, 0, t_Pb[0].second);
			}			
			Pb_mark[t_Pb[0].first * length + t_Pb[0].second] = false;
			t_Pb.assign(t_Pb.begin() + 1, t_Pb.end());

			if (IsMatch) {
				return;
			}
		}

		t_Pb.clear();
	}



}


void Construct_Pb::checkEmptyGroup(vector<vector<vector<pair<int, int>>>>& position_edges, int first, int second) {
	// cout<<"checkEmptyGroup"<<first<<" "<<second<<endl;
	int len_lim = position_edges[0].size();
	if (IsMatch) {
		return;
	}
	if (second + 1 >= len_lim) {
		return;
	}
	if (visited[first+1][second - w1_len+1][second+1]) {
		return;
	}
	if (position_edges[first + 1][second + 1].empty()) {
		return;
	}
	// cout<<"visited"<<first+1<<" "<<second - w1_len+1<<" "<<second+1<<endl;
	visited[first+1][second - w1_len+1][second+1] = true;
	auto& edges = position_edges[first + 1][second + 1];
	for (auto it = edges.begin(); it != edges.end(); ++it) {
		int ffirst = it->first;
		int ssecond = it->second;
		// cout<<"nextcheck"<<ffirst<<" "<<ssecond<<endl;
		if (ssecond + w1_len >= length) {
			continue;
		}
		if (ffirst == -1 && ssecond == -1) {
			IsMatch = true;
			return;
		}
		// cout<<"visited[ffirst+1][ssecond - w1_len+1][ssecond+1]"<<endl;
		// cout<<"visited"<<ffirst+1<<" "<<ssecond +1<<" "<<ssecond+w1_len+1<<endl;
		if (visited[ffirst+1][ssecond +1][ssecond+w1_len+1]) {
			return;
		}
		if (IsMatch) {
			return;
		}
		// cout<<"checkEmptyGroup"<<ffirst<<" "<<ssecond + w1_len<<endl;
		checkEmptyGroup(position_edges, ffirst, ssecond + w1_len);
	}
}
void Construct_Pb::checkmatch(vector<vector<vector<pair<int, int>>>>& position_edges, int first, int second) {
	int len_lim = position_edges[0].size();
	
	if (IsMatch) {
		return;
	}
	
	if (second + 1 >= len_lim ) {
		
		return;
	}
	
	if (visited[first+1][second - w1_len+1][second+1]) {
		return;
	}

	if (position_edges[first + 1][second + 1].empty()) {
		return;
	}
	

	visited[first+1][second - w1_len+1][second+1] = true;

	//int s_w1_diff = w1[0].second - w1[0].first;
	//cout << first << " " << second << " " << IsMatch << endl;



	auto& edges = position_edges[first + 1][second + 1];
	for (auto it = edges.begin(); it != edges.end(); ++it) {

		int ffirst = it->first;
		int ssecond = it->second;
		//cout << ffirst << " " << ssecond << endl;

		if (ssecond + w1_len >= length) {
			//cout << 111 << endl;
			continue;
		}
		//cout << ffirst << " " << ssecond   << endl;
		if (ffirst == -1 && ssecond == -1) {
			//cout << 222 << endl;
			IsMatch = true;
			return;
		}
		if (!Pb_mark[ssecond * length + ssecond + w1_len]) {
			//cout << "333" <<endl;
			continue;
		}
		//cout << "111" << endl;
		if (visited[ffirst+1][ssecond+1][ssecond + w1_len+1]) {
		return;
	}

		if (IsMatch) {
			return;
		}
		//cout << w1_len << endl;
		//cout << ffirst <<" " << ssecond + w1_len << endl;

		checkmatch(position_edges, ffirst, ssecond + w1_len);
	}
}
void Construct_Pb::muti_iden(string& w, vector<pair<int, int>>& section, vector<vector<vector<pair<int, int>>>>& position_edges, vector<vector<vector<bool>>>& groups, int CGs) {
	int sum = 0, n;
	unsigned char* Text;
	vector<pair<int, int>> temp_s;
	vector<pair<int, int>> Text_pair;//ÿһ�Ե�λ��

	//unordered_map<int, pair<int,int>> subwposition;//Textλ����w��λ�ö�Ӧ
		/*if (!section.size()) {
			IsMatch = true;
			return;
	}*/
	for (int i = 0; i < section.size(); i++) {//������к�׺
		//cout << section[i].first << " " << section[i].second << endl;
		for (int j = section[i].first; j <= section[i].second; j++) {
			temp_s.emplace_back(make_pair(j, section[i].second));
			//cout <<"section " << j << " " << section[i].second << endl;
		}
	}
	string* all_suffix = new string[temp_s.size() + 1];
	const int temp_s_size = temp_s.size();
	for (int i = 0; i < temp_s_size; i++) {//���к�׺�ַ���
		string a;
		a.assign(w, temp_s[i].first, temp_s[i].second - temp_s[i].first + 1);
		string b(a.rbegin(), a.rend());
		all_suffix[i].assign(b);
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
	////////////////////////////////////////////////////////
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
	//boost::dynamic_bitset<> visited(groups.size()*length * length);
	//visited.resize(groups.size()*length * length);
	//visited.assign(length, vector<vector<bool>>(length, vector<bool>(length, false)));
	visited.resize(referCount+1);
	for (int i = 0; i < referCount+1; i++) {
		visited[i].resize(w.length());
		for (int j = 0; j < w.length(); j++) {
			visited[i][j].resize(w.length());
		}
	}
	for (int i = temp_s.size() + 1; i < n; ++i) {

		//cout << temp_s.size() + 1;
		int a, b;

		//�ҵ������ı��λ��
		if (LCP[i] != Text_pair[DA[i]].second - SA[i] + 1) {//ǰ��ͺ�����ȵ���LCP[i]���������ַ������Ȳ���

			if (t_Pb.size()) {
				//for (int i = 0; i < t_Pb.size(); i++) {
				//	cout <<"<" << t_Pb[i].first << " " << t_Pb[i].second << ">";
				//}
				//cout << endl;
				//Pb.emplace_back(t_Pb);
				//visited.clear();
				for (int j = 0; j < t_Pb.size(); j++) {


					/*for (int it = 0; it < t_Pb.size(); it++) {
						cout <<"<" << t_Pb[it].first << "," << t_Pb[it].second << ">";
					}*/
					//cout << endl;
					/*cout << "size" << t_Pb.size() << endl;

					cout << "j"<<j << endl;*/
					//visited.clear();
					//clock_t begin = clock();
					if (groups[0][t_Pb[0].first][t_Pb[0].second + 1]) {//在groups0中
						checkmatch(position_edges, 0, t_Pb[0].second);
					}
					//


					//clock_t end = clock();
					//checkmatch_time += end - begin;
					Pb_mark[t_Pb[0].first * length + t_Pb[0].second] = false;
					t_Pb.assign(t_Pb.begin() + 1, t_Pb.end());

					if (IsMatch) {
						//cout << 111 << endl;
						return;
					}
				}
				//clock_t begin = clock();

				t_Pb.clear();


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
			/*for (int i = 0; i < t_Pb.size(); i++) {
				cout << t_Pb[i].first << " " << t_Pb[i].second << endl;
			}*/
			//visited.clear();


			//cout << "new" << endl;

			/*clock_t begin = clock();*/
			if (groups[0][t_Pb[0].first][t_Pb[0].second + 1]) {//在groups0中
				checkmatch(position_edges, 0, t_Pb[0].second);
			}			//clock_t end = clock();
			//checkmatch_time += end - begin;
			Pb_mark[t_Pb[0].first * length + t_Pb[0].second] = false;
			t_Pb.assign(t_Pb.begin() + 1, t_Pb.end());

			if (IsMatch) {
				return;
			}
		}
		//clock_t begin = clock();

		t_Pb.clear();
		//for (auto& subVector :Pb_mark) {
		//	subVector.erase(subVector.begin(), subVector.end());
		//}

		//Pb_mark.erase(Pb_mark.begin(), Pb_mark.end());


	}

	//visited.clear();
}




// void Construct_Pb::iden(vector<pair<int, int>>& subwpair, string& w, vector<pair<int, int>> &section, vector<vector<vector<pair<int, int>>>>& position_edges, vector<vector<pair<int, int>>> &groups) {
//     int n = w.length();
//     vector<string> all_suffix(section.size());
//     for (int i = 0; i < section.size(); i++) {
//         string a = w.substr(section[i].first, section[i].second - section[i].first + 1);
//         all_suffix[i] = string(a.rbegin(), a.rend());
//     }
//     string Text;
//     Text.reserve(n + section.size() + 1);
//     vector<pair<int, int>> Text_pair;
//     Text_pair.reserve(section.size() + 1);
//     for (int i = 0; i < section.size(); i++) {
//         Text += all_suffix[i];
//         Text += 1;
//         Text_pair.emplace_back(Text.size() - all_suffix[i].size() - 1, Text.size() - 2);
//     }
//     Text += w + 2;
//     Text_pair.emplace_back(n, n);
// 	printf("N = %d\n", n);
// 	//////////////////////////////////////////////////
// 	int i, j;
// 	printf("T^{cat} = ");
// 	for (i = 0; i < n - 1; i++) {
// 		if (Text[i] == 1) printf("$");
// 		else printf("%c", Text[i]);
// 	}
// 	printf("#\n");
// 	/////////////////////////////////////////////////////

//     uint_t* SA = (uint_t*)malloc((n + section.size() + 2) * sizeof(uint_t));
//     int_t* LCP = (int_t*)malloc((n + section.size() + 2) * sizeof(int_t));
//     int_t* DA = (int_t*)malloc((n + section.size() + 2) * sizeof(int_t));
//     gsacak((unsigned char*)Text.c_str(), SA, LCP, (int_da*)DA, n + section.size() + 2);

//     vector<vector<bool>> Pb_mark(n, vector<bool>(n));
//     visited.assign(groups.size(), vector<vector<bool>>(n, vector<bool>(n)));
//     for (int i = section.size() + 1; i <= n + section.size() + 1; ++i) {
//         if (LCP[i] != Text_pair[DA[i]].second - SA[i] + 1) {
//             if (Pb_mark[Text_pair[DA[i]].first][Text_pair[DA[i]].second]) {
//                 continue;
//             }
//             vector<pair<int, int>> t_Pb;
//             t_Pb.emplace_back(Text_pair[DA[i]].first, Text_pair[DA[i]].second);
//             int curDA = DA[i];
//             while (LCP[i] <= LCP[SA[curDA]]) {
//                 if (curDA == 0) {
//                     break;
//                 }
//                 if (LCP[SA[curDA - 1]] < LCP[SA[curDA]]) {
//                     t_Pb.emplace_back(Text_pair[curDA - 1].first, Text_pair[DA[i]].second);
//                 }
//                 curDA--;
//             }
//             reverse(t_Pb.begin(), t_Pb.end());
//             position_edges.emplace_back(extend_position_edges(subwpair, t_Pb));
//             construct_groups(subwpair, t_Pb, groups, visited);
//             Pb_mark[t_Pb[0].first][t_Pb.back().second] = true;
//         }
//     }
//     free(SA);
//     free(LCP);
//     free(DA);
// }


// void Construct_Pb::checkmatch(vector<vector<vector<pair<int, int>>>>& position_edges, vector<pair<int, int>>& w1, int first, int second, vector<vector<bool>>& Pb_mark, int length) {//first��groups��index��second�Ƕ��к���
	
// 	//cout << second - w1[0].second + w1[0].first << "," << second << endl;
// 	//cout << first <<" " << second << endl;
// 	int w1_len=w1[0].second - w1[0].first;

// 	int len_lim = position_edges[0].size();

// 	if (IsMatch) {//�Ѿ��ҵ�һ��·�����˳�
// 		//cout << "�Ѿ��ҵ�·����" << endl;
// 		return;
// 	}
// 	if (second + 1 >= len_lim || position_edges[first+1][second+1].size() == 0) {//�ߵ�Ҷ�ӽڵ�
// 		//cout << "�ߵ�Ҷ�ӽڵ�" << endl;
// 		return;
// 	}
// 	//if (visited.find({ first, { second - (w1[0].second - w1[0].first),second } })!=visited.end()) {//�ߵ�Ҷ�ӽڵ�����Ѿ����ʹ���
// 	//	//cout << "�Ѿ�����" << endl;
// 	//	return;
// 	//}

// 	if (visited[first][second -w1_len][second]) {//�ߵ�Ҷ�ӽڵ�����Ѿ����ʹ���
// 		//cout << "�Ѿ�����" << endl;
		
// 		return;
// 	}
// 	//number++;
// 	/*number++;
// 	cout << "number:" << number << endl;*/
// 	//if (temp_Pb[{second - w1[0].second + w1[0].first, second}] != temp_Pb[{w1[0].first, w1[0].second}]) {//����ڵ㲻ƥ��
// 	//	//cout << "����ڵ㲻ƥ��" << endl;
// 	//	return;
// 	//}
// 	//����ڵ㲻ƥ�䣨��λ��Pbͬһ�飩
	
// 	//����ڵ�ƥ��、
// 	// clock_t kaixiaob=clock();
// 	// clock_t kaixiaoe=clock();

// 	visited[first][second - w1_len][second] = true;
	


// 	//cout << second - w1[0].second + w1[0].first << "," << second << endl;
// 	//auto it = position_edges[first+1][second+1].begin();
// 	//for (it; it != position_edges[first+1][second+1].end(); it++) {//��������������յ�
// 	//	if (it->first == -1 && it->second == -1) {
// 	//		IsMatch = true;
// 	//		//cout << "ƥ��" << endl;
// 	//		return;
// 	//	}
// 	//}

// 	auto it = position_edges[first+1][second+1].begin();

// 	for (it; it != position_edges[first+1][second+1].end(); it++) {

// 		int ffirst = it->first;
// 		int ssecond = it->second;
// 		if (ffirst == -1 && ssecond == -1) {
// 			IsMatch = true;
// 			return;
// 		}

// 		//cout << it->first << "   " << it->second + w1[0].second - w1[0].first << endl;
// 		if (ssecond + w1_len >= length) {
// 			continue;
// 		}
// 		// clock_t begin=clock();
// 		// clock_t end =clock();
// 		// temp_time+=end-begin;
// 		// clock_t chbegin = clock();

// 		if (Pb_mark[ssecond][ssecond + w1_len]) {
// 			// clock_t chend = clock();
// 			// checkmatch_time += chend - chbegin;	
// 			checkmatch(position_edges, w1,ffirst, ssecond + w1_len, Pb_mark,length);

// 		}
// 		if (IsMatch)
// 			return;
// 	}
					

// }
bool Construct_Pb::checkahead(vector<vector<vector<pair<int, int>>>>& position_edges,int first, int second) {
	int len_lim = position_edges[0].size();
	if (second + 1-w1_len <0 || position_edges[first + 1][second + 1-w1_len].empty()) {
		return false;
	}
	if (first > -1)
	{
		if (visited[first][second - w1_len][second]) {
			return res[first][second - w1_len][second];
		}
		visited[first][second - w1_len][second] = true;
	}
	auto& edges = position_edges[first + 1][second + 1-w1_len];
	for (auto it = edges.begin(); it != edges.end(); ++it) {

		int ffirst = it->first;
		int ssecond = it->second;
		if (ffirst == -1 && ssecond == -1) {
			res[first][second - w1_len][second] = true;
			return true;
		}
		if (ssecond - w1_len < 0) {
			continue;
		}
		//cout << ssecond << " " << ssecond + w1_len << endl;
		if (!Pb_mark[(ssecond-w1_len) * length + ssecond]) {
			//cout << "111" <<endl;
			continue;
		}
		if (checkahead(position_edges, ffirst, ssecond))
		{
			if (first>-1)
			res[first][second - w1_len][second] = true;
			return true;
		}
	}
	return false;
}
bool Construct_Pb::checkbehind(vector<vector<vector<pair<int, int>>>>& position_edges, int first, int second) {
	int len_lim = position_edges[0].size();
	if (second + 1 >= len_lim || position_edges[first + 1][second + 1].empty()) {
		return false;
	}
	if (first > -1)
	{
		if (visited[first][second - w1_len][second]) {
			return res[first][second - w1_len][second];
		}
		visited[first][second - w1_len][second] = true;
	}
	auto& edges = position_edges[first + 1][second + 1];
	for (auto it = edges.begin(); it != edges.end(); ++it) {

		int ffirst = it->first;
		int ssecond = it->second;
		if (ssecond + w1_len >= length) {
			continue;
		}
		if (ffirst == -1 && ssecond == -1) {
			if(first>-1)
			res[first][second - w1_len][second] = true;
			return true;
		}
		//cout << ssecond << " " << ssecond + w1_len << endl;
		if (!Pb_mark[ssecond * length + ssecond + w1_len]) {
			//cout << "111" <<endl;
			continue;
		}
		if( checkbehind(position_edges,  ffirst, ssecond + w1_len))
		{
			if(first>-1)
				res[first][second - w1_len][second] = true;
			return true;
		}
	}
	return false;
}






