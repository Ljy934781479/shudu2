
#include <pch.h>
#include<map>
#include<queue>
#include "../_ClassHpp/Cshudu.hpp"
#include <wtypes.h>

bool cmp1(tagBox* a, tagBox* b)
{
	return a->countBeable < b->countBeable;
}

tagBox::tagBox(int _row, int _col, BYTE* _p)
{
	row = _row;
	col = _col;
	value = *_p;
	ptr_ = _p;
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
	bool bset = setBitInfo(b, _value);
	if (!bset && b->countBeable == 1)
	{
		bCheckLine_ = false;//����������ֻʣ��һ�ֿ��ܲ�������ʧ��,�����û��������Ѿ�������Խ���������
		setBitInfo(b, _value);
	}
	set<tagBox*> s;
	getAlCell(b,s);
	for (tagBox* t : s)
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
	bCheckLine_ = true;//��ҪҪ������Խ���.
	checkLine_ = 0;
}

CSHUDU::~CSHUDU()
{
	for (tagBox* a : _alBox)
		delete a;
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
				if(upData(z, a))
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
			tagBox* b = new tagBox(i, j, &dbgArry_[i][j]);
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
	return dfs(vec[0]);//�����
}

bool CSHUDU::dfs(tagBox* p,int no)
{
#define DFSTEMP(s,b)\
	for(auto it = s.begin();it!=s.end() && b==true;it++)\
	{\
		if ((*it)->value)\
			continue;\
		count++;\
		if (dfs(*it, i))\
			ok++;\
		else\
		{\
			resetBit(p);\
			bok = false;\
			_countFalse++;\
		}\
	}\

	set<tagBox*> sRow;
	set<tagBox*> sCol;
	getRowCell(p, sRow,true);
	getColCell(p, sCol,true);
	for (int i =1;i<10;i++)
	{//��һ�²�
		if (i==no ||  p->r[i]==false ||  !setBitInfo(p, i))
			continue;
		int count = 0;
		int ok = 0;
		bool bok = true;
		DFSTEMP(sRow,bok)
		DFSTEMP(sCol, bok)
		if (count == ok)
			return true;
	}
	return false;
}

bool CSHUDU::bfs(tagBox* p, int no)
{
	set<tagBox*> sAl;
	getAlCell(p, sAl);
	for (int i = 1; i < 10; i++)
	{//��һ�²�
		if (i == no || p->r[i] == false || !setBitInfo(p, i))
			continue;
		int count = 0;
		int ok = 0;
		bool bok = true;
		for (auto it = sAl.begin(); it != sAl.end() && bok == true; it++)
		{
			if ((*it)->value)
				continue;
			count++;
			if (dfs(*it, i))
				ok++; 
			else
			{
				resetBit(p); 
				bok = false; 
				_countFalse++;
			}
		}
		if (count == ok)
			return true;
	}
	return false;
}

bool CSHUDU::creatSST(vector<tagBox*>& vUnkow)
{//�������nƽ�����񲻿ɱ�����
	if (!vUnkow.size())
		return false;
	vector<tagBox*> vRes;
	vector<tagBox*> vTemp = vUnkow;
	tagBox* f = vTemp[0];
	vRes.push_back(f);
	while (1)
	{
		vector<tagBox*> vNext;
		for (tagBox* b : vTemp)
		{
			if (b == f)
				continue;
			if (f->row == b->row)
				f->sRchild.insert(b);
			else if (f->col == b->col)
				f->sCchild.insert(b);
			else
				vNext.push_back(b);
		}
		if (vNext.size() == 0)
			break;
		vTemp = vNext;
		f = vTemp[0];
		vRes.push_back(f);
	}
	vUnkow = vRes;
	return true;
}

bool CSHUDU::gusSstBox(tagBox* p)
{
	for (int i = 1; i < 10; i++)
	{//��һ�²�
		if (p->r[i] == false || !setBitInfo(p, i))
			continue;
		return true;
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
	int dl = lineCell(b);
	if (bCheckLine_)
	{
		if ((dl == 1 || dl == 3) && (checkLine_ << (val - 1) >> 31))
			return false;
		if ((dl == 2 || dl == 3) && (checkLine_ << (8 + val) >> 31))
			return false;
	}
	//�õ�bit�±�
	int o1, o2, o3 = 0;
	int i1, i2, i3 = 0;
	bool r1 = getBit(checkRow_, b->row, val, i1, o1);
	bool c1 = getBit(checkCol_, b->col, val, i2, o2);
	bool g1 = getBit(checkGong_, b->gong, val, i3, o3);
	//���bitλ,�����Ϊ0,����ʧ��
	if (r1 || c1 || g1)
		return false;
	//����bit
	checkRow_[i1] |= (1 << o1);
	checkCol_[i2] |= (1 << o2);
	checkGong_[i3] |= (1 << o3);
	if (bCheckLine_)
	{
		if (dl == 1)
			checkLine_ |= (1 << (32 - val));
		else if (dl == 2)
			checkLine_ |= (1 << (32 - 9 - val));
		else if (dl == 3)
		{
			checkLine_ |= (1 << (32 - val));
			checkLine_ |= (1 << (32 - 9 - val));
		}
	}
	b->value = val;
	*b->ptr_ = val;
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
	if (bCheckLine_)
	{
		int dl = lineCell(b);
		if (dl == 1)
			checkLine_ &= ~(1 << (32 - v));
		else if (dl == 2)
			checkLine_ &= ~(1 << (32 - 9 - v));
		else if (dl == 3)
		{
			checkLine_ &= ~(1 << (32 - v));
			checkLine_ &= ~(1 << (32 - 9 - v));
		}
	}
	b->value = 0;
	*b->ptr_ = 0;
	return false;
}

int CSHUDU::lineCell(tagBox* a)
{//��Խ���1  �ҶԽ���2
	int res = 0;
	if (a->row == a->col)
		res += 1;
	for (int i = 0, j = 8; i < 9; i++, j--)
		if (a->row == i && a->col == j)
		{
			res += 2;
			break;
		}
	return res;
}

void CSHUDU::getRowCell(tagBox* a, set<tagBox*>& s,bool one)
{
	for (int i = 0; i < 9; i++)
	{//ͬһ�е�
		tagBox* ref = _alBox[a->row * 9 + i];
		if (ref == a || ref->value)
			continue; \
		s.insert(ref);
		if (one)
			break;
	}
}

void CSHUDU::getColCell(tagBox* a, set<tagBox*>& s, bool one)
{
	for (int i = a->col; i < 81; i += 9)
	{//ͬһ�е�
		tagBox* ref = _alBox[i];
		if (ref == a || ref->value) 
			continue; 
		s.insert(ref);
		if (one)
			break;
	}
}

void CSHUDU::getGongCell(tagBox* a, set<tagBox*>& s, bool second)
{
	//ͬһ������
	int index = (a->gong - 1) / 3 * 27 + a->col / 3 * 3;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			tagBox* ref = _alBox[index + i * 9 + j];
			if (ref == a || ref->value)
				continue;
			s.insert(ref);
		}
}

void CSHUDU::getAlCell(tagBox* a, set<tagBox*>& s, bool second)
{
	getRowCell(a, s, second);
	getColCell(a, s, second);
	getGongCell(a, s, second);
}
