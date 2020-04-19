#include <wx/wx.h>
#include "GUIMyFrame1.h"
#define L 500
// UWAGA: TO JEST JEDYNY PLIK, KTORY NALEZY EDYTOWAC **************************

using namespace std;

void GUIMyFrame1::DrawMap(int N, float d[100][3], bool Contour, int MappingType, int NoLevels, bool ShowPoints) 
{
	wxMemoryDC memDC;
	memDC.SelectObject(MemoryBitmap);
	memDC.SetBackground(*wxWHITE_BRUSH);
	memDC.Clear();


	//NOWA TREŒÆ//////////////////////////////////////////////////////////////////////////////////////////////////

	//W ostatnim zadaniu udalo mi sie odnalezc problemy ktore decydowaly o slabej wydajnosci, tutaj niestety nie//
	//DODATKOWE FUNKCJE Z KTORYCH BEDZIEMY KORZYSTAC//////////////////////////////////////////////////////////////
	void setMinMax(float&, float&, float[][3], int);
	float shepardIP(int, float[][3], float, float);
	void fill(unsigned char*, int, int, float, float, float, float);
	void contour(float**, int, int, float, wxDC*);

	//DEKLARUJEMY WARTOSC MINIMALNA I MAKSYMALNA
	float min = 0;
	float max = 0;

	unsigned char* storedData = new unsigned char[L * L * 3];
	float *pxls[L];
	
	//WYPELNIAMY TABLICE
	for (int i = 0; i < L; i++) {

		pxls[i] = new float[L];

	}

	//ZA POMOCA FUNKCJI USTALAMY MINIMUM I MAKSIMUM
	setMinMax(min, max, d, N);

	//WYPELNIAMY TABLICE KORZYSTAJAC Z FUNKCJI INTERPOLACJI
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {

			pxls[j][i] = shepardIP(N, d, j / 100.0 - 2.5, -i / 100.0 + 2.5);

		}
	}

	//PETLE WYPELNIAJACE TABLICE Z DANYMI
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {

			float colour = ((pxls[i][j] - min) / (max - min) - 0.5) * 2;

			if (MappingType == 1) { //JEZELI KOLORY MAPOWANIA SA NIEBIESKO-CZERWONE
					
				float a = 127 - 127 * colour;
				float b = 0;
				float c = 127 + 127 * colour;

				fill(storedData, i, j, colour, a, b, c);
			}

			else if (MappingType == 2) { //JEZELI KOLORY MAPOWANIA SA NIEBIESKO-ZIELONO-CZERWONE

				float a = 0;

				if (colour > 0) {
					a = 0;
				}
				else if (colour < 0) {
					a = 255 * abs(colour);
				}

				float b = 255 - 255 * abs(colour);
					
				float c = 0;

				if (colour > 0) {
					c = 255 * abs(colour);
				}
				else if (colour < 0) {
					c = 0;
				}
					
				fill(storedData, i, j, colour, a, b, c);
			}

			else if (MappingType == 3) { //JEZELI KOLORY MAPOWANIA SA CZARNO-BIALE

				float a = 127 * colour + 127;
				float b = 127 * colour + 127;
				float c = 127 * colour + 127;

				fill(storedData, i, j, colour, a, b, c);
			}
		}
	}

	if (MappingType) { //JEZELI MAPOWANIE JEST WLACZONE TO RYSUJEMY
		//TWORZYMY WXIMAGE Z DANYCH
		wxImage image(L, L, storedData);
		//ZMIENIAMY W BITMAPE I JA RYSUJEMY
		memDC.DrawBitmap(wxBitmap(image), 0, 0);
	}
	
	if (ShowPoints) { //JEZELI WLACZONE JEST POKAZYWANIE PUNKTOW
		wxPen pen;

		pen.SetColour(0, 0, 0);
		pen.SetWidth(0.5);

		memDC.SetPen(pen);
		//USTAWIAMY KOLOR KRESKI NA CZARNY

		for (int i = 0; i < N; i++) {

			//RYSUJEMY PO DWIE PRZECINAJACE SIE KRESKI W DANYCH PUNKTACH
			memDC.DrawLine((d[i][0] + 2.5) * 100, (2.5 - d[i][1]) * 100 + 4, (d[i][0] + 2.5) * 100, (2.5 - d[i][1]) * 100 - 3);
			memDC.DrawLine((d[i][0] + 2.5) * 100 - 3, (2.5 - d[i][1]) * 100 + 1, (d[i][0] + 2.5) * 100 + 4, (2.5 - d[i][1]) * 100 + 1) ;

		}
	}

	if (Contour) { //JEZELI WLACZONE JEST POKAZYWANIE KONTUR
		wxPen pen;

		pen.SetColour(0,0,0);
		pen.SetWidth(0.5);

		memDC.SetPen(pen);
		//USTAWIAMY KOLOR KRESKI NA CZARNY
	

		for (int i = 0; i < NoLevels; i++) {

			float thPT = min + (i + 1.0) * (max - min) / (NoLevels + 1.0);

			for (int j = 0; j < L; ++j) {
				for (int k = 0; k < L; ++k) {

					contour(pxls, k, j, thPT, &memDC);
				}
			}

		}
	}



}

void setMinMax(float& min, float& max, float array[100][3], int N) { //USTALAMY MINIMUM I MAXIMUM

	for (int x = 0; x < N; x++) {

		array[x][2] < min ? min = array[x][2] : 0;
		array[x][2] > max ? max = array[x][2] : 0;
	
	}

}

float shepardIP(int N, float array[100][3], float x, float y) {//INTERPOLACJA SHEPARDA

	float numerator = 0;
	float denominator = 0;
	float tmp = 0;
	float result = 0;

	for (int i = 0; i < N; i++) {

		tmp = 1.0 / abs((x - array[i][0]) * (x - array[i][0]) + pow((y - array[i][1]), 2));

		numerator += tmp * array[i][2];
		denominator += tmp;

	}

	result = numerator / denominator;
	return result;
}

void fill(unsigned char* data, int x, int y, float color, float a, float b, float c) { //WYPELNIAMY TABLICE DATA

	data[500 * 3 * y + 3 * x + 0] = a;
	data[500 * 3 * y + 3 * x + 1] = b;
	data[500 * 3 * y + 3 * x + 2] = c;

}


void contour(float** array, int k, int j, float thPT, wxDC *memDC) { //RYSUJEMY KONTURY
	if (array[k][j] > thPT) {

		for (int l = -1; l <= 1; l++) {
			for (int m = -1; m <= 1; m++) {

				if (l && m && (k + l >= 0) && (k + l < 500) && (k + m >= 0) && (j + m < 500) && (array[k + l][j + m] < thPT)) {
					memDC->DrawLine(k, j, k + 1, j + 1);
				}

			}
		}
	}
}