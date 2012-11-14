/***************************************************************
 * Name:      fontinfo.h
 * Purpose:   declaration of FontInfo class
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-07-02
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/
#ifndef FONTINFO_H_INCLUDED
#define FONTINFO_H_INCLUDED

#include "symbolinfo.h"



class FontInfo
{

public:

	FontInfo():
		mMaxHeight( 0 ),
		mMinHeight( 0 ),
		mMaxWidth( 0 ),
		mMinWidth( 0 ),
		mBaseLine( 0 ),
		mCapLine( 0 ),
		mLowLine( 0 ),
		mBPP( BPP::bppMono ),
		mFontCodePage( 0 )
	{
		memcpy(mPalette, sVGApal, sizeof(mPalette));
	}

	FontInfo( const FontInfo& other ):
		mMaxHeight( other.mMaxHeight ),
		mMinHeight( other.mMinHeight ),
		mMaxWidth( other.mMaxWidth ),
		mMinWidth( other.mMinWidth ),
		mBaseLine( other.mBaseLine ),
		mCapLine( other.mCapLine ),
		mLowLine( other.mLowLine ),
		mBPP( other.mBPP ),
		mFontCodePage( other.mFontCodePage ),
		mSymbols( other.mSymbols )
	{
		memcpy(mPalette, other.mPalette, sizeof(mPalette));
	}

	FontInfo* Clone() const { return new FontInfo(*this); } 

	void SetValues( int maxWidth, int maxHeight, int minWidth, int minHeight,
					int bpp = BPP::bppMono,
					int fontCodePage = wxFONTENCODING_DEFAULT,
					int baseLine = 0,
					int capLine = 0,
					int lowLine = 0 );

	wxInt32 GetEncoding()
	{
		return mFontCodePage;
	}

	void SetEncoding(wxInt32 n)
	{
		mFontCodePage = n;
	}


	size_t GetSymbolsNum();
	void SetSymbolsNum(size_t n);



	SymbolInfo& GetSymbol(size_t n);


	Symbols& GetSymbols()
	{
		return mSymbols;
	}

	void SetSymbols(const Symbols& src)
	{
		mSymbols = src;
	}

	void AddSymbol( const char* data, int width, int height );

	wxInt32 GetMaxWidth()
	{
		return mMaxWidth;
	}

	void SetMaxWidth( wxInt32 width )
	{
		mMaxWidth = width;
	}

	wxInt32 GetMaxHeight()
	{
		return mMaxHeight;
	}

	void SetMaxHeight( wxInt32 height )
	{
		mMaxHeight = height;
	}

	wxInt32 GetMinWidth()
	{
		return mMinWidth;
	}

	void SetMinWidth( wxInt32 width )
	{
		mMinWidth = width;
	}

	wxInt32 GetMinHeight()
	{
		return mMinHeight;
	}

	void SetMinHeight( wxInt32 height )
	{
		mMinHeight = height;
	}

	wxInt32	GetBaseLine()
	{
		return mBaseLine;
	}

	void SetBaseLine(wxInt32 baseLine)
	{
		mBaseLine = baseLine;
	}

	wxInt32	GetCapLine()
	{
		return mCapLine;
	}

	void SetCapLine(wxInt32 capLine)
	{
		mCapLine = capLine;
	}

	wxInt32	GetLowLine()
	{
		return mLowLine;
	}

	void SetLowLine(wxInt32 lowLine)
	{
		mLowLine = lowLine;
	}

	void SetPalette(const char* src, bool shift);

	static SymbolInfo	sBadSymbol;

protected:

	wxInt32			mMaxHeight;							// ������������ ������
	wxInt32			mMinHeight;							// ����������� ������
	wxInt32			mMaxWidth;							// ������������ ������
	wxInt32			mMinWidth;							// ����������� ������
	wxInt32			mBaseLine;							// ������� ����� �������
	wxInt32			mCapLine;							// ����� ��������� ����
	wxInt32			mLowLine;							// ����� �������� ����
	wxInt32			mBPP;								// ��� �� �������
	wxInt32			mFontCodePage;						// ���������, ������������� ��������, ��� ������� ������������� ������
	Symbols			mSymbols;							// �������
	Palette         mPalette;							// �������

};



#endif // FONTINFO_H_INCLUDED
