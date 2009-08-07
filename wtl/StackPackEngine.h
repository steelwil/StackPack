//
//  Copyright (C) 2005 - William Bell 
//
//  This file is part of StackPack
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
//  Coded by William Bell
//  email: william.bell@absamail.co.za
//  Home Page: http://myweb.absamail.co.za/william.bell/
//
// StackPackEngine.h: interface for the CStackPackEngine class.
// This class is thread safe.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define WM_USER_PAINTGAMEGRID1 (WM_USER + 101)
#define WM_USER_PAINTGAMEGRID2 (WM_USER + 102)

// the top left corner=0,0
class CStackPackEngine  
{
public:
	// Construction / Destruction
	CStackPackEngine();
	virtual ~CStackPackEngine();

	void Create(HWND parent,
				UINT msgId,
		        int base, 
		        int height,
				int startingLevel = 1,
				int startingRows = 0,
				bool preview = true,
				int blkTypes=0x1fc,
				int lastLevel = 10);
	// Operations
	bool Start();
	bool EndGame();
	bool Pause();
	bool Left();
	bool Right();
	bool Rotate();
	bool ForceStepNow();
	int  Step();	
	bool Drop();
	void SetPreview(bool prev);
	bool AddRows(int nRows);
	bool IsGameInProgress();
	bool IsGamePaused();

public:
	int m_Level;
	int m_Lines;
	unsigned long m_Score;

	// Access Routines
	unsigned long GetPreview(int* color);
	void GetGrid(char grid[32][32]);

protected:
	bool MovePieceDown();
	void RemovePiece(int row, int col);
	bool PlacePiece(int row, int col);
	void ClearFullRows();
	void CalculateScore();
	void KillThread();

protected:

	char m_Grid[32][32];	// row, columns
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
	bool m_PiecePreview;
	HWND m_Parent;
	UINT m_MessageId;
	bool m_GameOver;
	bool m_Paused;
	int m_LastLevel;

private:
	// Thread stuff
	CComAutoCriticalSection m_cs;
	HANDLE m_hEventRun;
	HANDLE m_hEventKill;
	HANDLE m_hEventDead;
	HANDLE m_hEventStep;
	friend DWORD WINAPI StackPackThread(LPVOID pData);

};
