#ifndef __ZEDTOOL_FONTCREATOR_FONTWIDGET_H__
#define __ZEDTOOL_FONTCREATOR_FONTWIDGET_H__

#include <QWidget>
#include <vector>
#include <FontFile.h>
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

	FontFile GetFontFile( ) const;

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

	QString	m_FontFileName;
	QString	m_Characters;

	FontFile	m_FontFile;
};

#endif // __ZEDTOOL_FONTCREATOR_FONTWIDGET_H__

