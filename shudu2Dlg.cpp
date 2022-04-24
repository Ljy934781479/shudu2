
// shudu2Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "shudu2.h"
#include "shudu2Dlg.h"
#include "afxdialogex.h"
#include "_ClassHpp/Cshudu.hpp"
#include <chrono>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
	
END_MESSAGE_MAP()


// Cshudu2Dlg 对话框



Cshudu2Dlg::Cshudu2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHUDU2_DIALOG, pParent)
	, isOk_(0)
	, showStr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cshudu2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for(int i=1,j=1001;i<82;i++,j++)
		DDX_Text(pDX, j, sArry[i]);
	DDX_Text(pDX, IDC_EDIT82, showStr);
	DDX_Text(pDX, IDC_EDIT83, sCountAc);
	DDX_Text(pDX, IDC_EDIT84, sTime);
}

BEGIN_MESSAGE_MAP(Cshudu2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Cshudu2Dlg::OnFinish)
	ON_BN_CLICKED(IDC_BUTTON1, &Cshudu2Dlg::OnReset)
END_MESSAGE_MAP()


// Cshudu2Dlg 消息处理程序

BOOL Cshudu2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cshudu2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cshudu2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cshudu2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int Cshudu2Dlg::check(BYTE(*arry)[9])
{
	//计数,至少满足17个数才有唯一解
	int count = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			int temp = arry[i][j];
			if (temp == 0)
				continue;
			if (temp < 1 || temp>9)//如果不是0-9的数字
			{
				MessageBoxA(m_hWnd, "输入的数字非[1-9]", "错误：", 0);
				return 0;
			}
			count++;
		}
	//if (count < 17)
	//{
	//	MessageBoxA(0, "至少输入17个", "错误：", 0);
	//	return 0;
	//}
	return 1;
}

int Cshudu2Dlg::showResult(BYTE(*arry)[9])
{
	CSHUDU* c = new CSHUDU(arry);
	clock_t begin, end;
	begin = clock();
	int res = c->work();
	sCountAc.Format(_T("%d"), res);
	if (res < MAXFALSE)
	{
		showStr = "解题成功.";
		if (!res)
			sCountAc = "不存在具有多义性的格子。";
	}
	else 
		showStr = "解题失败.";
	end = clock();
	sTime.Format(_T("%d"), (end-begin)/ ((clock_t)1) );

	int nId = 1;
	for(int i =0;i<9;i++)
		for (int j = 0; j < 9; j++)
			sArry[nId++].Format(_T("%d"), arry[i][j]);
	UpdateData(FALSE);
	delete c;
	return 0;
}

void Cshudu2Dlg::OnFinish()
{
	if (isOk_)
		return;
	//获取9*9网格中输入的数字
	BYTE arry[9][9] = {
		{0,1,6, 3,2,0, 0,0,7},
		{0,7,9, 0,4,0, 0,0,6},
		{0,2,8, 6,0,9, 0,0,4},

		{6,0,2, 0,0,0, 5,4,0},
		{7,0,4, 1,0,0, 0,0,0},
		{0,0,1, 5,0,4, 7,6,2},

		{2,0,3, 0,0,6, 0,7,0},
		{0,0,0, 2,1,0, 0,0,0},
		{0,0,0, 7,0,0, 6,0,3},
	};
	/*上面方便调试初始化了9*9， 如果需要从MFC接收，执行下面函数*/
	getInput(arry);
	int input = check(arry);
	if (input)
	{
		showResult(arry);
		isOk_ = 1;
	}
}

void Cshudu2Dlg::OnReset()
{
	isOk_ = 0;
	for (int i = 0; i < 82; i++)
		sArry[i] = "";
	showStr = "";
	sCountAc = "";
	sTime = "";
	UpdateData(FALSE);
}


void Cshudu2Dlg::getInput(BYTE (*arry)[9])
{
	//获取手动输入
	for(int i = 1,j=1001;i<82;i++,j++)
		GetDlgItemTextW(j, sArry[i]);
	int nId = 1;
	for(int i =0;i<9;i++)
		for (int j = 0; j < 9; j++)
			arry[i][j] = _ttoi(sArry[nId++]);
}
