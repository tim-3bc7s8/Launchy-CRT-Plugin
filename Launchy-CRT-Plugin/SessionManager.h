#pragma once

#include "catalog.h"

class SessionManager
{
public:
private:
	QString sessionFolder;
	QString defaultLocation;
	QList<CatItem> sessionList;


public:
	SessionManager(){
		// Windows 7
		//sessionFolder = QDir::homePath() + "/AppData/Roaming/VanDyke/Config/Sessions";
		// Windows XP
		defaultLocation = QDir::homePath() + "/Application Data/VanDyke/Config/Sessions";
	}

	~SessionManager(){	}

	QStringList getSessions();
	QString getSessionPath();
	QString getDefaultLocation();
	bool setSessionPath(QString selectedLocation);
	QList<CatItem> getSessionList();
	void setSessionList(QList<CatItem> sessions);

private:

};
