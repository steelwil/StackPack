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
#include <gdk/gdkkeysyms.h>
#include "userData.h"
#include <string.h>

/*----------------------------------------------------------------------------*/
void ReadUserData(struct UserDataStr* data)
{
	GKeyFile* key_file = g_key_file_new();
	GError *error;
	if (TRUE == g_key_file_load_from_file(key_file,
                                          "stackpack.conf",
                                          G_KEY_FILE_NONE,
                                          NULL))
	{
		printf("Opened stackpack.conf OK\n");
		error = NULL;
		
		/* Geometry */
		data->x = g_key_file_get_integer(key_file, "Geometry", "x", &error);
		if (error != NULL)
		{
			data->x = 0;
			g_clear_error(&error);
		} 
		
		data->y = g_key_file_get_integer(key_file, "Geometry", "y", &error);
		if (error != NULL)
		{
			data->y = 0;
			g_clear_error(&error);
		}
		data->width = g_key_file_get_integer(key_file, "Geometry", "width", &error);
		if (error != NULL)
		{
			data->width = 500;
			g_clear_error(&error);
		}
		
		data->height = g_key_file_get_integer(key_file, "Geometry", "height", &error);
		if (error != NULL)
		{
			data->height = 400;
			g_clear_error(&error);
		}
		
		/*	Options */
		data->m_PiecePreview = g_key_file_get_integer(key_file, "Options", "piecepreview", &error);
		if (error != NULL)
		{
			data->m_PiecePreview = 1;
			g_clear_error(&error);
		}
		data->m_NumberOfPlayers = g_key_file_get_integer(key_file, "Options", "players", &error);
		if (error != NULL)
		{
			data->m_NumberOfPlayers = 1;
			g_clear_error(&error);
		}
		data->m_PieceStyle = g_key_file_get_integer(key_file, "Options", "piecestyle", &error);
		if (error != NULL)
		{
			data->m_PieceStyle = 1;
			g_clear_error(&error);
		}
		if (data->m_PieceStyle < 0 || data->m_PieceStyle > 2)
		{
			data->m_PieceStyle = 2;
		}
		data->m_GridStyle = g_key_file_get_integer(key_file, "Options", "gridstyle", &error);
		if (error != NULL)
		{
			data->m_GridStyle = 1;
			g_clear_error(&error);
		}
		if (data->m_GridStyle < 0 || data->m_GridStyle > 3)
		{
			data->m_GridStyle = 1;
		}
		data->m_Sound = g_key_file_get_integer(key_file, "Options", "sound", &error);
		if (error != NULL)
		{
			data->m_Sound = 1;
			g_clear_error(&error);
		}

		char* fn = g_key_file_get_string(key_file, "Options", "autoload", &error);		
		if (error != NULL || 4 > strlen(fn))
		{
			strcpy(data->m_FileName, "stackpack.gam");
			g_clear_error(&error);
		}
		else
		{
			strncpy(data->m_FileName, fn, 128);
			g_free(fn);
		}
		
		/* View*/
		data->m_ToolBar = g_key_file_get_integer(key_file, "View", "toolbar", &error);
		if (error != NULL)
		{
			data->m_ToolBar = TRUE;
			g_clear_error(&error);
		}
		data->m_StatusBar = g_key_file_get_integer(key_file, "View", "statusbar", &error);
		if (error != NULL)
		{
			data->m_StatusBar = TRUE;
			g_clear_error(&error);
		}

	}
	printf("ReadUserData: x=%d, y=%d\n", data->x, data->y);
	g_key_file_free(key_file);
}

/*----------------------------------------------------------------------------*/
void InitializeUserData(struct UserDataStr* data)
{
/* 	data->m_Penalize = 1; */
	data->m_PieceStyle = 1; /* 0=Flat Block, 1=3D Block, 2=Pyrimid Block */;
	data->m_PiecePreview = 1;
	data->m_GridStyle = 1; /* 1=vertical, 2=horizontal, 3=both */
	data->m_NumberOfPlayers = 1;
	data->m_ToolBar = TRUE;
	data->m_StatusBar = TRUE;
	data->m_Sound = 1;
	
	data->m_LeftKey1 = GDK_Left;
	data->m_RightKey1 = GDK_Right;
	data->m_DropKey1 = GDK_Down;
	data->m_RotateKey1 = GDK_Up;
	data->m_DownKey1 = GDK_End;
	
	data->m_LeftKey2 = GDK_J;
	data->m_RightKey2 = GDK_L;
	data->m_DownKey2 = GDK_space;
	data->m_RotateKey2 = GDK_I;
	data->m_DropKey2 = GDK_K;
	ReadUserData(data);
}

/*----------------------------------------------------------------------------*/
void SaveUserData(struct UserDataStr* data)
{
	printf("in SaveUserData\n");
	GKeyFile* key_file = g_key_file_new();
	GError *error;
	gsize length;
	/* Geometry */
	g_key_file_set_integer(key_file,
						   "Geometry",
						   "x",
						   data->x);
	g_key_file_set_integer(key_file,
						   "Geometry",
						   "y",
						   data->y);
	g_key_file_set_integer(key_file,
						   "Geometry",
						   "width",
						   data->width);
	g_key_file_set_integer(key_file,
						   "Geometry",
						   "height",
						   data->height);
	/* Options */
	g_key_file_set_integer(key_file,
						   "Options",
						   "piecepreview",
						   data->m_PiecePreview);
	g_key_file_set_integer(key_file,
						   "Options",
						   "players",
						   data->m_NumberOfPlayers);
	g_key_file_set_integer(key_file,
						   "Options",
						   "piecestyle",
						   data->m_PieceStyle);
	g_key_file_set_integer(key_file,
						   "Options",
						   "gridstyle",
						   data->m_GridStyle);
	g_key_file_set_integer(key_file,
						   "Options",
						   "sound",
						   data->m_Sound);
	g_key_file_set_string(key_file,
						   "Options",
						   "autoload",
						   data->m_FileName);
						   
	/* View */
	g_key_file_set_integer(key_file,
						   "View",
						   "statusbar",
						   data->m_StatusBar);
	g_key_file_set_integer(key_file,
						   "View",
						   "toolbar",
						   data->m_ToolBar);
	
	/* Controls Player 1 */
	g_key_file_set_integer(key_file,
						   "ControlsPlayer1",
						   "left",
						   data->m_LeftKey1);
	g_key_file_set_integer(key_file,
						   "ControlsPlayer1",
						   "right",
						   data->m_RightKey1);
	g_key_file_set_integer(key_file,
						   "ControlsPlayer1",
						   "drop",
						   data->m_DropKey1);
	g_key_file_set_integer(key_file,
						   "ControlsPlayer1",
						   "rotate",
						   data->m_RotateKey1);
	g_key_file_set_integer(key_file,
						   "ControlsPlayer1",
						   "down",
						   data->m_DownKey1);
	/* Controls Player 2 */
	g_key_file_set_integer(key_file,
						   "ControlsPlayer2",
						   "left",
						   data->m_LeftKey2);
	g_key_file_set_integer(key_file,
						   "ControlsPlayer2",
						   "right",
						   data->m_RightKey2);
	g_key_file_set_integer(key_file,
						   "ControlsPlayer2",
						   "drop",
						   data->m_DropKey2);
	g_key_file_set_integer(key_file,
						   "ControlsPlayer2",
						   "rotate",
						   data->m_RotateKey2);
	g_key_file_set_integer(key_file,
						   "ControlsPlayer2",
						   "down",
						   data->m_DownKey2);
	
	gchar* buf = g_key_file_to_data(key_file,
									&length,
									&error);
	FILE* fd = fopen("stackpack.conf", "w");
	if (fd != NULL)
	{
		fwrite(buf, sizeof(gchar), length, fd);
	}		
	g_free(buf);
	g_key_file_free(key_file);
}
