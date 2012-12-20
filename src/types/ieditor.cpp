/***************************************************************
 * Name:      ieditor.cpp
 * Purpose:   implementation of IEditor interface
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-12-12
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/
 
#include "pch.h"
#include "ieditor.h"



const wxString sEditorNames[ etNum ] = 
{
	"Font",
	"Image",
	"Library",
	"Animation",
	"Sound",
	"Video"
};


IEditor::IEditor( wxWindow* parent, EditorType edtype, const wxString& name ):
	mParent( parent ),
	mEditorType( edtype ),
	mChanged( false ),
	mEditorName( name ),
	mEditorId(-1)
{
}



IEditor::IEditor( const IEditor& other ):
	mParent( other.mParent ),
	mEditorType( other.mEditorType ),
	mChanged( other.mChanged ),
	mEditorName( other.mEditorName ),
	mEditorId( other.mEditorId )
{
}



wxString IEditor::CreateName()
{
	return wxString::Format("%s_%d", sEditorNames[mEditorType], (wxInt32) mEditorId);
}



/* virtual */ IEditor::~IEditor()
{
	CheckChanged();
}



/* virtual */ int IEditor::CheckChanged()
{
	wxString message = mEditorName + " modified";
	return wxMessageDialog(mParent, "Save changes?", message, wxYES_NO | wxCANCEL | wxCENTRE | wxCANCEL_DEFAULT).ShowModal();
}