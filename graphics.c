#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <string.h>

#include "rpg.h"

#define DRAW_SPRITE(sprite, spr_x, spr_y) ( al_draw_bitmap(sprite, spr_x, spr_y, 0) )

/* Graphics routines inc. basic tiling engine *
 * Joshun 2013
 */

int display_init(displayData *display, int res_x, int res_y, int bsize)
{
	ALLEGRO_DISPLAY *check_ptr = NULL;
	display->res_x = res_x;
	display->res_y = res_y;
	display->base_size = bsize;
	if( (check_ptr = al_create_display(res_x, res_y)) != NULL )
	{
		display->disp_ptr = check_ptr;
		return 1;
	}
	else
		return 0;
	}	

void init_tileset(rpgTile *tilearr, gameData *data, int num, int pos_x, int pos_y)
{	
	/* Generates and sets values for x1, x4, y1, y4 for a tileset */
	int scr_w = data->display.res_x;
	int scr_h = data->display.res_y;
	int base_size = data->display.base_size;
	int tolerance = data->player.tolerance;
	
	int max_y_iter = scr_h / base_size;
	int max_x_iter = scr_w / base_size;
	
	int player_x1 = data->player.x1;
	int player_y1 = data->player.y1;
	int player_x4 = data->player.x4;
	int player_y4 = data->player.y4;
	
	int cur_x = pos_x, cur_y = pos_y;
	int index = 0;
	
	int i, j;
	for(i=0; i<max_y_iter; i++) {
		for(j=0; j<max_x_iter; j++) {
			#if DEBUG >= 1
			//printf("Drawing at: (%d, %d)\n", cur_x, cur_y);
			#endif
			index = (max_x_iter * i) + j;
			tilearr[index].x1 = cur_x;
			tilearr[index].x4 = cur_x + base_size;
			tilearr[index].y1 = cur_y;
			tilearr[index].y4 = cur_y + base_size;
			#if DEBUG >= 2
			//printf("Array index: (%d * %d) + %d\n", max_y_iter, i, j);
			#endif
			cur_x += base_size;
		}
		cur_x = pos_x;
		cur_y += base_size;
	}
}

int draw_tiles(rpgTile *tilearr, displayData *display, int num)
{
	int scr_w = display->res_x;
	int scr_h = display->res_y;
	int base_size = display->base_size;
	
	//int cur_x = pos_x, cur_y = pos_y; /* Draw 'cursor' */
	
	if( scr_h % base_size != 0 || scr_w % base_size != 0) /* Check to make sure the tiles will fit cleanly on the screen */
	{
		printf("Error: screen dimensions don't divide by base size without remainder, aborting...\n");
		return 0;
	}
	
	//int max_y_iter = scr_h / base_size;
	//int max_x_iter = scr_w / base_size;
	//printf("Max x:%d, Max y:%d\n", max_x_iter, max_y_iter);
	int i, j;
	for(i=0; i<num; i++) {
		DRAW_SPRITE(tilearr[i].sprite, tilearr[i].x1, tilearr[i].y1);
	}
	//al_flip_display();
	return 1;
}

void draw_player(playerData *player)
{
	//al_draw_filled_rectangle(player->x1, player->y1, player->x4, player->y4, player->pcolour);
	DRAW_SPRITE(player->sprites[player->current_sprite], player->x1, player->y1);
}

void draw_health(playerData *player)
{
	int health_factor = (player->health) * 10;
	int hbar_width = health_factor * HBAR_SIZE;
	int x4 = HBAR_X_OFFSET + hbar_width;
	int y4 = HBAR_Y_OFFSET + HBAR_HEIGHT;
	al_draw_filled_rectangle(HBAR_X_OFFSET, HBAR_Y_OFFSET, x4, y4, al_map_rgb(255, 0, 0));
}

ALLEGRO_BITMAP *get_sprite_from_id(rpgSprite *spriteset, int num, char *sprite_id)
{
	int i;
	for(i=0; i<num; i++)
	{
		if( strcmp(sprite_id, spriteset[i].id) == 0 )
		{
			printf("Found sprite [id: %s] in catalog.\n", spriteset[i].id);
			return spriteset[i].sprite;
		}
	}
	
	return NULL;
}

int get_sprite_index(rpgSprite *spriteset, int num, char *sprite_id)
{
	int i;
	for(i=0; i<num; i++)
	{
		if( strcmp(sprite_id, spriteset[i].id) == 0 )
		{
			printf("Found sprite [id: %s] in catalog.\n", spriteset[i].id);
			return i;
		}
	}
	
	return -1;
}

int make_tile_set(rpgTile *tileset, int num_tiles, rpgSprite *spriteset, int num_sprites, char sprite_ids[NUM_TILES][BUFFER_SUB_SIZE])
{
	ALLEGRO_BITMAP *test_p = NULL;
	int i;
	for(i=0; i<num_tiles; i++)
	{
		test_p = get_sprite_from_id(spriteset, num_sprites, sprite_ids[i]);
		if( test_p )
		{
			tileset[i].sprite = test_p;
		}
		else
			return 0;
	}
	return 1;
}
