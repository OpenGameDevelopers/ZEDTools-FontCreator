#include <FontWidget.h>
#include <QPainter>

#define TRUNC(x) ((x) >> 6)

FontWidget::FontWidget( QString &p_FontFile, QWidget *p_pParent )
{
	FT_Error Error = FT_Err_Ok;

	m_FTLibrary = 0;

	Error  = FT_Init_FreeType( &m_FTLibrary );

	if( !Error )
	{
		for( int i = 0; i < 26; ++i )
		{
			FONT_RENDER FontRender;
			Error = FT_New_Face( m_FTLibrary,
				p_FontFile.toUtf8( ).constData( ), 0, &FontRender.Face );

			if( !Error )
			{
				Error = FT_Set_Char_Size( FontRender.Face, 0, 24*64,
					physicalDpiX( ), physicalDpiY( ) );

				if( !Error )
				{

					FT_UInt16 GlyphIndex = 0;
					GlyphIndex = FT_Get_Char_Index( FontRender.Face, 'A'+i );

					Error = FT_Load_Glyph( FontRender.Face, GlyphIndex,
						FT_LOAD_DEFAULT );

					if( !Error )
					{
						QRect Rect;
						FT_Pos Left =
							FontRender.Face->glyph->metrics.horiBearingX;
						FT_Pos Right = Left +
							FontRender.Face->glyph->metrics.width;
						FT_Pos Top =
							FontRender.Face->glyph->metrics.horiBearingY;
						FT_Pos Bottom = Top -
							FontRender.Face->glyph->metrics.height;

						Rect = QRect( QPoint( TRUNC( Left ),
							-TRUNC( Top ) + 1 ),
							QSize( TRUNC( Right - Left ) + 1,
							TRUNC( Top - Bottom ) + 1 ) );

						printf( "Adding %c\n", 'A'+i );

						FontRender.Rect = Rect;

						m_Faces.push_back( FontRender );
					}
				}
			}
		}
	}
}

FontWidget::~FontWidget( )
{
	FontArray::const_iterator FaceItr = m_Faces.begin( );

	while( FaceItr != m_Faces.end( ) )
	{
		FT_Done_Face( ( *FaceItr ).Face );
		++FaceItr;
	}

	FT_Done_FreeType( m_FTLibrary );
}

void FontWidget::paintEvent( QPaintEvent *p_pPaintEvent )
{
	printf( "Repainting\n" );
	QWidget::paintEvent( p_pPaintEvent );

	if( m_FTLibrary )
	{
		printf( "Painting char\n" );
		FT_Error Error = FT_Err_Ok;

		QPainter Painter( this );

		FontArray::const_iterator FaceItr = m_Faces.begin( );

		int PreviousX = 0;
		Painter.fillRect( Painter.window( ),
			QBrush( QColor( 0, 0, 0, 255 ) ) );

		for( ; FaceItr != m_Faces.end( ); ++FaceItr )
		{
			printf( "Drawing at: %d %d\n", -( *FaceItr ).Rect.x( ),
				-( *FaceItr ).Rect.y( ) );
			Painter.translate( -( *FaceItr ).Rect.x( ),
				-( *FaceItr ).Rect.y( ) );

			Error = FT_Render_Glyph( ( *FaceItr ).Face->glyph,
			FT_RENDER_MODE_NORMAL );

			QImage GlyphImage( ( *FaceItr ).Face->glyph->bitmap.buffer,
				( *FaceItr ).Face->glyph->bitmap.width,
				( *FaceItr ).Face->glyph->bitmap.rows,
				( *FaceItr ).Face->glyph->bitmap.pitch,
				QImage::Format_Indexed8 );

			Painter.translate( PreviousX,
				( *FaceItr ).Rect.y( ) );
			printf( "Translating to: %d %d\n", PreviousX,
				( *FaceItr ).Rect.y( ) );

			QVector< QRgb > ColourTable;
			for( int i = 0; i < 256; ++i )
			{
				ColourTable << qRgba( i, i, i, i );
			}
			GlyphImage.setColorTable( ColourTable );

			Painter.drawImage( QPoint( 0, 0 ), GlyphImage );
			PreviousX = ( *FaceItr ).Rect.width( )*2;
		}
	}
}

