
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "evolution of the wave packet.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EVOLUTION_OF_THE_WAVE_PACKET_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PACKET, m_PacketDrw);
	DDX_Control(pDX, IDC_COMBO1, FFTIDCtr);
	DDX_Control(pDX, IDC_COMBO2, FuncIdCtr);
	DDX_Control(pDX, IDC_SPECTR, SpectrDrw);
	DDX_Control(pDX, IDC_WAVEFUNC, WaveDrw);
	DDX_Control(pDX, IDC_EDIT1, EiCtr);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_MESSAGE(MS_READYFORFOURIER, &MyDlg::OnMsReadyForFourier)
	ON_MESSAGE(MS_FFTDONE, &MyDlg::OnMsFFTDone)
	ON_CBN_SELCHANGE(IDC_COMBO1, &MyDlg::OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &MyDlg::OnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON_POTENCIAL, &MyDlg::OnBnClickedButtonPotencial)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	evo.parent = GetSafeHwnd();

	m_PacketDrw.SetTitle(L"Эволюция Волнового Пакета (Надеюсь)");
	m_PacketDrw.SetDynamicRangeY(true);

	ThreadStoped = CreateEventW(NULL, TRUE, TRUE, NULL);
	FourierThreadStopped = CreateEventW(NULL, TRUE, TRUE, NULL);

	Udlg.pParent = this;
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


DWORD WINAPI EvolutionThreadFunction(LPVOID p)
{
	MyDlg* dlg = (MyDlg*)p;
	ResetEvent(dlg->ThreadStoped);
	dlg->evo.Main();
	dlg->KillTimer(dlg->TimerId);
	SetEvent(dlg->ThreadStoped);
	return 0;
}

void MyDlg::OnBnClickedOk()
{
	if (dlg.DoModal() == IDCANCEL)return;
	// дожадаться потока
	evo.Continue = false;
	WaitForSingleObject(ThreadStoped, INFINITE);
	WaitForSingleObject(FourierThreadStopped,INFINITE);

	//параметры задачи
	evo.SetRange(dlg.left * X0, dlg.right * X0);
	evo.MakeU(dlg.V0 * q, dlg.alpha, dlg.dots);
	evo.MakeY0(dlg.a * X0, dlg.Sigma * X0, dlg.Y0);
	evo.SetTau(dlg.dt * T);
	evo.SetYiNum(dlg.Yinum);
	evo.SetDelay(dlg.Delay);

	//подготовка к рисованию
	x = evo.GetX();
	NormalizeX(x);
	m_PacketDrw.SetKeys(x);
	first = true;
	m_PacketDrw.SetDynamicRangeY(dlg.DRCheck);
	m_PacketDrw.SetStaticRangeY(dlg.Top, dlg.Bot);
	mode = dlg.omode;

	
	CloseHandle(EvolutionThread);
	EvolutionThread = CreateThread(NULL, NULL, EvolutionThreadFunction, this, NULL, NULL);
	KillTimer(TimerId);
	TimerId = SetTimer(1, 16, NULL);
}


void MyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	vector<double> data;
	if (mode == 0)
	{
		evo.GetY(data);
	}
	else if (mode == 1)
	{
		evo.GetY(data, 'i');
	}
	else if (mode == 2)
	{
		evo.GetY(data, 'r');
	}
	else abort();
	if (first)
	{
		first = false;
		norm = max(fabs(*max_element(data.begin(), data.end())), fabs(*min_element(data.begin(), data.end())));
	}
	else
	{
		double cnorm = max(fabs(*max_element(data.begin(), data.end())), fabs(*min_element(data.begin(), data.end())));
		if (cnorm > norm)norm = cnorm;
	}
	NormalizeY(data);
	m_PacketDrw.SetData(data);
	m_PacketDrw.Invalidate();
	CDialogEx::OnTimer(nIDEvent);
}



void MyDlg::NormalizeX(vector<double>& data)
{
	for (int i = 0; i < data.size(); i++)
	{
		data[i] /= X0;
	}
}
void MyDlg::NormalizeY(vector<double>& data)
{
	for (int i = 0; i < data.size(); i++)
	{
		data[i] /= norm;
	}
}

MyDlg::~MyDlg()
{
	CloseHandle(ThreadStoped);
	CloseHandle(FourierThreadStopped);
}

DWORD WINAPI FourierThreadFunction(LPVOID p)
{
	MyDlg* dlg = (MyDlg*)p;
	ResetEvent(dlg->FourierThreadStopped);
	dlg->evo.fftAnalize();
	SetEvent(dlg->FourierThreadStopped);
	dlg->SendMessage(MS_FFTDONE);
	return 0;
}
afx_msg LRESULT MyDlg::OnMsReadyForFourier(WPARAM wParam, LPARAM lParam)
{
	WaitForSingleObject(FourierThreadStopped, INFINITE);
	CloseHandle(FourierThread);
	FourierThread = CreateThread(NULL, NULL, FourierThreadFunction, this, NULL, NULL);
	return NULL;
}

afx_msg LRESULT MyDlg::OnMsFFTDone(WPARAM wParam, LPARAM lParam)
{
	FFTIDCtr.ResetContent();
	FuncIdCtr.ResetContent();
	int dots = dlg.dots;
	CString str;
	for (int i = 0; i < dots; i++)
	{
		str.Format(L"%d", i);
		FFTIDCtr.InsertString(-1, str);
	}
	FFTIDCtr.SetCurSel(0);

	
	FuncIdCtr.SetCurSel(0);

	WaveDrw.SetKeys(x);
	vector<double>keys(dlg.Yinum);
	for (int i = 0; i < keys.size(); i++)keys[i] = i;
	SpectrDrw.SetKeys(keys);
	SpectrDrw.SetSelectedPos(NONE);

	return NULL;
}

void NormalizeSpectr(vector<double>& data)
{
	double mx = *max_element(data.begin(), data.end());
	double mn = *min_element(data.begin(), data.end());
	double m = max(fabs(mx),fabs(mn));
	for (int i = 0; i < data.size(); i++)data[i] /= m;
}
void MyDlg::OnSelchangeCombo1()
{
	// TODO: добавьте свой код обработчика уведомлений
	int cs = FFTIDCtr.GetCurSel();
	CString str;
	FFTIDCtr.GetLBText(cs, str);
	int id = _wtoi(str);
	char c = 0;
	if (mode == 0)c = 'm';
	else
		if (mode == 1)c = 'i';
		else
			if (mode == 2)c = 'r';
	auto data = evo.GetSpectr(id, c);
	NormalizeSpectr(data);
	SpectrDrw.SetData(data);
	SpectrDrw.Invalidate();

	FuncIdCtr.ResetContent();
	auto ids = evo.GetPeakList(id);
	for (int i = 0; i < ids.size(); i++)
	{
		str.Format(L"%d", ids[i]);
		FuncIdCtr.InsertString(-1, str);
	}
	SpectrDrw.SetSelectedPos(NONE);

}


void MyDlg::OnSelchangeCombo2()
{
	// TODO: добавьте свой код обработчика уведомлений
	int cs = FuncIdCtr.GetCurSel();
	CString str;
	FuncIdCtr.GetLBText(cs, str);
	int id = _wtoi(str);
	char c = 0;
	if (mode == 0)c = 'm';
	else
		if (mode == 1)c = 'i';
		else
			if (mode == 2)c = 'r';
	auto data = evo.GetPeak(id, c);
	NormalizeSpectr(data);
	WaveDrw.SetData(data);
	WaveDrw.Invalidate();
	SpectrDrw.SetSelectedPos(id);
	SpectrDrw.Invalidate();

	double Ei = hp * id / dlg.dt/q;
	if ((fabs(Ei) < 1e5) && (fabs(Ei) > 1e-2))str.Format(L"%.2f", Ei);
	else str.Format(L"%.2e", Ei);
	EiCtr.SetWindowTextW(str);
}


void MyDlg::OnBnClickedButtonPotencial()
{
	// TODO: добавьте свой код обработчика уведомлений
	auto temp = evo.GetU();
	Udlg.SetParams(L"График потенциала", L"U(x)", x, temp);
	Udlg.ShowWindow(SW_SHOW);
	Udlg.UpdateWindow();
}
