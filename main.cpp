#include"GA.hpp"
#include<iostream>
#include<fstream>
#include<cmath>
#define PI 3.1415926
using namespace std;
// f(x) = log(1+x*x)  ÊÊÓ¦¶Èº¯Êý
float func(float x);
float rastriginFunc(float x);
float func2(float x);

int main() {
	ofstream outfile;
	outfile.open("data.txt", ios::out);
	int gen = 0;
	float fitness = 0.0f;
	outfile<<gen<<"   "<<fitness<<'\n';


	GA gaTest(func);
	//_CrtDumpMemoryLeaks();
	gaTest.initializeChromosomes();
	while (gaTest.generation>=gaTest.evoluation()) {
	//	gaTest.calculateFitness();
		gaTest.sortChildren();
		gaTest.generateChromosomes();

		++gen;
		fitness = gaTest.getMaxFitness();
		outfile<<gen<<"   "<<fitness<<'\n';
	}

	outfile.close();

	float MaxX = gaTest.getMaxFitnessChromosome();
	// float MaxX = -0.707591;
	cout << "MaxX:" << MaxX << endl;
	char* codes = d2B(MaxX, 16);
	cout << "codes: " << codes << endl;
	float* x = decodeNX(codes);
	for(int i = 0; i<VARIABLE_NUMS;++i){
		cout <<"x"<<i<<": "<< x[i] << " ";
	}
	cout<<endl;
	cout << "MaxFitness: " << gaTest.getMaxFitness() << endl;
	//	_CrtDumpMemoryLeaks();
	return 0;
}

float func(float x) {
	return log(1.0f + x*x);
}

float rastriginFunc(float x) {
	char* code = d2B(x, CHROMOSOME_BITS, LOWER_BOUND, HIGHER_BOUND);
	float* nX = decodeNX(code, VARIABLE_NUMS, VARIABLE_CODE_BITS, LOWER_BOUND, HIGHER_BOUND);
//	std::cout << "debug:" << nX[0] << " " << nX[1] << std::endl;
	float y = 20.0f + nX[0] * nX[0] + nX[1] * nX[1] - 10.0*(cos(2.0f*PI*nX[0]) + cos(2.0f*PI*nX[1]));
	delete[]code;
	code = nullptr;
	delete[]nX;
	nX = nullptr;
	return y;
}

float func2(float x) {
	return x + 10 * sin(5 * x) + 7 * cos(4 * x);
}