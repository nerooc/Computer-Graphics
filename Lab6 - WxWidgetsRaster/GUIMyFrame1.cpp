#include "GUIMyFrame1.h"

//FUNKCJE DOMYŒLNE - WYPE£NIONE///////////////////////////////////////////////////////////////////////////////////////

GUIMyFrame1::GUIMyFrame1(wxWindow* parent)
	:
	MyFrame1(parent)
{
	m_staticText1->SetLabel(_(L"Jasno\u015B\u0107"));
	m_b_threshold->SetLabel(_(L"Pr\u00F3g 128"));
	this->SetBackgroundColour(wxColor(192, 192, 192));
	m_scrolledWindow->SetScrollbars(25, 25, 52, 40);
	m_scrolledWindow->SetBackgroundColour(wxColor(192, 192, 192));
}

void GUIMyFrame1::Repaint()
{
	wxBitmap bitmap(Img_Cpy);          // Tworzymy tymczasowa bitmape na podstawie Img_Cpy
	wxClientDC dc(m_scrolledWindow);   // Pobieramy kontekst okna
	m_scrolledWindow->DoPrepareDC(dc); // Musimy wywolac w przypadku wxScrolledWindow, zeby suwaki prawidlowo dzialaly
	dc.DrawBitmap(bitmap, 0, 0, true); // Rysujemy bitmape na kontekscie urzadzenia
}

void GUIMyFrame1::m_scrolledWindow_update(wxUpdateUIEvent& event)
{
	Repaint();
}

void GUIMyFrame1::m_s_brightness_scroll(wxScrollEvent& event)
{
	// Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
	// Brightness(...), ktora zmienia jasnosc. W tym miejscu nic nie
	// zmieniamy. Do uzupelnienia pozostaje funkcja Brightness(...)
	Brightness(m_s_brightness->GetValue() - 100);
	Repaint();
}

void GUIMyFrame1::m_s_contrast_scroll(wxScrollEvent& event)
{
	// Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
	// Contrast(...), ktora zmienia kontrast. W tym miejscu nic nie
	// zmieniamy. Do uzupelnienia pozostaje funkcja Contrast(...)
	Contrast(m_s_contrast->GetValue() - 100);
	Repaint();
}

//FUNKCJE DO WYPE£NIENIA//////////////////////////////////////////////////////////////////////////////////////////////

void GUIMyFrame1::m_b_grayscale_click(wxCommandEvent& event)
{
	//KONWERTUJEMY OBRAZ DO SKALI SZAROŒCI ZA POMOC¥ METODY KLASY WXIMAGE
	Img_Cpy = Img_Org.Copy().ConvertToGreyscale();
}

void GUIMyFrame1::m_b_blur_click(wxCommandEvent& event)
{
	//ROZMYWAMY OBRAZ ZA POMOC¥ METODY KLASY WXIMAGE
	Img_Cpy = Img_Org.Copy().Blur(5);
	//Z MOICH TESTÓW WYNIKA ZE TO CHYBA 5 JEST DOCELOW¥ WARTOŒCI¥, U¯YT¥ W DEMO
}

void GUIMyFrame1::m_b_mirror_click(wxCommandEvent& event)
{
	//PRZEKSZTA£CAMY OBRAZ W JEGO ODBICIE LUSTRZANE ZA POMOC¥ METODY KLASY WXIMAGE
	Img_Cpy = Img_Org.Copy().Mirror();
}

void GUIMyFrame1::m_b_replace_click(wxCommandEvent& event)
{
	//ZMIENIAMY KOLORY ZA POMOC¥ METODY KLASY WXIMAGE (r1, g1, b1, r2, g2, b2) -> zmienia kolor1 na kolor2
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.Replace(254, 0, 0, 0, 0, 255);
}

void GUIMyFrame1::m_b_rescale_click(wxCommandEvent& event)
{
	//ZMIENIEMY ROZMIAR OBRAZU DO 320x240 
	//MUSIMY ZADBAÆ O TO ¯EBY OBRAZ ORYGINALNEGO ROZMIARU ZNIKN¥£, WIÊC:
	//CZYŒCIMY T£O (USUWAMY AKTUALNY OBRAZEK)
	m_scrolledWindow->ClearBackground();
	//SKALUJEMY ORYGINALNY OBRAZEK
	Img_Cpy = Img_Org.Copy().Rescale(320, 240);
}

void GUIMyFrame1::m_b_rotate_click(wxCommandEvent& event)
{
	//OBRACAMY OBRAZEK O 30 STOPNI ZA POMOC¥ METODY KLASY WXIMAGE
	Img_Cpy = Img_Org.Copy().Rotate(30.0 * M_PI / 180.0, wxPoint(Img_Org.GetSize().x / 2, Img_Org.GetSize().y / 2));
	//NIE JEST WAZNE TO, CO WSTAWIMY DO WXPOINT
	//KAT W RADIANACH
}

void GUIMyFrame1::m_b_rotate_hue_click(wxCommandEvent& event)
{
	//PRZESUWAMY HUE O 180 STOPNI ZA POMOC¥ METODY KLASY WXIMAGE
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.RotateHue(180./360.);
}

void GUIMyFrame1::m_b_mask_click(wxCommandEvent& event)
{
	//USTAWIAMY MASKÊ OBRAZU ZA POMOC¥ METODY KLASY WXIMAGE
	//TWORZYMY CONTEXT Z OKNA NA KTÓRYM RYSUJEMY
	wxClientDC DC(m_scrolledWindow);
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.SetMaskFromImage(Img_Mask, 0, 0, 0);
	DC.Clear();
}


//4 OSTATNIE FUNKCJE ZAIMPLEMENTOWANE OD PODSTAW//////////////////////////////////////////////////////////////////////////////////
//KORZYSTAMY Z METODY wxImage::GetData() BY ZAPEWNIÆ DU¯¥ SZYBKOŒÆ

void GUIMyFrame1::m_b_prewitt_click(wxCommandEvent& event)
{
	//FUNKCJA USTAWIAJACA MASKE PREWITTA
	//DEKLARUJEMY ZMIENNA TYMCZASOWA I INICJUJEMY JA ZEREM
	int tmp = 0;

	//DEKLARUJEMY WIELKOSCI W OSIACH X I Y PONIEWAZ ZCZYTYWANIE NA BIEZACO STRASZNIE SPOWALNIA PROGRAM
	int X = Img_Cpy.GetSize().x;
	int Y = Img_Cpy.GetSize().y;

	//TWORZYMY KOPIÊ
	Img_Cpy = Img_Org.Copy();

	//ZAPISUJEMY DANE KOPII ZA POMOC¥ METODY GETDATA
	unsigned char* copy = Img_Cpy.GetData();

	//ZAPISUJEMY DANE ORYGINA£U ZA POMOC¥ METODY GETDATA
	unsigned char* initial = Img_Org.GetData();

	//NIE JESTEM PEWNY ALE MYSLE ZE MOZNA UZYWAC GetSize().y GetSize().x JAK I GetHeigth() GetWidth() ZAMIENNIE
	for (int i = 1; i < Y - 1; i++) 
	{

		for (int j = 1; j < X - 1; j++) 
		{

			for (int x = 0; x < 3; x++) 
			{

				tmp = 0;

				tmp = initial[i * X * 3 + (j + 1) * 3 + x] 
					- initial[i * X * 3 + (j - 1) * 3 + x] 
					+ initial[(i + 1) * X * 3 + (j + 1) * 3 + x] 
					- initial[(i + 1) * X * 3 + (j - 1) * 3 + x] 
					+ initial[(i - 1) * X * 3 + (j + 1) * 3 + x] 
					- initial[(i - 1) * X * 3 + (j - 1) * 3 + x];

				if (tmp > 255)
				{
					copy[i * X * 3 + j * 3 + x] = 255;
				}

				else
				{
					copy[i * X * 3 + j * 3 + x] = fabs(tmp) / 3.0;
				}

			}
		}
	}
} 

//FUNKCJA USTANAWIAJACA PROG O WARTOSCI 128 DLA KAZDEGO KANALU
void GUIMyFrame1::m_b_threshold_click(wxCommandEvent& event)
{
	//TWORZYMY KOPIÊ
	Img_Cpy = Img_Org.Copy();

	//ZAPISUJEMY DANE KOPII ZA POMOC¥ METODY GETDATA
	unsigned char* copy = Img_Cpy.GetData();

	//TWORZYMY ZMIENNA ODPOWIADAJACA ZA PROG
	int threshold = 128;
		
	//DEKLARUJEMY ZMIENNA GETSIZE KTORA BEDZIE WIELKOSCIA CALKOWITA, KTOREJ UZYJEMY POTEM W PETLI DLA ZAOSZCZEDZENIA CZASU DZIALANIA
	int getSize = Img_Cpy.GetSize().x * Img_Cpy.GetSize().y * 3;

	//for (int i = 1; i < Img_Cpy.GetSize().x * Img_Cpy.GetSize().y * 3; i++) -> uzywana wczesniej petla ktora okazala sie najwiekszym problemem wydajnosciowym

	for (int i = 0; i < getSize; i++)
	{
		if (copy[i] < threshold) 
		{
			copy[i] = 0;
		}
		
		else 
		{
			copy[i] = 255;
		}
	}
}

void GUIMyFrame1::Contrast(int value)
{
	//FUNKCJA ZMIENIAJACA KONTRAST OBRAZU W ZALEZNOSCI OD WARTOSCI NA SLIDERZE
	//DEKLARUJEMY ZMIENNA TYMCZASOWA I INICJUJEMY JA ZEREM
	double tmp = 0;

	//KOREKCJA KONTRASTU
	double x = (100.0 + value) / (101.0 - value);

	//TWORZYMY KOPIÊ
	Img_Cpy = Img_Org.Copy();

	//ZAPISUJEMY DANE KOPII ZA POMOCA METODY GETDATA
	unsigned char* copy = Img_Cpy.GetData();

	//DEKLARUJEMY ZMIENNA GETSIZE KTORA BEDZIE WIELKOSCIA CALKOWITA, KTOREJ UZYJEMY POTEM W PETLI DLA ZAOSZCZEDZENIA CZASU DZIALANIA
	int getSize = Img_Cpy.GetSize().x * Img_Cpy.GetSize().y * 3;

	//for (int i = 1; i < Img_Cpy.GetSize().x * Img_Cpy.GetSize().y * 3; i++) -> uzywana wczesniej petla ktora okazala sie najwiekszym problemem wydajnosciowym

	for (int i = 0; i < getSize; i++)
	{
		tmp = x * (copy[i] - 127) + 127;

		if (tmp > 255) 
		{
			copy[i] = 255;
		}
		
		else if(tmp < 0)
		{
			copy[i] = 0;
		}
		
		else
		{
			copy[i] = tmp;
		}
	
	}

}

void GUIMyFrame1::Brightness(int value)
{
	//FUNKCJA ZMIENIAJACA JASNOSC OBRAZU W ZALEZNOSCI OD WARTOSCI NA SLIDERZE
	//DEKLARUJEMY ZMIENNA TYMCZASOWA I INICJUJEMY JA ZEREM
	int tmp = 0;

	//TWORZYMY KOPIE
	Img_Cpy = Img_Org.Copy();

	//ZAPISUJEMY DANE KOPII ZA POMOCA METODY GETDATA
	unsigned char* copy = Img_Cpy.GetData();
	
	//DEKLARUJEMY ZMIENNA GETSIZE KTORA BEDZIE WIELKOSCIA CALKOWITA, KTOREJ UZYJEMY POTEM W PETLI DLA ZAOSZCZEDZENIA CZASU DZIALANIA
	int getSize = Img_Cpy.GetSize().x * Img_Cpy.GetSize().y * 3;

	//for (int i = 1; i < Img_Cpy.GetSize().x * Img_Cpy.GetSize().y * 3; i++) -> uzywana wczesniej petla ktora okazala sie najwiekszym problemem wydajnosciowym

	for (int i = 1; i < getSize; i++)
	{
		tmp = copy[i] + value;
		
		if (tmp > 255) 
		{
			copy[i] = 255;
		}
		
		else if (tmp < 0) 
		{
			copy[i] = 0;
		}

		else 
		{
			copy[i] = tmp;
		}
	}
}

