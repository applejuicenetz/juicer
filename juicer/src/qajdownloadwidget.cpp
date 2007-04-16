/***************************************************************************
 *   Copyright (C) 2004 by Matthias Reif                                   *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
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
#include "qajdownloadwidget.h"

QAjDownloadWidget::QAjDownloadWidget( QXMLModule* xml, QWidget *parent ) : QAjListWidget( xml, parent )
{
    userStatusDescr["1"] = QObject::tr("unasked ");
    userStatusDescr["2"] = QObject::tr("try to connect ");
    userStatusDescr["3"] = QObject::tr("partner have to old vers. ");
    userStatusDescr["4"] = QObject::tr("partner can't	 open file ");
    userStatusDescr["5"] = QObject::tr("queueing ");
    userStatusDescr["6"] = QObject::tr("no usable parts ");
    userStatusDescr["7"] = QObject::tr("loading ");
    userStatusDescr["8"] = QObject::tr("not enough disk space ");
    userStatusDescr["9"] = QObject::tr("finished ");
    userStatusDescr["11"] = QObject::tr("no connection possible ");
    userStatusDescr["12"] = QObject::tr("try indirect ");
    userStatusDescr["13"] = QObject::tr("paused ");
    userStatusDescr["14"] = QObject::tr("queue full ");
    userStatusDescr["15"] = QObject::tr("own limit reached ");
    userStatusDescr["16"] = QObject::tr("indirect conn. rejected ");
    downloadStatusDescr["-1"] = QObject::tr("loading ");
    downloadStatusDescr["0"] = QObject::tr("searching ");
    downloadStatusDescr["1"] = QObject::tr("error at creating ");
    downloadStatusDescr["12"] = QObject::tr("finishing ");
    downloadStatusDescr["13"] = QObject::tr("error at finishing ");
    downloadStatusDescr["14"] = QObject::tr("finished ");
    downloadStatusDescr["15"] = QObject::tr("canceling ");
    downloadStatusDescr["16"] = QObject::tr("creating .dat ");
    downloadStatusDescr["17"] = QObject::tr("canceled ");
    downloadStatusDescr["18"] = QObject::tr("paused ");

    linuxIcon = new QIcon(":/small/linux.png");
    windowsIcon = new QIcon(":/small/windows.png");
    macIcon = new QIcon(":/small/mac.png");
    solarisIcon = new QIcon(":/small/solaris.png");
    freeBsdIcon = new QIcon(":/small/freebsd.png");
    netwareIcon = new QIcon(":/small/netware.png");
    otherOsIcon = new QIcon();

    currIdRoundRobin = -1;
    QStringList headers;
    int i;
    for ( i=0; i<NUM_DOWN_COL; i++)
    {
        switch (i)
        {
        case FILENAME_DOWN_INDEX:
            headers.append( tr("filename") );
            break;
        case SPEED_DOWN_INDEX:
            headers.append( tr("speed") );
            break;
        case STATUS_DOWN_INDEX:
            headers.append( tr("status") );
            break;
        case SOURCES_DOWN_INDEX:
            headers.append( tr("sources") );
            break;
        case FINISHED_DOWN_INDEX:
            headers.append( tr("finished") );
            break;
        case SIZE_DOWN_INDEX:
            headers.append( tr("size") );
            break;
        case REMAIN_SIZE_DOWN_INDEX:
            headers.append( tr("remaining") );
            break;
        case FINISHED_SIZE_DOWN_INDEX:
            headers.append( tr("finished") );
            break;
        case REMAIN_TIME_DOWN_INDEX:
            headers.append( tr("eta") );
            break;
        case POWER_DOWN_INDEX:
            headers.append( tr("power") );
            break;
        case MISSING_DOWN_INDEX:
            headers.append( tr("not seen") );
            break;
        }
    }
    setHeaderLabels( headers );

    popup->setTitle( tr("&Download") );
    pausePopup = popup->addAction( QIcon(":/small/pause.png"), "pause", this, SLOT(pauseSlot()) );
    resumePopup = popup->addAction( QIcon(":/small/resume.png"), "resume", this, SLOT(resumeSlot()) );
    cancelPopup = popup->addAction( QIcon(":/small/cancel.png"), "cancel", this, SLOT(cancelSlot()) );
    partListPopup = popup->addAction( QIcon(":/small/partlist.png"), "part list", this, SLOT(partListSlot()) );
    renamePopup = popup->addAction( QIcon(":/small/rename.png"), "rename", this, SLOT(renameSlot()) );
    renamePlusPopup = popup->addAction( QIcon(":/small/rename_plus.png"), "rename by clipboard", this, SLOT(renamePlusSlot()) );
    openPopup = popup->addAction( QIcon(":/small/exec.png"), "open file", this, SLOT(openSlot()) );
    popup->addSeparator();
    popup->addAction( QIcon(":/small/filter.png"), "remove finished/canceld", this, SLOT(cleanSlot()) );
    pausePopup->setEnabled( false );
    resumePopup->setEnabled( false );
    cancelPopup->setEnabled( false );
    partListPopup->setEnabled( false );
    renamePopup->setEnabled( false );
    renamePlusPopup->setEnabled( false );
    openPopup->setEnabled( false );
    QObject::connect( this, SIGNAL( newSelection( bool ) ) , this, SLOT( selectionChanged1( bool ) ) );

    setIconSize( QSize( 100, 20 ) );

    initToolBar();
}

QAjDownloadWidget::~QAjDownloadWidget()
{
    delete linuxIcon;
    delete windowsIcon;
    delete macIcon;
    delete otherOsIcon;
    delete netwareIcon;
    delete solarisIcon;
    delete freeBsdIcon;
}

/*!
    \fn QAjDownloadWidget::initToolBar()
 */
void QAjDownloadWidget::initToolBar()
{
    toolBar = new QToolBar( "download operations", this );

    pauseDownloadButton = toolBar->addAction( QIcon(":/pause.png"), "pause download", this, SLOT( pauseSlot() ) );
    pauseDownloadButton->setToolTip( "pause download" );

    resumeDownloadButton = toolBar->addAction( QIcon(":/resume.png"), "resume download", this, SLOT( resumeSlot() ) );
    resumeDownloadButton->setToolTip( "resume download" );

    cancelDownloadButton = toolBar->addAction( QIcon(":/cancel.png"), "cancel download", this, SLOT( cancelSlot() ) );

    partListButton = toolBar->addAction( QIcon(":/partlist.png"), "show part list", this, SLOT( partListSlot() ) );
    renameDownloadButton = toolBar->addAction( QIcon(":/rename.png"), "rename download", this, SLOT( renameSlot() ) );
    renamePlusDownloadButton = toolBar->addAction( QIcon(":/rename_plus.png"), "rename download by clipboard", this, SLOT( renamePlusSlot() ) );

    openDownloadButton = toolBar->addAction( QIcon(":/exec.png"), "open download", this, SLOT( openSlot() ) );

    clearDownloadButton = toolBar->addAction( QIcon(":/filter.png"), "remove finished/canceld download", this, SLOT( cleanSlot() ) );

    pauseDownloadButton->setDisabled( true );
    resumeDownloadButton->setDisabled( true );
    cancelDownloadButton->setDisabled( true );
    partListButton->setDisabled( true );
    renameDownloadButton->setDisabled( true );
    renamePlusDownloadButton->setDisabled( true );
    openDownloadButton->setDisabled( true );

    toolBar->addSeparator();

    powerCheck = new QCheckBox( toolBar );
    powerCheck->setText( tr("Power Download:") );
    powerCheck->setChecked( false );
    powerCheck->adjustSize();
    toolBar->addWidget( powerCheck );

    powerSpin = new QDoubleSpinBox( toolBar );
    powerSpin->setRange( 2.2, 50.0 );
    powerSpin->setSingleStep( 0.1 );
    powerSpin->setDecimals( 1 );
    toolBar->addWidget( powerSpin );

    //connect( powerEdit, SIGNAL( returnPressed() ), this, SLOT( applyPowerDownload() ) );
    connect( powerSpin, SIGNAL( valueChanged( const QString&) ), this, SLOT( applyPowerDownload() ) );
    connect( powerSpin, SIGNAL( valueChanged( double ) ), this, SLOT( applyPowerDownload() ) );

    powerOkButton = toolBar->addAction( QIcon(":/ok.png"), "apply power download", this, SLOT( applyPowerDownload() ) );

    powerMaxButton = toolBar->addAction( QIcon(":/launch.png"), "set all downloads to 1:50", this, SLOT( maxPowerDownload() )  );

    #ifdef AJQTGUI_MODE_SPECIAL
        bool special = true;
    #else
        char* mode = getenv( "AJQTGUI_MODE" );
        bool special = (( mode != NULL )) && ( strcmp(mode, "SPECIAL") == 0 );
    #endif

    powerMaxButton->setVisible( special );
}


void QAjDownloadWidget::insertDownload(QString id, QString fileName, QString status, QString size, QString ready, QString power, QString tempNumber)
{
    QAjDownloadItem *downloadItem = findDownload( id );
    if ( downloadItem == NULL )
    {
        downloadItem = new QAjDownloadItem( id, this );
        downloads[ id ] = downloadItem;
    }
    downloadItem->update( fileName, status, size, ready, power, tempNumber );
}


void QAjDownloadWidget::insertUser(QString downloadId, QString id, QString fileName, QString speed, QString status, QString power, QString queuePos, QString os)
{
    QAjDownloadItem *downloadItem = findDownload( downloadId );
    if ( downloadItem == NULL )
    {
        downloadItem = new QAjDownloadItem( downloadId, this );
        downloads[ downloadId ] = downloadItem;
        downloadItem->setText( FILENAME_DOWN_INDEX, fileName );
    }
    QIcon *osIcon;
    if ( os == LINUX )
        osIcon = linuxIcon;
    else if ( os == WINDOWS )
        osIcon = windowsIcon;
    else if ( os == MAC )
        osIcon = macIcon;
    else if ( os == SOLARIS )
        osIcon = solarisIcon;
    else if ( os == FREEBSD )
        osIcon = freeBsdIcon;
    else if ( os == NETWARE )
        osIcon = netwareIcon;
    else
        osIcon = otherOsIcon;
    downloadItem->updateUser( id, fileName, speed, status, power, queuePos, userStatusDescr[status], osIcon );
}


bool QAjDownloadWidget::remove( QString id )
{
    if( removeDownload( id ) )
    {
        return true;
    }
    else
    {
        DownloadUser du = findParent( id );
        if( du.user != NULL )
        {
            du.download->decSources( du.user->getStatus() );
            du.download->removeUser( id );
            return true;
        }
    }
    return false;
}

void QAjDownloadWidget::cancelSlot()
{
    if ( QMessageBox::question( this, "Confirm", "Do you realy want to cancel this download(s)?", QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
    {
        processSelected( "canceldownload" );
    }
}

void QAjDownloadWidget::cleanSlot()
{
    xml->set( "cleandownloadlist" );
}

void QAjDownloadWidget::resumeSlot()
{
    processSelected( "resumedownload" );
}

void QAjDownloadWidget::pauseSlot()
{
    processSelected( "pausedownload" );
}

void QAjDownloadWidget::partListSlot()
{
    requestSelected( "downloadpartlist" );
}

void QAjDownloadWidget::renameSlot()
{
    QString oldFilename;
    QString newFilename;
    bool ok;
    QList<QAjItem *>  selectedItems = selectedAjItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        oldFilename = selectedItems[i]->text( FILENAME_DOWN_INDEX );
        newFilename = QInputDialog::getText( this, "rename download", "enter new filename for " + oldFilename, QLineEdit::Normal, oldFilename, &ok );
        newFilename = QString( QUrl::toPercentEncoding( newFilename ) );
        if ( ok && !newFilename.isEmpty() )
        {
            xml->set( "renamedownload", "&id=" + selectedItems[i]->getId() + "&name=" + newFilename );
        }
    }
}

void QAjDownloadWidget::renamePlusSlot()
{
    QString oldFilename;
    QString newFilename;
    QString newFilenameBase = qApp->clipboard()->text( QClipboard::Clipboard );
    QList<QAjItem *>  selectedItems = selectedAjItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        oldFilename = selectedItems[i]->text( FILENAME_DOWN_INDEX );
        newFilename = newFilenameBase;
        if (selectedItems.size() > 1)
        {
            newFilename += "_" + QString::number(i+1);
        }
        QStringList s = oldFilename.split(".");
        if (s.size() > 1)
        {
            newFilename += "." + s[s.size() - 1];
        }
        newFilename = QString( QUrl::toPercentEncoding( newFilename ) );
        if (!newFilename.isEmpty())
        {
            xml->set( "renamedownload", "&id=" + selectedItems[i]->getId() + "&name=" + newFilename );
        }
    }
}

void QAjDownloadWidget::openSlot()
{
    QSettings lokalSettings;
    QString launcher = lokalSettings.value( "launcher", DEFAULT_LAUNCHER ).toString().simplified();
    QStringList args = launcher.split(" ");

    QString exec = args.takeFirst();
    if ( launcher == KDE_LAUNCHER )
    {
        args.removeFirst();
        args.push_front("exec");
    }
    else if ( launcher == GNOME_LAUNCHER )
    {
        args.removeFirst();
    }
    else if ( launcher == MAC_LAUNCHER )
    {
        exec = "open";
        args.clear();
    }
    else if ( launcher == WIN_LAUNCHER )
    {
        exec = "start";
        args.clear();
        args.push_back("\"\"");
    }

    QString iDir, tDir;
    // determine the path
    QString location = lokalSettings.value( "location", "same" ).toString();
    if( location == "specific" )
    {
        iDir = lokalSettings.value( "incomingDirSpecific", "/" ).toString() + QDir::separator();
        tDir = lokalSettings.value( "tempDirSpecific", "/" ).toString() + QDir::separator();
    }
    else if( location == "same" )
    {
        iDir = incomingDir.absolutePath() + QDir::separator();
        tDir = tempDir.absolutePath() + QDir::separator();
    }
    else // ftp
    {
        // TODO
        return;
    }

    QList<QAjItem*> items = selectedAjItems();
    int i;
    for (i=0; i<items.size(); i++)
    {
        QAjDownloadItem* ajDownloadItem = (QAjDownloadItem*)items[i];
        if( ajDownloadItem->getStatus() == DOWN_FINISHED )
        {
            args.push_back( iDir + ajDownloadItem->text( FILENAME_DOWN_INDEX ) );
        }
        else
        {
            args.push_back( tDir + ajDownloadItem->getTempNumber() + ".data" );
        }
        QProcess::startDetached( exec, args );
        args.pop_back();
    }
}

void QAjDownloadWidget::selectionChanged1(  bool oneSelected  )
{
    pausePopup->setEnabled( oneSelected );
    resumePopup->setEnabled( oneSelected );
    cancelPopup->setEnabled( oneSelected );
    partListPopup->setEnabled( oneSelected );
    renamePopup->setEnabled( oneSelected );
    renamePlusPopup->setEnabled( oneSelected );
    openPopup->setEnabled( oneSelected );

    bool onePaused = false;
    bool oneActive = false;
    bool oneFinished = false;

    QList<QAjItem *>  selectedItems = selectedAjItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        QAjItem* downloadItem = selectedItems[i];
        if ( downloadItem->getStatus() == DOWN_PAUSED )
            onePaused = true;
        if ( ( downloadItem->getStatus() == DOWN_SEARCHING ) || ( downloadItem->getStatus() == DOWN_LOADING ) )
            oneActive = true;
        if ( downloadItem->getStatus() == DOWN_FINISHED )
            oneFinished = true;
        if ( onePaused && oneActive && oneFinished )
            break;
    }
    cancelDownloadButton->setEnabled( oneSelected );
    partListButton->setEnabled( oneSelected );
    renameDownloadButton->setEnabled( oneSelected );
    renamePlusDownloadButton->setEnabled( oneSelected );
    resumeDownloadButton->setEnabled( onePaused );
    pauseDownloadButton->setEnabled( oneActive );
    openDownloadButton->setEnabled( oneSelected );
}

void QAjDownloadWidget::updateView( bool force )
{
    if( force || this->isVisible() )
    {
        int i;
        for (i=0; i<topLevelItemCount(); i++)
        {
            ((QAjDownloadItem*)topLevelItem(i))->updateView( &downloadStatusDescr );
        }
    }
}

QAjDownloadItem* QAjDownloadWidget::findDownload( QString id )
{
    if (downloads.contains( id ))
        return downloads[id];
    else
        return NULL;
}

bool QAjDownloadWidget::removeDownload( QString id )
{
    if( downloads.contains( id ) )
    {
        // first remove it form the hashtable, than delete it
        QAjDownloadItem* item = downloads[ id ];
        downloads.remove( id );
        delete item;
        return true;
    }
    return false;
}


DownloadUser QAjDownloadWidget::findParent( QString id )
{
    DownloadUser du;
    du.download = NULL;
    du.user = NULL;
    int i;
    for (i=0; i<topLevelItemCount() && du.user == NULL; i++)
    {
        du.download = (QAjDownloadItem*)topLevelItem(i);
        du.user = du.download->findUser( id );
    }
    return du;
}


/*!
    \fn QAjDownloadWidget::getNextIdRoundRobin()
 */
QString QAjDownloadWidget::getNextIdRoundRobin()
{
    if (topLevelItemCount() < 1)
        return "";
    currIdRoundRobin ++;
    currIdRoundRobin %= topLevelItemCount();
    return ((QAjDownloadItem*)topLevelItem( currIdRoundRobin ))->getId();
}


/*!
    \fn QAjDownloadWidget::findDownloadByTempNum( QFileInfo tempFile )
 */
QString QAjDownloadWidget::findDownloadByTempNum( QFileInfo tempFile )
{
    if (tempDir.absolutePath() == tempFile.absolutePath())
    {
        QString tempNum = tempFile.baseName();
        QAjDownloadItem* item;
        int i;
        for ( i=0; i<topLevelItemCount(); i++ )
        {
            item = (QAjDownloadItem*)topLevelItem(i);
            if ( item->getTempNumber() == tempNum )
                return item->text( FILENAME_DOWN_INDEX );
        }
    }
    return tempFile.fileName();
}

void QAjDownloadWidget::storeDownloadFtp()
{
    FTP* ftp = NULL;
    QString filename, localDir;
    QList<QTreeWidgetItem *>  selectedItems = this->selectedItems();

    QSettings lokalSettings;
    lokalSettings.beginGroup("ftp");
    QString dir = lokalSettings.value( "dir", "/" ).toString();
    lokalSettings.endGroup();

    if ( ! dir.endsWith( '/' ) )
    {
        dir += '/';
    }
    ftp = new FTP( this );

    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        filename = selectedItems.at(i)->text( FILENAME_DOWN_INDEX );
        localDir = QFileDialog::getExistingDirectory( this, "save \"" + filename + "\" + to" );
        if ( localDir != "" )
        {
            if ( ! localDir.endsWith( QDir::separator() ) )
            {
                localDir += QDir::separator();
            }
            QFile* dstFile = new QFile( localDir + filename );
            if ( ! dstFile->exists() )
            {
                dstFile->open( QIODevice::WriteOnly );
                ftp->add( dir + filename, dstFile );
            }
        }
    }
    ftp->start();
}

void QAjDownloadWidget::processSelected( QString request, QString para )
{
    QList<QAjItem *>  selectedItems = selectedAjItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        xml->set( request, para + "&id=" + selectedItems[i]->getId() );
    }
}

void QAjDownloadWidget::requestSelected( QString request, QString para )
{
    QList<QAjItem *>  selectedItems = selectedAjItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        xml->get( request, para + "&id=" + selectedItems[i]->getId() );
    }
}

void QAjDownloadWidget::applyPowerDownload()
{
    float value;
    if ( powerCheck->isChecked() )
        value = powerSpin->value();
    else
        value = 1.0;
    processSelected( "setpowerdownload", "&Powerdownload=" + QConvert::power( value ) );
}


void QAjDownloadWidget::maxPowerDownload()
{
    QList<QString> ids = downloads.keys();
    int i;
    for ( i=0; i<ids.size(); i++ )
    {
        xml->set( "setpowerdownload", "&Powerdownload="+QConvert::power( 50 )+"&id="+ids[i] );
    }
}



/*!
    \fn QAjDownloadWidget::setDirs( QFileInfo tmpDir, QFileInfo inDir )
 */
void QAjDownloadWidget::setDirs( QFileInfo tmpDir, QFileInfo inDir )
{
    this->tempDir = tmpDir;
    this->incomingDir = inDir;
}