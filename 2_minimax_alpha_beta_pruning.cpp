#include <iostream>
#include <vector>
#include <algorithm>

#define WIN 1000;
#define LOSE -1000;
#define DRAW 0;

using namespace std;
int N; //3: 3x3? 4: 4x4?
int M; //1: minimax 2: alpha-beta pruning
int R; //100 / 10000 monte carlo repetition
typedef struct _Node {
	int index; // 0~15
	int score;
	char state; // A, B, E(empty)
} Node;

vector<Node> board;

vector<int> get_empty_nums();
bool is_end();
bool is_full();
Node return_best_node(char player);
int return_score(char player, int depth);
int mm_min_fun(char now_player, int depth);
int mm_max_fun(char now_player, int depth);
int ab_min_fun(int alpha, int beta, int depth, char now_player);
int ab_max_fun(int alpha, int beta, int depth, char now_player);
bool is_win(char player);

Node return_best_node(char player) {
	Node best_node{ 0, -1, 'E' };
	int mins;
	vector<int> empty_nums = get_empty_nums();
	//cout << "in return_best_node: empty nums size: " << empty_nums.size() << endl;
	for (int i = 0; i < empty_nums.size(); i++) {
		board[empty_nums[i]].state = player; //현재 플레이어
		if (M == 2) mins = ab_min_fun(-1000, 1000, 0, player);
		else if (M == 1) mins = mm_min_fun(player, 0);
		//cout << "num: " << empty_nums[i] << ", max: " << maxs << endl;
		board[empty_nums[i]].score = mins;
		board[empty_nums[i]].state = 'E'; // return to empty state
	}
	//cout << "in updating" << endl;
	for (int i = 0; i < empty_nums.size(); i++) {
		if (board[empty_nums[i]].score >= best_node.score) {
			best_node = board[empty_nums[i]]; //update to best node
		}
	}

	if (best_node.score == -1) {
		best_node = board[empty_nums.back()];
	}

	best_node.state = player;
	return best_node;
}

int return_score(char player, int depth) {
	//cout << "depth: " << depth << endl;
	char enemy;
	if (player == 'A') enemy = 'B';
	else enemy = 'A';

	if (is_win(player))
		{ return (1000 - depth*10); }
	else if (is_win(enemy))
		{ return (-1000 + depth*10); }
	else
		return 0;
}

bool is_full() {

	if (!get_empty_nums().size()) {
		//cout << "FULL! " << endl;
		return true;
	}
	else return false;
}

bool is_end() {
	if (is_full() || is_win('A') || is_win('B')) return true;
	else return false;
}

int mm_min_fun(char now_player, int depth) {
	int x = WIN;
	if (is_end()) {
		return return_score(now_player, depth);
	}
	char enemy;
	if (now_player == 'A') enemy = 'B';
	else enemy = 'A';

	vector<int> empty_nums = get_empty_nums();

	//cout << "in min now emptysize: " << empty_nums.size() << endl;

	for (int i = 0; i < empty_nums.size(); i++) {
		board[empty_nums[i]].state = enemy; //min val for enemy
		x = min(x, mm_max_fun(now_player, depth+1));
		board[empty_nums[i]].state = 'E'; //restore
	}
	return x;
}


int mm_max_fun(char now_player, int depth) {
	int x = LOSE;
	if (is_end()) {
		return return_score(now_player, depth);
	}

	vector<int> empty_nums = get_empty_nums();

	//cout << "in min now emptysize: " << empty_nums.size() << endl;

	for (int i = 0; i < empty_nums.size(); i++) {
		board[empty_nums[i]].state = now_player; //min val for enemy
		x = max(x, mm_min_fun(now_player, depth + 1 ));
		board[empty_nums[i]].state = 'E'; //restore
	}
	return x;
}
//min value for enemy
int ab_min_fun(int alpha, int beta, int depth, char now_player) {
	int x = WIN;
	if (is_end()) {
		return return_score(now_player, depth);
	}
	char enemy;
	if (now_player == 'A') enemy = 'B';
	else enemy = 'A';

	vector<int> empty_nums = get_empty_nums();

	//cout << "in min now emptysize: " << empty_nums.size() << endl;

	for (int i = 0; i < empty_nums.size(); i++) {
		board[empty_nums[i]].state = enemy; //min val for enemy
		x = min(x, ab_max_fun(alpha, beta, depth + 1, now_player));
		board[empty_nums[i]].state = 'E'; //restore
		if (x <= alpha) return x;
		beta = min(beta, x);
	}
	return x;
}

//max val for now player
int ab_max_fun(int alpha, int beta, int depth, char now_player) {
	int x = LOSE;
	if (is_end()) {
		return return_score(now_player, depth);
	}
	vector<int> empty_nums = get_empty_nums();
	//cout << "in max now emptysize: " << empty_nums.size() << endl;

	for (int i = 0; i < empty_nums.size(); i++) {
		board[empty_nums[i]].state = now_player; //max val for now player
		x = max(x, ab_min_fun(alpha, beta, depth + 1, now_player));
		board[empty_nums[i]].state = 'E'; //restore
		if (x >= beta) return x;
		alpha = max(alpha, x);
	}
	return x;
}


bool is_win(char player) {
	int i, j;
	if (N == 4) {
		//가로
		if ((board[0].state == player && board[0].state == board[1].state && board[1].state == board[2].state && board[2].state == board[3].state) ||
			(board[4].state == player && board[4].state == board[5].state && board[5].state == board[6].state && board[6].state == board[7].state) ||
			(board[8].state == player && board[8].state == board[9].state && board[9].state == board[10].state && board[10].state == board[11].state) ||
			(board[12].state == player && board[12].state == board[13].state && board[13].state == board[14].state && board[14].state == board[15].state) ||
			//세로
			(board[0].state == player && board[0].state == board[4].state && board[4].state == board[8].state && board[8].state == board[12].state) ||
			(board[1].state == player && board[1].state == board[5].state && board[5].state == board[9].state && board[9].state == board[13].state) ||
			(board[2].state == player && board[2].state == board[6].state && board[6].state == board[10].state && board[10].state == board[14].state) ||
			(board[3].state == player && board[3].state == board[7].state && board[7].state == board[11].state && board[11].state == board[15].state) ||
			//대각선
			(board[0].state == player && board[0].state == board[5].state && board[5].state == board[10].state && board[10].state == board[15].state) ||
			(board[3].state == player && board[3].state == board[6].state && board[6].state == board[9].state && board[9].state == board[12].state)
			) {
			//cout << "player " << player << "win! " << endl;
			return true;
		} else return false;
	}
	else if (N == 3) {
		// 가로
		if ((board[0].state == player && board[0].state == board[1].state && board[1].state == board[2].state) ||
			(board[3].state == player && board[3].state == board[4].state && board[4].state == board[5].state) ||
			(board[6].state == player && board[6].state == board[7].state && board[7].state == board[8].state) ||
			
			//세로
			(board[0].state == player && board[0].state == board[3].state && board[3].state == board[6].state) ||
			(board[1].state == player && board[1].state == board[4].state && board[4].state == board[7].state) ||
			(board[2].state == player && board[2].state == board[5].state && board[5].state == board[8].state) ||

			//대각선
			(board[0].state == player && board[0].state == board[4].state && board[4].state == board[8].state) ||
			(board[2].state == player && board[2].state == board[4].state && board[4].state == board[6].state)
			) {
			//cout << "player " << player << "win! " << endl;
			return true;
		}
		else return false;
	}
	
}

vector<int> get_empty_nums() {
	vector<int> ret;
	for (int i = 0; i < N*N; i++) {
		if (board[i].state == 'E') ret.push_back(i);
	}
	return ret;
}

void init_board() {
	for (int i = 0; i < N*N; i++) {
		board[i].index = i;
		board[i].score = 0;
		board[i].state = 'E';
	}
}


int monte_carlo(char now_player, int depth, char now_turn) {
	char next_player;
	if (now_turn == 'A') next_player = 'B';
	else if (now_turn == 'B') next_player = 'A';
	int x = LOSE;
	if (is_end()) {
		return return_score(now_player, depth);
	}
	vector<int> empty_nums = get_empty_nums();

	for (int i = 0; i < empty_nums.size(); i++) {
		board[empty_nums[i]].state = now_player; //max val for now player
		x = max(x, monte_carlo(now_player,depth+1, next_player));
		int a = rand() % 2;
		if (a == 0) 
		board[empty_nums[i]].state = 'E'; //restore
	}
}
int main() {
	cout << "3x3이면 3. 4x4면 4를 입력" << endl;
	cin >> N;
	cout << "minimax 알고리즘이면 1, alpha-beta pruning이면 2를 입력, monte carlo면 3을 입력" << endl;
	cin >> M;
	if (M == 3) {
		cout << "반복횟수 선택: 100 / 10000 중 입력" << endl;
		cin >> R;
	}
	board.resize(N * N);
	init_board();

	/* //can add state manually by: board[index].state = 'player';
	board[5].state = 'A';
	board[6].state = 'B';
	board[9].state = 'A';
	board[10].state = 'B';
	board[14].state = 'B';
	*/
	if (M == 1 || M == 2) {
		Node best = return_best_node('A');
		cout << "index of best node: " << best.index << endl;
	}
	else if (M == 3) {
		cout << "in monte carlo: " << endl;
		//monte_carlo('A');
	}
	return 0;
}