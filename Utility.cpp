#include"Utility.h"
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>  
#include <set>
#include <random>
#include<numeric>

using namespace std;
//Preprocessing function

int k, threads = 1;
string alg, seed, mergeMethod, expandMethod;
TStr dataset;

void usage() {
	printf("Usage:\n");
	printf("\tKVCC.exe [-a BkVCC/VCCE] [-d DBLP] [-k 10] [-s rand/clique/k+1-clique] [-e flow/nbr] [-m flow/old] [-t 1-16](thread num)\n");
	printf("e.g. KVCC.exe -a BkVCC -d DBLP -k 10 -s rand -e flow -m flow -t 8");
}



int get_vertex_id(int u, int& num, int* vertex_map_) {
	if (vertex_map_[u] < 0) {
		vertex_map_[u] = num++;
	}
	return vertex_map_[u];
}

void format_graph(string src)
{
	printf("Create binary graph from .txt file: %s\n", src.c_str());

	// input example "./dataset/stanford.txt"
	string::size_type pos = src.rfind("/");

	// get file direcotry
	// e.g. "./dataset/"
	string file_dir = src.substr(0, pos + 1);

	// get graph_name
	// e.g. "stanford"
	string graph_name = src.substr(pos + 1, src.rfind(".") - pos - 1);

	// create out file 
	// e.g. "./dataset/stanford"
	string out = file_dir +   graph_name + "_Processed" + ".txt";

	ifstream fp;
	ofstream fp2;
	fp.open(src.c_str());
	fp2.open(out.c_str());
	//FILE* fp = open_file(src.c_str(), "r");
	//FILE* fp2 = open_file(out.c_str(), "w");
	string line;
	int nodes = 0;
	int u, v;
	int num = 0;

	while (getline(fp, line)) {
		fp2 << line << endl;
		if (line[2] == 'F')
			break;
		if (line[2] == 'N') {
			string::size_type pos1 = line.find("Nodes: ");
			string file_dir1 = line.substr(pos1 + 7);
			cout << file_dir1 << endl;
			stringstream ss(file_dir1);
			ss >> nodes;
			cout << "Node_Nums:" << nodes << endl;
			
			
		}
		
		
	}
	int *vertex_map_ = new int[10000000];
	::memset(vertex_map_, -1, sizeof(int) * 10000000);
	
	
	while (getline(fp, line)) {

		if (line[0] < '0' || line[0] > '9')
			continue;

		sscanf_s(line.c_str(), "%d\t%d", &u, &v);

		if (u == v) continue;


		u = get_vertex_id(u, num, vertex_map_);
		v = get_vertex_id(v, num, vertex_map_);

		
		//fprintf(fp2, "%d\t%d", u, v);
		fp2 << u << "\t" << v << endl;

	}

	int* new2old = new int[num];
	int cur = 0;
	int i = 0;
	while (cur < num) {
		if (vertex_map_[i] == -1) {
			++i;
			continue;
		}
		new2old[vertex_map_[i]] = i;
		++cur;
		++i;
	}
	string mts = "./dataset/match.st";
	errno_t err;
	FILE *mch;
	err = fopen_s(&mch, mts.c_str(), "wb");
	//FILE* mch = fopen_s(mts.c_str(), "wb");
	if (mch) {
		fwrite(new2old, sizeof(int), num, mch);
		fclose(mch);
	}

	delete[] new2old;
	delete[] vertex_map_;



	/*read part*/
	//FILE *mch2;
	//int* new2old = new int[num];
	//err = fopen_s(&mch2, mts.c_str(), "rb");
	//fread(new2, sizeof(int), num, mch);
	//fclose(mch);

	//ifstream fp3;
	//fp3.open(out.c_str());
	//while (getline(fp3, line)) {//fgets(line, 100, fp)

	//	if (line[0] < '0' || line[0] > '9')
	//		continue;

	//	sscanf_s(line.c_str(), "%d\t%d", &u, &v);

	//	if (u == v) continue;
	//	u = new2old[u];
	//	v = new2old[v];
	//	cout << u << "\t" << v << endl;
	//}


}



//Others





//int conb[51][51] = {};

void backtrack(TIntV nums, int start, int k, int alpha, TIntV& track, TVec<TIntV>& res);


/* 返回数组中 k 个随机节点的值 */
TIntV getRandom(TIntV nums) {
	TIntV tmp;
	//int *tmp = new int[k];
	//cout << nums.Len() << endl;
	TIntV::TIter p = nums.BegI();

	// 前 k 个元素先默认选上
	for (int i = 0; i < k && p != nums.EndI(); i++) {
		tmp.Add(*p);
		p++;
	}

	int i = k;
	// while 循环遍历链表

	while (p != nums.EndI()) {
		i++;
		// 生成一个 [0, i) 之间的整数
		int j = rand() % i;
		//cout << j << endl;
		// 这个整数等于 0 的概率就是 1/i
		if (j < k) {
			tmp[j] = *p;
		}
		p++;

	}
	//track.AddUnique(val);

	tmp.Sort();
	return tmp;
}


int Count(int n, int m)
{
	if (n == m || m == 0) return 1;

	vector<int> dp(m + 1);
	for (int i = 0; i <= n; i++)
		for (int j = min(i, m); j >= 0; j--)
		{
			if (i == j || j == 0) dp[j] = 1;
			else dp[j] = dp[j] + dp[j - 1];
			if (dp[j] > 1000) return dp[j];
		}
			
	return dp[m];
}

TVec<TIntV> randSample(TIntV nums, int k, int alpha)
{
	TVec<TIntV> res;
	TIntV track;
	res.Clr();
	int n = nums.Len();
	int m = k;

	//int flag = 0;//标记组合数是否足够大，0则调用子集，1则调用随机生成

	
	TIntV temp_v;
	temp_v.Clr();
	/*cout << "num: " << Count(n, m) << endl;*/

	if (Count(n, m) < alpha) {
		if (m > n / 2) {
			TVec<TIntV>temp =  subsets(nums, n - k, alpha, track, res);
			for (int i = 0; i < temp.Len(); i++) {
				temp_v = temp[i];
				temp[i] = nums;
				temp[i].Diff(temp_v);
			}
			
			return temp;
			
		}
		else {
			
			return subsets(nums, k, alpha, track, res);
		}
		
	}

	
	
	//随机生成子集
	while (res.Len() < alpha) {
		track.Clr();
		
		/*while (track.Len() < k) {
			int t = rand() % (n + 1);
			track.AddUnique(nums[t]);
		}*/
		track = getRandom(nums);
		//cout << track.Len() << endl;
		res.Add(TIntV(track));
		

	}
	res.Merge();
	return res;
}




TVec<TIntV> subsets(TIntV nums, int k, int alpha, TIntV& track, TVec<TIntV>& res) {
	res.Clr();
	track.Clr();
	int n = nums.Len();
	backtrack(nums, 0, k, alpha, track, res);
	/*if(n < 50) backtrack(nums, 0, k, alpha);
	else {
		while (res.Len() < alpha) {
			track.Clr();
			while (track.Len() < k) {
				int t = rand() % n;
				track.Add(nums[t]);
			}
			res.Add(track);
		}
		res.Merge();
	}*/
	return res;
}

//TVec<TIntV> subsets(TIntV nums, int k, int alpha, TIntV& track, TVec<TIntV>& res) {
//	res.Clr();
//	track.Clr();
//
//	generateCombinations(nums, k, res, alpha);
//
//	return res;
//}

void printCombination(TIntV& nums, int bitmask, TVec<TIntV>& res) {
	TIntV combination;
	for (int i = 0; i < nums.Len(); i++) {
		if ((bitmask >> i) & 1) {
			combination.Add(nums[i]);
		}
	}
	res.Add(combination);
	//for (int i = 0; i < track.Len(); i++) {
	//	cout << track[i] << " ";
	//}
	//cout << endl;
}

void generateCombinations(TIntV& nums, int k, TVec<TIntV>& res, int alpha) {
	int n = nums.Len();
	if (k == 0) {
		return;
	}

	int bitmask = (1 << k) - 1;
	//cout << "k: " << k << endl;
	//cout << "bitmask: " << bitmask << endl;
	while (bitmask < (1 << n)) {
		if(res.Len() > alpha) 
		{
			cout << "res: " << res.Len() << endl;
			cout << "n: " << nums.Len() << endl;
			cout << "k: " << k << endl;
			cout << "num: " << Count(n, k) << endl;
		}
		//if (res.Len() > alpha) break;
		printCombination(nums, bitmask, res);
		//cout << "bitmask: " << bitmask << endl;
		int lowestBit = bitmask & -bitmask;
		int resetBits = bitmask + lowestBit;
		/*cout << "lowestBit: "<< lowestBit <<endl;*/
		bitmask = (((bitmask & ~resetBits) / lowestBit) >> 1) | resetBits;
	}
}




void backtrack(TIntV nums, int start, int k, int alpha, TIntV& track, TVec<TIntV>& res) {
	

	if (track.Len() == k) {
		res.Add(TIntV (track));
		return;
	}
	/*cout << "res.Len(): "<<res.Len() << endl;
	cout << track.Len() << endl;*/
	
	
	for (int i = start; i < nums.Len(); i++) {
		track.Add(nums[i]);
		backtrack(nums, i + 1, k, alpha, track, res);
		track.DelLast();
	}
}


double computeFscore(TIntVIntV S, TIntVIntV T)
{
	// 1/4 修改了F-score计算方式，应该是针对每个真实社区计算F-score，取最大值
	vector<double> F_score;
	double intrs, prec, rec, F, F_max;
	for (TIntVIntV::TIter I1 = S.BegI(); I1 < S.EndI(); I1++) {
		F_max = 0;

		for (TIntVIntV::TIter I2 = T.BegI(); I2 < T.EndI(); I2++) {
			intrs = I1->IntrsLen(*I2);
			if (intrs == 0) continue;
			prec = intrs / I1->Len(); // prec = |S intrs T| / |S|
			//rec = intrs / I2->Len(); // rec = |S intrs T| / |T|
			//F = 2 * (prec * rec) / (prec + rec);
			F = prec;
			if (F > F_max) F_max = F;
		}
		
		F_score.push_back(F_max);
		
	}
	double F_result = std::accumulate(F_score.begin(), F_score.end(), 0.0) / F_score.size();
	return F_result;
}


double computeFsame(TIntVIntV S, TIntVIntV T) {
	return(computeFscore(S, T) + computeFscore(T, S)) / 2.;
}

int getDegeneracy(PUNGraph G_in) {
	TUNGraph G = *G_in;
	TIntVIntV D(10000, 0); //bigger maximum degree need more memory.
	TIntV L;
	int deg, maxdeg = 0;
	for (TUNGraph::TNodeI NI = G.BegNI(); NI < G.EndNI(); NI++) {
		deg = NI.GetDeg();
		maxdeg = max(deg, maxdeg);
		D[deg].Add(NI.GetId());
		//cout <<deg<<" "<< L[deg].Len() << endl;
	}
	/*for (int i = 0; i < maxdeg; i++) {
		cout << "i= " << i << endl;;
		for (TIntV::TIter LI = D[i].BegI(); LI < D[i].EndI(); LI++) {
			cout << *LI <<" ";
		}
		cout << endl;
	}*/
	int k = 0, v;
	int n = G.GetNodes();
	while (n != 0) {
		//std::cout << n <<endl;
		for (int i = 0; i < maxdeg; i++) {
			if (!D[i].Empty()) {
				k = max(k, i);
				v = D[i].GetRndVal();
				//cout << v << endl;

				L.Add(v);
				D[i].DelIfIn(v);
				for (int d = 0; d < G.GetNI(v).GetDeg(); d++) {
					int nbr = G.GetNI(v).GetNbrNId(d);
					if (!L.IsIn(nbr)) {
						int d_nbr = G.GetNI(nbr).GetDeg();
						D[d_nbr].DelIfIn(nbr);
						D[d_nbr - 1].Add(nbr);
					}
				}
				G.DelNode(v);
				break;
			}

		}
		n--;
	}
	
	cout << k << endl;
	cout << G.GetNodes() << endl;
	cout << G_in->GetNodes() << endl;
	return k;

}