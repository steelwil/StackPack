/*
 *  Copyright (C) 2006 - William Bell 
 *
 *  This file is part of the StackPack game
 *
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
 
#include <gtk/gtk.h>
#include <string.h>
#include "gameData.h"
#include "userData.h"
#include "StackPackEngine.h"

extern struct GameDataStr gameData;
extern struct UserDataStr userData;
extern struct GameData gameEngineData[cMAXPLAYERS];

/* Backing pixmap for drawing area */
GdkPixmap *pixmap = NULL;
GdkRectangle m_PlayRect[cMAXPLAYERS];
GdkRectangle m_ScoreRect[cMAXPLAYERS];
GdkPixmap* backgrounds = NULL;
GdkGC* gc[13][3];
GdkGC* lightGreyGC;
GdkColor lightGrey = {0, 0xc0c0, 0xc0c0, 0xc0c0};
GdkGC* darkGreyGC;
GdkColor darkGrey = {0, 0x3030, 0x3030, 0x3030};

int prevLevel;

GdkColor color[13][3] =
{
	{
		{0, 0xffff, 0x0000, 0x0000}, /* 0 light red */
		{0, 0xc0c0, 0x0000, 0x0000},
		{0, 0x8080, 0x0000, 0x0000}
	},
	{
		{0, 0x0000, 0xffff, 0x0000}, /* 1 light green */
		{0, 0x0000, 0xc0c0, 0x0000},
		{0, 0x0000, 0x8080, 0x0000}
	},
	{
		{0, 0xffff, 0xffff, 0x0000}, /* 2 light yellow */
		{0, 0xc0c0, 0xc0c0, 0x0000},
		{0, 0x8080, 0x8080, 0x0000}
	},
	{
		{0, 0x0000, 0x0000, 0xffff}, /* 3  light blue */
		{0, 0x0000, 0x0000, 0xc0c0},
		{0, 0x0000, 0x0000, 0x8080}
	},
	{
		{0, 0xffff, 0x0000, 0xffff}, /* 4  light magenta */
		{0, 0xc0c0, 0x0000, 0xc0c0},
		{0, 0x8080, 0x0000, 0x8080}
	},
	{
		{0, 0x0000, 0xffff, 0xffff}, /* 5  light cyan */
		{0, 0x0000, 0xc0c0, 0xc0c0},
		{0, 0x0000, 0x8080, 0x8080}
	},
	{
		{0, 0xffff, 0xffff, 0xffff}, /* 6  white */
		{0, 0xc0c0, 0xc0c0, 0xc0c0},
		{0, 0x8080, 0x8080, 0x8080}
	},
	{
		{0, 0x8080, 0x0000, 0x0000}, /* 7  dark red */
		{0, 0x6060, 0x0000, 0x0000},
		{0, 0x4040, 0x0000, 0x0000}
	},
	{
		{0, 0x8080, 0x8080, 0x0000}, /* 8 dark yellow */
		{0, 0x6060, 0x6060, 0x0000},
		{0, 0x4040, 0x4040, 0x0000}
	},
	{
		{0, 0x0000, 0x0000, 0x8080}, /* 9 dark blue */
		{0, 0x0000, 0x0000, 0x6060},
		{0, 0x0000, 0x0000, 0x4040}
	},
	{
		{0, 0x8080, 0x0000, 0x8080}, /* 10 dark magenta */
		{0, 0x6060, 0x0000, 0x6060},
		{0, 0x4040, 0x0000, 0x4040}
	},
	{
		{0, 0x0000, 0x8080, 0x8080}, /* 11 dark cyan */
		{0, 0x0000, 0x6060, 0x6060},
		{0, 0x0000, 0x4040, 0x4040}
	},
	{
		{0, 0x8080, 0x8080, 0x8080}, /* 12 grey */
		{0, 0x6060, 0x6060, 0x6060},
		{0, 0x4040, 0x4040, 0x4040}
	}
};

/* -----------------------------------------------------------------------------
 * Initialize the drawing structures
 */
void viewInitialize(GtkWidget* parent)
{
	g_message("in viewInitialize");
	GdkWindow* window = gtk_widget_get_root_window(parent);
	int j, k;
	for (j=0; j<13; j++)
	{
		for (k=0; k<3; k++)
		{
		  gc[j][k] = gdk_gc_new(window);
		  gdk_gc_set_rgb_fg_color(gc[j][k], &color[j][k]);
			
		}
	}
	lightGreyGC = gdk_gc_new(window);
	gdk_gc_set_rgb_fg_color(lightGreyGC, &lightGrey);
	gdk_gc_set_line_attributes(lightGreyGC,
                               2, /* line_width, */
                               GDK_LINE_SOLID,
                               GDK_CAP_BUTT,
                               GDK_JOIN_MITER);
	darkGreyGC = gdk_gc_new(window);
	gdk_gc_set_rgb_fg_color(darkGreyGC, &darkGrey);
	gdk_gc_set_line_attributes(darkGreyGC,
                               1, /* line_width, */
                               GDK_LINE_SOLID,
                               GDK_CAP_BUTT,
                               GDK_JOIN_MITER);
	backgrounds = gdk_pixmap_create_from_xpm(window,
                                             NULL,
                                             NULL,
                                             "screens.xpm");
}
 
/* -----------------------------------------------------------------------------
 * get the coords of the game based on the size of the drawing widget
 */
void viewGetCoords(GtkWidget* parent)
{
	int p;
	int w = parent->allocation.width;
	int h = parent->allocation.height;
	int blockWidth = (h*0.9-1)/gameData.m_Height;
	int gridWidth = blockWidth*gameData.m_Base + 1;
	int gridInterval = (int)((w-userData.m_NumberOfPlayers*(gridWidth+100))/(userData.m_NumberOfPlayers+1));
	if (gridInterval < 10)
	{
		blockWidth = (w/userData.m_NumberOfPlayers-110)/gameData.m_Base;
		gridWidth = blockWidth*gameData.m_Base + 1;
		gridInterval = (int)((w-userData.m_NumberOfPlayers*(gridWidth+100))/(userData.m_NumberOfPlayers+1));
	}
	for (p=0; p<userData.m_NumberOfPlayers; p++)
	{
		m_PlayRect[p].y = (long)(h*0.05);
		printf("parent->allocation.width=%d %d %d \n", parent->allocation.width,gridInterval*(p+1),(gridWidth+100)*p);
		m_PlayRect[p].width = gridWidth;
		m_PlayRect[p].x = parent->allocation.width - gridInterval*(p+1) - (gridWidth+100)*p - gridWidth;
		m_PlayRect[p].height = blockWidth*gameData.m_Height + 1;

		m_ScoreRect[p].y = m_PlayRect[p].y;
		m_ScoreRect[p].height = 192;
		m_ScoreRect[p].x = m_PlayRect[p].x - 100;
		m_ScoreRect[p].width = 84;
	}
}
/* -----------------------------------------------------------------------------
 * Create a new backing pixmap of the appropriate size 
 */
void viewResize(GtkWidget* widget)
{
	printf("in viewResize\n");
	if (pixmap)
		g_object_unref (pixmap);

	pixmap = gdk_pixmap_new (widget->window,
			   widget->allocation.width,
			   widget->allocation.height,
			   -1);
/* 	gdk_draw_rectangle (pixmap,
		      widget->style->white_gc,
		      TRUE,
		      0, 0,
		      widget->allocation.width,
		      widget->allocation.height);
 */	
	viewGetCoords(widget);
		
	/* draw the tiled background */
	int x, y;
	int w = widget->allocation.width;
	int h = widget->allocation.height;
	int c = (w + 31)/32;
	int r = (h + 31)/32;
	for (x=0; x<c; x++)
	{
		for (y=0; y<r; y++)
		{
			gdk_draw_drawable(pixmap,
                              widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                              backgrounds,
                              32*(gameEngineData[0].m_Level-1),
                              0,
                              32*x,
                              32*y,
                              -1,
                              -1);
		}
	}

 }
 
/*----------------------------------------------------------------------------*/
void viewPaintFlatBlock(GtkWidget* widget, char block, GdkRectangle* rect)
{
	gdk_draw_rectangle(pixmap,
                       gc[(block-1)%13][0],
                       TRUE,
                       rect->x,
                       rect->y,
                       rect->width,
                       rect->height);
}

/*----------------------------------------------------------------------------*/
void viewPaintBlackBlock(GtkWidget* widget, GdkRectangle* rect)
{
	gdk_draw_rectangle(pixmap,
                       widget->style->black_gc,
                       TRUE,
                       rect->x,
                       rect->y,
                       rect->width+1,
                       rect->height+1);
}

/*----------------------------------------------------------------------------*/
void viewPaint3DBlock(GtkWidget* widget, char block, GdkRectangle* rect)
{
	//g_message("block = %d", block);
	GdkPoint pts[6];
	int ind = rect->width / 8;
	pts[0].x = rect->x;
	pts[0].y = rect->y + rect->height;

	pts[1].x = rect->x;
	pts[1].y = rect->y;

	pts[2].x = rect->x + rect->width;
	pts[2].y = rect->y;

	pts[3].x = rect->x + rect->width - ind;
	pts[3].y = rect->y + ind;

	pts[4].x = rect->x + ind;
	pts[4].y = rect->y + ind;

	pts[5].x = rect->x + ind;
	pts[5].y = rect->y + rect->height - ind;
 	gdk_draw_polygon(pixmap,
                     gc[(block-1)%13][0],
                     TRUE,
                     pts,
                     6);
 
	gdk_draw_rectangle(pixmap,
                       gc[(block-1)%13][1],
                       TRUE,
                       rect->x+ind,
                       rect->y+ind,
                       rect->width-ind-ind,
                       rect->height-ind-ind);

	pts[1].x = rect->x + rect->width;
	pts[1].y = rect->y + rect->height;

	pts[4].x = rect->x + rect->width - ind;
	pts[4].y = rect->y + rect->height - ind;

	//g_message("block2 = %d", block);
	gdk_draw_polygon(pixmap,
                     gc[(block-1)%13][2],
                     TRUE,
                     pts,
                     6);
}

/*----------------------------------------------------------------------------*/
void viewPaintPyrimidBlock(GtkWidget* widget, char block, GdkRectangle* rect)
{
	GdkPoint pts[4];

	pts[0].x = rect->x;
	pts[0].y = rect->y;

	pts[1].x = rect->x;
	pts[1].y = rect->y+rect->height;

	pts[2].x = rect->x + rect->width;
	pts[2].y = rect->y;

	pts[3].x = rect->x + rect->width;
	pts[3].y = rect->y + rect->height;
	gdk_draw_polygon(pixmap,
                     gc[(block-1)%13][1],
                     TRUE,
                     pts,
                     4);

	int mid = rect->width / 2;

	pts[1].x = rect->x + mid;
	pts[1].y = rect->y + mid;

	pts[2].x = rect->x + rect->width-1;
	gdk_draw_polygon(pixmap,
                     gc[(block-1)%13][0],
                     TRUE,
                     pts,
                     3);

	pts[0].x = rect->x+1;
	pts[0].y = rect->y + rect->height;

	pts[1].x = rect->x + rect->width;
	pts[1].y = rect->y + rect->height;

	pts[2].x = rect->x + rect->width - mid;
	pts[2].y = rect->y + rect->height - mid;

	gdk_draw_polygon(pixmap,
                     gc[(block-1)%13][2],
                     TRUE,
                     pts,
                     3);
}

/*---------------------------------------------------------------------------*/
 void viewPaintGrid(GtkWidget* widget, int p)
{
	//g_message("in viewPaintGrid");
	int r, c;
	GdkRectangle rect;
	int h = m_PlayRect[p].height;
	int bs = (h-1)/gameData.m_Height;

	char grid [32][32];
	GameGetGrid(&gameEngineData[p], grid);

	for (r=0; r<gameData.m_Height; r++)
	{
		for (c=0; c<gameData.m_Base; c++)
		{
			rect.x = m_PlayRect[p].x+ c*bs + 1;
			rect.y = m_PlayRect[p].y+ r*bs + 1;
			rect.width = bs-2;
			rect.height = bs-2;
			if (grid[r][c])
			{
				switch (userData.m_PieceStyle)
				{
				case 0:
					viewPaintFlatBlock(widget, grid[r][c], &rect);
					break;
				case 1:
					viewPaint3DBlock(widget, grid[r][c], &rect);
					break;
				case 2:
					viewPaintPyrimidBlock(widget, grid[r][c], &rect);
					break;
				}
			}
			else
				viewPaintBlackBlock(widget, &rect);
		}
	}
}

/*----------------------------------------------------------------------------*/
void viewPaintScore(GtkWidget* widget, int p)
{
	/* display the score */
	//g_message("in viewPaintScore");
	int r, c;
	char out[16];
	out[0] = 0;
	int middle = m_ScoreRect[p].x+m_ScoreRect[p].width/2;
	int xpos = 0;

	strcpy(out, "Score");
	PangoLayout* pl = gtk_widget_create_pango_layout(widget, out);
	int width =  pango_layout_get_width(pl);
	//printf("width=%d\n", width);
	xpos = middle -  width/2;
	gdk_draw_layout(pixmap,
					lightGreyGC,
					xpos,
					m_ScoreRect[p].y+5,
					pl);
	g_object_unref(pl);

	if (gameEngineData[p].m_Score == 0)
	{
		strcpy(out, "         ");
		pl = gtk_widget_create_pango_layout(widget, out);
		width =  pango_layout_get_width(pl);
		xpos = middle - width/2;
		gdk_draw_layout(pixmap,
						lightGreyGC,
						xpos,
						m_ScoreRect[p].y+25,
						pl);
		g_object_unref(pl);
	}
	sprintf(out, " %d ", (int)gameEngineData[p].m_Score);
	pl = gtk_widget_create_pango_layout(widget, out);
	width =  pango_layout_get_width(pl);
	xpos = middle - width/2;
	gdk_draw_layout(pixmap,
					lightGreyGC,
					xpos,
					m_ScoreRect[p].y+25,
					pl);
	g_object_unref(pl);
	
	strcpy(out, "Level");
	pl = gtk_widget_create_pango_layout(widget, out);
	width =  pango_layout_get_width(pl);
	xpos = middle - width/2;
	gdk_draw_layout(pixmap,
					lightGreyGC,
					xpos,
					m_ScoreRect[p].y+45,
					pl);
	g_object_unref(pl);

	sprintf(out, " %d ", gameEngineData[p].m_Level);
	pl = gtk_widget_create_pango_layout(widget, out);
	width =  pango_layout_get_width(pl);
	xpos = middle - width/2;
	gdk_draw_layout(pixmap,
					lightGreyGC,
					xpos,
					m_ScoreRect[p].y+65,
					pl);
	g_object_unref(pl);

	strcpy(out, "Lines");
	pl = gtk_widget_create_pango_layout(widget, out);
	width =  pango_layout_get_width(pl);
	xpos = middle - width/2;
	gdk_draw_layout(pixmap,
					lightGreyGC,
					xpos,
					m_ScoreRect[p].y+85,
					pl);
	g_object_unref(pl);

	sprintf(out, " %d ", gameEngineData[p].m_Lines);
	pl = gtk_widget_create_pango_layout(widget, out);
	width =  pango_layout_get_width(pl);
	xpos = middle - width/2;
	gdk_draw_layout(pixmap,
					lightGreyGC,
					xpos,
					m_ScoreRect[p].y+105,
					pl);
	g_object_unref(pl);

	/* display the preview */
	GdkRectangle rect;
	int icolor;
	int bs = 10;
	if (GameGetPreview(&gameEngineData[p], &icolor))
	{
		rect.x = m_ScoreRect[p].x+16;
		rect.y = m_ScoreRect[p].y+130;
		rect.width = 1 + (bs+2)*4;
		rect.height = 1 + (bs+2)*4;
		viewPaintBlackBlock(widget, &rect);

		unsigned long pv = GameGetPreview(&gameEngineData[p], &icolor);
		icolor++;
		unsigned long mask;
		for (r=0; r<5; r++)
		{
			for (c=0; c<5; c++)
			{
				rect.x = m_ScoreRect[p].x+16+c*bs+1;
				rect.y = m_ScoreRect[p].y+130+r*bs+1;
				rect.width = bs-2;
				rect.height = bs-2;
				mask = 0x80000000>>(r*5+c);
				if (pv&mask)
				{
					switch (userData.m_PieceStyle)
					{
					case 0:
						viewPaintFlatBlock(widget, (char)icolor, &rect);
						break;
					case 1:
						viewPaint3DBlock(widget, (char)icolor, &rect);
						break;
					case 2:
						viewPaintPyrimidBlock(widget, (char)icolor, &rect);
						break;
					}
				}
				else
					viewPaintBlackBlock(widget, &rect);
			}
		}
	}
}

/*----------------------------------------------------------------------------*/
void viewRedraw(GtkWidget* widget, int area, GdkEventExpose *event)
{
	//g_message("in viewRedraw");
	if (gameEngineData[0].m_Level != prevLevel)
	{
		viewResize(widget);
		prevLevel = gameEngineData[0].m_Level;
	}
	int p, r, c;
	for (p=0; p<userData.m_NumberOfPlayers; p++)
	{

		gdk_draw_rectangle(pixmap,
                           lightGreyGC,
                           FALSE,
                           m_ScoreRect[p].x,
                           m_ScoreRect[p].y,
                           m_ScoreRect[p].width,
                           m_ScoreRect[p].height);
		gdk_draw_rectangle(pixmap,
                           widget->style->black_gc,
                           TRUE,
                           m_ScoreRect[p].x+1,
                           m_ScoreRect[p].y+1,
                           m_ScoreRect[p].width-2,
                           m_ScoreRect[p].height-2);
		
		viewPaintScore(widget, p);
		
		gdk_draw_rectangle(pixmap,
                           lightGreyGC,
                           FALSE,
                           m_PlayRect[p].x,
                           m_PlayRect[p].y,
                           m_PlayRect[p].width,
                           m_PlayRect[p].height);
		gdk_draw_rectangle(pixmap,
                           widget->style->black_gc,
                           TRUE,
                           m_PlayRect[p].x+1,
                           m_PlayRect[p].y+1,
                           m_PlayRect[p].width-2,
                           m_PlayRect[p].height-2);

		if ((userData.m_GridStyle&1) == 1) /* vertical lines */
		{
			for (c=0; c<gameData.m_Base+1; c++)
			{
				int h = m_PlayRect[p].height;
				int bs = (h-1)/gameData.m_Height;
				gdk_draw_line(pixmap, darkGreyGC, 
				              m_PlayRect[p].x+c*bs,
                              m_PlayRect[p].y,
				              m_PlayRect[p].x+c*bs,
				              m_PlayRect[p].y + m_PlayRect[p].height);
			}
		}
		if ((userData.m_GridStyle&2) == 2) /* horizontal lines */
		{
			for (r=0; r<gameData.m_Height+1; r++)
			{
				int h = m_PlayRect[p].height;
				int bs = (h-1)/gameData.m_Height;
				gdk_draw_line(pixmap, darkGreyGC, 
				              m_PlayRect[p].x,
                              m_PlayRect[p].y+r*bs,
				              m_PlayRect[p].x+m_PlayRect[p].width,
				              m_PlayRect[p].y+r*bs);
			}
		}
		viewPaintGrid(widget, p);
	}

	if (area == 1) /* score area and playing grid */
	{
		event->area.x = m_ScoreRect[0].x;
		event->area.y = m_ScoreRect[0].y;
		event->area.width = m_PlayRect[0].x + m_PlayRect[0].width - m_ScoreRect[0].x;
		event->area.height = m_PlayRect[0].y + m_PlayRect[0].height;
	}
	else if (area == 2) /* whole background */
	{
		event->area.x = 0;
		event->area.y = 0;
		event->area.width = widget->allocation.width;
		event->area.height = widget->allocation.height;
	}
	gdk_draw_drawable (widget->window,
		     widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		     pixmap,
		     event->area.x, event->area.y,
		     event->area.x, event->area.y,
		     event->area.width, event->area.height);

}

/*----------------------------------------------------------------------------*/
