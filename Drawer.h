#pragma once


// Drawer
#include<gdiplus.h>
#include<vector>
#include<algorithm>
using namespace std;
using namespace Gdiplus;

#define NONE -1
class Drawer : public CStatic
{
	DECLARE_DYNAMIC(Drawer)
private:
	vector<double>data;
	vector<double>keys;

	double lPadding;
	double rPadding;
	double tPadding;
	double bPadding;
	int MarksLen;

	double left;
	double right;
	double bot;
	double top;


	ULONG_PTR token;
	
	CString title;
	bool DynamicRangeY = true;
	double StaticTop = 1;
	double StaticBot = 0;
	double DynamicTop = 1;
	double DynamicBot = 0;
	int selectedpos = NONE;
public:
	Drawer();
	virtual ~Drawer();

protected:
	double CalcStringLen(HDC hDC, CString string);
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void SetData(vector<double>&y);
	void SetKeys(vector<double>&x);
	void SetData(vector<double>&y, vector<double>&x);

	//������ �������� � ��������� (�������� �������� �� 0 �� 100)
	void SetPadding(double lPadding = 10, double rPadding = 5, double tPadding = 22, double bPadding = 22);
	void SetMarksLen(int len);
	void SetTitle(CString str);
	void Drawer::SetDynamicRangeY(bool bDRange = true);
	void SetStaticRangeY(double ymax = 1, double ymin = 0);
	void SetSelectedPos(int pos);
};


