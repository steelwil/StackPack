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
#include "gameData.h"

extern struct GameDataStr gameData;

static GtkWidget *window = NULL;
static GtkListStore *store;

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
	printf("in destroy %s\n", "hello");
	close_dialog(NULL, data);
	return TRUE;
}

/*----------------------------------------------------------------------------*/
static void populate_model()
{
	int i;
	GtkTreeIter iter;
	gtk_list_store_clear(store);
	char datebuf[12];

	for (i=0; i<10; i++)
	{
		sprintf(datebuf, "%04d-%02d-%02d", 
						(int)gameData.m_HighScore[i].date/10000,
						(int)gameData.m_HighScore[i].date/100%100,
						(int)gameData.m_HighScore[i].date%100);
		gtk_list_store_append(store, &iter);
		gtk_list_store_set (store, &iter,
			RANK_COLUMN, i+1,
			SCORE_COLUMN, gameData.m_HighScore[i].score,
			ROWS_COLUMN, gameData.m_HighScore[i].rows,
			LEVEL_COLUMN, gameData.m_HighScore[i].level,
			DATE_COLUMN, datebuf,
			NAME_COLUMN, gameData.m_HighScore[i].name,
			QUOTE_COLUMN, gameData.m_HighScore[i].quote,
			-1);
	}
}

/*----------------------------------------------------------------------------*/
void HighScoreDialogCreate(GtkWidget *parent)
{
	if (window != NULL)
	{
		printf("window already created\n");
		populate_model();
		gtk_widget_show (window);
		return;
	}
	/* GtkWidget is the storage type for widgets */
	GtkWidget *button;
	GtkWidget *box1;
	GtkWidget *box2;
	GtkWidget *tree;
	
	/* Create a new window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for(GTK_WINDOW (window), GTK_WINDOW (parent));
	gtk_window_set_title (GTK_WINDOW (window), "StackPack Highest Scores");
	gtk_window_set_modal(GTK_WINDOW (window), TRUE);
	gtk_window_set_transient_for(GTK_WINDOW (window), GTK_WINDOW (parent));
	gtk_window_set_type_hint(GTK_WINDOW (window), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_position(GTK_WINDOW (window), GTK_WIN_POS_CENTER_ON_PARENT);
	
 	gtk_container_set_border_width (GTK_CONTAINER (window), 6);
	box1 = gtk_vbox_new (FALSE, 12);
	box2 = gtk_hbox_new (FALSE, 12);
	gtk_container_add (GTK_CONTAINER (window), box1);
	
	/* Create the list box" */
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;

	store = gtk_list_store_new (N_COLUMNS,
								G_TYPE_INT,
								G_TYPE_INT,
								G_TYPE_INT,
								G_TYPE_INT,
								G_TYPE_STRING,
								G_TYPE_STRING,
								G_TYPE_STRING);
	
	/* custom function to fill the model with data */
	populate_model (store);
	
	/* Create a view */
	tree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
	
	/* The view now holds a reference.  We can get rid of our own
	* reference */
	g_object_unref (G_OBJECT (store));
	
	/* Rank */
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Rnk", renderer,
													  "text", RANK_COLUMN,
													  NULL);
	gtk_tree_view_column_set_resizable(column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	
	/* Score */
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Score",
													  renderer,
													  "text", SCORE_COLUMN,
													 NULL);
	gtk_tree_view_column_set_expand(column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	
	/* Rows */
	renderer = gtk_cell_renderer_text_new  ();
	column = gtk_tree_view_column_new_with_attributes ("Rows",
													  renderer,
													  "text", ROWS_COLUMN,
													 NULL);
	gtk_tree_view_column_set_expand(column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	
	/* Level */
	renderer = gtk_cell_renderer_text_new  ();
	column = gtk_tree_view_column_new_with_attributes ("Level",
													  renderer,
													  "text", LEVEL_COLUMN,
													  NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	/* Date */
	renderer = gtk_cell_renderer_text_new  ();
	column = gtk_tree_view_column_new_with_attributes ("Date",
													  renderer,
													  "text", DATE_COLUMN,
													  NULL);
	gtk_tree_view_column_set_expand(column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	/* Name */
	renderer = gtk_cell_renderer_text_new  ();
	column = gtk_tree_view_column_new_with_attributes ("Name",
													  renderer,
													  "text", NAME_COLUMN,
													  NULL);
	gtk_tree_view_column_set_expand(column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	/* Quote */
	renderer = gtk_cell_renderer_text_new  ();
	column = gtk_tree_view_column_new_with_attributes ("Quote",
													  renderer,
													  "text", QUOTE_COLUMN,
													  NULL);
	gtk_tree_view_column_set_expand(column, TRUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	
	
	/* Instead of gtk_container_add, we pack this button into the invisible
	 * box, which has been packed into the window. */
	gtk_box_pack_start (GTK_BOX(box1), tree, TRUE, TRUE, 0);
	
	//GtkTreeSelection *selection;
	
	/* Always remember this step, this tells GTK that our preparation for
	 * this button is complete, and it can now be displayed. */
	GtkTreePath *path = gtk_tree_path_new_from_string("0:0");
	gtk_tree_view_set_cursor        (GTK_TREE_VIEW (tree),
											 path,
											 NULL,
											 FALSE);
	gtk_widget_show (tree);
	//selection = gtk_tree_view_get_selection(GTK_TREE_VIEW (tree));
	//gtk_tree_selection_unselect_all (selection);
	
	/* Do these same steps again to create a second button */
	button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
	
	/* Call the same callback function with a different argument,
	 * passing a pointer to "button 2" instead. */
	g_signal_connect (G_OBJECT (button), "clicked",
			  G_CALLBACK (close_dialog), window);
    g_signal_connect (G_OBJECT (window), "delete_event",
		      G_CALLBACK (delete_event), window);
	
	gtk_box_pack_end (GTK_BOX (box2), button, FALSE, FALSE, 0);
	/* Show the label. */
	gtk_widget_show (button);
	
	/* Pack box2 into box1 (the vbox remember ? :) */
	gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, FALSE, 0);
	
	gtk_widget_show (box2);
	gtk_widget_show (box1);
	gtk_widget_show (window);
}
