#include <FontFile.h>
#include <Targa.h>
#include <cstring>

FontFile::FontFile( ) :
    m_pTarga( nullptr ),
    m_TargaSize( 0 )
{
}

FontFile::~FontFile( )
{
}

int FontFile::Write( const char *p_pFilePath )
{
    // Check glyph set
    // Check for Targs
    if( m_GlyphArray.size( ) <= 0 )
    {
	return 1;
    }

    if( m_TargaSize <= 0 )
    {
	return 1;
    }

    return 0;
}

void FontFile::ClearGlyphs( )
{
    while( !m_GlyphArray.empty( ) )
    {
	m_GlyphArray.pop_back( );
    }
}

void FontFile::ClearTarga( )
{
    if( m_pTarga )
    {
	delete [ ] m_pTarga;
	m_pTarga = nullptr;
    }

    m_TargaSize = 0;
}

void FontFile::AddGlyph( const FONTFILE_GLYPH &p_Glyph )
{
    m_GlyphArray.push_back( p_Glyph );
}

void FontFile::SetQImage( const QImage &p_Image )
{
    if( m_pTarga )
    {
	this->ClearTarga( );
    }

    m_TargaSize = ( p_Image.width( ) * p_Image.height( ) * 4 ) +
	sizeof( TARGA_HEADER );
    m_pTarga = new unsigned char[ m_TargaSize ];

    TARGA_HEADER TargaHeader;

    QRgb *pImageData = new QRgb[ p_Image.width( ) * p_Image.height( ) ];

    memset( &TargaHeader, 0, sizeof( TargaHeader ) );

    TargaHeader.Width = p_Image.width( );
    TargaHeader.Height = p_Image.height( );
    TargaHeader.ImageType = 0x02;
    TargaHeader.BitsPerPixel = 32;

    size_t DataOffset = 0;

    for( int i = p_Image.height( ); i > 0; --i )
    {
	    QRgb *Line = ( QRgb* )( p_Image.scanLine( i ) );
	    memcpy( &pImageData[ DataOffset ], Line, p_Image.bytesPerLine( ) );
	    DataOffset += p_Image.width( );
    }

    memcpy( m_pTarga, &TargaHeader, sizeof( TARGA_HEADER ) );
    memcpy( &( m_pTarga[ sizeof( TARGA_HEADER ) ] ), pImageData,
	m_TargaSize - sizeof( TARGA_HEADER ) );

    if( pImageData )
    {
	delete [ ] pImageData;
	pImageData = nullptr;
    }
}

