#ifndef __ZEDTOOL_GUI_HPP__
#define __ZEDTOOL_GUI_HPP__

#include <GUIElement.hpp>
#include <SDL2/SDL.h>
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

	private:
		void RenderDebugOverlay( ) const;

		typedef std::list< GUIElement * > GUIElementList;

		GUIElementList	m_GUIElements;
		SDL_Renderer	*m_pRenderer;

		bool m_RenderDebugMode;
	};
}

#endif // __ZEDTOOL_GUI_HPP__

