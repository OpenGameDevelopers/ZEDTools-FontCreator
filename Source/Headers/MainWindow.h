#ifndef __ZEDTOOL_FONTCREATOR_MAINWINDOW_H__
#define __ZEDTOOL_FONTCREATOR_MAINWINDOW_H__

#include <QMainWindow>

class FontWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow( );
	~MainWindow( );

	int Initialise( );

private:
	void CreateActions( );
	void CreateMenus( );
	void CreateWidgets( );

	QMenu		*m_pFileMenu;
	QAction		*m_pQuitAction;
	QAction		*m_pOpenFontFile;
	QAction		*m_pWriteFont;
	QString		m_FontFile;
	FontWidget	*m_pFontWidget;

public slots:
	void OpenFontFile( );
	void WriteFontFile( );
};

#endif // __ZEDTOOL_FONTCREATOR_MAINWINDOW_H__

