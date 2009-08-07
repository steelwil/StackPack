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
//  Coded by: William Bell
//  email: william.bell@absamail.co.za
//  Home Page: http://myweb.absamail.co.za/william.bell/
//
// Properties.h: interface for the CProperties class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CProperties : public CDialogImpl<CProperties>
{
public:
	CStackPackDoc* m_pDoc;
	CStatic	m_GridSize;
	CStatic	m_BlockSize;
	CStatic	m_BlockComplexity;
	CStatic	m_LevelLimit;

	enum { IDD = IDD_PROPERTIES };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	CProperties(CStackPackDoc* pDoc)
	{
		m_pDoc = pDoc;
	}
	
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		m_GridSize.Attach(GetDlgItem(IDC_GRID_STATIC));
		m_BlockSize.Attach(GetDlgItem(IDC_BLOCKSIZE_STATIC));
		m_BlockComplexity.Attach(GetDlgItem(IDC_BLOCKCOMPLEX_STATIC));
		m_LevelLimit.Attach(GetDlgItem(IDC_LEVELLIMIT_STATIC));
		RefreshDisplay();
		return TRUE;
	}

	void RefreshDisplay()
	{
		char buf[32];
		SetWindowText(m_pDoc->m_Game.m_Comment);
		wsprintf(buf, "Base %d, Height %d", m_pDoc->m_Game.m_Base, m_pDoc->m_Game.m_Height);
		m_GridSize.SetWindowText(buf);

		buf[0] = 0;
		int blkType = m_pDoc->m_Game.m_BlkType;
		int blkSize = 0;
		if ((blkType&0x3))
		{
			blkSize |= 0x3;
			strcpy(buf, "Small, ");
		}
		if ((blkType&0x1fc))
		{
			strcat(buf, "Medium, ");
			blkSize |= 0x1fc;
		}
		if ((blkType&0x7fffe00))
		{
			strcat(buf, "Large");
			blkSize |= 0x7fffe00;
		}
		int len = strlen(buf);
		if (buf[len-2] == ',') buf[len-2] = 0;
		m_BlockSize.SetWindowText(buf);

		buf[0] = 0;
		if ((blkType|(0x4000025&blkSize)) == blkType)
			strcpy(buf, "Lowest, ");
		if ((blkType|(0x80781a&blkSize)) == blkType)
			strcat(buf, "Low, ");
		if ((blkType|(0x3180080&blkSize)) == blkType)
			strcat(buf, "Medium, ");
		if ((blkType|(0x678740&blkSize)) == blkType)
			strcat(buf, "High");
		len = strlen(buf);
		if (buf[len-2] == ',') buf[len-2] = 0;
		m_BlockComplexity.SetWindowText(buf);

		if (m_pDoc->m_Game.m_LastLevel == 10)
		{
			m_LevelLimit.SetWindowText("No Limitation");
		}
		else
		{
			wsprintf(buf, "%d", m_pDoc->m_Game.m_LastLevel);
			m_LevelLimit.SetWindowText(buf);
		}
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
};
