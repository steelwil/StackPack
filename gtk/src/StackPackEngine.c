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
 
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "StackPackEngine.h"

//static UINT speedInt[]={1375, 1375, 1250, 1125, 875, 750, 625, 375, 250, 125 };
//static UINT speedInt[] = {615, 600, 540, 465, 390, 315, 240, 165, 90, 75 };

static int levelInt[] = {  10,   15,   24,   34,  36,  50,  64,  76,  94, 9999}; 
//static int levelInt[] = {  2,   4,   6,   8,  10,  12,  14,  16,  18, 9999}; 

// pentaShapes[][4] = // T, U, V, W, X, Y, Z, F, I, L, P and N
// max shape = 5x5=block + 2=rotation +5=peice
static unsigned long Shapes[][4] = 
{
	{0x07000000, 0x42100000, 0x07000000, 0x42100000},	// 1 3 block I  Lowest LSB
	{0x03100000, 0x43000000, 0x46000000, 0x06100000},	// 2 3 block L  Low

	{0x07800000, 0x21084000, 0x07800000, 0x21084000},	// 3 4 block I  Lowest
	{0x06180000, 0x02310000, 0x06180000, 0x02310000},	// 4 4 block Z  High
	{0x07200000, 0x42180000, 0x27000000, 0xc2100000},	// 5 4 block L  Low
	{0x01980000, 0x02184000, 0x01980000, 0x02184000},	// 6 4 block S  High
	{0x03840000, 0x31080000, 0x43800000, 0x21180000},	// 7 4 block J  Low
	{0x03180000, 0x03180000, 0x03180000, 0x03180000},	// 8 4 block O  Lowest
	{0x07100000, 0x43100000, 0x47000000, 0x46100000},	// 9 4 block T  Medium

	{0x3b000000, 0x210c2000, 0x1b800000, 0x21842000},	// 10 5 block rN High
	{0x70c00000, 0x11884000, 0x61c00000, 0x108c4000},	// 11 5 block N  High
	{0x03980000, 0x218c0000, 0x33800000, 0x63080000},	// 12 5 block rP Low
	{0x038c0000, 0x31880000, 0x63800000, 0x23180000},	// 13 5 block P  Low
	{0x07840000, 0x31084000, 0x87800000, 0x2108c000},	// 14 5 block rL Low
	{0x03d00000, 0x21086000, 0x0bc00000, 0x61084000},	// 15 5 block L  Low
	{0x23900000, 0x230c0000, 0x13880000, 0x61880000},	// 16 5 block rF High
	{0x43880000, 0x21980000, 0x23840000, 0x33080000},	// 17 5 block F  High
	{0x43840000, 0x31180000, 0x43840000, 0x31180000},	// 18 5 block S  High
	{0x13900000, 0x610c0000, 0x13900000, 0x610c0000},	// 19 5 block Z  High
	{0x27800000, 0x23084000, 0x07900000, 0x210c4000},	// 20 5 block Yl Medium
	{0x07880000, 0x21884000, 0x47800000, 0x21184000},	// 21 5 block Yr Medium
	{0x23880000, 0x23880000, 0x23880000, 0x23880000},	// 22 5 block X  High
	{0x61840000, 0x33100000, 0x430c0000, 0x11980000},	// 23 5 block W  High
	{0x72100000, 0x421c0000, 0x109c0000, 0x70840000},	// 24 5 block V  Low
	{0x43900000, 0x211c0000, 0x13840000, 0x71080000},	// 25 5 block T  Medium	
	{0x53800000, 0x61180000, 0x72800000, 0x310c0000},	// 26 5 block U  Medium	
	{0x003e0000, 0x21084200, 0x003e0000, 0x21084200}	// 27 5 block I  Lowest HSB
};

/* DWORD WINAPI StackPackThread(LPVOID pData)
 * {
 * 	DWORD dwMilliseconds = 0;   
 * 
 * 	CStackPackEngine* pT;
 * 	pT = (CStackPackEngine*) pData;
 * 
 * 	WaitForSingleObject(pT->m_hEventRun, INFINITE);
 * 	// loop but check for kill notification
 * 	while (WaitForSingleObject(pT->m_hEventKill, 0) == WAIT_TIMEOUT)
 * 	{
 * 		WaitForSingleObject(pT->m_hEventRun, INFINITE);
 * 		WaitForSingleObject(pT->m_hEventStep, dwMilliseconds);
 * 		//ATLTRACE("Step:%dms\n", dwMilliseconds);
 * 		pT->Step();
 * //		dwMilliseconds = pT->m_StepDuration - duration;
 * 		dwMilliseconds = pT->m_StepDuration;
 * //		Sleep(pT->m_StepDuration);
 * 	}
 * 	SetEvent(pT->m_hEventDead);
 * 	//ATLTRACE("Exiting the thread\n");
 * 	return 1;
 * }
 */

/*----------------------------------------------------------------------------*/
void GameInitialize(struct GameData* pGameData)
{
	pGameData->m_Score = 0;
	pGameData->m_Lines = 0;
	pGameData->m_Level = 1;
	pGameData->m_StepDuration = 0;
	pGameData->m_GameOver = 1;
	pGameData->m_Paused = 0;
	pGameData->m_Preview = 1;
	memset(pGameData->m_Grid, 0, sizeof(pGameData->m_Grid));
}

/*----------------------------------------------------------------------------*/
void GameCreate(struct GameData* pGameData,
							int wnd,
						    int msgId,
						    int base,  
							int height,
							int startingLevel,
							int startingRows,
							int preview,
							int blkType,
							int lastLevel)
{
	int i;
	
	pGameData->m_MoveCount = 0;
	pGameData->m_DropCount = 0;
	pGameData->m_FullRowCount = 0;
	pGameData->m_FallingRow = -1;
	pGameData->m_FallingCol = 0;
	pGameData->m_FallingPiece = 0;
	pGameData->m_BlkRot = 0;
	pGameData->m_Score = 0;
	pGameData->m_Lines = 0;
	pGameData->m_GameOver = 0;
	pGameData->m_Paused = 0;
	
	pGameData->m_Parent = wnd;
	pGameData->m_MessageId = msgId;
	pGameData->m_Base = base;
	pGameData->m_Height = height;
	pGameData->m_Level = startingLevel;
	pGameData->m_StepDuration = 640-55*pGameData->m_Level;	// speedInt[m_Level-1];
	pGameData->m_PiecePreview = preview;
	pGameData->m_LastLevel = lastLevel;

	memset(&pGameData->m_Grid, 0, sizeof(pGameData->m_Grid));

	pGameData->m_MaxPieces = 0;
	for (i=0; i<32; i++)
	{
		if (blkType&(1<<i))
		{
			memcpy(&pGameData->m_Shapes[pGameData->m_MaxPieces], &Shapes[i], sizeof(long[4]));
			pGameData->m_MaxPieces++;
		}
	}

	AddRows(pGameData, startingRows);

	srand((unsigned)clock());
	pGameData->m_BlkPreviewNum = (int)(rand()/(double)0x7fffffff*pGameData->m_MaxPieces);
	pGameData->m_Preview = pGameData->m_Shapes[pGameData->m_BlkPreviewNum][pGameData->m_BlkRot];
	pGameData->m_Center = (pGameData->m_Base-4)/2;
};

/*----------------------------------------------------------------------------*/
int GameLeft(struct GameData* pGameData)
{
	if (pGameData->m_FallingRow >= 0)
	{
		RemovePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// remove from old position

		if (!PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol-1))	// try to place in new position
		{		
			PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// else restore old position
			return 0;
		}
		pGameData->m_MoveCount++;
		pGameData->m_FallingCol--;
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
int GameRight(struct GameData* pGameData)
{
	if (pGameData->m_FallingRow >= 0)
	{
		RemovePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// remove from old position

		if (!PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol+1))	// try to place in new position
		{		
			PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// else restore old position
			return 0;
		}
		pGameData->m_MoveCount++;
		pGameData->m_FallingCol++;
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
int GameRotate(struct GameData* pGameData)
{
	unsigned long oldPosition;

	if (pGameData->m_FallingRow >= 0)
	{
		RemovePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// remove from old position
		oldPosition = pGameData->m_FallingPiece;
		pGameData->m_BlkRot = (pGameData->m_BlkRot+1)%4;					// rotate anti-clockwise
		pGameData->m_FallingPiece = pGameData->m_Shapes[pGameData->m_BlkNum][pGameData->m_BlkRot];

		if (!PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol))// try to place in new position
		{	
			pGameData->m_BlkRot = (pGameData->m_BlkRot+3)%4;					// rotate anti-clockwise
			pGameData->m_FallingPiece = oldPosition;
			PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// else restore old position
			return 0;
		}
		pGameData->m_MoveCount++;
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
int GameDrop(struct GameData* pGameData)
{
	printf("in GameDrop\n");
	int rtc = 0;
	pGameData->m_DropCount = 0;
	if (pGameData->m_FallingRow >= 0)
	{
		while (pGameData->m_FallingRow >= 0) 
		{
			if (MovePieceDown(pGameData)) pGameData->m_DropCount++;
		}
		rtc = GameClearFullRows(pGameData);
		CalculateScore(pGameData);
	}
	return rtc;
}

/*----------------------------------------------------------------------------*/
int GameForceStepNow(struct GameData* pGameData)
{
	if (pGameData->m_FallingRow >= 0)
		GameStep(pGameData);
	return 1;
}

/*----------------------------------------------------------------------------*/
int GameStep(struct GameData* pGameData)
{
	int rt = 1;		/* 0=game over, 1=0K, 2=new piece, 3=new level */
	if (pGameData->m_FallingRow == -1) 
	{
		rt = 2;
		pGameData->m_FallingCol = pGameData->m_Center;
		pGameData->m_FallingPiece = pGameData->m_Preview;
		pGameData->m_BlkRot = 0;
		pGameData->m_BlkNum = pGameData->m_BlkPreviewNum;
		pGameData->m_BlkPreviewNum = (int)(rand()/(double)0x7fffffff*pGameData->m_MaxPieces);
		pGameData->m_Preview = pGameData->m_Shapes[pGameData->m_BlkPreviewNum][pGameData->m_BlkRot];
		pGameData->m_FallingRow = 0;
		if (!PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol) || pGameData->m_Level == pGameData->m_LastLevel+1)	// try to place
		{
			pGameData->m_GameOver = 1;
			rt = 0;		/* game over */
		}
	}
	else
	{
		if (!MovePieceDown(pGameData))
		{
			pGameData->m_StepDuration = 640-55*pGameData->m_Level;	/* speedInt[m_Level-1]; */
			rt = GameClearFullRows(pGameData);
			CalculateScore(pGameData);
		}
	}
//		Sleep(10000);
	if (pGameData->m_DropCount > 0)
	{
		pGameData->m_DropCount++;
	}
	return rt;
}

/*----------------------------------------------------------------------------*/
int GameClearFullRows(struct GameData* pGameData)
{
	int rc = 1; /* 1=OK, 3=new level, 4=rows cleared,  */
	static int r;
	static int rr;
	static int c;
	static int cc;
	static int fullRow;
	
	for (r=0; r<pGameData->m_Height; r++)		
	{
		// find full rows
		fullRow = r;
		for (c=0; c<pGameData->m_Base; c++)
		{
			if (!pGameData->m_Grid[r][c]) 
			{
				fullRow = -1;
				break;
			}
		}
		// clear full rows
		if (fullRow >= 0)		
		{
			pGameData->m_FullRowCount++;
			for (rr=fullRow; rr>0; rr--)
				for (cc=0; cc<pGameData->m_Base; cc++)
					pGameData->m_Grid[rr][cc] = pGameData->m_Grid[rr-1][cc];
			for (cc=0; cc<pGameData->m_Base; cc++)	//	clear top row
				pGameData->m_Grid[0][cc] = 0;
		}
	}
	pGameData->m_Lines += pGameData->m_FullRowCount;
	// check for new level
	if (pGameData->m_Lines > levelInt[pGameData->m_Level-1])
	{
		pGameData->m_StepDuration = 640-55*pGameData->m_Level;	// speedInt[m_Level-1];
		pGameData->m_Level++;
		rc = 3;
	}
	else if (pGameData->m_FullRowCount > 0)
	{
		rc = 4;
	}
	return rc;
}

/*----------------------------------------------------------------------------*/
void SetPreview(struct GameData* pGameData, int prev)
{
	pGameData->m_PiecePreview = prev;
}

/*----------------------------------------------------------------------------*/
int MovePieceDown(struct GameData* pGameData)
{
	RemovePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// remove from old position

	if (!PlacePiece(pGameData, pGameData->m_FallingRow+1, pGameData->m_FallingCol))	// try to place in new position
	{		
		PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// else restore old position
		pGameData->m_FallingRow = -1;
		return 0;
	}
	pGameData->m_FallingRow = (pGameData->m_FallingRow+1)%pGameData->m_Height;
	return 1;
}

/*----------------------------------------------------------------------------*/
void RemovePiece(struct GameData* pGameData, int row, int col)
{
	static int r;
	static int c;
	static unsigned long mask;

	for (r=0; r<5; r++)
		for (c=0; c<5; c++)
		{
			mask = 0x80000000>>(r*5+c);
			if (pGameData->m_FallingPiece&mask)
				pGameData->m_Grid[r+row][c+col] = 0;
		}
}

/*----------------------------------------------------------------------------*/
int PlacePiece(struct GameData* pGameData, int row, int col)
{
	static int r;
	static int c;
	static unsigned long mask;

	// has the piece hit the bottom?
	if ((row>pGameData->m_Height-5 && pGameData->m_FallingPiece&0x00000f80) ||
		(row>pGameData->m_Height-4 && pGameData->m_FallingPiece&0x0001f000) ||
		(row>pGameData->m_Height-3 && pGameData->m_FallingPiece&0x003e0000) ||
		(row>pGameData->m_Height-2 && pGameData->m_FallingPiece&0x07c00000) ||
		row>pGameData->m_Height)
	{
		return 0;
	}

	// has the piece hit left wall?
	if ((col< 0 && pGameData->m_FallingPiece&0x84210800) ||
		(col<-1 && pGameData->m_FallingPiece&0xc6318c00) ||
		(col<-2 && pGameData->m_FallingPiece&0xe739ce00) ||
		(col<-3 && pGameData->m_FallingPiece&0xf7bdef00) ||
		col<-4)
	{
		return 0;
	}

	// has the piece hit right wall?
	if ((col>pGameData->m_Base-5 && pGameData->m_FallingPiece&0x08421080) ||
		(col>pGameData->m_Base-4 && pGameData->m_FallingPiece&0x18c63180) ||
		(col>pGameData->m_Base-3 && pGameData->m_FallingPiece&0x39ce7380) ||
		(col>pGameData->m_Base-2 && pGameData->m_FallingPiece&0x7bdef780) ||
		col>pGameData->m_Base)
	{
		return 0;
	}

	// has the piece hit another peice?
	for (r=0; r<5; r++)
		for (c=0; c<5; c++)
		{
			mask = 0x80000000>>(r*5+c);
			if (pGameData->m_Grid[r+row][c+col] && pGameData->m_FallingPiece&mask) 
			{
				return 0;
			}
		}

	// move the piece
	for (r=0; r<5; r++)
		for (c=0; c<5; c++)
		{
			mask = 0x80000000>>(r*5+c);
			if (pGameData->m_FallingPiece&mask)
				pGameData->m_Grid[r+row][c+col] = pGameData->m_BlkNum+1;
		}
	return 1;
}

/*----------------------------------------------------------------------------*/
void CalculateScore(struct GameData* pGameData)
{
	long score = pGameData->m_Level*(6-2*pGameData->m_PiecePreview);

	if (score-pGameData->m_MoveCount>6)
		score -= pGameData->m_MoveCount;

	score += pGameData->m_DropCount;

	score += (1<<(pGameData->m_FullRowCount-1)) * 100;

	pGameData->m_Score += score;
	pGameData->m_DropCount = pGameData->m_MoveCount = pGameData->m_FullRowCount = 0;
}

// Access Routines
/*----------------------------------------------------------------------------*/
unsigned long GameGetPreview(struct GameData* pGameData, int* color)
{
	if (pGameData->m_PiecePreview)
	{
		*color = pGameData->m_BlkPreviewNum;
		return pGameData->m_Preview;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
void GameGetGrid(struct GameData* pGameData, char grid[32][32])
{
	memcpy(grid, pGameData->m_Grid, sizeof(pGameData->m_Grid));
}

/*----------------------------------------------------------------------------*/
int GameStart(struct GameData* pGameData)
{
	if (!pGameData->m_GameOver)
	{
		pGameData->m_Paused = 0;
	}
	return 1;
}

/*----------------------------------------------------------------------------*/
int GamePause(struct GameData* pGameData)
{
	pGameData->m_Paused = 1;
	return pGameData->m_Paused;
}

/*----------------------------------------------------------------------------*/
int AddRows(struct GameData* pGameData, int nRows)
{
	int i,r,c;
	for (i=0; i<nRows; i++)
	{
		// is the top row free?
		if (pGameData->m_FallingRow >= 0)
			RemovePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);
		for (c=0; c<pGameData->m_Base; c++)
		{
			if (pGameData->m_Grid[0][c]) 
			{
				return 0;
			}
		}
		// move the whole grid one up
		for (r=0; r<pGameData->m_Height-1; r++)
			for (c=0; c<pGameData->m_Base; c++)
				pGameData->m_Grid[r][c] = pGameData->m_Grid[r+1][c];
		// add random pieces to the bottom
		for (c=0; c<pGameData->m_Base; c++)
			pGameData->m_Grid[pGameData->m_Height-1][c] = rand()%pGameData->m_MaxPieces+1;
		// add about 30% spaces to the rows
		pGameData->m_Grid[pGameData->m_Height-1][rand()%pGameData->m_Base] = 0;
		for (c=0; c<pGameData->m_Base; c++)
			if (rand()%10 < 3)
				pGameData->m_Grid[pGameData->m_Height-1][rand()%pGameData->m_Base] = 0;
		// take care of the falling piece
		if (pGameData->m_FallingRow > 0)
		{
			pGameData->m_FallingRow--;
			GameStep(pGameData);
		}
		if (pGameData->m_FallingRow == 0)
		{
			if (!PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol))	// try to place
			{
				pGameData->m_GameOver = 1;
				pGameData->m_FallingRow = -1;
				return 0;
			}
		}
	}
	return 1;
}

/*----------------------------------------------------------------------------*/
int EndGame(struct GameData* pGameData)
{
	pGameData->m_GameOver = 1;
	return pGameData->m_GameOver;
}

/*----------------------------------------------------------------------------*/
int IsGameInProgress(struct GameData* pGameData)
{
	return !pGameData->m_GameOver;
}

/*----------------------------------------------------------------------------*/
int IsGamePaused(struct GameData* pGameData)
{
	return pGameData->m_Paused;
}

/*----------------------------------------------------------------------------*/
