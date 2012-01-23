/*
Launchy: Application Launcher
Copyright (C) 2007  Josh Karlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#pragma once

#define PLUGIN_NAME "Launchy-CRT"
#define PLUGIN_VERSION "0.2"

#include "plugin_interface.h"
#include "SessionManager.h"
#include "gui.h"



class launchy_crtPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)


public:
	uint HASH_secureCRT;
	uint HASH_session;
private: 
	SessionManager sessionManager;
	Gui* gui;
	// settings
	bool telCmdSet;
	bool sshCmdSet;
	bool secCmdSet;
	bool allowIndexing;
	QList<CatItem> buildCatalog();
	// keywords
	QString telnetKeyword;
	QString sshKeyword;
	QString secureCrtKeyword;

public:
	launchy_crtPlugin() {
		HASH_secureCRT = qHash(QString(PLUGIN_NAME));
		HASH_session = qHash(QString("CRT-Session"));
		gui = NULL;
	}
	~launchy_crtPlugin() {}
	int msg(int msgId, void* wParam = NULL, void* lParam = NULL); 

	void getLabels(QList<InputData>*);
	void getID(uint*);
	void getName(QString*);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
	bool matchUserInput(CatItem session, QString userInput);
	void getCatalog(QList<CatItem>* items);
	void launchItem(QList<InputData>*, CatItem*);
	void doDialog(QWidget* parent, QWidget**);
	void endDialog(bool accept);
	void init();
	QString getIcon();
};

extern launchy_crtPlugin* gmypluginInstance;



