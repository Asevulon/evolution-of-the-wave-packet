#pragma once
#include "afxdialogex.h"


// Диалоговое окно SettingsDlg

class SettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SettingsDlg)

public:
	SettingsDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~SettingsDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	double Y0;
	double a;
	double Sigma;
	double left;
	double right;
	int dots;
	double dt;
	double V0;
	double alpha;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	CComboBox OutMode;
	int omode;
	double Top;
	double Bot;
	CButton DynamicRangeCheck;
	bool DRCheck = true;
	int Yinum;
	long long Delay;
};
