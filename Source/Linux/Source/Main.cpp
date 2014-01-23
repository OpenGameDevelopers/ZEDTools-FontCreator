#include <iostream>
#include <FontCreator.hpp>

int main( int p_Argc, const char **p_ppArgv )
{
	ZEDTool::FontCreator Program;

	if( Program.Initialise( ) != 0 )
	{
		std::cout << "" << std::endl;

		return 1;		
	}

	return Program.Execute( );
}

