///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/button.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/checkbox.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainWindow
///////////////////////////////////////////////////////////////////////////////
class MainWindow : public wxFrame
{
	private:

	protected:
		wxButton* m_loadButton;
		wxButton* m_censoreButton;
		wxButton* m_erodeButton;
		wxCheckBox* m_animateCB;
		wxRichTextCtrl* m_text;
		wxPanel* m_panel1;

		// Virtual event handlers, overide them in your derived class
		virtual void windowOnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void windowUIUpdated( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void m_loadImage( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_censore( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_erode( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_animate( wxCommandEvent& event ) { event.Skip(); }


	public:

		MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("GFK Lab 08 - Tomasz Gajda"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1100,571 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MainWindow();

};

