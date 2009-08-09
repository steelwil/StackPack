/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) William Bell 2001-2009 <william.bell@frog.za.net>
 * 
 * This file is part of the StackPack game
 *
 * StackPack is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * StackPack is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

void viewInitialize(GtkWidget* parent);
void viewResize(GtkWidget* parent);
void viewRedraw(GtkWidget* widget, int area, GdkEventExpose *event); /* area 1 = score area and playing grid, 2 =whole background */
void viewGetCoords(GtkWidget* parent);
void viewPaintGrid(GtkWidget* widget, int p);
void viewPaintFlatBlock(GtkWidget* widget, char block, GdkRectangle* rect);
void viewPaint3DBlock(GtkWidget* widget, char block, GdkRectangle* rect);
void viewPaintPyrimidBlock(GtkWidget* widget, char block, GdkRectangle* rect);
void viewPaintBlackBlock(GtkWidget* widget, GdkRectangle* rect);
void viewPaintScore(GtkWidget* widget, int p);
