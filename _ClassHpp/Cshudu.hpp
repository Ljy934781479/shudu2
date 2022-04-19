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
//数独的每一个格子
struct tagBox
{
	tagBox::tagBox(int _row, int _col, int _value);
	//跟另一个格子是否有关联.同行，列,宫
	bool isRel(tagBox* b);
	//给出可能是的数
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
	int gong = 999;//在第几个宫
	int countBeable = 9;
	int value = 0;//已经确定的值
	bool caice = false;//猜测占用
private:
};

class CSHUDU
{
public:
	CSHUDU(BYTE(*arry)[9]);
	~CSHUDU();
	//从参数容器中找出所有有关联的,找相关的所有                 
	set<tagBox*> getRelBox(tagBox* b,bool one);
	set<tagBox*> gusRelBox(tagBox* b);
	//找出能直接确定的,每行或者列
	int OnlyNum();
	//宫的检查
	int gongAlg();
	//初始化
	int init();
	//
	int work();
	//完全确认准确的时候更新数据.
	bool upData(int _value, tagBox* b);
	//算法，解析
	int parse();
	//循环递归求解,并且返回剩下的没找到值的(考虑到可能会猜错需要重置麻烦,所以最后一个参数不能用引用)
	bool guessAlg(tagBox* first, int no = -1);
	//更新debug数组，标记位.
	bool setBitInfo(tagBox* b,int val);
	int getGid(int& row, int& col);
	//
	int getBit(int arr[3], int count,int val, int& arrId, int& off);
	bool resetBit(tagBox* b);
private:
	vector<tagBox*> _alBox;
	BYTE (*dbgArry_)[9];
	//用一个bit来记录每行，列，宫的数字是否存在。  81个bit，10个字节，三个整型
	int checkRow_[3];
	//列
	int checkCol_[3];
	//宫计数
	int checkGong_[3];
	int _countFalse;
};

