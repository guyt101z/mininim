/*
  gui-position-control.c -- editor GUI position control module;

  Copyright (C) 2015, 2016, 2017 Bruno Félix Rezende Ribeiro
  <oitofelix@gnu.org>

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

#include "mininim.h"

static int destroy_cb (Ihandle *ih);
static int _update_cb (Ihandle *ih);
static int valuechanged_cb (Ihandle *ih);
static int killfocus_cb (Ihandle *ih);


Ihandle *
gui_create_position_control (struct pos *p, char *norm_group)
{
  Ihandle *ih, *hbox;

  Ihandle *level_spin, *room_spin, *floor_spin, *place_spin;

  ih = IupSetCallbacks
    (IupSetAttributes
     (IupFrame
      (hbox = IupSetAttributes
       (IupHbox
        (level_spin = IupSetCallbacks
         (IupSetAttributes
          (IupText (NULL),
           "SPIN = YES,"
           "SPINWRAP = YES,"
           "MASK = /d+,"
           "ACTIVE = NO,"),
          "VALUECHANGED_CB", valuechanged_cb,
          "KILLFOCUS_CB", killfocus_cb,
          NULL),

         room_spin = IupSetCallbacks
         (IupSetAttributes
          (IupText (NULL),
           "SPIN = YES,"
           "SPINWRAP = YES,"
           "MASK = /d+,"),
          "VALUECHANGED_CB", valuechanged_cb,
          "KILLFOCUS_CB", killfocus_cb,
          NULL),

         floor_spin = IupSetCallbacks
         (IupSetAttributes
          (IupText (NULL),
           "SPIN = YES,"
           "SPINWRAP = YES,"
           "MASK = /d,"),
          "VALUECHANGED_CB", valuechanged_cb,
          "KILLFOCUS_CB", killfocus_cb,
          NULL),

         place_spin = IupSetCallbacks
         (IupSetAttributes
          (IupText (NULL),
           "SPIN = YES,"
           "SPINWRAP = YES,"
           "MASK = /d,"),
          "VALUECHANGED_CB", valuechanged_cb,
          "KILLFOCUS_CB", killfocus_cb,
          NULL),

         NULL),
        "ALIGNMENT = ACENTER")),
      "TITLE = Position"),
     "DESTROY_CB", destroy_cb,
     "_UPDATE_CB", _update_cb,
     NULL);

  IupSetAttribute (ih, "_POS", (void *) p);

  IupSetAttribute (hbox, "NORMALIZERGROUP", norm_group);

  IupSetAttribute (ih, "_LEVEL_SPIN", (void *) level_spin);
  IupSetAttribute (ih, "_ROOM_SPIN", (void *) room_spin);
  IupSetAttribute (ih, "_FLOOR_SPIN", (void *) floor_spin);
  IupSetAttribute (ih, "_PLACE_SPIN", (void *) place_spin);

  IupSetInt (room_spin, "SPINMAX", ROOMS - 1);
  IupSetInt (floor_spin, "SPINMAX", FLOORS - 1);
  IupSetInt (place_spin, "SPINMAX", PLACES - 1);

#if WINDOWS_PORT
  IupSetInt (level_spin, "SIZE", 32);
  IupSetInt (room_spin, "SIZE", 32);
  IupSetInt (floor_spin, "SIZE", 24);
  IupSetInt (place_spin, "SIZE", 24);
#else
  IupSetInt (level_spin, "VISIBLECOLUMNS", 3);
  IupSetInt (room_spin, "VISIBLECOLUMNS", 3);
  IupSetInt (floor_spin, "VISIBLECOLUMNS", 1);
  IupSetInt (place_spin, "VISIBLECOLUMNS", 1);
#endif

  struct pos *last_p = xmalloc (sizeof (*last_p));
  invalid_pos (last_p);
  IupSetAttribute (ih, "_LAST_POS", (void *) last_p);

  return ih;
}

int
destroy_cb (Ihandle *ih)
{
  struct pos *last_p = (void *) IupGetAttribute (ih, "_LAST_POS");
  al_free (last_p);
  return IUP_DEFAULT;
}

int
_update_cb (Ihandle *ih)
{
  if (! IupGetInt (ih, "VISIBLE")) return IUP_DEFAULT;

  struct pos *p = (void *) IupGetAttribute (ih, "_POS");
  gui_control_active (ih, is_valid_pos (p) && selection_locked);

  struct pos *last_p = (void *) IupGetAttribute (ih, "_LAST_POS");
  if (! peq (p, last_p) && p->room > 0) {
    Ihandle *level_spin = (void *) IupGetAttribute (ih, "_LEVEL_SPIN");
    Ihandle *room_spin = (void *) IupGetAttribute (ih, "_ROOM_SPIN");
    Ihandle *floor_spin = (void *) IupGetAttribute (ih, "_FLOOR_SPIN");
    Ihandle *place_spin = (void *) IupGetAttribute (ih, "_PLACE_SPIN");
    struct pos np; npos (p, &np);
    *last_p = np;
    IupSetInt (level_spin, "SPINVALUE", np.l->n);
    IupSetInt (room_spin, "SPINVALUE", np.room);
    IupSetInt (floor_spin, "SPINVALUE", np.floor);
    IupSetInt (place_spin, "SPINVALUE", np.place);
  }

  return IUP_DEFAULT;
}

int
valuechanged_cb (Ihandle *ih)
{
  struct pos *p = (void *) IupGetAttribute (ih, "_POS");

  Ihandle *room_spin = (void *) IupGetAttribute (ih, "_ROOM_SPIN");
  Ihandle *floor_spin = (void *) IupGetAttribute (ih, "_FLOOR_SPIN");
  Ihandle *place_spin = (void *) IupGetAttribute (ih, "_PLACE_SPIN");

  int room = IupGetInt (room_spin, "SPINVALUE");
  int floor = IupGetInt (floor_spin, "SPINVALUE");
  int place = IupGetInt (place_spin, "SPINVALUE");

  if (room > 0) {
    struct pos np; new_pos (&np, p->l, room, floor, place);
    *p = np;
    mr_focus_room (np.room);
  }

  return IUP_DEFAULT;
}

int
killfocus_cb (Ihandle *ih)
{
  struct pos *p = (void *) IupGetAttribute (ih, "_POS");
  struct pos *last_p = (void *) IupGetAttribute (ih, "_LAST_POS");
  Ihandle *room_spin = (void *) IupGetAttribute (ih, "_ROOM_SPIN");
  Ihandle *floor_spin = (void *) IupGetAttribute (ih, "_FLOOR_SPIN");
  Ihandle *place_spin = (void *) IupGetAttribute (ih, "_PLACE_SPIN");
  struct pos np; npos (p, &np);
  *last_p = np;
  IupSetInt (room_spin, "SPINVALUE", np.room);
  IupSetInt (floor_spin, "SPINVALUE", np.floor);
  IupSetInt (place_spin, "SPINVALUE", np.place);
  gui_empty_value_to_0 (room_spin);
  gui_empty_value_to_0 (floor_spin);
  gui_empty_value_to_0 (place_spin);
  return IUP_DEFAULT;
}
