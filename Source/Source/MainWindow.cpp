#include <MainWindow.h>
#include <GitVersion.h>
#include <cstring>

MainWindow::MainWindow( )
{
}

MainWindow::~MainWindow( )
{
}

int MainWindow::Initialise( )
{
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
