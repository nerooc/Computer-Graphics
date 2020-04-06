#include "GUIMyFrame1.h"
#include <vector>
#include <fstream>
#include "vecmat.h"

#define PI 3.14159265359

////////////////////////////////////MACIERZE TRANSFORMACJI/////////////////////////////////////////////

//FUNKCJA SKALUJACA
//MACIERZ SKALUJACA:
//
//	                    _        _				
//	                   | Sx 0 0 0 |
//		  	 		   | 0 Sy 0 0 |
// Scale(Sx, Sy, Sz) = | 0 0 Sz 0 |
//	                   | 0 0 0  1 |
//                     |_        _|

void Set_Scale(Matrix4* scaleM, double Sx, double Sy, double Sz) {

	scaleM->data[0][0] = Sx;
	scaleM->data[1][1] = Sy;
	scaleM->data[2][2] = Sz;

	//scaleM->data[3][3] = 1.0; ustawiony automatycznie w konstruktorze
}


//FUNKCJE OBRACAJACE
//MACIERZ ROTACJI X:
//	              ___					    ___	
//	             | 1	  0			  0		  0 |
//               | 0  cos(alpha) -sin(alpha)  0 |
// RotX(alpha) = | 0  sin(alpha)  cos(alpha)  0 |
//               | 0      0           0       1 |
//               |___                        ___|

void Set_RotationX(Matrix4* rotateXM, double alphaX) {

	rotateXM->data[0][0] = 1.0;

	rotateXM->data[1][1] = rotateXM->data[2][2] = cos(alphaX * PI / 180);
	rotateXM->data[1][2] = -sin(alphaX * PI / 180);
	rotateXM->data[2][1] = sin(alphaX * PI / 180);


	//rotateXM->data[3][3] = 1.0; ustawiony automatycznie w konstruktorze 

}

//MACIERZ ROTACJI Y:
//	              ___					               ___	
//	             |	cos(alpha)	  0		   sin(alpha)   0 |
//               |		0		  1            0        0 |
// RotY(alpha) = | -sin(alpha)    0        cos(alpha)   0 |
//               |		0         0            0        1 |
//               |___                                  ___|

void Set_RotationY(Matrix4* rotateYM, double alphaY) {

	rotateYM->data[0][0] = rotateYM->data[2][2] = cos(-alphaY * PI / 180);
	rotateYM->data[0][2] = sin(-alphaY * PI / 180);

	rotateYM->data[1][1] = 1.0;

	rotateYM->data[2][0] = -sin(-alphaY * PI / 180);

	//rotateYM->data[3][3] = 1.0; ustawiony automatycznie w konstruktorze 
}

//MACIERZ ROTACJI Z:
//	              ___					               ___	
//	             |	cos(alpha)	  -sin(alpha)  0        0 |
//               |  sin(alpha)	   cos(alpha)  0        0 |
// RotZ(alpha) = |      0			   0       1        0 |
//               |		0              0       0        1 |
//               |___                                  ___|

void Set_RotationZ(Matrix4* rotateZM, double alphaZ) {

	rotateZM->data[0][0] = rotateZM->data[1][1] = cos(alphaZ * PI / 180);
	rotateZM->data[0][1] = -sin(alphaZ * PI / 180);
	rotateZM->data[1][0] = sin(alphaZ * PI / 180);

	rotateZM->data[2][2] = 1.0;
	//rotateZM->data[3][3] = 1.0; ustawiony automatycznie w konstruktorze 
}

//MACIERZ TRANSLACJI:
//	                _  	    _	
//				   | 1 0 0 a |
//	               | 0 1 0 b |
// Tran(a, b, c) = | 0 0 1 c |
//                 | 0 0 0 1 | 
//	               |_       _|

void Set_Translate(Matrix4* translateM, double a, double b, double c) {

	translateM->data[0][0] = translateM->data[1][1] = translateM->data[2][2] = 1.0;

	//translateM->data[3][3] = 1.0; ustawiony automatycznie w konstruktorze 

	translateM->data[0][3] = a;
	translateM->data[1][3] = -b; //BO CHCEMY BY PORUSZAL SIE W DRUGA STRONE
	translateM->data[2][3] = c;
}

//MACIERZ CENTRUM:
//JEST TO SPECJALNA MACIERZ TRANSLACJI KTÓRA S£U¯Y WYCENTROWANIU OBRAZU 3D
//	       _  	          _	
//		  | 1   0   0  0.5 |
//	      | 0   1   0  0.5 |
// Cent = | 0   0   1   0  |
//        | 0   0   0   1  | 
//	      |_              _|

void Set_Center(Matrix4* centerM) {

	centerM->data[0][0] = centerM->data[1][1] = centerM->data[2][2] = 1;

	centerM->data[0][3] = centerM->data[1][3] = 0.5;

	centerM->data[2][3] = 0;

	//centerM->data[3][3] = 1; ustawiony automatycznie w konstruktorze

}


struct Point {
	float x, y, z;
	Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct Color {
	int R, G, B;
	Color(int _R, int _G, int _B) : R(_R), G(_G), B(_B) {}
};

struct Segment {
	Point begin, end;
	Color color;
	Segment(Point _begin, Point _end, Color _color) : begin(_begin), end(_end), color(_color) {}
};

std::vector<Segment> data;

GUIMyFrame1::GUIMyFrame1(wxWindow* parent)
	:
	MyFrame1(parent)
{
	m_button_load_geometry->SetLabel(_("Wczytaj Geometri\u0119"));
	m_staticText25->SetLabel(_("Obr\u00F3t X:"));
	m_staticText27->SetLabel(_("Obr\u00F3t Y:"));
	m_staticText29->SetLabel(_("Obr\u00F3t Z:"));

	WxSB_TranslationX->SetRange(0, 200); WxSB_TranslationX->SetValue(100);
	WxSB_TranslationY->SetRange(0, 200); WxSB_TranslationY->SetValue(100);
	WxSB_TranslationZ->SetRange(0, 200); WxSB_TranslationZ->SetValue(100);

	WxSB_RotateX->SetRange(0, 360); WxSB_RotateX->SetValue(0);
	WxSB_RotateY->SetRange(0, 360); WxSB_RotateY->SetValue(0);
	WxSB_RotateZ->SetRange(0, 360); WxSB_RotateZ->SetValue(0);

	WxSB_ScaleX->SetRange(1, 200); WxSB_ScaleX->SetValue(100);
	WxSB_ScaleY->SetRange(1, 200); WxSB_ScaleY->SetValue(100);
	WxSB_ScaleZ->SetRange(1, 200); WxSB_ScaleZ->SetValue(100);
}

void GUIMyFrame1::WxPanel_Repaint(wxUpdateUIEvent& event)
{
	Repaint();
}

void GUIMyFrame1::m_button_load_geometry_click(wxCommandEvent& event)
{
	wxFileDialog WxOpenFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("Geometry file (*.geo)|*.geo"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (WxOpenFileDialog.ShowModal() == wxID_OK)
	{
		double x1, y1, z1, x2, y2, z2;
		int r, g, b;

		std::ifstream in(WxOpenFileDialog.GetPath().ToAscii());
		if (in.is_open())
		{
			data.clear();
			while (!in.eof())
			{
				in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> r >> g >> b;
				data.push_back(Segment(Point(x1, y1, z1), Point(x2, y2, z2), Color(r, g, b)));
			}
			in.close();
		}
	}
}

void GUIMyFrame1::Scrolls_Updated(wxScrollEvent& event)
{
	WxST_TranslationX->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationX->GetValue() - 100) / 50.0));
	WxST_TranslationY->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationY->GetValue() - 100) / 50.0));
	WxST_TranslationZ->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationZ->GetValue() - 100) / 50.0));

	WxST_RotateX->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateX->GetValue()));
	WxST_RotateY->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateY->GetValue()));
	WxST_RotateZ->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateZ->GetValue()));

	WxST_ScaleX->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleX->GetValue() / 100.0));
	WxST_ScaleY->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleY->GetValue() / 100.0));
	WxST_ScaleZ->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleZ->GetValue() / 100.0));


	Repaint();
}



//FUNKCJA RYSUJ¥CA POJEDYNCZY WEKTOR KORZYSTAJ¥C Z DANYCH ZAWARTYCH W TABLICY DATA
void drawVector(Matrix4 transformM, Matrix4 centerM, float w, float h, int i, wxBufferedDC& DC) {
	
	//POBIERAMY WARTOŒCI R G i B DO ZMIENNYCH
	float R = data[i].color.R;
	float G = data[i].color.G;
	float B = data[i].color.B;

	//USTAWIAMY KOLOR RGB NA PODSTAWIE DANYCH ZAWARTYCH W TABLICY DATA
	DC.SetPen(wxPen(RGB(R, G, B))); 

	//DEKLARUJEMY WEKTOR POCZ¥TKOWY I KOÑCOWY
	Vector4 initVec; Vector4 termVec;
	
	//WEKTOR INIT WYPE£NIAMY WARTOŒCIAMI ZAWARTYMI W TABLICY DATA
	initVec.Set(data[i].begin.x, data[i].begin.y, data[i].begin.z);

	//WEKTOR TERM WYPE£NIAMY WARTOŒCIAMI ZAWARTYMI W TABLICY DATA
	termVec.Set(data[i].end.x, data[i].end.y, data[i].end.z);

	//TRANSFORMUJEMY WEKTOR INIT ZAMIENIAJAC GO W TRANSFORMED INIT POPRZEZ POMNOZENIE PRZEZ MACIERZ TRANSFORMACJI
	Vector4 transformedInitVec = transformM * initVec;

	//POBIERAMY WARTOŒCI PO TRANSFORMACJI
	float initVecX = transformedInitVec.GetX();
	float initVecY = transformedInitVec.GetY();
	float initVecZ = transformedInitVec.GetZ();

	//USTAWIAMY JE PONOWNIE TWORZ¥C PERSPEKTYWÊ
	transformedInitVec.Set(initVecX / initVecZ, initVecY / initVecZ, initVecZ);
		
	//KORZYSTAJ¥C ZE SPECJALNEJ MACIERZY TRANSLACJI CENTRUJEMY RYSUNEK
	transformedInitVec = centerM * transformedInitVec;

	//TRANSFORMUJEMY WEKTOR TERM ZAMIENIAJAC GO W TRANSFORMED TERM POPRZEZ POMNOZENIE PRZEZ MACIERZ TRANSFORMACJI
	Vector4 transformedTermVec = transformM * termVec;

	//POBIERAMY WARTOŒCI PO TRANSFORMACJI
	float termVecX = transformedTermVec.GetX();
	float termVecY = transformedTermVec.GetY();
	float termVecZ = transformedTermVec.GetZ();

	//USTAWIAMY JE PONOWNIE TWORZ¥C PERSPEKTYWÊ
	transformedTermVec.Set(termVecX / termVecZ, termVecY / termVecZ, termVecZ);

	//KORZYSTAJ¥C ZE SPECJALNEJ MACIERZY TRANSLACJI CENTRUJEMY RYSUNEK
	transformedTermVec = centerM * transformedTermVec;

	//POBIERAMY WARTOŒCI PO TRANSFORMACJI I CENTROWANIU
	float initCentVecX = transformedInitVec.GetX();
	float initCentVecY = transformedInitVec.GetY();
	float termCentVecX = transformedTermVec.GetX();
	float termCentVecY = transformedTermVec.GetY();

	//RYSUJEMY DANY ELEMENT RYSUNKU
	DC.DrawLine(initCentVecX * w, initCentVecY * h, termCentVecX * w, termCentVecY * h);
}

//FUNKCJA S£U¯¥CA DO RYSOWANIA 
void GUIMyFrame1::Repaint()
{
	//TWORZYMY NOWY DC CLIENT
	wxClientDC* ClientDC = new wxClientDC(WxPanel);
	wxBufferedDC DC(ClientDC);

	//ZMIENIAMY KOLOR TLA
	DC.SetBackground(wxColour(*wxWHITE));

	//POBIERAMY WARTOŒCI KTÓRYCH CHCEMY U¯YÆ ZA POMOC¥ FUNKCJI GET
	float alphaX = WxSB_RotateX->GetValue();
	float alphaY = WxSB_RotateY->GetValue();
	float alphaZ = WxSB_RotateZ->GetValue() + 180;

	float moveX = WxSB_TranslationX->GetValue();
	float moveY = WxSB_TranslationY->GetValue();
	float moveZ = WxSB_TranslationZ->GetValue();

	float scaleX = WxSB_ScaleX->GetValue();
	float scaleY = WxSB_ScaleY->GetValue();
	float scaleZ = WxSB_ScaleZ->GetValue();

	//DEFINIUJEMY WYSOKOŒÆ I SZEROKOŒÆ PANELU NA KTÓRYM WYŒWIETLIMY MODEL
	int width, height;
	
	//POBIERAMY WYSOKOŒÆ I SZEROKOŒÆ DO ZMIENNYCH ZA POMOC¥ FUNKCJI GET
	WxPanel->GetSize(&width, &height);

	//USTAWIAMY CLIPPING REGION NA WIELKOŒÆ PANELU BY ZAPOBIEC RYSOWANIU POZA NIM
	DC.SetClippingRegion(wxRect(0, 0, width, height));

	//CZYSCIMY TLO
	DC.Clear();

	//MACIERZ SKALI
	Matrix4 scaleM;

	//MACIERZ ROTACJI
	Matrix4 rotationM;

	//MACIERZ ROTACJI OSI X
	Matrix4 rotationXM;

	//MACIERZ ROTACJI OSI Y
	Matrix4 rotationYM;

	//MACIERZ ROTACJI OSI Z
	Matrix4 rotationZM;

	//MACIERZ TRANSLACJI
	Matrix4 translationM;

	//OSOBNA fnkcja
	Matrix4 centerM;

	//WIELKOŒÆ TABLICY DATA
	int dataLength = data.size();

	//WYPE£NIAMY MACIERZ CENTRUJ¥C¥
	Set_Center(&centerM);
	
	//USTAWIAMY SKALÊ
	Set_Scale(&scaleM, scaleX / 100.0, scaleY / 100.0, scaleZ / 100.0);

	//USTAWIAMY ROTACJÊ OSI X
	Set_RotationX(&rotationXM, alphaX);

	//USTAWIAMY ROTACJÊ OSI X
	Set_RotationY(&rotationYM, alphaY);

	//USTAWIAMY ROTACJÊ OSI X
	Set_RotationZ(&rotationZM, alphaZ);

	//OTRZYMUJEMY WYNIKOW¥ MACIERZ ROTACJI
	rotationM = rotationXM * rotationYM * rotationZM;
	
	//USTAWIAMY MACIERZ TRANSLACJI
	Set_Translate(&translationM, (moveX - 100.0) / 50.0, (moveY - 100.0) / 50.0, (moveZ - 100.0) / 50.0 + 2.0);

	//OTRZYMUJEMY MACIERZ CA£KOWITEJ TRANSFORMACJI POPRZEZ MNO¯ENIE PRZEZ SIEBIE POSZCZEGÓLNYCH MACIERZY
	Matrix4 transformM = translationM * rotationM * scaleM;

	//RYSUJEMY ELEMENT PO ELEMENCIE ZA POMOC¥ PÊTLI PO TABLICY DATA
	for (int i = 0; i < dataLength; ++i)
	{
		//KORZYSTAMY Z FUNKCJI RYSUJ¥CEJ ELEMENT
		drawVector(transformM, centerM, width, height, i, DC);
	}
}


