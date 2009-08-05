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

#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "gameData.h"
#include "userData.h"
#include "preferences.h"
#include "StackPackEngine.h"
#include "view.h"
#include "scoresDlg.h"
#include "propertiesDlg.h"
#include "EnterNameDlg.h"
#include <string.h>

static void menuitem_new(GtkAction *action);
static void menuitem_play();
static void menuitem_scores (GtkAction *action);
static void menuitem_properties (GtkAction *action);
static void menuitem_preferences (GtkAction *action);
static void menuitem_quit(GtkAction *action);
static void menuitem_pause(GtkAction *action);
static void menuitem_endgame();
static gint timeout_callback (gpointer data);


struct GameDataStr gameData;
struct UserDataStr userData;
struct GameData gameEngineData[cMAXPLAYERS];


GtkWidget *window;
GdkEventExpose drawevent;
GtkWidget *drawwidget;
static gint timertag;
static GtkActionGroup *action_group = NULL;
static GtkToolbar *toolbar = NULL;
static GtkWidget *statusbar;
GtkWidget *drawing_area;


/*----------------------------------------------------------------------------*/
static void activate_action (GtkAction *action)
{
	const gchar *name = gtk_action_get_name (action);
	const gchar *typename = G_OBJECT_TYPE_NAME (action);

	g_message ("Action %s (type=%s) activated", name, typename);
	if (!strcmp("play", name))
	{
		menuitem_play();
	}
	else if (!strcmp("end", name))
	{
		menuitem_endgame();
	}
}

/*----------------------------------------------------------------------------*/
void AboutDialogActivateLinkFunc  (GtkAboutDialog *about,
	                               const gchar *link,
                                   gpointer data)
{
	GError *err = NULL;
	gtk_show_uri(gdk_screen_get_default (), link, gtk_get_current_event_time (), &err);
}

/*----------------------------------------------------------------------------*/
void AboutDialogActivateEmailLinkFunc  (GtkAboutDialog *about,
	                               const gchar *link_,
                                   gpointer data)
{
	GError *err = NULL;
	char *address;

	address = g_strdup_printf ("mailto:%s?subject=Stackpack Game Feedback", link_);
	gtk_show_uri(gdk_screen_get_default (), address, gtk_get_current_event_time (), &err);
	g_free (address);
}

/*----------------------------------------------------------------------------*/
static void activate_about (GtkAction *action)
{
    const gchar *authors[] = { 
      "William Bell <william.bell@frog.za.net>", 
      NULL 
    };
	
	gtk_about_dialog_set_email_hook (AboutDialogActivateEmailLinkFunc, NULL, NULL);
	gtk_about_dialog_set_url_hook (AboutDialogActivateLinkFunc, NULL, NULL);
	gtk_show_about_dialog(GTK_WINDOW (window),
							"copyright", "Copyright © 2006-2008 William Bell",
							"comments", "A Tetris clone.",
							"authors", authors,
							"version", "1.2",
						    "website-label", "Stackpack Web Stite",
							"website", "http://www.frog.za.net/stackpack/stackpack.html",
							NULL);
}

/*----------------------------------------------------------------------------*/
static void toggle_toolbar(GtkAction *action)
{
	printf("in toggle_toolbar\n");
	if (TRUE == GTK_WIDGET_VISIBLE(GTK_WIDGET(toolbar)))
		gtk_widget_hide(GTK_WIDGET(toolbar));
	else
		gtk_widget_show(GTK_WIDGET(toolbar));
}

/*----------------------------------------------------------------------------*/
static void toggle_statusbar(GtkAction *action)
{
	printf("in toggle_statusbar\n");
	if (TRUE == GTK_WIDGET_VISIBLE(GTK_WIDGET(statusbar)))
		gtk_widget_hide(GTK_WIDGET(statusbar));
	else
		gtk_widget_show(GTK_WIDGET(statusbar));
}

/*----------------------------------------------------------------------------*/
static void set_tip (GtkWidget *widget, GtkAction *action)
{
  gchar *tooltip;
  
  if (action) 
    {
      g_object_get (action, "tooltip", &tooltip, NULL);
      gtk_statusbar_push (GTK_STATUSBAR (statusbar), 0, 
                          tooltip ? tooltip : "");
      
      g_free (tooltip);
    }
}

/*----------------------------------------------------------------------------*/
static void unset_tip (GtkWidget *widget)
{
    gtk_statusbar_pop (GTK_STATUSBAR (statusbar), 0);
}
          
/*----------------------------------------------------------------------------*/
static void connect_proxy (GtkUIManager *merge,
               GtkAction    *action,
               GtkWidget    *proxy)
{
	if (GTK_IS_MENU_ITEM (proxy)) 
	{
		g_signal_connect (proxy, "select",  G_CALLBACK (set_tip), action);
		g_signal_connect (proxy, "deselect", G_CALLBACK (unset_tip), NULL);
	}
}

/*----------------------------------------------------------------------------*/
/* XML description of the menus for the test app.  The parser understands
 * a subset of the Bonobo UI XML format, and uses GMarkup for parsing */
static const gchar *ui_info =
"  <menubar>\n"
"    <menu name=\"_File\" action=\"MenuFileAction\">\n"
"      <menuitem name=\"new\" action=\"new\" />\n"
"      <menuitem name=\"open\" action=\"open\" />\n"
"      <separator name=\"sep1\" />\n"
"      <menuitem name=\"junior\" action=\"junior\" />\n"
"      <menuitem name=\"classic\" action=\"classic\" />\n"
"      <menuitem name=\"cool\" action=\"cool\" />\n"
"      <menuitem name=\"mania\" action=\"mania\" />\n"
"      <separator name=\"sep2\" />\n"
"      <menuitem name=\"quit\" action=\"quit\" />\n"
"    </menu>\n"
"    <menu name=\"_Game\" action=\"GameMenuAction\">\n"
"      <menuitem name=\"play\" action=\"play\" />\n"
"      <menuitem name=\"pause\" action=\"pause\" />\n"
"      <menuitem name=\"end\" action=\"end\" />\n"
"      <separator name=\"sep1\" />\n"
"      <menuitem name=\"scores\" action=\"scores\" />\n"
"      <menuitem name=\"properties\" action=\"properties\" />\n"
"    </menu>\n"
"    <menu name=\"_Settings\" action=\"SettingsMenuAction\">\n"
"      <menuitem name=\"toolbar\" action=\"toolbar1\" />\n"
"      <menuitem name=\"statusbar\" action=\"statusbar1\" />\n"
"      <separator name=\"sep4\"/>\n"
"      <menuitem name=\"preferences\" action=\"preferences\" />\n"
"    </menu>\n"
"    <menu name=\"DynamicMenu\" action=\"HelpMenuAction\" />\n"
"    <menu name=\"_Help\" action=\"HelpMenuAction\">\n"
"      <menuitem name=\"helpContents\" action=\"helpContents\" />\n"
"      <menuitem name=\"helpAbout\" action=\"helpAbout\" />\n"
"    </menu>\n"
"  </menubar>\n"
"  <toolbar name=\"toolbar\">\n"
"    <toolitem name=\"new\" action=\"new\" />\n"
"    <toolitem name=\"play\" action=\"play\" />\n"
"    <toolitem name=\"pause\" action=\"pause\" />\n"
"    <separator name=\"sep9\" />\n"
"    <toolitem name=\"quit\" action=\"quit\" />\n"
"  </toolbar>\n";

/*----------------------------------------------------------------------------*/
/* convenience functions for declaring actions */
static GtkActionEntry entries[] = {
	{ "MenuFileAction", NULL, "_File" },
	{ "GameMenuAction", NULL, "_Game" },
	{ "SettingsMenuAction", NULL, "_Settings" },
	{ "HelpMenuAction", NULL, "_Help" },

	{ "toolbar", NULL, "_Toolbar", NULL,
		"Show toolbar", G_CALLBACK (activate_action) },
	{ "statusbar", NULL, "_Status Bar", NULL,
		"Show status Bar", G_CALLBACK (activate_action) },
	{ "properties", GTK_STOCK_PROPERTIES, "_Properties", NULL,
		"See this games properties", G_CALLBACK (menuitem_properties) },
	{ "scores", GTK_STOCK_EDIT, "_Scores", NULL,
		"See the hightest scores", G_CALLBACK (menuitem_scores) },
	{ "preferences", GTK_STOCK_PREFERENCES, "P_references", NULL,
		"Change game preferences", G_CALLBACK (menuitem_preferences) },
	{ "open", GTK_STOCK_OPEN, "_Open", NULL,
		"Open an existing game", G_CALLBACK (activate_action) },
	{ "play", GTK_STOCK_MEDIA_PLAY, "P_lay", "F2",
		"Start the game", G_CALLBACK (activate_action) },
	{ "helpAbout", GTK_STOCK_ABOUT, "_About", NULL,
		"About this game", G_CALLBACK (activate_about) },
	{ "helpContents", GTK_STOCK_HELP, "_Contents", "F1",
		"Open the StackPack manual", G_CALLBACK (activate_action) },
	{ "new", GTK_STOCK_NEW, "_New", "<control>N",
		"Starts a new game", G_CALLBACK (menuitem_new) },
	{ "pause", GTK_STOCK_MEDIA_PAUSE, "_Pause", "Pause",
		"Pause the game", G_CALLBACK (menuitem_pause) },
	{ "end", NULL, "_End Game", NULL,
		"End the current game", G_CALLBACK (activate_action) },
	{ "quit", GTK_STOCK_QUIT,  NULL, "<control>Q",
		"Quit the application", G_CALLBACK (menuitem_quit) },
};
static guint n_entries = G_N_ELEMENTS (entries);

/*----------------------------------------------------------------------------*/
static GtkToggleActionEntry toggle_entries[] = {
	{ "toolbar1", NULL, "Toggle Toolbar", NULL,
		"Toggle the visibility of the toolbar",
		G_CALLBACK (toggle_toolbar), TRUE },
	{ "statusbar1", NULL, "Toggle Status Bar", NULL,
		"Toggle the visibility of the status bar",
		G_CALLBACK (toggle_statusbar), TRUE },
};
static guint n_toggle_entries = G_N_ELEMENTS (toggle_entries);

enum {
	JUNIOR_GAME,
	CLASSIC_GAME,
	COOL_GAME,
	MANIA_GAME
};

/*----------------------------------------------------------------------------*/
static GtkRadioActionEntry stock_game_entries[] = {
	{ "junior", NULL, "_Junior", NULL,
		"Only the most simple shapes", JUNIOR_GAME },
	{ "classic", NULL, "C_lassic", NULL,
		"Traditional shapes", CLASSIC_GAME },
	{ "cool", NULL, "_Cool", NULL,
		"Skips all difficult shapes", COOL_GAME },
	{ "mania", NULL, "_Mania", NULL,
		"The name says it all", MANIA_GAME }
};

static guint n_stock_game_entries = G_N_ELEMENTS (stock_game_entries);


/*----------------------------------------------------------------------------*/
static void radio_action (GtkAction *action)
{
	const gchar *name = gtk_action_get_name (action);
	const gchar *typename = G_OBJECT_TYPE_NAME (action);

	int gameID = gtk_radio_action_get_current_value (GTK_RADIO_ACTION (action));
	g_message ("Action %s (type=%s) activated (active=%d) (value %d)", name, typename,
	            gtk_toggle_action_get_active (GTK_TOGGLE_ACTION (action)),
	            gameID);
	SaveGameData(&gameData, userData.m_FileName);
	if (gameID == JUNIOR_GAME)
	{
		g_message("InitializeGameData junior.gam");
		strcpy(userData.m_FileName, "junior.gam");
		InitializeGameData(&gameData, userData.m_FileName);
	}
	else if (gameID == CLASSIC_GAME)
	{
		g_message("InitializeGameData classic.gam");
		strcpy(userData.m_FileName, "classic.gam");
		InitializeGameData(&gameData, userData.m_FileName);
	}
	else if (gameID == COOL_GAME)
	{
		g_message("InitializeGameData cool.gam");
		strcpy(userData.m_FileName, "cool.gam");
		InitializeGameData(&gameData, userData.m_FileName);
	}
	else if (gameID == MANIA_GAME)
	{
		g_message("InitializeGameData mania.gam");
		strcpy(userData.m_FileName, "mania.gam");
		InitializeGameData(&gameData, userData.m_FileName);
	}
	GameCreate(&gameEngineData[0],
							0,
							0,
							gameData.m_Base,  
							gameData.m_Height,
							gameData.m_StartingLevel,
							gameData.m_StartingRows,
							userData.m_PiecePreview,
							gameData.m_BlkType,
							gameData.m_LastLevel);
	viewResize(drawwidget);
	viewRedraw(drawwidget, 2, &drawevent);
}
	
/*----------------------------------------------------------------------------*/
static gboolean key_press_event(GtkWidget *widget, GdkEventKey *event)
{
 	/*printf("key pressed %d\n", event->keyval);*/
	int p, rc = 1;
	int area = 1;
	
	/* get highest level */
	int prevLevel = gameEngineData[0].m_Level;
	for (p=1; p<userData.m_NumberOfPlayers; p++)
	{
		if (prevLevel < gameEngineData[p].m_Level)
			prevLevel = gameEngineData[p].m_Level;
	}
	
	if (event->keyval == (userData.m_LeftKey1))
		GameLeft(&gameEngineData[0]);
	else if (event->keyval == (userData.m_RightKey1))
		GameRight(&gameEngineData[0]);
	else if (event->keyval == (userData.m_DownKey1))
		GameForceStepNow(&gameEngineData[0]);
	else if (event->keyval == (userData.m_RotateKey1))
		GameRotate(&gameEngineData[0]);
	else if (event->keyval == (userData.m_DropKey1))
		rc = GameDrop(&gameEngineData[0]);

	else if (event->keyval == (userData.m_LeftKey2))
		GameLeft(&gameEngineData[1]);
	else if (event->keyval == (userData.m_RightKey2))
		GameRight(&gameEngineData[1]);
	else if (event->keyval == (userData.m_DownKey2))
		GameForceStepNow(&gameEngineData[1]);
	else if (event->keyval == (userData.m_RotateKey2))
		GameRotate(&gameEngineData[1]);
	else if (event->keyval == (userData.m_DropKey2))
		rc = GameDrop(&gameEngineData[1]);
	else if (event->keyval == GDK_Escape)
	{
		if (!IsGamePaused(&gameEngineData[0]))
		{
			for (p=0; p<userData.m_NumberOfPlayers; p++)
			{
				GamePause(&gameEngineData[p]);
			}
			g_source_remove(timertag);
		}
		gtk_window_iconify(GTK_WINDOW (window));
	}
	else if (event->keyval == 0) /* event from timer */
	{
		for (p=0; p<userData.m_NumberOfPlayers; p++)
		{
			rc = GameStep(&gameEngineData[p]);
			if (rc == 0) break;
		}
	}
	else
	{
		return FALSE;
	}
	for (p=0; p<userData.m_NumberOfPlayers; p++)
	{
		if (prevLevel < gameEngineData[p].m_Level)
		{
			viewResize(drawwidget);
			g_source_remove(timertag);
			timertag = g_timeout_add(gameEngineData[0].m_StepDuration,
									 timeout_callback,
									 NULL);
			area = 2;
			break;
		}
	}
	viewRedraw(drawwidget, area, &drawevent);
	for (p=0; p<userData.m_NumberOfPlayers; p++)
	{
		if (!IsGameInProgress(&gameEngineData[p]))
		{
			if (IsHighScore(&gameData, gameEngineData[p].m_Score))
			{
				EnterNameDialogCreate(window);
			}
		}
	}
	
	return FALSE;
}

/*----------------------------------------------------------------------------*/
static gint timeout_callback (gpointer data)
{
	//printf("in timeout_callback\n");
	GdkEventKey eventkey;
	eventkey.keyval = 0;
	key_press_event(drawwidget, &eventkey);
	return (!gameEngineData[0].m_GameOver);
}

/*----------------------------------------------------------------------------*/
static void menuitem_new(GtkAction *action)
{
    g_message("in menuitem_new");
}

/*----------------------------------------------------------------------------*/
static void menuitem_play()
{
	g_message("in menuitem_play");
	int p;
	if (IsGamePaused(&gameEngineData[0]))
	{
		for (p=0; p<userData.m_NumberOfPlayers; p++)
		{
			GameStart(&gameEngineData[p]);
		}
		timertag = g_timeout_add(gameEngineData[0].m_StepDuration,
								 timeout_callback,
								 NULL);
	}
	else
	{
		GameCreate(&gameEngineData[0],
								0,
								0,
								gameData.m_Base,  
								gameData.m_Height,
								gameData.m_StartingLevel,
								gameData.m_StartingRows,
								userData.m_PiecePreview,
								gameData.m_BlkType,
								gameData.m_LastLevel);
		viewRedraw(drawwidget, 2, &drawevent);
		timertag = g_timeout_add(gameEngineData[0].m_StepDuration,
								 timeout_callback,
								 NULL);
		printf("timeout=%d\n", gameEngineData[0].m_StepDuration);
	}
}

/*----------------------------------------------------------------------------*/
static void menuitem_endgame()
{
	g_message("in menuitem_endgame");
	EndGame(&gameEngineData[0]);
	EndGame(&gameEngineData[1]);
}

/*----------------------------------------------------------------------------*/
static void menuitem_quit(GtkAction *action)
{
    printf ("quit\n");
	gtk_window_get_position(GTK_WINDOW (window), &userData.x, &userData.y);
	printf("in quit x=%d, y=%d\n", userData.x, userData.y);
	userData.m_ToolBar = GTK_WIDGET_VISIBLE(GTK_WIDGET(toolbar));
	userData.m_StatusBar = GTK_WIDGET_VISIBLE(GTK_WIDGET(statusbar));
	SaveUserData(&userData);
	SaveGameData(&gameData, userData.m_FileName);
	gtk_main_quit ();
}

/*----------------------------------------------------------------------------*/
static gboolean configure_event(GtkWidget* widget, GdkEventConfigure *event, gpointer window)
{
	userData.width = GTK_WIDGET (window)->allocation.width;
	userData.height = GTK_WIDGET (window)->allocation.height;
	printf("in configure_event width=%d, height=%d\n", userData.width, userData.height);
	viewResize(widget);
	return TRUE;
}

/*----------------------------------------------------------------------------*/
/* Redraw the screen from the backing pixmap */
static gboolean expose_event( GtkWidget *widget, GdkEventExpose *event)
{
	viewRedraw(widget, 0, event);
	drawwidget = widget;
	return FALSE;
}

/*------------------------------------------------------------------------------
  Display the hightest scores
*/
static void menuitem_scores (GtkAction *action)
{
	HighScoreDialogCreate(window);
	//EnterNameDialogCreate(window);
}

/*----------------------------------------------------------------------------*/
static void menuitem_properties (GtkAction *action)
{
	PropertiesDialogCreate(window);
	//EnterNameDialogCreate(window);
}

/*----------------------------------------------------------------------------*/
static void menuitem_preferences (GtkAction *action)
{
	PreferencesDialogCreate(window);
	//EnterNameDialogCreate(window);
}

/*----------------------------------------------------------------------------*/
static void menuitem_pause (GtkAction *action)
{
	g_message("in menuitem_pause");
	int p;
	if (!IsGamePaused(&gameEngineData[0]))
	{
		for (p=0; p<userData.m_NumberOfPlayers; p++)
		{
			GamePause(&gameEngineData[p]);
		}
		g_source_remove(timertag);
	}
	else
	{
		for (p=0; p<userData.m_NumberOfPlayers; p++)
		{
			GameStart(&gameEngineData[p]);
		}
		timertag = g_timeout_add(gameEngineData[0].m_StepDuration,
								 timeout_callback,
								 NULL);
	}
}

/*----------------------------------------------------------------------------*/
static gboolean delete_event(GtkWidget *widget, GdkEvent  *event, gpointer data)
{
	//gtk_window_get_position(GTK_WINDOW (data), &userData.x, &userData.y);
	//printf("x=%d, y=%d\n", userData.x, userData.y);
	printf("in destroy %s\n", "hello");
	menuitem_quit(NULL);
	return TRUE;
}

/*----------------------------------------------------------------------------*/
static void add_widget (GtkUIManager *merge,
						GtkWidget   *widget,
						GtkContainer *container)
{
	gtk_box_pack_start (GTK_BOX (container), widget, FALSE, FALSE, 0);
	gtk_widget_show (widget);

	if (GTK_IS_TOOLBAR (widget)) 
	{
		printf("found toolbar\n");
		toolbar = GTK_TOOLBAR (widget);
		gtk_toolbar_set_show_arrow (toolbar, TRUE);
		if (userData.m_ToolBar == FALSE)
		{
			g_message("hiding toolbar");
		    gtk_widget_hide(GTK_WIDGET(toolbar));
		}
	}
}

/*----------------------------------------------------------------------------*/
static void create_window (GtkActionGroup *action_group, GtkWidget *window, GtkWidget *vbox)
{
	GtkUIManager *merge;
	GError *error = NULL;

	merge = gtk_ui_manager_new ();

	gtk_ui_manager_insert_action_group (merge, action_group, 0);
	g_signal_connect (merge, "add-widget", G_CALLBACK (add_widget), vbox);

	gtk_window_add_accel_group (GTK_WINDOW (window), 
	gtk_ui_manager_get_accel_group (merge));

	if (!gtk_ui_manager_add_ui_from_string (merge, ui_info, -1, &error))
	{
		g_message ("building menus failed: %s", error->message);
		g_error_free (error);
	}

	statusbar = gtk_statusbar_new ();
	gtk_box_pack_end (GTK_BOX (vbox), statusbar, FALSE, FALSE, 0);

	g_signal_connect(merge, "connect-proxy", G_CALLBACK (connect_proxy), NULL);
}

/*============================================================================*/
int main( int   argc,
          char *argv[] )
{
    GtkWidget *vbox;

    gtk_init (&argc, &argv);
	InitializeUserData(&userData);
	InitializeGameData(&gameData, userData.m_FileName);

    /* create a new window */
	printf("width=%d, height=%d\n", userData.width, userData.height);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_resize (GTK_WINDOW (window), userData.width, userData.height);
	printf("in main x=%d, y=%d\n", userData.x, userData.y);
	gtk_window_move(GTK_WINDOW (window), userData.x, userData.y);
    gtk_window_set_title (GTK_WINDOW (window), "GTK StackPack");
    g_signal_connect (G_OBJECT (window), "delete_event",
                      G_CALLBACK (delete_event), NULL);
    /* A vbox to put a menu and a drawing_area in: */
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);

	action_group = gtk_action_group_new ("TestActions");
	create_window (action_group, window, vbox);
	gtk_action_group_add_actions (action_group, entries, n_entries, NULL);
	
	if (userData.m_ToolBar == FALSE)
	{
		//g_message("hiding toolbar");
	//	gtk_widget_hide(GTK_WIDGET(toolbar));
		//toggle_toolbar(NULL);
		toggle_entries[0].is_active = FALSE;
	}
	if (userData.m_StatusBar == TRUE)
	{
		gtk_widget_show (statusbar);
	}
	else
	{
		toggle_entries[1].is_active = FALSE;
	}
	gtk_action_group_add_toggle_actions (action_group, toggle_entries, n_toggle_entries, NULL);
	
	int gameID = -1;
	if (!strcmp(userData.m_FileName, "junior.gam"))
	{
		gameID = JUNIOR_GAME;
	}
	else if (!strcmp(userData.m_FileName, "classic.gam"))
	{
		gameID = CLASSIC_GAME;
	}
	else if (!strcmp(userData.m_FileName, "cool.gam"))
	{
		gameID = COOL_GAME;
	}
	else if (!strcmp(userData.m_FileName, "mania.gam"))
	{
		gameID = MANIA_GAME;
	}
	
	gtk_action_group_add_radio_actions (action_group, stock_game_entries, n_stock_game_entries, 
										gameID,
										G_CALLBACK (radio_action), NULL);


	/* Create the drawing area */
	drawing_area = gtk_drawing_area_new ();
	gtk_widget_set_size_request (GTK_WIDGET (drawing_area), 200, 200);
	gtk_box_pack_end (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);
	
	GameCreate(&gameEngineData[0],
							0,
						    0,
						    gameData.m_Base,  
							gameData.m_Height,
							gameData.m_StartingLevel,
							gameData.m_StartingRows,
							userData.m_PiecePreview,
							gameData.m_BlkType,
							gameData.m_LastLevel);
	viewInitialize(drawing_area);
	
	gtk_widget_show (drawing_area);
	
	/* Signals used to handle backing pixmap */
	
	g_signal_connect (G_OBJECT (drawing_area), "expose_event",
			G_CALLBACK (expose_event), NULL);
	g_signal_connect (G_OBJECT (drawing_area),"configure_event",
			G_CALLBACK (configure_event), window);
	
	/* Event signals */
	
	g_signal_connect (G_OBJECT (window), "key_press_event",
			G_CALLBACK (key_press_event), NULL);
	
	gtk_widget_set_events (drawing_area, GDK_EXPOSURE_MASK
			 | GDK_LEAVE_NOTIFY_MASK
			 | GDK_KEY_PRESS);
	

	/* always display the window as the last step so it all splashes on
     * the screen at once. */
    gtk_widget_show (window);

    gtk_main ();

    return 0;
}
