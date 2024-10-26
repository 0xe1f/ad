// Copyright (c) 2020 Akop Karapetyan
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

#include "screen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX(a, b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))

int screen_create(
    struct Screen *screen,
    int width,
    int height,
    int bpp
) {
    if (screen->buffer) {
        fprintf(stderr, "screen_create(): buffer already allocated\n");
        return 0;
    }

    int buffer_size = width * height * bpp;
    if ((screen->buffer = (unsigned char *)malloc(buffer_size)) == NULL) {
        fprintf(stderr, "screen_create(): malloc failed\n");
        return 0;
    }

    screen->buffer_size = buffer_size;
    screen->width = width;
    screen->height = height;
    screen->bpp = bpp;

    return 1;
}

void screen_destroy(
    struct Screen *screen
) {
    if (screen->buffer) {
        free(screen->buffer);
        screen->buffer = NULL;
    }
}

void screen_clear(
    const struct Screen *screen
) {
    memset(screen->buffer, 0, screen->buffer_size);
}

void screen_draw_sprite(
    const struct Screen *screen,
    const struct Sprite *sprite,
    int x,
    int y
) {
    if (
        x >= screen->width 
            || y >= screen->height
            || x + sprite->frame_width < 0
            || y + sprite->height < 0
    ) {
        // Nothing visible
        return;
    }

    // FIXME: This logic falls apart if sprite/screen bpp do not match

    int vcopy = sprite->height;
    int vread = 0;
    int vwrite = y;
    if (y + sprite->height >= screen->height) {
        vcopy = screen->height - y;
    } else if (y < 0) {
        vread = -y;
        vwrite = 0;
        vcopy += y;
    }
    unsigned char *in_row = sprite->bitmap + vread * sprite->width * sprite->bpp;
    unsigned char *out_row = screen->buffer + vwrite * screen->width * screen->bpp;
    int i;
    for (i = 0; i < vcopy; i++) {
        int hcopy = sprite->frame_width;
        int hread = 0;
        int hwrite = x;
        if (x + sprite->frame_width >= screen->width) {
            hcopy = screen->width - x;
        } else if (x < 0) {
            hread = -x;
            hwrite = 0;
            hcopy += x;
        }
        unsigned char *in_col = in_row + (sprite->frame_width * sprite->frame + hread) * sprite->bpp;
        unsigned char *out_col = out_row + hwrite * sprite->bpp;
        memcpy(out_col, in_col, hcopy * sprite->bpp);
        out_row += screen->width * screen->bpp;
        in_row += sprite->width * sprite->bpp;
    }
}
