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
