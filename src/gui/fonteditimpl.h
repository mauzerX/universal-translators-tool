/***************************************************************
 * Name:      mainframeimpl.h
 * Purpose:   FontEditGui implementation
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-01-17
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/
#ifndef FONTEDITIMPL_H_INCLUDED
#define FONTEDITIMPL_H_INCLUDED

#include "uttpanelsgui.h"
#include "types/ieditor.h"

class FontInfo;
class FontEditGui;
class SymbolEditGui;
class SymbolPanel;

class FontEditor :	public FontEditGui, public IEditor
{
public:
	FontEditor( wxWindow* parent );
	~FontEditor(void);
	
	//bool CheckChanges();

	SymbolPanel* GetSymbolPanel();
	void SetCurrentSymbol(int n);

	// from interfaces
	virtual bool SaveEditor();
	virtual bool LoadEditor();

	virtual bool SaveState( wxOutputStream& output );
	virtual bool LoadState( wxInputStream& input, int version );

	virtual const Origin*	GetOrigin() const;
	virtual void SetInfo( IInfo* info );

protected:

	virtual void OnBtnClick( wxCommandEvent& event );
	//virtual void OnFontChangeEvent( ChangeInfoEvent& event );
	virtual void OnSymbolSelection( SymbolSelectionEvent& event );
	virtual void OnRebuildDataEvent( EditorRebuildDataEvent& event );

private:


	bool ShowSettings();
	bool CreateFont();
	void ClearFont( bool force = false );	// force - to clear memory from destructor
	void ChangeFontPalette( Palette* pal );
	void SetPaletteAsMain();

	void UpdateFont();
	void UpdateRibbon();
	void CurrentSymbolChanged();

	SymbolEditGui*	mSymbolEditor;
	FontInfo*		mCurrentFont;
	wxUint32		mCurrentSymbol;

};

#endif
