#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define EXIT_ON_FAILURE() ( exit(EXIT_FAILURE) )

void game_init()
{
	if( ! al_init() )
		EXIT_ON_FAILURE();
	if( ! al_install_keyboard() )
		EXIT_ON_FAILURE();
	if( ! al_init_image_addon() )
		EXIT_ON_FAILURE();
	if( ! al_init_primitives_addon() )
		EXIT_ON_FAILURE();
}

