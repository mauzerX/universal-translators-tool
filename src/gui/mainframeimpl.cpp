/***************************************************************
 * Name:      mainframeimpl.cpp
 * Purpose:   Code for MainFrameImpl Class
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-01-17
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/

#include "pch.h"
#include "mainframeimpl.h"
#include "luacontrol.h"
#include "types/undo.h"
#include "types/imageinfo.h"

#include "fonteditimpl.h"
#include "logwindowimpl.h"
#include "editpanelimpl.h"
#include "palwindowimpl.h"
#include "libwindowimpl.h"

#include "panels/symbolpanel.h"

#define MODULE_MENU_START	(wxID_HIGHEST + 1)
#define MODULE_MENU_END		(MODULE_MENU_START + 32)

MainFrameImpl::MainFrameImpl(void):
	UttMainFrame(0L),
	mHelpController( NULL ),
	mFontEditor( new FontEditor( mAUINotebook ) ),
	mEditWindow( new ImageEditor( mAUINotebook, wxID_IMAGEEDITOR ) ),
	mPalWindow( new PaletteWindowImpl( mAUINotebook ) ),
	mLibWindow( new LibraryPanel( mAUINotebook ) )
{
	
	COMMAND->SetEditMenu( mEditMenu );
	COMMAND->Initialize();

	m_mgr.SetFlags( wxAUI_MGR_ALLOW_FLOATING		|
					wxAUI_MGR_ALLOW_ACTIVE_PANE		|
					wxAUI_MGR_TRANSPARENT_DRAG		|
					wxAUI_MGR_TRANSPARENT_HINT		|
					wxAUI_MGR_HINT_FADE				|
					wxAUI_MGR_LIVE_RESIZE              );


	this->AddPane(mFontEditor, "Font editor");
	this->AddPane(mEditWindow, "Image editor");
	this->AddPane(mPalWindow, "Palette window");
	this->AddPane(mLibWindow, "Library");

//		wxBMPHandler: For loading (including alpha support) and saving, always installed.
//		wxPNGHandler: For loading and saving. Includes alpha support.
//		wxJPEGHandler: For loading and saving.
//		wxGIFHandler: For loading and saving (see below).
//		wxPCXHandler: For loading and saving (see below).
//		wxPNMHandler: For loading and saving (see below).
//		wxTIFFHandler: For loading and saving. Includes alpha support.
//		wxTGAHandler: For loading and saving. Includes alpha support.
//		wxIFFHandler: For loading only.
//		wxXPMHandler: For loading and saving.
//		wxICOHandler: For loading and saving.
//		wxCURHandler: For loading and saving.
//		wxANIHandler: For loading only.

	wxImage::AddHandler(new wxPNGHandler);
	wxImage::AddHandler(new wxJPEGHandler);
	wxImage::AddHandler(new wxGIFHandler);
	wxImage::AddHandler(new wxPCXHandler);
	wxImage::AddHandler(new wxPNMHandler);
	wxImage::AddHandler(new wxTGAHandler);
	wxImage::AddHandler(new wxTIFFHandler);
	wxImage::AddHandler(new wxIFFHandler);
	wxImage::AddHandler(new wxXPMHandler);


	this->Bind( wxEVT_IDLE, &MainFrameImpl::OnIdle, this );
	this->Bind( wxEVT_SHOW, &MainFrameImpl::OnShow, this );

	mEditWindow->GetEditPanel()->Bind( uttEVT_COLOURPICK, &MainFrameImpl::OnColourPickEvent, this );
	mFontEditor->GetSymbolPanel()->Bind( uttEVT_COLOURPICK, &MainFrameImpl::OnColourPickEvent, this );
	wxTheApp->Bind( uttEVT_MODULECHANGED, &MainFrameImpl::OnModuleChanged, this );

	m_mgr.Update();
}



MainFrameImpl::~MainFrameImpl(void)
{
	wxTheApp->Unbind( uttEVT_MODULECHANGED, &MainFrameImpl::OnModuleChanged, this );
	this->Unbind( wxEVT_SHOW, &MainFrameImpl::OnShow, this );
	this->Unbind( wxEVT_IDLE, &MainFrameImpl::OnIdle, this );
}



void MainFrameImpl::OnColourPickEvent( ColourPickEvent& event )
{
	if ( event.GetButton() != wxMOUSE_BTN_LEFT && event.GetButton() != wxMOUSE_BTN_RIGHT )
	{
		return;
	}

	bool right = event.GetButton() == wxMOUSE_BTN_RIGHT;
	switch ( event.GetAction() )
	{
		case ColourPickEvent::cpeSetThisColour:
			mPalWindow->SetColour(right, event.GetColour() );
		break;

		case ColourPickEvent::cpeFindThisColour:
			mPalWindow->FindColour(right, event.GetColour(), true );
		break;

		default:
			wxLogError( wxString::Format( "MainFrameImpl::OnColourPickEvent: unknown action %d", event.GetAction() ) );
			return;
	}
	event.Skip();
}



void MainFrameImpl::OnModuleChanged( ModuleChangedEvent& event )
{
	DoModuleChanged();
	event.Skip();
}



void MainFrameImpl::AddPane( wxWindow* wnd, const wxString& name )
{
	mAUINotebook->AddPage( wnd, name );
}



void MainFrameImpl::OnIdle( wxIdleEvent& )
{
	if (Lua::GetRebootFlag())
	{
		Lua::Done();
		Lua::Init();
	}
}



void MainFrameImpl::OnShow( wxShowEvent& event )
{
	event.Skip();
	m_mgr.Update();
}



void MainFrameImpl::Init()
{
	wxLogMessage( "Initiating UTT" );
	if (!Lua::Init())
	{
		Lua::ShowLastError();
	}

	mHelpController = new wxHelpController( this );
	if (!mHelpController->Initialize( "utt.chm" ))
	{
		wxLogError("Help was not initialized! UTT.CHM lost?");
		delete mHelpController;
		mHelpController = NULL;
	}
}



void MainFrameImpl::Deinit()
{
	ImageInfo::Done();
	if (mHelpController)
	{
		delete mHelpController;
		mHelpController = NULL;
	}

	Lua::Done();
	wxLogMessage( "UTT exits" );
}



void MainFrameImpl::DoFileOpen()
{
	if (!Lua::Get().call( "getExtensions" ) )
	{
		Lua::ShowLastError();
		return;
	}
	std::string result;
	OOLUA::pull2cpp(Lua::Get(), result);
	wxString extensions( result );

	wxFileDialog openFileDialog(this, "Open file", "./", "", extensions, wxFD_OPEN|wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	if ( !Lua::Get().call( "openFile", openFileDialog.GetPath().ToStdString() ) )
	{
		Lua::ShowLastError();
		return;
	}

}



void MainFrameImpl::DoSelectModule()
{
	if ( !Lua::Get().call( "selectModule" ) )
	{
		Lua::ShowLastError();
		return;
	}
}



void MainFrameImpl::DoSelectVersion()
{
	if ( !Lua::Get().call( "selectVersion" ) )
	{
		Lua::ShowLastError();
		return;
	}
}



void MainFrameImpl::OnClose( wxCloseEvent& event )
{
	this->Deinit();
	event.Skip();
}



void MainFrameImpl::OnModuleMenuSelect( wxCommandEvent& event )
{
	int id = event.GetId();
	if (id >= MODULE_MENU_START && id < MODULE_MENU_END )
	{
		DoModuleCommand( id );
	}
}



void MainFrameImpl::OnMenuSelect( wxCommandEvent& event )
{
	int id = event.GetId();
	switch ( id )
	{
		case wxID_HELP_HELP:
			if (mHelpController)
			{
				if (!mHelpController->DisplayContents())
				{
					wxLogError("Help was initialized, but can not display contents!");
				}
			}
		break;

		case wxID_FILE_OPEN:
			DoFileOpen();
		break;

		case wxID_FILE_QUIT:
			this->Close();
		break;

		case wxID_LUA_SELECT:
			DoSelectModule();
		break;

		case wxID_LUA_VERSION:
			DoSelectVersion();
		break;

		case wxID_LUA_REBOOT:
			if ( AREYOUSURE("Reboot Lua virtual machine...") == wxID_YES )
			{
				Lua::SetRebootFlag();
			}
		break;

		case wxID_UNDO:
			COMMAND->Undo();
		break;

		case wxID_REDO:
			COMMAND->Redo();
		break;

		default:
			wxLogMessage( wxString::Format("Unknown command from main menu: %d", id) );
		break;
	}
	event.Skip();
}



/* virtual */ void MainFrameImpl::OnPageChanged( wxAuiNotebookEvent& event )
{
	wxWindow* wnd = mAUINotebook->GetPage( event.GetSelection() );
	if (wnd)
	{
		mPalWindow->ChangeOwnerToCurrentPage( wnd->GetId() );
	}
	event.Skip();
}



void MainFrameImpl::DoModuleChanged()
{
	if ( !Lua::Get().call( "getModuleName" ) )
	{
		Lua::ShowLastError();
		return;
	}

	std::string s;
	if ( OOLUA::pull2cpp(Lua::Get(), s ) )
	{
		mStatusBar->SetStatusText(s, 1 );
	}

	wxArrayString res;
	if ( Lua::Get().call( "getModuleMenu" ) )
	{
		Helpers::PullTableOfStrings( res );
	}
	else
	{
		Lua::ShowLastError();
	}
	
	UpdateModuleMenu( res );
}



void MainFrameImpl::DoModuleCommand( int n )
{
	wxString command = mModuleMenu->GetLabelText(n);
	if ( !Lua::Get().call( "executeModuleMenuCommand", command.ToStdString() ) )
	{
		Lua::ShowLastError();
		return;
	}
}



void MainFrameImpl::ClearModuleMenu()
{
	wxMenuItemList& list = mModuleMenu->GetMenuItems();
	for ( size_t i = 0; i < list.GetCount(); ++i )
	{
		wxMenuItem* item = list[i];
		this->Unbind( wxEVT_COMMAND_MENU_SELECTED, &MainFrameImpl::OnModuleMenuSelect, this, item->GetId() );
		mModuleMenu->Delete(item);
	}
}



void MainFrameImpl::UpdateModuleMenu( const wxArrayString& strings )
{
	wxASSERT( strings.size() + MODULE_MENU_START < MODULE_MENU_END ); 
	ClearModuleMenu();
	for (size_t i = 0; i < strings.size(); ++i)
	{
		wxMenuItem* item = new wxMenuItem( mModuleMenu, MODULE_MENU_START + i, strings[i], wxEmptyString, wxITEM_NORMAL );
		mModuleMenu->Append( item );
		this->Bind( wxEVT_COMMAND_MENU_SELECTED, &MainFrameImpl::OnModuleMenuSelect, this, item->GetId() );
	}
}

