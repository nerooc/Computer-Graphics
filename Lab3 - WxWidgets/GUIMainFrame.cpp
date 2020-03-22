#include "GUIMainFrame.h"

GUIMainFrame::GUIMainFrame( wxWindow* parent )
:
MainFrame( parent )
{
	//ustalamy wartości współrzędnych środka panelu na którym będziemy rysować
	dPx = drawingPanel->GetSize().x / 2;
	dPy = drawingPanel->GetSize().y / 2;

	//idziemy na łatwizne i wywołujemy wszystkie handlery zammiast tych potrzebnych :P
	wxInitAllImageHandlers();

	//ustawiamy pozycję scrolla na 0
	_position = 0;

	//inicjujemy wygląd startowy scrollbara
	handSlider->SetScrollbar(_position, 0, 100, 0, 1);

	//zaznaczamy ze na wstepie banan nie jest widoczny (i inne funkcje powiazane), jest to flaga
	_bananaActive = false;

	//scrollbar jest na wstępie wyłączony
	handSlider->Disable();

	//ustalamy wielkość obrazka banana
	wxImage bananaImg = wxImage(wxSize(60, 40));

	//ładujemy obrazek z png
	bananaImg.LoadFile("banan.png", wxBITMAP_TYPE_PNG);

	//zamieniamy w bitmape
	_banana = wxBitmap(bananaImg);

	//na wszelki wypadek na wstepie zmieniamy na gwiazdke
	symbolChooser->SetSelection(0); 

	//inicjujemy wybrany symbol wartoscia poczatkowa - gwiazdka
	_chosenSymbol = "gwiazdka";

	//tekst ktory bedzie wypisywany po obu stronach malpki, na wstepie "tekst"
	_text = "tekst";

}

//funkcja update'ujaca rysunek gdy nastapi zmiana wielkosci panelu
void GUIMainFrame::funcOnSizeUpdate( wxSizeEvent& event )
{
	wxClientDC MyDC(drawingPanel);
	paintOnPanel(MyDC);
}

//obsługa odświeżania panelu
void GUIMainFrame::funcOnUpUI( wxUpdateUIEvent& event )
{
	wxClientDC MyDC(drawingPanel);
	paintOnPanel(MyDC);
}

//funkcja po wcisnieciu przycisku zapisuje obrazek na panelu do pliku korzystajac z okna dialogowego
void GUIMainFrame::funcSaveImage( wxCommandEvent& event )
{
	wxFileDialog saveToFileDialog(this, ("Choose a file"), "", "", "All files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	
	//jesli ktos sie rozmysli to canceluje operacje
	if (saveToFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}

	wxFileOutputStream output_stream(saveToFileDialog.GetPath());

	//petla warunkowa sprawdzajaca czy plik moze zostac zapisany w danej lokalizacji
	if (!output_stream.IsOk()){
		wxLogError("Nie mozna zapisac pliku do '%s'.", saveToFileDialog.GetPath());
		return;
	}

	//zmieniamy bitmape w wxImage i zapisujemy do lokalizacji uzyskanej w oknie dialogowym
	_panelImage = _panelPicture.ConvertToImage();
	_panelImage.SaveFile(saveToFileDialog.GetPath());
}

//funkcja checkboxa ktora wyswietla obrazek banana i wywoluje dodatkowe funkcje
void GUIMainFrame::funcShowBanan( wxCommandEvent& event )
{
	//checbox decyduje o tym czy flaga powiazana z bananem jest true lub false
	_bananaActive = event.IsChecked();
	//wlaczamy scrollbara jezeli flaga powiazana z bananem jest true i wylaczamy jesli false
	handSlider -> Enable(_bananaActive);
}

//funkcja ktora obsluguje ruch scrollbara i ustawia pozycje scrollbara na wxGauge, steruje ona tez pozycja reki malpki
void GUIMainFrame::funcScrollSlider( wxScrollEvent& event )
{
	//pozyskujemy pozycje ze scrollbara
	_position = event.GetPosition();
	//pozycje przekazujemy do gauge'a
	handGauge -> SetValue(_position);
}

//funkcja obslugujaca przycisk, ktora uruchamia okno dialogowe do wyboru koloru
void GUIMainFrame::funcChangeColor( wxCommandEvent& event )
{
	//deklaracja okna dialogowego do wyboru koloru
	wxColourDialog colourDialog(this);
	
	//jesli zaakceptuje wybor koloru, to wybrany kolor jest zapisywany do zmiennej _colour ktora koloruje symbol gwiazdki
	if (colourDialog.ShowModal() == wxID_OK) {
		_colour = colourDialog.GetColourData().GetColour();
	}

}

//funkcja obslugujaca TextCtrl i zapisujaca do zmiennej _text string wpisany do kontrolera
void GUIMainFrame::funcUpdateText( wxCommandEvent& event )
{
	_text = testText->GetValue();
}

//funkcja obslugujaca wxChoice, ktory w zaleznosci od tego ktory element jest wybrany, zapisuje go do zmiennej _chosenSymbol
void GUIMainFrame::changeSymbol( wxCommandEvent& event )
{
	_chosenSymbol = symbolChooser->GetString(symbolChooser->GetSelection());
}

//funkcja rysująca wszystkie elementy które aktualnie mają znalezc sie na panelu
void GUIMainFrame::paintOnPanel(wxClientDC& dcClient) 
{
	//deklarujemy bitmape z ktorej pozniej bedziemy mogli wxImage do zapisu
	_panelPicture = wxBitmap(drawingPanel->GetSize().x, drawingPanel->GetSize().y);

	//ustawiamy pozycje reki malpki
	setHandPos();
	
	//wartosci współrzędnych ustalamy raz jeszcze 
	dPx = drawingPanel->GetSize().x / 2;
	dPy = drawingPanel->GetSize().y / 2; 

	//korzystamy z podwojnego buforowania, ponieważ po probie z rysowaniem na kontekscie rysunkowym wxClientDC występowało migotanie
	wxBufferedDC bufferedPanel(&dcClient, _panelPicture);
	
	PrepareDC(bufferedPanel);
	bufferedPanel.Clear();

	//ustalamy srodek ukladu wspolrzednych naszego buforowanego kontekstu
	bufferedPanel.SetDeviceOrigin(dPx, dPy);
	
	//wypisujemy teksty na panel
	drawText(bufferedPanel, _text);

	//rysujemy malpke (niezmienne elementy)
	drawMonkey(bufferedPanel);

	//zmieniamy kolor wypelnienia
	bufferedPanel.SetBrush(wxBrush(*wxBLACK_BRUSH));
	
	//na podstawie flagi z bananem decydujemy o tym co zostanie narysowane
	//jezeli banan jest aktywny
	if (_bananaActive) {
		//rysujemy bitmape ktora powstala z wczytanego obrazku banana, wspolrzedne uzaleznione od wspolrzednych konca reki malpki
		bufferedPanel.DrawBitmap(_banana,  xH-30,  yH-20); 
		//rysujemy prawa reke w zaleznosci od wspolrzednych
		bufferedPanel.DrawLine(0, 0, xH, yH);
		//zmiana koloru wypelnienia na bialy (by wpasowal sie w tlo)
		bufferedPanel.SetBrush(wxBrush(*wxWHITE));
		//rysujemy usmiechnieta buzke (poniewaz malpka ma banana)
		bufferedPanel.DrawEllipticArc(-14, -29, 28, 10, 180, 0);
		//rysujemy lewe oko ktore w tej wersji jest wyzsze niz szersze
		bufferedPanel.DrawEllipse(2, -37, 10, 6);
	}
	else {
		//rysujemy prawa reke w zaleznosci od wspolrzednych (tylko po to zeby sie zgadzala pozycja w razie przelaczenia flagi banana)
		bufferedPanel.DrawLine(0, 0, xH, yH);
		//zmieniamy kolor wypelnienia na bialy
		bufferedPanel.SetBrush(wxBrush(*wxWHITE));
		//rysujemy smutna buzke (poniewaz malpka nie ma banana)
		bufferedPanel.DrawEllipticArc(-14, -23, 28, 10, 0, 180);
		//rysujemy lewe oko ktore w tej wersji jest szersze niz wysze
		bufferedPanel.DrawEllipse(4, -40, 6, 10); //right eye
	}

	//RYSOWANIE SYMBOLI
	
	//jezeli w symbolChooser wybrana jest gwiazdka, rysujemy ja na panelu
	if (_chosenSymbol == "gwiazdka") {
		drawStar(bufferedPanel, _colour);
	}

	//jezeli w symbolChooser wybrany jest ksiezyc, rysujemy go na panelu
	if (_chosenSymbol == "ksiezyc") {
		drawMoon(bufferedPanel);
	}

	//jezeli w symbolChoose wybrane jest slonko, rysujemy je na panelu
	if (_chosenSymbol == "slonko") {
		drawSun(bufferedPanel);
	}

}

void GUIMainFrame::drawText(wxBufferedDC& bufferedPanel, wxString _text) {
	//ustalamy kolor piora na czarny
	bufferedPanel.SetPen(wxPen(wxColour(*wxBLACK)));
	//wypisujemy tekst po prawej stronie malpki
	bufferedPanel.DrawText(_text, -90, 78);
	//zmieniamy czcionke na Segoe Script
	bufferedPanel.SetFont(wxFont(30, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, false, wxT("Segoe Script")));
	//Jeny zdecydowanie za duzo czasu powiecilem na znalezienie tej czcionki w FormBuilderze :P

	//wypisujemy obrocony o 90 stopni tekst po prawej stronie malpki
	bufferedPanel.DrawRotatedText(_text, 60, 92, 90);
}

//funkcja rysujaca malpke (elementy ktore sie nie zmieniaja)
void GUIMainFrame::drawMonkey(wxBufferedDC& bufferedPanel) {
	bufferedPanel.SetPen(wxPen(*wxBLACK_PEN));
	//rysujemy tulow 
	bufferedPanel.DrawLine(0, -20, 0, 62);
	//rysujemy lewa reke (lewa z perspektywy malpki)
	bufferedPanel.DrawLine(0, 0, 40, 18);
	//rysujemy lewa noge
	bufferedPanel.DrawLine(0, 62, 30, 92);
	//rysujemy prawa noge
	bufferedPanel.DrawLine(0, 62, -30, 92);
	//rysujemy glowe
	bufferedPanel.DrawCircle(0, -30, 20);
	//rysujemy prawe oko poniewaz pozostaje niezmienne
	bufferedPanel.DrawEllipse(-12, -37, 10, 6);
}


//funkcja rysujaca gwiazdke
void GUIMainFrame::drawStar(wxBufferedDC& bufferedPanel, wxColour _colour) {
	//zmieniamy kolor na ten, ktory pobralismy z okna dialogowego w funkcji funcChangeColor
	bufferedPanel.SetPen(wxPen(_colour));
	//zmieniamy kolor wypelnienia rowniez na ten kolor
	bufferedPanel.SetBrush(wxBrush(_colour));

	//tworzymy tablice wspolrzednych do stworzenia gwiazdki
	wxPoint symbolPosition[5];

	//deklarujemy pozycje gdzie zostanie narysowana gwiazdka
	declareStarPosition(symbolPosition);

	//rysujemy gwiazdke na ekranie
	bufferedPanel.DrawPolygon(5, symbolPosition, wxWINDING_RULE);
}

//funkcja rysujca ksiezyc
void GUIMainFrame::drawMoon(wxBufferedDC& bufferedPanel) {
	//zmieniamy kolor na jasnoszary (tak jak w demo)
	bufferedPanel.SetPen(wxPen(wxColour(*wxLIGHT_GREY)));
	//zmieniamy kolor wypelnienia na ten sam kolor
	bufferedPanel.SetBrush(wxBrush((*wxLIGHT_GREY)));

	//rysujemy jedno kolo, ktore pozniej bedzie naszym ksiezycem
	bufferedPanel.DrawCircle(-70, -70, 20);

	//zmieniamy kolor na bialy (kolor tla)
	bufferedPanel.SetPen(wxPen(wxColour(*wxWHITE)));
	//zmieniamy kolor wypelnienia na ten sam kolor
	bufferedPanel.SetBrush(wxBrush((*wxWHITE)));

	//rysujemy drugie kolo, ktore przysloni to pierwsze do tego stopnia, ze bedzie wygladalo jak ksiezyc
	bufferedPanel.DrawCircle(-80, -80, 20);

	//zmieniamy kolor na czarny
	bufferedPanel.SetPen(wxPen(wxColour(*wxBLACK)));
	//kolor wypelnienia rowniez
	bufferedPanel.SetBrush(wxBrush((*wxBLACK)));

	//rysujemy oko ksiezyca
	bufferedPanel.DrawCircle(-60, -70, 2);
	//rysujemy buzke ksiezyca
	bufferedPanel.DrawLine(-70, -62, -65, -57);
}

//funkcja rysujaca sloneczko
void GUIMainFrame::drawSun(wxBufferedDC& bufferedPanel) {
	//zmieniamy kolor na zolty (tak jak w demo)
	bufferedPanel.SetPen(wxPen(wxColour(*wxYELLOW)));
	//zmieniamy kolor wypelnienia na ten sam kolor
	bufferedPanel.SetBrush(wxBrush((*wxYELLOW)));
	//rysujemy slonko
	bufferedPanel.DrawCircle(-70, -70, 20);
}

//funkcja ustalajaca wspolrzedne punktow gwiazdki
void GUIMainFrame::declareStarPosition(wxPoint *symbolPosition) {
	
	symbolPosition[0] = wxPoint(-70, -90);
	symbolPosition[1] = wxPoint(-45, -45);
	symbolPosition[2] = wxPoint(-100, -70);
	symbolPosition[3] = wxPoint(-40, -70);
	symbolPosition[4] = wxPoint(-90, -45);
	
}

//funkcja ustalająca współrzędne drugiego punktu prawej reki malpki
void GUIMainFrame::setHandPos()
{
	xH = -_position / 10 - 40;
	yH = -_position / 3 + 28;
}