///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GUI.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* firstSizer;
	firstSizer = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* panelSizer;
	panelSizer = new wxBoxSizer( wxVERTICAL );

	drawingPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	drawingPanel->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNHIGHLIGHT ) );

	panelSizer->Add( drawingPanel, 1, wxEXPAND | wxALL, 5 );


	firstSizer->Add( panelSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* buttonsSizer;
	buttonsSizer = new wxBoxSizer( wxVERTICAL );

	buttonsSizer->SetMinSize( wxSize( -1,300 ) );

	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	saveImageButton = new wxButton( this, wxID_ANY, wxT("Zapis do pliku"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( saveImageButton, 0, wxALIGN_CENTER|wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	bananBox = new wxCheckBox( this, wxID_ANY, wxT("banan"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( bananBox, 0, wxALIGN_CENTER|wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	handSlider = new wxScrollBar( this, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), wxSB_HORIZONTAL );
	buttonsSizer->Add( handSlider, 0, wxALIGN_CENTER|wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	handGauge = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( 150,-1 ), wxGA_HORIZONTAL );
	handGauge->SetValue( 0 );
	buttonsSizer->Add( handGauge, 0, wxALIGN_CENTER|wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	starColorButton = new wxButton( this, wxID_ANY, wxT("Kolor gwiazdki"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( starColorButton, 0, wxALIGN_CENTER|wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	testText = new wxTextCtrl( this, wxID_ANY, wxT("tekst"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( testText, 0, wxALIGN_CENTER|wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	wxString symbolChooserChoices[] = { wxT("gwiazdka"), wxT("ksiezyc"), wxT("slonko") };
	int symbolChooserNChoices = sizeof( symbolChooserChoices ) / sizeof( wxString );
	symbolChooser = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, symbolChooserNChoices, symbolChooserChoices, 0 );
	symbolChooser->SetSelection( 1 );
	buttonsSizer->Add( symbolChooser, 0, wxALIGN_CENTER|wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );


	firstSizer->Add( buttonsSizer, 0, 0, 5 );


	this->SetSizer( firstSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	drawingPanel->Connect( wxEVT_SIZE, wxSizeEventHandler( MainFrame::funcOnSizeUpdate ), NULL, this );
	drawingPanel->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainFrame::funcOnUpUI ), NULL, this );
	saveImageButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::funcSaveImage ), NULL, this );
	bananBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::funcShowBanan ), NULL, this );
	handSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	starColorButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::funcChangeColor ), NULL, this );
	testText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainFrame::funcUpdateText ), NULL, this );
	symbolChooser->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::changeSymbol ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	drawingPanel->Disconnect( wxEVT_SIZE, wxSizeEventHandler( MainFrame::funcOnSizeUpdate ), NULL, this );
	drawingPanel->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainFrame::funcOnUpUI ), NULL, this );
	saveImageButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::funcSaveImage ), NULL, this );
	bananBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::funcShowBanan ), NULL, this );
	handSlider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	handSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::funcScrollSlider ), NULL, this );
	starColorButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::funcChangeColor ), NULL, this );
	testText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainFrame::funcUpdateText ), NULL, this );
	symbolChooser->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::changeSymbol ), NULL, this );

}
