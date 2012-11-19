/***************************************************************
 * Name:      indexmask.cpp
 * Purpose:   Mask container for Bitmap with indexed palette
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-11-18
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/

#include "pch.h"
#include "indexmask.h"
#include "palette.h"
#include "helpers.h"



IndexMask::IndexMask( const IndexMask& other ):
	mWidth( 0 ),
	mHeight( 0 ),
	mSize( 0 ),
	mSrcWidth( 0 ),
	mSrcHeight( 0 ),
	mMask( NULL )
{
	SetMask( other.mMask, other.mWidth, other.mHeight, other.mSrcWidth, other.mSrcHeight );
}



IndexMask::~IndexMask()
{
	Clear();
}



void IndexMask::SetMask( const char* mask, int width, int height, int srcWidth /* -1 */, int srcHeight /* -1 */ )
{
	Clear();
	mWidth = width;
	mHeight = height;
	mSrcWidth = srcWidth;
	mSrcHeight = srcHeight;

	if (mSrcWidth == -1)
	{
		mSrcWidth = mWidth;
	}

	if (mSrcHeight == -1)
	{
		mSrcWidth = mHeight;
	}

	mSize = mWidth * mHeight;
	mMask = (char*) malloc( mSize );
	Helpers::CopyBuffer<char>( mMask, mWidth, mHeight, mask, mSrcWidth, mSrcHeight );
}



wxBitmap* IndexMask::GetBitmap( Palette* pal )
{
	wxBitmap* res = NULL;
	Pixel* buf = new Pixel[mSize];
	bool success = true;

	switch (pal->GetPalType())
	{
		case Palette::bpp8:
			Helpers::Buffer8bpp_to_Pixels(buf, mWidth, mHeight, mMask, mWidth, mHeight, pal);
		break;

		default:
			success = false;
			wxLogError("IndexMask::GetBitmap: unsupported palette type!");
	}
	
	if (success)
	{
		res = Helpers::CreateBitmap(buf, mWidth, mHeight);
	}

	delete[] buf;
	return res;
}



void IndexMask::Clear()
{
	if (mMask != NULL)
	{
		free(mMask);
		mMask = NULL;
	}
}



bool IndexMask::WriteIndex( const wxPoint& pos, int n )
{
	wxASSERT( n < 256 && pos.x < mWidth && pos.y < mHeight );
	mMask[ (pos.y * mWidth) + pos.x ] = (char) n;
	return true;
}



int IndexMask::ReadIndex( const wxPoint& pos )
{
	wxASSERT( pos.x < mWidth && pos.y < mHeight );
	return mMask[ (pos.y * mWidth) + pos.x ];
}
