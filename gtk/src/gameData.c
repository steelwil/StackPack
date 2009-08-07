/*
 *  Copyright (C) 2006 - William Bell 
 *
 *  This file is part of the StackPack game
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <gtk/gtk.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
#include <glib.h>
#include <glib/gstdio.h>
#include "gameData.h"
/*----------------------------------------------------------------------------*/
void Initialize(struct GameDataStr* data)
{
	int i;
	time_t now;
	time(&now);
	struct tm* nowtime = localtime(&now);
	unsigned long nowDate = (nowtime->tm_year+1900)*10000 + (nowtime->tm_mon+1)*100 + nowtime->tm_mday;

	data->m_Base = 10;
	data->m_Height = 20;
	data->m_BlkType = 0x1fc;
	data->m_StartingLevel = 1;
	data->m_StartingRows = 0;
	data->m_LastLevel = 10;	 /* Maximum level of this game */
	data->m_EndLevel = 10;	/* current last level of this game m_EndLevel <= m_LastLevel */
	data->m_Spare1 = 0;
	data->m_Penalize = 1;
	strcpy(data->m_Comment, "Standard StackPack");
	for (i=0; i<10; i++)
	{
		data->m_HighScore[i].level = 1;
		strcpy(data->m_HighScore[i].name, " ");
		strcpy(data->m_HighScore[i].quote, " ");
		data->m_HighScore[i].date = nowDate;
		data->m_HighScore[i].score = (10-i)*50*data->m_EndLevel;
		data->m_HighScore[i].rows = 0;
	}
}

/*----------------------------------------------------------------------------*/
void SaveGameData(struct GameDataStr* data, const gchar *filename)
{
	printf("in SaveGameData\n");
	int i;
	FILE* fd = fopen(filename, "w");
	if (fd == NULL) /* file not found */
	{
		printf("File %s not found\n", filename);
		fd = fopen(filename, "w");
		if (fd != NULL)
		{
			chmod(filename, S_IREAD | S_IWRITE);
		}
	}
	if (fd != NULL)
	{
		fprintf(fd, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
				data->m_Base, 
				data->m_Height,
				data->m_BlkType,
				data->m_StartingLevel,
				data->m_StartingRows,
				data->m_LastLevel,
				data->m_EndLevel,
				data->m_Spare1,
				data->m_Penalize,
				data->m_Comment);
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
				data->m_Base, 
				data->m_Height,
				data->m_BlkType,
				data->m_StartingLevel,
				data->m_StartingRows,
				data->m_LastLevel,
				data->m_EndLevel,
				data->m_Spare1,
				data->m_Penalize,
				data->m_Comment);
		for (i=0; i<10; i++)
		{
			printf("%d,%d,%d,%ld,%s,%s\n",
			data->m_HighScore[i].score,
			data->m_HighScore[i].rows,
			data->m_HighScore[i].level,
			data->m_HighScore[i].date,
			data->m_HighScore[i].name,
			data->m_HighScore[i].quote);
			fprintf(fd, "%d,%d,%d,%ld,%s,%s\n",
			data->m_HighScore[i].score,
			data->m_HighScore[i].rows,
			data->m_HighScore[i].level,
			data->m_HighScore[i].date,
			data->m_HighScore[i].name,
			data->m_HighScore[i].quote);
		}
		
		fclose(fd);
	}
}

/*----------------------------------------------------------------------------*/
void ReadGameData(struct GameDataStr* data, const gchar *filename)
{
	//todo check for buffer overflows
	printf("in ReadGameData\n");
	int i;
	FILE* fd = fopen(filename, "r");
	if (fd != NULL)
	{
		fscanf(fd, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%[^\n]\n",
				&data->m_Base, 
				&data->m_Height,
				&data->m_BlkType,
				&data->m_StartingLevel,
				&data->m_StartingRows,
				&data->m_LastLevel,
				&data->m_EndLevel,
				&data->m_Spare1,
				&data->m_Penalize,
				data->m_Comment);
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
				data->m_Base, 
				data->m_Height,
				data->m_BlkType,
				data->m_StartingLevel,
				data->m_StartingRows,
				data->m_LastLevel,
				data->m_EndLevel,
				data->m_Spare1,
				data->m_Penalize,
				data->m_Comment);
		for (i=0; i<10; i++)
		{
			fscanf(fd, "%d,%d,%d,%ld,%[^,\n],%[^,\n]\n",
			&data->m_HighScore[i].score,
			&data->m_HighScore[i].rows,
			&data->m_HighScore[i].level,
			&data->m_HighScore[i].date,
			data->m_HighScore[i].name,
			data->m_HighScore[i].quote);
			printf("%d,%d,%d,%ld,%s,%s\n",
			data->m_HighScore[i].score,
			data->m_HighScore[i].rows,
			data->m_HighScore[i].level,
			data->m_HighScore[i].date,
			data->m_HighScore[i].name,
			data->m_HighScore[i].quote);
		}
		fclose(fd);
	}
	/* do validation */
	if (data->m_Base < 5)
		data->m_Base = 5;
	if (data->m_Base > 32)
		data->m_Base = 32;
	if (data->m_Height < 5)
		data->m_Height = 5;
	if (data->m_Height > 32)
		data->m_Height = 32;
	if (data->m_Penalize)
		data->m_Penalize = 1;
	if (data->m_StartingRows < 0)
		data->m_StartingRows = 0;
	if (data->m_StartingRows > data->m_Height - 5)
		data->m_StartingRows = data->m_Height - 5;
	if (data->m_LastLevel > 10)
		data->m_LastLevel = 10;
	if (data->m_LastLevel < 1)
		data->m_LastLevel = 1;
	if (data->m_StartingLevel < 0)
		data->m_StartingLevel = 1;
	if (data->m_StartingLevel > data->m_LastLevel)
		data->m_StartingLevel = data->m_LastLevel;
	if (data->m_EndLevel <= data->m_StartingLevel)
		data->m_EndLevel = data->m_StartingLevel+1;
	if (data->m_EndLevel > data->m_LastLevel)
		data->m_EndLevel = data->m_LastLevel;
	data->m_Comment[31] = 0;
	for (i=0; i<10; i++)
	{
		if (data->m_HighScore[i].score < (10-i)*50*data->m_LastLevel)
			data->m_HighScore[i].score = (10-i)*50*data->m_LastLevel;
		if (data->m_HighScore[i].rows < 1)
			data->m_HighScore[i].rows = 1;
		if (data->m_HighScore[i].level < 0)
			data->m_HighScore[i].level = 1;
		if (data->m_HighScore[i].level > data->m_LastLevel)
			data->m_HighScore[i].level = data->m_LastLevel;
		if (data->m_HighScore[i].date < 20000101)
			data->m_HighScore[i].date = 20000101;
		data->m_HighScore[i].name[31] = 0;
		data->m_HighScore[i].quote[31] = 0;
	}

}

/*----------------------------------------------------------------------------*/
void InitializeGameData(struct GameDataStr* data, const char* filename)
{
	Initialize(data);
	
	ReadGameData(data, filename);
	if (!strcmp(filename, "junior.gam"))
	{
		if (data->m_BlkType != 0xd7)
			Initialize(data);
		strcpy(data->m_Comment, "Easy StackPack");
		data->m_BlkType = 0xd7;
	}
	else if (!strcmp(filename, "classic.gam"))
	{
		if (data->m_BlkType != 0x1fc)
			Initialize(data);
		strcpy(data->m_Comment, "Classic StackPack");
	}
	else if (!strcmp(filename, "cool.gam"))
	{
		if (data->m_BlkType != 0x79879d7)
			Initialize(data);
		strcpy(data->m_Comment, "Cool StackPack");
		data->m_Base = 14;
		data->m_BlkType = 0x79879d7;
	}
	else if (!strcmp(filename, "mania.gam"))
	{
		if (data->m_BlkType != 0x7fffffc)
			Initialize(data);
		strcpy(data->m_Comment, "StackPack Mania says it all!");
		data->m_BlkType = 0x7fffffc;
		data->m_Base = 20;
	}

}

/*----------------------------------------------------------------------------*/
int IsHighScore(struct GameDataStr* data, int score)
{
	int i, rtc = 0;
	for (i=0; i<10; i++)
	{
		if (data->m_HighScore[i].score < score)
		{
			rtc = 1;
			break;
		}
	}
	return rtc;
}

/*----------------------------------------------------------------------------*/
int AddHighScore(struct GameDataStr* data, const char* name, const char* quote, int score, int rows, int level)
{
	int i, rtc = 0;
	for (i=0; i<10; i++)
	{
		if (data->m_HighScore[i].score < score)
		{
			rtc = 1;
			if (i < 9 )
			{
				memmove(&data->m_HighScore[i+1], 
					&data->m_HighScore[i], 
					sizeof(struct HighScoreRecord)*(9-i));
			}
			data->m_HighScore[i].score = score;
			data->m_HighScore[i].rows = rows;
			data->m_HighScore[i].level = level;
			time_t now;
			time(&now);
			struct tm* nowtime = localtime(&now);
			unsigned long nowDate = (nowtime->tm_year+1900)*10000 + (nowtime->tm_mon+1)*100 + nowtime->tm_mday;
			data->m_HighScore[i].date = nowDate;
			if (strlen(name) > 0)
			{
				strcpy(data->m_HighScore[i].name, name);
			}
			else
			{
				strcpy(data->m_HighScore[i].name, " ");
			}
			if (strlen(quote) > 0)
			{
				strcpy(data->m_HighScore[i].quote, quote);
			}
			else
			{
				strcpy(data->m_HighScore[i].quote, " ");
			}
			break;
		}
	}
	return rtc;
}
