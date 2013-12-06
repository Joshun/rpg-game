#include <allegro5/allegro5.h>
#include "rpg.h"

void create_event_data(gameData *config)
{
	config->event_set.event_queue = al_create_event_queue();
}

void wait_for_event(gameData *config)
{
	al_wait_for_event(config->event_set.event_queue, &(config->event_set.event));
}
