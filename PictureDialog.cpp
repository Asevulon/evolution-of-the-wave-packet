// PictureDialog.cpp: файл реализации
//

#include "pch.h"
#include "evolution of the wave packet.h"
#include "afxdialogex.h"
#include "PictureDialog.h"


// Диалоговое окно PictureDialog

IMPLEMENT_DYNAMIC(PictureDialog, CDialogEx)

PictureDialog::PictureDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
	Create(IDD_DIALOG2, pParent);
}

PictureDialog::~PictureDialog()
{
}

void PictureDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POTENCIAL, drw);
}


BEGIN_MESSAGE_MAP(PictureDialog, CDialogEx)
END_MESSAGE_MAP()


// Обработчики сообщений PictureDialog


BOOL PictureDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void PictureDialog::SetParams(CString Name, CString Title, vector<double>& X, vector<double>& Y)
{
	name = Name;
	title = Title;
	x = X;
	y = Y;
	drw.SetPadding();
	drw.SetData(y, x);
	drw.SetTitle(title);

	SetWindowTextW(name);
}