#ifndef __ZEDTOOL_FONTCREATOR_HPP__
#define __ZEDTOOL_FONTCREATOR_HPP__

#include <SDL2/SDL.h>
#include <GUI.hpp>

namespace ZEDTool
{
	class FontCreator
	{
	public:
		FontCreator( );
		~FontCreator( );
		
		int Initialise( );

		int Execute( );

	private:
		void CleanUp( );

		SDL_Window		*m_pWindow;
		SDL_Renderer	*m_pRenderer;
		GUIManager		m_GUI;
		char			*m_pExecutableDirectory;
	};
}

#endif // __ZEDTOOL_FONTCREATOR_HPP__

