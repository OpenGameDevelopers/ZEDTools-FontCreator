#ifndef __ZEDTOOL_FONTCREATOR_MAINWINDOW_H__
#define __ZEDTOOL_FONTCREATOR_MAINWINDOW_H__

#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow( );
	~MainWindow( );

	int Initialise( );

private:
	void CreateActions( );
	void CreateMenus();

	QMenu	*m_pFileMenu;
	QAction	*m_pQuitAction;
	QAction	*m_pOpenFontFile;
	QString	m_FontFile;

public slots:
	void OpenFontFile( );
};

#endif // __ZEDTOOL_FONTCREATOR_MAINWINDOW_H__

