// Drawer.cpp: файл реализации
//

#include "pch.h"
#include "Drawer.h"


// Drawer

IMPLEMENT_DYNAMIC(Drawer, CStatic)

Drawer::Drawer()
{
	GdiplusStartupInput si;
	GdiplusStartup(&token, &si, NULL);
	SetPadding();
}

Drawer::~Drawer()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(Drawer, CStatic)
END_MESSAGE_MAP()



// Обработчики сообщений Drawer




void Drawer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	Graphics ToWindow(lpDrawItemStruct->hDC);
	int width = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	int height = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;
	Bitmap bmp(width, height);
	Graphics gr(&bmp);
	gr.Clear(Color::White);

	if (data.empty() || keys.empty() || (data.size()!=keys.size()))
	{
		ToWindow.DrawImage(&bmp, 0, 0);
		return;
	}

	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	if (DynamicRangeY)
	{
		top = DynamicTop;
		bot = DynamicBot;
	}
	else
	{
		top = StaticTop;
		bot = StaticBot;
	}
	if (top == bot)
	{
		top += 0.25 * fabs(top);
		bot -= 0.25 * fabs(bot);
	}
	if ((top == 0) && (bot == 0))
	{
		top = 1;
		bot = -1;
	}
	double actWidth(right - left);
	double actHeight(top - bot);
	double actLP = actWidth * lPadding / 100.;
	double actRP = actWidth * rPadding / 100.;
	double actTP = actHeight * tPadding / 100.;
	double actBP = actHeight * bPadding / 100.;

	double actTop = top + actTP;
	double actBot = bot - actBP;
	double actLeft = left - actLP;
	double actRight = right + actRP;

	actWidth = actRight - actLeft;
	actHeight = actTop - actBot;


	double xScale = width / actWidth;
	double yScale = -height / actHeight;

	Pen BackGroundPen(Color(200, 200, 200));

	Matrix matr;
	matr.Scale(xScale, yScale);
	matr.Translate(actLP - left, -actTop);


	float steplenY = (top - bot) / 10.;
	float steplenX = (right - left) / 10.;

	FontFamily FF(L"Times New Roman");
	Gdiplus::Font font(&FF, 12, FontStyle::FontStyleRegular, UnitPixel);
	SolidBrush brush(Color::Black);
	for (int i = 0; i < 11; i++)
	{
		PointF p1, p2;
		p1.X = left;
		p1.Y = top - i * steplenY;

		p2.X = right;
		p2.Y = top - i * steplenY;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);

		gr.DrawLine(&BackGroundPen, p1, p2);


		p1.X = right - i * steplenX;
		p1.Y = top;

		p2.X = right - i * steplenX;
		p2.Y = bot;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);

		gr.DrawLine(&BackGroundPen, p1, p2);


		CString str;
		double stry = top - i * steplenY;
		if (fabs(stry) < 1e5)
		{
			str.Format(L"%.2f", stry);
		}
		else
		{
			str.Format(L"%.2e", stry);
		}

		PointF strPoint;
		strPoint.X = left - actWidth / 100 - CalcStringLen(lpDrawItemStruct->hDC, str) / xScale;
		strPoint.Y = top - i * steplenY -6 / yScale;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);


		double strx = right - i * steplenX;
		if (fabs(strx) < 1e5)
		{
			str.Format(L"%.2f", strx);
		}
		else
		{
			str.Format(L"%.2e", strx);
		}

		strPoint.X = right - i * steplenX - CalcStringLen(lpDrawItemStruct->hDC, str) / xScale / 2.;
		strPoint.Y = bot + 6 / yScale;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	Pen pen(Color::Black, 1.1F);
	PointF p1(left, top), p2(left, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);

	p1 = PointF(left, bot);
	p2 = PointF(right, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);


	if (!title.IsEmpty())
	{

		p1.X = actLeft + actWidth / 2 - CalcStringLen(lpDrawItemStruct->hDC, title) / 2 / xScale;
		p1.Y = (actTop + top) / 2 - 6 / yScale;
		matr.TransformPoints(&p1);
		gr.DrawString(title, title.GetLength() + 1, &font, p1, &brush);
	}


	p1 = PointF(keys[0],data[0]);
	matr.TransformPoints(&p1);

	Pen DataPen(Color(200, 100, 0));

	for (int i = 1; i < data.size(); i++)
	{
		p2 = PointF(keys[i],data[i]);
		matr.TransformPoints(&p2);
		gr.DrawLine(&DataPen, p1, p2);
		p1 = p2;
	}
	
	if (selectedpos != NONE)
	{
		Pen PosPen(Color::LightGreen, 2);
		p1.X = keys[selectedpos];
		p1.Y = data[selectedpos]- actHeight * 0.025;
		p2.X = p1.X;
		p2.Y = data[selectedpos] + actHeight * 0.025;
		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);
		gr.DrawLine(&PosPen, p1, p2);
	}

	ToWindow.DrawImage(&bmp, 0, 0);
}


void Drawer::SetData(vector<double>& y)
{
	data = y;
	if (data.empty())return;
	DynamicTop = *max_element(data.begin(), data.end());
	DynamicBot = *min_element(data.begin(), data.end());
}
void Drawer::SetKeys(vector<double>& x)
{
	keys = x;
	if (keys.empty())return;
	left = *min_element(keys.begin(), keys.end());
	right = *max_element(keys.begin(),keys.end());
}
void Drawer::SetData(vector<double>& y, vector<double>& x)
{
	SetData(y);
	SetKeys(x);
}

void Drawer::SetPadding(double lP, double rP, double tP, double bP)
{
	lPadding = lP;
	rPadding = rP;
	tPadding = tP;
	bPadding = bP;
}
void Drawer::SetMarksLen(int len)
{
	MarksLen = len;
}
void Drawer::SetTitle(CString str)
{
	title = str;
}


double Drawer::CalcStringLen(HDC hDC, CString str)
{
	double numlen = 0;
	int size = str.GetLength() + 1;
	for (int i = 0; i < size; i++)
	{
		ABCFLOAT abc;
		GetCharABCWidthsFloatW(hDC, str[i], str[i], &abc);
		numlen += abc.abcfA + abc.abcfB + abc.abcfC;
	}
	return numlen;
}


void Drawer::SetDynamicRangeY(bool bDRange)
{
	DynamicRangeY = bDRange;
}
void Drawer::SetStaticRangeY(double ymax, double ymin)
{
	StaticTop = ymax;
	StaticBot = ymin;
}
void Drawer::SetSelectedPos(int pos)
{
	selectedpos = pos;
}