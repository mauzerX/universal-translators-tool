/***************************************************************
 * Name:      selectrect.cpp
 * Purpose:   Code for SelectionRectangle class
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-03-06
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/

#include "pch.h"
#include "selectrect.h"

SelectionRectangle::SelectionRectangle(  wxScrolledWindow* parent ):
	mMousePoint( -1, -1 ),
	mPreviousMousePoint( -1, -1 ),
	mParent( parent ),
	mWorkZone( 0, 0, 0, 0 ),
	mPointSize( 1.0f ),
	mSelectionDrag( false ),
	mIsZoneValid( false ),
	mZoneDrag( false ),
	mStartPoint( -1, -1 ),
	mEndPoint( -1, -1 ),
	mCoordRect( 0, 0, 0, 0 )
{
}

SelectionRectangle::~SelectionRectangle(void)
{
}

void SelectionRectangle::SetWorkZone( const wxRect& rect, wxDouble pointSize )
{
	mWorkZone = rect;
	mPointSize = pointSize;
}



wxPoint	SelectionRectangle::GetMousePosition()
{
	wxPoint pos = mParent->ScreenToClient( wxGetMousePosition() );
	return GetMousePosition( pos );
}



wxPoint	SelectionRectangle::GetMousePosition( const wxPoint& pos )
{
	wxPoint result = pos;
	if ( !mWorkZone.Contains( pos ) )
	{
		result.x = -1;
		result.y = -1;
	}
	return result;
}



wxPoint SelectionRectangle::MousePosition2PointCoords( const wxPoint& pos, bool zeroBased /* true */ )
{
	wxPoint result = GetMousePosition( pos );
	Position2Coords( result );
	if (!zeroBased && result.x != -1 && result.y != -1)
	{
		++result.x;
		++result.y;
	}
	return result;
}



wxPoint SelectionRectangle::MousePosition2PointCoords()
{
	wxPoint result = GetMousePosition();
	Position2Coords( result );
	return result;
}



inline void SelectionRectangle::Position2Coords( wxPoint& point )
{
	if (point.x != -1 && point.y != -1)
	{
		point += mParent->GetViewStart() - mWorkZone.GetLeftTop();
		point.x /= mPointSize;
		point.y /= mPointSize;
	}
}



void SelectionRectangle::SelectionBegin()
{
	SelectionBegin( GetMousePosition() );
}



void SelectionRectangle::OnSelectionMotion()
{
	OnSelectionMotion( GetMousePosition() );
}



void SelectionRectangle::SelectionEnd()
{
	SelectionEnd( GetMousePosition() );
}



void SelectionRectangle::SelectionBegin( const wxPoint& mousePos )
{
	wxPoint pos = this->MousePosition2PointCoords( mousePos );
	bool coorValid = pos.x != -1 && pos.y != -1;
	if ( coorValid )
	{
		mIsZoneValid = true;
		mSelectionDrag = true;
		mStartPoint = mousePos;
		mEndPoint = mStartPoint;
		UpdateCoords();
		return;
	}
	else
	{
		if (mIsZoneValid && !mCoordRect.Contains(pos) )
		{
			mIsZoneValid = false;
			mParent->Refresh();
			return;
		}
	}
}



void SelectionRectangle::OnSelectionMotion( const wxPoint& mousePos )
{
	if (mSelectionDrag)
	{
		wxPoint point = MousePosition2PointCoords( mousePos );
		if (point.x != -1 && point.y != -1)
		{
			mEndPoint = mousePos;
			UpdateCoords();
			mParent->Refresh();
		}
	}
}



void SelectionRectangle::SelectionEnd( const wxPoint& mousePos )
{
	if (mSelectionDrag)
	{
		if (mousePos.x != -1 && mousePos.y != -1)
		{
			mEndPoint = mousePos;
		}
		UpdateCoords();
		mSelectionDrag = false;
		mParent->Refresh();
	}
}



void SelectionRectangle::DrawSelection()
{
	wxClientDC dc(mParent);
	RenderSelection(dc);
}



void SelectionRectangle::RenderSelection(wxDC& dc)
{
	mCoordRect = wxRect( mStartCoord, mEndCoord );
	
	//wxPoint view = mParent->GetViewStart() - mWorkZone.GetLeftTop();
	//const static wxColour bg(0x80, 0x80, 0x80, 0x30);
	//if (mSelectionDrag)
	//{
	//	dc.SetBrush( *wxTRANSPARENT_BRUSH );
	//	dc.SetPen( *wxWHITE_PEN );
	//	dc.SetLogicalFunction( wxXOR );
	//	wxRect rect( mStartPoint + view, mEndPoint + view );
	//	dc.DrawRectangle( rect );

	//}

	//if (mIsZoneValid)
	//{
	//	wxRect rect( mCoordRect.GetTopLeft() * mPointSize, (mCoordRect.GetBottomRight() + wxPoint(1, 1)) * mPointSize );

	//	dc.SetBrush( mSelectionDrag ? *wxTRANSPARENT_BRUSH : bg );
	//	dc.SetPen( wxPen( *wxRED, 3, wxPENSTYLE_LONG_DASH ) );

	//	
	//	dc.SetLogicalFunction( wxXOR );
	//	dc.DrawRectangle( rect );
	//}
}



void SelectionRectangle::UpdateCoords()
{
	mStartCoord = MousePosition2PointCoords( mStartPoint );
	mEndCoord = MousePosition2PointCoords( mEndPoint );
}



void SelectionRectangle::ZoneDragBegin()
{
	mZoneDrag = !(mSelectionDrag || !mIsZoneValid || !mCoordRect.Contains(mMousePoint));
}



void SelectionRectangle::OnZoneDragMotion()
{
	if (mMousePoint.x == -1 && mMousePoint.y == -1)
	{
		ZoneDragEnd();
	}

	if (mZoneDrag && mMousePoint != mPreviousMousePoint)
	{
		wxPoint diff = mMousePoint - mPreviousMousePoint;
		wxPoint newStart = mStartCoord + diff;
		wxPoint newEnd = mEndCoord + diff;
		if (newStart.x >= 0 && newStart.y >= 0 && newEnd.x >= 0 && newEnd.y >= 0)
		{
			mStartCoord = newStart;
			mEndCoord = newEnd;
		}
		mParent->Refresh();
	}
}



void SelectionRectangle::ZoneDragEnd()
{
	mZoneDrag = false;
}






