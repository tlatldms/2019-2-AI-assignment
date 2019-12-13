#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

//input&target cases 
vector<double> input1 = { 1,1,0,0 };
vector<double> input2 = { 1,0,1,0 };
vector<double> target = { 0,1,1,0 };
vector<double> weight1 = { -0.089, 0.028, 0.092 };
vector<double> weight2 = { 0.098, -0.07, -0.01 };
vector<double> v = {0.056, 0.067, 0.016};
vector<double> vsum(3), w1sum(3), w2sum(3);
vector<vector<double>> dv(4, vector<double> (3)), dw1(4, vector<double>(3)), dw2(4, vector<double>(3));

//int i: 몇번째 케이스인지
void fun(int idx) {

	double net1_1 = input1[idx] * weight1[0]+ input2[idx] * weight1[1]
		+ 1 * weight1[2];
	double net1_2 = input1[idx] * weight2[0] + input2[idx] * weight2[1]
		+ 1 * weight2[2];

	vector<double> h(3);
	
	h[0] = 1 / (1 + exp(-net1_1));
	h[1] = 1 / (1 + exp(-net1_2));
	h[2] = 1;

	double net2 = h[0]*v[0] + h[1]*v[1] + h[2]*v[2];

	double output = 1 / (1 + exp(-net2));

	printf("net1_1: %f, net1_2: %f, h1: %f, h2: %f, net2: %f, output: %f\n"
		, net1_1, net1_2, h[0], h[1], net2, output);
	
	cout << "now idx(input case): " << idx+1 << endl;
	for (int i = 0; i < 3; i++) {
	
		printf("i: %d\n", i);
		dw1[idx][i] = -input1[idx] * h[i] * (1 - h[i]) * (v[i] * (target[idx] - output) * output * (1 - output));
		dw2[idx][i] = -input2[idx] * h[i] * (1 - h[i]) * (v[i] * (target[idx] - output) * output * (1 - output));
		printf("dw1: %f, dw2: %f\n", dw1[idx][i], dw2[idx][i]);

		dv[idx][i] = -(target[idx] - output) * output * (1 - output) * h[i];
		printf("dv: %f\n", dv[idx][i]);
	}


}

int main() {	
	for (int i = 0; i < 4; i++) {
		fun(i);
		cout << "\n\n\n";
	}
	for (int i = 0; i < 3; i++) { //i: v number
		for (int j = 0; j < 4; j++) { //j: case number
			vsum[i] += dv[j][i];
			w1sum[i] += dw1[j][i];
			w2sum[i] += dw2[j][i];
		}

		printf("i: %d, vsum: %f, w1sum: %f, w2sum: %f \n", i, vsum[i], w1sum[i], w2sum[i]);
	}
	
	return 0;
}