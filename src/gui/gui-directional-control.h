/*
  gui-directional-control.c -- GUI directional control module;

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

#ifndef MININIM_GUI_DIRECTIONAL_CONTROL_H
#define MININIM_GUI_DIRECTIONAL_CONTROL_H

Ihandle *gui_create_directional_control (char *title, char *c_image_hname,
                                         Icallback action_cb, char *norm_group);

#endif	/* MININIM_GUI_DIRECTIONAL_CONTROL */
