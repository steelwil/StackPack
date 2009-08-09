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
 * defines game specific data
 */
 
#define cMAXPLAYERS 2

struct HighScoreRecord
{
	int score;
	int rows;
	int level;
	unsigned long date;		/* date yyyy*10000 + mm*100 + dd */
	char name[32];
	char quote[32];
};

struct GameDataStr
{
	int m_Base;
	int m_Height;
	int m_BlkType;
	int m_StartingLevel;
	int m_StartingRows;
	int m_LastLevel;	 // Maximum level of this game
	int m_EndLevel;	 // current last level of this game m_EndLevel <= m_LastLevel
	int m_Spare1;
	int m_Penalize;
	char m_Comment[32];
	struct HighScoreRecord m_HighScore[10];
};

void InitializeGameData(struct GameDataStr* data, const char* filename);
int IsHighScore(struct GameDataStr* data, int score);
int AddHighScore(struct GameDataStr* data, const char* name, const char* quote, int score, int rows, int level);
void SaveGameData(struct GameDataStr* data, const gchar *filename);
