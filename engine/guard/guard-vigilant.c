/*
  guard-vigilant.c -- guard vigilant module;

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

#include "prince.h"
#include "kernel/video.h"
#include "kernel/keyboard.h"
#include "engine/anim.h"
#include "engine/physics.h"
#include "engine/door.h"
#include "engine/potion.h"
#include "engine/sword.h"
#include "engine/loose-floor.h"
#include "guard.h"

struct frameset guard_vigilant_frameset[GUARD_VIGILANT_FRAMESET_NMEMB];
struct frameset fat_guard_vigilant_frameset[GUARD_VIGILANT_FRAMESET_NMEMB];
struct frameset vizier_vigilant_frameset[GUARD_VIGILANT_FRAMESET_NMEMB];

static void init_guard_vigilant_frameset (void);
static void init_fat_guard_vigilant_frameset (void);
static void init_vizier_vigilant_frameset (void);
static bool flow (struct anim *g);
static bool physics_in (struct anim *g);
static void physics_out (struct anim *g);

/* guard */
ALLEGRO_BITMAP *guard_vigilant_01, *guard_vigilant_02,
  *guard_vigilant_03;

/* fat guard */
ALLEGRO_BITMAP *fat_guard_vigilant_01, *fat_guard_vigilant_02,
  *fat_guard_vigilant_03;

/* vizier */
ALLEGRO_BITMAP *vizier_vigilant_01, *vizier_vigilant_02,
  *vizier_vigilant_03;

static void
init_guard_vigilant_frameset (void)
{
  struct frameset frameset[GUARD_VIGILANT_FRAMESET_NMEMB] =
    {{guard_vigilant_01,+0,0},{guard_vigilant_02,+0,0},
     {guard_vigilant_03,+0,0}};

  memcpy (&guard_vigilant_frameset, &frameset,
          GUARD_VIGILANT_FRAMESET_NMEMB * sizeof (struct frameset));
}

static void
init_fat_guard_vigilant_frameset (void)
{
  struct frameset frameset[GUARD_VIGILANT_FRAMESET_NMEMB] =
    {{fat_guard_vigilant_01,+0,0},{fat_guard_vigilant_02,+0,0},
     {fat_guard_vigilant_03,+0,0}};

  memcpy (&fat_guard_vigilant_frameset, &frameset,
          GUARD_VIGILANT_FRAMESET_NMEMB * sizeof (struct frameset));
}

static void
init_vizier_vigilant_frameset (void)
{
  struct frameset frameset[GUARD_VIGILANT_FRAMESET_NMEMB] =
    {{vizier_vigilant_01,+0,0},{vizier_vigilant_02,+0,0},
     {vizier_vigilant_03,+0,0}};

  memcpy (&vizier_vigilant_frameset, &frameset,
          GUARD_VIGILANT_FRAMESET_NMEMB * sizeof (struct frameset));
}

struct frameset *
get_guard_vigilant_frameset (enum anim_type t)
{
  switch (t) {
  case GUARD: default: return guard_vigilant_frameset;
  case FAT_GUARD: return fat_guard_vigilant_frameset;
  case VIZIER: return vizier_vigilant_frameset;
  }
}

void
load_guard_vigilant (void)
{
  /* guard */
  guard_vigilant_01 = load_bitmap (GUARD_VIGILANT_01);
  guard_vigilant_02 = load_bitmap (GUARD_VIGILANT_02);
  guard_vigilant_03 = load_bitmap (GUARD_VIGILANT_03);

  /* fat_guard */
  fat_guard_vigilant_01 = load_bitmap (FAT_GUARD_VIGILANT_01);
  fat_guard_vigilant_02 = load_bitmap (FAT_GUARD_VIGILANT_02);
  fat_guard_vigilant_03 = load_bitmap (FAT_GUARD_VIGILANT_03);

  /* vizier */
  vizier_vigilant_01 = load_bitmap (VIZIER_VIGILANT_01);
  vizier_vigilant_02 = load_bitmap (VIZIER_VIGILANT_02);
  vizier_vigilant_03 = load_bitmap (VIZIER_VIGILANT_03);

  /* frameset */
  init_guard_vigilant_frameset ();
  init_fat_guard_vigilant_frameset ();
  init_vizier_vigilant_frameset ();
}

void
unload_guard_vigilant (void)
{
  /* guard */
  al_destroy_bitmap (guard_vigilant_01);
  al_destroy_bitmap (guard_vigilant_02);
  al_destroy_bitmap (guard_vigilant_03);

  /* fat_guard */
  al_destroy_bitmap (fat_guard_vigilant_01);
  al_destroy_bitmap (fat_guard_vigilant_02);
  al_destroy_bitmap (fat_guard_vigilant_03);

  /* vizier */
  al_destroy_bitmap (vizier_vigilant_01);
  al_destroy_bitmap (vizier_vigilant_02);
  al_destroy_bitmap (vizier_vigilant_03);
}

void
guard_vigilant (struct anim *g)
{
  g->oaction = g->action;
  g->action = guard_vigilant;
  g->f.flip = (g->f.dir == RIGHT) ?  ALLEGRO_FLIP_HORIZONTAL : 0;

  if (! flow (g)) return;
  if (! physics_in (g)) return;
  next_frame (&g->f, &g->f, &g->fo);
  physics_out (g);
}

static bool
flow (struct anim *g)
{
  struct coord nc;
  struct pos np, pmt;

  bool normal = g->key.down;
  bool walkf = ((g->f.dir == RIGHT) && g->key.right)
    || ((g->f.dir == LEFT) && g->key.left);
  bool walkb = ((g->f.dir == RIGHT) && g->key.left)
    || ((g->f.dir == LEFT) && g->key.right);
  bool defense = g->key.up && ! g->key.shift
    && ! g->key.left && ! g->key.right;
  bool attack = g->key.shift && ! g->key.up
    && ! g->key.left && ! g->key.right;

  if (g->oaction != guard_vigilant) g->i = -1;

  if (g->oaction == guard_vigilant) {
    /* death */
    if (g->current_lives <= 0) {
      survey (_mt, pos, &g->f, &nc, &pmt, &np);
      g->p = pmt;
      guard_die (g);
      return false;
    }

    /* normal */
    if (g->i == 2 && normal) {
      guard_normal (g);
      return false;
    }

    /* walkf */
    if (walkf) {
      guard_walkf (g);
      return false;
    }

    /* walkb */
    if (walkb) {
      guard_walkb (g);
      return false;
    }

    /* attack */
    if (attack) {
      guard_attack (g);
      return false;
    }

    /* defense */
    if (defense) {
      guard_defense (g);
      return false;
    }
  }

  struct frameset *frameset = get_guard_vigilant_frameset (g->type);
  select_frame (g, frameset, g->i == 2 ? g->i : g->i + 1);
  select_xframe (&g->xf, sword_frameset, 4);

  if (g->oaction == guard_normal) g->fo.dx += -1;
  if (g->oaction == guard_walkf) g->fo.dx += +2;

  return true;
}

static bool
physics_in (struct anim *g)
{
  struct coord tf; struct pos np, ptf;

  /* collision */
  if (is_colliding (&g->f, &g->fo, +PLACE_WIDTH, false, &g->ci)
      && g->i == 0) {
    guard_normal (g);
    return false;
  }

  /* fall */
  survey (_tf, pos, &g->f, &tf, &ptf, &np);
  if (is_strictly_traversable (&ptf)) {
    guard_fall (g);
    return false;
  }

  return true;
}

static void
physics_out (struct anim *g)
{
  /* depressible floors */
  if (g->i == 0) update_depressible_floor (g, -1, -27);
  else keep_depressible_floor (g);
}
