#ifndef __ZEDTOOL_HELPER_HPP__
#define __ZEDTOOL_HELPER_HPP__

#include <sys/types.h>

#define SAFE_DELETE_ARRAY( Pointer ) \
	{ if( Pointer ){ delete [ ] Pointer; Pointer = NULL; } }

namespace ZEDTool
{
	int GetExecutableDirectory( char **p_ppBuffer, ssize_t p_BufferSize );
	int FileExists( const char *p_pFilePath, const bool p_IncludeSymLinks );
}

#endif // __ZEDTOOL_HELPER_HPP__

