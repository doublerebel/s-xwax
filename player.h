/*
 * Copyright (C) 2011 Mark Hills <mark@pogo.org.uk>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#include "track.h"

#define PLAYER_CHANNELS 2

struct player_t {
    struct track_t *track;

    /* Current playback parameters */

    double position, /* seconds */
        target_position, /* seconds */
        offset, /* track start point in timecode */
        last_difference; /* last known position minus target_position */
    float pitch, /* from timecoder */
        sync_pitch, /* pitch required to sync to timecode signal */
        volume;

    bool target_valid;
    bool playing; /* flag indicating whether a deck is running */
    bool deck_protection; /* flag indicating whether deck protection is enabled */

    /* Timecode control */

    struct timecoder_t *timecoder;
    bool timecode_control,
        recalibrate; /* re-sync offset at next opportunity */
};

void player_init(struct player_t *pl, struct track_t *track);
void player_clear(struct player_t *pl);

void player_connect_timecoder(struct player_t *pl, struct timecoder_t *tc);
void player_set_timecode_control(struct player_t *pl, bool on);
bool player_toggle_timecode_control(struct player_t *pl);

int player_recue(struct player_t *pl);

void player_collect(struct player_t *pl, signed short *pcm,
                    int samples, int rate);

#endif
