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

/*
 * defines user specific data
 */

struct UserDataStr
{
	int x;
	int y;
	int width;
	int height;
	int m_ToolBar;
	int m_StatusBar;
	int m_Sound;
	int m_PieceStyle;
	char m_PiecePreview;
	int m_GridStyle;
	int m_NumberOfPlayers;

	char m_FileName[128];
	char m_MRUFile[4][128];

	unsigned int m_RotateKey2;
	unsigned int m_RotateKey1;
	unsigned int m_RightKey2;
	unsigned int m_RightKey1;
	unsigned int m_LeftKey2;
	unsigned int m_LeftKey1;
	unsigned int m_DropKey2;
	unsigned int m_DropKey1;
	unsigned int m_DownKey1;
	unsigned int m_DownKey2;
};

void InitializeUserData(struct UserDataStr* data);
void ReadUserData(struct UserDataStr* data);
void SaveUserData(struct UserDataStr* data);
