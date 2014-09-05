#ifndef __ZEDTOOL_FONTCREATOR_FONTFILE_H__
#define __ZEDTOOL_FONTCREATOR_FONTFILE_H__

#include <stdint.h>
#include <cstddef>
#include <vector>
#include <QImage>

typedef enum __tagFONT_TYPE
{
	BITMAP_FONT = 1,
	VECTOR_FONT,
	SIGNED_DISTANCE_FONT
}FONT_TYPE;

typedef enum __tagFONT_BACKGROUNDMODE
{
	COLOURKEY_BACKGROUND = 1,
	SOLID_BACKGROUND,
	TRANSPARENT_BACKGROUND,
	TRANSLUCENT_BACKGROUND
}FONT_BACKGROUNDMODE;

#pragma pack( 1 )
typedef struct __tagCOLOURKEY
{
	float	Red;
	float	Green;
	float	Blue;
	float	Alpha;
}COLOURKEY;

typedef struct __tagFONTFILE_HEADER
{
	char				ID[ 4 ];
	FONT_TYPE			Type;
	FONT_BACKGROUNDMODE	BackgroundMode;
	COLOURKEY			ColourKey;

}FONTFILE_HEADER;

typedef struct __tagFONTFILE_GLYPH
{
	char	Character;
	uint32_t	X;
	uint32_t	Y;
	uint32_t	Width;
	uint32_t	Height;
	uint32_t	BearingY;
}FONTFILE_GLYPH;
#pragma pack( )

class FontFile
{
public:
    FontFile( );
    ~FontFile( );

    int Write( const char *p_pFilePath );

    void ClearGlyphs( );
    void ClearTarga( );
    void AddGlyph( const FONTFILE_GLYPH &p_Glyph );
    void SetQImage( const QImage &p_Image );

private:
    typedef std::vector< FONTFILE_GLYPH > GlyphArray;

    unsigned char	*m_pTarga;
    size_t		m_TargaSize;
    GlyphArray		m_GlyphArray;
};

#endif // __ZEDTOOL_FONTCREATOR_FONTFILE_H__

