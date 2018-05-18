

#ifndef _GA_HPP_
#define _GA_HPP_

#define DEBUG_TEST 0 

// interface config
#define VARIABLE_NUMS 1          //变量数
#define VARIABLE_CODE_BITS 16    //单个变量的编码位数

//configuration
#define POPULATION 100            //种群数
#define CHROMOSOME_NUMS 80        //染色体数
#define CHROMOSOME_BITS (VARIABLE_NUMS*VARIABLE_CODE_BITS)    //单个染色体位数
#define GENERATION_NUMS 50     //代数
#define PRESION  0.0000001     
#define LOWER_BOUND 0.0f       //变量下界
#define HIGHER_BOUND 15.0f     //变量上界

#define ELITIST_RESERVED 20   //精英保留数
#define USING_ELITIST_STRATEGY 1  //1表示使用精英策略，0为不使用

class GA {
public:
	GA(float(*f)(float));  //适应度函数
	~GA();
	void initializeChromosomes();
	//逆序排列
	void sortChildren(int lower = 0, int higher = POPULATION);
	//高位在数组低位索引
	char* dynamic2Binary(float x);
	float binary2Dynamic(char* biCode);
	//交叉同时改变两个染色体
	float* crossover(float x1, float x2);
	float select(float x1);
	float mutate(float x1);
	int evoluation();
	float random(float low, float high);
	int randomInt(int low, int high);
	void generateChromosomes();
	//种群大小与实际染色体可大可小
	void calculateFitness();
	float getMaxFitnessChromosome();
	float getMaxFitness();

private:
	int partition(int low, int high);
	float(*function)(float);  // f(x) = log(1+x*x)
	int length;   // 染色体位数
	int chromosomeNum;   //实际染色体个数
	int evoluationCount;  //进化计数
	int population;   //种群大小
	float* fitness;   //染色体适应度
	float* children;   //子代
	float* chromosomes; //实际产生的染色体,从子代中选择适应度高的作为染色体(父代染色体)
	float lowerBound;    //下界
	float higherBound;   //上界
						 //概率
	float crossoverProbability;
	float mutateProbability;
	float selectProbability;

public:
	int generation;   //代数
};
//interface
float b2D(char* biCode, int codeLength=8, float lowerBound=LOWER_BOUND, float higherBound=HIGHER_BOUND);
//高位在数组低位索引
char* d2B(float fValue,int codeLength=2*8, float lowerBound=LOWER_BOUND, float higherBound=HIGHER_BOUND);
char* encode(float* x, int nx=VARIABLE_NUMS, int xBits=VARIABLE_CODE_BITS, float lowerBound=LOWER_BOUND, float higherBound=HIGHER_BOUND);  //各变量取值范围相同
float* decodeNX(char* code, int nx=VARIABLE_NUMS, int xBits=VARIABLE_CODE_BITS, float lowerBound=LOWER_BOUND, float higherBound=HIGHER_BOUND);  //各变量取值范围相同
//float decodeX(char* code, int bits = VARIABLE_CODE_BITS*VARIABLE_NUMS, float lowerBound = LOWER_BOUND, float higherBound = HIGHER_BOUND);

#endif