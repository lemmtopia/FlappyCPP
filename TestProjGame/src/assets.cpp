#include "assets.h"

SpriteSheet player_sprite_sheet = { 0, 0, 70 * 3, 70, 3, 70 };
SpriteSheet enemy_sprite_sheet = { 210, 0, 70 * 3, 86, 3, 70 };
SpriteSheet tree_sprite_sheet = { 4, 193, 118, 224, 1, 118 };

CollisionBox player_box = { 0, -5, 40, 20 };
CollisionBox enemy_box = { 0, 0, 24, 18 };
CollisionBox tree_box = { 0, 0, 32, 186 };