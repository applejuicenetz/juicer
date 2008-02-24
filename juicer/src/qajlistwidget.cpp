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
#include "qajlistwidget.h"

QAjListWidget::QAjListWidget( QXMLModule* xml, QWidget *parent ) : QTreeWidget( parent )
{
    this->xml = xml;
    setContextMenuPolicy( Qt::CustomContextMenu );
    setAlternatingRowColors( QAjOptionsDialog::getSetting("altRows", true).toBool() );

    popup = new QMenu( this );
    toolBar = NULL;

    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setRootIsDecorated( true );
    setSortingEnabled( true );
    header()->setStretchLastSection( false );

    connect( this, SIGNAL( itemSelectionChanged() ) , this, SLOT( selectionChanged() ) );
    connect( this, SIGNAL( customContextMenuRequested( const QPoint & ) ), this, SLOT( contextMenuRequested( const QPoint & ) ) );
}

QAjListWidget::~QAjListWidget()
{}

void QAjListWidget::selectionChanged()
{
    newSelection( isOneSelected() );
}

void QAjListWidget::contextMenuRequested( const QPoint &pos )
{
    QAjItem* item = (QAjItem*)this->itemAt( pos );
    popupItem = item;
    popup->exec( QCursor::pos() );
}

bool QAjListWidget::isOneSelected()
{
    return  !selectedItems().empty();
}

/*!
    \fn QAjListWidget::adjustSizeOfColumns()
 */
void QAjListWidget::adjustSizeOfColumns()
{
    int i;
    for ( i=0; i<this->columnCount(); i++ )
        resizeColumnToContents( i );
}


/*!
    \fn QAjListWidget::selectedAjItems () const
 */
QList<QAjItem *> QAjListWidget::selectedAjItems () const
{
    QList<QAjItem*> ajItems;
    QList<QTreeWidgetItem*> items = QTreeWidget::selectedItems();
    while(!items.isEmpty()) {
        ajItems << (QAjItem*)items.takeFirst();
    }
    return ajItems;
}


/*!
    \fn QAjListWidget::setActive( bool active )
 */
void QAjListWidget::setActive( bool active )
{
    popup->setEnabled( active );
    if( toolBar != NULL )
        toolBar->setVisible( active );
}

void QAjListWidget::sortItemsInitially(QString settingsGroup)
{
    QSettings lokalSettings;
    lokalSettings.beginGroup(settingsGroup);
    int column = lokalSettings.value("sortColumn", 0 ).toInt(NULL);
    int order = lokalSettings.value("sortOrder", Qt::AscendingOrder ).toInt(NULL);
    this->sortItems(column, (Qt::SortOrder)order );
    lokalSettings.endGroup();
}

void QAjListWidget::saveSortOrder(QString settingsGroup)
{
    QSettings lokalSettings;
    lokalSettings.beginGroup(settingsGroup);
    lokalSettings.setValue( "sortColumn", sortColumn() );
    lokalSettings.setValue( "sortOrder", header()->sortIndicatorOrder() );
    lokalSettings.endGroup();
}
