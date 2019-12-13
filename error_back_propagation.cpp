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
vector<double> dv(3), dw1(3), dw2(3);

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
	
	for (int i = 0; i < 3; i++) {
	

		//dw1[i] = -input1[idx]*h[i]*(1-h[i])*
		
		dv[i] = -(target[idx] - output) * output * (1 - output) * h[i];
		printf("i: %d, dv: %f\n", i, dv[i]);

	}
}

int main() {	
	
	fun(0);

	return 0;
}