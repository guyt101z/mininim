/*
  room.c -- room module;

  Copyright (C) 2015, 2016 Bruno Félix Rezende Ribeiro <oitofelix@gnu.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <error.h>
#include <allegro5/allegro.h>

#include "prince.h"
#include "kernel/video.h"
#include "kernel/random.h"
#include "physics.h"
#include "level.h"
#include "kid.h"
#include "floor.h"
#include "pillar.h"
#include "wall.h"
#include "room.h"


ALLEGRO_BITMAP *bricks_1, *torch;

/* room bitmap */
ALLEGRO_BITMAP *room_bg;

/* current room */
static unsigned int room = -1;

void
load_room (void)
{
  room_bg = create_bitmap (ORIGINAL_WIDTH, ORIGINAL_HEIGHT);

  switch (level->type) {
  case DUNGEON:
    switch (video_mode) {
    case VGA:
      load_vdungeon_floor ();
      load_vdungeon_wall ();
      load_vdungeon_pillar ();
      bricks_1 = load_bitmap (VDUNGEON_BRICKS_1);
      torch = load_bitmap (VDUNGEON_TORCH);
      break;
    default:
      error (-1, 0, "%s: unknown video mode (%u)", __func__, video_mode);
    }
    break;
  case PALACE:
    break;
  default:
    error (-1, 0, "%s: unknown level type (%i)", __func__, level->type);
  }
}

void
unload_room (void)
{
  unload_floor ();
  unload_wall ();
  unload_pillar ();

  /* bitmaps */
  al_destroy_bitmap (bricks_1);
  al_destroy_bitmap (torch);
}

void
draw_room (int _room)
{
  if (_room != room) {
    room = _room;
    draw_room_bg ();
  }
  draw_bitmap (room_bg, screen, 0, 0, 0);
}


void
draw_room_bg (void)
{
  struct pos p;
  p.room = room;

  clear_bitmap (room_bg, BLACK);

  for (p.floor = FLOORS - 1; p.floor >= -1; p.floor--)
    for (p.place = -1; p.place < PLACES; p.place++)
      draw_construct (room_bg, p);
}

void
draw_construct (ALLEGRO_BITMAP *bitmap, struct pos p)
{
  draw_construct_fg (bitmap, p);
  draw_construct_bg (bitmap, p);
}

void
draw_construct_fg (ALLEGRO_BITMAP *bitmap, struct pos p)
{
  switch (construct (p).fg) {
  case NO_FLOOR:
    break;
  case FLOOR:
    draw_floor (bitmap, p);
    break;
  case BROKEN_FLOOR:
    draw_broken_floor (bitmap, p);
    break;
  case LOOSE_FLOOR:
    draw_floor (bitmap, p);
    break;
  case PILLAR:
    draw_pillar (bitmap, p);
    break;
  case WALL:
    draw_wall (bitmap, p);
    break;
  default:
    error (-1, 0, "%s: unknown foreground (%u)",
           __func__, construct (p).fg);
  }
}

void
draw_construct_bg (ALLEGRO_BITMAP *bitmap, struct pos p)
{
  switch (construct (p).bg) {
  case NO_BG:
    break;
  case BRICKS_01:
    draw_bricks_01 (bitmap, p);
    break;
  case TORCH:
    draw_torch (bitmap, p);
    break;
  default:
    error (-1, 0, "%s: unknown background (%u)",
           __func__, construct (p).bg);
  }
}

void
draw_bricks_01 (ALLEGRO_BITMAP *bitmap, struct pos p)
{
  draw_bitmap_xy (bricks_1, bitmap, bricks_xy (p) , 0);
}

struct xy
bricks_xy (struct pos p)
{
  struct xy xy;
  xy.x = PLACE_WIDTH * (p.place + 1);
  xy.y = PLACE_HEIGHT * p.floor + 15;
  return xy;
}

void
draw_torch (ALLEGRO_BITMAP *bitmap, struct pos p)
{
  draw_bitmap_xy (torch, bitmap, torch_xy (p), 0);
}

struct xy
torch_xy (struct pos p)
{
  struct xy xy;
  xy.x = PLACE_WIDTH * (p.place + 1);
  xy.y = PLACE_HEIGHT * p.floor + 22;
  return xy;
}

void
draw_room_anim_fg (struct anim a)
{
  draw_room_fg (room_pos_bl (a));
  draw_room_fg (room_pos_br (a));
  draw_room_fg (room_pos_mid (a));
  draw_room_fg (room_pos_tl (a));
  draw_room_fg (room_pos_tr (a));
}

void
draw_room_fg (struct pos p)
{
  switch (construct (p).fg) {
  case NO_FLOOR:
    break;
  case FLOOR:
    draw_floor_fg (screen, p);
    break;
  case BROKEN_FLOOR:
    draw_broken_floor_fg (screen, p);
    break;
  case LOOSE_FLOOR:
    draw_floor_fg (screen, p);
    break;
  case PILLAR:
    draw_pillar_fg (screen, p);
    break;
  case WALL:
    draw_wall_fg (screen, p);
    break;
  default:
    error (-1, 0, "%s: unknown foreground (%u)",
           __func__, construct (p).fg);
  }
}

struct pos
room_pos_bl (struct anim a)
{
  int h = al_get_bitmap_height (a.frame);
  return room_pos_xy (a.room, a.x, a.y + h - 1);
}

struct pos
room_pos_br (struct anim a)
{
  int w = al_get_bitmap_width (a.frame);
  int h = al_get_bitmap_height (a.frame);
  return room_pos_xy (a.room, a.x + w - 1, a.y + h - 1);
}

struct pos
room_pos_mid (struct anim a)
{
  int w = al_get_bitmap_width (a.frame);
  int h = al_get_bitmap_height (a.frame);
  return room_pos_xy (a.room, a.x + w / 2, a.y + h / 2);
}

struct pos
room_pos_tl (struct anim a)
{
  return room_pos_xy (a.room, a.x, a.y);
}

struct pos
room_pos_tr (struct anim a)
{
  int w = al_get_bitmap_width (a.frame);
  return room_pos_xy (a.room, a.x + w - 1, a.y);
}

struct pos
room_pos_xy (unsigned int room, int x, int y)
{
  struct pos p;

  unsigned int qy = y / PLACE_HEIGHT;
  unsigned int ry = y % PLACE_HEIGHT;

  p.room = room;
  p.place = x / PLACE_WIDTH;
  p.floor = (ry < 3) ? qy - 1 : qy;

  if (x < 0) p.place = -1;
  if (y < 0) p.floor = -1;

  return p;
}