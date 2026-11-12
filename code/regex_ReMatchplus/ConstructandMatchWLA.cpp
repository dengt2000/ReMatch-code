
#include"ConstructandMatchWLA.h"
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
template <typename... Args>void Construct_Pb_WLA<Args...>::multi_iden(string& w, vector<pair<int, int>>& section, vector<vector<vector<vector<tuple<int, int, int>>>>>& position_edges, vector<vector<vector<bool>>>& groups, int groupNum, unordered_map<int, int>& referNo2num) {
	int PbNum = 1;
	int sum = 0, n;
	unsigned char* Text;
	vector<pair<int, int>> temp_s;
	vector<pair<int, int>> Text_pair;
	this->groupNum = groupNum;
	for (int i = 0; i < section.size(); i++) {
		for (int j = section[i].first; j <= section[i].second; j++) {
			temp_s.emplace_back(make_pair(j, section[i].second));
		}
	}
	string* all_suffix = new string[temp_s.size() + 1];
	const int temp_s_size = temp_s.size();
	for (int i = 0; i < temp_s_size; i++) {//   к ׺ ַ   
		string a;
		if ((temp_s[i].second - temp_s[i].first) >= 0) {
			a.assign(w, temp_s[i].first, temp_s[i].second - temp_s[i].first + 1);
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
	if(n>=3)
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
	this->length = w.length();
	//vector<vector<bool>> Pb_mark;
	//Pb_mark.resize(length);
	//for (int i = 0; i < length; i++) {
	//	Pb_mark[i].resize(length);
	//}
	//unordered_map<pair<int, int>, bool, Hashfunc, Equalfunc> Pb_mark;
	//visited.clear();
	//vector<vector<bool>> Pb_mark(length, vector<bool>(length, false));
	//vector<bool> Pb_mark(length*length,false);
	this->Pb_mark.resize(this->length * this->length);
	this->Pb.resize(this->length + 1);
	for (int i = 0; i < this->length + 1; i++) {
		this->Pb[i].resize(this->length + 1);
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




					this->Pb[t_Pb[j].first][t_Pb[j].second] = PbNum;

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
			this->w1_len = b - a;
		}
		this->Pb_mark[a * this->length + b] = true;
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
			this->Pb[t_Pb[j].first][t_Pb[j].second] = PbNum;


		}
		//clock_t begin = clock();

		t_Pb.clear();
		PbNum++;
		//for (auto& subVector :Pb_mark) {
		//	subVector.erase(subVector.begin(), subVector.end());
		//}

		//Pb_mark.erase(Pb_mark.begin(), Pb_mark.end());
	}

	init_data_mem();
	multi_checkmatch2(position_edges, -1, -1, 1, referNo2num);




}

template <typename... Args>void Construct_Pb_WLA<Args...>::init_data_mem() {
	// length=w.length();
	// cout<<"length"<<length<<endl;
	this->leftPosition.resize(this->referCount+this->lookarounds->size());
	this->current_PbNum.resize(this->referCount + this->lookarounds->size());
	this->IsMatch = false;
	this->left.resize(this->referCount + this->lookarounds->size());
	// catchGroupPair.resize(referCount);
	this->catchGroup.resize(this->referCount);
	this->catchLength.resize(this->referCount);
	for (int i = 0; i < this->referCount; i++) {
		this->current_PbNum[i] = -1;
		// catchGroupPair[i].first = -1;
		// catchGroupPair[i].second = -1;
		this->catchGroup[i] = -1;
	}
}



template <typename... TArgs> void Construct_Pb_WLA<TArgs...>::multi_check_empty_section(vector<vector<vector<vector<tuple<int, int, int>>>>>& position_edges, int first, int second, int third, unordered_map<int, int>& referNo2num) {
	// cout<<first<<" "<<second<<" "<<third<<endl; 
	if (first == -1 && second == -1 && third == 1) {
		for (auto temp : position_edges[first + 1][second + 1][third]) {
			init_data_mem();
			int ffirst = get<0>(temp);
			int ssecond = get<1>(temp);
			int tthird = get<2>(temp);
			if (ffirst == -1 && ssecond == -1) {
				this->IsMatch = true;
				return;
			}

			// cout << ffirst << "-1,-1,1" << ssecond << " " << tthird << endl;
			multi_check_empty_section(position_edges, ffirst, ssecond, tthird, referNo2num);
			if (this->IsMatch)
				return;
		}
		return;
	}
	// cout<<visited.size()<<endl;
	int referNo = referNo2num[first];
	if (this->IsMatch) {
		return;
	}


	int len_lim = position_edges[0].size();
	if (second + 1 >= len_lim) {
		return;
	}
	if (first >= this->referCount) {
		int lookaroundNo = first - this->referCount;
		shared_ptr<LookAroundState> la = (*this->lookarounds)[lookaroundNo];
		this->inLa = true;
		if (!la->ahead && la->positive) {
			// cout <<__FILE__<< first << " " << second << " " << third << endl;
			if (third == 0) {
				LaPos = second;
				for (auto temp : position_edges[first + 1][second + 1][0]) {
					int ffirst = get<0>(temp);
					int ssecond = get<1>(temp);
					int tthird = get<2>(temp);
					multi_check_empty_section(position_edges, ffirst, ssecond, tthird, referNo2num);
					if (this->IsMatch)
						return;
				}
				this->inLa = false;
			}
			else {
				for (auto temp : position_edges[first + 1][LaPos + 1][1]) {
					int ffirst = get<0>(temp);
					int ssecond = get<1>(temp);
					int tthird = get<2>(temp);
					this->inLa = false;
					multi_check_empty_section(position_edges, ffirst, ssecond, tthird, referNo2num);
					if (this->IsMatch)
						return;
				}
				this->inLa = true;
			}
		}
		else if (!la->ahead && !la->positive) {
			if (third == 0) {
				for (auto temp : position_edges[first + 1][second + 1][0]) {
					int ffirst = get<0>(temp);
					int ssecond = get<1>(temp);
					int tthird = get<2>(temp);
					multi_check_empty_section(position_edges, ffirst, ssecond, tthird, referNo2num);
					if (this->LaMatch) {
						LaMatch = false;
						return;
					}
				}
				for (auto temp : position_edges[first + 1][second + 1][1]) {
					int ffirst = get<0>(temp);
					int ssecond = get<1>(temp);
					int tthird = get<2>(temp);
					this->inLa = false;
					multi_check_empty_section(position_edges, ffirst, ssecond, tthird, referNo2num);
					if (this->IsMatch)
						return;
				}
				this->inLa = false;
			}
			else {
				this->LaMatch = true;
				return;
			}
		}
		return;
	}

	if ((first != -1 && second != -1) && third == 0) {
		// cout<<1111<<endl;
		leftPosition[first] = second;
		// cout<<referNo<<endl;
		int cG_referno = catchGroup[referNo];

		if (catchGroup[referNo] != first && catchGroup[referNo] != -1) {
			// cout<<111<<endl;
			// cout<<catchLength[referNo]<<endl;


			// cout<<length<<endl;
			if (second + catchLength[referNo] >= this->length) {
				// cout<<1111<<endl;

				return;
			}
			using KeyType = std::tuple<TArgs...>;
			KeyType key;
			std::vector<int> indices;
			for (int i = 0; i < this->groupNum; i++) {
				// if (i != referNo) {
					this->indices.emplace_back(-1);
				// }
			}

			if (indices.size()) {
				this->assign_tuple_elements<KeyType, 0>(key, indices);

			}
			std::get<(sizeof...(TArgs)) - 1>(key) = first;
			std::get<(sizeof...(TArgs)) - 2>(key) = second;
			// std::get<(sizeof...(TArgs)) - 3>(key) = second + catchLength[referNo];

			//if (pathNum > 1) {
			if (this->visited[key]) {
				return;
			}
			else if(!inLa) {
				this->visited[key] = true;
			}

		}
	}
	else if ((first != -1 || second != -1) && third == 1)
	{
		// cout<<first<<" "<<second<<" "<<third<<endl;
		// cout<<first<<" "<<second<<endl;
		int lposition = leftPosition[first];
		int cG_referno = catchGroup[referNo];

		if (second < lposition) {
			if (first == cG_referno || cG_referno == -1) {
				//int rN2n_first=referNo2num[first];
				// current_PbNum[referNo2num[first]] =-1;
				// catchGroupPair[referNo].first = lposition;
				// catchGroupPair[referNo].second = second;
				catchGroup[referNo] = first;
				catchLength[referNo] = -1;
			}
		}
		else {
			// int cG_referno=catchGroup[referNo];
			if (first == cG_referno || cG_referno == -1) {
				// catchGroupPair[referNo].first = lposition;
				// catchGroupPair[referNo].second = second;
				catchGroup[referNo] = first;
				catchLength[referNo] = second - lposition;
			}

		}

	}
	int cG_referno = catchGroup[referNo];
	// cout<<11111<<endl;
	if (first != -1 && cG_referno != first && cG_referno != -1) {//
		// int rN2n_first= referNo2num[first];
		// cout<<1<<endl;
		// cout<<first <<" "<<second + catchLength[referNo]<<endl;
		for (auto temp : position_edges[first + 1][second + catchLength[referNo] + 1][1]) {
			// pathNum = position_edges[first + 1][second + catchLength[referNo] + 1][1].size();
			int ffirst = get<0>(temp);
			int ssecond = get<1>(temp);
			int tthird = get<2>(temp);
			// cout<<6666<<endl;
			// cout << ffirst << " 1 " << ssecond << " " << tthird << endl;

			if (ffirst == -1 && ssecond == -1) {
				this->IsMatch = true;
				return;
			}
			// cout << ffirst << " 1 " << ssecond << " " << tthird << endl;



			multi_check_empty_section(position_edges, ffirst, ssecond, tthird, referNo2num);
			if (this->IsMatch)
				return;
		}


	}
	else {
		// cout<<2<<endl;
		// cout<<position_edges[first + 1][second + 1][third]<<endl;
		// cout<<0000<<endl;
		// cout<<third<<" "<<position_edges[first + 1][second + 1].size()<<endl;
		// cout<<position_edges[first+1][second+1][third].size()<<endl;
		if (!position_edges[first + 1][second + 1][third].size()) {
			// cout<<1111<<endl;
			return;
		}
		for (auto temp : position_edges[first + 1][second + 1][third]) {
			// pathNum = position_edges[first + 1][second + 1][third].size();
			int ffirst = get<0>(temp);
			int ssecond = get<1>(temp);
			int tthird = get<2>(temp);
			// cout << ffirst << " " << ssecond << " " << tthird << endl;

			if (ffirst == -1 && ssecond == -1) {
				this->IsMatch = true;
				//cout << 22222222 << endl;
				return;
			}
			if (this->IsMatch)
				return;
			// cout << ffirst << " 2  " << ssecond << " " << tthird << endl;

			multi_check_empty_section(position_edges, ffirst, ssecond, tthird, referNo2num);
			if (this->IsMatch)
				return;
		}
	}
}




template <typename... TArgs>void Construct_Pb_WLA<TArgs...>::multi_checkmatch2(vector<vector<vector<vector<tuple<int, int, int>>>>>& position_edges, int first, int second, int third, unordered_map<int, int>& referNo2num) {
	int referNo;
	if (first == -1 && second == -1 && third == 0)
	{
		this->IsMatch = true;
		return;
	}
	if (first == -1 && second == -1 && third == 1) {
		for (auto temp : position_edges[first + 1][second + 1][third]) {
			init_data_mem();
			int ffirst = get<0>(temp);
			int ssecond = get<1>(temp);
			int tthird = get<2>(temp);
			if (ffirst == -1 && ssecond == -1) {
				this->IsMatch = true;
				return;
			}
			// cout<<first<<" "<<second<<" "<<third<<endl;
			// cout << " -1,-1,1 " <<ffirst <<" " << ssecond << " " << tthird << endl;
			multi_checkmatch2(position_edges, ffirst, ssecond, tthird, referNo2num);
			if (this->IsMatch)
				return;
		}
		return;
	}
	

	// if(first>=0&&first<referNo2num.size()){
	referNo = referNo2num[first];
	// cout<<first<<" "<<referNo<<endl;  
// }

// cout <<first<<" " << referNo << endl;

	if (this->IsMatch) {
		return;
	}
	int len_lim = position_edges[0].size();
	if (second + 1 >= len_lim) {
		return;
	}
	if (first >= this->referCount) {
		int lookaroundNo = first - this->referCount;
		shared_ptr<LookAroundState> la = (*this->lookarounds)[lookaroundNo];
		this->inLa = true;
		if (!la->ahead && la->positive) {
			if (third == 0) {
				LaPos = second;
				for (auto temp : position_edges[first + 1][second + 1][0]) {
					int ffirst = get<0>(temp);
					int ssecond = get<1>(temp);
					int tthird = get<2>(temp);
					multi_checkmatch2(position_edges, ffirst, ssecond, tthird, referNo2num);
					if (this->IsMatch)
						return;
				}
				this->inLa = false;
			}
			else {
				for (auto temp : position_edges[first + 1][LaPos + 1][1]) {
					int ffirst = get<0>(temp);
					int ssecond = get<1>(temp);
					int tthird = get<2>(temp);
					this->inLa = false;
					multi_checkmatch2(position_edges, ffirst, ssecond, tthird, referNo2num);
					if (this->IsMatch)
						return;
				}
				this->inLa = true;
			}
		}
		else if (!la->ahead && !la->positive) {
			if (third == 0) {
				for (auto temp : position_edges[first + 1][second + 1][0]) {
					int ffirst = get<0>(temp);
					int ssecond = get<1>(temp);
					int tthird = get<2>(temp);
					multi_checkmatch2(position_edges, ffirst, ssecond, tthird, referNo2num);
					if (this->LaMatch) {
						LaMatch = false;
						return;
					}
				}
				for (auto temp : position_edges[first + 1][second + 1][1]) {
					int ffirst = get<0>(temp);
					int ssecond = get<1>(temp);
					int tthird = get<2>(temp);
					this->inLa = false;
					multi_checkmatch2(position_edges, ffirst, ssecond, tthird, referNo2num);
					if (this->IsMatch)
						return;
				}
				this->inLa = false;
			}
			else {
				this->LaMatch = true;
				return;
			}
		}
		return;
	}
	if ((first != -1 && second != -1) && third == 0) {
		leftPosition[first] = second;

		if (catchGroup[referNo] != first && catchGroup[referNo] != -1) {
			if (second + catchLength[referNo] >= this->length) {
				return;
			}
			using KeyType = std::tuple<TArgs...>;
			KeyType key;
			std::vector<int> indices;

			for (int i = 0; i < this->groupNum; i++) {
				// if (i != referNo) {
					indices.emplace_back(this->current_PbNum[i]);
					/*if (i < referNo) {
						indices.emplace_back(current_PbNum[i]);
					}
					else {
						indices.emplace_back(-1);

					}*/
				// }
			}
			if (indices.size()) {
				this->assign_tuple_elements<KeyType, 0>(key, indices);

			}
			std::get<(sizeof...(TArgs)) - 1>(key) = first;
			std::get<(sizeof...(TArgs)) - 2>(key) = second;
			// std::get<(sizeof...(TArgs)) - 3>(key) = second + catchLength[referNo];

			//if (pathNum > 1) {
			if (this->visited[key]) {
				return;
			}
			else if(!inLa) {
				this->visited[key] = true;
			}
			// cout<<"visited"<<" "<<visited.size()<<endl;

			if (this->catchLength[referNo] == -1) {
				if (this->current_PbNum[referNo] != -1) {
					// cout<<1111111111<<endl;
					return;
				}
			}
			else {
				if (this->current_PbNum[referNo] != this->Pb[second][second + this->catchLength[referNo]]) {
					// cout<<2222222222<<endl;
					return;
				}
			}



		}
	}
	else if ((first != -1 || second != -1) && third == 1)
	{
		//int catchGroup[referNo] =catchGroup[referNo];
		int cG_referno = catchGroup[referNo];
		int lposition = leftPosition[first];
		if (second < lposition) {
			if (first == cG_referno || cG_referno == -1) {
				this->current_PbNum[referNo] = -1;
				// catchGroupPair[referNo].first = leftPosition[first];
				// catchGroupPair[referNo].second = second;
				catchGroup[referNo] = first;
				catchLength[referNo] = -1;
			}
			else {
				if (this->current_PbNum[referNo] != -1)
					return;
			}
		}
		else {
			//int cG_referno=catchGroup[referNo];
			if (first == cG_referno || cG_referno == -1) {

				this->current_PbNum[referNo] = this->Pb[lposition][second];
				// catchGroupPair[referNo].first = leftPosition[first];
				// catchGroupPair[referNo].second = second;
				catchGroup[referNo] = first;
				catchLength[referNo] = second - lposition;
			}
			else {
				if (this->current_PbNum[referNo] != this->Pb[lposition][second])
					return;
			}
		}



	}

	if (catchGroup[referNo] != first && first != -1 && catchGroup[referNo] != -1) {//
		// cout<<1111111111<<endl;

		for (auto temp : position_edges[first + 1][second + catchLength[referNo] + 1][1]) {
			// pathNum = position_edges[first + 1][second + catchLength[referNo] + 1][1].size();
			int ffirst = get<0>(temp);
			int ssecond = get<1>(temp);
			int tthird = get<2>(temp);

			if (ffirst == -1 && ssecond == -1) {
				this->IsMatch = true;
				return;
			}
			if (this->IsMatch)
				return;

			// cout<<"catchLength"<<catchLength[referNo]<<endl;
			// cout << " 11 " <<ffirst<<" " << ssecond << " " << tthird << endl;

			multi_checkmatch2(position_edges, ffirst, ssecond, tthird, referNo2num);
			if (this->IsMatch)
				return;
		}


	}
	else {
		for (auto temp : position_edges[first + 1][second + 1][third]) {
			// pathNum = position_edges[first + 1][second + 1][third].size();
			int ffirst = get<0>(temp);
			int ssecond = get<1>(temp);
			int tthird = get<2>(temp);
			//cout << ffirst << " " << ssecond << " " << tthird << endl;

			if (ffirst == -1 && ssecond == -1) {
				this->IsMatch = true;

				return;
			}
			if (this->IsMatch)
				return;
			// cout<<first<<" "<<second<<" "<<third<<"->";
			// cout <<" 22 " << ffirst <<" " << ssecond << " " << tthird << endl;

			multi_checkmatch2(position_edges, ffirst, ssecond, tthird, referNo2num);
			if (this->IsMatch)
				return;
		}
	}
}







