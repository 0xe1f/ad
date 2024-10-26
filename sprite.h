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

#ifndef SPRITE_H
#define SPRITE_H

struct Sprite {
    unsigned char *bitmap;
    int width;
    int height;
    int bpp;
    int frame_width;
    int frame;
    int reverse_anim;
    int anim_dir;
};

inline int sprite_frame_count(const struct Sprite *sprite) {
    return sprite->width / sprite->frame_width;
}

void sprite_animate(struct Sprite *sprite);

#endif // SPRITE_H