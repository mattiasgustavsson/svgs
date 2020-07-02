

int app_proc( app_t* app, void* user_data ) {
	(void) user_data;

	app_title( app, "SuperVGS" );
	app_screenmode( app, APP_SCREENMODE_WINDOW );
	app_window_size( app, 768, 576 );
	app_interpolation( app, APP_INTERPOLATION_NONE );

	APP_U32 canvas[ 320 * 200 ]; 
	memset( canvas, 0xC0, sizeof( canvas ) ); 

	while( app_yield( app ) != APP_STATE_EXIT_REQUESTED ) {

		app_present( app, canvas, 320, 200, 0xffffff, 0x000000 );
	}

	return 0;
}


int svgs( void ) {
   return app_run( app_proc, NULL, NULL, NULL, NULL ); 
}