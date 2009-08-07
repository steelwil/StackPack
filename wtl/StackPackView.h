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
// StackPackView.h : interface of the CStackPackView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"
#include "EnterHighScore.h"

class CStackPackView : public CWindowImpl<CStackPackView>
{
public:
	CStackPackDoc m_Doc;
	CStackPackEngine m_StackPackEngine[cMAXPLAYERS];

	CBitmap m_Screens[10];
	CRect m_RectClient;
	CDC m_dcBMP;
	CRgn m_PlayArea[cMAXPLAYERS];
	CRect m_PlayRect[cMAXPLAYERS];
	CRgn m_ScoreArea[cMAXPLAYERS];
	CRect m_ScoreRect[cMAXPLAYERS];

	CBrush m_Brush;
	COLORREF m_ColorRef[15][3];

private:
	void GetPlayAreaCoords();
	void PaintGrid(CDC& hDC, int p);
	void PaintScore(CDC& hDC, int p);
	inline void PaintFlatBlock(CDC& hDC, char block, RECT& rect);
	inline void Paint3DBlock(CDC& hDC, char block, RECT& rect);
	inline void PaintPyrimidBlock(CDC& hDC, char block, RECT& rect);
	inline void PaintBackBlock(CDC& hDC, RECT& rect);
	CBitmap m_bmpPrevFrame;
public:

	BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_WND_CLASS(NULL)

	BEGIN_MSG_MAP(CStackPackView)
		MESSAGE_RANGE_HANDLER(WM_USER_PAINTGAMEGRID1, WM_USER_PAINTGAMEGRID2, MessageFromStackPackEngine)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		//MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT MessageFromStackPackEngine(UINT uMsg, WPARAM id, LPARAM nRows, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
};
//=============================================================================

//-----------------------------------------------------------------------------
COLORREF GetColorRef(int blk, int shade)
{
	blk = 32 - blk;
	int multiplier = (blk>>2&0x4^0x4)+4;	// = 8 or 4
	multiplier -= shade;
	COLORREF col = (0x1f*multiplier)*(blk&1) |
				   (0x1f00*multiplier)*(blk>>1&1) |
				   (0x1f0000*multiplier)*(blk>>2&1);
	return col;

}

//-----------------------------------------------------------------------------
BOOL CStackPackView::PreTranslateMessage(MSG* /*pMsg*/)
{
	//pMsg;
	return FALSE;
}

//-----------------------------------------------------------------------------
LRESULT CStackPackView::MessageFromStackPackEngine(UINT uMsg, WPARAM id, LPARAM nRows, BOOL& /*bHandled*/)
{
	int p = uMsg-WM_USER_PAINTGAMEGRID1;

	CDC dc = GetDC();
	switch (id)
	{
	case 0:				// update grid painting
		PaintGrid(dc, p);
		break;
	case 1:				// game over
		// In a multi player game we must pause the game and then 
		// do this stuff once complete the other player can carry on
		if (m_Doc.m_NumberOfPlayers > 1)
		{
			::PostMessage(GetParent(), WM_COMMAND, ID_GAME_PAUSEF3, 1);
			char msg[32];
			wsprintf(msg, "Player %d your game is over.", p+1);
			MessageBox(msg, "Game Over", MB_ICONASTERISK);
		}
		else
		{
			MessageBox("Your game is over.", "Game Over", MB_ICONASTERISK);
		}
		if (m_Doc.IsHighScore(m_StackPackEngine[p].m_Score))
		{
			CEnterHighScore dlg;
			dlg.DoModal();
			m_Doc.AddHighScore(	dlg.m_Name, dlg.m_Quote, 
								m_StackPackEngine[p].m_Score,
								m_StackPackEngine[p].m_Lines,
								m_StackPackEngine[p].m_Level);
			::PostMessage(GetParent(), WM_COMMAND, ID_GAME_HIGHSCORES, NULL);
		}
		break;
	case 2:				// next level
		if (m_Doc.m_Sound) MessageBeep(MB_OK);
		Invalidate(TRUE);
		break;
	case 3:				// next piece
		PaintScore(dc, p);
		PaintGrid(dc, p);
		break;
	case 4:				// cleared lines 
		// do the sound
		if (m_Doc.m_Sound) MessageBeep(MB_OK);
		if (m_Doc.m_Game.m_Penalize && nRows > 1)
			for (int n=0; n<m_Doc.m_NumberOfPlayers; n++)
				if (n != p) m_StackPackEngine[n].AddRows(nRows);
		break;
	}
	return 0;
}

//-----------------------------------------------------------------------------
LRESULT CStackPackView::OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//ATLTRACE("CStackPackView::OnEraseBkgnd\n");
	CRect rect;
	GetClientRect(&m_RectClient);
	if (m_RectClient.right <= 0) return 0;
	
	int w = m_RectClient.right - m_RectClient.left;
	int h = m_RectClient.bottom - m_RectClient.top;
	int c = (w + 31)/32;
	int r = (h + 31)/32;

	CDCHandle dc = (HDC)wParam;
	CDC dcMem;
	CDC dcTile;

	if(!m_bmpPrevFrame.IsNull())
		m_bmpPrevFrame.DeleteObject();
	m_bmpPrevFrame.CreateCompatibleBitmap(dc, m_RectClient.Width(), m_RectClient.Height());

	if (m_PlayArea[0].m_hRgn == NULL)
		for (int p=0; p<cMAXPLAYERS; p++)
			m_PlayArea[p].CreateRectRgn(0, 0, 1, 1);

	// create the tiled background and put it in m_bmpPrevFrame
	dcTile.CreateCompatibleDC(dc);
	HBITMAP hOlddcTile = dcTile.SelectBitmap(m_Screens[m_StackPackEngine[0].m_Level-1].m_hBitmap);
	if (!hOlddcTile) return 0;

	dcMem.CreateCompatibleDC(dc);
	HBITMAP hOlddcMem = dcMem.SelectBitmap(m_bmpPrevFrame);
	for (int x=0; x<c; x++)
		for (int y=0; y<r; y++)
			dcMem.BitBlt(32*x, 32*y, 32, 32, dcTile, 0, 0, SRCCOPY);

	GetPlayAreaCoords();
	for (int p=0; p<m_Doc.m_NumberOfPlayers; p++)
	{
		if (m_ScoreArea[p].m_hRgn)
			m_ScoreArea[p].DeleteObject();
		m_ScoreArea[p].CreateRectRgnIndirect(m_ScoreRect[p]);

		if (m_PlayArea[p].m_hRgn)
			m_PlayArea[p].DeleteObject();
		m_PlayArea[p].CreateRectRgn(m_PlayRect[p].left-5, m_PlayRect[p].top-5, 
									m_PlayRect[p].right+5, m_PlayRect[p].bottom+5);
	}

	//dc.BitBlt(0, 0, m_RectClient.Width(), m_RectClient.Height(), dcMem, 0, 0, SRCCOPY);

	dcMem.SelectBitmap(hOlddcMem);
	dcTile.SelectBitmap(hOlddcTile);

	return 0;
}

//-----------------------------------------------------------------------------
LRESULT CStackPackView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//ATLTRACE("CStackPackView::OnPaint\n");
	CPaintDC dc(m_hWnd);
	//TODO: Add your drawing code here
	CDC dcMem;
	dcMem.CreateCompatibleDC(dc);
	HBITMAP hOlddcMem = dcMem.SelectBitmap(m_bmpPrevFrame);

	for (int p=0; p<m_Doc.m_NumberOfPlayers; p++)
	{
		//dcMem.SelectBrush(m_lightgrey);
		dcMem.SelectBrush((HBRUSH)::GetStockObject(LTGRAY_BRUSH));
		dcMem.SetBkColor(0x00c0c0c0);
		dcMem.PaintRgn(m_ScoreArea[p]);
		dcMem.FrameRgn(m_ScoreArea[p].m_hRgn, (HBRUSH)::GetStockObject(GRAY_BRUSH), 2, 2 );
		PaintScore(dcMem, p);

		//dcMem.SelectBrush(m_black);
		dcMem.SelectBrush((HBRUSH)::GetStockObject(BLACK_BRUSH));
		//dcMem.SelectStockBrush(BLACK_BRUSH);
		dcMem.PaintRgn(m_PlayArea[p]);
		dcMem.FrameRgn(m_PlayArea[p], (HBRUSH)::GetStockObject(GRAY_BRUSH), 5, 5 );
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, 0x00303030);
		dcMem.SelectPen(pen);
		if ((m_Doc.m_GridStyle&1) == 1) // vertical
		{
			for (int c=0; c<m_Doc.m_Game.m_Base+1; c++)
			{
				int h = m_PlayRect[p].bottom - m_PlayRect[p].top;
				int bs = (h-1)/m_Doc.m_Game.m_Height;
				dcMem.MoveTo(m_PlayRect[p].left+c*bs, m_PlayRect[p].top);
				dcMem.LineTo(m_PlayRect[p].left+c*bs, m_PlayRect[p].bottom);
			}
		}
		if ((m_Doc.m_GridStyle&2) == 2) // horizontal
		{
			for (int r=0; r<m_Doc.m_Game.m_Height+1; r++)
			{
				int h = m_PlayRect[p].bottom - m_PlayRect[p].top;
				int bs = (h-1)/m_Doc.m_Game.m_Height;
				dcMem.MoveTo(m_PlayRect[p].left, m_PlayRect[p].top+r*bs);
				dcMem.LineTo(m_PlayRect[p].right, m_PlayRect[p].top+r*bs);
			}
		}
		PaintGrid(dcMem, p);
	}
	dc.BitBlt(0, 0, m_RectClient.Width(), m_RectClient.Height(), dcMem, 0, 0, SRCCOPY);
	dcMem.SelectBitmap(hOlddcMem);

	return 0;
}
//-----------------------------------------------------------------------------
void CStackPackView::GetPlayAreaCoords()
{
	int w = m_RectClient.right - m_RectClient.left;
	int h = m_RectClient.bottom - m_RectClient.top;
	int blockWidth = int((h*0.9-1)/m_Doc.m_Game.m_Height);
	int gridWidth = blockWidth*m_Doc.m_Game.m_Base + 1;
	int gridInterval = (int)((w-m_Doc.m_NumberOfPlayers*(gridWidth+100))/(m_Doc.m_NumberOfPlayers+1));
	if (gridInterval < 10)
	{
		blockWidth = int((w/m_Doc.m_NumberOfPlayers-110)/m_Doc.m_Game.m_Base);
		gridWidth = blockWidth*m_Doc.m_Game.m_Base + 1;
		gridInterval = (int)((w-m_Doc.m_NumberOfPlayers*(gridWidth+100))/(m_Doc.m_NumberOfPlayers+1));
	}
	for (int p=0; p<m_Doc.m_NumberOfPlayers; p++)
	{
		m_PlayRect[p].top = (long)(h*0.05);
		m_PlayRect[p].right = m_RectClient.right - gridInterval*(p+1) - (gridWidth+100)*p;
		m_PlayRect[p].left = m_PlayRect[p].right - gridWidth;
		m_PlayRect[p].bottom = m_PlayRect[p].top + blockWidth*m_Doc.m_Game.m_Height + 1;

		m_ScoreRect[p].top = m_PlayRect[p].top;
		m_ScoreRect[p].bottom = m_ScoreRect[p].top + 192;
		m_ScoreRect[p].left = m_PlayRect[p].left - 100;
		m_ScoreRect[p].right= m_ScoreRect[p].left + 84;
	}
}

//-----------------------------------------------------------------------------
void CStackPackView::PaintGrid(CDC& hDC, int p)
{
	//ATLTRACE("OnPaintGrid\n");
	RECT rect;
	int h = m_PlayRect[p].bottom - m_PlayRect[p].top;
	int bs = (h-1)/m_Doc.m_Game.m_Height;

	char grid [32][32];
	m_StackPackEngine[p].GetGrid(grid);

	for (int r=0; r<m_Doc.m_Game.m_Height; r++)
	{
		for (int c=0; c<m_Doc.m_Game.m_Base; c++)
		{
			rect.left = m_PlayRect[p].left+ c*bs + 1;
			rect.top = m_PlayRect[p].top+ r*bs + 1;
			rect.right = rect.left+bs-2;
			rect.bottom = rect.top+bs-2;
			//hDC.FillRect(&rect);
			if (grid[r][c])
			{
				switch (m_Doc.m_PieceStyle)
				{
				case 0:
					PaintFlatBlock(hDC, grid[r][c], rect);
					break;
				case 1:
					Paint3DBlock(hDC, grid[r][c], rect);
					break;
				case 2:
					PaintPyrimidBlock(hDC, grid[r][c], rect);
					break;
				}
			}
			else
				PaintBackBlock(hDC, rect);
		}
	}

	//ATLTRACE("doneOnPaintGrid\n");
}

//-----------------------------------------------------------------------------
void CStackPackView::PaintScore(CDC& hDC, int p)
{
	// display the score
	CSize sz;

	hDC.SetBkColor(0x00c0c0c0);
	//hDC.SelectBrush(m_lightgrey);
	hDC.SelectBrush((HBRUSH)::GetStockObject(LTGRAY_BRUSH));

	char out[16];
	out[0] = 0;
	int middle = m_ScoreRect[p].left+(m_ScoreRect[p].right - m_ScoreRect[p].left)/2;
	int xpos = 0;

	strcpy(out, "Score");
	hDC.GetTextExtent(out, strlen(out), &sz);
	xpos = middle - sz.cx/2;
	hDC.TextOut(xpos, m_ScoreRect[p].top+5, out);

	if (m_StackPackEngine[p].m_Score == 0)
	{
		strcpy(out, "         ");
		hDC.GetTextExtent(out, strlen(out), &sz);
		xpos = middle - sz.cx/2;
		hDC.TextOut(xpos, m_ScoreRect[p].top+25, out);
	}
	wsprintf(out, " %d ", m_StackPackEngine[p].m_Score);
	hDC.GetTextExtent(out, strlen(out), &sz);
	xpos = middle - sz.cx/2;
	hDC.TextOut(xpos, m_ScoreRect[p].top+25, out);
	
	strcpy(out, "Level");
	hDC.GetTextExtent(out, strlen(out), &sz);
	xpos = middle - sz.cx/2;
	hDC.TextOut(xpos, m_ScoreRect[p].top+45, out);

	wsprintf(out, " %d ", m_StackPackEngine[p].m_Level);
	hDC.GetTextExtent(out, strlen(out), &sz);
	xpos = middle - sz.cx/2;
	hDC.TextOut(xpos, m_ScoreRect[p].top+65, out);

	strcpy(out, "Lines");
	hDC.GetTextExtent(out, strlen(out), &sz);
	xpos = middle - sz.cx/2;
	hDC.TextOut(xpos, m_ScoreRect[p].top+85, out);

	wsprintf(out, " %d ", m_StackPackEngine[p].m_Lines);
	hDC.GetTextExtent(out, strlen(out), &sz);
	xpos = middle - sz.cx/2;
	hDC.TextOut(xpos, m_ScoreRect[p].top+105, out);

// display the preview
	RECT rect;
	int icolor;
	int bs = 10;
	if (m_StackPackEngine[p].GetPreview(&icolor))
	{
		rect.left = m_ScoreRect[p].left+16;
		rect.top = m_ScoreRect[p].top+130;
		rect.right = rect.left+1 + (bs+2)*4;
		rect.bottom = rect.top+1 + (bs+2)*4;
		PaintBackBlock(hDC, rect);

		unsigned long pv = m_StackPackEngine[p].GetPreview(&icolor);
		icolor++;
		unsigned long mask;
		for (int r=0; r<5; r++)
		{
			for (int c=0; c<5; c++)
			{
				rect.left = m_ScoreRect[p].left+16+c*bs+1;
				rect.top = m_ScoreRect[p].top+130+r*bs+1;
				rect.right = rect.left + bs-2;
				rect.bottom = rect.top + bs-2;
				mask = 0x80000000>>(r*5+c);
				if (pv&mask)
				{
					switch (m_Doc.m_PieceStyle)
					{
					case 0:
						PaintFlatBlock(hDC, char(icolor), rect);
						break;
					case 1:
						Paint3DBlock(hDC, char(icolor), rect);
						break;
					case 2:
						PaintPyrimidBlock(hDC, char(icolor), rect);
						break;
					}
				}
				else
					PaintBackBlock(hDC, rect);
			}
		}
	}
}

//-----------------------------------------------------------------------------
LRESULT CStackPackView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{	// COLORREF = 0x00bbggrr
	m_ColorRef[0][0] = 0x000000ff;		// 0  light red
	m_ColorRef[0][1] = 0x000000c0;
	m_ColorRef[0][2] = 0x00000080;

	m_ColorRef[1][0] = 0x0000ff00;		// 6  light green
	m_ColorRef[1][1] = 0x0000c000;	
	m_ColorRef[1][2] = 0x00008000;	

	m_ColorRef[2][0] = 0x0000ffff;		// 2  light yellow
	m_ColorRef[2][1] = 0x0000c0c0;	
	m_ColorRef[2][2] = 0x00008080;	

	m_ColorRef[3][0] = 0x00ff0000;		// 4  light blue
	m_ColorRef[3][1] = 0x00c00000;	
	m_ColorRef[3][2] = 0x00800000;	

	m_ColorRef[4][0] = 0x00ff00ff;		// 1  light magenta
	m_ColorRef[4][1] = 0x00c000c0;
	m_ColorRef[4][2] = 0x00800080;

	m_ColorRef[5][0] = 0x00ffff00;		// 3  light cyan
	m_ColorRef[5][1] = 0x00c0c000;
	m_ColorRef[5][2] = 0x00808000;

	m_ColorRef[6][0] = 0x00ffffff;		// 7  white
	m_ColorRef[6][1] = 0x00c0c0c0;
	m_ColorRef[6][2] = 0x00808080;

	m_ColorRef[7][0] = 0x00000080;		// 8  dark red
	m_ColorRef[7][1] = 0x00000060;
	m_ColorRef[7][2] = 0x00000040;

	m_ColorRef[8][0] = 0x00008000;		// 9  dark green
	m_ColorRef[8][1] = 0x00006000;
	m_ColorRef[8][2] = 0x00004000;

	m_ColorRef[9][0] = 0x00008080;		// 10 dark yellow
	m_ColorRef[9][1] = 0x00006060;
	m_ColorRef[9][2] = 0x00004040;

	m_ColorRef[10][0] = 0x00800000;		// 13 dark blue
	m_ColorRef[10][1] = 0x00600000;
	m_ColorRef[10][2] = 0x00400000;

	m_ColorRef[11][0] = 0x00800080;		// 14 dark magenta
	m_ColorRef[11][1] = 0x00600060;
	m_ColorRef[11][2] = 0x00400040;

	m_ColorRef[12][0] = 0x00808000;		// 11 dark cyan
	m_ColorRef[12][1] = 0x00606000;
	m_ColorRef[12][2] = 0x00404000;

	m_ColorRef[13][0] = 0x00c0c0c0;		// 12 grey
	m_ColorRef[13][1] = 0x00808080;
	m_ColorRef[13][2] = 0x00404040;

//LARGE_INTEGER pc1, pc2;
//QueryPerformanceCounter(&pc1);
//for (int k=0; k<10000; k++)
//{
//		m_Brushes[0][0].CreateSolidBrush(0x0000ff);		// 0  light red
//		m_Brushes[0][0].DeleteObject();
//}
//QueryPerformanceCounter(&pc2);
//pc2.QuadPart -= pc1.QuadPart;
//char pdat[32];
//_ui64toa(pc2.QuadPart, pdat, 10);
//MessageBox(pdat);
	for (int i=0; i<10; i++)
		m_Screens[i].LoadBitmap(MAKEINTRESOURCE(IDB_SCREEN0+i));
		//if (m_Screens[i] == NULL) MessageBeep((WORD)-1);
	return 0;
}

//-----------------------------------------------------------------------------
LRESULT CStackPackView::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//ATLTRACE("CStackPackView::OnKeyDown\n");

	if (wParam == (m_Doc.m_LeftKey1&0xff))
		m_StackPackEngine[0].Left();
	else if (wParam == (m_Doc.m_RightKey1&0xff))
		m_StackPackEngine[0].Right();
	else if (wParam == (m_Doc.m_DownKey1&0xff))
		m_StackPackEngine[0].ForceStepNow();
	else if (wParam == (m_Doc.m_RotateKey1&0xff))
		m_StackPackEngine[0].Rotate();
	else if (wParam == (m_Doc.m_DropKey1&0xff))
		m_StackPackEngine[0].Drop();

	else if (wParam == (m_Doc.m_LeftKey2&0xff))
		m_StackPackEngine[1].Left();
	else if (wParam == (m_Doc.m_RightKey2&0xff))
		m_StackPackEngine[1].Right();
	else if (wParam == (m_Doc.m_DownKey2&0xff))
		m_StackPackEngine[1].ForceStepNow();
	else if (wParam == (m_Doc.m_RotateKey2&0xff))
		m_StackPackEngine[1].Rotate();
	else if (wParam == (m_Doc.m_DropKey2&0xff))
		m_StackPackEngine[1].Drop();

	else if (wParam == 19)		// the pause key
		::PostMessage(GetParent(), WM_COMMAND, ID_GAME_PAUSEF3, NULL);
	else if (wParam == 27)		// the escape key
	{
		::PostMessage(GetParent(), WM_COMMAND, ID_GAME_PAUSEF3, 1);
		::ShowWindow(GetParent(), SW_MINIMIZE);
		return 0;
	}
	return 0;
}

//-----------------------------------------------------------------------------
LRESULT CStackPackView::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//ATLTRACE("CStackPackView::OnKillFocus\n");
	::PostMessage(GetParent(), WM_COMMAND, ID_GAME_PAUSEF3, 1);
	return 0;
}

//-----------------------------------------------------------------------------
LRESULT CStackPackView::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	// TODO : Add Code for message handler. Call DefWindowProc if necessary.
	for (int p=0; p<m_Doc.m_NumberOfPlayers; p++)
	{
		if (m_PlayArea[p].m_hRgn == NULL) return 0;
		if (m_PlayArea[p].PtInRegion(LOWORD(lParam), HIWORD(lParam)))
			::SetCursor(::LoadCursor(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDC_CLEARCURSOR)));
	}
	return 0;
}

//-----------------------------------------------------------------------------
inline void CStackPackView::PaintFlatBlock(CDC& hDC, char block, RECT& rect)
{
	hDC.FillSolidRect( rect.left, rect.top, 
			rect.right - rect.left+1, rect.bottom - rect.top+1, m_ColorRef[(block-1)%14][0]);
}

//-----------------------------------------------------------------------------
inline void CStackPackView::Paint3DBlock(CDC& hDC, char block, RECT& rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_ColorRef[(block-1)%14][0]);
	hDC.SelectPen(pen);
	m_Brush.CreateSolidBrush(m_ColorRef[(block-1)%14][0]);
	hDC.SelectBrush(m_Brush);

	CPoint pts[6];
	int ind = (rect.right - rect.left) / 8;
	pts[0].x = rect.left;
	pts[0].y = rect.bottom;

	pts[1].x = rect.left;
	pts[1].y = rect.top;

	pts[2].x = rect.right;
	pts[2].y = rect.top;

	pts[3].x = rect.right - ind;
	pts[3].y = rect.top + ind;

	pts[4].x = rect.left + ind;
	pts[4].y = rect.top + ind;

	pts[5].x = rect.left + ind;
	pts[5].y = rect.bottom - ind;
	hDC.Polygon(pts, 6);

	hDC.FillSolidRect( rect.left+ind, rect.top+ind, rect.right-rect.left-ind-ind, rect.bottom-rect.top-ind-ind, m_ColorRef[(block-1)%14][1]);

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, m_ColorRef[(block-1)%14][2]);
	hDC.SelectPen(pen);

	m_Brush.DeleteObject();
	m_Brush.CreateSolidBrush(m_ColorRef[(block-1)%14][2]);
	hDC.SelectBrush(m_Brush);
	//pts[0].x = rect.left;
	//pts[0].y = rect.bottom;

	pts[1].x = rect.right;
	pts[1].y = rect.bottom;

	//pts[2].x = rect.right;
	//pts[2].y = rect.top;

	//pts[3].x = rect.right - ind;
	pts[3].y = rect.top + ind;

	pts[4].x = rect.right - ind;
	pts[4].y = rect.bottom - ind;

	//pts[5].x = rect.left + ind;
	//pts[5].y = rect.bottom - ind;
	hDC.Polygon(pts, 6);
	m_Brush.DeleteObject();
}

//-----------------------------------------------------------------------------
inline void CStackPackView::PaintPyrimidBlock(CDC& hDC, char block, RECT& rect)
{
	CPoint pts[4];
	CPen pen;
	//hDC.FillSolidRect( rect.left, rect.top, 
	//		rect.right - rect.left+1, rect.bottom - rect.top+1, m_ColorRef[(block-1)%14][1]);

	pen.CreatePen(PS_SOLID, 1, m_ColorRef[(block-1)%14][1]);
	hDC.SelectPen(pen);
	m_Brush.CreateSolidBrush(m_ColorRef[(block-1)%14][1]);
	hDC.SelectBrush(m_Brush);
	pts[0].x = rect.left;
	pts[0].y = rect.top;

	pts[1].x = rect.left;
	pts[1].y = rect.bottom;

	pts[2].x = rect.right;
	pts[2].y = rect.top;

	pts[3].x = rect.right;
	pts[3].y = rect.bottom;
	hDC.Polygon(pts, 4);

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, m_ColorRef[(block-1)%14][0]);
	hDC.SelectPen(pen);
	m_Brush.DeleteObject();
	m_Brush.CreateSolidBrush(m_ColorRef[(block-1)%14][0]);
	hDC.SelectBrush(m_Brush);

	int mid = (rect.right - rect.left) / 2;
	//pts[0].x = rect.left;
	//pts[0].y = rect.top;

	pts[1].x = rect.left + mid;
	pts[1].y = rect.top + mid;

	pts[2].x = rect.right-1;
	//pts[2].y = rect.top;
	hDC.Polygon(pts, 3);

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, m_ColorRef[(block-1)%14][2]);
	hDC.SelectPen(pen);
	m_Brush.DeleteObject();
	m_Brush.CreateSolidBrush(m_ColorRef[(block-1)%14][2]);
	hDC.SelectBrush(m_Brush);
	pts[0].x = rect.left+1;
	pts[0].y = rect.bottom;

	pts[1].x = rect.right;
	pts[1].y = rect.bottom;

	pts[2].x = rect.right - mid;
	pts[2].y = rect.bottom - mid;

	hDC.Polygon(pts, 3);
	m_Brush.DeleteObject();
}

//-----------------------------------------------------------------------------
inline void CStackPackView::PaintBackBlock(CDC& hDC, RECT& rect)
{
	hDC.FillSolidRect( rect.left, rect.top, 
			rect.right - rect.left+1, rect.bottom - rect.top+1, 0x00000000);
}

//-----------------------------------------------------------------------------
