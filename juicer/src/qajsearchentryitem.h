/***************************************************************************
 *   Copyright (C) 2007 by Matthias Reif   *
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
#ifndef QAJSEARCHENTRYITEM_H
#define QAJSEARCHENTRYITEM_H

#include "qajitem.h"

class QAjSearchItem;

/**
	@author Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>
*/
class QAjSearchEntryItem : public QAjItem
{
public:
    QAjSearchEntryItem( QString id, QAjSearchItem* search, QString checksum, QString size, QTreeWidgetItem* parent );

    ~QAjSearchEntryItem();

//     QString checksum;
    QAjSearchItem* search;

    virtual bool operator<( const QTreeWidgetItem & other ) const;

    void setFilename( QString filename );
};

#endif
