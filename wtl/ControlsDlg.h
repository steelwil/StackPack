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
// ControlsDlg.h: interface for the CControlsDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

// We create CControlKey which is sub classed on CHotKeyCtrl
class CControlKey : public CWindowImpl<CControlKey,CHotKeyCtrl>
{
public:
	BEGIN_MSG_MAP(CControlKey)	
		MESSAGE_HANDLER(WM_CHAR, OnChar)
	END_MSG_MAP()

	void Attach(HWND hWndNew)
	{
		SubclassWindow(hWndNew);
		//CWindow::Attach(hWndNew);
	}

	LRESULT OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		if (wParam == 32) SetHotKey(32, 0);
		return 0;
	}
};

class CControlsDlg  : public CDialogImpl<CControlsDlg>
{
public:
	enum { IDD = IDD_CONTROLS_DIALOG };

	CControlKey	m_DownKey1;
	CControlKey	m_DownKey2;
	CControlKey	m_RotateKey2;
	CControlKey	m_RotateKey1;
	CControlKey	m_RightKey2;
	CControlKey	m_RightKey1;
	CControlKey	m_LeftKey2;
	CControlKey	m_LeftKey1;
	CControlKey	m_DropKey2;
	CControlKey	m_DropKey1;
	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancelCmd)
		COMMAND_HANDLER(IDC_DEFAULTS_BUTTON, BN_CLICKED, OnClickedDefaults_button)
	END_MSG_MAP()

	CStackPackDoc* m_pDoc;

	CControlsDlg(CStackPackDoc* pDoc)
	{
		m_pDoc = pDoc;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		
//		LPCTSTR str = CHotKeyCtrl::GetWndClassName();//"msctls_hotkey32"
		m_LeftKey1.Attach(GetDlgItem(IDC_LEFT1_HOTKEY));
		m_RightKey1.Attach(GetDlgItem(IDC_RIGHT1_HOTKEY));
		m_DownKey1.Attach(GetDlgItem(IDC_DOWN1_HOTKEY));
		m_DropKey1.Attach(GetDlgItem(IDC_DROP1_HOTKEY));
		m_RotateKey1.Attach(GetDlgItem(IDC_ROTATE1_HOTKEY));
		m_LeftKey2.Attach(GetDlgItem(IDC_LEFT2_HOTKEY));
		m_RightKey2.Attach(GetDlgItem(IDC_RIGHT2_HOTKEY));
		m_DownKey2.Attach(GetDlgItem(IDC_DOWN2_HOTKEY));
		m_DropKey2.Attach(GetDlgItem(IDC_DROP2_HOTKEY));
		m_RotateKey2.Attach(GetDlgItem(IDC_ROTATE2_HOTKEY));

		WORD wInvalidComb = 0xff;
   
		m_LeftKey1.SetRules(wInvalidComb, 0);
		m_LeftKey1.SetHotKey((WORD)m_pDoc->m_LeftKey1, WORD(m_pDoc->m_LeftKey1>>8));
		m_RightKey1.SetRules(wInvalidComb, 0);
		m_RightKey1.SetHotKey((WORD)m_pDoc->m_RightKey1, WORD(m_pDoc->m_RightKey1>>8));
		m_DownKey1.SetRules(wInvalidComb, 0);
		m_DownKey1.SetHotKey((WORD)m_pDoc->m_DownKey1, WORD(m_pDoc->m_DownKey1>>8));
		m_RotateKey1.SetRules(wInvalidComb, 0);
		m_RotateKey1.SetHotKey((WORD)m_pDoc->m_RotateKey1, WORD(m_pDoc->m_RotateKey1>>8));
		m_DropKey1.SetRules(wInvalidComb, 0);
		m_DropKey1.SetHotKey((WORD)m_pDoc->m_DropKey1, WORD(m_pDoc->m_DropKey1>>8));

		m_LeftKey2.SetRules(wInvalidComb, 0);
		m_LeftKey2.SetHotKey((WORD)m_pDoc->m_LeftKey2, WORD(m_pDoc->m_LeftKey2>>8));
		m_RightKey2.SetRules(wInvalidComb, 0);
		m_RightKey2.SetHotKey((WORD)m_pDoc->m_RightKey2, WORD(m_pDoc->m_RightKey2>>8));
		m_DownKey2.SetRules(wInvalidComb, 0);
		m_DownKey2.SetHotKey((WORD)m_pDoc->m_DownKey2, WORD(m_pDoc->m_DownKey2>>8));
		m_RotateKey2.SetRules(wInvalidComb, 0);
		m_RotateKey2.SetHotKey((WORD)m_pDoc->m_RotateKey2, WORD(m_pDoc->m_RotateKey2>>8));
		m_DropKey2.SetRules(wInvalidComb, 0);
		m_DropKey2.SetHotKey((WORD)m_pDoc->m_DropKey2, WORD(m_pDoc->m_DropKey2>>8));

		return TRUE;
	}

	LRESULT OnCancelCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_pDoc->m_RotateKey2 = m_RotateKey2.GetHotKey();
		m_pDoc->m_RotateKey1 = m_RotateKey1.GetHotKey();
		m_pDoc->m_RightKey2 = m_RightKey2.GetHotKey();
		m_pDoc->m_RightKey1 = m_RightKey1.GetHotKey();
		m_pDoc->m_DownKey1 = m_DownKey1.GetHotKey();
		m_pDoc->m_DownKey2 = m_DownKey2.GetHotKey();
		m_pDoc->m_LeftKey2 = m_LeftKey2.GetHotKey();
		m_pDoc->m_LeftKey1 = m_LeftKey1.GetHotKey();
		m_pDoc->m_DropKey2 = m_DropKey2.GetHotKey();
		m_pDoc->m_DropKey1 = m_DropKey1.GetHotKey();
		EndDialog(wID);
		return 0;
	}
	LRESULT OnClickedDefaults_button(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO : Add Code for control notification handler.
		m_LeftKey1.SetHotKey(37, HOTKEYF_EXT);
		m_RightKey1.SetHotKey(39, HOTKEYF_EXT);
		m_DownKey1.SetHotKey(35, HOTKEYF_EXT);
		m_RotateKey1.SetHotKey(38, HOTKEYF_EXT);
		m_DropKey1.SetHotKey(40, HOTKEYF_EXT);

		m_LeftKey2.SetHotKey(74, 0);
		m_RightKey2.SetHotKey(76, 0);
		m_DownKey2.SetHotKey(32, 0);
		m_RotateKey2.SetHotKey(73, 0);
		m_DropKey2.SetHotKey(75, 0);
		return 0;
	}
};
