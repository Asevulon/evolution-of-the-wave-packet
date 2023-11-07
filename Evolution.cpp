#include"pch.h"
#include "Evolution.h"


vector<double> Real(vector<complex<double>>& data)
{
	vector<double> res; 
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i] = data[i].real();
	}
	return res;
}
vector<double> Image(vector<complex<double>>& data)
{
	vector<double> res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i] = data[i].imag();
	}
	return res;
}
vector<double> Abs(vector<complex<double>>& data)
{
	vector<double> res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i] = sqrt(data[i].real() * data[i].real() + data[i].imag() * data[i].imag());
	}
	return res;
}

Evolution::Evolution()
{
	IsYAvailable = CreateEventW(NULL, TRUE, TRUE, NULL);
}
Evolution::~Evolution()
{
	CloseHandle(IsYAvailable);
}
void Evolution::SetTau(double dt)
{
	m_tau = dt;
}
void Evolution::SetRange(double Left, double Right)
{
	m_left = Left;
	m_right = Right;
}
void Evolution::SetDelay(int del)
{
	m_delay = del;
}
void Evolution::MakeU(double V0, double alpha, int dots)
{
	U.resize(dots);
	m_x.resize(dots);

	m_dots = dots;
	double step = (m_right - m_left) / (dots - 1);
	for (int i = 0; i < dots; i++)
	{
		m_x[i] = i * step;
		U[i] = sin(alpha * m_x[i]);
		U[i] *= V0 * U[i];
	}
}
void Evolution::MakeY0(double a, double sigma, double Y0)
{
	Y.resize(m_dots);
	for (int i = 0; i < m_dots; i++)
	{
		Y[i] = Y0 * exp(-(m_x[i] - a) * (m_x[i] - a) / (4 * sigma * sigma));
	}


	A.resize(m_dots);
	B.resize(m_dots);
	C.resize(m_dots);
	D.resize(m_dots);
	alpha.resize(m_dots);
	betta.resize(m_dots);

}
void Evolution::SetYiNum(int num)
{
	m_Yinum = num;
	Yi.resize(m_Yinum);
}

double Evolution::GetTau()
{
	return m_tau;
}
vector<double> Evolution::GetX()
{
	return m_x;
}
vector<double> Evolution::GetU()
{
	return U;
}
void Evolution::GetY(vector<double>& out, char type)
{
	WaitForSingleObject(IsYAvailable, INFINITE);
	ResetEvent(IsYAvailable);

	switch (type)
	{
	case 'm':out = Abs(Y);
		break;
	case 'r':out = Real(Y);
		break;
	case 'i':out = Image(Y);
		break;
	default:
		abort();
		break;
	}
	SetEvent(IsYAvailable);
}


complex<double> Evolution::d2Y(int i)
{
	double dx1_1 = m_x[i + 1] - m_x[i - 1];
	double dx10 = m_x[i + 1] - m_x[i];
	double dx0_1 = m_x[i] - m_x[i - 1];
	complex<double> val1 = (Y[i + 1] - Y[i]) / dx10;
	complex<double> val2 = (Y[i] - Y[i - 1]) / dx0_1;
	return (val1 - val2) * 2. / dx1_1;
}


complex<double> Evolution::CalcA(int j)
{
	double dx1_1 = m_x[j + 1] - m_x[j - 1];
	double dx0_1 = m_x[j] - m_x[j - 1];
	return -complex<double>(0, m_tau / (dx1_1 * dx0_1));
}
complex<double> Evolution::CalcB(int i)
{
	double dx1 = m_x[i + 1] - m_x[i - 1];
	double dx2 = m_x[i + 1] - m_x[i];
	return -complex<double>(0, m_tau / (dx1 * dx2));
}
complex<double> Evolution::CalcC(int i)
{
	double dx1_1 = m_x[i + 1] - m_x[i - 1];
	double dx10 = m_x[i + 1] - m_x[i];
	double dx0_1 = m_x[i] - m_x[i - 1];
	double img = m_tau * U[i] / 2.;
	img += m_tau * (1. / dx10 + 1. / dx0_1) / dx1_1;
	return complex<double>(1, img);
}
complex<double> Evolution::CalcD(int i)
{
	complex<double>mul(0, m_tau / 2);
	return Y[i] +  mul * (d2Y(i) - U[i] * Y[i]);
}
void Evolution::ABCD()
{
	for (int i = 1; i < m_dots - 1; i++)
	{
		A[i] = CalcA(i);
		B[i] = CalcB(i);
		C[i] = CalcC(i);
		D[i] = CalcD(i);
	}
}

complex<double> Evolution::CalcAlpha(int i)
{
	i--;
	return -B[i] / (C[i] + A[i] * alpha[i]);
}
complex<double> Evolution::CalcBetta(int i)
{
	i--;
	return (D[i] - A[i] * betta[i]) / (C[i] + A[i] * alpha[i]);
}
void Evolution::AlphaBetta()
{
	alpha[1] = complex<double>(0, 0);
	betta[1] = complex<double>(0, 0);

	for (int i = 2; i < m_dots; i++)
	{
		alpha[i] = CalcAlpha(i);
		betta[i] = CalcBetta(i);
	}
}


void Evolution::Backward()
{
	Y[m_dots - 1] = complex<double>(0, 0);
	Y[0] = complex<double>(0, 0);
	for (int i = m_dots - 1; i > 0; i--)
	{
		Y[i - 1] = alpha[i] * Y[i] + betta[i];
	}
}


void Evolution::Main()
{
	Continue = true;
	m_Yictr = 0;
	timerctr = 0;
	while (Continue)
	{
		ABCD();
		AlphaBetta();
		WaitForSingleObject(IsYAvailable, INFINITE);
		ResetEvent(IsYAvailable);
		Backward();
		if ((m_Yictr < m_Yinum) && (timerctr > m_delay))
		{
			Yi[m_Yictr] = Y;
			m_Yictr++;
			if (m_Yictr == m_Yinum)SendMessageW(parent, MS_READYFORFOURIER, NULL, NULL);
		}
		SetEvent(IsYAvailable);
		if ((isnan(Y[0].real())) || (isnan(Y[0].imag())))
		{
			MessageBoxW(NULL, L"Встречен nan", L"Ошибка", MB_ICONHAND);
			break;
		}
		timerctr++;
	}
}

void Evolution::RestartFFT()
{
	m_Yictr = 0;
}
void Evolution::fftAnalize()
{
	FYi.resize(m_dots);
	for (int i = 0; i < m_dots; i++)
	{
		vector<complex<double>> temp(m_Yinum);
		for (int j = 0; j < m_Yinum; j++)
		{
			temp[j] = Yi[j][i];
		}
		fft(m_Yinum, temp, -1);
		FYi[i] = temp;
	}
}
vector<int> Evolution::GetPeakList(int id)
{
	vector<int>res;
	if ((id < 0) || (id > m_dots))return res;
	auto temp = Abs(FYi[id]);
	double cap = *max_element(temp.begin(), temp.end()) / 4.;
	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i] > cap)res.push_back(i);
	}
	return res;
}

vector<double>Evolution::GetSpectr(int id, char type)
{
	if (type == 'm')return Abs(FYi[id]);
	if (type == 'i')return Image(FYi[id]);
	if (type == 'r')return Real(FYi[id]);
}
vector<double>Evolution::GetPeak(int id, char type)
{
	vector<double>res;
	if ((id < 0) || (id > m_Yinum))return res;

	vector<complex<double>> temp(m_dots);
	for (int i = 0; i < m_dots; i++)
	{
		temp[i] = FYi[i][id];
	}
	if (type == 'm')return Abs(temp);
	if (type == 'i')return Image(temp);
	if (type == 'r')return Real(temp);
}




	void Evolution::fft(long v_size, vector<complex<double>>&F, double is)
	{
		complex<double>  temp, w, c;
		long i, i1, j, j1, istep;
		long m, mmax;
		long n = v_size;
		double fn, r1, theta;
		fn = (double)n;
		double r = PI * is;
		j = 1;
		for (i = 1; i <= n; i++)
		{
			i1 = i - 1;
			if (i < j)
			{
				j1 = j - 1;
				temp = F[j1];
				F[j1] = F[i1];
				F[i1] = temp;
			}
			m = n / 2;
			while (j > m) { j -= m;	m = (m + 1) / 2; }
			j += m;
		}
		mmax = 1;
		while (mmax < n)
		{
			istep = 2 * mmax;
			r1 = r / (double)mmax;
			for (m = 1; m <= mmax; m++)
			{
				theta = r1 * (double)(m - 1);
				w = complex<double>(cos(theta), sin(theta));
				for (i = m - 1; i < n; i += istep)
				{
					j = i + mmax;
					c = F[j];
					temp = w * c;
					F[j] = F[i] - temp;
					F[i] = F[i] + temp;
				}
			}
			mmax = istep;
		}
		if (is > 0)  for (i = 0; i < n; i++) { F[i] /= fn;}
		return;
	}