#ifndef __ZEDTOOL_FONTCREATOR_FONTWIDGET_H__
#define __ZEDTOOL_FONTCREATOR_FONTWIDGET_H__

#include <QWidget>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

typedef struct __tagFONT_RENDER
{
	FT_Face	Face;
	QRect	Rect;
	int		YOffset;
	int		Overhang;
}FONT_RENDER;

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

class QImage;

class FontWidget : public QWidget
{
	Q_OBJECT
public:
	FontWidget( );
	FontWidget( QString &p_FontFile, const int p_Width, const int p_Height,
		const int p_FontSize = 16, QWidget *p_pParent = 0 );
	~FontWidget( );

	int Initialise( );

	void SetDimensions( const int p_Width, const int p_Height );
	void SetPointSize( const int p_PointSize );
	void SetFont( const QString &p_FontFile );
	void SetPadding( const int p_Padding );

	void CreateGlyphs( const QString &p_Characters );

private:
	typedef std::vector< FONT_RENDER > FontArray;
	typedef std::vector< FONTFILE_GLYPH > GlyphArray;
	void paintEvent( QPaintEvent *p_pPaintEvent );
	void WriteGlyphArray( );

	void RegenerateGlyphs( );

	FT_Library		m_FTLibrary;
	FontArray		m_Faces;
	GlyphArray		m_GlyphArray;

	int		m_Padding;
	int		m_Width;
	int		m_Height;
	int		m_PointSize;

	QString	m_FontFile;
	QString	m_Characters;

};

#endif // __ZEETOOL_FONTCREATOR_FONTWIDGET_H__

