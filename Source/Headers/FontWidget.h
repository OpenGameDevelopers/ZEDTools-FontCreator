#ifndef __ZEDTOOL_FONTCREATOR_FONTWIDGET_H__
#define __ZEDTOOL_FONTCREATOR_FONTWIDGET_H__

#include <QWidget>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

typedef struct __tagFONT_RENDER
{
	int a;
	FT_Face	Face;
	QRect	Rect;
}FONT_RENDER;

class FontWidget : public QWidget
{
	Q_OBJECT
public:
	FontWidget( QString &p_FontFile, QWidget *p_pParent = 0 );
	~FontWidget( );

private:
	typedef std::vector< FONT_RENDER > FontArray;
	void paintEvent( QPaintEvent *p_pPaintEvent );

	FT_Library		m_FTLibrary;
	FontArray		m_Faces;
};

#endif // __ZEETOOL_FONTCREATOR_FONTWIDGET_H__

