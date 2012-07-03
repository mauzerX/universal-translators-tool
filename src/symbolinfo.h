/***************************************************************
 * Name:      symbolinfo.h
 * Purpose:   declaration of SymbolInfo class
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-07-02
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/
#ifndef SYMBOLINFO_H_INCLUDED
#define SYMBOLINFO_H_INCLUDED


struct SymbolInfo
{

	explicit SymbolInfo():
		mWidth( 0 ),
		mHeight( 0 ),
		mCode( 0 ),
		mData( NULL )
	{
	}

	RGBA GetPixel( int x, int y );
	void SetPixel( int x, int y, RGBA color );

	virtual ~SymbolInfo()
	{
		EraseData();
	}

	LetterBox* GetData();
	void SetData(const char* data = NULL);

	void SetValues(wxInt32 width, wxInt32 height, wxUint32 code, const char* data = NULL);

	SymbolInfo &operator = ( const SymbolInfo &src );

	wxInt32	mWidth;											// ������ �������
	wxInt32 mHeight;										// ������ �������
	wxUint32 mCode;											// ��� �������, ��� ������� 32 � �.�.

protected:

	LetterBox* mData;										// ������ �������, � protected - ���
															// �������� �� ��������������

private:

	void CreateData();
	void EraseData();
	int BoxOffset(int x, int y);
};

typedef wxVector<SymbolInfo> Symbols;



#endif // SYMBOLINFO_H_INCLUDED
