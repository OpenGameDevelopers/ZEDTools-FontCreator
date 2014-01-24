#include <GUI.hpp>
#include <Helper.hpp>

namespace ZEDTool
{
	GUIManager::GUIManager( ) :
		m_pRenderer( NULL ),
		m_RenderDebugMode( false )
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

	void GUIManager::Render( )
	{
		Colour PreviousColour;
		SDL_GetRenderDrawColor( m_pRenderer, &PreviousColour.Red,
			&PreviousColour.Green, &PreviousColour.Blue,
			&PreviousColour.Alpha );

		GUIElementList::const_iterator ElementIterator =
			m_GUIElements.begin( );

		while( ElementIterator != m_GUIElements.end( ) )
		{
			( *ElementIterator )->Render( m_pRenderer );

			++ElementIterator;
		}

		if( m_RenderDebugMode )
		{
			this->RenderDebugOverlay( );
		}

		SDL_SetRenderDrawColor( m_pRenderer, PreviousColour.Red,
			PreviousColour.Green, PreviousColour.Blue, PreviousColour.Alpha );
	}

	void GUIManager::ToggleDebugRendering( )
	{
		m_RenderDebugMode = !m_RenderDebugMode;
	}

	int GUIManager::AddGUIElement( GUIElement * const &p_pElement )
	{
		m_GUIElements.push_back( p_pElement );

		return 0;
	}

	void GUIManager::RenderDebugOverlay( ) const
	{
		GUIElementList::const_iterator ElementIterator =
			m_GUIElements.begin( );

		for( size_t i = 0; i < m_GUIElements.size( ); ++i )
		{
			SDL_Rect OutlineBorder;
			( *ElementIterator )->GetBoundingBox( &OutlineBorder );
			Colour OutlineColour;
			( *ElementIterator )->GetDebugOutlineColour( &OutlineColour );

			SDL_SetRenderDrawColor( m_pRenderer, OutlineColour.Red,
				OutlineColour.Green, OutlineColour.Blue, OutlineColour.Alpha );
			SDL_RenderDrawRect( m_pRenderer, &OutlineBorder );

			++ElementIterator;
		}
	}
}

