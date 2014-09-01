#ifndef __ZEDTOOL_FONTCREATOR_TARGA_H__
#define __ZEDTOOL_FONTCREATOR_TARGA_H__

// Takes a QImage and converts it to a 32-bit ARGB Targa
#include <QImage>

#pragma pack( 1 )
typedef struct __tagTARGA_HEADER
{
	unsigned char	IDLength;
	unsigned char	ColourmapType;
	unsigned char	ImageType;
	unsigned char	ColourmapSpecification[ 5 ];
	unsigned short	X;
	unsigned short	Y;
	unsigned short	Width;
	unsigned short	Height;
	unsigned char	BitsPerPixel;
	unsigned char	ImageDescription;
}TARGA_HEADER;
#pragma pack( )

bool WriteQImageToTarga( const QImage &p_Image, const QString p_Path );

#endif // __ZEDTOOL_FONTCREATOR_TARGA_H__

