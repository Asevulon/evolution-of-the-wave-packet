
// MyDlg.h: файл заголовка
//

#pragma once
#include"Drawer.h"
#include"Evolution.h"
#include"SettingsDlg.h"
#include"PictureDialog.h"
// Диалоговое окно MyDlg

#define MS_FFTDONE WM_USER + 2

class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EVOLUTION_OF_THE_WAVE_PACKET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void NormalizeX(vector<double>& data);
	void NormalizeY(vector<double>& data);
	
	DECLARE_MESSAGE_MAP()
public:
	~MyDlg();
	Evolution evo;
	afx_msg LRESULT MyDlg::OnMsReadyForFourier(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT MyDlg::OnMsFFTDone(WPARAM wParam, LPARAM lParam);

	Drawer m_PacketDrw;
	afx_msg void OnBnClickedOk();
	SettingsDlg dlg;
	double X0 = 1e-10;
	double q = -1.602176620898e-19;
	double T = 6.5e-16;
	double hp = 1.05456e-34;

	HANDLE FourierThread;
	HANDLE FourierThreadStopped;
	HANDLE EvolutionThread;
	HANDLE ThreadStoped;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	ULONG_PTR TimerId;
	int mode;
	vector<double> x;
	bool first = true;
	double norm;
	CComboBox FFTIDCtr;
	CComboBox FuncIdCtr;
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	Drawer SpectrDrw;
	Drawer WaveDrw;

	PictureDialog Udlg;
	afx_msg void OnBnClickedButtonPotencial();
	CEdit EiCtr;
};
