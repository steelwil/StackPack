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
 
 /* This handles the highest scores dialog */
#include <gtk/gtk.h>
#include <string.h>
#include "gameData.h"

extern struct GameDataStr gameData;

static GtkWidget *window = NULL;
static GtkWidget *labelGridSize;
static GtkWidget *labelBlockSize;
static GtkWidget *labelBlockComplexity;
static GtkWidget *labelLevelLimit;

/*----------------------------------------------------------------------------*/
enum
{
	RANK_COLUMN,
	SCORE_COLUMN,
	ROWS_COLUMN,
	LEVEL_COLUMN,
	DATE_COLUMN,
	NAME_COLUMN,
	QUOTE_COLUMN,
	N_COLUMNS
};

/*----------------------------------------------------------------------------*/
static void close_dialog(GtkWidget *widget, gpointer data)
{
    g_print ("in close_dialog\n");
	gtk_widget_hide (GTK_WIDGET(data));
}

/*----------------------------------------------------------------------------*/
static gboolean delete_event(GtkWidget *widget, GdkEvent  *event, gpointer data)
{
	printf("propertiesDlg in destroy %s\n", "hello");
	close_dialog(NULL, data);
	return TRUE;
}

/*----------------------------------------------------------------------------*/
static void Refresh()
{
	printf("in Refresh()\n");
	char buf[32];
	
	gtk_window_set_title (GTK_WINDOW (window), gameData.m_Comment);
	
	/* Grid size */
	sprintf(buf, "Base %d, Height %d", gameData.m_Base, gameData.m_Height);
	gtk_label_set_text(GTK_LABEL(labelGridSize), buf);
	
	/* Block size */
	buf[0] = 0;
	int blkType = gameData.m_BlkType;
	int blkSize = 0;
	if ((blkType&0x3))
	{
		blkSize |= 0x3;
		strcpy(buf, "Small, ");
	}
	if ((blkType&0x1fc))
	{
		strcat(buf, "Medium, ");
		blkSize |= 0x1fc;
	}
	if ((blkType&0x7fffe00))
	{
		strcat(buf, "Large");
		blkSize |= 0x7fffe00;
	}
	int len = strlen(buf);
	if (buf[len-2] == ',') buf[len-2] = 0;
	gtk_label_set_text(GTK_LABEL(labelBlockSize), buf);
	
	/* Block Complexity */
		buf[0] = 0;
	if ((blkType|(0x4000025&blkSize)) == blkType)
		strcpy(buf, "Lowest, ");
	if ((blkType|(0x80781a&blkSize)) == blkType)
		strcat(buf, "Low, ");
	if ((blkType|(0x3180080&blkSize)) == blkType)
		strcat(buf, "Medium, ");
	if ((blkType|(0x678740&blkSize)) == blkType)
		strcat(buf, "High");
	len = strlen(buf);
	if (buf[len-2] == ',') buf[len-2] = 0;
	gtk_label_set_text(GTK_LABEL(labelBlockComplexity), buf);
	
	/* Level Limit */
	if (gameData.m_LastLevel == 10)
	{
		strcpy(buf, "No Limitation");
	}
	else
	{
		sprintf(buf, "%d", gameData.m_LastLevel);
	}
	gtk_label_set_text(GTK_LABEL(labelLevelLimit), buf);
}

/*----------------------------------------------------------------------------*/
void PropertiesDialogCreate(GtkWidget *parent)
{
	if (window != NULL)
	{
		printf("window already created\n");
		Refresh();
		gtk_widget_show (window);
		return;
	}
	/* GtkWidget is the storage type for widgets */
	GtkWidget *label;
	GtkWidget *button;
    GtkWidget *table;
	PangoFontDescription* font;
	
	font = pango_font_description_from_string ("Sans Bold");

	/* Create a new window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for(GTK_WINDOW (window), GTK_WINDOW (parent));
	gtk_window_set_title (GTK_WINDOW (window), "Properties");
	gtk_window_set_modal(GTK_WINDOW (window), TRUE);
	gtk_window_set_type_hint(GTK_WINDOW (window), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_position(GTK_WINDOW (window), GTK_WIN_POS_CENTER_ON_PARENT);
	
 	gtk_container_set_border_width (GTK_CONTAINER (window), 6);
	table = gtk_table_new (5, 2, FALSE);
	gtk_container_add (GTK_CONTAINER (window), table);

   /* Grid size */
    label = gtk_label_new ("Grid Size:  ");
	gtk_widget_modify_font (label, font);
	gtk_misc_set_alignment (GTK_MISC (label), 1, 0.5);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 6, 6);
    gtk_widget_show (label);

    labelGridSize = gtk_label_new ("null");
	gtk_table_attach_defaults (GTK_TABLE (table), labelGridSize, 1, 2, 0, 1);
    gtk_widget_show (labelGridSize);

    /* Block size */
    label = gtk_label_new ("Block Size:  ");
	gtk_widget_modify_font (label, font);
	gtk_misc_set_alignment (GTK_MISC (label), 1, 0.5);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 6, 6);
    gtk_widget_show (label);

    labelBlockSize = gtk_label_new ("null");
	gtk_table_attach_defaults (GTK_TABLE (table), labelBlockSize, 1, 2, 1, 2);
    gtk_widget_show (labelBlockSize);

	/* Block complexity */
    label = gtk_label_new ("Block Complexity:  ");
	gtk_widget_modify_font (label, font);
	gtk_misc_set_alignment (GTK_MISC (label), 1, 0.5);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 2, 3, GTK_FILL, GTK_FILL, 6, 6);
    gtk_widget_show (label);

    labelBlockComplexity = gtk_label_new ("null");
	gtk_table_attach_defaults (GTK_TABLE (table), labelBlockComplexity, 1, 2, 2, 3);
    gtk_widget_show (labelBlockComplexity);

	/* Create "Limit to level" Label */
    label = gtk_label_new ("Limit to level:  ");
	gtk_widget_modify_font (label, font);
	gtk_misc_set_alignment (GTK_MISC (label), 1, 0.5);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 3, 4, GTK_FILL, GTK_FILL, 6, 6);
	gtk_widget_show (label);

    labelLevelLimit = gtk_label_new ("null");
	gtk_table_attach_defaults (GTK_TABLE (table), labelLevelLimit, 1, 2, 3, 4);
    gtk_widget_show (labelLevelLimit);

	/* Create "Quit" button */
    button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
	//gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 2, 4, 5);
	gtk_table_attach (GTK_TABLE (table), button, 1, 2, 4, 5, GTK_FILL, GTK_FILL, 6, 6);

    /* When the button is clicked, we call the "delete_event" function
     * and the program exits */
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (close_dialog), window);

    gtk_widget_show (button);

    g_signal_connect (G_OBJECT (window), "delete_event",
	      G_CALLBACK (delete_event), window);


    gtk_widget_show (table);
	Refresh();
    gtk_widget_show (window);
}
