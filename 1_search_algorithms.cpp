#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <malloc.h>
#include <algorithm>
#include <cstdlib>

using namespace std;

int dxy[4][2] = { {1,0}, {0,1}, {-1,0}, {0,-1} };

int dfs_cnt = 0;
int dfs_m = -1;
vector<vector<vector<int>>> closed_list;
vector<vector<int>> goal = {
	{1,2,3},
	{4,5,6},
	{7,8,0} };

typedef struct N {
	int now_x,now_y, len;
	vector<vector<int>> arr;

	N(int _now_x, int _now_y, int _len, vector<vector<int>> _arr) :
		now_x(_now_x), now_y(_now_y), len(_len), arr(_arr) {};
} forq;

typedef struct B {
	int prev_x, prev_y, now_x, now_y, len;
	vector<vector<int>> arr;

	B(int _now_x, int _now_y, int _prev_x, int _prev_y, int _len, vector<vector<int>> _arr) :
		now_x(_now_x), now_y(_now_y), prev_x(_prev_x), prev_y(_prev_y), len(_len), arr(_arr) {};
} BB;

typedef struct P {
	int now_x, now_y,oop, len;
	vector<vector<int>> arr;

	P(int _now_x, int _now_y, int oop, int _len, vector<vector<int>> _arr) :
		now_x(_now_x), now_y(_now_y), oop(oop), len(_len), arr(_arr) {};
} pqNode;

vector<vector<int>> swap_arr(vector<vector<int>> v, int now_x, int now_y, int next_x, int next_y) {
	int i, j;
	vector<vector<int>> r(3, vector<int> (3));
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			r[i][j] = v[i][j];
		}
	}

	r[now_x][now_y] = v[next_x][next_y];
	r[next_x][next_y] = 0;

	return r;
}

void print_arr(vector<vector<int>> v) {
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			cout << v[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

/*
bool check(vector<vector<int>> now, vector<vector<int>> target) {
	if (now ==)
}
*/

pair<int,int> find_initial_zero(vector<vector<int>> v) {
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (!v[i][j]) return make_pair(i, j);
		}
	}
}

/*
bool is_in_closed_list(vector<vector<int>> arr) {
	if (std::find(closed_list.begin(), closed_list.end(), arr) != closed_list.end())
		return true;
	return false;
}
*/

void bfs(vector<vector<int>> initial) {
	closed_list.clear();
	queue <BB> q;
	int prev_x, prev_y, now_x, now_y, next_x, next_y, i, len, cnt=0, m=-1;
	pair<int, int> zero = find_initial_zero(initial);
	BB init(zero.first, zero.second, -1, -1, 0, initial);
	q.push(init);
	while (!q.empty()) {
		m = max(m, int(q.size()));
		cnt++;
		BB now = q.front();
		q.pop();

		now_x = now.now_x;
		now_y = now.now_y;
		prev_x = now.prev_x;
		prev_y = now.prev_y;
		len = now.len;
		cout << len<<" ";
		vector<vector<int>> now_arr = now.arr;
		//cout << "now_x: " << now_x << ", now_y: " << now_y << endl;
		//print_arr(now_arr);
		if (now_arr == goal) {
			cout << "< bfs >" << endl;
			cout << "len: " << len<<endl;
			cout << "\n cnt: " << cnt << ", max open len: " << m << endl;
			print_arr(now_arr);
			return;
		}

		for (i = 0; i < 4; i++) {
			next_x = now_x + dxy[i][0];
			next_y = now_y + dxy[i][1];

			if (next_x < 0 || next_x > 2 || next_y < 0 || next_y > 2
				|| (next_x ==prev_x && next_y == prev_y)) continue;
			vector<vector<int>> next_arr = swap_arr(now_arr, now_x, now_y, next_x, next_y);
			
			/*
			forq next(next_x, next_y, now_x, now_y, len+1, next_arr);
			q.push(next);
			*/
			if (std::find(closed_list.begin(), closed_list.end(), next_arr) != closed_list.end()) continue;
			BB next(next_x, next_y, now_x, now_y, len + 1, next_arr);
			closed_list.push_back(next_arr);
			q.push(next);
			
		}
	}
}


void recursion_dfs(int now_x, int now_y, int len, vector<vector<int>> now_arr) {
	if (now_arr == goal) {
		cout << "< dfs >" << endl;
		cout << "len: " << len << endl;
		cout << "\n cnt: " << dfs_cnt << ", max open len: " << dfs_m << endl;
		print_arr(now_arr);
		return;
	}
	dfs_cnt++;
	int i, next_x, next_y;
	for (i = 0; i < 4; i++) {
		next_x = now_x + dxy[i][0];
		next_y = now_y + dxy[i][1];

		if (next_x < 0 || next_x > 2 || next_y < 0 || next_y > 2) continue;

		vector<vector<int>> next_arr = swap_arr(now_arr, now_x, now_y, next_x, next_y);
		if (std::find(closed_list.begin(), closed_list.end(), next_arr) != closed_list.end()) continue;
		closed_list.push_back(next_arr);
		recursion_dfs(next_x, next_y, len + 1, next_arr);
	}
}

int dfs(vector<vector<int>> initial) {
	closed_list.clear();
	stack <forq> st;
	int now_x, now_y, next_x, next_y, i, len, cnt=0, m=-1;
	pair<int, int> zero = find_initial_zero(initial);
	forq init(zero.first, zero.second, 0, initial);
	st.push(init);
	while (!st.empty()) {
		m = max(m, int(st.size()));
		cnt++;
		forq now = st.top();
		st.pop();

		now_x = now.now_x;
		now_y = now.now_y;

		len = now.len;
		vector<vector<int>> now_arr = now.arr;
		//cout << "now_x: " << now_x << ", now_y: " << now_y << endl;
		//print_arr(now_arr);
		if (now_arr == goal) {
			cout << "< dfs >" << endl;
			cout << "len: " << len << endl;
			cout << "\n cnt: " << cnt << ", max open len: " << m << endl;
			print_arr(now_arr);
			break;
		}

		for (i = 0; i < 4; i++) {
			next_x = now_x + dxy[i][0];
			next_y = now_y + dxy[i][1];

			if (next_x < 0 || next_x > 2 || next_y < 0 || next_y > 2 ) continue;

			vector<vector<int>> next_arr = swap_arr(now_arr, now_x, now_y, next_x, next_y);
			if (std::find(closed_list.begin(), closed_list.end(), next_arr) != closed_list.end()) continue;
			forq next(next_x, next_y, len + 1, next_arr);
			closed_list.push_back(next_arr);
			st.push(next);
		}
	}

	return cnt;
}


struct comp_for_priorityQ {
	bool operator() (pqNode a, pqNode b) {
		return a.oop > b.oop;
	}
};

int out_of_place(vector<vector<int>> arr) {
	int cnt = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (goal[i][j] != arr[i][j]) cnt++;
		}
	}
	return cnt;
}

int manhattan(vector<vector<int>> arr) {
	int sum = 0;
	int num;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (arr[i][j] == 0) continue;
			num = arr[i][j]-1;
			sum += (abs((num / 3) - i) + abs((num % 3) - j));
		}
	}
	return sum;
}
void Heuristic_by_num(vector<vector<int>> initial) {
	closed_list.clear();
	priority_queue < pqNode, vector <pqNode >, comp_for_priorityQ> q;
	int now_x, now_y, next_x, next_y, i, len, cnt=0, m=-1;
	pair<int, int> zero = find_initial_zero(initial);

	pqNode init(zero.first, zero.second, out_of_place(initial), 0, initial);
	q.push(init);
	while (!q.empty()) {
		m = max(m, int(q.size()));
		cnt++;
		pqNode now = q.top();
		q.pop();

		now_x = now.now_x;
		now_y = now.now_y;
		len = now.len;
		vector<vector<int>> now_arr = now.arr;
		//cout << "now_x: " << now_x << ", now_y: " << now_y << endl;
		//print_arr(now_arr);
		if (now_arr == goal) {
			cout << "< Heuristic_by_num >" << endl;
			cout << "len: " << len << endl;
			cout << "\n cnt: " << cnt << ", max open len: " << m << endl;
			print_arr(now_arr);
			return;
		}

		for (i = 0; i < 4; i++) {
			next_x = now_x + dxy[i][0];
			next_y = now_y + dxy[i][1];

			if (next_x < 0 || next_x > 2 || next_y < 0 || next_y > 2) continue;
			vector<vector<int>> next_arr = swap_arr(now_arr, now_x, now_y, next_x, next_y);

			if (std::find(closed_list.begin(), closed_list.end(), next_arr) != closed_list.end()) continue;
			closed_list.push_back(next_arr);
			pqNode next(next_x, next_y, out_of_place(next_arr), len + 1, next_arr);
			q.push(next);

		}
	}
}

void Heuristic_by_distance(vector<vector<int>> initial) {
	closed_list.clear();
	priority_queue < pqNode, vector <pqNode >, comp_for_priorityQ > q;
	int now_x, now_y, next_x, next_y, i, len, cnt=0, m=-1;
	pair<int, int> zero = find_initial_zero(initial);
	
	pqNode init(zero.first, zero.second, manhattan(initial), 0, initial);
	q.push(init);
	while (!q.empty()) {
		m = max(m, int(q.size()));
		cnt++;
		pqNode now = q.top();
		q.pop();

		now_x = now.now_x;
		now_y = now.now_y;
		len = now.len;
		vector<vector<int>> now_arr = now.arr;
		//cout << "now_x: " << now_x << ", now_y: " << now_y << endl;
		//print_arr(now_arr);
		if (now_arr == goal) {
			cout << "< Heuristic_by_distance >" << endl;
			cout << "len: " << len << endl;
			cout << "\n cnt: " << cnt << ", max open len: " << m << endl;
			print_arr(now_arr);
			return;
		}

		for (i = 0; i < 4; i++) {
			next_x = now_x + dxy[i][0];
			next_y = now_y + dxy[i][1];

			if (next_x < 0 || next_x > 2 || next_y < 0 || next_y > 2 ) continue;
			vector<vector<int>> next_arr = swap_arr(now_arr, now_x, now_y, next_x, next_y);
			
			if (std::find(closed_list.begin(), closed_list.end(), next_arr) != closed_list.end()) continue;
			closed_list.push_back(next_arr);
			pqNode next(next_x, next_y, manhattan(next_arr), len + 1, next_arr);
			q.push(next);

		}
	}

}

void sol(vector<vector<int>> a) {
	
	//bfs(a);
	cout << "Heuristic_by_num: " << endl;
	//Heuristic_by_num(a);
	cout << "Heuristic_by_distance: " << endl;
	Heuristic_by_distance(a);
}

int main() {
	vector<vector<int>> p1 = {
		{4,1,0},
		{6,3,2},
		{7,5,8} };
	vector<vector<int>> pp = {
		{4,1,0},
		{6,3,2},
		{7,5,8} };
	vector<vector<int>> p2 = {
		{4,3,1},
		{6,5,2},
		{0,7,8} };
	vector<vector<int>> p3 = {
		{3,1,0},
		{4,5,2},
		{6,7,8}, };
	vector<vector<int>> p4 = {
		{0,1,2},
		{3,4,5},
		{6,7,8} };

	//Heuristic_by_num(p4);
	//Heuristic_by_distance(p4);
	bfs(p4);
	//dfs(p1);
	//pair<int, int> zero = find_initial_zero(p4);
	//recursion_dfs(zero.first, zero.second, 0, p4);
	return 0;
}