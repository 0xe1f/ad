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

#ifndef SPRITE_H
#define SPRITE_H

struct Sprite {
    unsigned char *bitmap;
    unsigned char type;
    unsigned short width;
    unsigned short height;
    unsigned char bpp;
    unsigned char frame_width;
    unsigned char frame;
    signed char anim_dir;
    signed short x;
    signed short y;
    unsigned char format;
    float velocity;
    short delta;
};

#define FORMAT_RGBA5551 1

inline int sprite_frame_count(const struct Sprite *sprite) {
    return sprite->width / sprite->frame_width;
}

#endif // SPRITE_H