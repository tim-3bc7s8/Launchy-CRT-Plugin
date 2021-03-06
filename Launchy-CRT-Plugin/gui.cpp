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


#include "gui.h"


Gui::Gui(QWidget* parent, QSettings* settings, SessionManager* sessionManager) 
: QWidget(parent)
{
	if (settings == NULL)
		return;	

	this->settings = settings;
	this->sessionManager = sessionManager;

	// Group box for Commands
	QGroupBox *commandsBox = new QGroupBox(tr("Commands"), this);
	commandsBox->setGeometry(12, 5, 460, 100);	
		// Telnet Checkbox. This turns the telnet command on/off
			telnetSelect = new QCheckBox(tr("telnet - <tab> /address/ to telnet to a device."), this);
			telnetSelect->setGeometry(25, 30, 400, 20);
			telnetSelect->setChecked(settings->value("secureCRT/telnetCommand", true).toBool());
		// SSH Checkbox. Turns ssh command on/off
			sshSelect = new QCheckBox(tr("ssh - <tab> /address/ to ssh to a device."), this);
			sshSelect->setGeometry(25, 50, 400, 20);
			sshSelect->setChecked(settings->value("secureCRT/sshCommand", true).toBool());
		// SecureCRT Checkbox. Turns searchable sessions on/off
			secureCrtSelect = new QCheckBox(tr("SecureCRT - Allows searching SecureCRT sessions."), this);
			secureCrtSelect->setGeometry(25, 70, 400, 20);
			secureCrtSelect->setChecked(settings->value("secureCRT/secureCrtCommand", true).toBool());
	// Keyword Options
		// load keywords
			QString telnetKeyword = settings->value("secureCRT/telnetKeyword", "Telnet").toString();
			QString sshKeyword = settings->value("secureCRT/sshKeyword", "SSH").toString();
			QString secureCrtKeyword = settings->value("secureCRT/secureCrtKeyword", "SecureCRT").toString();
		// Alternate keyword label
			QLabel *keywordLabel = new QLabel(tr("Keywords"), this);
			keywordLabel->setGeometry(390, 10, 70, 20);
		// Textbox for telnet keyword
			telnetKeywordEdit = new QLineEdit(telnetKeyword, this);
			telnetKeywordEdit->setGeometry(370, 30, 90, 18);
		// Textbox for ssh keyword
			sshKeywordEdit = new QLineEdit(sshKeyword, this);
			sshKeywordEdit->setGeometry(370, 50, 90, 18);
		// Textbox for SecureCRT keyword
			secureCrtKeywordEdit = new QLineEdit(secureCrtKeyword, this);
			secureCrtKeywordEdit->setGeometry(370, 70, 90, 18);
	
	// Group box for Search 
	QGroupBox *searchBox = new QGroupBox(tr("Search"), this);
	searchBox->setGeometry(12, 110, 460, 100);
		// Allow indexing of sessions in main Launchy search.
			allowSessionIndexingSelect = new QCheckBox(tr("Allow SecureCRT sessions to be indexed within Launchy catalog."), this);
			allowSessionIndexingSelect->setGeometry(25, 135, 400, 20);
			allowSessionIndexingSelect->setChecked(settings->value("secureCRT/allowSessionIndexing", true).toBool());
		// Label
			QLabel *sessionsLocationLabel = new QLabel(tr("Location of SecureCRT sessions folder:"), this);
			sessionsLocationLabel->setGeometry(25, 155, 400, 20);
		// Text box with location of SecureCRT Sessions
			QString loc = sessionManager->getDefaultLocation();
			loc = settings->value("secureCRT/sessionsLocation", loc).toString();
			sessionsFolderLocationText = new QLineEdit(loc, this);
			sessionsFolderLocationText->setGeometry(45, 175, 390, 20);
			sessionsFolderLocationText->setEchoMode(QLineEdit::Normal);
		// Push button to open FileDialog
			openFileDialog = new QPushButton(tr("..."), this);
			openFileDialog->setGeometry(440, 175, 25, 20);
			QObject::connect(openFileDialog, SIGNAL(clicked()), this, SLOT(clickedFileDialog()));

	// Group box for Info
	QGroupBox *infoBox = new QGroupBox(tr("About"), this);
	infoBox->setGeometry(12, 340, 460, 50);		
		// Displays a link to homepage
			QString link = "<a href=\"https://github.com/timmy0020/Launchy-CRT-Plugin\">https://github.com/timmy0020/Launchy-CRT-Plugin</a>";
			QLabel *linkToSite = new QLabel(link, this);
			linkToSite->setOpenExternalLinks(true);
			linkToSite->setGeometry(25, 360, 400, 20);
		// Displays the version of the plugin
			QString version = "Version: ";
			version.append(settings->value("secureCRT/version").toString());
			QLabel *versionNumber = new QLabel(version, this);
			versionNumber->setGeometry(400, 360, 400, 20);
}

void Gui::writeOptions()
{
	if (settings == NULL)
		return;
	settings->setValue("secureCRT/telnetCommand", telnetSelect->isChecked());
	settings->setValue("secureCRT/sshCommand", sshSelect->isChecked());
	settings->setValue("secureCRT/secureCrtCommand", secureCrtSelect->isChecked());
	settings->setValue("secureCRT/allowSessionIndexing", allowSessionIndexingSelect->isChecked());
	if (sessionManager->setSessionPath(sessionsFolderLocationText->text()))		
		settings->setValue("secureCRT/sessionsLocation", sessionsFolderLocationText->text());	
	if (!telnetKeywordEdit->text().isEmpty())
		settings->setValue("secureCRT/telnetKeyword", telnetKeywordEdit->text());
	if (!sshKeywordEdit->text().isEmpty())
		settings->setValue("secureCRT/sshKeyword", sshKeywordEdit->text());
	if (!secureCrtKeywordEdit->text().isEmpty())
		settings->setValue("secureCRT/secureCrtKeyword", secureCrtKeywordEdit->text());
}

Gui::~Gui()
{
	this->hide();
}