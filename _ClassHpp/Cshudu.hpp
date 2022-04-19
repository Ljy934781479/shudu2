#pragma once
#include<stack>
#include<set>
#include<vector>
#include<map>
#include<algorithm>

#define MAXFALSE 7000000
using namespace std;

template <class T, class T1>
void mapToVec(T& m, T1& vec)
{
	for (auto it = m.begin(); it != m.end(); it++)
		for (tagBox*b : it->second)
			vec.push_back(b);
}
//������ÿһ������
struct tagBox
{
	tagBox::tagBox(int _row, int _col, int _value);
	//����һ�������Ƿ��й���.ͬ�У���,��
	bool isRel(tagBox* b);
	//���������ǵ���
	vector<int> getBeable()
	{
		vector<int> result;
		for (int i = 1; i < 10; i++)
			if (r[i])
				result.push_back(i);
		countBeable = result.size();
		return result;
	}
	bool operator < (const tagBox& x)
	{
		return countBeable < x.countBeable;
	}
	int row;
	int col;
	bool r[10];
	int gong = 999;//�ڵڼ�����
	int countBeable = 9;
	int value = 0;//�Ѿ�ȷ����ֵ
	bool caice = false;//�²�ռ��
private:
};

class CSHUDU
{
public:
	CSHUDU(BYTE(*arry)[9]);
	~CSHUDU();
	//�Ӳ����������ҳ������й�����,����ص�����                 
	set<tagBox*> getRelBox(tagBox* b);
	//����������²�Ļ���²²�ռ��ֵ,���������̫���ظ���ظ���
	set<tagBox*> gusRelBox(tagBox* b);
	//�ҳ���ֱ��ȷ����,ÿ�л�����
	int OnlyNum();
	//���ļ��
	int gongAlg();
	//��ʼ��
	int init();
	//
	int work();
	//��ȫȷ��׼ȷ��ʱ���������.
	bool upData(int _value, tagBox* b);
	//�㷨������
	int parse();
	//ѭ���ݹ����,���ҷ���ʣ�µ�û�ҵ�ֵ��(���ǵ����ܻ�´���Ҫ�����鷳,�������һ����������������)
	bool guessAlg(tagBox* first, int no = -1);
	//����debug���飬���λ.
	bool setBitInfo(tagBox* b,int val);
	int getGid(int& row, int& col);
	//
	int getBit(int arr[3], int count,int val, int& arrId, int& off);
	bool resetBit(tagBox* b);
private:
	vector<tagBox*> _alBox;
	BYTE (*dbgArry_)[9];
	//��һ��bit����¼ÿ�У��У����������Ƿ���ڡ�  81��bit��10���ֽڣ���������
	int checkRow_[3];
	//��
	int checkCol_[3];
	//������
	int checkGong_[3];
	int _countFalse;
};

