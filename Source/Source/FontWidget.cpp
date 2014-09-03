#include <FontWidget.h>
#include <QPainter>
#include <QImage>
#include <QPen>
#include <Targa.h>

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
	QWidget::paintEvent( p_pPaintEvent );

	if( m_FTLibrary )
	{
		FT_Error Error = FT_Err_Ok;

		m_SpriteFont = QImage( this->size( ), QImage::Format_ARGB32 );
		QPainter OverlayPainter;
		QImage OverlayImage = QImage( this->size( ), QImage::Format_ARGB32 );
		OverlayPainter.begin( &OverlayImage );

		QPainter Painter;

		Painter.begin( &m_SpriteFont );

		FontArray::iterator FaceItr = m_Faces.begin( );

		Painter.fillRect( Painter.window( ),
			QBrush( QColor( 0, 0, 0, 0 ) ) );
		QPen OutlinePen( QColor( 0, 255, 0, 255 ), 1 );
		QPen BaseLinePen( QColor( 255, 0, 0, 255 ), 1 );
		QPen AdvancePen( QColor( 0, 255, 255, 255 ), 1 );

		int CurrentMaxHeight = 0;
		int CurrentXPosition = m_Padding;
		int CurrentYPosition = 0;

		Painter.translate( m_Padding, m_Padding );
		OverlayPainter.fillRect( OverlayPainter.window( ),
			QBrush( QColor( 0, 0, 0, 0 ) ) );
		OverlayPainter.translate( m_Padding, m_Padding );

		char GlyphChar = 'A';

		FontArray::iterator BeginningItr = FaceItr;
		std::vector< int > LineSpacing;
		LineSpacing.push_back( 0 );
		int MaxTop = 0;
		int MaxBottom = 0;
		for( ; FaceItr != m_Faces.end( ); ++FaceItr )
		{
			Error = FT_Render_Glyph( ( *FaceItr ).Face->glyph,
				FT_RENDER_MODE_NORMAL );

			CurrentXPosition += ( ( *FaceItr ).Face->glyph->bitmap.width ) +
				m_Padding;

			if( CurrentXPosition > m_Width )
			{
				FontArray::iterator EndItr = FaceItr;
				CurrentXPosition = m_Padding +
					( *FaceItr ).Face->glyph->bitmap.width;

				// THERE'S SOMETHING WRONG HERE, FOR SEGA SATURN'S '_', IT
				// SHOULD BE 32, BUT IT'S 27 FOR THE MAX HEIGHT
				CurrentMaxHeight = MaxTop + MaxBottom;
				printf( "Max Height: %d\n", CurrentMaxHeight );
				for( ; BeginningItr != EndItr; ++BeginningItr )
				{
					( *BeginningItr ).YOffset = CurrentMaxHeight -
						( *BeginningItr ).Face->glyph->metrics.horiBearingY /
							64;
					printf( "Char: %c\n", GlyphChar++ );
					printf( "Y Offset: %d\n", ( *BeginningItr ).YOffset );
					printf( "Height: %d\n",
						( *BeginningItr ).Face->glyph->bitmap.rows );
					printf( "Bearing Y: %d\n",
						( *BeginningItr ).Face->glyph->metrics.horiBearingY /
							64 );
					printf( "Max top: %d\n", MaxTop );
					printf( "Max bottom: %d\n", MaxBottom );
					printf( "Current max height: %d\n", CurrentMaxHeight );
				}

				printf( "Line spacing: %d\n", MaxBottom );
				LineSpacing.push_back( MaxBottom );
				MaxTop = 0;
				MaxBottom = 0;
				if( EndItr != m_Faces.end( ) )
				{
					BeginningItr = EndItr;
				}
			}

			int Top = ( *FaceItr ).Face->glyph->metrics.horiBearingY / 64;
			int Bottom = 0;
			if( ( ( *FaceItr ).Face->glyph->metrics.horiBearingY / 64 ) > 0 )
			{
				Bottom = ( *FaceItr ).Face->glyph->bitmap.rows -
					( ( *FaceItr ).Face->glyph->metrics.horiBearingY / 64 );
			}

			if( ( ( *FaceItr ).Face->glyph->metrics.horiBearingY / 64 ) == 0 )
			{
				Bottom = ( *FaceItr ).Face->glyph->bitmap.rows;

				printf( "BOTTOM: \t\t\t\t\t%d\n", Bottom );
			}

			if( Top > MaxTop )
			{
				MaxTop = Top;
			}
			if( Bottom > MaxBottom )
			{
				MaxBottom = Bottom;
			}
		}

		// Process the last row
		if( BeginningItr != m_Faces.end( ) )
		{
			CurrentMaxHeight = MaxTop + MaxBottom;
			printf( "Max Height: %d\n", CurrentMaxHeight );
			for( ; BeginningItr != m_Faces.end( ); ++BeginningItr )
			{
				( *BeginningItr ).YOffset = CurrentMaxHeight -
					( *BeginningItr ).Face->glyph->metrics.horiBearingY / 64;
				printf( "Char: %c\n", GlyphChar++ );
				printf( "Y Offset: %d\n", ( *BeginningItr ).YOffset );
				printf( "Row count: %d\n",
					( *BeginningItr ).Face->glyph->bitmap.rows );
				printf( "BearingY: %d\n",
					( *BeginningItr ).Face->glyph->metrics.horiBearingY / 64 );
			}

			printf( "Line spacing: %d\n", MaxBottom );
			LineSpacing.push_back( MaxBottom );
		}

		CurrentXPosition = m_Padding;
		CurrentYPosition = m_Padding;

		FaceItr = m_Faces.begin( );

		std::vector< int >::const_iterator LineSpace = LineSpacing.begin( );

		printf( "Line spacing count: %d\n", LineSpacing.size( ) );

		for( ; FaceItr != m_Faces.end( ); ++FaceItr )
		{
			Error = FT_Render_Glyph( ( *FaceItr ).Face->glyph,
				FT_RENDER_MODE_NORMAL );

			CurrentXPosition += ( ( *FaceItr ).Face->glyph->bitmap.width ) +
				m_Padding;

			if( CurrentXPosition > m_Width )
			{
				printf( "Current line spacing: %d\n", ( *LineSpace ) );
				++LineSpace;
				printf( "New line spacing: %d\n", ( *LineSpace ) );
				Painter.resetTransform( );
				OverlayPainter.resetTransform( );
				CurrentMaxHeight = MaxTop + MaxBottom;
				CurrentYPosition += CurrentMaxHeight;
				Painter.translate( m_Padding, CurrentYPosition + m_Padding +
					( *LineSpace ) );
				OverlayPainter.translate( m_Padding,
					CurrentYPosition + m_Padding + ( *LineSpace ) );
				CurrentXPosition = m_Padding +
					( *FaceItr ).Face->glyph->bitmap.width;

				printf( "Position:             %d\n", CurrentYPosition +
					m_Padding + ( *LineSpace ) );
				printf( "Current Position:     %d\n", CurrentYPosition );
				printf( "Current Padding:      %d\n", m_Padding );
				printf( "Current Line Spacing: %d\n", ( *LineSpace ) );
				printf( "Current max height:   %d\n", CurrentMaxHeight );

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

			Painter.translate( 0, ( *FaceItr ).YOffset );
			OverlayPainter.translate( 0, ( *FaceItr ).YOffset );

			Painter.drawImage( QPoint( 0, 0 ), GlyphImage );

			OverlayPainter.setPen( OutlinePen );
			// Bottom line
			OverlayPainter.drawLine( 0, ( *FaceItr ).Face->glyph->bitmap.rows,
				( *FaceItr ).Face->glyph->bitmap.width,
				( *FaceItr ).Face->glyph->bitmap.rows );
			// Top line
			OverlayPainter.drawLine( 0, 0,
				( *FaceItr ).Face->glyph->bitmap.width, 0 );
			// Left line
			OverlayPainter.drawLine( 0, 0, 0,
				( *FaceItr ).Face->glyph->bitmap.rows );
			// Right line
			OverlayPainter.drawLine( ( *FaceItr ).Face->glyph->bitmap.width, 0,
				( *FaceItr ).Face->glyph->bitmap.width,
				( *FaceItr ).Face->glyph->bitmap.rows );
			
			OverlayPainter.setPen( BaseLinePen );
			// Base line
			OverlayPainter.drawLine( ( *FaceItr ).Rect.x( ),
				( *FaceItr ).Face->glyph->metrics.horiBearingY / 64,
				( *FaceItr ).Rect.width( ),
				( *FaceItr ).Face->glyph->metrics.horiBearingY / 64);

			OverlayPainter.setPen( AdvancePen );
			// Advance
			OverlayPainter.drawLine( 0,
				( *FaceItr ).Rect.height( ),
				( *FaceItr ).Face->glyph->metrics.horiAdvance / 64,
				( *FaceItr ).Rect.height( ) );


			int Top = ( *FaceItr ).Face->glyph->metrics.horiBearingY / 64;
			int Bottom = ( *FaceItr ).Face->glyph->bitmap.rows -
				( ( *FaceItr ).Face->glyph->metrics.horiBearingY / 64 );

			if( Top > MaxTop )
			{
				MaxTop = Top;
			}
			if( Bottom > MaxBottom )
			{
				MaxBottom = Bottom;
			}

			Painter.translate( 
				( *FaceItr ).Face->glyph->bitmap.width + m_Padding,
				-( *FaceItr ).YOffset );
			OverlayPainter.translate( 
				( *FaceItr ).Face->glyph->bitmap.width + m_Padding,
				-( *FaceItr ).YOffset );
		}

		Painter.end( );
		OverlayPainter.end( );

		QPainter WidgetPainter( this );

		QRect Source( 0.0f, 0.0f, this->width( ), this->height( ) );

		WidgetPainter.fillRect( WidgetPainter.window( ),
			QBrush( QColor( 0, 0, 0, 255 ) ) );

		WidgetPainter.drawImage( Source, m_SpriteFont, Source );
		WidgetPainter.drawImage( Source, OverlayImage, Source );

		WriteQImageToTarga( m_SpriteFont, "Sprite.tga" );
	}
}

