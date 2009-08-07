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
// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "customizedlg.h"
#include "HighSoreDisplay.h"
#include "EnterHighScore.h"
#include "FileDlg.h"
#include "NewGameDlg.h"
#include "Properties.h"
#include "Htmlhelp.h"

static BOOL handled = TRUE;

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CStackPackView m_view;

	CCommandBarCtrl m_CmdBar;

	WINDOWPLACEMENT m_wp;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_view.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_JUNIORSTACKPACK, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_CLASSICSTACKPACK, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_COOLSTACKPACK, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_STACKPACKMANIA, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_OPTIONS_CUSTOMIZE, OnOptionsCustomize)
		COMMAND_ID_HANDLER(ID_GAME_PLAY, OnGamePlay)
		COMMAND_ID_HANDLER(ID_GAME_PAUSEF3, OnGamePauseToggle)
		COMMAND_ID_HANDLER(ID_GAME_HIGHSCORES, OnGameHighscores)
		COMMAND_ID_HANDLER(ID_GAME_PROPERTIES, OnGameProperties)
		COMMAND_RANGE_HANDLER(ID_FILE_JUNIORSTACKPACK, ID_FILE_STACKPACKMANIA, OnStockGame)
		COMMAND_ID_HANDLER(ID_HELP_CONTENTS, OnHelp)
		COMMAND_RANGE_HANDLER(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE4, OnFileMruFile)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

//-----------------------------------------------------------------------------
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// read the INI file
		m_view.m_Doc.ReadINIFile(&m_wp);

		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
		if (!m_view.m_Doc.m_ToolBar) 
		{
			CReBarCtrl rebar = m_hWndToolBar;
			int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
			rebar.ShowBand(nBandIndex, FALSE);
		}
		DWORD dwStyle = WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP;
		if (m_view.m_Doc.m_StatusBar) 
			dwStyle |= WS_VISIBLE;
	    CreateSimpleStatusBar(ATL_IDS_IDLEMESSAGE, dwStyle);

		m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, m_view.m_Doc.m_ToolBar);
		UISetCheck(ID_VIEW_STATUS_BAR, m_view.m_Doc.m_StatusBar);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		// restore the window's width and height
		//if (m_wp.showCmd == SW_SHOWMAXIMIZED)
		//	ModifyStyle(0, WS_MAXIMIZE);	// the state is actually restored in StackPack.cpp
		//if (m_wp.rcNormalPosition.left >= 0 && m_wp.rcNormalPosition.left < m_wp.rcNormalPosition.right)
		//	MoveWindow(m_wp.rcNormalPosition.left, m_wp.rcNormalPosition.top, m_wp.rcNormalPosition.right-m_wp.rcNormalPosition.left, m_wp.rcNormalPosition.bottom-m_wp.rcNormalPosition.top, FALSE); 
		// The view must be created already because this routine handles the MRU's
		m_view.m_Doc.OpenGameFile(m_view.m_Doc.m_FileName);
		UpdateFrame();
		BuildMRUMenu();

		return 0;
	}

//-----------------------------------------------------------------------------
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

//-----------------------------------------------------------------------------
	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: add code to initialize document
		if (TerminateCurrentGame())
		{
			CNewGameDlg dlg(&m_view.m_Doc);
			if (IDOK == dlg.DoModal())
				UpdateFrame();
		}

		return 0;
	}
//-----------------------------------------------------------------------------
	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (TerminateCurrentGame())
		{
			CFileDlg dlg(TRUE);
			dlg.m_ofn.lpstrTitle = "Open Game";
			if (dlg.DoModal() == IDOK)
			{
				m_view.m_Doc.OpenGameFile(dlg.m_szFileName);
				UpdateFrame();
				BuildMRUMenu();		// just incase a file failed and prev was a stock game 
									// and the file was in the MRU list
			}
		}
		return 0;
	}

//-----------------------------------------------------------------------------
	LRESULT OnStockGame(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (TerminateCurrentGame())
		{
			m_view.m_Doc.OpenStockGameFile(wID-ID_FILE_JUNIORSTACKPACK);
			UpdateFrame();
		}
		return 0;
	}

//-----------------------------------------------------------------------------
	LRESULT OnFileMruFile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (TerminateCurrentGame())
			m_view.m_Doc.OpenGameFile(m_view.m_Doc.m_MRUFile[wID-ID_FILE_MRU_FILE1]);
		UpdateFrame();
		BuildMRUMenu();		// just incase a file failed and prev was a stock game 
		return 0;
	}

//-----------------------------------------------------------------------------
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = m_view.m_Doc.m_ToolBar;	// initial visibility
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		m_view.m_Doc.m_ToolBar = bVisible;
		UpdateLayout();
		return 0;
	}

//-----------------------------------------------------------------------------
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		m_view.m_Doc.m_StatusBar = bVisible;
		UpdateLayout();
		return 0;
	}

//-----------------------------------------------------------------------------
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}
//-----------------------------------------------------------------------------
	LRESULT OnOptionsCustomize(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CCustomizeDlg cc(&m_view.m_Doc, false);
		if (IDOK == cc.DoModal())
		{
			bool busy = false;
			for (int p=0; p<m_view.m_Doc.m_NumberOfPlayers; p++)
			{
				m_view.m_StackPackEngine[p].SetPreview(m_view.m_Doc.m_Game.m_PiecePreview>0);
				if (m_view.m_StackPackEngine[p].IsGameInProgress())
					busy = true;
			}

			// if no game is busy set the starting level so long
			if (!busy)
				m_view.m_StackPackEngine[0].m_Level = m_view.m_Doc.m_Game.m_StartingLevel;

			m_view.Invalidate(TRUE);
		}
		return 0;
	}

//-----------------------------------------------------------------------------
	LRESULT OnGamePlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		bool unpaused = false;
		for (int p=0; p<m_view.m_Doc.m_NumberOfPlayers; p++)
		{
			if (m_view.m_StackPackEngine[p].IsGameInProgress())
			{
				if (m_view.m_StackPackEngine[p].IsGamePaused())
				{
					m_view.m_StackPackEngine[p].Start();
					SetWindowText(m_view.m_Doc.m_Game.m_Comment);
					unpaused = true;
				}
			}
		}
		if (!unpaused)
		{
			if (TerminateCurrentGame())
			{
				for (int p=0; p<m_view.m_Doc.m_NumberOfPlayers; p++)
				{
					m_view.m_StackPackEngine[p].Create(m_view, WM_USER_PAINTGAMEGRID1+p, 
												m_view.m_Doc.m_Game.m_Base, 
												m_view.m_Doc.m_Game.m_Height, 
												m_view.m_Doc.m_Game.m_StartingLevel,
												m_view.m_Doc.m_Game.m_StartingRows,
												m_view.m_Doc.m_Game.m_PiecePreview!=0,
												m_view.m_Doc.m_Game.m_BlkType,
												m_view.m_Doc.m_Game.m_EndLevel);
					m_view.m_StackPackEngine[p].Start();
					SetWindowText(m_view.m_Doc.m_Game.m_Comment);
				}
				m_view.Invalidate(TRUE);
			}
		}
		return 0;
	}

//-----------------------------------------------------------------------------
	LRESULT OnGamePauseToggle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
	{
		//ATLTRACE("OnGamePauseF3\n");
		for (int p=0; p<m_view.m_Doc.m_NumberOfPlayers; p++)
		{
			if (m_view.m_StackPackEngine[p].IsGameInProgress())
			{
				if (m_view.m_StackPackEngine[p].IsGamePaused())
				{
					if  ((int)hWndCtl != 1)		// Unconditional pause was issued
					{
						m_view.m_StackPackEngine[p].Start();
						SetWindowText(m_view.m_Doc.m_Game.m_Comment);
					}
				}
				else
				{
					m_view.m_StackPackEngine[p].Pause();
					SetWindowText("Game Paused!");
				}
			}
		}
		return 0;
	}
//-----------------------------------------------------------------------------
	LRESULT OnGameHighscores(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CHighSoreDisplay hsDlg(&m_view.m_Doc);
		hsDlg.DoModal();
		return 0;
	}

//-----------------------------------------------------------------------------
	LRESULT OnGameProperties(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CProperties propDlg(&m_view.m_Doc);
		propDlg.DoModal();
		return 0;
	}

//-----------------------------------------------------------------------------
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.

		m_wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(&m_wp);
		m_view.m_Doc.WriteINIFile(&m_wp);
		m_view.m_Doc.SaveGameFile();
		return 0;
	}

//-----------------------------------------------------------------------------
	LRESULT OnHelp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
//		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		//WinHelp("StackPack.hlp", HELP_CONTENTS);
		HtmlHelp(m_hWnd, "StackPack.chm", HH_DISPLAY_TOPIC, NULL);
		return 0;
	}
//-----------------------------------------------------------------------------
	bool TerminateCurrentGame()
	{
		bool rtc = true;
		bool bGameBusy = false;
	
		for (int p=0; p<m_view.m_Doc.m_NumberOfPlayers; p++)
		{
			if (m_view.m_StackPackEngine[p].IsGameInProgress())
			{
				if (!bGameBusy)		// The first time round we do the messagebox
				{
					if (IDCANCEL == MessageBox("End the current game?", "Warning", MB_OKCANCEL|MB_ICONWARNING))
					{
						rtc = false;
						break;
					}
				}
				bGameBusy = true;
				m_view.m_StackPackEngine[p].EndGame();
				if (m_view.m_Doc.IsHighScore(m_view.m_StackPackEngine[p].m_Score))
				{
					CEnterHighScore dlg;
					dlg.DoModal();
					m_view.m_Doc.AddHighScore(	dlg.m_Name, 
												dlg.m_Quote, 
												m_view.m_StackPackEngine[p].m_Score,
												m_view.m_StackPackEngine[p].m_Lines,
												m_view.m_StackPackEngine[p].m_Level);
					OnGameHighscores(NULL, NULL, NULL, handled);
				}
			}
		}
		if (rtc)
			m_view.m_Doc.SaveGameFile();
		return rtc;
	}

//-----------------------------------------------------------------------------
	bool UpdateFrame()
	{
		//ATLASSERT(::IsWindow(m_view));

		SetWindowText(m_view.m_Doc.m_Game.m_Comment);

		UISetCheck(ID_FILE_STACKPACKMANIA, 0);
		UISetCheck(ID_FILE_JUNIORSTACKPACK, 0);
		UISetCheck(ID_FILE_CLASSICSTACKPACK, 0);
		UISetCheck(ID_FILE_COOLSTACKPACK, 0);

		if (0 == lstrcmpi(m_view.m_Doc.m_FileName, "StackPackMania.gam"))
			UISetCheck(ID_FILE_STACKPACKMANIA, 1);
		else if (0 == lstrcmpi(m_view.m_Doc.m_FileName, "StackPack.gam"))
			UISetCheck(ID_FILE_CLASSICSTACKPACK, 1);
		else if (0 == lstrcmpi(m_view.m_Doc.m_FileName, "JuniorStackPack.gam"))
			UISetCheck(ID_FILE_JUNIORSTACKPACK, 1);
		else if (0 == lstrcmpi(m_view.m_Doc.m_FileName, "CoolStackPack.gam"))
			UISetCheck(ID_FILE_COOLSTACKPACK, 1);
		else		// do mru list
			BuildMRUMenu();
		m_view.m_StackPackEngine[0].m_Level = m_view.m_Doc.m_Game.m_StartingLevel;
		m_view.Invalidate(TRUE);
		return true;
	}
//-----------------------------------------------------------------------------
	bool BuildMRUMenu()
	{
		// build the MRU list
		char fn[MAX_PATH];		// without extension
		CMenuHandle hMenu = m_CmdBar.GetMenu().GetSubMenu(0);
		for (int i=0; i<4; i++)
		{
			strcpy(fn, m_view.m_Doc.m_MRUFile[i]);
			int len = strlen(fn);
			if (len > 3)
				fn[len-4] = 0;
			if (hMenu.GetMenuState(ID_FILE_MRU_FILE1+i, MF_BYCOMMAND) == 0xFFFFFFFF) 
				if (m_view.m_Doc.m_MRUFile[i][0] != 0)
					hMenu.InsertMenu(8+i, MF_BYPOSITION|MF_STRING, ID_FILE_MRU_FILE1+i, fn);
			else					// a MRU item was found in the menu position
			{
				if (m_view.m_Doc.m_MRUFile[i][0] != 0)
					hMenu.ModifyMenu(8+i, MF_BYPOSITION|MF_STRING, ID_FILE_MRU_FILE1+i, fn);
				else
				{
					if (i == 0)		// The MRU list is now empty
					{
						//ATLTRACE("Recent Game\n");
						hMenu.ModifyMenu(8+i, MF_BYPOSITION|MF_STRING|MF_GRAYED, ID_FILE_MRU_FILE1+i, 
							"Recent File");
						break;
					}
					else
					{
						//ATLTRACE("Deleting menu\n");
						hMenu.DeleteMenu(8+i, MF_BYPOSITION);
					}
				}
			}
		}
		return true;
	}
};
