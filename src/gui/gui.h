/*
  gui.h -- GUI module;

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

#ifndef MININIM_GUI_H
#define MININIM_GUI_H

/* functions */
void init_gui (int argc, char **argv);
void finalize_gui (void);

bool load_led (const char *filename);

Ihandle *bitmap_to_iup_image (ALLEGRO_BITMAP *b, palette p);
void gui_set_image (Ihandle *ih, ALLEGRO_BITMAP *b, palette p);
ALLEGRO_COLOR transp_to_black (ALLEGRO_COLOR c);
ALLEGRO_COLOR white_to_transp (ALLEGRO_COLOR c);
void gui_set_stock_image (Ihandle *ih, char *new_image_hname);
void gui_set_tip (Ihandle *ih, char *new_tip);

int hide_dialog (Ihandle *ih);

bool gui_control_active (Ihandle *ih, bool a);
bool gui_control_attribute (Ihandle *ih, char *name, char *value);
bool gui_control_strf_attribute (Ihandle *ih, char *name, char *format, ...)
  __attribute__ ((format (printf, 3, 4)));

int gui_default_key_cb (Ihandle *ih, int c);
int gui_debug_print_key_cb (Ihandle *ih, int c);
int gui_empty_value_to_0 (Ihandle *ih);

int gui_run_callback_IFn (const char *name, Ihandle *ih);
int gui_run_callback_IFni (const char *name, Ihandle *ih, int i);

void dialog_fit_natural_size (Ihandle *ih);

int gui_destroy_image_cb (Ihandle *ih);

#endif	/* MININIM_GUI_H */
