#include <MainWindow.h>
#include <GitVersion.h>
#include <cstring>
#include <QAction>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QFileDialog>
#include <FontWidget.h>
#include <FontFile.h>

MainWindow::MainWindow( ) :
	m_pFontWidget( nullptr )
{
}

MainWindow::~MainWindow( )
{
}

int MainWindow::Initialise( )
{
	this->CreateActions( );
	this->CreateMenus( );
	this->CreateWidgets( );

	char Title[ 1000 ];
	memset( Title, '\0', sizeof( Title ) );
	sprintf( Title, "ZED Font Creator [%s] %s //Build date: %s%s",
		GIT_BUILD_VERSION, GIT_COMMITHASH, GIT_COMMITTERDATE,
#if defined BUILD_DEBUG
		" [DEBUG]"
#else
		""
#endif
		);
	setWindowTitle( tr( Title ) );
	setMinimumSize( 640, 480 );

	if( m_pFontWidget->Initialise( ) == 0 )
	{
		m_pFontWidget->SetDimensions( 256, 256 );
		m_pFontWidget->SetPointSize( 16 );
		m_pFontWidget->SetPadding( 3 );

		QString Glyphs;

		char GlyphChar = ' ';

		while( GlyphChar != 0x7F )
		{
			Glyphs.append( GlyphChar );
			++GlyphChar;
		}

		m_pFontWidget->CreateGlyphs( Glyphs );
	}

	setCentralWidget( m_pFontWidget );

	return 0;
}

void MainWindow::CreateActions( )
{
	m_pOpenFontFile = new QAction( tr( "&Select Font" ), this );
	m_pWriteFont = new QAction( tr( "&Write Font" ), this );
	m_pQuitAction = new QAction( tr( "&Quit" ), this );
	m_pQuitAction->setShortcuts( QKeySequence::Quit );

	connect( m_pOpenFontFile, SIGNAL( triggered( ) ), this,
		SLOT( OpenFontFile( ) ) );
	connect( m_pWriteFont, SIGNAL( triggered( ) ), this,
		SLOT( WriteFontFile( ) ) );
	connect( m_pQuitAction, SIGNAL( triggered( ) ), this, SLOT( close( ) ) );
}

void MainWindow::CreateMenus( )
{
	m_pFileMenu = menuBar( )->addMenu( tr( "&File" ) );
	m_pFileMenu->addAction( m_pOpenFontFile );
	m_pFileMenu->addAction( m_pWriteFont );
	m_pFileMenu->addAction( m_pQuitAction );
}

void MainWindow::CreateWidgets( )
{
	m_pFontWidget = new FontWidget( );
}

void MainWindow::OpenFontFile( )
{
	m_FontFile = QFileDialog::getOpenFileName( this, tr( "Open Font File" ),
		tr( "" ), tr( "Font Files (*.ttf)" ) );
	
	m_pFontWidget->SetFont( m_FontFile );
	//m_pFontWidget = new FontWidget( m_FontFile, 256, 256 );
}

void MainWindow::WriteFontFile( )
{
    QString FileName = QFileDialog::getSaveFileName( this, tr( "Save font" ),
		tr( "" ), tr( "ZED Font File (*.zed)" ) );
	
	// Check if the .zed extension has been applied

	printf( "%s\n", FileName.toUtf8( ).constData( ) );

    m_pFontWidget->GetFontFile( ).Write( FileName.toUtf8( ).constData( ) );
}

