/***************************************************************
 * Name:      selectrect.h
 * Purpose:   SelectionRectangle declaration
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-03-06
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/
#ifndef SELECTRECT_H_INCLUDED
#define SELECTRECT_H_INCLUDED

class SelectionRectangle
{
public:
	SelectionRectangle( wxScrolledWindow* parent );
	~SelectionRectangle(void);

	void SetWorkZone( const wxRect& rect, wxDouble pointSize );

	// current position of mouse over bitmap
	wxPoint	GetMousePosition();
	// custom position of mouse over bitmap
	wxPoint	GetMousePosition( const wxPoint& pos );

	// current position of mouse to pixels coordinates
	wxPoint MousePosition2PointCoords();
	// custom position of mouse to pixels coordinates
	wxPoint MousePosition2PointCoords( const wxPoint& pos, bool zeroBased = true );

	void RenderSelection(wxDC& dc);

	bool IsZone()
	{
		return mIsZoneValid;
	}

	bool PointInZone(const wxPoint& point)
	{
		return mIsZoneValid && mCoordRect.Contains(point);
	}

	void ResetZone()
	{
		mIsZoneValid = false;
		mParent->Refresh();
	}

	bool SelectionDragging()
	{
		return mSelectionDrag;
	}

	bool ZoneDragging()
	{
		return mZoneDrag;
	}

	inline const wxRect& GetSelectionRect()
	{
		return mCoordRect;
	}

	void SetSelection(int x, int y, int w, int h);

protected:

	void SelectionBegin();
	void OnSelectionMotion();
	void SelectionEnd();

	void SelectionBegin( const wxPoint& mousePos );
	void OnSelectionMotion( const wxPoint& mousePos );
	void SelectionEnd( const wxPoint& mousePos );

	void ZoneDragBegin();
	void OnZoneDragMotion();
	void ZoneDragEnd();

	wxPoint				mMousePoint;			// pixel coordinates where mouse cursor points
	wxPoint				mPreviousMousePoint;

private:

	void UpdateCoords();
	void Position2Coords( wxPoint& point );


	wxScrolledWindow*	mParent;
	wxRect				mWorkZone;
	wxDouble			mPointSize;
	bool				mSelectionDrag;
	bool				mIsZoneValid;
	bool				mZoneDrag;
	wxPoint				mStartPoint;
	wxPoint				mEndPoint;
	wxPoint				mStartCoord;
	wxPoint				mEndCoord;
	wxRect				mCoordRect;

};

#endif
