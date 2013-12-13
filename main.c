#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "rpg.h"

int main(int argc, char **argv)
{
	ALLEGRO_DISPLAY *display_ptr;
	gameData config;
	config.quit = 0;
	config.keys[0] = 0; config.keys[1] = 0; config.keys[2] = 0; config.keys[3] = 0; config.keys[4] = 0; config.keys[5] = 0; config.keys[6] = 0;
	
	int offset_x = 0, offset_y = 0;

	rpgTile tileset[NUM_TILES];
	rpgSprite *spriteset;
	printf("%d tiles\n", NUM_TILES);
	
	int player_x1 = 1, player_y1 = 70;
	int player_size = 20;
	config.player.x1 = 1, config.player.y1 = 70;
	config.player.x4 = config.player.x1 + player_size;
	config.player.y4 = config.player.y1 + player_size;
	config.player.tolerance = 10;
	config.player.current_sprite = 0;
	config.player.health = 10.0;

	game_init();

	create_event_data(&config);
	
	config.timer_set.frame_timer = al_create_timer(1.0 / FPS);
	config.timer_set.sprite_timer = al_create_timer(SPRITE_UPDATE_T);

	init_tileset(tileset, &config, NUM_TILES, offset_x, offset_y);

	if( ! display_init(&(config.display), SCREEN_RES_X, SCREEN_RES_Y, BASE_SIZE) )
		exit(EXIT_FAILURE);

	al_register_event_source(RPG_EVENT_QUEUE(config), al_get_display_event_source(RPG_DISPLAY(config)));
	al_register_event_source(RPG_EVENT_QUEUE(config), al_get_keyboard_event_source() );
	al_register_event_source(RPG_EVENT_QUEUE(config), al_get_timer_event_source(RPG_TIMER_SET(config).frame_timer));
	al_register_event_source(RPG_EVENT_QUEUE(config), al_get_timer_event_source(RPG_TIMER_SET(config).sprite_timer));
	
	config.sprites[0] = al_load_bitmap("sprites/image1.png");
	config.sprites[1] = al_load_bitmap("sprites/image2.png");

	int num_sprites = make_sprite_catalog("sprites/index.dat", &spriteset);

	config.player.inventory_open = 0;
	config.player.pcolour = al_map_rgb(255, 0, 255);
	
	config.player.sprites[0] = get_sprite_from_id(spriteset, num_sprites, "player1");
	//config.player.sprites[0] = al_load_bitmap("sprites/character/char_frame_1.png");
	config.player.sprites[1] = get_sprite_from_id(spriteset, num_sprites, "player2");
	config.player.sprites[2] = get_sprite_from_id(spriteset, num_sprites, "player3");
	config.player.sprites[3] = get_sprite_from_id(spriteset, num_sprites, "player4");
	
	ALLEGRO_BITMAP *grass1 = get_sprite_from_id(spriteset, num_sprites, "grass1");
	ALLEGRO_BITMAP *grass2 = get_sprite_from_id(spriteset, num_sprites, "grass2");

	int i;
	for(i=0; i<NUM_TILES; i++) {
		tileset[i].sprite = grass1;
		tileset[i].action = NULL;
	}
	
	//poll_intersect(tileset, &config, num_tiles);
	//draw_tiles(tileset, &(config.display), num_tiles);
	//al_draw_filled_rectangle(player.x1, player.y1, player.x4, player.y4, player.pcolour);
	draw_player(&(config.player));
	al_flip_display();
	//sleep(2);
	
	al_start_timer(RPG_TIMER_SET(config).frame_timer);
	while( ! config.quit ) {
		wait_for_event(&config);
		switch( RPG_EVENT(config).type )
		{
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				config.quit = 1;
				break;
			case ALLEGRO_EVENT_TIMER:
				if( RPG_EVENT(config).timer.source == config.timer_set.frame_timer )
				{
					//printf("Keys: [%d][%d][%d][%d][%d]\n", config.keys[0], config.keys[1], config.keys[2], config.keys[3], config.keys[4], config.keys[5]);
					/* Frame timer code */
					process_player(&(config.player), config.keys);
					al_clear_to_color(al_map_rgb(0,0,0));
					poll_intersect(tileset, &config, NUM_TILES);
					draw_tiles(tileset, &(config.display), NUM_TILES);
					draw_player(&(config.player));
					draw_health(&(config.player));
					draw_inventory(&(config.player));
					al_flip_display();
				}
				else if (RPG_EVENT(config).timer.source == config.timer_set.sprite_timer )
				{
					/* Sprite timer swapping code */
					update_sprite(&(config.player.current_sprite), (NUM_PLAYER_SPRITES - 1));
					printf("Sprite set to [%d]\n", config.player.current_sprite);
				}
				break;
			case ALLEGRO_EVENT_KEY_UP:
				process_keyboard(&(RPG_EVENT(config)), config.keys, 0);
				if( ! config.keys[0] && ! config.keys[1] && ! config.keys[2] && ! config.keys[3] )
					al_stop_timer(RPG_TIMER_SET(config).sprite_timer);
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				process_keyboard(&(RPG_EVENT(config)), config.keys, 1);
				toggle_inventory(&(config.player), config.keys);
				if( config.keys[0] || config.keys[1] || config.keys[2] || config.keys[3] )
					al_start_timer(RPG_TIMER_SET(config).sprite_timer);
				break;
		}
	}
	
	//al_destroy_bitmap(config.sprites[0]);
	//al_destroy_bitmap(config.sprites[1]);
	//al_destroy_bitmap(config.player.sprites[0]);
	//al_destroy_bitmap(config.player.sprites[1]);
	//al_destroy_bitmap(config.player.sprites[2]);
	//al_destroy_bitmap(config.player.sprites[3]);
	//al_destroy_bitmap(spriteset[0].sprite);
	//al_destroy_bitmap(spriteset[1].sprite);
	//al_destroy_bitmap(spriteset[2].sprite);
	//al_destroy_bitmap(spriteset[3].sprite);
	destroy_sprite_catalogue(spriteset, num_sprites);
	al_destroy_display(config.display.disp_ptr);
	al_destroy_timer(config.timer_set.frame_timer);
	al_destroy_timer(config.timer_set.sprite_timer);

	return 0;
}
