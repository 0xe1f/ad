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

#include "sprite.h"

#include <string.h>

void sprite_animate(
    struct Sprite *sprite
) {
    sprite->frame += sprite->anim_dir;
    int fc = sprite_frame_count(sprite);
    if (sprite->reverse_anim) {
        if (sprite->frame >= (fc - 1) || sprite->frame <= 0) {
            sprite->anim_dir *= -1;
        }
    } else {
        if (sprite->frame >= fc) {
            sprite->frame = 0;
        } else if (sprite->frame < 0) {
            sprite->frame = fc - 1;
        }
    }
}

void sprite_draw(
    const struct Sprite *sprite,
    const struct Screen *screen,
    int x,
    int y
) {
    if (
        x >= screen->width 
            || y >= screen->height
            || x + sprite->frame_width < 0
            || y + sprite->height < 0
    ) {
        return;
    }

    unsigned char *out = screen->buffer;
    unsigned char *in = sprite->bitmap;
    int i;
    int vcopy = sprite->height;
    for (i = 0; i < sprite->height; i++) {
        unsigned char *in_start = in + sprite->frame_width * sprite->frame * sprite->bpp;
        unsigned char *out_start = out;
        int hcopy = sprite->frame_width * sprite->bpp;
        memcpy(out_start, in_start, hcopy);
        out += screen->width * screen->bpp;
        in += sprite->width * sprite->bpp;
    }
}
