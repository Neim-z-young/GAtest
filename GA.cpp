#include"GA.hpp"
#include<iostream>
#include<time.h>
#include<math.h>
GA::GA(float(*f)(float)) :length(CHROMOSOME_BITS), chromosomeNum(CHROMOSOME_NUMS),
generation(GENERATION_NUMS), evoluationCount(0),
population(POPULATION),
lowerBound(LOWER_BOUND), higherBound(HIGHER_BOUND),
crossoverProbability(0.4f), mutateProbability(0.3f),
selectProbability(0.3f)
{
	function = f;
	fitness = new float[population];
	children = new float[population];
	chromosomes = new float[chromosomeNum];
	memset(fitness, 0, population);
	memset(children, 0, population);
	memset(chromosomes, 0, chromosomeNum);
	srand((unsigned)time(NULL));  //初始化随机数种子
								  // for(int i=0; i < population; ++i){
								  //     fitness[i] = 0.0f;
								  //     children[i] = 0.0f;
								  // }
								  // for(int i=0; i<chromosomeNum; ++i){
								  //     chromosomes[i] = 0.0f;
								  // }

}

GA::~GA() {
	delete[] fitness;
	delete[] children;
	delete[] chromosomes;
//	_CrtDumpMemoryLeaks();
	fitness = nullptr;
	children = nullptr;
	chromosomes = nullptr;
}

int GA::partition(int low, int high) {
	float tempFitness = fitness[low];
	float tempChild = children[low];
	float pivotKey = fitness[low];
	while (low<high) {
		while (low<high&&fitness[high] <= pivotKey) --high;
		fitness[low] = fitness[high];
		children[low] = children[high];
		while (low<high&&fitness[low] >= pivotKey) ++low;
		fitness[high] = fitness[low];
		children[high] = children[low];
	}
	fitness[low] = tempFitness;
	children[low] = tempChild;
	return low;
}

void GA::initializeChromosomes() {
	for (int i = 0; i<chromosomeNum; ++i) {
		float initValue = random(lowerBound, higherBound);
		chromosomes[i] = initValue;
	}
}

void GA::sortChildren(int lower, int higher) {
	calculateFitness();
	//快排
	if (lower<higher) {
		int pivotloc = partition(lower, higher);
		sortChildren(lower, pivotloc - 1);
		sortChildren(pivotloc + 1, higher);
	}
	//冒泡排序
	/*float temp=0.0f, tempChild=0.0f;
	for (int i = 0; i < population; ++i) {
	for (int j = i + 1; j < population; ++j) {
	if (fitness[j] > fitness[i]) {
	temp = fitness[i];
	tempChild = children[i];
	fitness[i] = fitness[j];
	children[i] = children[j];
	fitness[j] = temp;
	children[j] = tempChild;
	}
	}
	}*/
}

char* GA::dynamic2Binary(float x) {
	char* biCode = new char[length + 1];
	biCode[length] = '\0';   //字符数组结尾加字符串结尾标志
	int parts = (int)pow(2.0f, (float)length);
	float delta = (higherBound - lowerBound) / (parts - 1);
	int xParts = (int)((x - lowerBound) / delta);
	int index = length;
	int quotient = xParts;
	while (quotient) {
		--index;
		biCode[index] = quotient % 2 + '0';
		quotient = quotient / 2;
	}
	while (index >0) {
		index--;
		biCode[index] = '0';
	}
	return biCode;
}

float GA::binary2Dynamic(char* biCode) {
	int parts = (int)pow(2.0f, (float)length);
	float delta = (higherBound - lowerBound) / (parts - 1);
	int xParts = 0;
	for (int i = 0; i<length; ++i) {
		xParts *= 2;
		xParts += (biCode[i] - '0');
	}
	return xParts*delta + lowerBound;
}

float* GA::crossover(float x1, float x2) {
	char* bi1 = dynamic2Binary(x1);
	char* bi2 = dynamic2Binary(x2);
	int index = randomInt(0, length);
	for (int i = index; i<length; ++i) {
		char temp = bi1[i];
		bi2[i] = bi1[i];
		bi1[i] = temp;
	}
	float* newX = new float[2];
	newX[0] = binary2Dynamic(bi1);
	newX[1] = binary2Dynamic(bi2);
	delete[] bi1;
	delete[] bi2;
	bi1 = nullptr;
	bi2 = nullptr;
	return newX;
}

float GA::select(float x1) {
	return x1;
}

float GA::mutate(float x1) {
	char* bi1 = dynamic2Binary(x1);
	int index = randomInt(0, length);
	if (bi1[index] == '0')
		bi1[index] = '1';
	else
		bi1[index] = '0';
	float x2 = binary2Dynamic(bi1);
	delete[] bi1;
	bi1 = nullptr;
	return x2;
}

int GA::evoluation() {

#if USING_ELITIST_STRATEGY==0
	for (int i = 0; i<population; ++i) {
		int randomIndex1 = randomInt(0, chromosomeNum);
		float probability = random(0.0f, 1.0f);
		if (probability >= 0 && probability<mutateProbability) {
			children[i] = mutate(chromosomes[randomIndex1]);
		}
		else if (probability >= mutateProbability&&
			probability<(mutateProbability + crossoverProbability)) {
			int randomIndex2 = randomInt(0, chromosomeNum);
			float* crossed = crossover(chromosomes[randomIndex1], chromosomes[randomIndex2]);
			children[i] = crossed[0];
			if (++i < population) {
				children[i] = crossed[1];      //++i后会越界
			}
			delete[] crossed;
			crossed = nullptr;
		}
		else {
			children[i] = select(chromosomes[randomIndex1]);
		}
	}
#elif USING_ELITIST_STRATEGY==1
	for(int i =0;i<ELITIST_RESERVED;++i){
		children[i] = chromosomes[i];
	}
	for (int i = ELITIST_RESERVED; i<population; ++i) {
		int randomIndex1 = randomInt(ELITIST_RESERVED, chromosomeNum);
		float probability = random(0.0f, 1.0f);
		if (probability >= 0 && probability<mutateProbability) {
			children[i] = mutate(chromosomes[randomIndex1]);
		}
		else if (probability >= mutateProbability&&
			probability<(mutateProbability + crossoverProbability)) {
			int randomIndex2 = randomInt(ELITIST_RESERVED, chromosomeNum);
			float* crossed = crossover(chromosomes[randomIndex1], chromosomes[randomIndex2]);
			children[i] = crossed[0];
			if (++i < population) {
				children[i] = crossed[1];      //++i后会越界
			}
			delete[] crossed;
			crossed = nullptr;
		}
		else {
			children[i] = select(chromosomes[randomIndex1]);
		}
	}
#endif
	evoluationCount++;
	return evoluationCount;
}

float GA::random(float low, float high) {
	return ((float)rand() / RAND_MAX)*(high - low) + low;
}

int GA::randomInt(int low, int high) {
	return rand() % (high - low) + low;
}

void GA::generateChromosomes() {
	sortChildren(0, population);
	for (int i = 0; i<population && i<chromosomeNum; ++i) {    //与或的问题
		chromosomes[i] = children[i];
	}
	if (population<chromosomeNum) {
		for (int i = population; i<chromosomeNum; ++i) {
			chromosomes[i] = chromosomes[population - 1];
		}
	}
}

void GA::calculateFitness() {
	for (int i = 0; i<population; ++i) {
		fitness[i] = function(children[i]);
	}
}

float GA::getMaxFitnessChromosome() {
	if (chromosomes != nullptr) {
		return chromosomes[0];
	}
	else
		return INFINITY;
}

float GA::getMaxFitness() {
	if (chromosomes != nullptr) {
		return function(chromosomes[0]);
	}
	else
		return INFINITY;
}

float b2D(char* biCode, int codeLength, float lowerBound, float higherBound) {
	int parts = (int)pow(2.0f, (float)codeLength);
	float delta = (higherBound - lowerBound) / (parts - 1);
	int xParts = 0;
	#if DEBUG_TEST==1
		std::cout <<"debug:"<< biCode << std::endl;
	#endif
	for (int i = 0; i<codeLength; ++i) {
		xParts *= 2;
		xParts += (biCode[i] - '0');
	}
	return xParts*delta + lowerBound;
}

char* d2B(float fValue, int codeLength, float lowerBound, float higherBound) {
	char* biCode = new char[codeLength + 1];
	biCode[codeLength] = '\0';   //字符数组结尾加字符串结尾标志
	int parts = (int)pow(2.0f, (float)codeLength);
	float delta = (higherBound - lowerBound) / (parts - 1);
	int xParts = (int)((fValue - lowerBound) / delta);
	int index = codeLength;
	int quotient = xParts;
	while (quotient) {
		--index;
		biCode[index] = quotient % 2 + '0';
		quotient = quotient / 2;
	}
	while (index >0) {
		index--;
		biCode[index] = '0';
	}
	return biCode;
}

char* encode(float* x, int nx, int xBits, float lowerBound, float higherBound) {
	char* code = new char[nx*xBits + 1];
	code[nx*xBits] = '\0';
	int index = 0;
	for (int i = 0; i<nx; ++i) {
		char *xCode = d2B(x[i], xBits, lowerBound, higherBound);
		for (int j = 0; j<xBits; ++j) {
			code[index] = xCode[j];
			++index;
		}
		delete[] xCode;
		xCode = nullptr;
	}
	return code;
}

float* decodeNX(char* code, int nx, int xBits, float lowerBound, float higherBound) {
	float* x = new float[nx];
	for (int i = 0; i<nx; ++i) {
		char* xCode = new char[xBits + 1];
		xCode[xBits] = '\0';
		int index = 0;
		for (int j = xBits*i; j<xBits*i + xBits; ++j) {
			xCode[index] = code[j];
			index++;
		}
	#if DEBUG_TEST==1
		std::cout << "dubug: " << xCode << std:: endl;
	#endif
		x[i] = b2D(xCode, xBits, lowerBound, higherBound);
		delete[] xCode;
		xCode = nullptr;
	}
	return x;
}

// float decodeX(char* code, int bits, float lowerBound, float higherBound){
// 	return b2D(code, bits, lowerBound, higherBound);
// }