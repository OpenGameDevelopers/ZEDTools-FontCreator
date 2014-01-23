#include <GUI.hpp>

namespace ZEDTool
{
	GUIManager::GUIManager( ) :
		m_pRenderer( NULL )
	{
	}

	GUIManager::~GUIManager( )
	{
	}

	int GUIManager::SetRenderer( SDL_Renderer * const &p_pRenderer )
	{
		if( p_pRenderer )
		{
			m_pRenderer = p_pRenderer;
		}
		else
		{
			return 1;
		}

		return 0;
	}

	void GUIManager::RenderDebugOverlay( ) const
	{
	}
}

