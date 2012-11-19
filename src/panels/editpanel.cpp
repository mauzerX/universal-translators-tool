/***************************************************************
 * Name:      editpanel.cpp
 * Purpose:   Code for EditPanel class
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-02-29
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/

#include "pch.h"
#include "editpanel.h"
#include "types/indexmask.h"

#define GRID_EDGE	6.0f		// the value after the grid will be shown

UttColour	EditPanel::gGlobalLeftColour = *wxBLACK;
UttColour	EditPanel::gGlobalRightColour = *wxWHITE;

EditPanel::EditPanel(  wxWindow* parent ):
	DrawPanel( parent ),
	mDrawing( false ),
	mDrawCursor( true ),
	mCursor( 0, 0 ),
	mDrawGrid( true ),
	mGridColour( *wxWHITE ),
	mGridPoints( NULL ),
	mPointsNumber( 0 ),
	mGridPen( mGridColour ),
	mGridLogic( wxCOPY ),
	mCurrentColour( *wxBLACK ),
	mPreviousPoint( -1, -1),
	mAllowEdit( true )
{
}

EditPanel::~EditPanel(void)
{
	ClearGridPoints();
}

inline void EditPanel::ClearGridPoints()
{
	if ( mPointsNumber != 0 )
	{
		delete[] mGridPoints;
		mPointsNumber = 0;
	}
}

void EditPanel::SetGridColour(const wxColour& color)
{
	mGridColour = color;
	mGridPen.SetColour( mGridColour );
}



void EditPanel::SetGridLogic(wxInt32 logic)
{
	mGridLogic = logic;
}



/* virtual */ void EditPanel::Render(wxDC& dc)
{
	if (!IsOk())
	{
		return;
	}

	DrawPanel::Render( dc );

	if (mDrawGrid)
	{
		DrawGrid( dc );
	}

	dc.SetBrush( *wxTRANSPARENT_BRUSH );

	if (mDrawFocus)
	{
		dc.SetLogicalFunction(wxCOPY);
		wxRect rect = this->GetClientRect();
		if (rect.GetWidth() > mShowWidth)
		{
			rect.SetWidth( mShowWidth );
		}
		if (rect.GetHeight() > mShowHeight)
		{
			rect.SetHeight( mShowHeight );
		}
		wxPen borderPen( this->HasFocus() ? *wxRED : *wxWHITE, 3, wxDOT_DASH );
		dc.SetPen( borderPen );
		rect.SetLeftTop( this->GetViewStart() );
		dc.DrawRectangle(rect);
	}

	if (mDrawCursor)
	{
		dc.SetLogicalFunction(wxXOR);
		dc.SetPen( wxPen( *wxWHITE, 3, wxSOLID ) );
		wxPoint cursPos( mCursor );
		cursPos.x *= mRealScale;
		cursPos.y *= mRealScale;
		dc.DrawRectangle( cursPos, wxSize( mRealScale, mRealScale ) );
	}
}



/* virtual */ void EditPanel::SetShowParams()
{
	DrawPanel::SetShowParams();
	ClearGridPoints();

	if (mRealScale < GRID_EDGE)
	{
		// ничего хорошего не нарисуется, один хрен
		return;
	}

	wxSize bounds = this->GetClientSize();
	bool correctX = false;
	bool correctY = false;

	if (bounds.GetWidth() > mShowWidth)
	{
		bounds.SetWidth( mShowWidth );
		correctX = true;
	}

	if (bounds.GetHeight() > mShowHeight)
	{
		bounds.SetHeight( mShowHeight );
		correctY = true;
	}

	wxCoord gridWidth = bounds.GetWidth();
	wxCoord gridHeight = bounds.GetHeight();
	wxCoord width = 1 + ceil( (wxDouble) gridWidth / mRealScale );
	wxCoord height = 1 + ceil( (wxDouble) gridHeight / mRealScale );
	if (width < 2 || height < 2)
	{
		return;
	}

	mPointsNumber = (width + height) * 2;
	mGridPoints = new wxPoint[mPointsNumber];
	wxDouble lx = 0, ly = 0;
	wxCoord startX = 0, startY = 0;
	bounds = this->GetClientSize();

	/*if (correctX)
	{
		startX = mRealScale;
		lx += startX;
	}
	else
	{
		gridWidth += ceil(mRealScale);
	}

	if (correctY)
	{
		startY = mRealScale;
		ly += startY;
	}
	else
	{
		gridHeight += ceil(mRealScale);
	}*/

	wxCoord endX = startX + gridWidth, endY = startY + gridHeight;
	int count = 0;

	for (wxCoord y = 0; y < height; ++y)
	{
		mGridPoints[count++] = wxPoint(startX, ly);
		mGridPoints[count++] = wxPoint(endX, ly);
		ly += mRealScale;
	}

	for (wxCoord x = 0; x < width; ++x)
	{
		mGridPoints[count++] = wxPoint(lx, startY);
		mGridPoints[count++] = wxPoint(lx, endY);
		lx += mRealScale;
	}
}



void EditPanel::DrawGrid( wxDC& dc )
{
	if (mPointsNumber == 0)
	{
		return;
	}
	dc.SetPen( mGridPen );
	dc.SetLogicalFunction( (wxRasterOperationMode) mGridLogic );
	wxPoint view = this->GetViewStart();
	float horFloat = (float) view.x / mRealScale;
	float vertFloat = (float) view.y / mRealScale;
	int horizCorr = ( ceil(horFloat) * mRealScale );
	int vertCorr = ( ceil(vertFloat) * mRealScale );
	wxPoint corrPoint( horizCorr, vertCorr );
	for (int i = 0; i < mPointsNumber; )
	{
		wxPoint& first = mGridPoints[i++];
		wxPoint& second = mGridPoints[i++];
		dc.DrawLine( first + corrPoint, second + corrPoint);
	}
}

void EditPanel::PlacePixel( const wxPoint& pos, const UttColour& color )
{
	if (mRealScale < 1.0f)
	{
		wxLogMessage( "Unable to edit image with such scale.");
		EndDrawing();
		return;
	}
	
	if (mIndexMask)
	{
		mIndexMask->WriteIndex(pos, color.GetIndex());
	}
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*mBitmap);
	temp_dc.SetPen( wxPen(color) );
	temp_dc.DrawPoint ( pos );
	PaintNow();
}

bool EditPanel::GetPixel( const wxPoint& pos, UttColour& color )
{
	int n = -1;
	if (mIndexMask)
	{
		 n = mIndexMask->ReadIndex(pos);
	}
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*mBitmap);

	bool res = temp_dc.GetPixel( pos , &color );
	color.SetIndex( n );
	return res;
}

/* virtual */ bool EditPanel::MouseButton( int btn, bool up )
{
	if ( mDrawing && up )
	{
		EndDrawing();
		return true;
	}

	if ( DrawPanel::MouseButton( btn, up ) )
	{
		return true;
	}

	bool left = btn == wxMOUSE_BTN_LEFT;
	bool right = btn == wxMOUSE_BTN_RIGHT;

	if (mMousePoint.x == -1 || mMousePoint.y == -1 || PointInZone( mMousePoint ) || !this->HasFocus() )
	{
		return false;
	}

	if (left || right)
	{
		mCurrentColour = right ? gGlobalRightColour : gGlobalLeftColour;
	}

	if ( !up && ( left || right ) )
	{
		return BeginDrawing();
	}
	return false;
}



/* virtual */ bool EditPanel::MouseModifiersButton( int modifier, int btn, bool up )
{
	if ( DrawPanel::MouseModifiersButton( modifier, btn, up ) )
	{
		return true;
	}
	bool left = btn == wxMOUSE_BTN_LEFT;
	bool right = btn == wxMOUSE_BTN_RIGHT;
	bool both = left && right;
	bool findColour = !up && !both && modifier == wxMOD_SHIFT;
	bool setColour =  !up && !both && modifier == wxMOD_ALT;
	if ( findColour || setColour )
	{
		UttColour colour;
		if ( mMousePoint.x != -1 && mMousePoint.y != -1 && GetPixel(mMousePoint, colour) )
		{
			ColourPickEvent* colourEvent = new ColourPickEvent( colour, btn,
				findColour ? ColourPickEvent::cpeFindThisColour : ColourPickEvent::cpeSetThisColour );
			wxEvtHandler::QueueEvent( colourEvent );
			return true;
		}
	}
	return false;
}



/* virtual */ bool EditPanel::MouseMoving( int modifier, int btn )
{
	if ( DrawPanel::MouseMoving( modifier, btn ) )
	{
		return true;
	}
	if ( this->HasFocus() && mBitmapRect.Contains( mMousePoint ) && mMousePoint != mPreviousPoint)
	{
		mPreviousPoint = mCursor;
		mCursor = mMousePoint;
		if (!DoEdit())
		{
			PaintNow();
		}
		return true;
	}
	return false;
}



/* virtual */ bool EditPanel::KeyDown( int modifier, int keyCode )
{
	if ( DrawPanel::KeyDown( modifier, keyCode ) )
	{
		return true;
	}
	bool res = false;
	switch ( keyCode )
	{
		case WXK_V:
			if ( modifier == wxMOD_CONTROL )
			{	
				res = PasteSelection();
			}
		break;

		case WXK_NUM_ONE:
		case WXK_NUM_TWO:
			mCurrentColour = keyCode == WXK_NUM_TWO ? gGlobalRightColour : gGlobalLeftColour;
			res = BeginDrawing();
		break;

		case WXK_LEFT:
		case WXK_RIGHT:
		case WXK_UP:
		case WXK_DOWN:
			CursorPressed( keyCode );
			res = true;
		break;

		default:
		break;
	}
	return res;
}



/* virtual */ bool EditPanel::KeyUp( int modifier, int keyCode )
{
	if ( DrawPanel::KeyDown( modifier, keyCode ) )
	{
		return true;
	}
	bool res = false;
	switch ( keyCode )
	{
		case WXK_NUM_ONE:
		case WXK_NUM_TWO:
			EndDrawing();
			res = true;
		break;

		default:
		break;
	}
	return res;
}



/* virtual */ bool EditPanel::CursorPressed( int directon )
{
	wxPoint dir( 0, 0 );
	switch ( directon )
	{
		case WXK_LEFT:
			dir.x = -1;
		break;

		case WXK_RIGHT:
			dir.x = 1;
		break;

		case WXK_UP:
			dir.y = -1;
		break;

		case WXK_DOWN:
			dir.y = 1;
		break;

		default:
            return false;
		break;
	}
	if (mBitmapRect.Contains( mCursor + dir ))
	{
		mCursor += dir;
	}
	if (!DoEdit())
	{
		PaintNow();
	}
	return true;
}



bool EditPanel::DoEdit()
{
	if (!mAllowEdit || !mDrawing)
	{
		return false;
	}
	PlacePixel( mCursor, mCurrentColour );
	return true;
}



bool EditPanel::BeginDrawing()
{
	mDrawing = mAllowEdit;
	DoEdit();
	return mDrawing;

}



void EditPanel::EndDrawing()
{
	mDrawing = false;
}




bool EditPanel::PasteSelection()
{
	bool res = false;

	if ( !IsOk() || !mAllowEdit )
	{
		return res;
	}
	
	if ( wxTheClipboard->Open() )
	{
		wxImage img = mBitmap->ConvertToImage();
		if ( wxTheClipboard->IsSupported( wxDF_BITMAP ) && img.IsOk() )
		{
			wxBitmapDataObject data;
			wxTheClipboard->GetData( data );
			img.Paste( data.GetBitmap().ConvertToImage(), mCursor.x, mCursor.y );
			DestroyBitmap();
			SetBitmap( new wxBitmap( img ) );
			res = true;
		}
		wxTheClipboard->Close();
	}
	return res;
}
