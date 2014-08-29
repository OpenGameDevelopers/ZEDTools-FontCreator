#include <MainWindow.h>
#include <GitVersion.h>
#include <cstring>
#include <QAction>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QFileDialog>
#include <FontWidget.h>

MainWindow::MainWindow( )
{
}

MainWindow::~MainWindow( )
{
}

int MainWindow::Initialise( )
{
	this->CreateActions( );
	this->CreateMenus( );

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

	return 0;
}

void MainWindow::CreateActions( )
{
	m_pOpenFontFile = new QAction( tr( "&Select Font" ), this );
	m_pQuitAction = new QAction( tr( "&Quit" ), this );
	m_pQuitAction->setShortcuts( QKeySequence::Quit );

	connect( m_pOpenFontFile, SIGNAL( triggered( ) ), this, SLOT( OpenFontFile( ) ) );
	connect( m_pQuitAction, SIGNAL( triggered( ) ), this, SLOT( close( ) ) );
}

void MainWindow::CreateMenus( )
{
	m_pFileMenu = menuBar( )->addMenu( tr( "&File" ) );
	m_pFileMenu->addAction( m_pOpenFontFile );
	m_pFileMenu->addAction( m_pQuitAction );
}

void MainWindow::OpenFontFile( )
{
	m_FontFile = QFileDialog::getOpenFileName( this, tr( "Open Font File" ),
		tr( "" ), tr( "Font Files (*.ttf)" ) );
	printf( "%s\n", m_FontFile.toUtf8( ).constData( ) );
	fflush( stdout );
	m_pFontWidget = new FontWidget( m_FontFile );
	setCentralWidget( m_pFontWidget );
}

