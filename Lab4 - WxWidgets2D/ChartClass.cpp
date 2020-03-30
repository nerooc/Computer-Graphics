#include <wx/DC.h>
#include <memory>

#include "ChartClass.h"
#include "vecmat.h"

#define PI 3.14159265359

//FUNKCJE NA MACIERZACH////////////////////////////////

//FUNKCJA TRANSLACJI - POZWALA NA PORUSZANIE WYKRESEM
//MACIERZ TRANSLACJI:
//	 _  	_	
//	| 1 0 Tx |
//	| 0 1 Ty |
//	| 0 0 1  |
//	|_      _|

Matrix ChartClass::Set_Translate(double Tx, double Ty)
{
	Matrix translateM;

	translateM.data[0][0] = 1;
	translateM.data[1][0] = 0;
	translateM.data[1][1] = 1;
	translateM.data[0][1] = 0;
	translateM.data[2][2] = 1;
	translateM.data[0][2] = Tx;
	translateM.data[1][2] = Ty;

	return translateM;
}

//FUNKCJA SKALUJACA
//MACIERZ SKALUJACA:
//
//	 _   _				
//	|Sx  0|
//	|0	Sy|
//	|_   _|

Matrix ChartClass::Set_Scale(int w, int h)
{
	Matrix scaleM;

	double Sx = (w - 20.0) / (cfg->Get_x1() - cfg->Get_x0());
	double Sy = (h - 20.0) / (cfg->Get_y1() - cfg->Get_y0());

	scaleM.data[0][0] = Sx;
	scaleM.data[1][0] = 0;
	scaleM.data[1][1] = Sy;
	scaleM.data[0][1] = 0;

	return scaleM;
}

//FUNKCJA OBRACAJSCA
//MACIERZ ROTACJI:
//	 ___				___	
//	|cos(alpha) -sin(alpha)|
//	|sin(alpha)	cos(alpha) |
//	 ___			    ___

Matrix ChartClass::Set_Rotation(double alpha)
{
	Matrix rotationM;

	double angle = alpha * PI / 180;

	rotationM.data[0][0] = cos(angle);
	rotationM.data[0][1] = sin(angle);
	rotationM.data[1][0] = -sin(angle);
	rotationM.data[1][1] = cos(angle);

	return rotationM;
}


//FUNKCJE PODSTAWOWE//////////////////////////////////


ChartClass::ChartClass(std::shared_ptr<ConfigClass> c)
{
	cfg = std::move(c);
	x_step = 200;
}

void ChartClass::Set_Range()
{
	double xmin = 9999.9, xmax = -9999.9, ymin = 9999.9, ymax = -9999.9;
	double x, y, step;
	int i;

	xmin = cfg->Get_x_start();
	xmax = cfg->Get_x_stop();

	step = (cfg->Get_x_stop() - cfg->Get_x_start()) / (double)x_step;
	x = cfg->Get_x_start();

	for (i = 0; i <= x_step; i++)
	{
		y = GetFunctionValue(x);
		if (y > ymax) ymax = y;
		if (y < ymin) ymin = y;
		x = x + step;
	}

	y_min = ymin;
	y_max = ymax;
	x_min = xmin;
	x_max = xmax;
}


double ChartClass::GetFunctionValue(double x)
{
	if (cfg->Get_F_type() == 1) return x * x;
	if (cfg->Get_F_type() == 2) return 0.5 * exp(4 * x - 3 * x * x);
	return x + sin(x * 4.0);
}


double ChartClass::Get_Y_min()
{
	Set_Range();
	return y_min;
}

double ChartClass::Get_Y_max()
{
	Set_Range();
	return y_max;
}


//FUNKCJE RYSUJACE/OBSLUGUJACE////////////////////////////////////


void ChartClass::Draw(wxDC* DC, int w, int h)
{
	//USTALAMY CENTRUM
	double x0, y0;
	setCenter(x0, y0, w, h);

	//ZMIENIAMY KOLOR TLA
	DC->SetBackground(wxBrush(wxColour(*wxWHITE)));
	
	//CZYSCIMY TLO
	DC->Clear();

	//ZMIENIAMY KOLOR NA CZERWONY
	DC->SetPen(wxPen(wxColour(*wxRED)));

	//RYSUJEMY PROSTOKAT OKALAJACY WYKRES
	DC->DrawRectangle(10, 10, w - 20, h - 20);

	//TWORZYMY MACIERZ TRANSFORMACJI
	Matrix transformationM;
	setTransformM(transformationM, x0, y0, w, h);
	
	//USTALAMY KROK
	double step;
	setStep(step);

	//ustawiamy ClippingRegion zeby nie wychodzilo poza granice
	DC->SetClippingRegion(10, 10, w - 20, h - 20);

	//RYSUJEMY OSIE
	drawAxes(DC, transformationM, w, h);
	
	//DEFINIUJE WARTOSCI KTORYCH UZYJE DO NARYSOWANIA OZNACZEN WARTOSCI NA OSIACH
	double v1 = cfg->Get_x_start() / 2;
	double v2 = cfg->Get_x_stop() / 3;
	double v3 = 2 * cfg->Get_x_stop() / 3;
	double v4 = Get_Y_min() / 2;
	double v5 = Get_Y_max() / 3;
	double v6 = 2 * Get_Y_max() / 3;
	
	//ULADNIENIE STRZALEK I DANIE W FUNKCJE
	drawArrows(DC, transformationM, w, h);
	
	//RYSUJEMY WARTOSCI NA OSI X
	drawMarkX(DC, transformationM, v1, w, h);
	drawMarkX(DC, transformationM, v2, w, h);
	drawMarkX(DC, transformationM, v3, w, h);
	
	//RYSUJEMY WARTOSCI NA OSI Y
	drawMarkY(DC, transformationM, v4, w, h);
	drawMarkY(DC, transformationM, v5, w, h);
	drawMarkY(DC, transformationM, v6, w, h);
		
	//RYSOWANIE FUNKCJI
	drawFunction(DC, transformationM, w, h, step);
	
}

//FUNKCJA ZA POMOC¥ KTÓREJ RYSUJEMY WSZYSTKO NA EKRANIE, WSZYSTKO CO MA BYÆ OBJÊTE TRANSFORMACJAMI
void ChartClass::line2d(wxDC* DC,  Matrix transfromationM, double x1, double y1, double x2, double y2,  int w, int h)
{
	//PUNKT (WEKTOR) POCZATKOWY
	Vector init;
	init.Set(x1, y1);
	init = transfromationM * init;

	//PUNKT (WEKTOR) KONCOWY
	Vector extr;
	extr.Set(x2, y2);
	extr = transfromationM * extr;

	//RYSUJEMY
	DC->DrawLine(init.GetX(), init.GetY(), extr.GetX(), extr.GetY());

}

//SKLADAMY MACIERZ TRANSFORMACJI
void ChartClass::setTransformM(Matrix& transformationM, double x0, double y0, int w, int h) {
	//ZA POMOCA¥ INSTRUKCJI SWITCH ZALEZNIE OD TEGO CZY MAMY OBRACAC DOOKOLA SRODKA EKRANU LUB SRODKA UK£ADU, KOLEJNOSC MNOZEN JEST ZAMIENIONA
	transformationM = (cfg->RotateScreenCenter() ? Set_Translate(x0, y0) * Set_Rotation(cfg->Get_Alpha()) * Set_Translate(cfg->Get_dX(), cfg->Get_dY()) * Set_Scale(w, h) : Set_Translate(x0, y0) * Set_Translate(cfg->Get_dX(), cfg->Get_dY()) * Set_Rotation(cfg->Get_Alpha()) * Set_Scale(w, h));
}

//FUNKCJA USTALAJACA SRODEK
void ChartClass::setCenter(double& x0, double& y0, int w, int h) {

	//POBIERAMY WARTOSCI ZA POMOCA FUNKCJI GET_[ELEMENT]
	double actx0 = cfg->Get_x0();
	double actx1 = cfg->Get_x1();
	double acty0 = cfg->Get_y0();
	double acty1 = cfg->Get_y1();

	//DEKLARUJEMY X0 I Y0
	x0 = - (((double)w - 20) / (actx1 - actx0)) * actx0 + 10;
	y0 = - (((double)h - 20) / (acty0 - acty1)) * acty1 + 10;
}

//FUNKCJA RYSUJACA OSIE X I Y
void ChartClass::drawAxes(wxDC* DC, Matrix transformationM, int w, int h) {
	
	//POBIERAMY WARTOSCI ZA POMOC¥ FUNKCJI GET_[ELEMENT]
	double initX = cfg->Get_x_start();
	double extrX = cfg->Get_x_stop();

	double initY = -Get_Y_min();
	double extrY = -Get_Y_max();

	//ZMIENIAMY KOLOR NA NIEBIESKI
	DC->SetPen(wxPen(wxColour(*wxBLUE)));

	//RYSUJEMY OS X
	line2d(DC, transformationM, initX, 0, extrX, 0, w, h);

	//RYSUJEMY OS Y
	line2d(DC, transformationM, 0, initY, 0, extrY, w, h);
}

//FUNKCJA RYSUJACA STRZALKI NA KONCACH OSI
void ChartClass::drawArrows(wxDC* DC, Matrix transformationM, int w, int h) {

	//POBIERAMY WARTOSCI ZA POMOCA FUNKCJI GET_[ELEMENT]
	double x1 = cfg->Get_x_stop();
	double y1 = Get_Y_max();

	//STRZA£KA NA OSI X
	line2d(DC, transformationM, x1, 0, x1 - 0.08, 0.03, w, h);
	line2d(DC, transformationM, x1, 0, x1 - 0.08, -0.03, w, h);
	
	//STRZA£KA NA OSI Y
	line2d(DC, transformationM, 0, -y1, 0.03, -y1 + 0.08, w, h);
	line2d(DC, transformationM, 0, -y1, -0.03, -y1 + 0.08, w, h);
}

//FUNKCJA RYSUJACA WARTOSCI NA OSI X
void ChartClass::drawMarkX(wxDC* DC, Matrix transformationM, double v, int w, int h) {
	
	//PUNKT (WEKTOR) W KTORYM BEDZIE ZNAJDOWAL SIE NAPIS
	Vector vc;
	vc.Set(v, 0.07);

	//POBIERMY WARTOSC KATA
	double alpha = cfg->Get_Alpha();

	//RYSUJEMY LINIE
	line2d(DC, transformationM, v, 0.06, v, -0.06, w, h);

	//RYSUJEMY WARTOSC
	DC->DrawRotatedText(wxString::Format(wxT("%.2f"), v), (transformationM * vc).GetX(), (transformationM * vc).GetY(), alpha);
	//DWA MIEJSCA PO PRZECINKU LICZBY ZMIENNOPRZECINKOWEJ
}

//FUNKCJA RYSUJACA WARTOSCI NA OSI Y
void ChartClass::drawMarkY(wxDC* DC, Matrix transformationM, double v, int w, int h) {
	
	//PUNKT (WEKTOR) W KTORYM BEDZIE ZNAJDOWAL SIE NAPIS
	Vector vc;
	vc.Set(0.04, -v);

	//POBIERMY WARTOSC KATA
	double alpha = cfg->Get_Alpha();

	//RYSUJEMY LINIE
	line2d(DC, transformationM, 0.04, -v, -0.04, -v, w, h);
	
	//RYSUJEMY WARTOSC
	DC->DrawRotatedText(wxString::Format(wxT("%.2f"), v), (transformationM * vc).GetX(), (transformationM * vc).GetY(), alpha);
}

//FUNKCJA RYSUJACA DANA FUNKCJE
void ChartClass::drawFunction(wxDC* DC, Matrix transformationM, int w, int h, double step) {
	
	//ZMIENIAMY KOLOR NA CZERWONY
	DC->SetPen(wxPen(wxColour(*wxGREEN)));

	//RYSUJEMY FUNKCJE "STEP BY STEP"
	for (double x = cfg->Get_x_start(); x <= cfg->Get_x_stop(); x += step) {
		line2d(DC, transformationM, x, -GetFunctionValue(x), x + step, -GetFunctionValue(x + step), w, h);
	}
}

//FUNKCJA USTALAJACA KROK
void ChartClass::setStep(double& step) {

	//POBIERAMY WARTOSCI ZA POMOCA FUNKCJI GET_[ELEMENT]
	double x0 = cfg->Get_x_start();
	double x1 = cfg->Get_x_stop();

	step = (x1 - x0) / static_cast<double>(x_step);
}
