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
// HighSoreDisplay.h: interface for the CHighSoreDisplay class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define DATESPLIT(date) \
	date/10000%10000, date/100%100, date%100

class CHighSoreDisplay : public CDialogImpl<CHighSoreDisplay>
{
private:
	DWORD m_bkcolor;
	CBrush m_bkBrush;
public:
	CStackPackDoc* m_pDoc;
	CStatic	m_RankList;
	CStatic	m_ScoreList;
	CStatic	m_RowList;
	CStatic	m_LevelList;
	CStatic	m_DateList;
	CStatic	m_NameList;
	CStatic	m_QuoteList;
	CStatic	m_Title;

	enum { IDD = IDD_HIGHSCORETABLE };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancelCmd)
	END_MSG_MAP()

	CHighSoreDisplay(CStackPackDoc* pDoc)
	{
		m_pDoc = pDoc;
	}
	
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_bkcolor = GetSysColor(COLOR_3DFACE);
		m_bkBrush.CreateSolidBrush(m_bkcolor);

		CenterWindow(GetParent());
		m_RankList.Attach(GetDlgItem(IDC_RANK_STATIC));
		m_ScoreList.Attach(GetDlgItem(IDC_SCORE_STATIC));
		m_RowList.Attach(GetDlgItem(IDC_ROWS_STATIC));
		m_LevelList.Attach(GetDlgItem(IDC_LEVEL_STATIC));
		m_DateList.Attach(GetDlgItem(IDC_DATE_STATIC));
		m_NameList.Attach(GetDlgItem(IDC_NAME_STATIC));
		m_QuoteList.Attach(GetDlgItem(IDC_QUOTE_STATIC));
		RefreshDisplay();
		return TRUE;
	}

	void RefreshDisplay()
	{
		char out[512];

		SetWindowText(m_pDoc->m_Game.m_Comment);

		m_RankList.SetWindowText("Rnk\n1\n\n2\n\n3\n\n4\n\n5\n\n6\n\n7\n\n8\n\n9\n\n10");

		wsprintf(out, "Score\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d",
			m_pDoc->m_Game.m_HighScore[0].score,
			m_pDoc->m_Game.m_HighScore[1].score,
			m_pDoc->m_Game.m_HighScore[2].score,
			m_pDoc->m_Game.m_HighScore[3].score,
			m_pDoc->m_Game.m_HighScore[4].score,
			m_pDoc->m_Game.m_HighScore[5].score,
			m_pDoc->m_Game.m_HighScore[6].score,
			m_pDoc->m_Game.m_HighScore[7].score,
			m_pDoc->m_Game.m_HighScore[8].score,
			m_pDoc->m_Game.m_HighScore[9].score);
		m_ScoreList.SetWindowText(out);

		wsprintf(out, "Rows\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d",
			m_pDoc->m_Game.m_HighScore[0].rows,
			m_pDoc->m_Game.m_HighScore[1].rows,
			m_pDoc->m_Game.m_HighScore[2].rows,
			m_pDoc->m_Game.m_HighScore[3].rows,
			m_pDoc->m_Game.m_HighScore[4].rows,
			m_pDoc->m_Game.m_HighScore[5].rows,
			m_pDoc->m_Game.m_HighScore[6].rows,
			m_pDoc->m_Game.m_HighScore[7].rows,
			m_pDoc->m_Game.m_HighScore[8].rows,
			m_pDoc->m_Game.m_HighScore[9].rows);
		m_RowList.SetWindowText(out);

		wsprintf(out, "Level\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d\n\n%d",
			m_pDoc->m_Game.m_HighScore[0].level,
			m_pDoc->m_Game.m_HighScore[1].level,
			m_pDoc->m_Game.m_HighScore[2].level,
			m_pDoc->m_Game.m_HighScore[3].level,
			m_pDoc->m_Game.m_HighScore[4].level,
			m_pDoc->m_Game.m_HighScore[5].level,
			m_pDoc->m_Game.m_HighScore[6].level,
			m_pDoc->m_Game.m_HighScore[7].level,
			m_pDoc->m_Game.m_HighScore[8].level,
			m_pDoc->m_Game.m_HighScore[9].level);
		m_LevelList.SetWindowText(out);

		wsprintf(out, "Date\n%d-%02d-%02d\n\n%d-%02d-%02d\n\n%d-%02d-%02d\n\n%d-%02d-%02d\n\n%d-%02d-%02d\n\n%d-%02d-%02d\n\n%d-%02d-%02d\n\n%d-%02d-%02d\n\n%d-%02d-%02d\n\n%d-%02d-%02d\n\n",
			DATESPLIT(m_pDoc->m_Game.m_HighScore[0].date),
			DATESPLIT(m_pDoc->m_Game.m_HighScore[1].date),
			DATESPLIT(m_pDoc->m_Game.m_HighScore[2].date),
			DATESPLIT(m_pDoc->m_Game.m_HighScore[3].date),
			DATESPLIT(m_pDoc->m_Game.m_HighScore[4].date),
			DATESPLIT(m_pDoc->m_Game.m_HighScore[5].date),
			DATESPLIT(m_pDoc->m_Game.m_HighScore[6].date),
			DATESPLIT(m_pDoc->m_Game.m_HighScore[7].date),
			DATESPLIT(m_pDoc->m_Game.m_HighScore[8].date),
			DATESPLIT(m_pDoc->m_Game.m_HighScore[9].date));
		m_DateList.SetWindowText(out);

		wsprintf(out, "Name\n%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s",
			m_pDoc->m_Game.m_HighScore[0].name,
			m_pDoc->m_Game.m_HighScore[1].name,
			m_pDoc->m_Game.m_HighScore[2].name,
			m_pDoc->m_Game.m_HighScore[3].name,
			m_pDoc->m_Game.m_HighScore[4].name,
			m_pDoc->m_Game.m_HighScore[5].name,
			m_pDoc->m_Game.m_HighScore[6].name,
			m_pDoc->m_Game.m_HighScore[7].name,
			m_pDoc->m_Game.m_HighScore[8].name,
			m_pDoc->m_Game.m_HighScore[9].name);
		m_NameList.SetWindowText(out);

		wsprintf(out, "Quote\n%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s",
			m_pDoc->m_Game.m_HighScore[0].quote,
			m_pDoc->m_Game.m_HighScore[1].quote,
			m_pDoc->m_Game.m_HighScore[2].quote,
			m_pDoc->m_Game.m_HighScore[3].quote,
			m_pDoc->m_Game.m_HighScore[4].quote,
			m_pDoc->m_Game.m_HighScore[5].quote,
			m_pDoc->m_Game.m_HighScore[6].quote,
			m_pDoc->m_Game.m_HighScore[7].quote,
			m_pDoc->m_Game.m_HighScore[8].quote,
			m_pDoc->m_Game.m_HighScore[9].quote);
		m_QuoteList.SetWindowText(out);
	}

	LRESULT OnCancelCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
};
