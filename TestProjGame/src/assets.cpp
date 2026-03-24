#include "assets.h"

SpriteSheet player_sprite_sheet = { 0, 0, 70 * 3, 70, 3 };
SpriteSheet enemy_sprite_sheet = { 210, 0, 70 * 3, 86, 3 };
SpriteSheet tree_sprite_sheet = { 4, 193, 118, 224, 1 };

CollisionBox player_box = { 40, 20, 0, -5  };
CollisionBox enemy_box = { 24, 18, 0, 0 };
CollisionBox tree_box = { 32, 186, 0, 0 };