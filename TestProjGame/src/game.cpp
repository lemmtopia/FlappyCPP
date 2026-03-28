#include "game.h"

GameState game_state;

static SDL_Texture* atlas_texture;
static MIX_Audio* levelup_sound;
static MIX_Audio* hurt_sound;
static TTF_Font* game_font;

static f32 gravity = 0.2;

static SDL_Color text_color = { 255, 255, 255, 255 };
static SDL_FRect text_rect = { 0, 0, 18 * 6, 20 };

void game_setup(void) {
    // Load textures
    atlas_texture = sdl_state_load_texture(sdl_state, "assets/textures/flappy_atlas.png");
    SDL_SetTextureScaleMode(atlas_texture, SDL_SCALEMODE_NEAREST);

    // Load sounds
    levelup_sound = MIX_LoadAudio(sdl_state.mixer, "assets/sounds/levelup.wav", true);
    hurt_sound = MIX_LoadAudio(sdl_state.mixer, "assets/sounds/hurt.wav", true);

    // Load font
    game_font = TTF_OpenFont("assets/fonts/BadComic-Regular.ttf", 18);

    game_reset();

    game_state.player_death = true;
    gravity = 0;

    // Setup input state
    initialize_input_state();
}

void game_update(void) {
    if (!game_state.player_death) {
        game_state.score += 0.1;
        if (game_state.score >= game_state.score_next) {
            game_state.speed += 0.2;
            game_state.score_next *= 1.5;
        }

        if (game_state.score > 9999) {
            game_state.score = 9999;
        }

        if (game_state.update_rate > 0) {
            game_state.spawn_timer--;
            if (game_state.spawn_timer <= 0) {
                if (randf() > 0.45) {
                    spawn_enemy();
                }
                else {
                    spawn_tree();
                }

                game_state.spawn_timer = randf_range(game_state.update_rate * 2, game_state.update_rate * 4);
            }
        }
    }

    for (size_t i = 0; i < game_state.entity_count; i++) {
        Entity* e = &game_state.entities[i];

        update_entity(e, i);
    }

    update_input_state();
}

void game_render(void) {
    SDL_SetRenderDrawColor(sdl_state.renderer, 80, 80, 80, 255);
    SDL_RenderClear(sdl_state.renderer);

    SDL_SetRenderDrawColor(sdl_state.renderer, 255, 255, 255, 255);

    for (size_t i = 0; i < game_state.entity_count; i++) {
        if (game_state.entities[i].allocated) {
            draw_entity(game_state.entities[i]);
        }
    }

    char score_buff[16];
    sprintf_s(score_buff, 16, "Score: %04d", (i32)game_state.score);

    SDL_Texture* score_text = make_text_simple(game_font, score_buff, 11, text_color);
    SDL_SetTextureScaleMode(score_text, SDL_SCALEMODE_NEAREST);

    SDL_RenderTexture(sdl_state.renderer, score_text, NULL, &text_rect);

    SDL_DestroyTexture(score_text);

    SDL_RenderPresent(sdl_state.renderer);
}

void game_destroy_resources(void) {
    // Destroy font
    TTF_CloseFont(game_font);

    // Destroy mixer
    MIX_DestroyAudio(levelup_sound);
    MIX_DestroyAudio(hurt_sound);

    // Unload textures
    SDL_DestroyTexture(atlas_texture);
}

void entity_change_sprite_sheet(Entity* entity, u16 x, u16 y, u16 w, u16 h, u8 frame_count) {
    entity->sheet.src_x = x;
    entity->sheet.src_y = y;
    entity->sheet.src_w = w;
    entity->sheet.src_h = h;

    entity->sheet.frame_count = frame_count;
}

void entity_make_box(Entity* entity, i8 offx, i8 offy, u8 w, u8 h) {
    entity->box.offx = offx;
    entity->box.offy = offy;
    entity->box.w = w;
    entity->box.h = h;
}

void entity_setup_animation(Entity* entity, u8 count, u8 speed) {
    entity->sheet.frame_count = count;
    entity->frame_speed = speed;
    entity->frame_timer = 0;
    entity->cur_frame = 0;
}

void destroy_entity(Entity* entity) {
    entity->allocated = false;
}

bool entity_collide(Entity* e, Entity* e2) {
    SDL_FRect a = { (e->x - e->box.w / 2) + e->box.offx, (e->y - e->box.h / 2) + e->box.offy, e->box.w, e->box.h };
    SDL_FRect b = { (e2->x - e->box.w / 2) + e2->box.offx, (e2->y - e2->box.h / 2) + e2->box.offy, e2->box.w, e2->box.h };

    return ((a.x <= b.x + b.w && a.x + a.w >= b.x) && (a.y <= b.y + b.h && a.y + a.h >= b.y));
}

Entity* initialize_entity(f32 x, f32 y, EntityType type) {
    int index = -1;
    for (size_t i = 0; i < MAX_ENTITIES; i++) {
        if (!game_state.entities[i].allocated) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        return NULL;
    }

    game_state.entities[index].x = x;
    game_state.entities[index].y = y;
    game_state.entities[index].type = type;
    game_state.entities[index].allocated = true;

    Entity* e = &game_state.entities[index];
    game_state.entity_count++;

    return e;
}

void update_entity(Entity* e, size_t e_index) {
    if (!e->allocated) {
        return;
    }

    u16 frame_w = e->sheet.src_w / e->sheet.frame_count;

    switch (e->type) {
    case ENTITY_TYPE_PLAYER:
        if (game_state.player_death) {
            e->velocity_y += gravity;
            e->frame_timer = 0;

            // Game over
            if (e->y > BASE_H + 80) {
                e->y = BASE_H + 80;
            }

            if (input_state.keys[SDL_SCANCODE_SPACE] && !input_state.old_keys[SDL_SCANCODE_SPACE]) {
                game_state.entity_count = 0;
                game_reset();
            }
        }
        else {
            if (input_state.keys[SDL_SCANCODE_SPACE] && !input_state.old_keys[SDL_SCANCODE_SPACE]) {
                e->velocity_y = -4;
            }

            e->velocity_y += gravity;
            if (e->velocity_y > 5) {
                e->velocity_y = 5;
            }
            else if (e->velocity_y < -5) {
                e->velocity_y = -5;
            }

            if (e->y > BASE_H + 80 || e->y < 0) {
                // Death by going outside the screen
                e->velocity_y = -5;
                e->flip = 2;
                game_state.player_death = true;
            }

            for (size_t j = 0; j < game_state.entity_count; j++) {
                if (j != e_index) {
                    Entity* e2 = &game_state.entities[j];

                    if (entity_collide(e, e2) && (e2->type == ENTITY_TYPE_ENEMY || e2->type == ENTITY_TYPE_TREE)) {
                        e->velocity_y = -5;

                        MIX_PlayAudio(sdl_state.mixer, hurt_sound);

                        // Death by collision
                        e->flip = 2;
                        game_state.player_death = true;
                        //destroy_entity(e2);
                        break;
                    }
                }

            }
        }
        break;
    case ENTITY_TYPE_ENEMY:
        if (e->x < -frame_w / 2) {
            destroy_entity(e);
        }
        break;
    case ENTITY_TYPE_TREE:
        if (e->x < -frame_w / 2) {
            destroy_entity(e);
        }
        break;
    }

    e->frame_timer++;
    if (e->frame_timer >= e->frame_speed) {
        e->cur_frame++;
        if (e->cur_frame >= e->sheet.frame_count) {
            e->cur_frame = 0;
        }

        e->frame_timer = 0;
    }

    e->x += e->velocity_x;
    e->y += e->velocity_y;
}

void game_clear_entity_array(void) {
    for (size_t i = 0; i < MAX_ENTITIES; i++) {
        game_state.entities[i] = {};
        game_state.entities[i].allocated = false;
    }
}

void game_reset() {
    game_clear_entity_array();

    game_state.spawn_timer = 0;
    game_state.score = 0;
    game_state.score_next = 50;
    game_state.speed = 1;

    game_state.entity_count = 0;
    game_state.player_death = false;

    game_state.spawn_timer = game_state.spawn_timer * 3;

    Entity* player = initialize_entity(80, BASE_H / 3, ENTITY_TYPE_PLAYER);

    player->sheet = player_sprite_sheet;
    player->box = player_box;
    entity_setup_animation(player, 3, 12);

    gravity = 0.2;
}

void draw_entity(Entity entity) {
    u16 frame_w = entity.sheet.src_w / entity.sheet.frame_count;
    u16 src_x = entity.sheet.src_x + frame_w * entity.cur_frame;

    SDL_FRect src = { src_x, entity.sheet.src_y, frame_w, entity.sheet.src_h };
    SDL_FRect dst = { entity.x - frame_w / 2, entity.y - entity.sheet.src_h / 2, frame_w, entity.sheet.src_h };
    SDL_FlipMode flip = SDL_FLIP_NONE;
    switch (entity.flip) {
    case 1:
        flip = SDL_FLIP_HORIZONTAL;
        break;
    case 2:
        flip = SDL_FLIP_VERTICAL;
        break;
    case 3:
        flip = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
        break;
    default:
        flip = SDL_FLIP_NONE;
        break;
    }
    SDL_RenderTextureRotated(sdl_state.renderer, atlas_texture, &src, &dst, 0, NULL, flip);

    if (game_state.is_debug) {
        SDL_FRect box = { (entity.x - entity.box.w / 2) + entity.box.offx, (entity.y - entity.box.h / 2) + entity.box.offy, entity.box.w, entity.box.h };
        SDL_RenderRect(sdl_state.renderer, &box);
    }
}

void spawn_enemy(void) {
    // Spawn enemy
    Entity* enemy = initialize_entity(randf_range(BASE_W + (2 * 32), BASE_W + (4 * 32)), randf_range(90, BASE_H - 90), ENTITY_TYPE_ENEMY);
    enemy->flip = 1;
    enemy->velocity_x = -2 * game_state.speed;
    enemy->sheet = enemy_sprite_sheet;
    enemy->box = enemy_box;
    entity_setup_animation(enemy, 3, 18);
}

void spawn_tree(void) {
    // Spawn tree
    Entity* tree = initialize_entity(randf_range(BASE_W + (5 * 32), BASE_W + (8 * 32)), randf_range(270, BASE_H), ENTITY_TYPE_TREE);
    tree->velocity_x = -1 * game_state.speed;
    tree->sheet = tree_sprite_sheet;
    tree->box = tree_box;
    entity_setup_animation(tree, 1, 0);
}

SDL_Texture* make_text_simple(TTF_Font* font, char* text, size_t length, SDL_Color color) {
    SDL_Surface* surf = TTF_RenderText_Solid(font, text, length, color);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(sdl_state.renderer, surf);

    SDL_DestroySurface(surf);

    return tex;
}