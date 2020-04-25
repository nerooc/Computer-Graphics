#include "GUIMainWindow.h"

GUIMainWindow::GUIMainWindow(wxWindow* parent)
	:
	MainWindow(parent)
{
	//INICJALIZUJEMY OBRAZEK O ROZMIARACH NASZEGO TESTOWEGO
	initImage = wxImage(800, 533);

	//KOPIUJEMY OBRAZEK
	copyImage = initImage.Copy();

	//WLACZAMY HANDLERY WSZYSTKICH TYPOW PLIKOW
	wxInitAllImageHandlers();

	//USTAWIAMY FLAGE ANIMATE NA FALSZ NA POCZATEK, JAKO ZE CHECKBOX NIE JEST ZAZNACZONY
	ANIMATE = false;
}

void GUIMainWindow::windowOnSize(wxSizeEvent& event)
{
	//UPDATE WARTOSCI WYSOKOSCI I SZEROKOSCI W RAZIE ZMIANY WIELKOSCI OKNA
	HEIGHT = event.GetSize().GetHeight();
	WIDTH = event.GetSize().GetWidth();

	//RYSOWANIE NOWYCH ROZMIAROW
	draw();
}

void GUIMainWindow::windowUIUpdated(wxUpdateUIEvent& event)
{
	//W RAZIE UPDATE'U RYSUJE PONOWNIE
	draw();
}

//FUNKCJA WCZYTUJACA OBRAZEK
void GUIMainWindow::m_loadImage(wxCommandEvent& event) {

	//KORZYSTAMY Z WXFILEDIALOG BY ULATWIC OTWIERANIE PLIKU
	wxFileDialog WxOpenFileDialog(this, wxT("Wybierz obraz:"), wxT(""), wxT(""), wxT("JPG file (*.jpg)|*.jpg"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (WxOpenFileDialog.ShowModal()) {

		//W RAZIE BLEDU POKAZUJEMY ERROR W MESSAGEBOX'IE
		if (initImage.LoadFile(WxOpenFileDialog.GetPath())) {

			//WSKAZNIK OBSLUGUJACY METADATA W FREEIMAGE
			FITAG* fitag = NULL;

			//WCZYTUJEMY PLIK DO INITIMAGE
			initImage.LoadFile(WxOpenFileDialog.GetPath());

			//WCZYTUJEMY PLIK DO BITMAPY
			bitmap = FreeImage_Load(FIF_JPEG, WxOpenFileDialog.GetPath(), JPEG_DEFAULT);

			//INICJALIZUJEMY STRING KTORY UZYJEMY JAKO POJEMNIK NA CALY TEKST INFORMACJI EXIF, KTORE WYPISZEMY W m_text
			std::string EXIFinformation("");

			//CALA OBSLUGE TAGOW JESTESMY W STANIE WYKONAC DZIEKI BILBIOTECE FREEIMAGE
			//WPISUJEMY DO STRINGA WLASCIWOSCI KTORE CHCEMY BY POJAWILY SIE W RICHTEXT'cie:

			//ROZMIAR
			EXIFinformation += std::string("Rozmiar obrazka: ") + std::to_string(FreeImage_GetWidth(bitmap)) + "*" + std::to_string(FreeImage_GetHeight(bitmap)) + "\nEXIF Info:\n";

			//POBIERAMY METADATE ARTIST DO fitag
			FreeImage_GetMetadata(FIMD_EXIF_MAIN, bitmap, "Artist", &fitag);

			//ZAPISUJEMY DO STRINGA ARTIST ZA POMOCA KONKATENACJI ORAZ FUNCKJI FreeImage_TagToString 
			EXIFinformation += std::string("Artist: ") + std::string((FreeImage_TagToString(FIMD_EXIF_MAIN, fitag))) + std::string("\n");

			//POBIERAMY METADATE COPYRIGHT DO fitag
			FreeImage_GetMetadata(FIMD_EXIF_MAIN, bitmap, "Copyright", &fitag);

			//ZAPISUJEMY DO STRINGA COPYRIGHT ZA POMOCA KONKATENACJI ORAZ FUNCKJI FreeImage_TagToString 
			EXIFinformation += std::string("Copyright: ") + FreeImage_TagToString(FIMD_EXIF_MAIN, fitag) + std::string("\n");

			//POBIERAMY METADATE RESOLUTIONUNIT DO fitag
			FreeImage_GetMetadata(FIMD_EXIF_MAIN, bitmap, "ResolutionUnit", &fitag);

			//ZAPISUJEMY DO STRINGA RESOLUTIONUNIT ZA POMOCA KONKATENACJI ORAZ FUNCKJI FreeImage_TagToString 
			EXIFinformation += std::string("ResolutionUnit: ") + FreeImage_TagToString(FIMD_EXIF_MAIN, fitag) + std::string("\n");

			//POBIERAMY METADATE XRESOLUTION DO fitag
			FreeImage_GetMetadata(FIMD_EXIF_MAIN, bitmap, "XResolution", &fitag);

			//ZAPISUJEMY DO STRINGA XRESOLUTION ZA POMOCA KONKATENACJI ORAZ FUNCKJI FreeImage_TagToString 
			EXIFinformation += std::string("XResolution: ") + FreeImage_TagToString(FIMD_EXIF_MAIN, fitag) + std::string("\n");

			//POBIERAMY METADATE YCBCRPOSITIONING DO fitag
			FreeImage_GetMetadata(FIMD_EXIF_MAIN, bitmap, "YCbCrPositioning", &fitag);

			//ZAPISUJEMY DO STRINGA YCBCRPOSITIONING ZA POMOCA KONKATENACJI ORAZ FUNCKJI FreeImage_TagToString 
			EXIFinformation += std::string("YCbCrPositioning: ") + FreeImage_TagToString(FIMD_EXIF_MAIN, fitag) + std::string("\n");

			//POBIERAMY METADATE YRESOLUTION DO fitag
			FreeImage_GetMetadata(FIMD_EXIF_MAIN, bitmap, "YResolution", &fitag);

			//ZAPISUJEMY DO STRINGA YRESOLUTION ZA POMOCA KONKATENACJI ORAZ FUNCKJI FreeImage_TagToString 
			EXIFinformation += std::string("YResolution: ") + FreeImage_TagToString(FIMD_EXIF_MAIN, fitag) + std::string("\n");

			//ZWALNIAMY bitmap
			FreeImage_Unload(bitmap);

			//DOPISUJEMY DO RICHTEXT'U ZAWARTOSC STRINGA EXIFinformation
			m_text->AppendText(EXIFinformation);

			//ZAPISUJEMY DO KOPII
			copyImage = initImage.Copy();

			//RYSUJEMY JESZCZE RAZ
			draw();


		} else {

			//BLAD W RAZIE NIEMOZNOSCI WCZYTANIA PLIKU
			wxMessageBox(_("ERROR - File could not be loaded"));

		}
	}
}

//FUNKCJA CENZURUJACA GLOWE STRUSIA
void GUIMainWindow::m_censore(wxCommandEvent& event) {

	//INICJALIZUJEMY WYSOKOSC I SZEROKOSC NASZEGO ZDJECIA 
	int w = copyImage.GetWidth();
	int h = copyImage.GetHeight();

	//INICJALIZUJEMY OBIEKT CImg Z BIBLIOTEKI CIMG W KTORY BEDZIEMY ZAMIENIAC NASZE ZDJECIE BY ZBLUR'OWAC GLOWE STRUSIA
	cimg_library::CImg<unsigned char> cImage(w, h, 1, 3);

	//"PRZEPISUJEMY" ZDJECIE DO CIMG
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {

			cImage(x, y, 0) = initImage.GetRed(x, y);
			cImage(x, y, 1) = initImage.GetGreen(x, y);
			cImage(x, y, 2) = initImage.GetBlue(x, y);

		}
	}

	//KORZYSTAMY Z METODY BLUR Z BIBLIOTEKI CIMAGE
	cImage.blur(3);

	//RESETUJEMY NASZA KOPIE
	copyImage = initImage.Copy();

	//W MIEJSCU GLOWY STRUSIA KORZYSTAMY Z ZAPISANEGO cImage Z BLUREM I USTAWIAMY JE, BY W NASZYM OBRAZIE POZSOTAL BLUR
	for (int x = 460; x < 570; x++) {
		for (int y = 20; y < 100; y++) {

			copyImage.SetRGB(x, y, cImage(x, y, 0), cImage(x, y, 1), cImage(x, y, 2));

		}
	}

	//RYSUJEMY PONOWNIE NASZ OBRAZEK
	draw();
}

//FUNKCJA OBSLUGUJACA METODE ERODE BIBLIOTEKI CIMG DLA NASZEGO OBRAZKA
void GUIMainWindow::m_erode(wxCommandEvent& event)
{
	//INICJALIZUJEMY OBIEKT CImg Z BIBLIOTEKI CIMG W KTORY BEDZIEMY ZAMIENIAC NASZE ZDJECIE BY WYKONAC METODE ERODE
	cimg_library::CImg<unsigned char> cImage(initImage.GetWidth(), initImage.GetHeight(), 1, 3);

	//INICJALIZUJEMY WYSOKOSC I SZEROKOSC NASZEGO ZDJECIA 
	int w = copyImage.GetWidth();
	int h = copyImage.GetHeight();

	//"PRZEPISUJEMY" ZDJECIE DO CIMG
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {

			cImage(x, y, 0) = initImage.GetRed(x, y);
			cImage(x, y, 1) = initImage.GetGreen(x, y);
			cImage(x, y, 2) = initImage.GetBlue(x, y);

		}
	}

	//KORZYSTAMY Z METODY ERODE Z BIBLIOTEKI CIMAGE
	cImage.erode(5);

	//RESETUJEMY NASZA KOPIE
	copyImage = initImage.Copy();

	//NA CALEJ POWIERZCHNI OBRAZU USTAWIAMY OBRAZ Z CIMG BYSMY MOGLI WIDZIEC OBRAZ Z FUNKCJA ERODE
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {

			copyImage.SetRGB(i, j, cImage(i, j, 0), cImage(i, j, 1), cImage(i, j, 2));

		}
	}

	//RYSUJEMY PONOWNIE NASZ OBRAZEK
	draw();
}

//TUTAJ POWINNA BYC FUNKCJA KTORA TWORZY ANIMACJE Z GAUSSEM
void GUIMainWindow::m_animate(wxCommandEvent& event)
{
	ANIMATE = true;
	if (ANIMATE) {
		wxMessageBox(wxT("*zamykamy oczy i wyobrazamy sobie ze animacja dziala* :)"));
	}
}

//FUNKCJA KTORA RYSUJE OBRAZEK NA EKRANIE
void GUIMainWindow::draw(){

	//USTALAMY WIELKOSC PANELU
	m_panel1->SetSize(WIDTH - 300, HEIGHT - 38);

	//TWORZYMY KONTEKST WXCLIENT DLA PANELU, BO TO NA NIM BEDZIEMY RYSOWAC
	wxClientDC wxDC(m_panel1);

	//TWORZYMY OBIEKT KLASY WXIMAGE BY ZMIENIAC ROZMIAR WCZYTANEGO OBRAZKA W MOMENCIE ZMIANY WIELKOSCI OKNA PROGRAMU
	wxImage IMG_SCALED = copyImage.Scale(WIDTH - 300, HEIGHT - 38);

	//TWORZYMY BITMAPE KTORA BEDZIEMY MOGLI NARYSOWAC NA wxDC
	wxBitmap BITMAP(IMG_SCALED);

	//RYSUJEMY STWORZONA BITMAPE NA PANELU
	wxDC.DrawBitmap(BITMAP, 0, 0, true);
}

