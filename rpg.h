#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

/*===Misc / Debug Settings===*/
#define DEBUG 2
/*===========================*/

/*======Display Settings=====*/
#define SCREEN_RES_X 640
#define SCREEN_RES_Y 480
#define BASE_SIZE 32
#define FPS 30

#define NUM_TILES ( SCREEN_RES_X / BASE_SIZE ) * (SCREEN_RES_Y / BASE_SIZE )
/*===========================*/

/*======Player Settings======*/
#define NUM_PLAYER_SPRITES 4
#define PLAYER_SPEED 4
#define SPRITE_UPDATE_T 0.1
/*===========================*/

/*====Health Bar Settings====*/
#define HBAR_SIZE 2
#define HBAR_HEIGHT 10
#define HBAR_X_OFFSET 400
#define HBAR_Y_OFFSET 5
/*===========================*/

#define BIT_ITER(iter) ( 1 << iter )

#define BUFFER_SIZE 2000
#define BUFFER_SUB_SIZE 1000

typedef struct {
	int res_x;
	int res_y;
	int base_size;
	ALLEGRO_DISPLAY *disp_ptr;
	int fps;
} displayData;

typedef struct {
	int x1, x4;
	int y1, y4;
	int tolerance; /* Amount that player can be larger than tile */
	ALLEGRO_COLOR pcolour;
	ALLEGRO_BITMAP *sprites[NUM_PLAYER_SPRITES];
	int current_sprite;
	int inventory;
	float health; /* Health 0.0 - 10.0 */
} playerData;

typedef struct {
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_EVENT event;
} eventData;

typedef struct {
	ALLEGRO_TIMER *frame_timer;
	ALLEGRO_TIMER *sprite_timer;
} timerData;

typedef struct {
	int quit;
	int score;
	displayData display;
	playerData player;
	eventData event_set;
	timerData timer_set;
	ALLEGRO_BITMAP *sprites[2];
	int keys[5];
} gameData;

enum PLAYER_KEYS { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ACTION };

enum INVENTORY_ITEMS { KEY_1 = BIT_ITER(0), KEY_2 = BIT_ITER(1) };

typedef struct {
	ALLEGRO_BITMAP *sprite;
	void (*action)(gameData *data, int id);
	int id;
	int x1, x4;
	int y1, y4;
} rpgTile;

typedef struct {
	ALLEGRO_BITMAP *sprite;
	char id[200];
	int base_size;
} rpgSprite;


int display_init(displayData *display, int res_x, int res_y, int bsize);
int draw_tiles(rpgTile *tilearr, displayData *display, int num);
void draw_player(playerData *player);
void draw_health(playerData *player);

int make_tile_set(rpgTile *tileset, int num_tiles, rpgSprite *spriteset, int num_sprites, char sprite_ids[NUM_TILES][BUFFER_SUB_SIZE]);
ALLEGRO_BITMAP *get_sprite_from_id(rpgSprite *spriteset, int num, char *sprite_id);
int get_sprite_index(rpgSprite *spriteset, int num, char *sprite_id);

void init_tileset(rpgTile *tilearr, gameData *data, int num, int pos_x, int pos_y);
void poll_intersect(rpgTile *tilearr, gameData *data, int num);
void process_player(playerData *player, int keys[5]);
int update_sprite(int *current, int max);

int make_sprite_catalog(const char *cfgfile, rpgSprite **spriteptr);
void destroy_sprite_catalogue(rpgSprite *spritearr, int num);

void create_event_data(gameData *config);
void wait_for_event(gameData *config);

#define RPG_DISPLAY(gamestruct) (gamestruct.display.disp_ptr)
#define RPG_EVENT(gamestruct) (gamestruct.event_set.event)
#define RPG_EVENT_QUEUE(gamestruct) (gamestruct.event_set.event_queue)
#define RPG_TIMER_SET(gamestruct) (gamestruct.timer_set)
