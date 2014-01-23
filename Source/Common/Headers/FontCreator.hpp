#ifndef __ZEDTOOL_FONTCREATOR_HPP__
#define __ZEDTOOL_FONTCREATOR_HPP__

#include <SDL2/SDL.h>

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
	};
}

#endif // __ZEDTOOL_FONTCREATOR_HPP__

