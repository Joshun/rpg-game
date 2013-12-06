#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "rpg.h"

#define TAB_CHAR 0x9

static int prepare_sprite_catalog(rpgSprite *spriteset, int num, FILE *spritecfg)
{
	ALLEGRO_BITMAP *test_p = NULL;
	int i, index = 0;
	char line_buffer[BUFFER_SIZE] = { 0 };
	char fname_buffer[BUFFER_SUB_SIZE] = { 0 };
	char sprname_buffer[BUFFER_SUB_SIZE] = { 0 };
	
	int spr_index;
	
	for(spr_index = 0; spr_index < num; spr_index++)
	{
		if( ! fgets(line_buffer, (BUFFER_SIZE - 1), spritecfg))
			break;
		
		for(i=0; ( line_buffer[index] != '\0' && line_buffer[index] != TAB_CHAR); i++)
		{
			sprname_buffer[i] = line_buffer[index];
			index++;
		}
		sprname_buffer[i] = '\0';
				
		index++; /* Skip space character */
		
		for(i=0; (line_buffer[index] != '\0' && line_buffer[index] != '\n'); i++)
		{
			fname_buffer[i] = line_buffer[index];
			index++;
		}
		//fname_buffer[i - 1] = '\0';
		fname_buffer[i] = '\0';

		printf("Sprite name read: %s\n", sprname_buffer);
		printf("File name read: %s\n", fname_buffer);
		
		strncpy(spriteset[spr_index].id, sprname_buffer, (200 - 1));
		test_p = al_load_bitmap(fname_buffer);
		
		if( test_p )
		{
			spriteset[spr_index].sprite = test_p;
			printf("Loaded\n");
		}
		else
			return 0;
		index = 0;
		
		memset(sprname_buffer, '\0', strlen(sprname_buffer));
		memset(fname_buffer, '\0', strlen(fname_buffer));

	}
}

static int get_num_sprites(FILE *spritecfg)
{
	/* get total number of sprites - first line of config file after tab */
	char line_buffer[BUFFER_SIZE] = { 0 };
	char num_buffer[BUFFER_SUB_SIZE] = { 0 };
	if( ! fgets(line_buffer, (BUFFER_SIZE - 1), spritecfg) )
		return 0;
	else
	{
		int i, index = 0;
		for(i=0; line_buffer[i] != TAB_CHAR; i++);
		i++; /* skip tab */
		for(i=i; line_buffer[i] != '\n' && line_buffer[i] != '\0'; i++)
		{
			num_buffer[index] = line_buffer[i];
			index++;
		}
		return atoi(num_buffer);
	}
}

int make_sprite_catalog(const char *cfgfile, rpgSprite **spriteptr)
{
	FILE *fp;
	int num_sprites;
	rpgSprite *spritearr = NULL;
	
	fp = fopen(cfgfile, "r");
	if( ! fp )
	{
		return 0;
	}
	
	num_sprites = get_num_sprites(fp); printf("Total: %d sprites\n", num_sprites);
	if( num_sprites < 1)
	{
		return 0;
	}
	
	spritearr = malloc(num_sprites * sizeof(rpgSprite));
	if( prepare_sprite_catalog(spritearr, num_sprites, fp) ) {
		fclose(fp);
		*spriteptr = spritearr;
		//printf("Address 1: %p\n", *spriteptr);
		return num_sprites;
	}
	else {
		fclose(fp);
		free(spritearr);
		return 0;
	}
}

void destroy_sprite_catalogue(rpgSprite *spritearr, int num)
{
	free(spritearr);
	
	int i;
	for(i=0; i<num; i++) {
		if( spritearr[i].sprite )
			al_destroy_bitmap(spritearr[i].sprite);
	}
}
