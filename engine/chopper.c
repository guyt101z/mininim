/*
  chopper.c -- chopper module;

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

#include <stdio.h>
#include <error.h>
#include "kernel/audio.h"
#include "kernel/array.h"
#include "kernel/random.h"
#include "anim.h"
#include "room.h"
#include "floor.h"
#include "loose-floor.h"
#include "kid/kid.h"
#include "level.h"
#include "chopper.h"

/* dungeon vga */
ALLEGRO_BITMAP *dv_chopper_01, *dv_chopper_fg_01,
  *dv_chopper_02, *dv_chopper_fg_02,
  *dv_chopper_03, *dv_chopper_fg_03,
  *dv_chopper_04, *dv_chopper_fg_04,
  *dv_chopper_05, *dv_chopper_fg_05;

/* palace vga */
ALLEGRO_BITMAP *pv_chopper_01, *pv_chopper_fg_01,
  *pv_chopper_02, *pv_chopper_fg_02,
  *pv_chopper_03, *pv_chopper_fg_03,
  *pv_chopper_04, *pv_chopper_fg_04,
  *pv_chopper_05, *pv_chopper_fg_05;

/* palettable */
ALLEGRO_BITMAP *chopper_blood_01, *chopper_blood_02,
  *chopper_blood_03, *chopper_blood_04, *chopper_blood_05;

ALLEGRO_SAMPLE *chopper_sample;

bool sample_chopper;

static struct chopper *chopper = NULL;
static size_t chopper_nmemb = 0;

void
load_chopper (void)
{
  ALLEGRO_BITMAP *chopper_blood_template_01, *chopper_blood_template_02,
    *chopper_blood_template_03, *chopper_blood_template_04,
    *chopper_blood_template_05;

  /* dungeon vga */
  dv_chopper_01 = load_bitmap (DV_CHOPPER_01);
  dv_chopper_fg_01 = load_bitmap (DV_CHOPPER_FG_01);
  dv_chopper_02 = load_bitmap (DV_CHOPPER_02);
  dv_chopper_fg_02 = load_bitmap (DV_CHOPPER_FG_02);
  dv_chopper_03 = load_bitmap (DV_CHOPPER_03);
  dv_chopper_fg_03 = load_bitmap (DV_CHOPPER_FG_03);
  dv_chopper_04 = load_bitmap (DV_CHOPPER_04);
  dv_chopper_fg_04 = load_bitmap (DV_CHOPPER_FG_04);
  dv_chopper_05 = load_bitmap (DV_CHOPPER_05);
  dv_chopper_fg_05 = load_bitmap (DV_CHOPPER_FG_05);

  /* palace vga */
  pv_chopper_01 = load_bitmap (PV_CHOPPER_01);
  pv_chopper_fg_01 = load_bitmap (PV_CHOPPER_FG_01);
  pv_chopper_02 = load_bitmap (PV_CHOPPER_02);
  pv_chopper_fg_02 = load_bitmap (PV_CHOPPER_FG_02);
  pv_chopper_03 = load_bitmap (PV_CHOPPER_03);
  pv_chopper_fg_03 = load_bitmap (PV_CHOPPER_FG_03);
  pv_chopper_04 = load_bitmap (PV_CHOPPER_04);
  pv_chopper_fg_04 = load_bitmap (PV_CHOPPER_FG_04);
  pv_chopper_05 = load_bitmap (PV_CHOPPER_05);
  pv_chopper_fg_05 = load_bitmap (PV_CHOPPER_FG_05);

  /* palettable */
  chopper_blood_template_01 = load_bitmap (CHOPPER_BLOOD_01);
  chopper_blood_template_02 = load_bitmap (CHOPPER_BLOOD_02);
  chopper_blood_template_03 = load_bitmap (CHOPPER_BLOOD_03);
  chopper_blood_template_04 = load_bitmap (CHOPPER_BLOOD_04);
  chopper_blood_template_05 = load_bitmap (CHOPPER_BLOOD_05);

  /* apply blood palette */
  chopper_blood_01 = apply_palette (chopper_blood_template_01, blood_palette);
  chopper_blood_02 = apply_palette (chopper_blood_template_02, blood_palette);
  chopper_blood_03 = apply_palette (chopper_blood_template_03, blood_palette);
  chopper_blood_04 = apply_palette (chopper_blood_template_04, blood_palette);
  chopper_blood_05 = apply_palette (chopper_blood_template_05, blood_palette);
  al_destroy_bitmap (chopper_blood_template_01);
  al_destroy_bitmap (chopper_blood_template_02);
  al_destroy_bitmap (chopper_blood_template_03);
  al_destroy_bitmap (chopper_blood_template_04);
  al_destroy_bitmap (chopper_blood_template_05);
}

void
unload_chopper (void)
{
  /* dungeon vga */
  al_destroy_bitmap (dv_chopper_01);
  al_destroy_bitmap (dv_chopper_fg_01);
  al_destroy_bitmap (dv_chopper_02);
  al_destroy_bitmap (dv_chopper_fg_02);
  al_destroy_bitmap (dv_chopper_03);
  al_destroy_bitmap (dv_chopper_fg_03);
  al_destroy_bitmap (dv_chopper_04);
  al_destroy_bitmap (dv_chopper_fg_04);
  al_destroy_bitmap (dv_chopper_05);
  al_destroy_bitmap (dv_chopper_fg_05);

  /* palace vga */
  al_destroy_bitmap (pv_chopper_01);
  al_destroy_bitmap (pv_chopper_fg_01);
  al_destroy_bitmap (pv_chopper_02);
  al_destroy_bitmap (pv_chopper_fg_02);
  al_destroy_bitmap (pv_chopper_03);
  al_destroy_bitmap (pv_chopper_fg_03);
  al_destroy_bitmap (pv_chopper_04);
  al_destroy_bitmap (pv_chopper_fg_04);
  al_destroy_bitmap (pv_chopper_05);
  al_destroy_bitmap (pv_chopper_fg_05);

  /* palettable */
  al_destroy_bitmap (chopper_blood_01);
  al_destroy_bitmap (chopper_blood_02);
  al_destroy_bitmap (chopper_blood_03);
  al_destroy_bitmap (chopper_blood_04);
  al_destroy_bitmap (chopper_blood_05);
}

void
load_chopper_samples (void)
{
  chopper_sample = load_sample (CHOPPER_SAMPLE);
}

void
unload_chopper_samples (void)
{
  al_destroy_sample (chopper_sample);
}

void
register_chopper (struct pos *p)
{
  struct chopper c;

  c.p = *p;
  c.i = 0;
  c.wait = CHOPPER_WAIT;
  c.blood = false;

  chopper =
    add_to_array (&c, 1, chopper, &chopper_nmemb, chopper_nmemb, sizeof (c));

  qsort (chopper, chopper_nmemb, sizeof (c), compare_choppers);
}

int
compare_choppers (const void *c0, const void *c1)
{
  return cpos (&((struct chopper *) c0)->p, &((struct chopper *) c1)->p);
}

struct chopper *
chopper_at_pos (struct pos *p)
{
  struct chopper c;
  c.p = *p;
  return bsearch (&c, chopper, chopper_nmemb, sizeof (c), compare_choppers);
}

bool
should_chomp (struct pos *p)
{
  int i;
  struct anim *k;
  struct coord nc;
  struct pos np, pm, _p;

  for (i = 0; i < kid_nmemb; i++) {
    k = &kid[i];
    if (is_kid_dead (&k->f)) continue;
    survey (_m, pos, &k->f, &nc, &pm, &np);
    int inc = p->place < pm.place ? +1 : -1;
    if (p->room == pm.room && p->floor == pm.floor) {
      for (_p = *p; _p.place != pm.place; _p.place += inc)
        if (con (&_p)->fg == WALL) return false;
      return true;
    }
  }

  return false;
}

void
compute_choppers (void)
{
  size_t i, j;

  for (i = 0; i < chopper_nmemb; i++) {
    struct chopper *c = &chopper[i];
    if (c->p.room == -1) {
      /* remove_chopper (d); i--; */
      continue;
    }
    switch (c->i) {
    case 0:
      if (c->wait-- <= 0 && should_chomp (&c->p)
          && (draw_cycle % CHOPPER_WAIT) ==
          prandom_pos (&c->p, 0, 1, CHOPPER_WAIT - 1)) c->i++; break;
    case 1: c->i++; sample_chopper = true; break;
    case 2: c->i++; break;
    case 3: c->i++; break;
    case 4: c->i = 0; c->wait = CHOPPER_WAIT; break;
    }

    if (c->i != 1 && c->i != 2 ) continue;

    /* chomp kid */
    for (j = 0; j < kid_nmemb; j++) {
      struct anim *k = &kid[j];
      if (is_kid_dead (&k->f)
          || is_kid_fall (&k->f)
          || k->chopper_immune) continue;
      struct coord nc; struct pos np, ptf, ptb;
      survey (_tf, pos, &k->f, &nc, &ptf, &np);
      survey (_tb, pos, &k->f, &nc, &ptb, &np);
      pos2room (&ptf, c->p.room, &ptf);
      pos2room (&ptb, c->p.room, &ptb);
      if (((ptf.room == c->p.room
            && ptf.floor == c->p.floor)
           || (ptb.room == c->p.room
               && ptb.floor == c->p.floor))
          && ((k->f.dir == LEFT && ptf.place < c->p.place
               && ptb.place >= c->p.place)
              || (k->f.dir == RIGHT && ptf.place >= c->p.place
                  && ptb.place < c->p.place))) {
        c->blood = true;
        k->splash = true;
        k->current_lives = 0;
        k->p = c->p;
        video_effect.color = RED;
        start_video_effect (VIDEO_FLICKERING, SECS_TO_VCYCLES (0.1));
        sample_chopped = true;
        kid_die_chopped (k);
      }
    }
  }
}

void
sample_choppers (void)
{
  if (sample_chopper) play_sample (chopper_sample);
  sample_chopper = false;
}

void
draw_chopper_left (ALLEGRO_BITMAP *bitmap, struct pos *p,
                   enum em em, enum vm vm)
{
  struct chopper *ch = chopper_at_pos (p);
  if (! ch) return;

  switch (ch->i) {
  case 0: draw_chopper_left_01 (bitmap, p, ch, em, vm); break;
  case 1: draw_chopper_left_02 (bitmap, p, ch, em, vm); break;
  case 2: draw_chopper_left_03 (bitmap, p, ch, em, vm); break;
  case 3: draw_chopper_left_04 (bitmap, p, ch, em, vm); break;
  case 4: draw_chopper_left_05 (bitmap, p, ch, em, vm); break;
  }
}

void
draw_chopper_left_01 (ALLEGRO_BITMAP *bitmap, struct pos *p, struct chopper *ch,
                      enum em em, enum vm vm)
{
  ALLEGRO_BITMAP *chopper_01 = NULL;

  switch (em) {
  case DUNGEON:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_01 = dv_chopper_01; break;
    }
    break;
  case PALACE:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_01 = pv_chopper_01; break;
    }
    break;
  }

  struct coord c;
  draw_bitmapc (chopper_01, bitmap, chopper_coord (p, &c), 0);
  if (ch->blood)
    draw_bitmapc (chopper_blood_01, bitmap, chopper_blood_01_coord (p, &c), 0);
}

void
draw_chopper_left_02 (ALLEGRO_BITMAP *bitmap, struct pos *p, struct chopper *ch,
                      enum em em, enum vm vm)
{
  ALLEGRO_BITMAP *chopper_02 = NULL;

  switch (em) {
  case DUNGEON:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_02 = dv_chopper_02; break;
    }
    break;
  case PALACE:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_02 = pv_chopper_02; break;
    }
    break;
  }

  struct coord c;
  draw_bitmapc (chopper_02, bitmap, chopper_coord (p, &c), 0);
  if (ch->blood)
    draw_bitmapc (chopper_blood_02, bitmap, chopper_blood_02_coord (p, &c), 0);
}

void
draw_chopper_left_03 (ALLEGRO_BITMAP *bitmap, struct pos *p, struct chopper *ch,
                      enum em em, enum vm vm)
{
  ALLEGRO_BITMAP *chopper_03 = NULL;

  switch (em) {
  case DUNGEON:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_03 = dv_chopper_03; break;
    }
    break;
  case PALACE:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_03 = pv_chopper_03; break;
    }
    break;
  }

  struct coord c;
  draw_bitmapc (chopper_03, bitmap, chopper_coord (p, &c), 0);
  if (ch->blood)
    draw_bitmapc (chopper_blood_03, bitmap, chopper_blood_03_coord (p, &c), 0);
}

void
draw_chopper_left_04 (ALLEGRO_BITMAP *bitmap, struct pos *p, struct chopper *ch,
                      enum em em, enum vm vm)
{
  ALLEGRO_BITMAP *chopper_04 = NULL;

  switch (em) {
  case DUNGEON:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_04 = dv_chopper_04; break;
    }
    break;
  case PALACE:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_04 = pv_chopper_04; break;
    }
    break;
  }

  struct coord c;
  draw_bitmapc (chopper_04, bitmap, chopper_coord (p, &c), 0);
  if (ch->blood)
    draw_bitmapc (chopper_blood_04, bitmap, chopper_blood_04_coord (p, &c), 0);
}

void
draw_chopper_left_05 (ALLEGRO_BITMAP *bitmap, struct pos *p, struct chopper *ch,
                      enum em em, enum vm vm)
{
  ALLEGRO_BITMAP *chopper_05 = NULL;

  switch (em) {
  case DUNGEON:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_05 = dv_chopper_05; break;
    }
    break;
  case PALACE:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_05 = pv_chopper_05; break;
    }
    break;
  }

  struct coord c;
  draw_bitmapc (chopper_05, bitmap, chopper_coord (p, &c), 0);
  if (ch->blood)
    draw_bitmapc (chopper_blood_05, bitmap, chopper_blood_05_coord (p, &c), 0);
}

void
draw_chopper_fg (ALLEGRO_BITMAP *bitmap, struct pos *p,
                 enum em em, enum vm vm)
{
  struct chopper *ch = chopper_at_pos (p);
  if (! ch) return;

  switch (ch->i) {
  case 0: draw_chopper_fg_01 (bitmap, p, ch, em, vm); break;
  case 1: draw_chopper_fg_02 (bitmap, p, ch, em, vm); break;
  case 2: draw_chopper_fg_03 (bitmap, p, ch, em, vm); break;
  case 3: draw_chopper_fg_04 (bitmap, p, ch, em, vm); break;
  case 4: draw_chopper_fg_05 (bitmap, p, ch, em, vm); break;
  }
}

void
draw_chopper_fg_01 (ALLEGRO_BITMAP *bitmap, struct pos *p, struct chopper *ch,
                    enum em em, enum vm vm)
{
  ALLEGRO_BITMAP *chopper_fg_01 = NULL;

  switch (em) {
  case DUNGEON:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_fg_01 = dv_chopper_fg_01; break;
    }
    break;
  case PALACE:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_fg_01 = pv_chopper_fg_01; break;
    }
    break;
  }

  struct coord c;
  draw_bitmapc (chopper_fg_01, bitmap, chopper_coord (p, &c), 0);
  if (ch->blood) {
    int h = al_get_bitmap_height (chopper_blood_01);
    draw_bitmap_regionc (chopper_blood_01, bitmap, 0, 0, 2, h, chopper_blood_01_coord (p, &c), 0);
  }
}

void
draw_chopper_fg_02 (ALLEGRO_BITMAP *bitmap, struct pos *p, struct chopper *ch,
                    enum em em, enum vm vm)
{
  ALLEGRO_BITMAP *chopper_fg_02 = NULL;

  switch (em) {
  case DUNGEON:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_fg_02 = dv_chopper_fg_02; break;
    }
    break;
  case PALACE:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_fg_02 = pv_chopper_fg_02; break;
    }
    break;
  }

  struct coord c;
  draw_bitmapc (chopper_fg_02, bitmap, chopper_coord (p, &c), 0);
  if (ch->blood) {
    int h = al_get_bitmap_height (chopper_blood_02);
    draw_bitmap_regionc (chopper_blood_02, bitmap, 0, 0, 2, h, chopper_blood_02_coord (p, &c), 0);
  }
}

void
draw_chopper_fg_03 (ALLEGRO_BITMAP *bitmap, struct pos *p, struct chopper *ch,
                    enum em em, enum vm vm)
{
  ALLEGRO_BITMAP *chopper_fg_03 = NULL;

  switch (em) {
  case DUNGEON:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_fg_03 = dv_chopper_fg_03; break;
    }
    break;
  case PALACE:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_fg_03 = pv_chopper_fg_03; break;
    }
    break;
  }

  struct coord c;
  draw_bitmapc (chopper_fg_03, bitmap, chopper_coord (p, &c), 0);
  if (ch->blood) {
    int h = al_get_bitmap_height (chopper_blood_03);
    draw_bitmap_regionc (chopper_blood_03, bitmap, 0, 0, 2, h, chopper_blood_03_coord (p, &c), 0);
  }
}

void
draw_chopper_fg_04 (ALLEGRO_BITMAP *bitmap, struct pos *p, struct chopper *ch,
                    enum em em, enum vm vm)
{
  ALLEGRO_BITMAP *chopper_fg_04 = NULL;

  switch (em) {
  case DUNGEON:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_fg_04 = dv_chopper_fg_04; break;
    }
    break;
  case PALACE:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_fg_04 = pv_chopper_fg_04; break;
    }
    break;
  }

  struct coord c;
  draw_bitmapc (chopper_fg_04, bitmap, chopper_coord (p, &c), 0);
  if (ch->blood) {
    int h = al_get_bitmap_height (chopper_blood_04);
    draw_bitmap_regionc (chopper_blood_04, bitmap, 0, 0, 2, h, chopper_blood_04_coord (p, &c), 0);
  }
}

void
draw_chopper_fg_05 (ALLEGRO_BITMAP *bitmap, struct pos *p, struct chopper *ch,
                    enum em em, enum vm vm)
{
  ALLEGRO_BITMAP *chopper_fg_05 = NULL;

  switch (em) {
  case DUNGEON:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_fg_05 = dv_chopper_fg_05; break;
    }
    break;
  case PALACE:
    switch (vm) {
    case CGA: break;
    case EGA: break;
    case VGA: chopper_fg_05 = pv_chopper_fg_05; break;
    }
    break;
  }

  struct coord c;
  draw_bitmapc (chopper_fg_05, bitmap, chopper_coord (p, &c), 0);
  if (ch->blood) {
    int h = al_get_bitmap_height (chopper_blood_05);
    draw_bitmap_regionc (chopper_blood_05, bitmap, 0, 0, 2, h, chopper_blood_05_coord (p, &c), 0);
  }
}

struct coord *
chopper_coord (struct pos *p, struct coord *c)
{
  c->x = PLACE_WIDTH * p->place;
  c->y = PLACE_HEIGHT * p->floor + 3;
  c->room = p->room;
  return c;
}

struct coord *
chopper_blood_01_coord (struct pos *p, struct coord *c)
{
  c->x = PLACE_WIDTH * p->place + 12;
  c->y = PLACE_HEIGHT * p->floor + 48;
  c->room = p->room;
  return c;
}

struct coord *
chopper_blood_02_coord (struct pos *p, struct coord *c)
{
  c->x = PLACE_WIDTH * p->place + 12;
  c->y = PLACE_HEIGHT * p->floor + 39;
  c->room = p->room;
  return c;
}

struct coord *
chopper_blood_03_coord (struct pos *p, struct coord *c)
{
  c->x = PLACE_WIDTH * p->place + 12;
  c->y = PLACE_HEIGHT * p->floor + 28;
  c->room = p->room;
  return c;
}

struct coord *
chopper_blood_04_coord (struct pos *p, struct coord *c)
{
  c->x = PLACE_WIDTH * p->place + 12;
  c->y = PLACE_HEIGHT * p->floor + 32;
  c->room = p->room;
  return c;
}

struct coord *
chopper_blood_05_coord (struct pos *p, struct coord *c)
{
  c->x = PLACE_WIDTH * p->place + 12;
  c->y = PLACE_HEIGHT * p->floor + 52;
  c->room = p->room;
  return c;
}

ALLEGRO_COLOR
blood_palette (ALLEGRO_COLOR c)
{
  if (color_eq (c, WHITE)) return BLOOD_COLOR;
  else return c;
}