///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GUI.h"

///////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTIONTEXT ) );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	bSizer2->SetMinSize( wxSize( 300,-1 ) );
	m_loadButton = new wxButton( this, wxID_ANY, wxT("Wczytaj obrazek"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_loadButton, 0, wxALIGN_CENTER|wxALL, 5 );

	m_censoreButton = new wxButton( this, wxID_ANY, wxT("Cenzura"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_censoreButton, 0, wxALIGN_CENTER|wxALL, 5 );

	m_erodeButton = new wxButton( this, wxID_ANY, wxT("Erode"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_erodeButton, 0, wxALIGN_CENTER|wxALL, 5 );

	m_animateCB = new wxCheckBox( this, wxID_ANY, wxT("Animacja"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_animateCB, 0, wxALIGN_CENTER|wxALL, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_text = new wxRichTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	bSizer4->Add( m_text, 1, wxEXPAND | wxALL, 5 );


	bSizer2->Add( bSizer4, 1, wxEXPAND, 5 );


	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	bSizer3->SetMinSize( wxSize( 800,-1 ) );
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 800,533 ), wxTAB_TRAVERSAL );
	m_panel1->SetForegroundColour( wxColour( 64, 64, 64 ) );
	m_panel1->SetBackgroundColour( wxColour( 26, 26, 26 ) );

	bSizer3->Add( m_panel1, 1, wxEXPAND | wxALL, 0 );


	bSizer1->Add( bSizer3, 5, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( MainWindow::windowOnSize ) );
	this->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainWindow::windowUIUpdated ) );
	m_loadButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::m_loadImage ), NULL, this );
	m_censoreButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::m_censore ), NULL, this );
	m_erodeButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::m_erode ), NULL, this );
	m_animateCB->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainWindow::m_animate ), NULL, this );
}

MainWindow::~MainWindow()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( MainWindow::windowOnSize ) );
	this->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainWindow::windowUIUpdated ) );
	m_loadButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::m_loadImage ), NULL, this );
	m_censoreButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::m_censore ), NULL, this );
	m_erodeButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::m_erode ), NULL, this );
	m_animateCB->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainWindow::m_animate ), NULL, this );

}
