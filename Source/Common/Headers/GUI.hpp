#ifndef __ZEDTOOL_GUI_HPP__
#define __ZEDTOOL_GUI_HPP__

#include <GUIElement.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <list>

namespace ZEDTool
{
	class GUIManager
	{
	public:
		GUIManager( );
		~GUIManager( );

		int SetRenderer( SDL_Renderer * const &p_pRenderer );

		void Render( );

		void ToggleDebugRendering( );

		int AddGUIElement( GUIElement * const &p_pElement );

		int SetFont( const char *p_pFontFile, const int p_PointSize );

	private:
		void RenderDebugOverlay( ) const;

		typedef std::list< GUIElement * > GUIElementList;

		GUIElementList	m_GUIElements;
		SDL_Renderer	*m_pRenderer;
		TTF_Font		*m_pFont;
		bool			m_RenderDebugMode;
	};
}

#endif // __ZEDTOOL_GUI_HPP__

