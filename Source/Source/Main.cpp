#include <MainWindow.h>
#include <QApplication>

int main( int p_Argc, char **p_ppArgv )
{
	QApplication Application( p_Argc, p_ppArgv );

	MainWindow QtWindow;

	if( QtWindow.Initialise( ) != 0 )
	{
		return 1;
	}

	QtWindow.show( );

	return Application.exec( );
}

