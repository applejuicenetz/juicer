/***************************************************************************
 *   Copyright (C) 2005 by Matthias Reif   *
 *   matthias.reif@informatik.tu-chemnitz.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "qajoptionsdialog.h"

QAjOptionsDialog::QAjOptionsDialog( QWidget* parent ) : QDialog( parent )
{
	setupUi( this );
	connect( incomingButton, SIGNAL( clicked() ), this, SLOT( selectIncomingDir() ) );
	connect( tempButton, SIGNAL( clicked() ), this, SLOT( selectTempDir() ) );

    winLauncher = "Windows default";
    macLauncher = "MacOS default";
    kdeLauncher = "kfmclient (KDE)";
    gnomeLauncher = "gnome-open (Gnome)";

    #ifdef Q_WS_WIN
        launchCombo->addItem( winLauncher );
    #else
        #ifdef Q_WS_MAC
            launchCombo->addItem( macLauncher );
        #else
            launchCombo->addItem( kdeLauncher );
            launchCombo->addItem( gnomeLauncher );
        #endif
    #endif
}

QAjOptionsDialog::~QAjOptionsDialog()
{
}

AjSettings QAjOptionsDialog::getAjSettings()
{
	AjSettings settings;
	settings.nick = nickEdit->text();
	settings.password = passwordEdit->text();
	settings.coreAddress = coreEdit->text();
	settings.xmlPort = xmlEdit->text();
	settings.incomingDir = incomingEdit->text();
	settings.tempDir = tempEdit->text();
	
	settings.serverURL = serverEdit->text();
	
	settings.refresh = refreshSpin->value();
	
	settings.autoconnect = autoConnect->isChecked()?"true":"false";
	settings.savePassword = savePassword->isChecked()?"true":"false";

	settings.maxDown = QString::number( downSpin->value() );
	settings.maxUp = QString::number( upSpin->value() );
	settings.maxSlot = QString::number( slotSpin->value() );
	settings.maxSources = QString::number( sourcesSpin->value() );
	settings.maxCon = QString::number( connectionsSpin->value() );
	settings.maxNewCon = QString::number( newSpin->value() );

	settings.tcpPort = tcpEdit->text();

   settings.launcher = launchCombo->currentText();

	settings.ftpServer = ftpServerEdit->text();
	settings.ftpPort = ftpPortEdit->text();
	settings.ftpUser = ftpUserEdit->text();
	settings.ftpPassword = ftpPasswordEdit->text();
	settings.ftpDir = ftpDirEdit->text();

	return settings;
}

void QAjOptionsDialog::setAjSettings( AjSettings settings )
{
	nickEdit->setText( settings.nick );
	passwordEdit->setText( settings.password );
	coreEdit->setText( settings.coreAddress );
	xmlEdit->setText( settings.xmlPort );
	incomingEdit->setText( settings.incomingDir );
	tempEdit->setText( settings.tempDir );
	serverEdit->setText( settings.serverURL );

	refreshSpin->setValue( settings.refresh );

	autoConnect->setChecked( ( settings.autoconnect == "true" ) );
	savePassword->setChecked( ( settings.savePassword == "true" ) );

	downSpin->setValue( settings.maxDown.toInt() / 1024 );
	upSpin->setValue( settings.maxUp.toInt() / 1024 );
	slotSpin->setValue( settings.maxSlot.toInt() );
	sourcesSpin->setValue( settings.maxSources.toInt() );
	connectionsSpin->setValue( settings.maxCon.toInt() );
	newSpin->setValue( settings.maxNewCon.toInt() );
	
	tcpEdit->setText( settings.tcpPort );

   launchCombo->setEditText( settings.launcher );

	ftpServerEdit->setText( settings.ftpServer );
	ftpPortEdit->setText( settings.ftpPort );
	ftpUserEdit->setText( settings.ftpUser );
	ftpPasswordEdit->setText( settings.ftpPassword );
	ftpDirEdit->setText( settings.ftpDir );
}

void QAjOptionsDialog::selectIncomingDir()
{
	QString dir = QFileDialog::getExistingDirectory( this, "Choose a directory", incomingEdit->text() );
	if( dir != "" )
	{
		incomingEdit->setText( dir );
	}
}

void QAjOptionsDialog::selectTempDir()
{
	QString dir = QFileDialog::getExistingDirectory( this, "Choose a directory", tempEdit->text() );
	if( dir != "" )
	{
		tempEdit->setText( dir );
	}
}

/*!
    \fn QAjOptionsDialog::setSpecial( bool special )
 */
void QAjOptionsDialog::setSpecial( bool special )
{
	if( ! special )
	{
		tabWidget->removeTab( 2 );
	}
}
