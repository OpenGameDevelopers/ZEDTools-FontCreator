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

		void RenderDebugOverlay( ) const;

	private:
		std::list< GUIElement * > m_GUIElements;

		SDL_Renderer	*m_pRenderer;
	};
}

#endif // __ZEDTOOL_GUI_HPP__

