#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

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

int draw_tiles(rpgTile *tilearr, displayData *display, int pos_x, int pos_y)
{
	int scr_w = display->res_x;
	int scr_h = display->res_y;
	int base_size = display->base_size;
	
	int cur_x = pos_x, cur_y = pos_y; /* Draw 'cursor' */
	
	if( scr_h % base_size != 0 || scr_w % base_size != 0) /* Check to make sure the tiles will fit cleanly on the screen */
	{
		printf("Error: screen dimensions don't divide by base size without remainder, aborting...\n");
		return 0;
	}
	
	int max_y_iter = scr_h / base_size;
	int max_x_iter = scr_w / base_size;
	//printf("Max x:%d, Max y:%d\n", max_x_iter, max_y_iter);
	int i, j;
	for(i=0; i<max_y_iter; i++) {
		for(j=0; j<max_x_iter; j++) {
			#if DEBUG >= 1
			//printf("Drawing at: (%d, %d)\n", cur_x, cur_y);
			#endif
			//if( (base_size * j) + i > 300 ) return 0;
			DRAW_SPRITE(tilearr[(max_x_iter * i) + j].sprite, cur_x, cur_y);
			#if DEBUG >= 2
			//printf("(%d)\n", (max_x_iter * i) + j );
			#endif
			//printf("i:%d j:%d\n", i, j);
			cur_x += base_size;
			//printf("x: %d y: %d\n", cur_x, cur_y);
		}
		cur_x = pos_x;
		cur_y += base_size;
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
