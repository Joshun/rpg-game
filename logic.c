#include "rpg.h"

#include <stdio.h>

static int check_intersect(rpgTile *tile, gameData *data, playerData *player)
{
	int player_x1 = player->x1;
	int player_x4 = player->x4;
	int player_y1 = player->y1;
	int player_y4 = player->y4;
	
	player_x1 += player->tolerance;
	player_x4 -= player->tolerance;
	player_y1 += player->tolerance;
	player_y4 -= player->tolerance;
	
	//printf("Player: (%d,%d):(%d,%d)\n", player_x1, player_y1, player_x4, player_y4);
	//printf("Tile: (%d,%d):(%d,%d)\n", tile_x1, tile_y1, tile_x4, tile_y4);
	//if( ((player_x1 < tile_x4) && (player_x1 > tile_x1) && (player_y1 > tile_y1) && (player_y4 < tile_y4)) || ((player_x4 < tile_x4) && (player_x4 > tile_x1) && (player_y1 > tile_y1) && (player_y4 < tile_y4)) )
	if( (player_x1 < tile->x4 ) && (player_x1 > tile->x1) && (player_x4 < tile->x4) && (player_x4 > tile->x1) && (player_y1 > tile->y1) && (player_y1 < tile->y4) && (player_y4 > tile->y1) && (player_y4 < tile->y4) )
	{
		tile->sprite = data->sprites[1];
		//printf("Intersect\n");
		//exit(1);
		if(	tile->action != NULL )
			tile->action(data, tile->id);
		return 1;
	}
	else
		return 0;
}

void poll_intersect(rpgTile *tilearr, gameData *data, int num)
{	
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
	
	int cur_x = 0, cur_y = 0;
	
	int i, j;
	for(i=0; i<num; i++) {
		check_intersect(&(tilearr[i]), data, &(data->player));
	}
		
}

void process_keyboard(ALLEGRO_EVENT *events, int keys[NUM_KEYS], int mode)
{
	
	switch( events->keyboard.keycode ) {
		case ALLEGRO_KEY_UP:
			keys[KEY_UP] = mode;
			break;
		case ALLEGRO_KEY_DOWN:
			keys[KEY_DOWN] = mode;
			break;
		case ALLEGRO_KEY_LEFT:
			keys[KEY_LEFT] = mode;
			break;
		case ALLEGRO_KEY_RIGHT:
			keys[KEY_RIGHT] = mode;
			break;
		case ALLEGRO_KEY_SPACE:
			keys[KEY_ACTION] = mode;
			break;
		case ALLEGRO_KEY_I:
			keys[KEY_INV] = mode;
			break;
	}
}

void process_player(playerData *player, int keys[NUM_KEYS])
{
	//printf("%d\n", player->y1); exit(1);
	if( keys[KEY_UP] )
	{
		player->y1-=PLAYER_SPEED;
		player->y4-=PLAYER_SPEED;
	}
	if( keys[KEY_DOWN] )
	{
		player->y1+=PLAYER_SPEED;
		player->y4+=PLAYER_SPEED;
	}
	if( keys[KEY_LEFT] )
	{
		player->x1-=PLAYER_SPEED;
		player->x4-=PLAYER_SPEED;
	}
	if( keys[KEY_RIGHT] )
	{
		player->x1+=PLAYER_SPEED;
		player->x4+=PLAYER_SPEED;
	}
}
	
int update_sprite(int *current, int max)
{
	if( *current >= max )
	{
		*current = 0;
		//printf("Sprite is now %d\n", *current);
		return 1;
	}
	else
	{
		*current = *current + 1;
		//printf("Sprite is now %d\n", *current);
		return 0;
	}
}

void toggle_inventory(playerData *player, int keys[NUM_KEYS])
{
	if( keys[KEY_INV] )
	{
		if(player->inventory_open == 0)
			player->inventory_open = 1;
		else
			player->inventory_open = 0;
	}
}
