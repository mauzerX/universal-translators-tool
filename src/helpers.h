/***************************************************************
 * Name:      helpers.h
 * Purpose:   some global functions
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-03-14
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/
#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

	//forward declarations
class Palette;

namespace Helpers
{

extern wxWindowID wxCustomPanelId;

wxBitmap* CreateBitmap(Pixel* buffer, int width, int height);

// �������� ����� � ����� � ��������������� �����������
// dst ������ ���� ������ src
// dst �� ����� - x
//	xxxooo
//	xxxooo
//	xxxooo
//	oooooo
template<typename T>
void CropBuffer(T* dst, int dstWidth, int dstHeight, const T* src, int srcWidth )
{
	memset( dst, 0, dstWidth * dstHeight * sizeof( T ) );
	size_t copyLength = sizeof( T ) * dstWidth;
	//size_t correction = srcWidth - copyLength;
	for (int  y = 0; y < dstHeight; ++y)
	{
		memcpy(dst, src, copyLength);
		dst += dstWidth;
		src += srcWidth;
	}
}



template<typename T>
void ExpandBuffer(T* dst, int dstWidth, int dstHeight, const T* src, int srcWidth, int srcHeight)
{
	memset( dst, 0, dstWidth * dstHeight * sizeof( T ) );
	size_t copyLength = sizeof( T ) * srcWidth;
	size_t correction = dstWidth - copyLength;
	for (int  y = 0; y < srcHeight; ++y)
	{
		memcpy(dst, src, copyLength);
		dst += correction;
		src += srcWidth;
	}
}



template<typename T>
void CopyBuffer(T* dst, int dstWidth, int dstHeight, const T* src, int srcWidth, int srcHeight)
{
	size_t dstSize = dstWidth * dstHeight;
	size_t srcSize = srcWidth * srcHeight;

	if (dstSize == srcSize)
	{
		memcpy( dst, src, srcSize * sizeof(T) );
		return;
	}

	if (dstSize < srcSize)
	{
		CropBuffer(dst, dstWidth, dstHeight, src, srcWidth );
	}
	else
	{
		ExpandBuffer(dst, dstWidth, dstHeight, src, srcWidth, srcHeight);
	}
}



template<typename T>
void CropSubBuffer(T* dst, int dstWidth, int dstHeight, const T* src, int srcWidth, int cx, int cy)
{
	memset( dst, 0, dstWidth * dstHeight * sizeof( T ) );
	size_t copyLength = sizeof( T ) * dstWidth;
	src += (cy * srcWidth) + cx;
	for (int  y = 0; y < dstHeight; ++y)
	{
		memcpy(dst, src, copyLength);
		dst += dstWidth;
		src += srcWidth;
	}
}



template<typename T>
void InsertSubBuffer(T* dst, int dstWidth,  
					 const T* src, int srcWidth, int srcHeight, int ix, int iy)
{
	size_t copyLength = sizeof( T ) * srcWidth;
	dst += (iy * dstWidth) + ix;
	for (int  y = 0; y < srcHeight; ++y)
	{
		memcpy(dst, src, copyLength);
		dst += dstWidth;
		src += srcWidth;
	}
}


bool CopyToClipboard( const wxImage& img );
bool CopyToClipboard( const wxRect& rect, const wxBitmap* bmp );

void Buffer8bpp_to_Pixels(Pixel*dst, int dstWidth, int dstHeight, const char* src, int srcWidth, int srcHeight, const Palette* pal );

} // namespace Helpers


#endif

