#pragma once

#include "typedefs.h"

#define ATLAS_TEXTURE_PATH "assets/textures/flappy_atlas.png"
#define HURT_SOUND_PATH "assets/sounds/flappy_atlas.png"

struct SpriteSheet {
    u16 src_x = 0;
    u16 src_y = 0;
    u16 src_w = 0;
    u16 src_h = 0;
    u16 frame_count;
    u16 frame_w;
};

struct CollisionBox {
    i16 offx = 0;
    i16 offy = 0;
    u16 w = 0;
    u16 h = 0;
};

extern SpriteSheet player_sprite_sheet;
extern SpriteSheet enemy_sprite_sheet;
extern SpriteSheet tree_sprite_sheet;

extern CollisionBox player_box;
extern CollisionBox enemy_box;
extern CollisionBox tree_box;