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
// EnterHighScore.h: interface for the CEnterHighScore class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CEnterHighScore : public CDialogImpl<CEnterHighScore>, public CWinDataExchange<CEnterHighScore>
{
public:
	enum { IDD = IDD_ENTERHIGHSCORE };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CCustomizeDlg)
		DDX_TEXT(IDC_NAME, m_Name)
		DDX_TEXT(IDC_QUOTE, m_Quote)
	END_DDX_MAP()

	char	m_Name[32];
	char	m_Quote[32];

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		DoDataExchange(TRUE);
		EndDialog(wID);
		return 0;
	}
};
