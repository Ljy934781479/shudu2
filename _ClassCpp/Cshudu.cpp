
#include <pch.h>
#include<map>
#include<queue>
#include "../_ClassHpp/Cshudu.hpp"
#include <wtypes.h>

bool cmp1(tagBox* a, tagBox* b)
{
	return a->countBeable < b->countBeable;
}

tagBox::tagBox(int _row, int _col, int _value)
{
	row = _row;
	col = _col;
	value = _value;
	//求出在第几个宫
	int cid = col / 3 + 1;
	int rid = row / 3 + 1;
	gong = (rid - 1) * 3 + cid;
	memset(r, true, 10);
}


//跟另一个格子是否有关联.同行，列,宫
bool tagBox::isRel(tagBox* b)
{
	if (this == b)
		return false;
	if (b->col != col && b->row != row && b->gong != gong)
		return false;
	return true;
}

bool CSHUDU::upData(int _value, tagBox* b)
{
	setBitInfo(b, _value);
	set<tagBox*> vRel = getRelBox(b,false);
	for (tagBox* t : vRel)
		if (t->r[_value])
		{
			t->countBeable -= 1;
			t->r[_value] = false;
		}
	return true;
}

//----------------------------------------------------------------------------------------

CSHUDU::CSHUDU(BYTE(*arry)[9])
{
	_countFalse = 0;
	dbgArry_ = arry;
	memset(checkRow_, 0, 12);
	memset(checkCol_, 0, 12);
	memset(checkGong_, 0, 12);
}

CSHUDU::~CSHUDU()
{
	for (tagBox* a : _alBox)
		delete a;
}

set<tagBox*> CSHUDU::getRelBox(tagBox* b, bool one)
{//行列宫全要
#define TEMPCODE \
	if (ref == b || ref->value ) \
		continue;\
	res.insert(ref);\
	if(one)\
		break;\

	set<tagBox*> res;
	for (int i = 0; i < 9; i++)
	{//同一行的
		tagBox* ref = _alBox[b->row * 9 + i];
		TEMPCODE
	}
	for (int i = b->col; i < 81; i += 9)
	{//同一列的
		tagBox* ref = _alBox[i];
		TEMPCODE
	}
	//同一个宫的
	int index = (b->gong - 1) / 3 * 27 + b->col / 3 * 3;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			tagBox* ref = _alBox[index + i * 9 + j];
			TEMPCODE
		}
	return res;
}

//好像不是很细致的理解清楚这里。无论什么情况找一个子节点就可以了吗？
/*会不会出现漏掉了分支的情况？虽然子节点只找一个会快很多很多*/
set<tagBox*> CSHUDU::gusRelBox(tagBox* b)
{//行列宫只要任意一个就行
#define TEMPCODE2 \
	if (ref == b || ref->value ) \
		continue;\
	result.insert(ref);\
	return result;\

	set<tagBox*> result;
	for (int i = 0; i < 9; i++)
	{//同一行的
		tagBox* ref = _alBox[b->row * 9 + i];
		TEMPCODE2
	}
	for (int i = b->col; i < 81; i += 9)
	{//同一列的
		tagBox* ref = _alBox[i];
		TEMPCODE2
	}
	//同一个宫的
	int index = (b->gong - 1) / 3 * 27 + b->col / 3 * 3;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			tagBox* ref = _alBox[index + i * 9 + j];
			TEMPCODE2
		}
	return result;
}

//---4420
int CSHUDU::OnlyNum()
{
	int total = 0;
	vector<tagBox*> vRound;
	for (tagBox* a : _alBox)
		if (!a->value)
			vRound.push_back(a);
	while (vRound.size())
	{//遍历每一个不确定值的，尝试找出
		vector<tagBox*> vNext;
		int count = 0;
		for (tagBox* a : vRound)
		{
			if (a->countBeable == 1)
			{//如果可能的数字只有一个
				int z = a->getBeable()[0];
				upData(z, a);
				count++;//只要有新的,就可能还有连带关系,直到找不到任何更新
			}
			else
				vNext.push_back(a);
		}
		if (!count)
			break;
		total += count;
		vRound = vNext;
	}
	return total;
}

int CSHUDU::init()
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			tagBox* b = new tagBox(i, j, dbgArry_[i][j]);
			_alBox.push_back(b);
		}
	for (tagBox* a : _alBox)
	{
		if (!a->value)
			continue;
		upData(a->value, a);
	}
	return 0;
}

int CSHUDU::work()
{
	//创造出所有的格子,并分类
	init();
	while (1)
	{//反复检查每个宫，行，列.  直到再也找不到连带的更新
		int upCount = 0;
		upCount += OnlyNum();
		upCount += gongAlg();
		if (upCount == 0)
			break;
	}
	parse();//最后剩下的只能递归猜测
	return _countFalse;
}

int CSHUDU::gongAlg()
{
	int total = 0;
	for (int i =1;i<10;i++)//每一个宫内的检查
	{//用一个数组来记录这个宫是否已经存在某个数字
		bool r[10] = {false};
		vector<tagBox*> vUnknow;
		int offest = i%3;
		int index = (i - 1) / 3 * 27 + (offest ? (offest-1)*3 : 6);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				tagBox* a = _alBox[index + i * 9 + j];
				int v = a->value;
				if (v)//确定值就标记
					r[v] = true;
				else
					vUnknow.push_back(a);
			}
		for (int j = 1; j < 10; j++)
		{//如果这个宫内已经有这个值
			if (r[j])
				continue;
			vector<tagBox*> v1;
			for (tagBox* a : vUnknow)
				if (a->r[j])
					v1.push_back(a);
			//如果可能是数值j的格子只有一个,那么就可以确定
			if (v1.size() != 1)
				continue;
			upData(j, v1[0]);
			total++;
		}
	}
	return total;
}

int CSHUDU::parse()
{
	vector<tagBox*> vec;
	for (tagBox* it : _alBox)
	{
		if (it->value)
			continue;
		vec.push_back(it);
	}
	if (vec.size()==0)
		return 999;
	/**************************************/
	sort(vec.begin(),vec.end(), cmp1);
	return guessAlg(vec[0]);//递归猜测;
}

bool CSHUDU::guessAlg(tagBox* p,int no)
{
	if (p->value)
		return true;
	//找有关的子节点
	set<tagBox*> vRel = gusRelBox(p);
	for (int i =1;i<10;i++)
	{//逐一猜测
		if (i==no ||  p->r[i]==false ||  !setBitInfo(p, i))
			continue;
		if (p->row == 2 && p->col == 5 && i == 5)
			int z = 0;
		if (vRel.size() == 0)
			return true;
		int count = 0;
		for (tagBox* b : vRel)
		{
			bool cur = guessAlg(b,i);
			if (!cur)//儿子错了老子肯定也错了
			{
				if(_countFalse == MAXFALSE)
					return false;//如果达到最大错误上限
				else
					_countFalse++;
				resetBit(p);
				break;
			}
			if (++count == vRel.size())
				return true;
		}
	}
	return false;
}

int CSHUDU::getGid(int& row, int& col)
{
	//求出在第几个宫
	int cid = col / 3 + 1;
	int rid = row / 3 + 1;
	return (rid - 1) * 3 + cid;
}

int CSHUDU::getBit(int arr[3], int count, int val,int& arrId, int& off)
{
	arrId = count * 9 + val - 1;//bit位在第几个
	int r = arrId % 32;//余数
	arrId = arrId / 32;//在第几个int.
	off = 32 - r - 1;//偏移量
	return arr[arrId] << r >> 31;
}

bool CSHUDU::setBitInfo(tagBox* b, int val)
{
	//得到bit下标
	int o1, o2, o3 = 0;
	int i1, i2, i3 = 0;
	int r1 = getBit(checkRow_, b->row, val, i1, o1);
	int c1 = getBit(checkCol_, b->col, val, i2, o2);
	int g1 = getBit(checkGong_, b->gong, val, i3, o3);
	//检查bit位,如果不为0,设置失败
	if (r1 || c1 || g1)
		return false;
	//更新bit
	checkRow_[i1] |= (1 << o1);
	checkCol_[i2] |= (1 << o2);
	checkGong_[i3] |= (1 << o3);
	b->value = val;
	dbgArry_[b->row][b->col] = val;
	return true;
}

bool CSHUDU::resetBit(tagBox* b)
{
	int v = b->value;
	int o1, o2, o3 = 0;
	int i1, i2, i3 = 0;
	getBit(checkRow_, b->row, v, i1, o1);
	getBit(checkCol_, b->col, v, i2, o2);
	getBit(checkGong_, b->gong, v, i3, o3);
	//重置bit位为0
	checkRow_[i1] &= ~(1 << o1);
	checkCol_[i2] &= ~(1 << o2);
	checkGong_[i3] &= ~(1 << o3);
	b->value = 0;
	dbgArry_[b->row][b->col] = 0;
	return false;
}