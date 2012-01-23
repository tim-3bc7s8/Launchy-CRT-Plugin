#include <QtGui>
#include <QUrl>
#include <QFile>
#include <QRegExp>
#include <QTextCodec>

#include "SessionManager.h"

// Get the session files from the directory
QStringList SessionManager::getSessions() 
{
	QDir dir(sessionFolder);
	QStringList sessions;	
	QString relativePath;
	QDirIterator sessionsDirectory(sessionFolder, 
								   QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
								   QDirIterator::Subdirectories);

	// recursively get all .ini files in the sessionsDirectory
	while(sessionsDirectory.hasNext()) {
		sessionsDirectory.next();
		if (sessionsDirectory.fileInfo().completeSuffix().contains("ini",Qt::CaseInsensitive)) {
			// get the relative filepath						
			relativePath = dir.relativeFilePath(sessionsDirectory.fileInfo().filePath());
			// remove the .ini at the end of the session file
			relativePath = relativePath.replace(".ini", "");
			// add the session to the list of sessions
			if (!relativePath.contains("FolderData")) {
				sessions.append(relativePath);
			}
		}
	}
	return sessions;
}

// Returns the session folders path
QString SessionManager::getSessionPath()
{
	return sessionFolder;
}

// Returns the default sessions location 
QString SessionManager::getDefaultLocation()
{
	return defaultLocation;
}

// Sets the session folder path. returns true if it was successfully
// changed. Otherwise it returns false.
bool SessionManager::setSessionPath(QString selectedLocation)
{
	if (!QDir(selectedLocation).exists()) 
		return false;

	sessionFolder = selectedLocation;
	return true;
}

QList<CatItem> SessionManager::getSessionList()
{
	return this->sessionList;
}


void SessionManager::setSessionList(QList<CatItem> sessions)
{
	this->sessionList = sessions;
}
