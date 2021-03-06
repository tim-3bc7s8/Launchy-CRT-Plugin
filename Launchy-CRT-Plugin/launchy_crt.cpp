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

#include <QtGui>
#include <QUrl>
#include <QFile>
#include <QRegExp>
#include <QTextCodec>

#ifdef Q_WS_WIN
#include <windows.h>
#include <shlobj.h>
#include <tchar.h>

#endif

#include "launchy_crt.h"
#include "SessionManager.h"
#include "gui.h"

launchy_crtPlugin* gmypluginInstance = NULL;


void launchy_crtPlugin::getID(uint* id)
{
	*id = HASH_secureCRT;	
}

void launchy_crtPlugin::getName(QString* str)
{
	*str = PLUGIN_NAME;
}

void launchy_crtPlugin::init()
{
	QSettings* set = *settings;
	set->setValue("secureCRT/version", PLUGIN_VERSION); 
	telCmdSet = set->value("secureCRT/telnetCommand", true).toBool();
	sshCmdSet = set->value("secureCRT/sshCommand", true).toBool();
	secCmdSet = set->value("secureCRT/secureCrtCommand", true).toBool();
	allowIndexing = set->value("secureCRT/allowSessionIndexing", true).toBool();
	sessionManager.setSessionPath(set->value("secureCRT/sessionsLocation", sessionManager.getDefaultLocation()).toString());
	// loads the sessions into the catalog. otherwise, the <tab> function
	// won't show any sessions until the library is refreshed.
	if (secCmdSet && sessionManager.getSessionList().isEmpty())
		sessionManager.setSessionList(buildCatalog());
	// load keywords
	telnetKeyword = set->value("secureCRT/telnetKeyword", "Telnet").toString();
	sshKeyword = set->value("secureCRT/sshKeyword", "SSH").toString();
	secureCrtKeyword = set->value("secureCRT/secureCrtKeyword", "SecureCRT").toString();
}



void launchy_crtPlugin::getLabels(QList<InputData>* id)
{	
}

void launchy_crtPlugin::getResults(QList<InputData>* id, QList<CatItem>* results)
{ 	
	if (id->count() <= 1)
		return;

	if (id->first().getTopResult().id != HASH_secureCRT)
		return;

	// This section handles the SecureCRT <tab> feature
	if (id->count() >= 2 && secCmdSet && id->first().getText() == secureCrtKeyword) {		
		// Get a list of all the sessions available
		QList<CatItem> sessions = sessionManager.getSessionList();
		foreach (CatItem session, sessions) {
			if (matchUserInput(session, id->last().getText())) {				
				// display results
				results->push_back(session);  
			}
		}
	}

	// This section handles the telnet <tab> feature
	if (id->count() == 2 && telCmdSet && id->first().getText() == telnetKeyword) {
		QString text = id->last().getText();
		results->push_front(CatItem("SecureCRT Telnet", text, HASH_secureCRT, getIcon()));
	}

	// This section handles the ssh <tab> feature
	if (id->count() == 2 && sshCmdSet && id->first().getText() == sshKeyword) {
		QString text = id->last().getText();
		results->push_front(CatItem("SecureCRT SSH", text, HASH_secureCRT, getIcon()));
	}
}

// Compares the session with the user input
bool launchy_crtPlugin::matchUserInput(CatItem session, QString userInput) {
	if (userInput.trimmed().isEmpty())
		return true;

	QStringList userInputList;
	userInputList = userInput.split(" ");
	foreach (QString text, userInputList) {
		if (!session.fullPath.contains(text,Qt::CaseInsensitive))
			return false;
	}
	return true;
}

QString launchy_crtPlugin::getIcon()
{
#ifdef Q_WS_WIN
	return qApp->applicationDirPath() + "/plugins/icons/secureCRT.ico";
#endif
}

void launchy_crtPlugin::getCatalog(QList<CatItem>* items)
{
	// This is used for the SecureCRT <tab> feature
	if (secCmdSet) {
		items->push_back(CatItem("SecureCRT", secureCrtKeyword, HASH_secureCRT, getIcon()));
	}

	// This is the telnet feature
	if (telCmdSet) {
		items->push_back(CatItem("SecureCRT Telnet", telnetKeyword, HASH_secureCRT, getIcon()));
	}

	// This is the ssh feature
	if (sshCmdSet) {
		items->push_back(CatItem("SecureCRT SSH", sshKeyword, HASH_secureCRT, getIcon()));
	}

	// Build a list of sessions
	if (allowIndexing || secCmdSet) {
		QList<CatItem> sessionList = buildCatalog();		
		// Adds sessions info Launchy's catalog
		if (allowIndexing) {
			foreach (CatItem s, sessionList) {
				items->push_back(s);
			}
		}
		// Adds sessions to the session manager
		if (secCmdSet) {
			sessionManager.setSessionList(sessionList);
		}
	}	
}

void launchy_crtPlugin::launchItem(QList<InputData>* id, CatItem* item)
{
	QString program = "C:\\Program Files\\VanDyke Software\\SecureCRT\\SecureCRT";
	item = &id->last().getTopResult();

	if (id->count() == 1) {		
		if (item->shortName == secureCrtKeyword) {
			// Run SecureCRT without any arguments
			runProgram(program, "");
		} else {
			// Run a session instance
			QString args = "/T /S \"" + item->fullPath + "\"";
			runProgram(program, args);
		}	
	}

	// these are <tab> features
	if (id->count() == 2) {
		if (secCmdSet && id->first().getText() == secureCrtKeyword) {
			// Run a SecureCRT session
			QString args = "/T /S \"" + item->fullPath + "\"";
			runProgram(program, args); 
			QList<CatItem> tempList = sessionManager.getSessionList();
			int i = tempList.indexOf(*item);
			tempList[i].usage++;
			sessionManager.setSessionList(tempList);
		}
		else if (telCmdSet && id->first().getText() == telnetKeyword) {
			// Run a telnet session
			QString args = "/T /TELNET " + item->shortName;
			runProgram(program, args);
		}
		else if (sshCmdSet && id->first().getText() == sshKeyword) {
			// Run a SSH session
			QString args = "/T /SSH2 " + item->shortName;
			runProgram(program, args);
		}
	}
	
	// opening more than one session
	if (id->count() > 2) {
		if (secCmdSet && id->first().getText() == secureCrtKeyword) {
			QString args = "/T ";
			for (int i = 1; i < id->count(); i++) {
				item = &(*id)[i].getTopResult();
				args.append("/S \"" + item->fullPath + "\"");
			}
		}
	}
}

void launchy_crtPlugin::doDialog(QWidget* parent, QWidget** newDlg) 
{
	if (gui != NULL){
		return;
	}
	gui = new Gui(parent, *settings, &sessionManager);
	*newDlg = gui;
}

void launchy_crtPlugin::endDialog(bool accept) 
{
	if (accept) {
		gui->writeOptions();
		init();
	}
	if (gui != NULL) {
		delete gui;
	}
	gui = NULL;
}

QList<CatItem> launchy_crtPlugin::buildCatalog()
{
	QList<CatItem> sessionList;
	QStringList sessionNames = sessionManager.getSessions();
	foreach (QString session, sessionNames) {
		// if session contains a filepath, remove the filepath from the name
		QString sessionName;
		if (session.contains("/")) {
			QStringList filePath = session.split("/");
			sessionName = filePath[filePath.count()-1];
		} else {
			sessionName = session;
		}
		sessionList.push_back(CatItem(session, sessionName, HASH_secureCRT, getIcon()));
	}
	return sessionList;
}

int launchy_crtPlugin::msg(int msgId, void* wParam, void* lParam)
{
	bool handled = false;
	switch (msgId)
	{		
		case MSG_INIT:
			init();
			handled = true;
			break;
		case MSG_GET_LABELS:
			getLabels((QList<InputData>*) wParam);
			handled = true;
			break;
		case MSG_GET_ID:
			getID((uint*) wParam);
			handled = true;
			break;
		case MSG_GET_NAME:
			getName((QString*) wParam);
			handled = true;
			break;
		case MSG_GET_RESULTS:
			getResults((QList<InputData>*) wParam, (QList<CatItem>*) lParam);
			handled = true;
			break;
		case MSG_GET_CATALOG:
			getCatalog((QList<CatItem>*) wParam);
			handled = true;
			break;
		case MSG_LAUNCH_ITEM:
			launchItem((QList<InputData>*) wParam, (CatItem*) lParam);
			handled = true;
			break;
		case MSG_HAS_DIALOG:
			// Set to true if you provide a gui
			handled = true;
			break;
		case MSG_DO_DIALOG:
			// This isn't called unless you return true to MSG_HAS_DIALOG
			doDialog((QWidget*) wParam, (QWidget**) lParam);
			break;
		case MSG_END_DIALOG:
			// This isn't called unless you return true to MSG_HAS_DIALOG
			endDialog((bool) wParam);
			break;

		default:
			break;
	}
	return handled;
}


Q_EXPORT_PLUGIN2(myplugin, launchy_crtPlugin) 

