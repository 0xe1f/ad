// Copyright (c) 2024 Akop Karapetyan
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rgbserver.h"

#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>

#include "screen.h"
#include "sprite.h"

#include "toaster.h"
#include "toasts.h"

struct Screen screen = {};

volatile int interrupt_received = 0;

#define SPRITE_TOASTER 1
#define SPRITE_TOAST_0 2
#define SPRITE_TOAST_1 3
#define SPRITE_TOAST_2 4
#define SPRITE_TOAST_3 5

#define SYNC_ANIM_DELAY ((long long)((0.05)*1000000.0))

struct SpriteDef {
    unsigned char type;
    signed short x;
    signed short y;
};

static void InterruptHandler(int signo) {
    interrupt_received = 1;
    rgbs_end();
    screen_destroy(&screen);
}

long long millis() {
    static struct timeval tv;
    gettimeofday(&tv, 0);
    return (tv.tv_sec) * 1000LL + tv.tv_usec / 1000LL;
}

void sprite_init(
    struct Sprite *sprite,
    const struct SpriteDef *def
) {
    if (def->type == SPRITE_TOASTER) {
        sprite->bitmap = (unsigned char *) toaster_bmp;
        sprite->width = 256;
        sprite->height = 64;
        sprite->frame_width = 64;
        sprite->anim_dir = 1;
        sprite->frame = 0;
    } else if (
        def->type == SPRITE_TOAST_0
        || def->type == SPRITE_TOAST_1
        || def->type == SPRITE_TOAST_2
        || def->type == SPRITE_TOAST_3
    ) {
        sprite->bitmap = (unsigned char *) toasts_bmp;
        sprite->width = 256;
        sprite->height = 64;
        sprite->frame_width = 64;
        sprite->frame = def->type - SPRITE_TOAST_0;
    } else {
        return;
    }

    sprite->bpp = 2;
    sprite->x = def->x;
    sprite->y = def->y;
    sprite->type = def->type;
    sprite->format = FORMAT_RGBA5551;
}

void sprite_animate(
    struct Sprite *sprite
) {
    int frame = sprite->frame;
    if (sprite->type == SPRITE_TOASTER) {
        frame += sprite->anim_dir;
        int fc = sprite_frame_count(sprite);
        if (frame >= (fc - 1) || frame <= 0) {
            sprite->anim_dir *= -1;
        }
    } else if (
        sprite->type == SPRITE_TOAST_0
        || sprite->type == SPRITE_TOAST_1
        || sprite->type == SPRITE_TOAST_2
        || sprite->type == SPRITE_TOAST_3
    ) {
    }
    sprite->frame = frame;
}

int main(int argc, char **argv) {
    if (!screen_create(&screen, 320, 240, 2)) {
        fprintf(stderr, "screen_create() failed\n");
        return 1;
    }

    struct FrameGeometry data = {
        screen.buffer_size,
        screen.width * screen.bpp,
        screen.width,
        screen.height,
        PIXEL_FORMAT_RGBA5551,
        0,
        MAGIC_NUMBER
    };
    rgbs_set_buffer_data(data);

    if (!rgbs_start()) {
        fprintf(stderr, "Error initializing rgbs\n");
        return 1;
    }

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    struct SpriteDef defaults[] = {
        { SPRITE_TOASTER, 320, -64 },
        { SPRITE_TOAST_0, 240, -80 },
        { SPRITE_TOASTER, 160, -128 },
        { SPRITE_TOAST_1, 80, -64 },
    };
    const int kount = 4;

    struct Sprite sprites[kount];
    int s;
    for (s = 0; s < kount; s++) {
        sprite_init(&sprites[s], &defaults[s]);
    }

    while (!interrupt_received) {
        // long long tick = millis();
        usleep(SYNC_ANIM_DELAY);

        screen_clear(&screen);

        for (s = 0; s < kount; s++) {
            struct Sprite *sprite = &sprites[s];
            sprite_animate(sprite);
            sprite->x--;
            sprite->y++;

            if (sprite->x < 64 && sprite->y > 256) {
                struct SpriteDef *def = &defaults[s];
                sprite->x = def->x;
                sprite->y = def->y;
            }
            screen_draw_sprite(&screen, sprite);
        }

        rgbs_poll();
        rgbs_send(screen.buffer, screen.buffer_size);
    }

    screen_destroy(&screen);

    return 0;
}
