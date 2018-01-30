//============================================================================
// Name        : test.cpp
// Author      : Junhua Zhang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

struct Query_Graph {
	vector<char> labels;
	vector<vector<int> > queryVertexToEdges;
};

vector<unsigned int> selfsimulation(Query_Graph & q) {
	vector<unsigned int> prevsims;
	vector<unsigned int> sims;
	vector<unsigned int> removes;
	for (unsigned int v = 0; v < q.labels.size(); v++) {
		//init prevsim of v
		unsigned int prevsim = 0;
		for (unsigned int j = 0; j < q.labels.size(); j++) {
			prevsim |= 1 << j;
		}
		prevsims.push_back(prevsim);

		//init sim of v
		if (q.queryVertexToEdges[v].size() == 0) {
			unsigned int sim = 0;
			for (unsigned int u = 0; u < q.labels.size(); u++) {
				if (q.labels[v] == q.labels[u]) {
					sim |= 1 << u;
				}
			}
			sims.push_back(sim);
		} else {
			unsigned int sim = 0;
			for (unsigned int u = 0; u < q.labels.size(); u++) {
				if (q.labels[v] == q.labels[u]
						&& q.queryVertexToEdges[u].size() > 0) {
					sim |= 1 << u;
				}
			}
			sims.push_back(sim);
		}

		//init remove of v
		unsigned int pre_V = 0;
		for (unsigned int i = 0; i < q.labels.size(); i++) { //init pre_V
			if (q.queryVertexToEdges[i].size() > 0) {
				pre_V |= 1 << i;
			}
		}

		unsigned int pre_sim = 0;
		for (unsigned int i = 0; i < q.labels.size(); i++) { //init pre_sim_v
			unsigned int child = 0;
			for (unsigned int j = 0; j < q.queryVertexToEdges[i].size(); j++) {
				child |= 1 << q.queryVertexToEdges[i][j];
			}
			if (child & sims[v]) {
				pre_sim |= 1 << i;
			}
		}

		removes.push_back(pre_V & (~pre_sim)); //push the value of remove_v
	}

	bool exist = false;
	int v = -1;
	for (vector<unsigned int>::iterator it = removes.begin();
			it != removes.end(); it++) {
		exist = exist || *it;
		v++;
		if (exist)
			break;
	}

	while (exist) {
		for (unsigned int u = 0; u < q.labels.size(); u++) {
			unsigned int childs = 0;
			for (vector<int>::iterator child = q.queryVertexToEdges[u].begin();
					child != q.queryVertexToEdges[u].end(); child++) {
				childs |= 1 << *child;
			}
			if (childs & 1 << v) { //for all u in pre(v)
				for (unsigned int w = 0; w < q.labels.size(); w++) {
					if (removes[v] & 1 << w) { //for all w in remove(v)
						if (sims[u] & 1 << w) { //if w in sim(u)
							sims[u] &= ~(1 << w);

							for (unsigned int w__ = 0; w__ < q.labels.size();
									w__++) {
								unsigned int childs = 0;
								for (vector<int>::iterator child =
										q.queryVertexToEdges[w__].begin();
										child != q.queryVertexToEdges[w__].end();
										child++) {
									childs |= 1 << *child;
								}
								if (childs & 1 << w) { //for all w__ in pre(w)
									if (!(childs & sims[u])) {
										removes[u] |= 1 << w__;
									}
								}
							}
						}
					}
				}
			}
		}

		prevsims[v] = sims[v];
		removes[v] = 0;

		exist = false;
		v = -1;
		for (vector<unsigned int>::iterator it = removes.begin();
				it != removes.end(); it++) {
			exist = exist || *it;
			v++;
			if (exist)
				break;
		}
	}

	return sims;
}

vector<int> leastMatchCount(vector<unsigned int> & sims) {
	vector<int> least_match_counts;

	for (unsigned int v = 0; v < sims.size(); v++) {
		int least_match_count = 0;
		for (unsigned int i = 0; i < sims.size(); i++) {
			if (sims[v] & 1 << i) {
				least_match_count++;
			}
		}
		least_match_counts.push_back(least_match_count);
	}

	return least_match_counts;
}

vector<vector<int> > leastMatchCount2(vector<char> & labels,
		vector<vector<int> > & queryVertexToEdges) {
	vector<unsigned int> prevsims;
	vector<unsigned int> sims;
	vector<unsigned int> removes;
	for (unsigned int v = 0; v < labels.size(); v++) {
		//init prevsim of v
		unsigned int prevsim = 0;
		for (unsigned int j = 0; j < labels.size(); j++) {
			prevsim |= 1 << j;
		}
		prevsims.push_back(prevsim);

		//init sim of v
		if (queryVertexToEdges[v].size() == 0) {
			unsigned int sim = 0;
			for (unsigned int u = 0; u < labels.size(); u++) {
				if (labels[v] == labels[u]) {
					sim |= 1 << u;
				}
			}
			sims.push_back(sim);
		} else {
			unsigned int sim = 0;
			for (unsigned int u = 0; u < labels.size(); u++) {
				if (labels[v] == labels[u]
						&& queryVertexToEdges[u].size() > 0) {
					sim |= 1 << u;
				}
			}
			sims.push_back(sim);
		}

		//init remove of v
		unsigned int pre_V = 0;
		for (unsigned int i = 0; i < labels.size(); i++) { //init pre_V
			if (queryVertexToEdges[i].size() > 0) {
				pre_V |= 1 << i;
			}
		}

		unsigned int pre_sim = 0;
		for (unsigned int i = 0; i < labels.size(); i++) { //init pre_sim_v
			unsigned int child = 0;
			for (unsigned int j = 0; j < queryVertexToEdges[i].size(); j++) {
				child |= 1 << queryVertexToEdges[i][j];
			}
			if (child & sims[v]) {
				pre_sim |= 1 << i;
			}
		}

		removes.push_back(pre_V & (~pre_sim)); //push the value of remove_v
	}

	bool exist = false;
	int v = -1;
	for (vector<unsigned int>::iterator it = removes.begin();
			it != removes.end(); it++) {
		exist = exist || *it;
		v++;
		if (exist)
			break;
	}

	while (exist) {
		for (unsigned int u = 0; u < labels.size(); u++) {
			unsigned int childs = 0;
			for (vector<int>::iterator child = queryVertexToEdges[u].begin();
					child != queryVertexToEdges[u].end(); child++) {
				childs |= 1 << *child;
			}
			if (childs & 1 << v) { //for all u in pre(v)
				for (unsigned int w = 0; w < labels.size(); w++) {
					if (removes[v] & 1 << w) { //for all w in remove(v)
						if (sims[u] & 1 << w) { //if w in sim(u)
							sims[u] &= ~(1 << w);

							for (unsigned int w__ = 0; w__ < labels.size();
									w__++) {
								unsigned int childs = 0;
								for (vector<int>::iterator child =
										queryVertexToEdges[w__].begin();
										child != queryVertexToEdges[w__].end();
										child++) {
									childs |= 1 << *child;
								}
								if (childs & 1 << w) { //for all w__ in pre(w)
									if (!(childs & sims[u])) {
										removes[u] |= 1 << w__;
									}
								}
							}
						}
					}
				}
			}
		}

		prevsims[v] = sims[v];
		removes[v] = 0;

		exist = false;
		v = -1;
		for (vector<unsigned int>::iterator it = removes.begin();
				it != removes.end(); it++) {
			exist = exist || *it;
			v++;
			if (exist)
				break;
		}
	}

//	return sims;

	vector<vector<int> > least_match_counts;
	least_match_counts.resize(queryVertexToEdges.size());

	for (unsigned int src = 0; src < queryVertexToEdges.size(); src++) {
		least_match_counts[src].resize(queryVertexToEdges[src].size(), 0);
		for (unsigned int dst = 0; dst < queryVertexToEdges[src].size();
				dst++) {
			for (unsigned int i = 0; i < queryVertexToEdges[src].size(); i++) {
				if (sims[queryVertexToEdges[src][dst]]
						& 1 << queryVertexToEdges[src][i]) {
					least_match_counts[src][dst]++;
				}
			}
		}
	}
	return least_match_counts;
}

void testsimulation() {
	Query_Graph q;

	{/*
	 q.labels.push_back('a');
	 q.labels.push_back('b');
	 q.labels.push_back('b');
	 q.labels.push_back('b');
	 q.labels.push_back('c');
	 q.labels.push_back('c');
	 q.labels.push_back('d');

	 vector<int> neighborEdges;

	 neighborEdges.push_back(1);
	 neighborEdges.push_back(2);
	 neighborEdges.push_back(3);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 neighborEdges.push_back(4);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 neighborEdges.push_back(5);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 neighborEdges.push_back(6);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();
	 */
	}

	{
		q.labels.push_back('a');
		q.labels.push_back('d');
		q.labels.push_back('c');
		q.labels.push_back('b');
		q.labels.push_back('c');
		q.labels.push_back('c');
		q.labels.push_back('a');
		q.labels.push_back('b');
		q.labels.push_back('d');

		vector<int> neighborEdges;

		neighborEdges.push_back(3);
		neighborEdges.push_back(4);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(0);
		neighborEdges.push_back(2);
		neighborEdges.push_back(3);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(1);
		neighborEdges.push_back(8);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(2);
		neighborEdges.push_back(4);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(5);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(8);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(5);
		neighborEdges.push_back(7);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(5);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(3);
		neighborEdges.push_back(5);
		neighborEdges.push_back(7);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

	}

//	for (unsigned int i = 0; i < q.labels.size(); i++) {
//		printf("id:%d label:%c neib:", i, q.labels[i]);
//		for (vector<int>::iterator it = q.queryVertexToEdges[i].begin();
//				it != q.queryVertexToEdges[i].end(); it++) {
//			printf(" %d", *it);
//		}
//		printf("\n");
//	}

	//=====================================algorithm================================================
	vector<unsigned int> sims = selfsimulation(q);

	for (unsigned int v = 0; v < q.labels.size(); v++) {
		printf("sim(%d)=", v);
		for (unsigned int i = 0; i < q.labels.size(); i++) {
			if (sims[v] & 1 << i) {
				printf("%d ", i);
			}
		}
		printf("\n");
	}

	vector<int> least_match_counts = leastMatchCount(sims);

	for (unsigned int v = 0; v < least_match_counts.size(); v++) {
		printf("the least match count of %d is %d.\n", v,
				least_match_counts[v]);
	}

//	printf("\n");
}

void testsimulation2() {
	Query_Graph q;

	{
		q.labels.push_back('a');
		q.labels.push_back('b');
		q.labels.push_back('b');
		q.labels.push_back('b');
		q.labels.push_back('c');
		q.labels.push_back('c');
		q.labels.push_back('d');
		q.labels.push_back('b');

		vector<int> neighborEdges;

		neighborEdges.push_back(1);
		neighborEdges.push_back(2);
		neighborEdges.push_back(3);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(4);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(5);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

//	 neighborEdges.push_back(7);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(6);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(7);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();
	}

	{/*
	 q.labels.push_back('a');
	 q.labels.push_back('d');
	 q.labels.push_back('c');
	 q.labels.push_back('b');
	 q.labels.push_back('c');
	 q.labels.push_back('c');
	 q.labels.push_back('a');
	 q.labels.push_back('b');
	 q.labels.push_back('d');

	 vector<int> neighborEdges;

	 neighborEdges.push_back(3);
	 neighborEdges.push_back(4);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 neighborEdges.push_back(0);
	 neighborEdges.push_back(2);
	 neighborEdges.push_back(3);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 neighborEdges.push_back(1);
	 neighborEdges.push_back(8);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 neighborEdges.push_back(2);
	 neighborEdges.push_back(4);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 neighborEdges.push_back(5);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 neighborEdges.push_back(8);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 neighborEdges.push_back(5);
	 neighborEdges.push_back(7);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 neighborEdges.push_back(5);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 neighborEdges.push_back(3);
	 neighborEdges.push_back(5);
	 neighborEdges.push_back(7);
	 q.queryVertexToEdges.push_back(neighborEdges);
	 neighborEdges.clear();

	 */
	}

//	for (unsigned int i = 0; i < q.labels.size(); i++) {
//		printf("id:%d label:%c neib:", i, q.labels[i]);
//		for (vector<int>::iterator it = q.queryVertexToEdges[i].begin();
//				it != q.queryVertexToEdges[i].end(); it++) {
//			printf(" %d", *it);
//		}
//		printf("\n");
//	}

	//=====================================algorithm================================================
	vector<unsigned int> sims = selfsimulation(q);

	for (unsigned int v = 0; v < q.labels.size(); v++) {
		printf("sim(%d)=", v);
		for (unsigned int i = 0; i < q.labels.size(); i++) {
			if (sims[v] & 1 << i) {
				printf("%d ", i);
			}
		}
		printf("\n");
	}

	vector<int> least_match_counts = leastMatchCount(sims);

	for (unsigned int v = 0; v < least_match_counts.size(); v++) {
		printf("the least match count of %d is %d.\n", v,
				least_match_counts[v]);
	}

	vector<vector<int> > least_match_counts2 = leastMatchCount2(q.labels,
			q.queryVertexToEdges);

	for (int src = 0; src < least_match_counts2.size(); src++) {
		for (int dst = 0; dst < least_match_counts2[src].size(); dst++) {
			printf("%d ", least_match_counts2[src][dst]);
		}
		printf("\n");
	}
//	printf("\n");
}

void testvector() {
	vector<int> a;
	a.resize(10, 1);
	for (int i = 0; i < a.size(); i++) {
		printf("%d,", a[i]);
	}
}

vector<unsigned int> selfdualsimulation(vector<char> & labels,
		vector<vector<int> > & outEdges) {

	vector<vector<int> > inEdges;
	inEdges.resize(outEdges.size());
	for (int i = 0; i < outEdges.size(); i++) {
		for (int j = 0; j < outEdges[i].size(); j++) {
			inEdges[outEdges[i][j]].push_back(i);
		}
	}

	vector<unsigned int> prevsims;
	vector<unsigned int> sims;
	vector<unsigned int> removes_pre;
	vector<unsigned int> removes_suc;

	for (unsigned int v = 0; v < labels.size(); v++) {
		//init prevsim of v
		unsigned int prevsim = 0;
		for (unsigned int j = 0; j < labels.size(); j++) {
			prevsim |= 1 << j;
		}
		prevsims.push_back(prevsim);

		//init sim of v
		unsigned int sim = 0;
		if (outEdges[v].size() == 0) {
			if (inEdges[v].size() == 0) {
				for (unsigned int u = 0; u < labels.size(); u++)
					if (labels[v] == labels[u])
						sim |= 1 << u;
			} else {
				for (unsigned int u = 0; u < labels.size(); u++)
					if (labels[v] == labels[u] && inEdges[u].size() > 0)
						sim |= 1 << u;
			}
		} else {
			if (inEdges[v].size() == 0) {
				for (unsigned int u = 0; u < labels.size(); u++)
					if (labels[v] == labels[u] && outEdges[u].size() > 0)
						sim |= 1 << u;
			} else {
				for (unsigned int u = 0; u < labels.size(); u++)
					if (labels[v] == labels[u] && outEdges[u].size() > 0
							&& inEdges[u].size() > 0)
						sim |= 1 << u;
			}
		}
		sims.push_back(sim);

		//init remove_pre of v
		unsigned int pre_V = 0;
		for (unsigned int i = 0; i < labels.size(); i++) { //init pre_V
			if (outEdges[i].size() > 0) {
				pre_V |= 1 << i;
			}
		}

		unsigned int pre_sim = 0;
		for (unsigned int i = 0; i < labels.size(); i++) { //init pre_sim_v
			unsigned int child = 0;
			for (unsigned int j = 0; j < outEdges[i].size(); j++) {
				child |= 1 << outEdges[i][j];
			}
			if (child & sims[v]) {
				pre_sim |= 1 << i;
			}
		}

		removes_pre.push_back(pre_V & (~pre_sim)); //push the value of remove_pre_v

		//init remove_suc of v
		unsigned int suc_V = 0;
		for (unsigned int i = 0; i < labels.size(); i++) { //init suc_V
			if (inEdges[i].size() > 0) {
				suc_V |= 1 << i;
			}
		}

		unsigned int suc_sim = 0;
		for (unsigned int i = 0; i < labels.size(); i++) { //init suc_sim_V
			if (sims[v] & 1 << i) {
				for (unsigned int j = 0; j < outEdges[i].size(); j++) {
					suc_sim |= 1 << outEdges[i][j];
				}
			}
		}
		removes_suc.push_back(suc_V & (~suc_sim)); //push the value of remove_suc_v
	}


//	vector<unsigned int> prevsims;
//	vector<unsigned int> sims;
//	vector<unsigned int> removes_pre;
//	vector<unsigned int> removes_suc;
	for(int i=0;i<sims.size();i++){
		printf("sims of %d:",i);
		for(int j=0;j<labels.size();j++){
			if(sims[i] & 1 << j){
				printf("%d ",j);
			}
		}
		printf("\n");
	}
	printf("=============sims end=============\n\n");

	for(int i=0;i<removes_pre.size();i++){
		printf("remove_pre of %d:",i);
		for(int j=0;j<labels.size();j++){
			if(removes_pre[i] & 1<<j){
				printf("%d ",j);
			}
		}
		printf("\n");
	}
	printf("===========removes_pre end===========\n\n");

	for(int i=0;i<removes_pre.size();i++){
		printf("remove_suc of %d:",i);
		for(int j=0;j<labels.size();j++){
			if(removes_suc[i] & 1<<j){
				printf("%d ",j);
			}
		}
		printf("\n");
	}
	printf("===========removes_suc end===========\n\n");



	bool exist = false;
	int v;
	for (unsigned int i = 0; i < removes_pre.size(); i++) {
		exist = exist || removes_pre[i] || removes_suc[i];
		v = i;
		if (exist)
			break;
	}

	while (exist) {
		if (removes_pre[v]) {
			for (unsigned int u = 0; u < labels.size(); u++) {
				unsigned int childs = 0;
				for (vector<int>::iterator child = outEdges[u].begin();
						child != outEdges[u].end(); child++) {
					childs |= 1 << *child;
				}
				if (childs & 1 << v) { //for all u in pre(v)
					for (unsigned int w = 0; w < labels.size(); w++) {
						if (removes_pre[v] & 1 << w) { //for all w in remove(v)
							if (sims[u] & 1 << w) { //if w in sim(u)
								sims[u] &= ~(1 << w);

								for (unsigned int w__ = 0; w__ < labels.size();
										w__++) {
									unsigned int childs = 0;
									for (vector<int>::iterator child =
											outEdges[w__].begin();
											child != outEdges[w__].end();
											child++) {
										childs |= 1 << *child;
									}
									if (childs & 1 << w) { //for all w__ in pre(w)
										if (!(childs & sims[u])) {
											removes_pre[u] |= 1 << w__;
										}
									}
								}
							}
						}
					}
				}
			}

			prevsims[v] = sims[v];
			removes_pre[v] = 0;
		} else {
			for (unsigned int u = 0; u < outEdges[v].size(); u++) { //for all u in suc(v)
				for (unsigned int w = 0; w < labels.size(); w++) {
					if (removes_suc[v] & 1 << w) { //for all w in remove(v)
						if (sims[u] & 1 << w) { //w simulate u
							sims[u] &= ~(1 << w);

							for (unsigned int w__ = 0; w__ < outEdges[w].size();
									w__++) {
								unsigned int parents = 0;
								for (vector<int>::iterator parent =
										inEdges[w__].begin();
										parent != inEdges[w__].end();
										parent++) {
									parents |= 1 << *parent;
								}

								if (!(parents & sims[u])) {
									removes_suc[u] |= 1 << w__;
								}

							}

						}
					}
				}
			}
			prevsims[v]=sims[v];
			removes_suc[v]=0;
		}

		exist = false;
		for (unsigned int i = 0; i < removes_pre.size(); i++) {
			exist = exist || removes_pre[i] || removes_suc[i];
			v = i;
			if (exist)
				break;
		}
	}
	return sims;
}

void testselfdualsimulation1(){

	Query_Graph q;

	{
		q.labels.push_back('a');
		q.labels.push_back('b');
		q.labels.push_back('b');
		q.labels.push_back('b');
		q.labels.push_back('c');
		q.labels.push_back('c');
		q.labels.push_back('d');
		q.labels.push_back('b');

		vector<int> neighborEdges;

		neighborEdges.push_back(1);
		neighborEdges.push_back(2);
		neighborEdges.push_back(3);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(4);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(5);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(6);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		neighborEdges.push_back(7);
		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();

		q.queryVertexToEdges.push_back(neighborEdges);
		neighborEdges.clear();
	}

	//=====================================algorithm================================================
	vector<unsigned int> sims = selfdualsimulation(q.labels,q.queryVertexToEdges);
//	vector<unsigned int> sims = selfsimulation(q);


	for (unsigned int v = 0; v < q.labels.size(); v++) {
		printf("sim(%d)=", v);
		for (unsigned int i = 0; i < q.labels.size(); i++) {
			if (sims[v] & 1 << i) {
				printf("%d ", i);
			}
		}
		printf("\n");
	}

	vector<int> least_match_counts = leastMatchCount(sims);

	for (unsigned int v = 0; v < least_match_counts.size(); v++) {
		printf("the least match count of %d is %d.\n", v,
				least_match_counts[v]);
	}

	vector<vector<int> > least_match_counts2 = leastMatchCount2(q.labels,
			q.queryVertexToEdges);

	for (int src = 0; src < least_match_counts2.size(); src++) {
		for (int dst = 0; dst < least_match_counts2[src].size(); dst++) {
			printf("%d ", least_match_counts2[src][dst]);
		}
		printf("\n");
	}
//	printf("\n");

}


std::string bitmap2string(unsigned int a) {
	std::string s="";
	for (int i = 31; i >= 0; i--) {
		if(a & 1<<i){
			s=s+"1";
		}else{
			s=s+"0";
		}
		if (i % 4 == 0)
			s=s+" ";
	}
	return s;
}


void outputbitmap(unsigned int a){
	for(int i=31;i>=0;i--){
		printf("%d",(a & 1<<i)>>i);
		if(i%4==0)
			printf(" ");
	}
}

void testoutputbitmap(){
	unsigned int a=0x78f;
	outputbitmap(a);
	printf("\na%sa\n",bitmap2string(a).c_str());
}

int main() {
//	testvector();
//	testsimulation2();
//	testselfdualsimulation1();
	testoutputbitmap();
	return 0;
}

