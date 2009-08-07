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
// FileDlg.h: interface for the CFileDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CFileDlg : public CFileDialogImpl<CFileDlg>
{
public:
	CFileDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = "gam",
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST,
		LPCTSTR lpszFilter = "StackPack Files (*.gam)\0*.gam\0All Files (*.*)\0*.*\0",
		HWND hWndParent = NULL)
		: CFileDialogImpl<CFileDlg>(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, hWndParent)
	{}

	// override base class map and references to handlers
	BEGIN_MSG_MAP(CFileDlg)
		NOTIFY_CODE_HANDLER(CDN_INITDONE, OnInitComplete)
	END_MSG_MAP()

	LRESULT OnInitComplete(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	{
		CWindow(GetParent()).CenterWindow();
		return 0;
	}
};
