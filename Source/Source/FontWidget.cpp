#include <FontWidget.h>
#include <QPainter>
#include <QPen>

#define TRUNC(x) ((x) >> 6)

FontWidget::FontWidget( QString &p_FontFile, const int p_Width,
	const int p_Height, QWidget *p_pParent )
{
	setMinimumSize( p_Width, p_Height );
	setMaximumSize( p_Width, p_Height );

	m_Width = p_Width;
	m_Height = p_Height;

	m_Padding = 3;

	FT_Error Error = FT_Err_Ok;

	m_FTLibrary = 0;

	Error  = FT_Init_FreeType( &m_FTLibrary );

	if( !Error )
	{
		for( int i = 0; i < 26+6+26; ++i )
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

		Painter.fillRect( Painter.window( ),
			QBrush( QColor( 0, 0, 0, 255 ) ) );
		QPen OutlinePen( QColor( 0, 255, 0, 255 ), 1 );
		QPen BaseLinePen( QColor( 255, 0, 0, 255 ), 1 );
		QPen AdvancePen( QColor( 0, 255, 255, 255 ), 1 );

		int CurrentMaxHeight = 0;
		int CurrentXPosition = m_Padding;
		int CurrentYPosition = 0;

		Painter.translate( m_Padding, m_Padding );

		for( ; FaceItr != m_Faces.end( ); ++FaceItr )
		{
			Error = FT_Render_Glyph( ( *FaceItr ).Face->glyph,
				FT_RENDER_MODE_NORMAL );

			CurrentXPosition += ( ( *FaceItr ).Face->glyph->bitmap.width ) +
				m_Padding;

			if( CurrentXPosition > m_Width )
			{
				printf( "Reset\n" );
				Painter.resetTransform( );
				CurrentYPosition += CurrentMaxHeight;
				Painter.translate( m_Padding, CurrentYPosition + m_Padding );
				CurrentXPosition = m_Padding +
					( *FaceItr ).Face->glyph->bitmap.width;
				CurrentMaxHeight = 0;
			}

			QImage GlyphImage( ( *FaceItr ).Face->glyph->bitmap.buffer,
				( *FaceItr ).Face->glyph->bitmap.width,
				( *FaceItr ).Face->glyph->bitmap.rows,
				( *FaceItr ).Face->glyph->bitmap.pitch,
				QImage::Format_Indexed8 );

			QVector< QRgb > ColourTable;
			for( int i = 0; i < 256; ++i )
			{
				ColourTable << qRgba( i, i, i, i );
			}
			GlyphImage.setColorTable( ColourTable );

			Painter.drawImage( QPoint( 0, 0 ), GlyphImage );

			Painter.setPen( OutlinePen );
			// Bottom line
			Painter.drawLine( ( *FaceItr ).Rect.x( ),
				( *FaceItr ).Rect.height( ),
				( *FaceItr ).Rect.width( ), ( *FaceItr ).Rect.height( ) );
			// Top line
			Painter.drawLine( ( *FaceItr ).Rect.x( ),
				( *FaceItr ).Rect.y( ), ( *FaceItr ).Rect.width( ),
				( *FaceItr ).Rect.y( ) );
			// Left line
			Painter.drawLine( ( *FaceItr ).Rect.x( ),
				( *FaceItr ).Rect.y( ), ( *FaceItr ).Rect.x( ),
				( *FaceItr ).Rect.height( ) );
			// Right line
			Painter.drawLine( ( *FaceItr ).Rect.width( ),
				( *FaceItr ).Rect.y( ), ( *FaceItr ).Rect.width( ),
				( *FaceItr ).Rect.height( ) );
			
			Painter.setPen( BaseLinePen );
			// Base line
			Painter.drawLine( ( *FaceItr ).Rect.x( ),
				( *FaceItr ).Face->glyph->metrics.horiBearingY / 64,
				( *FaceItr ).Rect.width( ),
				( *FaceItr ).Face->glyph->metrics.horiBearingY / 64);

			Painter.setPen( AdvancePen );
			// Advance
			Painter.drawLine( ( *FaceItr ).Rect.x( ),
				( *FaceItr ).Rect.height( )+10,
				( *FaceItr ).Face->glyph->metrics.horiAdvance / 64,
				( *FaceItr ).Rect.height( )+10 );

			if( ( *FaceItr ).Rect.height( ) > CurrentMaxHeight )
			{
				CurrentMaxHeight = ( *FaceItr ).Rect.height( );
			}

			Painter.translate( 
				( *FaceItr ).Face->glyph->bitmap.width + m_Padding, 0 );
		}
	}
}

