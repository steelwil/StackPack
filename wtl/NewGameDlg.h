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
// NewGameDlg.h: interface for the CNewGameDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CNewGameDlg : public CDialogImpl<CNewGameDlg>, public CWinDataExchange<CNewGameDlg>  
{
public:
	enum { IDD = IDD_NEWGAME_DIALOG };

	CUpDownCtrl	m_HeightSpin;
	CUpDownCtrl	m_BaseSpin;
	CUpDownCtrl	m_EndAfterSpin;
	int		m_EndAfter;
	BOOL	m_FiveBlocks;
	BOOL	m_FourBlocks;
	BOOL	m_ThreeBlocks;
	//int		m_Height;
	//int		m_Base;
	//char	m_Title[32];
	BOOL	m_Medium;
	BOOL	m_Lowest;
	BOOL	m_Low;
	BOOL	m_High;

	CStackPackDoc* m_pDoc;

	BEGIN_MSG_MAP(CNewGameDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOpenSaveDlg)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CCustomizeDlg)
		DDX_TEXT(IDC_TITLE_EDIT, m_pDoc->m_Game.m_Comment)
		DDX_CHECK(IDC_THREE_CHECK, m_ThreeBlocks)
		DDX_CHECK(IDC_FOUR_CHECK, m_FourBlocks)
		DDX_CHECK(IDC_FIVE_CHECK, m_FiveBlocks)
		DDX_INT(IDC_BASE_EDIT, m_pDoc->m_Game.m_Base)
		DDX_INT(IDC_HEIGHT_EDIT, m_pDoc->m_Game.m_Height)
		DDX_INT(IDC_ENDAFTER_EDIT, m_EndAfter)
		DDX_CHECK(IDC_LOWEST_CHECK, m_Lowest)
		DDX_CHECK(IDC_LOW_CHECK, m_Low)
		DDX_CHECK(IDC_MEDUIM_CHECK, m_Medium)
		DDX_CHECK(IDC_HIGH_CHECK, m_High)
	END_DDX_MAP()

	CNewGameDlg(CStackPackDoc* pDoc)
	{
		m_pDoc = pDoc;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());

		m_HeightSpin.Attach(GetDlgItem(IDC_HEIGHT_SPIN));
		m_BaseSpin.Attach(GetDlgItem(IDC_BASE_SPIN));
		m_EndAfterSpin.Attach(GetDlgItem(IDC_ENDAFTER_SPIN));
		m_BaseSpin.SetRange(5, 32);
		m_HeightSpin.SetRange(5, 32);
		m_EndAfterSpin.SetRange(1, 10);
		//m_Base = m_pDoc->m_Game.m_Base;
		//m_Height = m_pDoc->m_Game.m_Height;
		m_EndAfter = m_pDoc->m_Game.m_EndLevel;
		
		strcpy(m_pDoc->m_Game.m_Comment, "My New StackPack Game");
		int blkType = m_pDoc->m_Game.m_BlkType;
		int blkSize = 0;
		if ((blkType&0x3))
		{
			m_ThreeBlocks = TRUE;
			blkSize |= 0x3;
		}
		else
			m_ThreeBlocks = FALSE;
		if ((blkType&0x1fc))
		{
			m_FourBlocks = TRUE;
			blkSize |= 0x1fc;
		}
		else
			m_FourBlocks = FALSE;
		if ((blkType&0x7fffe00))
		{
			m_FiveBlocks = TRUE;
			blkSize |= 0x7fffe00;
		}
		else
			m_FiveBlocks = FALSE;

		if ((blkType|(0x4000025&blkSize)) == blkType)
			m_Lowest = TRUE;
		else
			m_Lowest = FALSE;
		if ((blkType|(0x80781a&blkSize)) == blkType)
			m_Low = TRUE;
		else
			m_Low = FALSE;
		if ((blkType|(0x3180080&blkSize)) == blkType)
			m_Medium = TRUE;
		else
			m_Medium = FALSE;
		if ((blkType|(0x678740&blkSize)) == blkType)
			m_High = TRUE;
		else
			m_High = FALSE;
		DoDataExchange(FALSE);
		return TRUE;
	}

	LRESULT OnOpenSaveDlg(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CFileDlg dlg(FALSE);
		dlg.m_ofn.lpstrTitle = "Save Game";
		if (dlg.DoModal() == IDOK)
		{
			DoDataExchange(TRUE);

			m_pDoc->OpenNewGameFile(dlg.m_szFileName, m_EndAfter);
			
			//strncpy(m_pDoc->m_Game.m_Comment, m_Title, 31);
			//m_pDoc->m_Game.m_Comment[31] = 0;
			//m_pDoc->m_Game.m_Base = m_Base;
			//m_pDoc->m_Game.m_Height = m_Height;
			m_pDoc->m_Game.m_EndLevel = char(m_EndAfter);
			
			int blkSize = 0;
			if (m_ThreeBlocks == TRUE)
				blkSize |= 0x3;
			if (m_FourBlocks == TRUE)
				blkSize |= 0x1fc;
			if (m_FiveBlocks == TRUE)
				blkSize |= 0x7fffe00;

			int blkType = 0;
			if (m_Lowest == TRUE)
				blkType |= 0x4000025;
			if (m_Low == TRUE)
				blkType |= 0x80781a;
			if (m_Medium == TRUE)
				blkType |= 0x3180080;
			if (m_High == TRUE)
				blkType |= 0x678740;

			blkType &= blkSize;
			if (blkType == 0)
				blkType = 0x1fc;
			m_pDoc->m_Game.m_BlkType = blkType;
			m_pDoc->SaveGameFile();
			// Update the main form's title & MRU list
			//::PostMessage(GetParent(), WM_NEWGAME, NULL, NULL);
			EndDialog(wID);
		}
		return 0;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

};
