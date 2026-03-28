#pragma once

#include "typedefs.h"
#include "sdl_state.h"
#include "input_state.h"
#include "util.h"
#include "assets.h"

#define MAX_ENTITIES 64

enum EntityType : u8 {
    ENTITY_TYPE_NONE,
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_TREE,
    ENTITY_TYPE_ENEMY,
    ENTITY_TYPE_PICKUP
};

struct Entity {
    f32 x = 0;
    f32 y = 0;
    f32 velocity_x = 0;
    f32 velocity_y = 0;

    CollisionBox box;
    SpriteSheet sheet;

    u8 frame_timer = 0;
    u8 frame_speed = 1;
    u8 cur_frame = 0;

    u8 flip = 0;
    bool allocated = false;

    EntityType type = ENTITY_TYPE_NONE;
};

struct GameState {
    bool player_death = false;
    bool is_debug = true;

    i16 update_rate = 60;
    i16 spawn_timer = 0;
    u16 entity_count = 0;

    f32 score = 0;
    f32 score_next = 50;
    f32 speed = 1;

    Entity entities[MAX_ENTITIES];
};

extern GameState game_state;

void game_setup(void);
void game_update(void);
void game_render(void);
void game_destroy_resources(void);

void game_clear_entity_array(void);

void game_reset();

Entity* initialize_entity(f32 x, f32 y, EntityType type);
void update_entity(Entity* e, size_t e_index);
void entity_change_sprite_sheet(Entity* entity, u16 x, u16 y, u16 w, u16 h, u8 frame_count);
void entity_make_box(Entity* entity, i8 offx, i8 offy, u8 w, u8 h);
void entity_setup_animation(Entity* entity, u8 count, u8 speed);
void draw_entity(Entity entity);

void spawn_enemy(void);
void spawn_tree(void);

SDL_Texture* make_text_simple(TTF_Font* font, char* text, size_t length, SDL_Color color);