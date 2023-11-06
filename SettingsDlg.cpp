// SettingsDlg.cpp: файл реализации
//

#include "pch.h"
#include "evolution of the wave packet.h"
#include "afxdialogex.h"
#include "SettingsDlg.h"


// Диалоговое окно SettingsDlg

IMPLEMENT_DYNAMIC(SettingsDlg, CDialogEx)

SettingsDlg::SettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, Y0(1)
	, a(5)
	, Sigma(0.2)
	, left(0)
	, right(10)
	, dots(500)
	, dt(5e-10)
	, V0(1e40)
	, alpha(1e10)
	, omode(0)
	, Top(1)
	, Bot(0)
	, DRCheck(false)
	, Yinum(256)
	, Delay(0)
{

}

SettingsDlg::~SettingsDlg()
{
}

void SettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Y0);
	DDX_Text(pDX, IDC_EDIT2, a);
	DDX_Text(pDX, IDC_EDIT3, Sigma);
	DDX_Text(pDX, IDC_EDIT4, left);
	DDX_Text(pDX, IDC_EDIT5, right);
	DDX_Text(pDX, IDC_EDIT9, dots);
	DDX_Text(pDX, IDC_EDIT8, dt);
	DDX_Text(pDX, IDC_EDIT6, V0);
	DDX_Text(pDX, IDC_EDIT7, alpha);
	DDX_Control(pDX, IDC_COMBO1, OutMode);
	DDX_Text(pDX, IDC_EDIT10, Top);
	DDX_Text(pDX, IDC_EDIT11, Bot);
	DDX_Control(pDX, IDC_CHECK1, DynamicRangeCheck);
	DDX_Text(pDX, IDC_EDIT12, Yinum);
	DDX_Text(pDX, IDC_EDIT13, Delay);
}


BEGIN_MESSAGE_MAP(SettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SettingsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &SettingsDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Обработчики сообщений SettingsDlg


void SettingsDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	omode = OutMode.GetCurSel();

	int c = DynamicRangeCheck.GetCheck();
	if (c == BST_CHECKED)DRCheck = true;
	else
		if (c == BST_UNCHECKED)DRCheck = false;
	CDialogEx::OnOK();
}


void SettingsDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}


BOOL SettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	SetWindowText(L"Настройки запуска");
	OutMode.InsertString(-1, L"Модуль");
	OutMode.InsertString(-1, L"Мнимая часть");
	OutMode.InsertString(-1, L"Реальная часть");
	OutMode.SetCurSel(omode);

	if (DRCheck)DynamicRangeCheck.SetCheck(BST_CHECKED);
	else DynamicRangeCheck.SetCheck(BST_UNCHECKED);
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}
