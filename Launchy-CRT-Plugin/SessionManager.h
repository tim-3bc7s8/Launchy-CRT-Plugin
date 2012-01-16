#pragma once

class SessionManager
{
public:
private:
	QString sessionFolder;


public:
	SessionManager(){
		// Windows 7
		//sessionFolder = QDir::homePath() + "\\AppData\\Roaming\\VanDyke\\Config\\Sessions";
		// Windows XP
		sessionFolder = QDir::homePath() + "\\Application Data\\VanDyke\\Config\\Sessions";
	}

	~SessionManager(){	}

	QStringList getSessions();

private:

};
