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
 
 /* This handles the preference dialog */

#include <gtk/gtk.h>
#include "gameData.h"
#include "userData.h"
#include "view.h"

extern struct GameDataStr gameData;
extern struct UserDataStr userData;
extern GtkWidget *drawwidget;
extern GdkEventExpose drawevent;


static GtkWidget *window = NULL; 	/* main window */
static GtkWidget *gameWidget = NULL; /* game preferences */
static GtkWidget *viewWidget = NULL; /* view preferences */
static GtkWidget *keysWidget = NULL; /* key preferences */

/* game preferences */
static GtkWidget *buttonOnePlayer;
static GtkWidget *buttonTwoPlayer;
static GtkWidget *buttonPenelize;
static GtkWidget *buttonSound;
static GtkWidget *spinboxStartingRows;
static GtkWidget *spinboxStartingLevel;

/* view preferences */
static GtkWidget *buttonPiecePreview;
static GtkWidget *buttonHorizontalGrid;
static GtkWidget *buttonVerticalGrid;
static GtkWidget *comboboxPieceType;

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
 void OnePlayerSelected(GtkRadioButton *radiobutton, gpointer data)
 {
	g_print ("in OnePlayerSelected\n");
	userData.m_NumberOfPlayers = 1;
 	viewResize(drawwidget);
 	viewRedraw(drawwidget, 2, &drawevent);
 }

/*----------------------------------------------------------------------------*/
 void TwoPlayerSelected(GtkRadioButton *radiobutton, gpointer data)
 {
	g_print ("in TwoPlayerSelected\n");
 	userData.m_NumberOfPlayers = 2;
 	viewResize(drawwidget);
 	viewRedraw(drawwidget, 2, &drawevent);
}


/*----------------------------------------------------------------------------*/
 void PenelizeOpponentToggled(GtkToggleButton *checkbutton, gpointer data)
 {
	if (gtk_toggle_button_get_active(checkbutton))
	{
		g_print ("PenelizeOpponentToggled to TRUE\n");
 		gameData.m_Penalize = 1;
 	}
 	else
 	{ 
		g_print ("PenelizeOpponentToggled to FALSE\n");
 		gameData.m_Penalize = 0;
	}
}

/*----------------------------------------------------------------------------*/
 void SoundToggled(GtkToggleButton *checkbutton, gpointer data)
 {
	if (gtk_toggle_button_get_active(checkbutton))
	{
		g_print ("Sound Toggled to TRUE\n");
 		userData.m_Sound = 1;
 	}
 	else
 	{ 
		g_print ("Sound Toggled to FALSE\n");
 		userData.m_Sound = 0;
	}
}

/*----------------------------------------------------------------------------*/
 void StartingRowsChanged(GtkSpinButton *spinbutton, gpointer data)
 {
	gdouble val = gtk_spin_button_get_value (spinbutton);
	g_print ("in StartingRowsChanged to %f\n", val);
	gameData.m_StartingRows = val;
  	viewResize(drawwidget);
 	viewRedraw(drawwidget, 2, &drawevent);
}
 
/*----------------------------------------------------------------------------*/
 void StartingLevelChanged(GtkSpinButton *spinbutton, gpointer data)
 {
	gdouble val = gtk_spin_button_get_value (spinbutton);
	g_print ("in StartingLevelChanged to %f\n", val);
	gameData.m_StartingLevel = val;
  	viewResize(drawwidget);
 	viewRedraw(drawwidget, 2, &drawevent);
}

/*----------------------------------------------------------------------------*/
 void PreviewToggled(GtkToggleButton *checkbutton, gpointer data)
 {
	if (gtk_toggle_button_get_active(checkbutton))
	{
		g_print ("Preview Toggled to TRUE\n");
 		userData.m_PiecePreview = 1;
 	}
 	else
 	{ 
		g_print ("Preview Toggled to FALSE\n");
 		userData.m_PiecePreview = 0;
	}
 	viewResize(drawwidget);
 	viewRedraw(drawwidget, 2, &drawevent);
}

/*----------------------------------------------------------------------------*/
 void VerticalGridToggled(GtkToggleButton *checkbutton, gpointer data)
 {
	if (gtk_toggle_button_get_active(checkbutton))
	{
		g_print ("VerticalGrid Toggled to TRUE\n");
		userData.m_GridStyle = userData.m_GridStyle | 1;
 	}
 	else
 	{ 
		g_print ("VerticalGrid Toggled to FALSE\n");
		userData.m_GridStyle = userData.m_GridStyle & ~1;
	}
	g_print("userData.m_GridStyle = %d\n", userData.m_GridStyle);
 	viewResize(drawwidget);
 	viewRedraw(drawwidget, 2, &drawevent);
}

/*----------------------------------------------------------------------------*/
 void HorizontalGridToggled(GtkToggleButton *checkbutton, gpointer data)
 {
	if (gtk_toggle_button_get_active(checkbutton))
	{
		g_print ("HorizontalGrid Toggled to TRUE\n");
		userData.m_GridStyle = userData.m_GridStyle | 2;
 	}
 	else
 	{ 
		g_print ("HorizontalGrid Toggled to FALSE\n");
		userData.m_GridStyle = userData.m_GridStyle & ~2;
	}
	g_print("userData.m_GridStyle = %d\n", userData.m_GridStyle);
 	viewResize(drawwidget);
 	viewRedraw(drawwidget, 2, &drawevent);
}

/*----------------------------------------------------------------------------*/
static void  PieceTypeChanged(GtkComboBox *widget, gpointer data) 
{
	gchar*  txt = gtk_combo_box_get_active_text(widget);
	g_print ("in PieceTypeChanged to %s\n", txt);
	if (txt[0] == 'F') /* Flat */
		userData.m_PieceStyle = 0;
	else if (txt[0] == '3') /* 3D */
		userData.m_PieceStyle = 1;
	else
		userData.m_PieceStyle = 2; /* Pyramid */
 	viewResize(drawwidget);
 	viewRedraw(drawwidget, 2, &drawevent);
}
/*----------------------------------------------------------------------------*/
static void Refresh()
{
	// number of players
	if (userData.m_NumberOfPlayers == 2)
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (buttonTwoPlayer), TRUE);
	}
	else
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (buttonOnePlayer), TRUE);
	}
	
	// penalize opponent
	if (gameData.m_Penalize == 1)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttonPenelize), TRUE);
	}
	
	// sound
	if (userData.m_Sound == 1)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttonSound), TRUE);
	}
	
	// starting rows
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinboxStartingRows), gameData.m_StartingRows);
	
	// starting level
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinboxStartingLevel), gameData.m_StartingLevel);

	// piece Preview
	if (userData.m_PiecePreview == 1)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttonPiecePreview), TRUE);
	}
	// vertical grid lines
	if (userData.m_GridStyle == 1 || userData.m_GridStyle == 3)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttonVerticalGrid), TRUE);
	}
	// Horizontal grid lines
	if (userData.m_GridStyle == 2 || userData.m_GridStyle == 3)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttonHorizontalGrid), TRUE);
	}
	// Piece Type
	gtk_combo_box_set_active (GTK_COMBO_BOX(comboboxPieceType), userData.m_PieceStyle);
	
}

/*----------------------------------------------------------------------------*/
void PreferencesDialogCreate(GtkWidget *parent)
{
	if (window != NULL)
	{
		printf("preferences window already created\n");
		Refresh();
		gtk_widget_show (window);
		return;
	}
	GtkWidget *button;
	GtkWidget *box1;
	GtkWidget *box2;
	GtkWidget *notebook;
	GtkWidget *label;
	GtkWidget *buttonbox;
	
	/* Create a new window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for(GTK_WINDOW (window), GTK_WINDOW (parent));
	gtk_window_set_title (GTK_WINDOW (window), "StackPack Preferences");
	gtk_window_set_modal(GTK_WINDOW (window), TRUE);
	gtk_window_set_transient_for(GTK_WINDOW (window), GTK_WINDOW (parent));
	gtk_window_set_type_hint(GTK_WINDOW (window), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_position(GTK_WINDOW (window), GTK_WIN_POS_CENTER_ON_PARENT);
	
 	gtk_container_set_border_width (GTK_CONTAINER (window), 6);
	box1 = gtk_vbox_new (FALSE, 12);
	gtk_container_add (GTK_CONTAINER (window), box1);
	button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);

	notebook = gtk_notebook_new ();
	buttonbox = gtk_hbutton_box_new();
	gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonbox), GTK_BUTTONBOX_END);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
	gtk_box_pack_start (GTK_BOX(box1), notebook, TRUE, TRUE, 0);
	gtk_box_pack_end (GTK_BOX(box1), buttonbox, FALSE, FALSE, 0);
	gtk_box_pack_end (GTK_BOX(buttonbox), button, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (button), "clicked",
			  G_CALLBACK (close_dialog), window);
    g_signal_connect (G_OBJECT (window), "delete_event",
		      G_CALLBACK (delete_event), window);
	gtk_widget_show (button);
	gtk_widget_show (buttonbox);
	 

	/* Create Game Settings page */
	GSList *group;

	gameWidget = gtk_vbox_new (FALSE, 12);
    gtk_container_set_border_width (GTK_CONTAINER (gameWidget), 6);
	
	//	player count
	buttonOnePlayer = gtk_radio_button_new_with_label (NULL, "One player");
	gtk_box_pack_start (GTK_BOX (gameWidget), buttonOnePlayer, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (buttonOnePlayer), "clicked",
			  G_CALLBACK (OnePlayerSelected), window);
	gtk_widget_show (buttonOnePlayer);
	
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (buttonOnePlayer));
	buttonTwoPlayer = gtk_radio_button_new_with_label (group, "Two players");
	gtk_box_pack_start (GTK_BOX (gameWidget), buttonTwoPlayer, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (buttonTwoPlayer), "clicked",
			  G_CALLBACK (TwoPlayerSelected), window);
	gtk_widget_show (buttonTwoPlayer);
	
	// penelize
	buttonPenelize = gtk_check_button_new_with_label ("Penelize opponent");
	gtk_box_pack_start (GTK_BOX (gameWidget), buttonPenelize, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (buttonPenelize), "clicked",
			  G_CALLBACK (PenelizeOpponentToggled), window);
	gtk_widget_show (buttonPenelize);

	// sound
	buttonSound = gtk_check_button_new_with_label ("Sound");
	gtk_box_pack_start (GTK_BOX (gameWidget), buttonSound, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (buttonSound), "clicked",
			  G_CALLBACK (SoundToggled), window);
	gtk_widget_show (buttonSound);

	box2 = gtk_hbox_new (FALSE, 12);
	gtk_box_pack_start (GTK_BOX (gameWidget), box2, TRUE, TRUE, 0);
	
	// starting rows
	label = gtk_label_new ("Starting rows");
	gtk_box_pack_start (GTK_BOX (box2), label, TRUE, TRUE, 0);
	gtk_widget_show (label);
	spinboxStartingRows = gtk_spin_button_new_with_range(0, 32, 1);
	gtk_box_pack_start (GTK_BOX (box2), spinboxStartingRows, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (spinboxStartingRows), "value-changed",
		  G_CALLBACK (StartingRowsChanged), window);
	gtk_widget_show (spinboxStartingRows);
	gtk_widget_show (box2);

	box2 = gtk_hbox_new (FALSE, 12);
	gtk_box_pack_start (GTK_BOX (gameWidget), box2, TRUE, TRUE, 0);
	
	// starting level
	label = gtk_label_new ("Starting level");
	gtk_box_pack_start (GTK_BOX (box2), label, TRUE, TRUE, 0);
	gtk_widget_show (label);
	spinboxStartingLevel = gtk_spin_button_new_with_range(1, 10, 1);
	gtk_box_pack_start (GTK_BOX (box2), spinboxStartingLevel, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (spinboxStartingLevel), "value-changed",
		  G_CALLBACK (StartingLevelChanged), window);
	gtk_widget_show (spinboxStartingLevel);
	gtk_widget_show (box2);

	label = gtk_label_new ("Game");
	gtk_widget_show (gameWidget);
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), gameWidget, label);	

	/* Create Game View page */
	viewWidget = gtk_vbox_new (FALSE, 12);
    gtk_container_set_border_width (GTK_CONTAINER (viewWidget), 6);
	
	// piece preview
	buttonPiecePreview = gtk_check_button_new_with_label ("Piece preview");
	gtk_box_pack_start (GTK_BOX (viewWidget), buttonPiecePreview, FALSE, TRUE, 0);
	g_signal_connect (G_OBJECT (buttonPiecePreview), "clicked",
			  G_CALLBACK (PreviewToggled), window);
	gtk_widget_show (buttonPiecePreview);

	// vertical grid lines
	buttonVerticalGrid = gtk_check_button_new_with_label ("Vertical grid lines");
	gtk_box_pack_start (GTK_BOX (viewWidget), buttonVerticalGrid, FALSE, TRUE, 0);
	g_signal_connect (G_OBJECT (buttonVerticalGrid), "clicked",
			  G_CALLBACK (VerticalGridToggled), window);
	gtk_widget_show (buttonVerticalGrid);

	// Horizontal grid lines
	buttonHorizontalGrid = gtk_check_button_new_with_label ("Horizontal grid lines");
	gtk_box_pack_start (GTK_BOX (viewWidget), buttonHorizontalGrid, FALSE, TRUE, 0);
	g_signal_connect (G_OBJECT (buttonHorizontalGrid), "clicked",
			  G_CALLBACK (HorizontalGridToggled), window);
	gtk_widget_show (buttonHorizontalGrid);

	box2 = gtk_hbox_new (FALSE, 12);
	gtk_box_pack_start (GTK_BOX (viewWidget), box2, FALSE, TRUE, 0);
	
	// Piece type
	label = gtk_label_new ("Piece type");
	gtk_box_pack_start (GTK_BOX (box2), label, FALSE, TRUE, 0);
	gtk_widget_show (label);
	
	comboboxPieceType = gtk_combo_box_new_text ();
	gtk_box_pack_start (GTK_BOX (box2), comboboxPieceType, FALSE, TRUE, 0);
	gtk_combo_box_append_text (GTK_COMBO_BOX(comboboxPieceType), "Flat");
	gtk_combo_box_append_text (GTK_COMBO_BOX(comboboxPieceType), "3D");
	gtk_combo_box_append_text (GTK_COMBO_BOX(comboboxPieceType), "Diamond");
	g_signal_connect (G_OBJECT (comboboxPieceType), "changed",
		  G_CALLBACK (PieceTypeChanged), window);
	gtk_widget_show (comboboxPieceType);
	gtk_widget_show (box2);

	label = gtk_label_new ("View");
	gtk_widget_show (viewWidget);
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), viewWidget, label);	


	/* Create Game Keys page */
	keysWidget = gtk_vbox_new (FALSE, 12);
    gtk_container_set_border_width (GTK_CONTAINER (keysWidget), 6);
	label = gtk_label_new ("Keys");
	gtk_widget_show (keysWidget);
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), keysWidget, label);	

	Refresh();
	gtk_widget_show (notebook);
	gtk_widget_show (box1);
	gtk_widget_show (window);

}
