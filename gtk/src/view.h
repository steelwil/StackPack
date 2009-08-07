/*
 *  Copyright (C) 2006 - William Bell 
 *
 *  This file is part of the StackPack game
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
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
