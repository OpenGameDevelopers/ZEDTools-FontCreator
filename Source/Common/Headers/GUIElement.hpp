#ifndef __ZEDTOOL_GUIELEMENT_HPP__
#define __ZEDTOOL_GUIELEMENT_HPP__

#include <SDL2/SDL.h>

namespace ZEDTool
{
	class GUIElement
	{
	public:
		GUIElement( );
		virtual ~GUIElement( );

		void GetBoundingBox( SDL_Rect *p_pBoundingBox );

	protected:
		SDL_Rect	m_BoundingBox;
	};
}

#endif // __ZEDTOOL_GUIELEMENT_HPP__

