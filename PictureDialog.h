#pragma once
#include "afxdialogex.h"
#include"Drawer.h"

// Диалоговое окно PictureDialog

class PictureDialog : public CDialogEx
{
	DECLARE_DYNAMIC(PictureDialog)

public:
	PictureDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~PictureDialog();
	vector<double>x, y;
	CString title;
	CString name;
	CWnd* pParent;
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	Drawer drw;
	virtual BOOL OnInitDialog();

	void SetParams(CString Name, CString Title, vector<double>& x, vector<double>& Y);
};
