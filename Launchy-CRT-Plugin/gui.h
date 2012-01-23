/*
Launchy: Application Launcher
Copyright (C) 2007-2010  Josh Karlin

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

#ifndef GUI_H
#define GUI_H

#include "globals.h"
#include <QSettings>
#include <QtGui>
#include "SessionManager.h"

class Gui : public QWidget
{
	Q_OBJECT

public:
	Gui(QWidget* parent, QSettings* settings, SessionManager* sessionManager);
	~Gui();
	void writeOptions();

private:
	QSettings *settings;
	SessionManager* sessionManager;
	QCheckBox *telnetSelect;
	QCheckBox *sshSelect;
	QCheckBox *secureCrtSelect;
	QCheckBox *allowSessionIndexingSelect;
	QLineEdit *sessionsFolderLocationText;
	QPushButton *openFileDialog;
	QLineEdit *telnetKeywordEdit;
	QLineEdit *sshKeywordEdit;
	QLineEdit *secureCrtKeywordEdit;

public slots:
	void clickedFileDialog()
	{
		QString filePath = QFileDialog::getExistingDirectory(this, 
															 tr("Directory Path"),
															 "C:", 
															 QFileDialog::ShowDirsOnly);
		if (!filePath.isEmpty())
			sessionsFolderLocationText->setText(filePath);
	}
};


#endif 