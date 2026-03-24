#pragma once

#include "typedefs.h"
#include "sdl_state.h"
#include "input_state.h"
#include "util.h"
#include "assets.h"

#define MAX_ENTITIES 128

enum EntityType : u8 {
    ENTITY_TYPE_NONE,
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_TREE,
    ENTITY_TYPE_ENEMY,
    ENTITY_TYPE_PICKUP
};

struct Entity {
    bool flip = false;
    u8 frame_timer = 0;
    u8 frame_speed = 1;
    u8 cur_frame = 0;
    EntityType type = ENTITY_TYPE_NONE;
    bool allocated = false;

    CollisionBox box;
    SpriteSheet sheet;

    f32 x = 0;
    f32 y = 0;
    f32 angle = 0;
    f32 velocity_x = 0;
    f32 velocity_y = 0;
};

struct GameState {
    Entity entities[MAX_ENTITIES];

    bool player_death = false;
    bool is_debug = true;
    int entity_count = 0;
    f32 gravity = 0.2;

    i16 update_rate = 60;
    i16 spawn_timer = 0;
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
void entity_change_sprite_sheet(Entity* entity, u16 x, u16 y, u16 w, u16 h, u16 frame_count);
void entity_make_box(Entity* entity, i16 offx, i16 offy, u16 w, u16 h);
void entity_setup_animation(Entity* entity, u8 count, u8 speed);
void draw_entity(Entity entity);