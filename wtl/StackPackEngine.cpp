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
// StackPackEngine.cpp: implementation of the CStackPackEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StackPackEngine.h"

extern "C"
{
	long random(void);
	void srandom(unsigned long x);
}

//static UINT speedInt[]={1375, 1375, 1250, 1125, 875, 750, 625, 375, 250, 125 };
//static UINT speedInt[] = {615, 600, 540, 465, 390, 315, 240, 165, 90, 75 };
static int levelInt[] = {  10,   15,   24,   34,  36,  50,  64,  76,  94, 9999}; 

// pentaShapes[][4] = // T, U, V, W, X, Y, Z, F, I, L, P and N
// max shape = 5x5=block + 2=rotation +5=peice
static unsigned long Shapes[][4] = 
{
	0x07000000,	0x42100000,	0x07000000,	0x42100000,	// 3 block I  Lowest LSB
	0x03100000,	0x43000000,	0x46000000,	0x06100000,	// 3 block L  Low

	0x07800000, 0x21084000, 0x07800000, 0x21084000,	// 4 block I  Lowest
	0x06180000,	0x02310000,	0x06180000,	0x02310000,	// 4 block Z  High
	0x07200000,	0x42180000,	0x27000000,	0xc2100000,	// 4 block L  Low
	0x01980000,	0x02184000,	0x01980000,	0x02184000,	// 4 block S  High
	0x03840000,	0x31080000,	0x43800000,	0x21180000,	// 4 block J  Low
	0x03180000,	0x03180000,	0x03180000,	0x03180000,	// 4 block O  Lowest
	0x07100000,	0x43100000,	0x47000000,	0x46100000,	// 4 block T  Medium

	0x3b000000, 0x210c2000,	0x1b800000,	0x21842000,	// 5 block rN High
	0x70c00000, 0x11884000,	0x61c00000,	0x108c4000,	// 5 block N  High
	0x03980000, 0x218c0000,	0x33800000,	0x63080000,	// 5 block rP Low
	0x038c0000, 0x31880000,	0x63800000,	0x23180000,	// 5 block P  Low
	0x07840000, 0x31084000,	0x87800000,	0x2108c000,	// 5 block rL Low
	0x03d00000, 0x21086000,	0x0bc00000,	0x61084000,	// 5 block L  Low
	0x23900000,	0x230c0000,	0x13880000,	0x61880000,	// 5 block rF High
	0x43880000,	0x21980000,	0x23840000,	0x33080000,	// 5 block F  High
	0x43840000,	0x31180000,	0x43840000,	0x31180000,	// 5 block S  High
	0x13900000,	0x610c0000,	0x13900000,	0x610c0000,	// 5 block Z  High
	0x27800000,	0x23084000,	0x07900000,	0x210c4000,	// 5 block Yl Medium
	0x07880000,	0x21884000,	0x47800000,	0x21184000,	// 5 block Yr Medium
	0x23880000,	0x23880000,	0x23880000,	0x23880000,	// 5 block X  High
	0x61840000,	0x33100000,	0x430c0000,	0x11980000,	// 5 block W  High
	0x72100000,	0x421c0000,	0x109c0000,	0x70840000,	// 5 block V  Low
	0x43900000,	0x211c0000,	0x13840000,	0x71080000,	// 5 block T  Medium	
	0x53800000,	0x61180000,	0x72800000,	0x310c0000,	// 5 block U  Medium	
	0x003e0000,	0x21084200,	0x003e0000,	0x21084200	// 5 block I  Lowest HSB
};

DWORD WINAPI StackPackThread(LPVOID pData)
{
	DWORD dwMilliseconds = 0;   

	CStackPackEngine* pT;
	pT = (CStackPackEngine*) pData;

	WaitForSingleObject(pT->m_hEventRun, INFINITE);
	// loop but check for kill notification
	while (WaitForSingleObject(pT->m_hEventKill, 0) == WAIT_TIMEOUT)
	{
		WaitForSingleObject(pT->m_hEventRun, INFINITE);
		WaitForSingleObject(pT->m_hEventStep, dwMilliseconds);
		//ATLTRACE("Step:%dms\n", dwMilliseconds);
		pT->Step();
//		dwMilliseconds = pT->m_StepDuration - duration;
		dwMilliseconds = pT->m_StepDuration;
//		Sleep(pT->m_StepDuration);
	}
	SetEvent(pT->m_hEventDead);
	//ATLTRACE("Exiting the thread\n");
	return 1;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStackPackEngine::CStackPackEngine()
{
	m_hEventRun = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventStep = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_Score = 0;
	m_Lines = 0;
	m_Level = 1;
	m_StepDuration = 0;
	m_GameOver = true;
	m_Paused = false;
	m_Preview = 0;
	memset(m_Grid, 0, sizeof(m_Grid));
	DWORD ThreadId;
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, StackPackThread, this, 0, &ThreadId);
}

CStackPackEngine::~CStackPackEngine()
{
	SetEvent(m_hEventKill);
	SetEvent(m_hEventRun);
	WaitForSingleObject(m_hEventDead, m_StepDuration+500);
	Sleep(200);			//If this is not here there are memory leaks???
	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
	CloseHandle(m_hEventRun);
	CloseHandle(m_hEventStep);
}

void CStackPackEngine::Create(	HWND wnd,
						    UINT msgId,
						    int base,  
							int height,
							int startingLevel,
							int startingRows,
							bool preview,
							int blkType,
							int lastLevel)
{
//	m_cs.Lock();
	m_MoveCount = 0;
	m_DropCount = 0;
	m_FullRowCount = 0;
	m_FallingRow = -1;
	m_FallingCol = 0;
	m_FallingPiece = 0;
	m_BlkRot = 0;
	m_Score = 0;
	m_Lines = 0;
	m_GameOver = false;
	m_Paused = false;
	
	m_Parent = wnd;
	m_MessageId = msgId;
	m_Base = base;
	m_Height = height;
	m_Level = startingLevel;
	m_StepDuration = 640-55*m_Level;	// speedInt[m_Level-1];
	m_PiecePreview = preview;
	m_LastLevel = lastLevel;

	memset(m_Grid, 0, sizeof(m_Grid));

	m_MaxPieces = 0;
	for (int i=0; i<32; i++)
		if (blkType&(1<<i))
		{
			memcpy(&m_Shapes[m_MaxPieces], &Shapes[i], sizeof(long[4]));
			m_MaxPieces++;
		}

	AddRows(startingRows);

	srandom((unsigned)GetTickCount());
	m_BlkPreviewNum = (int)(random()/(double)0x7fffffff*m_MaxPieces);
	m_Preview = m_Shapes[m_BlkPreviewNum][m_BlkRot];
	m_Center = (m_Base-4)/2;
//	m_cs.Unlock();
}

bool CStackPackEngine::Left()
{
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		if (m_FallingRow >= 0)
		{
			m_cs.Lock();
			RemovePiece(m_FallingRow, m_FallingCol);	// remove from old position

			if (!PlacePiece(m_FallingRow, m_FallingCol-1))	// try to place in new position
			{		
				PlacePiece(m_FallingRow, m_FallingCol);	// else restore old position
				m_cs.Unlock();
				return false;
			}
			m_MoveCount++;
			m_FallingCol--;
			m_cs.Unlock();
			::PostMessage(m_Parent, m_MessageId, 0, 0);
			return true;
		}
	}
	return false;
}

bool CStackPackEngine::Right()
{
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		if (m_FallingRow >= 0)
		{
			m_cs.Lock();
			RemovePiece(m_FallingRow, m_FallingCol);	// remove from old position

			if (!PlacePiece(m_FallingRow, m_FallingCol+1))	// try to place in new position
			{		
				PlacePiece(m_FallingRow, m_FallingCol);	// else restore old position
				m_cs.Unlock();
				return false;
			}
			m_MoveCount++;
			m_FallingCol++;
			m_cs.Unlock();
			::PostMessage(m_Parent, m_MessageId, 0, 0);
			return true;
		}
	}
	return false;
}

bool CStackPackEngine::Rotate()
{
	unsigned long oldPosition;

	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
		if (m_FallingRow >= 0)
		{
			m_cs.Lock();
			RemovePiece(m_FallingRow, m_FallingCol);	// remove from old position
			oldPosition = m_FallingPiece;
			m_BlkRot = (m_BlkRot+1)%4;					// rotate anti-clockwise
			m_FallingPiece = m_Shapes[m_BlkNum][m_BlkRot];

			if (!PlacePiece(m_FallingRow, m_FallingCol))// try to place in new position
			{	
				m_BlkRot = (m_BlkRot+3)%4;					// rotate anti-clockwise
				m_FallingPiece = oldPosition;
				PlacePiece(m_FallingRow, m_FallingCol);	// else restore old position
				m_cs.Unlock();
				return false;
			}
			m_MoveCount++;
			m_cs.Unlock();
			::PostMessage(m_Parent, m_MessageId, 0, 0);
			return true;
		}
	return false;
}

/*	m_DropCount = 0;
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		if (m_FallingRow >= 0)
		{
			m_cs.Lock();
			while (m_FallingRow >= 0) 
			{
				SetEvent(m_hEventStep);
				if (MovePieceDown()) m_DropCount++;
			}
			ClearFullRows();
			CalculateScore();
			m_cs.Unlock();
			::PostMessage(m_Parent, m_MessageId, 3, 0);
		}
	}
	return m_DropCount;
*/

bool CStackPackEngine::Drop()
{
	m_DropCount = 0;
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		if (m_FallingRow >= 0)
		{
			m_DropCount = 1;
			SetEvent(m_hEventStep);
		}
	}
	return true;
}

bool CStackPackEngine::ForceStepNow()
{
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		if (m_FallingRow >= 0)
			SetEvent(m_hEventStep);
	}
	return true;
}

int CStackPackEngine::Step()
{
	int rt = 1;		// 0=game over, 1=0K, 2=new peice, 3=game over
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		m_cs.Lock();
		if (m_FallingRow == -1) 
		{
			rt = 2;
			m_FallingCol = m_Center;
			m_FallingPiece = m_Preview;
			m_BlkRot = 0;
			m_BlkNum = m_BlkPreviewNum;
			m_BlkPreviewNum = (int)(random()/(double)0x7fffffff*m_MaxPieces);
			m_Preview = m_Shapes[m_BlkPreviewNum][m_BlkRot];
			::PostMessage(m_Parent, m_MessageId, 3, 0);
			m_FallingRow = 0;
			if (!PlacePiece(m_FallingRow, m_FallingCol) || m_Level == m_LastLevel+1)	// try to place
			{
				ResetEvent(m_hEventRun);
				m_GameOver = true;
				rt = 0;		// game over
			}
		}
		else
		{
			if (!MovePieceDown())
			{
				m_StepDuration = 640-55*m_Level;	// speedInt[m_Level-1];
				ClearFullRows();
				CalculateScore();
			}
		}
//		Sleep(10000);
		m_cs.Unlock();
		if (rt == 0)	// game over
			::PostMessage(m_Parent, m_MessageId, 1, 0);
		else if (m_DropCount == 0)
		{
			if (rt == 1)
			{
				if (WAIT_OBJECT_0 == WaitForSingleObject(m_hEventStep, 0))
					ResetEvent(m_hEventStep);
				::PostMessage(m_Parent, m_MessageId, 0, 0);
			}
			else // (rt==2)	// new peice
				::PostMessage(m_Parent, m_MessageId, 3, 0);
		}
		else
			m_DropCount++;
	}
	return rt;
}

void CStackPackEngine::ClearFullRows()
{
	static int r;
	static int rr;
	static int c;
	static int cc;
	static int fullRow;
	
	m_cs.Lock();
	for (r=0; r<m_Height; r++)		
	{
		// find full rows
		fullRow = r;
		for (c=0; c<m_Base; c++)
		{
			if (!m_Grid[r][c]) 
			{
				fullRow = -1;
				break;
			}
		}
		// clear full rows
		if (fullRow >= 0)		
		{
			m_FullRowCount++;
			for (rr=fullRow; rr>0; rr--)
				for (cc=0; cc<m_Base; cc++)
					m_Grid[rr][cc] = m_Grid[rr-1][cc];
			for (cc=0; cc<m_Base; cc++)	//	clear top row
				m_Grid[0][cc] = 0;
		}
	}
	m_Lines += m_FullRowCount;
	// check for new level
	if (m_Lines > levelInt[m_Level-1])
	{
		m_StepDuration = 640-55*m_Level;	// speedInt[m_Level-1];
		m_Level++;
		::PostMessage(m_Parent, m_MessageId, 2, 0);
	}
	else if (m_FullRowCount > 0)
		::PostMessage(m_Parent, m_MessageId, 4, m_FullRowCount);
	m_cs.Unlock();
}

void CStackPackEngine::SetPreview(bool prev)
{
	m_PiecePreview = prev;
}

bool CStackPackEngine::MovePieceDown()
{
	m_cs.Lock();
	RemovePiece(m_FallingRow, m_FallingCol);	// remove from old position

	if (!PlacePiece(m_FallingRow+1, m_FallingCol))	// try to place in new position
	{		
		PlacePiece(m_FallingRow, m_FallingCol);	// else restore old position
		m_FallingRow = -1;
		m_cs.Unlock();
		return false;
	}
	m_FallingRow = (m_FallingRow+1)%m_Height;
	m_cs.Unlock();
	return true;
}

void CStackPackEngine::RemovePiece(int row, int col)
{
	static int r;
	static int c;
	static unsigned long mask;

	m_cs.Lock();
	for (r=0; r<5; r++)
		for (c=0; c<5; c++)
		{
			mask = 0x80000000>>(r*5+c);
			if (m_FallingPiece&mask)
				m_Grid[r+row][c+col] = 0;
		}
	m_cs.Unlock();
}

bool CStackPackEngine::PlacePiece(int row, int col)
{
	static int r;
	static int c;
	static unsigned long mask;

	m_cs.Lock();
	// has the peice hit the bottom?
	if (row>m_Height-5 && m_FallingPiece&0x00000f80 ||
		row>m_Height-4 && m_FallingPiece&0x0001f000 ||
		row>m_Height-3 && m_FallingPiece&0x003e0000 ||
		row>m_Height-2 && m_FallingPiece&0x07c00000 ||
		row>m_Height)
	{
		m_cs.Unlock();
		return false;
	}

	// has the peice hit left wall?
	if (col< 0 && m_FallingPiece&0x84210800 ||
		col<-1 && m_FallingPiece&0xc6318c00 ||
		col<-2 && m_FallingPiece&0xe739ce00 ||
		col<-3 && m_FallingPiece&0xf7bdef00 ||
		col<-4)
	{
		m_cs.Unlock();
		return false;
	}

	// has the peice hit right wall?
	if (col>m_Base-5 && m_FallingPiece&0x08421080 ||
		col>m_Base-4 && m_FallingPiece&0x18c63180 ||
		col>m_Base-3 && m_FallingPiece&0x39ce7380 ||
		col>m_Base-2 && m_FallingPiece&0x7bdef780 ||
		col>m_Base)
	{
		m_cs.Unlock();
		return false;
	}

	// has the peice hit another peice?
	for (r=0; r<5; r++)
		for (c=0; c<5; c++)
		{
			mask = 0x80000000>>(r*5+c);
			if (m_Grid[r+row][c+col] && m_FallingPiece&mask) 
			{
				m_cs.Unlock();
				return false;
			}
		}

	// move the peice
	for (r=0; r<5; r++)
		for (c=0; c<5; c++)
		{
			mask = 0x80000000>>(r*5+c);
			if (m_FallingPiece&mask)
				m_Grid[r+row][c+col] = char(m_BlkNum+1);
		}
	m_cs.Unlock();
	return true;
}

void CStackPackEngine::CalculateScore()
{
	long score = m_Level*(6-2*m_PiecePreview);

	if (score-m_MoveCount>6)
		score -= m_MoveCount;

	score += m_DropCount;

	score += (1<<(m_FullRowCount-1)) * 100;

	m_Score += score;
	m_DropCount = m_MoveCount = m_FullRowCount = 0;
}

// Access Routines
unsigned long CStackPackEngine::GetPreview(int* color)
{
	if (m_PiecePreview)
	{
		*color = m_BlkPreviewNum;
		return m_Preview;
	}
	return 0;
}

void CStackPackEngine::GetGrid(char grid[32][32])
{
	m_cs.Lock();
	memcpy(grid, m_Grid, sizeof(m_Grid));
	m_cs.Unlock();
}

bool CStackPackEngine::Start()
{
	if (!m_GameOver)
	{
		SetEvent(m_hEventRun);
		m_Paused = false;
	}
	::PostMessage(m_Parent, m_MessageId, 3, 0);
	return true;
}

bool CStackPackEngine::Pause()
{
	ResetEvent(m_hEventRun);
	m_Paused = true;
	return m_Paused;
}

bool CStackPackEngine::AddRows(int nRows)
{
	m_cs.Lock();
	for (int i=0; i<nRows; i++)
	{
		// is the top row free?
		if (m_FallingRow >= 0)
			RemovePiece(m_FallingRow, m_FallingCol);
		for (int c=0; c<m_Base; c++)
			if (m_Grid[0][c]) 
			{
				m_cs.Unlock();
				return false;
			}
		// move the whole grid one up
		for (int r=0; r<m_Height-1; r++)
			for (c=0; c<m_Base; c++)
				m_Grid[r][c] = m_Grid[r+1][c];
		// add random pieces to the bottom
		for (c=0; c<m_Base; c++)
			m_Grid[m_Height-1][c] = char(random()%m_MaxPieces+1);
		// add about 30% spaces to the rows
		m_Grid[m_Height-1][random()%m_Base] = 0;
		for (c=0; c<m_Base; c++)
			if (random()%10 < 3)
				m_Grid[m_Height-1][random()%m_Base] = 0;
		// take care of the falling piece
		if (m_FallingRow > 0)
		{
			m_FallingRow--;
			Step();
		}
		if (m_FallingRow == 0)
		{
			if (!PlacePiece(m_FallingRow, m_FallingCol))	// try to place
			{
				ResetEvent(m_hEventRun);
				m_GameOver = true;
				m_FallingRow = -1;
				m_cs.Unlock();
				::PostMessage(m_Parent, m_MessageId, 1, 0);
				return false;
			}
		}
	}
	m_cs.Unlock();
	return true;
}

bool CStackPackEngine::EndGame()
{
	ResetEvent(m_hEventRun);
	m_GameOver = true;
	return m_GameOver;
}

bool CStackPackEngine::IsGameInProgress()
{
	return !m_GameOver;
}

bool CStackPackEngine::IsGamePaused()
{
	return m_Paused;
}
	
