#pragma once
#include<stack>
#include<set>
#include<vector>
#include<map>
#include<algorithm>

#define MAXFALSE 70000000
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
	tagBox::tagBox(int _row, int _col, BYTE* _p);
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
	BYTE* ptr_;//绑定一个地址
	tagBox* rowChild = NULL;
	tagBox* colChild = NULL;
private:
};

class CSHUDU
{
public:
	CSHUDU(BYTE(*arry)[9]);
	~CSHUDU();
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
	//深度优先
	bool dfs(tagBox* p, int no = -1);
	//广度优先
	bool bfs(tagBox* p, int no = -1);
	//创建解空间树
	bool creatSST(vector<tagBox*>& vUnkow);
	//猜一个格子
	bool gusSstBox(tagBox*p);
	//更新debug数组，标记位.
	bool setBitInfo(tagBox* b,int val);
	int getGid(int& row, int& col);
	//
	int getBit(int arr[3], int count,int val, int& arrId, int& off);
	bool resetBit(tagBox* b);
	int lineCell(tagBox* a);
	void getRowCell(tagBox* a, set<tagBox*>& s, bool second = false);
	void getColCell(tagBox* a, set<tagBox*>& s, bool second = false);
	void getGongCell(tagBox* a, set<tagBox*>& s, bool second = false);
	void getAlCell(tagBox* a, set<tagBox*>& s, bool second = false);
private:
	int bCheckLine_;
	vector<tagBox*> _alBox;
	BYTE (*dbgArry_)[9];
	//用一个bit来记录每行，列，宫的数字是否存在。  81个bit，10个字节，三个整型
	int checkRow_[3];
	//列
	int checkCol_[3];
	//宫
	int checkGong_[3];
	//对角线
	int checkLine_;
	int _countFalse;
};

