#ifndef __ZEDTOOL_GUIELEMENT_HPP__
#define __ZEDTOOL_GUIELEMENT_HPP__

#include <SDL2/SDL.h>

namespace ZEDTool
{
	typedef struct __Colour
	{
		Uint8	Red;
		Uint8	Green;
		Uint8	Blue;
		Uint8	Alpha;
	}Colour;

	class GUIElement
	{
	public:
		GUIElement( );
		virtual ~GUIElement( );

		void GetBoundingBox( SDL_Rect *p_pBoundingBox ) const;

		void SetPosition( const int p_X, const int p_Y );

		void SetDebugOutlineColour( const Uint8 p_Red, const Uint8 p_Green,
			const Uint8 p_Blue, const Uint8 p_Alpha );

		void GetDebugOutlineColour( Uint8 *p_pRed, Uint8 *p_pGreen,
			Uint8 *p_pBlue, Uint8 *p_pAlpha ) const;
		void GetDebugOutlineColour( Colour *p_pColour ) const;

	protected:
		SDL_Rect	m_BoundingBox;
		Colour		m_DebugColour;
	};
}

#endif // __ZEDTOOL_GUIELEMENT_HPP__

