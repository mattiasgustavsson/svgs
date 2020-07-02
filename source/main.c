#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "libs/app.h"
#include "opset.h"
#include "svgs.h"

/*
-------------------------
    ENTRY POINT (MAIN)
-------------------------
*/     

#if defined( _WIN32 ) && !defined( __TINYC__ )
	#ifndef NDEBUG
		#pragma warning( push ) 
		#pragma warning( disable: 4619 ) // pragma warning : there is no warning number 'number'
		#pragma warning( disable: 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
		#include <crtdbg.h>
		#pragma warning( pop ) 
	#endif
#endif /* _WIN32 && !__TINYC__ */

int main( int argc, char** argv ) {
	(void) argc, (void) argv;
	#if defined( _WIN32 ) && !defined( __TINYC__ )
		#ifndef NDEBUG
			int flag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ); // Get current flag
			flag ^= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
			_CrtSetDbgFlag( flag ); // Set flag to the new value
			_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG );
			_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
			//_CrtSetBreakAlloc( 0 );
		#endif
	#endif

    cpu_gen_docs( "svgs_cpu.md" );
	return svgs();
} 

#ifdef _WIN32
	#include <stdlib.h>
	// pass-through so the program will build with either /SUBSYSTEM:WINDOWS or /SUBSYSTEM:CONSOLE
	struct HINSTANCE__;
	int __stdcall WinMain( struct HINSTANCE__* a, struct HINSTANCE__* b, char* c, int d ) { 
		(void) a, (void) b, (void) c, (void) d; 
		return main( __argc, __argv ); 
	}
#endif /* _WIN32 */


/*
---------------------------------
    LIBRARIES IMPLEMENTATIONS
---------------------------------
*/

    
#define SVGS_IMPLEMENTATION
#include "opset.h"

#define APP_IMPLEMENTATION
#ifdef _WIN32
	#define APP_WINDOWS
#else
	#define APP_SDL    
#endif
#define APP_LOG( ctx, level, message )
#include "libs/app.h"
