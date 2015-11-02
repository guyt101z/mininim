/*
  room.h -- room module;

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

#ifndef FREEPOP_ROOM_H
#define FREEPOP_ROOM_H

#define VDUNGEON_BRICKS_1 "dat/vdungeon/background/bricks01.png"
#define VDUNGEON_TORCH "dat/vdungeon/background/torch.png"

void load_room (void);
void unload_room (void);
void draw_room (int _room);
void draw_room_bg (void);
void draw_construct (ALLEGRO_BITMAP *bitmap, struct pos pos);
void draw_construct_fg (ALLEGRO_BITMAP *bitmap, struct pos pos);
void draw_construct_bg (ALLEGRO_BITMAP *bitmap, struct pos pos);
void draw_bricks_01 (ALLEGRO_BITMAP *bitmap, struct pos pos);
struct xy bricks_xy (struct pos pos);
void draw_torch (ALLEGRO_BITMAP *bitmap, struct pos pos);
struct xy torch_xy (struct pos pos);
void draw_room_anim_fg (struct anim a);
void draw_room_fg (struct pos p);
struct pos room_pos_bl (struct anim a);
struct pos room_pos_br (struct anim a);
struct pos room_pos_mid (struct anim a);
struct pos room_pos_tl (struct anim a);
struct pos room_pos_tr (struct anim a);
struct pos room_pos_xy (unsigned int room, int x, int y);

#endif	/* FREEPOP_ROOM_H */
