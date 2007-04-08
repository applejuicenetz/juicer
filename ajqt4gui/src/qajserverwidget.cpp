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
#include "qajserverwidget.h"

#include "./xpm/connect_small.xpm"
#include "./xpm/cancel_small.xpm"
#include "./xpm/insert_small.xpm"
#include "./xpm/connected_small.xpm"

QAjServerWidget::QAjServerWidget( QWidget *parent, const char *name) : QAjListWidget( ID_SERVER_INDEX, parent, name )
{
	connectedWithId = "";
	setColumnCount( NUM_SERVER_COL );
	QStringList headers;
	int i;
	for( i=0; i<NUM_SERVER_COL; i++)
	{
		switch(i)
		{
			case ID_SERVER_INDEX:
				headers.append( tr("id") );
				break;
			case NAME_SERVER_INDEX:
				headers.append( tr("name") );
				break;
			case HOST_SERVER_INDEX:
				headers.append( tr("host") );
				break;
			case PORT_SERVER_INDEX:
				headers.append( tr("port") );
				break;
			case LASTSEEN_SERVER_INDEX:
				headers.append( tr("last seen") );
				break;
			case TESTS_SERVER_INDEX:
				headers.append( tr("tests") );
				break;
		}
	}
	setHeaderLabels( headers );
	
	//setColumnWidthMode(ID_SERVER_INDEX, Q3ListView::Manual );
	setColumnHidden( ID_SERVER_INDEX, true );
	
	/*setColumnAlignment( PORT_SERVER_INDEX, Qt::AlignRight );
	setColumnAlignment( LASTSEEN_SERVER_INDEX, Qt::AlignRight );
	setColumnAlignment( TESTS_SERVER_INDEX, Qt::AlignRight );
	*/

   
	connectId = popup->addAction( QIcon(QPixmap(connect_small_xpm)), "connect", this, SLOT(connectSlot()) );
	removeId =popup->addAction( QIcon(QPixmap(cancel_small_xpm)), "remove", this, SLOT(removeSlot()) );
	popup->addAction( QIcon(QPixmap(insert_small_xpm)), "find new", this, SLOT(findSlot()) );
	removeId->setEnabled( false );
	connectId->setEnabled( false );
    QObject::connect( this, SIGNAL( newSelection( bool ) ) , this, SLOT( selectionChanged1( bool ) ) );
}


QAjServerWidget::~QAjServerWidget()
{
}

void QAjServerWidget::insertServer( QString id, QString name, QString host, QString port, QString lastseen, QString tests )
{
	QTreeWidgetItem *item = findServer( id );
	if( item == NULL )
	{
		lastseen = lastseen.left(lastseen.length()-3);
		time_t time = lastseen.toLong();
		struct tm *time2;
		time2 = localtime( &time );
		//QString timeString(asctime(time2));
		QString timeString = "";
		timeString += QConvert::num( time2->tm_mday ) + ". ";
		timeString += QConvert::num( time2->tm_mon + 1 ) + ". ";
		timeString += QConvert::num( time2->tm_year + 1900 );
		timeString += " ";
		timeString += QConvert::num( time2->tm_hour ) + ":";
		timeString += QConvert::num( time2->tm_min );
		item = new QTreeWidgetItem( this );
		servers[ id.toULong() ] = item;
		item->setText( ID_SERVER_INDEX, id );
		item->setText( NAME_SERVER_INDEX, name );
		item->setText( HOST_SERVER_INDEX, host );
		item->setText( PORT_SERVER_INDEX, port );
		item->setText( LASTSEEN_SERVER_INDEX, timeString );
		item->setText( TESTS_SERVER_INDEX, tests );
		item->setTextAlignment( TESTS_SERVER_INDEX, Qt::AlignRight );
	}
}

bool QAjServerWidget::remove( QString id )
{
	QTreeWidgetItem *item = this->removeServer( id );
	if( item != NULL )
	{
		delete item;
		return true;
	}
	return false;
}

void QAjServerWidget::removeSlot()
{
	remove();
}
void QAjServerWidget::connectSlot()
{
	connect();
}
void QAjServerWidget::findSlot()
{
	find();
}

void QAjServerWidget::connectedWith( QString id )
{
	QTreeWidgetItem *item;
	if( connectedWithId != "" )
	{
		item = servers[ connectedWithId.toULong() ];// findItem( connectedWithId, ID_SERVER_INDEX );
		if( item !=  NULL )
		{
			item->setIcon( NAME_SERVER_INDEX, QIcon() );
		}
	}
	item = servers[ id.toULong() ];
	if( item !=  NULL )
	{
		item->setIcon( NAME_SERVER_INDEX, QIcon(QPixmap(connected_small_xpm)) );
		connected( item->text( NAME_SERVER_INDEX ) );
	}
	connectedWithId = id;
}

void QAjServerWidget::selectionChanged1( bool oneSelected )
{
	removeId->setEnabled( oneSelected );
	connectId->setEnabled( oneSelected );
}

QTreeWidgetItem* QAjServerWidget::findServer( QString id )
{
	serversIt = servers.find( id.toULong() );
	if( serversIt != servers.end() )
		return serversIt->second;
	else
		return NULL;
}

QTreeWidgetItem* QAjServerWidget::removeServer( QString id )
{
	serversIt = servers.find( id.toULong() );
	if( serversIt != servers.end() )
	{
		servers.erase( serversIt );
		return serversIt->second;
	}
	else
	{
		return NULL;
	}
}

