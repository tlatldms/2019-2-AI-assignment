#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;
#define RUNNINGRATE 0.5
//input&target cases 
vector<vector<double>> input = { {1,1,1}, { 1,0,1} , {0,1,1}, {0,0,1} };
vector<double> target = { 0,1, 1,0 };
vector<double> weight1 = { -0.089, 0.028, 0.092 };
vector<double> weight2 = { 0.098, -0.07, -0.01 };
vector<double> v = {0.056, 0.067, 0.016};
vector<double> output(4);
vector<vector<double>> dv(4, vector<double> (3)), dw1(4, vector<double>(3)), dw2(4, vector<double>(3));
vector<vector<double>> h(4, vector<double> (3));
int cnt;

//int i: 몇번째 케이스인지
void fun(int case_num) {

	double net1_1 = input[case_num][0] * weight1[0]+ input[case_num][1] * weight1[1]
		+ 1 * weight1[2];
	double net1_2 = input[case_num][0] * weight2[0] + input[case_num][1] * weight2[1]
		+ 1 * weight2[2];

	
	
	h[case_num][0] = 1 / (1 + exp(-net1_1));
	h[case_num][1] = 1 / (1 + exp(-net1_2));
	h[case_num][2] = 1;
	double net2 = h[case_num][0]*v[0] + h[case_num][1]*v[1] + h[case_num][2]*v[2];

	output[case_num] = 1 / (1 + exp(-net2));

	printf("case_num: %d, output: %f\n" ,case_num,output[case_num]);
	//printf("net1_1: %f, net1_2: %f, h1: %f, h2: %f, net2: %f, output: %f\n"
	//	, net1_1, net1_2, h[case_num][0], h[case_num][1], net2, output[case_num]);
	
	//cout << "now case_num(input case): " << case_num+1 << endl;
	for (int i = 0; i < 3; i++) {

		//printf("i: %d\n", i);
		dw1[case_num][i] = -input[case_num][i] * h[case_num][0] * (1 - h[case_num][0]) * (v[0] * (target[case_num] - output[case_num]) * output[case_num] * (1 - output[case_num]));
		dw2[case_num][i] = -input[case_num][i] * h[case_num][1] * (1 - h[case_num][1]) * (v[1] * (target[case_num] - output[case_num]) * output[case_num] * (1 - output[case_num]));
		//printf("dw1: %f, dw2: %f\n", dw1[case_num][i], dw2[case_num][i]);
	}

	for (int j=0; j<3; j++) {
		dv[case_num][j] = -(target[case_num] - output[case_num]) * output[case_num] * (1 - output[case_num]) * h[case_num][j];
		//printf("dv: %f\n", dv[case_num][j]);
	}
}

void run() {
	cout <<"------- " << cnt++ << "번째 --------" <<endl;
	vector<double> vsum(3,0), w1sum(3,0), w2sum(3,0);
	//printf("weight1-1: %f, 1-2: %f, 1-3: %f, 2-1: %f, 2-2: %f, 2-3: %f\n", weight1[0], weight1[1], weight1[2], weight2[0], weight2[1], weight2[2]);
	
	for (int i = 0; i < 4; i++) {
		
		fun(i);
		cout << "\n";
	}

	for (int i = 0; i < 3; i++) { //i: index number
		for (int j = 0; j < 4; j++) { //j: case_num
			vsum[i] += dv[j][i];
			w1sum[i] += dw1[j][i];
			w2sum[i] += dw2[j][i];
		}
		//printf("i: %d, vsum: %f, w1sum: %f, w2sum: %f \n", i, vsum[i], w1sum[i], w2sum[i]);
	}

	for (int i = 0; i < 3; i++) {
		v[i] = v[i] - (RUNNINGRATE * vsum[i]);
		weight1[i] = weight1[i] - (RUNNINGRATE * w1sum[i]);
		weight2[i] = weight2[i] - (RUNNINGRATE * w2sum[i]);

	//	printf("i: %d, v: %f, w1: %f, w2: %f \n", i, v[i], weight1[i], weight2[i]);
	}

}

int main() {	
	for (int i=0; i<40000; i++) run();
	return 0;
}