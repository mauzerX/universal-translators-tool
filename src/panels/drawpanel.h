/***************************************************************
 * Name:      drawpanel.h
 * Purpose:   DrawPanel class declaration
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-02-02
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/
#ifndef DRAWPANEL_H_INCLUDED
#define DRAWPANEL_H_INCLUDED

// TODO: remove when will be done
#include "selectrect.h"

enum UTTDrawParams
{
	utdNone		= 0x0000,
	utdReserve	= 0x0001,
	utdLeft		= 0x0002,
	utdHCenter  = 0x0004,
	utdVCenter  = 0x0008,
	utdRight	= 0x0010,
	utdUp		= 0x0020,
	utdDown		= 0x0040,
	utdExpand	= 0x0080,
	utdExactFit = 0x0100,
	utdStretch	= 0x0200
};

class DrawPanel :
	public wxScrolledWindow, public SelectionRectangle
{
public:

	explicit DrawPanel( wxWindow* parent );
	virtual ~DrawPanel(void);

	void SetBitmap( wxBitmap* bitmap );
	void SetBitmap(Pixel* buffer, int width, int height);

	void SetBuffer( Pixel* buffer );
	void SetBitmapScale( wxDouble scale );
	void SetScaleRange( wxDouble min, wxDouble max );
	void SetAlign( int align );
	void SetAllowScaling( bool b = true );
	void SetDrawFocus( bool b = true );

	static void RefCheck();
	static wxBitmap* CreateBitmap(Pixel* buffer, int width, int height);

protected:

	virtual void OnMouseWheel( wxMouseEvent &event );
	virtual void OnBtnDown( wxMouseEvent& event );
	virtual void OnMotion( wxMouseEvent& event );
	virtual void OnBtnUp( wxMouseEvent& event );
	virtual void OnKeyDown( wxKeyEvent& event );
	virtual void OnKeyUp( wxKeyEvent& event );

	virtual void OnPaint(wxPaintEvent& event);
	virtual void OnSize(wxSizeEvent& event);
	virtual void Render(wxDC& dc);

	virtual void OnEnterWindow( wxMouseEvent& event );
	virtual void OnLeaveWindow( wxMouseEvent& event );

	virtual bool MouseButton( int btn, bool up );
	virtual bool MouseModifiersButton( int modifier, int btn, bool up );
	virtual bool MouseMoving( int modifier, int btn );
	virtual bool MouseWheel( int modifier, int delta );

	virtual bool KeyDown( int modifier, int keyCode );
	virtual bool KeyUp( int modifier, int keyCode );
	virtual bool PlusMinusPressed( bool plus );

	virtual void SetShowParams();

	void PaintNow();
	void DestroyBitmap();
	void ApplyBitmap();


	bool		mDrawFocus;
	wxDouble	mXAspectRatio;
	wxDouble	mYAspectRatio;
	int		    mShowWidth;				// ширина картинки для отображения
	int		    mShowHeight;
	int		    mPosX;					// стартовая позиция для отображения
	int		    mPosY;
	wxDouble	mScale;
	wxDouble	mScaleMin;
	wxDouble	mScaleMax;
	int 		mScaledWidth;
	int		    mScaledHeight;
	wxRect		mBitmapRect;
	wxPoint		mMousePoint;			// pixel coordinats where mouse cursor points

	wxBitmap*	mBitmap;
	int		    mWidth;
	int		    mHeight;

private:
	void DrawFocus(wxDC& dc);
	void CalculateScrollBars();

	int			mAlign;
	bool		mAllowScaling;			// allows scaling by mouse wheel
	wxSize      mPreviousSize;


	static int			sRefCount;
};


WX_DEFINE_ARRAY( DrawPanel* , DrawPanelArray );

#endif