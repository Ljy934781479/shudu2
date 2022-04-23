
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
	//����ڵڼ�����
	int cid = col / 3 + 1;
	int rid = row / 3 + 1;
	gong = (rid - 1) * 3 + cid;
	memset(r, true, 10);
}


//����һ�������Ƿ��й���.ͬ�У���,��
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
{//���й�ȫҪ
#define TEMPCODE \
	if (ref == b || ref->value ) \
		continue;\
	res.insert(ref);\
	if(one)\
		break;\

	set<tagBox*> res;
	for (int i = 0; i < 9; i++)
	{//ͬһ�е�
		tagBox* ref = _alBox[b->row * 9 + i];
		TEMPCODE
	}
	for (int i = b->col; i < 81; i += 9)
	{//ͬһ�е�
		tagBox* ref = _alBox[i];
		TEMPCODE
	}
	//ͬһ������
	int index = (b->gong - 1) / 3 * 27 + b->col / 3 * 3;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			tagBox* ref = _alBox[index + i * 9 + j];
			TEMPCODE
		}
	return res;
}

//�����Ǻ�ϸ�µ��������������ʲô�����һ���ӽڵ�Ϳ�������
/*�᲻�����©���˷�֧���������Ȼ�ӽڵ�ֻ��һ�����ܶ�ܶ�*/
set<tagBox*> CSHUDU::gusRelBox(tagBox* b)
{//���й�ֻҪ����һ������
#define TEMPCODE2 \
	if (ref == b || ref->value ) \
		continue;\
	result.insert(ref);\
	return result;\

	set<tagBox*> result;
	for (int i = 0; i < 9; i++)
	{//ͬһ�е�
		tagBox* ref = _alBox[b->row * 9 + i];
		TEMPCODE2
	}
	for (int i = b->col; i < 81; i += 9)
	{//ͬһ�е�
		tagBox* ref = _alBox[i];
		TEMPCODE2
	}
	//ͬһ������
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
	{//����ÿһ����ȷ��ֵ�ģ������ҳ�
		vector<tagBox*> vNext;
		int count = 0;
		for (tagBox* a : vRound)
		{
			if (a->countBeable == 1)
			{//������ܵ�����ֻ��һ��
				int z = a->getBeable()[0];
				upData(z, a);
				count++;//ֻҪ���µ�,�Ϳ��ܻ���������ϵ,ֱ���Ҳ����κθ���
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
	//��������еĸ���,������
	init();
	while (1)
	{//�������ÿ�������У���.  ֱ����Ҳ�Ҳ��������ĸ���
		int upCount = 0;
		upCount += OnlyNum();
		upCount += gongAlg();
		if (upCount == 0)
			break;
	}
	parse();//���ʣ�µ�ֻ�ܵݹ�²�
	return _countFalse;
}

int CSHUDU::gongAlg()
{
	int total = 0;
	for (int i =1;i<10;i++)//ÿһ�����ڵļ��
	{//��һ����������¼������Ƿ��Ѿ�����ĳ������
		bool r[10] = {false};
		vector<tagBox*> vUnknow;
		int offest = i%3;
		int index = (i - 1) / 3 * 27 + (offest ? (offest-1)*3 : 6);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				tagBox* a = _alBox[index + i * 9 + j];
				int v = a->value;
				if (v)//ȷ��ֵ�ͱ��
					r[v] = true;
				else
					vUnknow.push_back(a);
			}
		for (int j = 1; j < 10; j++)
		{//�����������Ѿ������ֵ
			if (r[j])
				continue;
			vector<tagBox*> v1;
			for (tagBox* a : vUnknow)
				if (a->r[j])
					v1.push_back(a);
			//�����������ֵj�ĸ���ֻ��һ��,��ô�Ϳ���ȷ��
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
	return guessAlg(vec[0]);//�ݹ�²�;
}

bool CSHUDU::guessAlg(tagBox* p,int no)
{
	if (p->value)
		return true;
	//���йص��ӽڵ�
	set<tagBox*> vRel = gusRelBox(p);
	for (int i =1;i<10;i++)
	{//��һ�²�
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
			if (!cur)//���Ӵ������ӿ϶�Ҳ����
			{
				if(_countFalse == MAXFALSE)
					return false;//����ﵽ����������
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
	//����ڵڼ�����
	int cid = col / 3 + 1;
	int rid = row / 3 + 1;
	return (rid - 1) * 3 + cid;
}

int CSHUDU::getBit(int arr[3], int count, int val,int& arrId, int& off)
{
	arrId = count * 9 + val - 1;//bitλ�ڵڼ���
	int r = arrId % 32;//����
	arrId = arrId / 32;//�ڵڼ���int.
	off = 32 - r - 1;//ƫ����
	return arr[arrId] << r >> 31;
}

bool CSHUDU::setBitInfo(tagBox* b, int val)
{
	//�õ�bit�±�
	int o1, o2, o3 = 0;
	int i1, i2, i3 = 0;
	int r1 = getBit(checkRow_, b->row, val, i1, o1);
	int c1 = getBit(checkCol_, b->col, val, i2, o2);
	int g1 = getBit(checkGong_, b->gong, val, i3, o3);
	//���bitλ,�����Ϊ0,����ʧ��
	if (r1 || c1 || g1)
		return false;
	//����bit
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
	//����bitλΪ0
	checkRow_[i1] &= ~(1 << o1);
	checkCol_[i2] &= ~(1 << o2);
	checkGong_[i3] &= ~(1 << o3);
	b->value = 0;
	dbgArry_[b->row][b->col] = 0;
	return false;
}