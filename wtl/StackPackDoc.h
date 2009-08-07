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
// StackPackDoc.h: interface for the StackPackDoc class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

const char cJuniorStackPack = 0;
const char cClassicStackPack = 1;
const char cCoolStackPack = 2;
const char cStackPackMania = 3;

struct HighScoreRecord
{
	int score;
	int rows;
	int level;
	unsigned long date;		// date yyyy*10000 + mm*100 + dd
	char name[32];
	char quote[32];
};

struct GameRecord
{
	int m_Base;
	int m_Height;
	int m_BlkType;
	int m_StartingLevel;
	int m_StartingRows;
	char m_PiecePreview;
	char m_LastLevel;	 // Maximum level of this game
	char m_EndLevel;	 // current last level of this game m_EndLevel <= m_LastLevel
	char m_Spare1;
	int m_Penalize;
	char m_Comment[32];
	HighScoreRecord m_HighScore[10];
};

//-----------------------------------------------------------------------------
class CStackPackDoc  
{
public:
	GameRecord m_Game;
	//int left, right, top, bottom;	// window placement

	int m_ToolBar;
	int m_StatusBar;
	int m_Sound;
	int m_PieceStyle;
	int m_GridStyle;
	int m_NumberOfPlayers;

	char m_FileName[128];
	char m_MRUFile[4][128];

	unsigned int m_RotateKey2;
	unsigned int m_RotateKey1;
	unsigned int m_RightKey2;
	unsigned int m_RightKey1;
	unsigned int m_LeftKey2;
	unsigned int m_LeftKey1;
	unsigned int m_DropKey2;
	unsigned int m_DropKey1;
	unsigned int m_DownKey1;
	unsigned int m_DownKey2;

	HANDLE m_hGameFile;

public:
//-----------------------------------------------------------------------------
	CStackPackDoc() 
	{
		InitializeGameData();
		m_hGameFile = 0;
	}
//-----------------------------------------------------------------------------
	virtual ~CStackPackDoc()
	{
		if (m_hGameFile)
			CloseHandle(m_hGameFile);
	}
//-----------------------------------------------------------------------------
	void InitializeGameData(int endLevel = 10)
	{
		SYSTEMTIME tm;
		GetLocalTime(&tm);   
		unsigned long nowDate = tm.wYear*10000 + tm.wMonth*100 + tm.wDay;

		strcpy(m_FileName, "StackPack.gam");
		m_Game.m_Base = 10;
		m_Game.m_Height = 20;
		m_Game.m_BlkType = 0x1fc;
		m_Game.m_StartingLevel = 1;
		m_Game.m_StartingRows = 0;
		m_Game.m_LastLevel = 10;	 // Maximum level of this game
		m_Game.m_EndLevel = 10;		 // current last level of this game m_EndLevel <= m_LastLevel
		m_Game.m_Spare1 = 0;
		m_Game.m_PiecePreview = 1;
		m_Game.m_Penalize = 1;
		strcpy(m_Game.m_Comment, "Standard StackPack");
		for (int i=0; i<10; i++)
		{
			m_Game.m_HighScore[i].level = 1;
			m_Game.m_HighScore[i].name[0] = 0;
			m_Game.m_HighScore[i].quote[0] = 0;
			m_Game.m_HighScore[i].date = nowDate;
			m_Game.m_HighScore[i].score = (10-i)*50*endLevel;
			m_Game.m_HighScore[i].rows = 0;
		}
		//m_Level = 1;
	}
//-----------------------------------------------------------------------------
	void SaveGameFile()
	{
		// TODO: add storing code here
		//ATLTRACE("CStackPackDoc::SaveFile\n");
		DWORD nWritten;
		SetFilePointer(m_hGameFile, 0, NULL, FILE_BEGIN);
		WriteFile(m_hGameFile, &m_Game, sizeof(m_Game), &nWritten, NULL);
	}
//-----------------------------------------------------------------------------
	bool GetRelativeFileName(const char* fn)
	{
		// This function gets the file path relative to the current directory
		bool rtc = false;
		if (fn != m_FileName)		// cant do overlapped copy
		{
			if (!PathIsRelative(fn))
			{
				char dir[MAX_PATH];
				char rfn[MAX_PATH];
				DWORD rt = GetCurrentDirectory(MAX_PATH, dir);
				if (rt != 0 && rt < MAX_PATH)
				{
					PathRelativePathTo(rfn, dir, FILE_ATTRIBUTE_DIRECTORY, fn, FILE_ATTRIBUTE_NORMAL);
					if (rfn[0] == '.' && rfn[1] == '\\')
						strcpy(m_FileName, &rfn[2]);
					else
						strcpy(m_FileName, rfn);;
					rtc = true;
				}
			}
			else
			{
				strcpy(m_FileName, fn);
			}
		}
		return rtc;
	}

//-----------------------------------------------------------------------------
	bool OpenGameFile(const char* fn)
	{
		// TODO: add storing code here
		//ATLTRACE("CStackPackDoc::OpenGameFile\n");
		bool rtc = true;
		if (fn)
		{
			if (m_hGameFile)
				CloseHandle(m_hGameFile);

			char prevFile[MAX_PATH];
			strcpy(prevFile, m_FileName);
			GetRelativeFileName(fn);

			m_hGameFile = CreateFile(m_FileName, GENERIC_READ|GENERIC_WRITE,
									NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
			if (INVALID_HANDLE_VALUE == m_hGameFile)	// could not open file, reopen
			{
				// take out of mru
				UpdateMRUList(false);
				// load previous file or Kolumnz.gam
				strcpy(m_FileName, prevFile);
				m_hGameFile = CreateFile(m_FileName, GENERIC_READ|GENERIC_WRITE,
										NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
				rtc = false;
			}

			if (INVALID_HANDLE_VALUE == m_hGameFile)	// could not open file, reopen
			{
				// take out of mru
				UpdateMRUList(false);
				OpenStockGameFile(cClassicStackPack);
			}
			else
			{
				UpdateMRUList(true);
				ReadGameFile();
			}
		}
		return rtc;
	}
	//-----------------------------------------------------------------------------
	void OpenStockGameFile(int gameID)
	{
		// TODO: add storing code here
		//ATLTRACE("CStackPackDoc::OpenStockGameFile\n");

		if (m_hGameFile)
			CloseHandle(m_hGameFile);

		switch (gameID)
		{
		case cJuniorStackPack:
			strcpy(m_FileName, "JuniorStackPack.gam");
			break;
		case cClassicStackPack:
			strcpy(m_FileName, "StackPack.gam");
			break;
		case cCoolStackPack:
			strcpy(m_FileName, "CoolStackPack.gam");
			break;
		case cStackPackMania:
			strcpy(m_FileName, "StackPackMania.gam");
			break;
		}

		m_hGameFile = CreateFile(m_FileName, GENERIC_READ|GENERIC_WRITE,
								NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
		ReadGameFile();

		switch (gameID)
		{
		case cJuniorStackPack:
			if (m_Game.m_BlkType != 0x3f)
				InitializeGameData();
			strcpy(m_Game.m_Comment, "Easy StackPack");
			m_Game.m_BlkType = 0x3f;
			break;
		case cClassicStackPack:
			if (m_Game.m_BlkType != 0x1fc)
				InitializeGameData();
			strcpy(m_Game.m_Comment, "Classic StackPack");
			break;
		case cCoolStackPack:
			if (m_Game.m_BlkType != 0x79878bc)
				InitializeGameData();
			strcpy(m_Game.m_Comment, "Cool StackPack");
			m_Game.m_Base = 14;
			m_Game.m_BlkType = 0x79878bc;
			break;
		case cStackPackMania:
			if (m_Game.m_BlkType != 0x7fffffc)
				InitializeGameData();
			strcpy(m_Game.m_Comment, "StackPackMania says it all!");
			m_Game.m_Base = 20;
			m_Game.m_BlkType = 0x7fffffc;
			break;
		}
	}
	//-----------------------------------------------------------------------------
	bool OpenNewGameFile(const char* fn, int endLevel)
	{
		// TODO: add storing code here
		//ATLTRACE("CStackPackDoc::OpenNewGameFile\n");
		bool rtc = false;
		if (fn)
		{
			if (m_hGameFile)
				CloseHandle(m_hGameFile);

			InitializeGameData(endLevel);

			GetRelativeFileName(fn);
			m_hGameFile = CreateFile(m_FileName, GENERIC_READ|GENERIC_WRITE,
									NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
			UpdateMRUList(true);
			rtc = true;
		}
		return rtc;
	}

//-----------------------------------------------------------------------------
	void UpdateMRUList(bool found)
	{
		// TODO: add storing code here
		//ATLTRACE("CStackPackDoc::UpdateMRUList\n");
		if ((0 != lstrcmpi(m_FileName, "PackMania.gam")) &&
			(0 != lstrcmpi(m_FileName, "StackPack.gam")) &&
			(0 != lstrcmpi(m_FileName, "JuniorStackPack.gam")) &&
			(0 != lstrcmpi(m_FileName, "CoolStackPack.gam")))
		{	// do mru list
			int from = 3;
			bool inMRU = false;
			for (int f=0; f<4; f++)
			{
				// see if the file in already in the mru list
				if (lstrcmpi(m_MRUFile[f], m_FileName) == 0)
				{
					from = f;
					inMRU = true;
					break;
				}
			}
			if (found)
			{
				for (int m=from; m>0; m--)
					strcpy(m_MRUFile[m], m_MRUFile[m-1]);
				strcpy(m_MRUFile[0], m_FileName);
			}
			else if (inMRU)			// remove this out of the list 
			{
				for (int m=from; m<3; m++)
					strcpy(m_MRUFile[m], m_MRUFile[m+1]);
				m_MRUFile[3][0] = 0;
			}
		}
	}

//-----------------------------------------------------------------------------
	bool ReadGameFile()
	{
		// TODO: add loading code here
		//ATLTRACE("CStackPackDoc::ReadGameFile\n");
		bool rtc = false;
		DWORD nRead;

		if (INVALID_HANDLE_VALUE != m_hGameFile)
		{
			ReadFile(m_hGameFile, &m_Game, sizeof(m_Game), &nRead, NULL);
			if (nRead == 0)		
			{
				InitializeGameData();		// we must of created a new file
			}
			else
			{
				if (m_Game.m_LastLevel > 10 || m_Game.m_Base > 32 || m_Game.m_PiecePreview > 1)
				{
					InitializeGameData();		// corrupted file was read in
				}
				rtc = true;
			}
		}
		return rtc;
	}
//-----------------------------------------------------------------------------
	bool ReadINIFile(WINDOWPLACEMENT* wp)
	{
		char path[128];
		if (GetCurrentDirectory(128, path))
		{
			strcat(path, "\\StackPack.ini");

			wp->showCmd = GetPrivateProfileInt("WindowPosition", "status", 1, path);
			wp->rcNormalPosition.left = GetPrivateProfileInt("WindowPosition", "left", 0, path);
			wp->rcNormalPosition.right = GetPrivateProfileInt("WindowPosition", "right", 0, path);
			wp->rcNormalPosition.top = GetPrivateProfileInt("WindowPosition", "top", 0, path);
			wp->rcNormalPosition.bottom = GetPrivateProfileInt("WindowPosition", "bottom", 0, path);

			m_ToolBar = GetPrivateProfileInt("WindowControls", "toolbar", 1, path);
			m_StatusBar = GetPrivateProfileInt("WindowControls", "statusbar", 1, path);

			GetPrivateProfileString("MRUList", "file1", "", m_MRUFile[0], 128, path);
			GetPrivateProfileString("MRUList", "file2", "", m_MRUFile[1], 128, path);
			GetPrivateProfileString("MRUList", "file3", "", m_MRUFile[2], 128, path);
			GetPrivateProfileString("MRUList", "file4", "", m_MRUFile[3], 128, path);

			GetPrivateProfileString("Options", "autoload", "StackPack.gam", m_FileName, 128, path);
			m_NumberOfPlayers = GetPrivateProfileInt("Options", "players", 1, path);
			if (m_NumberOfPlayers < 1 || m_NumberOfPlayers > 2)
				m_NumberOfPlayers = 1;
			m_Sound = GetPrivateProfileInt("Options", "sound", 1, path);
			m_PieceStyle = GetPrivateProfileInt("Options", "piecestyle", 1, path);
			m_GridStyle = GetPrivateProfileInt("Options", "gridstyle", 1, path);

			m_LeftKey1 = GetPrivateProfileInt("ControlsPlayer1", "left", 2085, path);
			m_RightKey1 = GetPrivateProfileInt("ControlsPlayer1", "right", 2087, path);
			m_DownKey1 = GetPrivateProfileInt("ControlsPlayer1", "down", 2083, path);
			m_RotateKey1 = GetPrivateProfileInt("ControlsPlayer1", "rotate", 2086, path);
			m_DropKey1 = GetPrivateProfileInt("ControlsPlayer1", "drop", 2088, path);

			m_LeftKey2 = GetPrivateProfileInt("ControlsPlayer2", "left", 74, path);
			m_RightKey2 = GetPrivateProfileInt("ControlsPlayer2", "right", 76, path);
			m_DownKey2 = GetPrivateProfileInt("ControlsPlayer2", "down", 32, path);
			m_RotateKey2 = GetPrivateProfileInt("ControlsPlayer2", "rotate", 73, path);
			m_DropKey2 = GetPrivateProfileInt("ControlsPlayer2", "drop", 75, path);
			return true;
		}
		return false;
	}

//-----------------------------------------------------------------------------
	bool WriteINIFile(WINDOWPLACEMENT* wp)
	{
		char path[128];
		char data[32];
		if (GetCurrentDirectory(128, path))
		{
			strcat(path, "\\StackPack.ini");

			itoa(wp->showCmd, data, 10);
			WritePrivateProfileString("WindowPosition", "status", data, path);
			itoa(wp->rcNormalPosition.left, data, 10);
			WritePrivateProfileString("WindowPosition", "left", data, path);
			itoa(wp->rcNormalPosition.right, data, 10);
			WritePrivateProfileString("WindowPosition", "right", data, path);
			itoa(wp->rcNormalPosition.top, data, 10);
			WritePrivateProfileString("WindowPosition", "top", data, path);
			itoa(wp->rcNormalPosition.bottom, data, 10);
			WritePrivateProfileString("WindowPosition", "bottom", data, path);

			itoa(m_ToolBar, data, 10);
			WritePrivateProfileString("WindowControls", "toolbar", data, path);
			itoa(m_StatusBar, data, 10);
			WritePrivateProfileString("WindowControls", "statusbar", data, path);

			WritePrivateProfileString("MRUList", "file1", m_MRUFile[0], path);
			WritePrivateProfileString("MRUList", "file2", m_MRUFile[1], path);
			WritePrivateProfileString("MRUList", "file3", m_MRUFile[2], path);
			WritePrivateProfileString("MRUList", "file4", m_MRUFile[3], path);

			WritePrivateProfileString("Options", "autoload", m_FileName, path);
			
			itoa(m_NumberOfPlayers, data, 10);
			WritePrivateProfileString("Options", "players", data, path);
			itoa(m_Sound, data, 10);
			WritePrivateProfileString("Options", "sound", data, path);
			itoa(m_PieceStyle, data, 10);
			WritePrivateProfileString("Options", "piecestyle", data, path);
			itoa(m_GridStyle, data, 10);
			WritePrivateProfileString("Options", "gridstyle", data, path);

			itoa(m_LeftKey1, data, 10);
			WritePrivateProfileString("ControlsPlayer1", "left", data, path);
			itoa(m_RightKey1, data, 10);				
			WritePrivateProfileString("ControlsPlayer1", "right", data, path);
			itoa(m_DownKey1, data, 10);
			WritePrivateProfileString("ControlsPlayer1", "down", data, path);
			itoa(m_RotateKey1, data, 10);
			WritePrivateProfileString("ControlsPlayer1", "rotate", data, path);
			itoa(m_DropKey1, data, 10);
			WritePrivateProfileString("ControlsPlayer1", "drop", data, path);

			itoa(m_LeftKey2, data, 10);
			WritePrivateProfileString("ControlsPlayer2", "left", data, path);
			itoa(m_RightKey2, data, 10);				
			WritePrivateProfileString("ControlsPlayer2", "right", data, path);
			itoa(m_DownKey2, data, 10);
			WritePrivateProfileString("ControlsPlayer2", "down", data, path);
			itoa(m_RotateKey2, data, 10);
			WritePrivateProfileString("ControlsPlayer2", "rotate", data, path);
			itoa(m_DropKey2, data, 10);
			WritePrivateProfileString("ControlsPlayer2", "drop", data, path);

			return true;
		}
		return false;
	}

//-----------------------------------------------------------------------------
	bool IsHighScore(int score)
	{
		bool rtc = false;
		for (int i=0; i<10; i++)
		{
			if (m_Game.m_HighScore[i].score < score)
			{
				rtc = true;
				break;
			}
		}
		return rtc;
	}

//-----------------------------------------------------------------------------
	bool AddHighScore(const char* name, const char* quote, int score, int rows, int level)
	{
		bool rtc = false;
		for (int i=0; i<10; i++)
		{
			if (m_Game.m_HighScore[i].score < score)
			{
				rtc = true;
				if (i < 9 )
				{
					memmove(&m_Game.m_HighScore[i+1], 
						&m_Game.m_HighScore[i], 
						sizeof(HighScoreRecord)*(9-i));
				}
				m_Game.m_HighScore[i].score = score;
				m_Game.m_HighScore[i].rows = rows;
				m_Game.m_HighScore[i].level = level;
				SYSTEMTIME tm;
				GetLocalTime(&tm);   
				unsigned long nowDate = tm.wYear*10000 + tm.wMonth*100 + tm.wDay;
				m_Game.m_HighScore[i].date = nowDate;
				strcpy(m_Game.m_HighScore[i].name,name);
				strcpy(m_Game.m_HighScore[i].quote, quote);
				break;
			}
		}
		return rtc;
	}

};
