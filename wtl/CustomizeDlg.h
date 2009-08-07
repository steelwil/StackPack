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
// CustomizeDlg.h: interface for the CCustomizeDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "ControlsDlg.h"

class CCustomizeDlg : public CDialogImpl<CCustomizeDlg>, public CWinDataExchange<CCustomizeDlg>
{
public:
	enum { IDD = IDD_OPTIONS_DIALOG };

	bool m_IsNew;

	CUpDownCtrl	m_RowsSpin;
	CUpDownCtrl	m_StartingLevelSpin;
	CUpDownCtrl	m_EndLevelSpin;
	BOOL	m_PiecePreview;
	//BOOL	m_Penalize;
	int		m_Players;
	//int		m_StartingRows;
	//int		m_StartingLevel;
	int		m_EndLevel;
	//BOOL	m_Sound;

	//int m_PieceStyle;
	BOOL m_GridVertical;
	BOOL m_GridHorizontal;
	
	CStackPackDoc* m_pDoc;

	BEGIN_MSG_MAP(CCustomizeDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancelCmd)
		COMMAND_ID_HANDLER(IDC_KEYBOARD_BUTTON, OnKeyboardButton)
		COMMAND_HANDLER(IDC_LEVEL_EDIT, EN_CHANGE, OnStartLevelChanged) 
		COMMAND_HANDLER(IDC_ENDLEVEL_EDIT, EN_CHANGE, OnEndLevelChanged) 
	END_MSG_MAP()

	BEGIN_DDX_MAP(CCustomizeDlg)
		DDX_CHECK(IDC_PREVEIW_CHECK, m_PiecePreview)
		DDX_CHECK(IDC_PENALIZE_CHECK, m_pDoc->m_Game.m_Penalize)
		DDX_CHECK(IDC_SOUND_CHECK, m_pDoc->m_Sound)
		DDX_INT(IDC_ROWS_EDIT, m_pDoc->m_Game.m_StartingRows)
		DDX_INT(IDC_ENDLEVEL_EDIT, m_EndLevel)  // must be before IDC_LEVEL_EDIT
		DDX_INT(IDC_LEVEL_EDIT, m_pDoc->m_Game.m_StartingLevel)
		DDX_RADIO(IDC_PLAYERS_RADIO, m_Players)
		DDX_RADIO(IDC_PIECESTYLE_RADIO, m_pDoc->m_PieceStyle)
		DDX_CHECK(IDC_GRIDVERTICAL_CHECK, m_GridVertical)
		DDX_CHECK(IDC_GRIDHORIZONTAL_CHECK, m_GridHorizontal)
		// todo part of properties
	END_DDX_MAP()

	CCustomizeDlg(CStackPackDoc* pDoc, bool IsNew)
	{
		m_IsNew = IsNew;
		m_pDoc = pDoc;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		m_RowsSpin.Attach(GetDlgItem(IDC_ROWS_SPIN));
		m_StartingLevelSpin.Attach(GetDlgItem(IDC_LEVEL_SPIN));
		m_EndLevelSpin.Attach(GetDlgItem(IDC_ENDLEVEL_SPIN));
		m_RowsSpin.SetRange(0, 32);
		m_StartingLevelSpin.SetRange(1, m_pDoc->m_Game.m_LastLevel);
		m_EndLevelSpin.SetRange(1, m_pDoc->m_Game.m_LastLevel);

		m_Players = m_pDoc->m_NumberOfPlayers-1;
		//m_StartingLevel = m_pDoc->m_Game.m_StartingLevel;
		m_EndLevel = m_pDoc->m_Game.m_EndLevel;
		//m_StartingRows = m_pDoc->m_Game.m_StartingRows;
		//m_Sound = m_pDoc->m_Sound;
		m_PiecePreview = m_pDoc->m_Game.m_PiecePreview;
		//m_Penalize = m_pDoc->m_Game.m_Penalize;
		//m_PieceStyle = m_pDoc->m_PieceStyle;
		m_GridVertical = m_pDoc->m_GridStyle & 1;
		m_GridHorizontal = m_pDoc->m_GridStyle & 2;
		
		DoDataExchange(FALSE);

		return TRUE;
	}

	LRESULT OnCancelCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		DoDataExchange(TRUE);
		
		m_pDoc->m_NumberOfPlayers = m_Players+1;
		if (m_pDoc->m_Game.m_StartingLevel > m_EndLevel)
			m_pDoc->m_Game.m_StartingLevel = m_EndLevel;
		//m_pDoc->m_Game.m_StartingLevel = m_StartingLevel;
		m_pDoc->m_Game.m_EndLevel = char(m_EndLevel);
		//m_pDoc->m_Game.m_StartingRows = m_StartingRows;
		//m_pDoc->m_Sound = m_Sound;
		//m_pDoc->m_Game.m_Penalize = m_Penalize;
		m_pDoc->m_Game.m_PiecePreview = char(m_PiecePreview);
		//m_pDoc->m_PieceStyle = m_PieceStyle;
		m_pDoc->m_GridStyle = (m_GridHorizontal << 1) + m_GridVertical;
		
		EndDialog(wID);
		return 0;
	}

	LRESULT OnKeyboardButton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CControlsDlg controlsDlg(m_pDoc);
		controlsDlg.DoModal();
		return 0;
	}


	LRESULT OnStartLevelChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		UINT startLevel = GetDlgItemInt(IDC_LEVEL_EDIT);
		UINT endLevel = GetDlgItemInt(IDC_ENDLEVEL_EDIT);
		if (startLevel > endLevel)
		{
			SetDlgItemInt(IDC_LEVEL_EDIT, endLevel);
		}
		return 0;
	}

	LRESULT OnEndLevelChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		UINT startLevel = GetDlgItemInt(IDC_LEVEL_EDIT);
		UINT endLevel = GetDlgItemInt(IDC_ENDLEVEL_EDIT);
		if (startLevel > endLevel)
		{
			SetDlgItemInt(IDC_ENDLEVEL_EDIT, startLevel);
		}
		return 0;
	}

};
