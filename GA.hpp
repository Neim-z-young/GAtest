

#ifndef _GA_HPP_
#define _GA_HPP_

#define DEBUG_TEST 0 

// interface config
#define VARIABLE_NUMS 1          //������
#define VARIABLE_CODE_BITS 16    //���������ı���λ��

//configuration
#define POPULATION 100            //��Ⱥ��
#define CHROMOSOME_NUMS 80        //Ⱦɫ����
#define CHROMOSOME_BITS (VARIABLE_NUMS*VARIABLE_CODE_BITS)    //����Ⱦɫ��λ��
#define GENERATION_NUMS 50     //����
#define PRESION  0.0000001     
#define LOWER_BOUND 0.0f       //�����½�
#define HIGHER_BOUND 15.0f     //�����Ͻ�

#define ELITIST_RESERVED 20   //��Ӣ������
#define USING_ELITIST_STRATEGY 1  //1��ʾʹ�þ�Ӣ���ԣ�0Ϊ��ʹ��

class GA {
public:
	GA(float(*f)(float));  //��Ӧ�Ⱥ���
	~GA();
	void initializeChromosomes();
	//��������
	void sortChildren(int lower = 0, int higher = POPULATION);
	//��λ�������λ����
	char* dynamic2Binary(float x);
	float binary2Dynamic(char* biCode);
	//����ͬʱ�ı�����Ⱦɫ��
	float* crossover(float x1, float x2);
	float select(float x1);
	float mutate(float x1);
	int evoluation();
	float random(float low, float high);
	int randomInt(int low, int high);
	void generateChromosomes();
	//��Ⱥ��С��ʵ��Ⱦɫ��ɴ��С
	void calculateFitness();
	float getMaxFitnessChromosome();
	float getMaxFitness();

private:
	int partition(int low, int high);
	float(*function)(float);  // f(x) = log(1+x*x)
	int length;   // Ⱦɫ��λ��
	int chromosomeNum;   //ʵ��Ⱦɫ�����
	int evoluationCount;  //��������
	int population;   //��Ⱥ��С
	float* fitness;   //Ⱦɫ����Ӧ��
	float* children;   //�Ӵ�
	float* chromosomes; //ʵ�ʲ�����Ⱦɫ��,���Ӵ���ѡ����Ӧ�ȸߵ���ΪȾɫ��(����Ⱦɫ��)
	float lowerBound;    //�½�
	float higherBound;   //�Ͻ�
						 //����
	float crossoverProbability;
	float mutateProbability;
	float selectProbability;

public:
	int generation;   //����
};
//interface
float b2D(char* biCode, int codeLength=8, float lowerBound=LOWER_BOUND, float higherBound=HIGHER_BOUND);
//��λ�������λ����
char* d2B(float fValue,int codeLength=2*8, float lowerBound=LOWER_BOUND, float higherBound=HIGHER_BOUND);
char* encode(float* x, int nx=VARIABLE_NUMS, int xBits=VARIABLE_CODE_BITS, float lowerBound=LOWER_BOUND, float higherBound=HIGHER_BOUND);  //������ȡֵ��Χ��ͬ
float* decodeNX(char* code, int nx=VARIABLE_NUMS, int xBits=VARIABLE_CODE_BITS, float lowerBound=LOWER_BOUND, float higherBound=HIGHER_BOUND);  //������ȡֵ��Χ��ͬ
//float decodeX(char* code, int bits = VARIABLE_CODE_BITS*VARIABLE_NUMS, float lowerBound = LOWER_BOUND, float higherBound = HIGHER_BOUND);

#endif