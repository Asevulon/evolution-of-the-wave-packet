#pragma once
#include<complex>
#include<vector>
using namespace std;
#define PI 3.141592653589793
#define MS_READYFORFOURIER WM_USER + 1

class Evolution
{
private:
	vector<complex<double>> Y, A, B, C, D, alpha, betta;
	double m_left, m_right;
	vector<double> m_x, U;

	double m_tau;
	int m_dots;

	int m_Yinum;
	vector<vector<complex<double>>>Yi, FYi;
	int m_Yictr = 0;
	long long m_delay = 0;
	long long timerctr = 0;
protected:
	// Быстрое фурье преобразование is = -1 -prjamoe  +1 -obratnoe, спектр записывается в вектор сигнала
	void fft(long v_size, vector<complex<double>>& F, double is);
	void AddYi(vector<complex<double>>& data);
	void Backward();
	void AlphaBetta();
	void ABCD();
	inline complex<double> d2Y(int i);
	inline complex<double> CalcA(int i);
	inline complex<double> CalcB(int i);
	inline complex<double> CalcC(int i);
	inline complex<double> CalcD(int i);
	inline complex<double> Evolution::CalcAlpha(int i);
	inline complex<double> Evolution::CalcBetta(int i);
public:
	Evolution();
	~Evolution();
	void SetRange(double left, double right);
	void SetTau(double dt);
	void MakeU(double V0, double alpha, int dots);
	void MakeY0(double a, double sigma, double Y0);
	void SetYiNum(int num);
	void SetDelay(int del);
	vector<double>GetX();
	//m - module, i - img, r - real
	vector<double>GetU();
	void Evolution::GetY(vector<double>& out, char type = 'm');


	void Main();
	void fftAnalize();
	vector<int> GetPeakList(int id);
	vector<double>GetPeak(int id, char type = 'm');
	vector<double>GetSpectr(int id, char type = 'm');

	HANDLE IsYAvailable;
	bool Continue;
	HWND parent = NULL;
};


vector<double> Real(vector<complex<double>>& data);
vector<double> Image(vector<complex<double>>& data);
vector<double> Abs(vector<complex<double>>& data);