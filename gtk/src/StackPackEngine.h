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

#pragma once

#define WM_USER_PAINTGAMEGRID1 (101)
#define WM_USER_PAINTGAMEGRID2 (102)

struct GameData
{
	int m_Level;
	int m_Lines;
	unsigned long m_Score;

	char m_Grid[32][32];	/* row, columns */
	unsigned long m_Shapes[32][4];
	unsigned long m_Preview;

	unsigned long m_FallingPiece;
	int m_FallingRow;
	int m_FallingCol;
	int m_MaxPieces;
	int m_Base;
	int m_Height;
	int m_MoveCount;
	int m_DropCount;
	int m_FullRowCount;
	int m_Center;
	int m_BlkNum;
	int m_BlkPreviewNum;
	int m_BlkRot;
	int m_StepDuration;
	int m_PiecePreview;
	int m_Parent;
	int m_MessageId;
	int m_GameOver;
	int m_Paused;
	int m_LastLevel;
};


/* the top left corner=0,0 */
void GameInitialize(struct GameData* pGameData);
void GameCreate(struct GameData* pGameData,
				int parent,
				int msgId,
		        int base, 
		        int height,
				int startingLevel,
				int startingRows,
				int preview,
				int blkTypes,
				int lastLevel);
/* Operations */
int GameStart(struct GameData* pGameData);
int EndGame(struct GameData* pGameData);
int GamePause(struct GameData* pGameData);
int GameLeft(struct GameData* pGameData);
int GameRight(struct GameData* pGameData);
int GameRotate(struct GameData* pGameData);
int GameForceStepNow(struct GameData* pGameData);
int GameStep(struct GameData* pGameData);	
int GameDrop(struct GameData* pGameData);
void SetPreview(struct GameData* pGameData, int prev);
int AddRows(struct GameData* pGameData, int nRows);
int IsGameInProgress(struct GameData* pGameData);
int IsGamePaused(struct GameData* pGameData);

/* Access Routines */
unsigned long GameGetPreview(struct GameData* pGameData, int* color);
void GameGetGrid(struct GameData* pGameData, char grid[32][32]);

/* Movement routines */
int MovePieceDown(struct GameData* pGameData);
void RemovePiece(struct GameData* pGameData, int row, int col);
int PlacePiece(struct GameData* pGameData, int row, int col);
int GameClearFullRows(struct GameData* pGameData);
void CalculateScore(struct GameData* pGameData);
void KillThread(struct GameData* pGameData);
