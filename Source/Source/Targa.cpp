#include <Targa.h>

bool WriteQImageToTarga( const QImage &p_Image, const QString p_Path )
{
	TARGA_HEADER TargaHeader;
	FILE *pFile = fopen( p_Path.toUtf8( ).constData( ), "wb" );

	QRgb *pImageData = new QRgb[ p_Image.width( ) * p_Image.height( ) ];

	if( !pFile )
	{
		printf( "Failed to open file: %s\n", p_Path.toUtf8( ).constData( ) );
		return false;
	}

	memset( &TargaHeader, 0, sizeof( TargaHeader ) );

	TargaHeader.Width = p_Image.width( );
	TargaHeader.Height = p_Image.height( );
	TargaHeader.ImageType = 0x02;
	TargaHeader.BitsPerPixel = 32;

	fwrite( &TargaHeader, sizeof( TargaHeader ), 1, pFile );

	size_t DataOffset = 0;

	for( int i = 0; i < p_Image.height( ); ++i )
	{
		QRgb *Line = ( QRgb* )( p_Image.scanLine( i ) );
		memcpy( &pImageData[ DataOffset ], Line, p_Image.bytesPerLine( ) );
		DataOffset += p_Image.width( );
	}

	fwrite( pImageData, sizeof( QRgb ), p_Image.width( ) * p_Image.height( ),
		pFile );

	if( pFile )
	{
		fclose( pFile );
		pFile = nullptr;
	}

	if( pImageData )
	{
		delete [ ] pImageData;
		pImageData = nullptr;
	}

	return true;
}

