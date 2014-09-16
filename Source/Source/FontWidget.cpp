#include <FontWidget.h>
#include <QPainter>
#include <QImage>
#include <QPen>
#include <Targa.h>

#define TRUNC(x) ((x) >> 6)

FontWidget::FontWidget( ) :
	m_Padding( 0 ),
	m_Width( 128 ),
	m_Height( 128 ),
	m_PointSize( 16 )
{
	setMinimumSize( m_Width, m_Height );
	setMaximumSize( m_Width, m_Height );
	m_FontFileName.clear( );
}

FontWidget::FontWidget( QString &p_FontFile, const int p_Width,
	const int p_Height, const int p_FontSize, QWidget *p_pParent ) :
	m_Padding( 0 )
{
	setMinimumSize( p_Width, p_Height );
	setMaximumSize( p_Width, p_Height );

	m_Width = p_Width;
	m_Height = p_Height;
}

FontWidget::~FontWidget( )
{
	if( m_FTLibrary )
	{
		FontArray::const_iterator FaceItr = m_Faces.begin( );

		while( FaceItr != m_Faces.end( ) )
		{
			FT_Done_Face( ( *FaceItr ).Face );
			++FaceItr;
		}

		FT_Done_FreeType( m_FTLibrary );
	}
}

int FontWidget::Initialise( )
{
	FT_Error Error = FT_Err_Ok;
	m_FTLibrary = 0;

	Error = FT_Init_FreeType( &m_FTLibrary );

	printf( "Error: %p\n", m_FTLibrary );

	if( !Error )
	{
		return 0;
	}

	return 1;
}

void FontWidget::SetDimensions( const int p_Width, const int p_Height )
{
	setMinimumSize( p_Width, p_Height );
	setMaximumSize( p_Width, p_Height );
	m_Width = p_Width;
	m_Height = p_Height;

	printf( "Width: %d\nHeight: %d\n", p_Width, p_Height );
}

void FontWidget::SetPointSize( const int p_PointSize )
{
	m_PointSize = p_PointSize;

	this->RegenerateGlyphs( );
}

void FontWidget::SetFont( const QString &p_FontFile )
{
	m_FontFileName = p_FontFile;

	printf( "Setting font\n" );

	this->RegenerateGlyphs( );
}

void FontWidget::SetPadding( const int p_Padding )
{
	m_Padding = p_Padding;

	this->RegenerateGlyphs( );
}

void FontWidget::CreateGlyphs( const QString &p_Characters )
{
	m_Characters = p_Characters;

	this->RegenerateGlyphs( );
}

void FontWidget::RegenerateGlyphs( )
{
	printf( "Regen\n" );
	printf( "Lib: %p %d\n", m_FTLibrary, m_FTLibrary );
	if( m_FTLibrary )
	{

		printf( "Creating\n" );
		FontArray::const_iterator FaceItr = m_Faces.begin( );
		while( FaceItr != m_Faces.end( ) )
		{
			printf( "deleting\n" );
			FT_Done_Face( ( *FaceItr ).Face );
			++FaceItr;
		}

		QChar *Character = m_Characters.data( );

		while( !Character->isNull( ) )
		{
			printf( "Not null: %c\n", Character->toLatin1( ) );
			FONT_RENDER FontRender;
			FT_Error Error = FT_New_Face( m_FTLibrary,
				m_FontFileName.toUtf8( ).constData( ), 0, &FontRender.Face );

			if( !Error )
			{
				printf( "OK\n" );
				Error = FT_Set_Char_Size( FontRender.Face, 0, m_PointSize * 64,
					physicalDpiX( ), physicalDpiY( ) );

				if( !Error )
				{
					printf( "Rendering glyph\n" );
					FT_UInt16 GlyphIndex = 0;
					GlyphIndex = FT_Get_Char_Index( FontRender.Face,
						Character->toLatin1( ) );

					Error = FT_Load_Glyph( FontRender.Face, GlyphIndex,
						FT_LOAD_DEFAULT );

					if( !Error )
					{
						printf( "Adding to array\n" );
						FT_Pos Left =
							FontRender.Face->glyph->metrics.horiBearingX;
						FT_Pos Right =
							FontRender.Face->glyph->metrics.width;
						FT_Pos Top =
							FontRender.Face->glyph->metrics.horiBearingY;
						FT_Pos Bottom =
							FontRender.Face->glyph->metrics.height;

						FontRender.Rect = QRect(
							QPoint( TRUNC( Left ), -TRUNC( Top ) + 1 ),
							QSize( TRUNC( Right - Left ) + 1,
								TRUNC( Top - Bottom ) + 1 ) );

						m_Faces.push_back( FontRender );

						printf( "Rendered: %c\n", Character->toLatin1( ) );
					}
				}
			}

			++Character;
		}
	}

	this->update( );
}

FontFile FontWidget::GetFontFile( ) const
{
	return m_FontFile;
}

void FontWidget::paintEvent( QPaintEvent *p_pPaintEvent )
{
	QWidget::paintEvent( p_pPaintEvent );

	if( m_FTLibrary )
	{
		FT_Error Error = FT_Err_Ok;

		QImage	m_SpriteFont;

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

		char GlyphChar = ' ';

		FontArray::iterator BeginningItr = FaceItr;
		std::vector< int > LineSpacing;
		LineSpacing.push_back( 0 );
		int MaxTop = 0;
		int MaxBottom = 0;


		for( ; FaceItr != m_Faces.end( ); ++FaceItr )
		{
			m_FontFile.ClearGlyphs( );
			m_FontFile.ClearTarga( );
			Error = FT_Render_Glyph( ( *FaceItr ).Face->glyph,
				FT_RENDER_MODE_NORMAL );

			CurrentXPosition += ( ( *FaceItr ).Face->glyph->bitmap.width ) +
				m_Padding;

			if( CurrentXPosition > m_Width )
			{
				FontArray::iterator EndItr = FaceItr;
				CurrentXPosition = m_Padding +
					( *FaceItr ).Face->glyph->bitmap.width;

				// A problem still exists when rendering characters that only
				// go below the baseline, the Y offset ignores it when
				// rendering
				CurrentMaxHeight = MaxTop + MaxBottom;
				printf( "Max Height: %d\n", CurrentMaxHeight );
				for( ; BeginningItr != EndItr; ++BeginningItr )
				{
					( *BeginningItr ).YOffset = CurrentMaxHeight -
						( ( *BeginningItr ).Face->glyph->metrics.horiBearingY /
							64 );
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

			( *FaceItr ).Overhang = Bottom;
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

		std::vector< int >::const_iterator LineSpace = LineSpacing.begin( );
		CurrentXPosition = m_Padding;
		CurrentYPosition = m_Padding + ( *LineSpace );

		FaceItr = m_Faces.begin( );

		int GlyphCounter = 0;

		printf( "Line spacing count: %d\n", LineSpacing.size( ) );
		printf( "Size: %d\n", m_Faces.size( ) );

		for( ; FaceItr != m_Faces.end( ); ++FaceItr )
		{
			Error = FT_Render_Glyph( ( *FaceItr ).Face->glyph,
				FT_RENDER_MODE_NORMAL );

			FONTFILE_GLYPH FileGlyph;
			FileGlyph.Character = ' ' + GlyphCounter;
			FileGlyph.X = CurrentXPosition;
			FileGlyph.Y = CurrentYPosition + ( *LineSpace ) +
				( *FaceItr ).YOffset;
			FileGlyph.Width = ( *FaceItr ).Face->glyph->bitmap.width;
			FileGlyph.Height = ( *FaceItr ).Face->glyph->bitmap.rows;
			FileGlyph.BearingY = ( *FaceItr ).Overhang;
			if( CurrentYPosition != ( m_Padding + ( *LineSpace ) ) )
			{
				FileGlyph.X += m_Padding;
				FileGlyph.Y += m_Padding;
			}

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

				FileGlyph.X = m_Padding;
				FileGlyph.Y = CurrentYPosition + m_Padding + ( *LineSpace ) +
					( *FaceItr ).YOffset;

				printf( "FileGlyph.Y: %d\n", FileGlyph.Y );
			}


			m_GlyphArray.push_back( FileGlyph );

			m_FontFile.AddGlyph( FileGlyph );

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
			++GlyphCounter;
		}

		Painter.end( );
		OverlayPainter.end( );

		QPainter WidgetPainter( this );

		QRect Source( 0.0f, 0.0f, this->width( ), this->height( ) );

		WidgetPainter.fillRect( WidgetPainter.window( ),
			QBrush( QColor( 0, 0, 0, 255 ) ) );

		WidgetPainter.drawImage( Source, m_SpriteFont, Source );
		WidgetPainter.drawImage( Source, OverlayImage, Source );
		
		m_FontFile.SetQImage( m_SpriteFont );

		WriteQImageToTarga( m_SpriteFont, "Sprite.tga" );
		this->WriteGlyphArray( );

		printf( "Redraw\n" );
	}
}


void FontWidget::WriteGlyphArray( )
{
	GlyphArray::const_iterator GlyphItr = m_GlyphArray.begin( );

	FILE *pGlyphFile = fopen( "Glyphs.glyph", "w" );

	for( ; GlyphItr != m_GlyphArray.end( ); ++GlyphItr )
	{
		FONTFILE_GLYPH Glyph = ( *GlyphItr );
		fprintf( pGlyphFile, "%c %d %d %d %d %d\n", Glyph.Character,
			Glyph.X, Glyph.Y, Glyph.Width, Glyph.Height, Glyph.BearingY );
	}

	fclose( pGlyphFile );
}

