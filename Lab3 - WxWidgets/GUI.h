///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/scrolbar.h>
#include <wx/gauge.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame
{
	private:

	protected:
		wxPanel* drawingPanel;
		wxButton* saveImageButton;
		wxCheckBox* bananBox;
		wxScrollBar* handSlider;
		wxGauge* handGauge;
		wxButton* starColorButton;
		wxTextCtrl* testText;
		wxChoice* symbolChooser;

		// Virtual event handlers, overide them in your derived class
		virtual void funcOnSizeUpdate( wxSizeEvent& event ) { event.Skip(); }
		virtual void funcOnUpUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void funcSaveImage( wxCommandEvent& event ) { event.Skip(); }
		virtual void funcShowBanan( wxCommandEvent& event ) { event.Skip(); }
		virtual void funcScrollSlider( wxScrollEvent& event ) { event.Skip(); }
		virtual void funcChangeColor( wxCommandEvent& event ) { event.Skip(); }
		virtual void funcUpdateText( wxCommandEvent& event ) { event.Skip(); }
		virtual void changeSymbol( wxCommandEvent& event ) { event.Skip(); }


	public:

		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("GFK Lab 03 "), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 586,387 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MainFrame();

};

