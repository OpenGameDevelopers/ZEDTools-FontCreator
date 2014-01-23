#include <GUIElement.hpp>
#include <cstring>

namespace ZEDTool
{
	GUIElement::GUIElement( )
	{
		m_BoundingBox.w = 0;
		m_BoundingBox.h = 0;
		m_BoundingBox.x = 0;
		m_BoundingBox.y = 0;

		m_DebugColour.Red	= 255;
		m_DebugColour.Green	= 255;
		m_DebugColour.Blue	= 255;
		m_DebugColour.Alpha	= 255;
	}

	GUIElement::~GUIElement( )
	{
	}

	void GUIElement::GetBoundingBox( SDL_Rect *p_pBoundingBox ) const
	{
		memcpy( p_pBoundingBox, &m_BoundingBox, sizeof( m_BoundingBox ) );
	}

	void GUIElement::SetPosition( const int p_X, const int p_Y )
	{
		m_BoundingBox.x += p_X;
		m_BoundingBox.y += p_Y;

		m_BoundingBox.w += p_X;
		m_BoundingBox.h += p_Y;
	}

	void GUIElement::SetDebugOutlineColour( const Uint8 p_Red,
		const Uint8 p_Green, const Uint8 p_Blue, const Uint8 p_Alpha )
	{
		m_DebugColour.Red = p_Red;
		m_DebugColour.Green = p_Green;
		m_DebugColour.Blue = p_Blue;
		m_DebugColour.Alpha = p_Alpha;
	}

	void GUIElement::GetDebugOutlineColour( Uint8 *p_pRed, Uint8 *p_pBlue,
		Uint8 *p_pGreen, Uint8 *p_pAlpha ) const
	{
		( *p_pRed ) = m_DebugColour.Red;
		( *p_pGreen ) = m_DebugColour.Green;
		( *p_pBlue ) = m_DebugColour.Blue;
		( *p_pAlpha ) = m_DebugColour.Alpha;
	}
}

