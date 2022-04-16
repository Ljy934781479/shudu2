
// shudu2Dlg.h: 头文件
//

#pragma once


// Cshudu2Dlg 对话框
class Cshudu2Dlg : public CDialogEx
{
// 构造
public:
	Cshudu2Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHUDU2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//检查输入数据的合法性,如果用户没有输入，或者异常返回0
	int check(BYTE (*arry)[9]);
	//展示结果
	int showResult(BYTE(*arry)[9]);
	afx_msg void OnFinish();
	afx_msg void OnReset();
	int isOk_;
	
	void getInput(BYTE(*arry)[9]);
public:

	CString sArry[82];
	CString showStr;
	CString sCountAc;
	CString sTime;
};
