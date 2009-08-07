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
// StackPack.cpp : main source file for StackPack.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>

#include "resource.h"

#include "StackPackDoc.h"
#include "StackPackEngine.h"
#include "StackPackView.h"
#include "aboutdlg.h"
#include "MainFrm.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	if (wndMain.m_wp.showCmd == SW_MAXIMIZE)
		nCmdShow = SW_MAXIMIZE;
	if (wndMain.m_wp.rcNormalPosition.right > 0 &&
		wndMain.m_wp.rcNormalPosition.bottom > 0 &&
		wndMain.m_wp.rcNormalPosition.top < wndMain.m_wp.rcNormalPosition.bottom && 
		wndMain.m_wp.rcNormalPosition.left < wndMain.m_wp.rcNormalPosition.right)
		wndMain.MoveWindow(	
			wndMain.m_wp.rcNormalPosition.left, 
			wndMain.m_wp.rcNormalPosition.top, 
			wndMain.m_wp.rcNormalPosition.right-wndMain.m_wp.rcNormalPosition.left, 
			wndMain.m_wp.rcNormalPosition.bottom-wndMain.m_wp.rcNormalPosition.top, FALSE); 
	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
//	HRESULT hRes = ::CoInitialize(NULL);  not using com
	HRESULT hRes = S_OK;
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	//::CoUninitialize();  not using com

	return nRet;
}
