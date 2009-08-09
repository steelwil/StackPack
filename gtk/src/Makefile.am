## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(STACKPACK_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = stackpack

stackpack_SOURCES = \
	main.c     \
	EnterNameDlg.c     \
	EnterNameDlg.h     \
	gameData.c     \
	gameData.h     \
	preferences.c     \
	preferences.h     \
	propertiesDlg.c     \
	propertiesDlg.h     \
	scoresDlg.c     \
	scoresDlg.h     \
	StackPackEngine.c     \
	StackPackEngine.h     \
	userData.c     \
	userData.h     \
	view.c     \
	view.h     

stackpack_LDFLAGS = 

stackpack_LDADD = $(STACKPACK_LIBS)

