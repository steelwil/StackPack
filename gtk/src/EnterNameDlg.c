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

#include <gtk/gtk.h>
#include "scoresDlg.h"
#include "gameData.h"
#include "StackPackEngine.h"

extern struct GameDataStr gameData;
extern struct GameData gameEngineData[cMAXPLAYERS];

static GtkWidget *window = NULL;
static GtkWidget *name_entry;
static GtkWidget *quote_entry;

/*----------------------------------------------------------------------------*/
void close_dialog(GtkWidget *widget, gpointer data)
{
	const gchar* name;
	const gchar* quote;
    g_print ("in close_dialog\n");
	name = gtk_entry_get_text(GTK_ENTRY(name_entry));
	quote = gtk_entry_get_text(GTK_ENTRY(quote_entry));
	printf("name=%s, quote=%s\n", name, quote);
	gtk_widget_hide (GTK_WIDGET(data));
	AddHighScore(&gameData, name, quote, 
				 gameEngineData[0].m_Score, 
				 gameEngineData[0].m_Lines, 
				 gameEngineData[0].m_Level);
	HighScoreDialogCreate(NULL);
}

/*----------------------------------------------------------------------------*/
static gboolean delete_event(GtkWidget *widget, GdkEvent  *event, gpointer data)
{
	printf("in destroy %s\n", "hello");
	close_dialog(NULL, data);
	return TRUE;
}

/*----------------------------------------------------------------------------*/
void EnterNameDialogCreate(GtkWidget *parent)
{
	if (window != NULL)
	{
		printf("window already created\n");
		
		gtk_entry_set_text(GTK_ENTRY(name_entry), "");
		gtk_entry_set_text(GTK_ENTRY(quote_entry), "");
		gtk_widget_show (window);
		return;
	}
	GtkWidget *label;
	GtkWidget *button;
	GtkWidget *box1;
	GtkWidget *box2;
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Enter Name Dialog");
	gtk_window_set_modal(GTK_WINDOW (window), TRUE);
	gtk_window_set_transient_for(GTK_WINDOW (window), GTK_WINDOW (parent));
	gtk_window_set_type_hint(GTK_WINDOW (window), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_container_set_border_width (GTK_CONTAINER (window), 6);
	box1 = gtk_vbox_new (FALSE, 12);
	box2 = gtk_hbox_new (FALSE, 12);
	gtk_container_add (GTK_CONTAINER (window), box1);

	label = gtk_label_new("Congradulations you made the StackPack\n"
						  "Hall of fame!");
	gtk_label_set_justify (GTK_LABEL(label), GTK_JUSTIFY_CENTER);
	gtk_box_pack_start (GTK_BOX(box1), label, TRUE, TRUE, 6);
	gtk_widget_show (label);

	label = gtk_label_new("Enter in your name:");
	gtk_box_pack_start (GTK_BOX(box1), label, FALSE, FALSE, 6);
	gtk_widget_show (label);
	
	name_entry = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY(name_entry), 31);
	gtk_box_pack_start (GTK_BOX(box1), name_entry, FALSE, FALSE, 6);
	gtk_widget_show (name_entry);
	
	label = gtk_label_new("Enter in your cool quote:");
	gtk_box_pack_start (GTK_BOX(box1), label, FALSE, FALSE, 6);
	gtk_widget_show (label);
	
	quote_entry = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY(quote_entry), 31);
	gtk_box_pack_start (GTK_BOX(box1), quote_entry, FALSE, FALSE, 6);
	gtk_widget_show (quote_entry);
	
	button = gtk_button_new_from_stock (GTK_STOCK_OK);
	gtk_box_pack_end (GTK_BOX (box2), button, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (button), "clicked",
		  G_CALLBACK (close_dialog), window);
    g_signal_connect (G_OBJECT (window), "delete_event",
		      G_CALLBACK (delete_event), window);

	gtk_widget_show (button);

	gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, FALSE, 0);

	gtk_widget_show (box2);
	gtk_widget_show (box1);
	gtk_widget_show (window);
}
