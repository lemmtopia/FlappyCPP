#pragma once

#include "typedefs.h"

struct SpriteSheet {
    u16 src_x = 0;
    u16 src_y = 0;
    u16 src_w = 0;
    u16 src_h = 0;
    
    u8 frame_count;
};

struct CollisionBox {
    u8 w = 0;
    u8 h = 0;

    i8 offx = 0;
    i8 offy = 0;
};

extern SpriteSheet player_sprite_sheet;
extern SpriteSheet enemy_sprite_sheet;
extern SpriteSheet tree_sprite_sheet;

extern CollisionBox player_box;
extern CollisionBox enemy_box;
extern CollisionBox tree_box;