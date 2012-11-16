/***************************************************************
 * Name:      custumevents.h
 * Purpose:   custom events for internal use
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-03-12
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/
#ifndef CUSTOMEVENTS_H_INCLUDED
#define CUSTOMEVENTS_H_INCLUDED

class ModuleChangedEvent;
class ColourPickEvent;
class ChangeFontEvent;
class ChangePaletteEvent;
class SymbolSelectionEvent;


wxDECLARE_EVENT( uttEVT_MODULECHANGED, ModuleChangedEvent );
wxDECLARE_EVENT( uttEVT_COLOURPICK, ColourPickEvent );
wxDECLARE_EVENT( uttEVT_CHANGEFONT, ChangeFontEvent );
wxDECLARE_EVENT( uttEVT_CHANGEPALETTE, ChangePaletteEvent );
wxDECLARE_EVENT( uttEVT_SYMBOLSELECT, SymbolSelectionEvent );



//////////////////////////////////////////////////////////////////////////



class ModuleChangedEvent : public wxEvent
{
public:
	
    ModuleChangedEvent( )
        : wxEvent(0, uttEVT_MODULECHANGED)
	{}
	
    ModuleChangedEvent(const ModuleChangedEvent& event)
        : wxEvent(event)
    {}

    virtual wxEvent *Clone() const { return new ModuleChangedEvent(*this); }

private:

    DECLARE_DYNAMIC_CLASS_NO_ASSIGN(ModuleChangedEvent)
};

typedef void (wxEvtHandler::*ModuleChangedEventFunction)(ModuleChangedEvent&);



#define ModuleChangedEventHandler(func) \
    wxEVENT_HANDLER_CAST(ModuleChangedEventFunction, func)

#define EVT_MODULECHANGED(func) wx__DECLARE_EVT0(uttEVT_MODULECHANGED, ModuleChangedEventHandler(func))



//////////////////////////////////////////////////////////////////////////



class ColourPickEvent : public wxEvent
{
public:
	
	enum ColourOperation
	{	
		cpeSetThisColour,
		cpeFindThisColour
	};

    ColourPickEvent( )
        : wxEvent(0, uttEVT_COLOURPICK),
			mColour( *wxBLACK ),
			mButton( 0 ),
			mAction( -1 )
	{ }
	
    ColourPickEvent( const wxColour& colour, int button, ColourOperation what )
        : wxEvent(0, uttEVT_COLOURPICK),
			mColour( colour ),
			mButton( button ),
			mAction( what )
	{ }
	
    ColourPickEvent(const ColourPickEvent& event)
        : wxEvent(event),
   			mColour( event.mColour ),
			mButton( event.mButton ),
			mAction( event.mAction )
    { }

    virtual wxEvent *Clone() const { return new ColourPickEvent(*this); }
    
    int GetButton()
    {
		return mButton;
    }

	int GetAction()
	{
		return mAction;
	}
    
    const wxColour& GetColour()
    {
		return mColour;
    }

protected:
	
private:

	wxColour	mColour;
	int			mButton;
	int			mAction;
    DECLARE_DYNAMIC_CLASS_NO_ASSIGN(ColourPickEvent)
};

typedef void (wxEvtHandler::*ColourPickEventFunction)(ColourPickEvent&);



#define ColourPickEventHandler(func) \
    wxEVENT_HANDLER_CAST(ColourPickEventFunction, func)

#define EVT_COLOURPICK(func) wx__DECLARE_EVT0(uttEVT_COLOURPICK, ColourPickEventHandler(func))


//////////////////////////////////////////////////////////////////////////


class FontInfo;

class ChangeFontEvent : public wxEvent
{
public:
	

    ChangeFontEvent( )
        : wxEvent(0, uttEVT_CHANGEFONT),
		mFontInfo( NULL )
	{ }
	
    ChangeFontEvent( FontInfo* fontInfo )
        : wxEvent(0, uttEVT_CHANGEFONT),
		mFontInfo( fontInfo )
	{ }
	
    ChangeFontEvent(const ChangeFontEvent& event)
        : wxEvent(event),
		mFontInfo( event.mFontInfo )
    { }

    virtual wxEvent *Clone() const { return new ChangeFontEvent(*this); }
    
    FontInfo*	GetFontInfo()
	{
		return mFontInfo;
	}

protected:
	
private:

	FontInfo*	mFontInfo;

	DECLARE_DYNAMIC_CLASS_NO_ASSIGN(ChangeFontEvent)
};

typedef void (wxEvtHandler::*ChangeFontEventFunction)(ChangeFontEvent&);



#define ChangeFontEventHandler(func) \
    wxEVENT_HANDLER_CAST(ChangeFontEventFunction, func)

#define EVT_CHANGEFONT(func) wx__DECLARE_EVT0(uttEVT_CHANGEFONT, ChangeFontEventHandler(func))


//////////////////////////////////////////////////////////////////////////

class Palette;

class ChangePaletteEvent : public wxEvent
{
public:
	

    ChangePaletteEvent( )
        : wxEvent(0, uttEVT_CHANGEPALETTE),
		mData( NULL )
	{ 	
	}
	


    ChangePaletteEvent( int winid, Palette* pal )
        : wxEvent(winid, uttEVT_CHANGEPALETTE),
		mData( pal )
	{ 
	}
	
    ChangePaletteEvent(const ChangePaletteEvent& event)
        : wxEvent(event),
		mData( event.mData )
    { 
	}

    virtual wxEvent *Clone() const { return new ChangePaletteEvent(*this); }
  
	Palette*	GetPalette() { return mData; }

protected:
	
private:

	Palette*	mData;

	DECLARE_DYNAMIC_CLASS_NO_ASSIGN(ChangePaletteEvent)
};

typedef void (wxEvtHandler::*ChangePaletteEventFunction)(ChangePaletteEvent&);



#define ChangePaletteEventHandler(func) \
    wxEVENT_HANDLER_CAST(ChangePaletteEventFunction, func)

#define EVT_CHANGEPALETTE(func) wx__DECLARE_EVT0(uttEVT_CHANGEPALETTE, ChangePaletteEventHandler(func))



//////////////////////////////////////////////////////////////////////////



class SymbolSelectionEvent : public wxEvent
{
public:
	

    SymbolSelectionEvent( )
        : wxEvent(0, uttEVT_SYMBOLSELECT),
		mKey( -1 )
	{ 	
	}
	


    SymbolSelectionEvent(int key )
        : wxEvent(0, uttEVT_SYMBOLSELECT),
		mKey( key )
	{ 
	}
	
    SymbolSelectionEvent(const SymbolSelectionEvent& event)
        : wxEvent(event),
		mKey( event.mKey )
    { 
	}

    virtual wxEvent *Clone() const { return new SymbolSelectionEvent(*this); }
  
	int		GetKey() { return mKey; }

protected:
	
private:

	int		mKey;

	DECLARE_DYNAMIC_CLASS_NO_ASSIGN(SymbolSelectionEvent)
};

typedef void (wxEvtHandler::*SymbolSelectionEventFunction)(SymbolSelectionEvent&);



#define SymbolSelectionEventHandler(func) \
    wxEVENT_HANDLER_CAST(SymbolSelectionEventFunction, func)

#define EVT_SYMBOLSELECT(func) wx__DECLARE_EVT0(uttEVT_SYMBOLSELECT, SymbolSelectionEventHandler(func))



//////////////////////////////////////////////////////////////////////////
#endif  // CUSTOMEVENTS_H_INCLUDED
